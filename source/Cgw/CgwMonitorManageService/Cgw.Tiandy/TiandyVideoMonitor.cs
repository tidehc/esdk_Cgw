/*-----------------------------------------------------------------------
//文件名：TiandyVideoMonitor.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-2-25
//描述：用于调用天地伟业接口，解析返回数据
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using CgwMonitorManage.SmcError;
using CgwMonitorManage.Common;
using System.Threading;
using System.IO;

namespace CgwMonitorManage.Tiandy
{
    public class TiandyVideoMonitor : IVideoMonitor
    {
        /// <summary>
        /// 刷新设备成功
        /// </summary>
        bool isRefreshSucess = true;

        /// <summary>
        /// 获取设备是否结束
        /// </summary>
        bool isGetDevicesFinish = true;

        /// <summary>
        /// 刷新设备列表等待时间
        /// </summary>
        uint refreshDeviceListOverTime = 0;

        /// <summary>
        /// 设备缓存列表未被使用等待时间
        /// </summary>
        uint deviceListUnusedOverTime = 0;

        /// <summary>
        /// 日志
        /// </summary>
        private static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// http服务客户端
        /// </summary>
        private TiandyHttpClient httpClient;

        /// <summary>
        /// SDK客户端
        /// </summary>
        private TiandySdkClient sdkClient;

        /// <summary>
        /// 保存摄像头跟预览句柄的关系，key为摄像头编号，value为预览句柄，一对一的关系
        /// </summary>
        private Dictionary<string, int> cameraVideoHandeDic = new Dictionary<string, int>();

        /// <summary>
        /// 保存预览句柄跟摄像头的关系，key为预览句柄，value为摄像头编号，一对一的关系
        /// </summary>
        private Dictionary<int, string> videoHandleCameraDic = new Dictionary<int, string>();

        /// <summary>
        /// 预览句柄操作锁，用于cameraVideoHandeDic、videoHandleCameraDic的操作
        /// </summary>
        private ReaderWriterLockSlim handelOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 摄像头操作锁，用于tiandyCameraDictionary、mediaServerDictionary、hostDictionary、groupList、nodeRelationList的操作
        /// </summary>
        private ReaderWriterLockSlim cameraOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 摄像头列表，key为摄像头编号
        /// </summary>
        private Dictionary<string, TiandyCamera> tiandyCameraDictionary = new Dictionary<string, TiandyCamera>();

        /// <summary>
        /// 流媒体（中间件）服务器列表，key为流媒体服务器编号
        /// </summary>
        private Dictionary<string, MediaServer> mediaServerDictionary = new Dictionary<string, MediaServer>();

        /// <summary>
        /// 主机（编码器）列表，key为主机编号
        /// </summary>
        private Dictionary<string, Host> hostDictionary = new Dictionary<string, Host>();

        /// <summary>
        /// 自定义组列表,key 为编号
        /// </summary>
        private Dictionary<string, CameraGroup> groupDic = new Dictionary<string, CameraGroup>();

        /// <summary>
        /// 分组关系列表
        /// </summary>
        private List<NodeRelation> nodeRelationList = new List<NodeRelation>();

        /// <summary>
        /// 向上码流回调函数
        /// </summary>
        private DataCallBack dataCallBack;

        /// <summary>
        /// 码流回调者
        /// </summary>
        private string sender;

        /// <summary>
        /// 监控平台ID
        /// </summary>
        private string monitorId;

        /// <summary>
        /// mic状态dictionary操作锁
        /// </summary>
        private ReaderWriterLockSlim micOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 保存摄像头和mic状态，key为camera编号，value为mic是否开启标记
        /// </summary>
        private Dictionary<string, bool> cameraMicStatusDic = new Dictionary<string, bool>();

        //定时器器，用于定时更新摄像头
        private System.Timers.Timer updateCameraTimer = new System.Timers.Timer();

        /// <summary>
        /// 刷新设备列表操作锁
        /// </summary>
        private ReaderWriterLockSlim refreshMonitorCameraOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 定时器器，用于获取设备列表
        /// </summary>
        private System.Timers.Timer monitorManageServiceGetCameraList = new System.Timers.Timer();

        public TiandyVideoMonitor()
        {
            //初始话定时器
            this.updateCameraTimer.AutoReset = true;
            this.updateCameraTimer.Elapsed += new System.Timers.ElapsedEventHandler(GetAllCamerasTimer);
            //摄像头变化不会太快，间隔时间暂定为1小时
            this.updateCameraTimer.Interval = CgwConst.REFRESH_CAMERA_LIST_WAIT_TIME;

            //获取配置文件中刷新设备列表超时时间（s）
            if (!uint.TryParse(ConfigSettings.RefrshDeviceListOverTime, out refreshDeviceListOverTime))
            {
                //缺省为180秒钟
                refreshDeviceListOverTime = 180;
            }

            //获取配置文件中设备列表未被使用超时时间（s）
            if (!uint.TryParse(ConfigSettings.DeviceListUnusedOverTime, out deviceListUnusedOverTime))
            {
                //缺省为5秒钟
                deviceListUnusedOverTime = 5;
            }

            monitorManageServiceGetCameraList.AutoReset = true;
            monitorManageServiceGetCameraList.Elapsed += new System.Timers.ElapsedEventHandler(monitorManageServiceGetCameraList_Elapsed);
            monitorManageServiceGetCameraList.Interval = deviceListUnusedOverTime * 1000;
            monitorManageServiceGetCameraList.Start();
        }

        void monitorManageServiceGetCameraList_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            isGetDevicesFinish = true;
        }

        /// <summary>
        /// 初始化天地伟业监控平台
        /// </summary>
        /// <param name="monitorConfigElement">监控平台配置节点</param>
        /// <returns></returns>
        public SmcErr Load(XmlElement monitorConfigElement)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.Load().");
            SmcErr err = new CgwError();

            string userLevel = "";
            //解析xml节点，获取所需参数            
            try
            {
                #region Tiandy视频监控平台（Easy7）连接信息
                this.monitorId = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.ID_TAG);

                userLevel = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.PTZCONTROLLEVEL_TAG);

                string httpUser = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.PLATFORM_USER_TAG);

                string httpPassword = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.PLATFORM_PASSWORD_TAG);

                string httpUrl = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.PLATFORM_URL_TAG);

                string mediaServerId = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.MEDIASERVER_SERVERID_TAG);
                #endregion
                this.httpClient = new TiandyHttpClient(httpUrl, httpUser, httpPassword);

                //使用线程轮询加载监控平台
                Thread loaderThread = new Thread(LoaderThread);
                loaderThread.Start(mediaServerId);

            }
            catch (Exception e)
            {
                err.SetErrorNo(CgwError.MONITOR_CONFIG_FILE_INVALID);
                logEx.Error("Load Tiandy monitor failed.Execption message:{0}.", e.Message);
                return err;
            }

            this.sdkClient = new TiandySdkClient();
            //如果控制级别配置错误，只记录错误日志，不向外报错
            try
            {
                this.sdkClient.PtzControlLevel = int.Parse(userLevel);
            }
            catch (Exception e)
            {
                logEx.Error("Ptz Control Level is invalid.Execption message:{0}.", e.Message);
                this.sdkClient.PtzControlLevel = CgwConst.PTZ_CONTROL_LEVEL;
            }
            logEx.Info("Load video monitor success.Monitor id:{0}.", this.monitorId);
            return err;
        }

        /// <summary>
        /// 监控平台加载器，轮询加载，直到成功
        /// </summary>
        /// <param name="mediaServerIdObj"></param>
        private void LoaderThread(Object mediaServerIdObj)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.LoadTiandyMonitorThread().");
            try
            {
                bool loadResult = false;
                string mediaServerId = mediaServerIdObj as string;
                while (!loadResult)
                {
                    this.GetAllCamerasTimer(null, null);
                    if (this.cameraOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                    {
                        try
                        {
                            loadResult = true;
                            this.sdkClient.SdkMediaServer = null;

                            //用户没有指定流媒体服务器，预览视频时，采用摄像头所连接的服务器。
                            if (!string.IsNullOrEmpty(mediaServerId))
                            {
                                if (this.mediaServerDictionary.ContainsKey(mediaServerId))
                                {
                                    this.sdkClient.SdkMediaServer = this.mediaServerDictionary[mediaServerId];
                                }
                                else
                                {
                                    //流媒体服务器id配置错误，Id不存在
                                    logEx.Error("Load Tiandy monitor failed.Media server id is not found.MediaServerId:{0}", mediaServerId);
                                    loadResult = false;
                                }
                            }


                            if (loadResult)
                            {
                                //设置回调函数到SDK
                                this.sdkClient.SetDataCallBackFunc(TiandyRealDataCallbackFunc);
                            }

                        }
                        finally
                        {
                            this.cameraOperateLock.ExitReadLock();
                        }
                    }

                    if (!loadResult)
                    {
                        loadResult = false;
                        Thread.Sleep(CgwConst.RELOAD_MONITOR_WAIT_TIME);
                        continue;
                    }

                    if (!this.sdkClient.Load(this.monitorId))
                    {
                        logEx.Error("Load video monitor failed.Monitor id:{0}.", this.monitorId);
                        loadResult = false;
                        Thread.Sleep(CgwConst.RELOAD_MONITOR_WAIT_TIME);
                        continue;
                    }

                    //启动定时器
                    this.updateCameraTimer.Start();
                    loadResult = true;
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("LoaderThread catch exception:{0}", ex.Message);
            }
            logEx.Trace("Exit: TiandyVideoMonitor.LoadTiandyMonitorThread().");
        }

        /// <summary>
        /// 注销天地伟业监控平台资源
        /// </summary>
        /// <returns></returns>
        public SmcErr Unload()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.Unload().");
            SmcErr err = new CgwError();
            //停止定时器
            this.updateCameraTimer.Stop();
            if (this.sdkClient.Unload())
            {
                logEx.Info("Unload video monitor success.Monitor id:{0}.", this.monitorId);
            }
            else
            {
                err.SetErrorNo(CgwError.UNLOAD_MONITOR_FAILED);
                logEx.Error("Unload video monitor failed.Monitor id:{0}.", this.monitorId);
            }
            return err;
        }

        /// <summary>
        /// 指定码流回调函数
        /// </summary>
        /// <param name="videoDataCallBack"></param>
        /// <param name="sender"></param>
        public void SetDataCallBackFunc(DataCallBack dataCallBack, string sender)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.SetDataCallBackFunc().");
            this.dataCallBack = dataCallBack;
            this.sender = sender;
        }

        /// <summary>
        /// 获取摄像头列表及分组信息
        /// </summary>
        /// <param name="fromMonitorSys">如果该值为true，则实时从监控平台获取,否则从融合网关缓存获取</param>
        /// <param name="cameraList">摄像头列表</param>
        /// <param name="groupList">组信息</param>
        /// <param name="nodeRelationListT">分组关系</param>
        /// <returns></returns>
        public SmcErr GetAllCameras(out List<Camera> cameraList, out List<CameraGroup> groupList, out List<NodeRelation> nodeRelationListT)
        {
            monitorManageServiceGetCameraList.Stop();
            isGetDevicesFinish = false;

            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.GetAllCameras().");
            SmcErr err = new CgwError();
            cameraList = new List<Camera>();
            groupList = new List<CameraGroup>();
            nodeRelationListT = new List<NodeRelation>();

            if (this.cameraOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    #region 深度克隆数据
                    foreach (KeyValuePair<string, TiandyCamera> tiandyCameraKeyValue in this.tiandyCameraDictionary)
                    {
                        TiandyCamera tiandyCamera = tiandyCameraKeyValue.Value;
                        //从缓存获取                        
                        Camera camera = new Camera(tiandyCamera.No, tiandyCamera.Name);
                        cameraList.Add(camera);
                    }
                    foreach (KeyValuePair<string, CameraGroup> groupDicKeyValue in this.groupDic)
                    {
                        CameraGroup cameraGroupTemp = new CameraGroup(groupDicKeyValue.Value.No, groupDicKeyValue.Value.Name);
                        groupList.Add(cameraGroupTemp);
                    }

                    foreach (NodeRelation nodeRelation in this.nodeRelationList)
                    {
                        NodeRelation nodeRelationTemp = new NodeRelation(nodeRelation.No,
                                                                          nodeRelation.Path,
                                                                          nodeRelation.Type);
                        nodeRelationListT.Add(nodeRelationTemp);
                    }
                    #endregion
                }
                catch (Exception e)
                {
                    err.SetErrorNo(CgwError.GET_ALL_CAMERAS_FAILED);
                    logEx.Error("Get all cameras failed.Execption message:{0}", e.Message);
                    return err;
                }
                finally
                {
                    this.cameraOperateLock.ExitReadLock();
                }
            }
            monitorManageServiceGetCameraList.Start();
            logEx.Info("Get all cameras success.");
            return err;
        }

        /// <summary>
        /// 启动摄像头预览
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        public SmcErr StartReceiveVideo(string cameraNo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.StartReceiveVideo({0}).", cameraNo);
            SmcErr err = new CgwError();

            TiandyCamera camera = null;
            Host host = null;

            //摄像头所连接的流媒体服务器
            MediaServer cameraMediaServer = null;
            if (this.cameraOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (!this.tiandyCameraDictionary.ContainsKey(cameraNo))
                    {
                        err.SetErrorNo(CgwError.CAMERA_NOT_FOUND);
                        logEx.Error("Start Receive camera video data failed.Camera No is not found.Camera No:{0}", cameraNo);
                        return err;
                    }
                    camera = this.tiandyCameraDictionary[cameraNo];
                    host = this.hostDictionary[camera.HostNo];
                    cameraMediaServer = this.mediaServerDictionary[host.MediaServerNo];
                }
                catch (Exception e)
                {
                    err.SetErrorNo(CgwError.START_RECEIVE_VIDEO_FAILED);
                    logEx.Error("Start Receive camera video data failed.Camera No:{0},Execption message:{1}", cameraNo, e.Message);
                    return err;
                }
                finally
                {
                    this.cameraOperateLock.ExitReadLock();
                }
            }

            if (camera == null)
            {
                err.SetErrorNo(CgwError.START_RECEIVE_VIDEO_FAILED);
                logEx.Error("Start Receive camera video data failed.Camera No is not found.Camera No:{0}", cameraNo);
                return err;
            }

            RealPlayInfo real = new RealPlayInfo();
            real.ch = camera.Channel;
            real.data_type = (int)TiandyDateType.PRIMARY;
            real.hPlayWnd = null;

            int result = this.sdkClient.StartReceiveVideo(host.No, cameraMediaServer, ref real);

            //如果为负数，表示预览失败
            if (result < 0)
            {
                err.SetErrorNo(CgwError.START_RECEIVE_VIDEO_FAILED);
                logEx.Error("Start Receive camera video data failed.Camera No:{0}.", cameraNo);
                return err;
            }
            else
            {
                //需要停止的预览句柄
                int needToStopHandel = CgwConst.START_RECEIVE_VIDEO_DATA_FAILED;
                if (this.handelOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                {
                    try
                    {
                        //如果预览句柄已经存在，删除掉原来的句柄,用新的句柄替换
                        if (this.cameraVideoHandeDic.ContainsKey(cameraNo))
                        {
                            needToStopHandel = this.cameraVideoHandeDic[cameraNo];
                            this.videoHandleCameraDic.Remove(needToStopHandel);
                            this.cameraVideoHandeDic.Remove(cameraNo);
                        }
                        this.cameraVideoHandeDic.Add(cameraNo, result);
                        this.videoHandleCameraDic.Add(result, cameraNo);
                    }
                    finally
                    {
                        this.handelOperateLock.ExitWriteLock();
                    }
                }
                logEx.Info("Start Receive camera video data success.Camera No:{0},Handle:{1}.", cameraNo, result);
                //重新预览后，更新了预览句柄，需要将原来的预览停止，放在handelOperateLock外面，防止长时间占用锁
                if (needToStopHandel != CgwConst.START_RECEIVE_VIDEO_DATA_FAILED)
                {
                    this.sdkClient.StopReceiveVideo(needToStopHandel);
                }
            }

            return err;
        }

        /// <summary>
        /// 停止预览
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns>成功返回0，失败返回错误码</returns>
        public SmcErr StopReceiveVideo(string cameraNo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.StopReceiveVideo({0}).", cameraNo);
            SmcErr err = new CgwError();

            //需要停止的预览句柄
            int needToStopHandel = CgwConst.START_RECEIVE_VIDEO_DATA_FAILED;
            if (this.handelOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (this.cameraVideoHandeDic.ContainsKey(cameraNo))
                    {
                        needToStopHandel = this.cameraVideoHandeDic[cameraNo];
                        this.videoHandleCameraDic.Remove(needToStopHandel);
                        this.cameraVideoHandeDic.Remove(cameraNo);
                    }
                    else
                    {
                        //不存在，表示已经删除，不返回错误
                        //err.SetErrorNo(CgwError.STOP_RECEIVE_VIDEO_FAILED);
                        logEx.Warn("Stop Receive camera video data failed. Don't need to end the preview.Camera No:{0}.", cameraNo);
                    }
                }
                finally
                {
                    this.handelOperateLock.ExitWriteLock();
                }
            }

            //调用sdk的停止方法，放在handelOperateLock外面，防止长时间占用锁
            if (needToStopHandel != CgwConst.START_RECEIVE_VIDEO_DATA_FAILED)
            {
                this.sdkClient.StopReceiveVideo(needToStopHandel);
            }

            logEx.Info("Stop Receive camera video data success.Camera No:{0},Handle:{1}.", cameraNo, needToStopHandel);
            return err;
        }

        /// <summary>
        /// 开始云台控制，摄像头控制
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="ptzCommand">命令类型</param>
        /// <param name="param">命令参数（速度、倍数）</param>
        /// <returns></returns>
        public SmcErr StartControlPtz(string cameraNo, PtzCommandType ptzCommand, int param)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.StartControlPtz({0},{1},{2}).", cameraNo, ptzCommand, param);
            SmcErr err = new CgwError();
            int handel = CgwConst.START_RECEIVE_VIDEO_DATA_FAILED;

            if (this.handelOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (this.cameraVideoHandeDic.ContainsKey(cameraNo))
                    {
                        handel = this.cameraVideoHandeDic[cameraNo];
                    }
                    else
                    {
                        err.SetErrorNo(CgwError.START_CONTROL_PTZ_FAILED);
                        logEx.Error("The camera was not previewed.Camera No:{0}.", cameraNo);
                        return err;
                    }
                }
                finally
                {
                    this.handelOperateLock.ExitReadLock();
                }
            }

            if (handel != CgwConst.START_RECEIVE_VIDEO_DATA_FAILED)
            {
                Client_PTZ_Command client_ptz_command = new Client_PTZ_Command();

                client_ptz_command.param = param;
                client_ptz_command.start = true;

                //将ptz命令转换成天地伟业的命令
                string name = Enum.GetName(typeof(PtzCommandType), ptzCommand);
                if (Enum.IsDefined(typeof(TiandyPtzCommandType), name))
                {
                    TiandyPtzCommandType tiandyPtzCommandType = (TiandyPtzCommandType)Enum.Parse(typeof(TiandyPtzCommandType), name);

                    client_ptz_command.cmd = (int)tiandyPtzCommandType;
                }
                else
                {
                    err.SetErrorNo(CgwError.START_CONTROL_PTZ_FAILED);
                    logEx.Error("The tiandy monitor is not support the command.ptzCommand:{0}.", ptzCommand);
                    return err;
                }

                if (this.sdkClient.ControlPtz(handel, client_ptz_command))
                {
                    logEx.Info("Start control camera ptz success.Camera No:{0}.", cameraNo);
                }
                else
                {
                    err.SetErrorNo(CgwError.START_CONTROL_PTZ_FAILED);
                    logEx.Error("Start control camera ptz failed.Camera No:{0}.", cameraNo);
                }
            }
            else
            {
                err.SetErrorNo(CgwError.START_CONTROL_PTZ_FAILED);
                logEx.Error("Handel is not found. Start control camera ptz failed. Camera No:{0}.", cameraNo);
                return err;
            }

            return err;
        }

        /// <summary>
        /// 停止云台控制，摄像头控制
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="ptzCommand">命令类型</param>
        /// <returns></returns>
        public SmcErr StopControlPtz(string cameraNo, PtzCommandType ptzCommand)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.StopControlPtz({0}).", cameraNo);
            SmcErr err = new CgwError();

            int handel = CgwConst.START_RECEIVE_VIDEO_DATA_FAILED;
            if (this.handelOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (this.cameraVideoHandeDic.ContainsKey(cameraNo))
                    {
                        handel = this.cameraVideoHandeDic[cameraNo];
                    }
                    else
                    {
                        logEx.Warn("The camera was not Preview.Don't need to end the control.Camera No:{0}.", cameraNo);
                    }
                }
                finally
                {
                    this.handelOperateLock.ExitReadLock();
                }

            }

            if (handel != CgwConst.START_RECEIVE_VIDEO_DATA_FAILED)
            {
                Client_PTZ_Command client_ptz_command = new Client_PTZ_Command();
                client_ptz_command.start = false;

                //将ptz命令转换成天地伟业的命令
                string name = Enum.GetName(typeof(PtzCommandType), ptzCommand);
                if (Enum.IsDefined(typeof(TiandyPtzCommandType), name))
                {
                    TiandyPtzCommandType tiandyPtzCommandType = (TiandyPtzCommandType)Enum.Parse(typeof(TiandyPtzCommandType), name);
                    client_ptz_command.cmd = (int)tiandyPtzCommandType;
                }
                else
                {
                    err.SetErrorNo(CgwError.STOP_CONTROL_PTZ_FAILED);
                    logEx.Error("The tiandy monitor is not support the command.ptzCommand:{0}.", ptzCommand);
                    return err;
                }

                bool result = this.sdkClient.ControlPtz(handel, client_ptz_command);
                if (result)
                {
                    logEx.Info("Stop control camera ptz success.Camera No:{0}.", cameraNo);
                }
                else
                {
                    err.SetErrorNo(CgwError.STOP_CONTROL_PTZ_FAILED);
                    logEx.Error("Stop control camera ptz failed.Camera No:{0}.", cameraNo);
                }
            }
            else
            {
                err.SetErrorNo(CgwError.STOP_CONTROL_PTZ_FAILED);
                logEx.Error("Handel is not found. Stop control camera ptz failed. Camera No:{0}.", cameraNo);
                return err;
            }

            return err;
        }

        /// <summary>
        /// 重发I帧
        /// 2013-7-15，监控平台自动定时发I帧，默认是4S内，因此重发I帧作用不大，还有可能造成内存泄露
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        public SmcErr MakeIFrame(string cameraNo)
        {
            //NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            //logEx.Trace("Enter: TiandyVideoMonitor.MakeIFrame({0}).", cameraNo);
            SmcErr err = new CgwError();

            //TiandyCamera camera = null;
            //Host host = null;

            //if (this.cameraOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            //{
            //    try
            //    {
            //        if (!this.tiandyCameraDictionary.ContainsKey(cameraNo))
            //        {
            //            err.SetErrorNo(CgwError.CAMERA_NOT_FOUND);
            //            logEx.Error("Make I frame failed.Camera No is not found.Camera No:{0}.", cameraNo);
            //            return err;
            //        }

            //        camera = this.tiandyCameraDictionary[cameraNo];
            //        host = this.hostDictionary[camera.HostNo];

            //    }
            //    catch (Exception e)
            //    {
            //        err.SetErrorNo(CgwError.MAKE_I_FRAME_FAILED);
            //        logEx.Error("Make I frame failed.Camera No:{0},Execption message:{1}", cameraNo, e.Message);
            //        return err;
            //    }
            //    finally
            //    {
            //        this.cameraOperateLock.ExitReadLock();
            //    }
            //}

            //if (camera == null)
            //{
            //    err.SetErrorNo(CgwError.MAKE_I_FRAME_FAILED);
            //    logEx.Error("Make I frame failed.Camera No is not found.Camera No:{0}", cameraNo);
            //    return err;
            //}

            //if (this.sdkClient.MakeIFrame(host.No, camera.Channel, host.MediaServerNo))
            //{
            //    logEx.Info("Make I frame success.Camera No:{0}", cameraNo);
            //}
            //else
            //{
            //    err.SetErrorNo(CgwError.MAKE_I_FRAME_FAILED);
            //    logEx.Error("Make I frame failed.Camera No:{0}", cameraNo);
            //}

            return err;
        }

        /// <summary>
        /// 设置扬声器状态（暂不支持）
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="isOn">扬声器是否开启</param>
        /// <returns></returns>
        public SmcErr SetSpeaker(string cameraNo, bool isOn)
        {
            SmcErr err = new CgwError();
            return err;
        }

        /// <summary>
        /// 设置麦克风状态
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="isOn">麦克风是否开启</param>
        /// <returns></returns>
        public SmcErr SetMic(string cameraNo, bool isOn)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.SetMic({0}，{1}).", cameraNo, isOn);
            SmcErr err = new CgwError();

            if (this.micOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (this.cameraMicStatusDic.ContainsKey(cameraNo))
                    {
                        this.cameraMicStatusDic[cameraNo] = isOn;
                    }
                    else
                    {
                        this.cameraMicStatusDic.Add(cameraNo, isOn);
                    }
                }
                finally
                {
                    this.micOperateLock.ExitWriteLock();
                }
            }
            logEx.Info("Set Mic status success.Camera no:{0}，isOn:{1}).", cameraNo, isOn);
            return err;
        }

        /// <summary>
        /// 清除缓存数据
        /// </summary>
        private void ClearCamera()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.ClearCamera().");
            if (this.cameraOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    this.tiandyCameraDictionary = new Dictionary<string, TiandyCamera>();
                    this.hostDictionary = new Dictionary<string, Host>();
                    this.mediaServerDictionary = new Dictionary<string, MediaServer>();

                    this.groupDic = new Dictionary<string, CameraGroup>();
                    this.nodeRelationList = new List<NodeRelation>();
                }
                finally
                {
                    this.cameraOperateLock.ExitWriteLock();
                }
            }
            logEx.Trace("Clear Camera which in the cache success.");
        }

        /// <summary>
        /// 获取摄像头列表及分组信息
        /// </summary>
        private void GetAllCamerasMethod()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.GetAllCamerasTimer().");

            try
            {

                //调用天地伟业http接口获取天地伟业设备管理树列表
                List<Resource> resourceTree = this.httpClient.GetResourceTree();
                if (resourceTree == null)
                {
                    logEx.Error("GetResourceTree failed.");
                    ClearCamera();
                    isRefreshSucess = false;
                    return;
                }

                //调用天地伟业http接口获取自定义设备树列表
                List<Resource> customTree = this.httpClient.GetCustomTree();

                if (customTree == null)
                {
                    logEx.Error("GetCustomTree failed.");
                    ClearCamera();
                    isRefreshSucess = false;
                    return;
                }

                Dictionary<string, TiandyCamera> tiandyCameraDictionaryTemp = new Dictionary<string, TiandyCamera>();
                Dictionary<string, Host> hostDictionaryTemp = new Dictionary<string, Host>();
                Dictionary<string, MediaServer> mediaServerDictionaryTemp = new Dictionary<string, MediaServer>();
                //递归处理，将摄像头、主机、流媒体服务器遍历出来
                RecursionCamera(resourceTree, tiandyCameraDictionaryTemp, hostDictionaryTemp, mediaServerDictionaryTemp);

                Dictionary<string, CameraGroup> groupDicTemp = new Dictionary<string, CameraGroup>();
                Dictionary<string, NodeRelation> nodeRelationDicTemp = new Dictionary<string, NodeRelation>();
                List<NodeRelation> nodeRelationListTemp = new List<NodeRelation>();

                //递归处理，获取组，摄像头、分组关系
                RecursionCameraGroup(customTree, null, groupDicTemp, nodeRelationDicTemp, nodeRelationListTemp);

                //对于未分组的摄像头，父节点设置为空
                foreach (KeyValuePair<string, TiandyCamera> tiandyCameraKeyValue in tiandyCameraDictionaryTemp)
                {
                    if (!nodeRelationDicTemp.ContainsKey(tiandyCameraKeyValue.Key))
                    {
                        NodeRelation nodeRelation = new NodeRelation(tiandyCameraKeyValue.Key,
                                                                     new List<string>(),
                                                                     CgwMonitorManage.Common.NodeType.CAMERA);
                        nodeRelationListTemp.Add(nodeRelation);
                    }
                }

                DateTime dtStart = DateTime.Now;
                DateTime dtNow = new DateTime();
                while (!isGetDevicesFinish)
                {
                    dtNow = DateTime.Now;

                    if ((dtNow - dtStart).TotalSeconds > refreshDeviceListOverTime)
                    {
                        isRefreshSucess = false;
                        return;
                    }
                    Thread.Sleep(1);
                    continue;
                }

                if (this.cameraOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                {
                    try
                    {
                        this.tiandyCameraDictionary = tiandyCameraDictionaryTemp;
                        this.hostDictionary = hostDictionaryTemp;
                        this.mediaServerDictionary = mediaServerDictionaryTemp;

                        this.groupDic = groupDicTemp;
                        this.nodeRelationList = nodeRelationListTemp;

                        isRefreshSucess = true;

                    }
                    catch (Exception ex)
                    {
                        isRefreshSucess = false;
                        logEx.Error("Recursion camera failed.Execption message:{0}", ex.Message);
                    }
                    finally
                    {
                        this.cameraOperateLock.ExitWriteLock();
                    }
                }
            }
            catch (System.Exception ex)
            {
                isRefreshSucess = false;
                logEx.Error("GetAllCamerasTimer catch Exception:{0}", ex.Message);
            }
        }

        /// <summary>
        /// 获取摄像头列表及分组信息
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GetAllCamerasTimer(object sender, System.Timers.ElapsedEventArgs e)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.GetAllCamerasTimer().");

            if (refreshMonitorCameraOperateLock.TryEnterWriteLock(CgwConst.EFRESH_MONITOR_CAMERA_WAIT_TIME))
            {
                try
                {
                    GetAllCamerasMethod();
                }
                finally
                {
                    refreshMonitorCameraOperateLock.ExitWriteLock();
                }
            }
        }

        /// <summary>
        /// 递归天地伟业返回的设备树，获取所有通道（一个通道对应一个摄像头）、主机、流媒体（中间件）服务器
        /// </summary>
        /// <param name="resourceTree">天地伟业返回的设备树</param>
        /// <param name="tiandyCameraDictionary">摄像头键值对，key为摄像头编号</param>
        /// <param name="hostDictionary">主机键值对，key为主机编号</param>
        /// <param name="mediaServerDictionary">流媒体（中间件）服务键值对，key为编号</param>
        private void RecursionCamera(List<Resource> resourceTree, Dictionary<string, TiandyCamera> tiandyCameraDictionary, Dictionary<string, Host> hostDictionary, Dictionary<string, MediaServer> mediaServerDictionary)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            if (resourceTree == null)
            {
                logEx.Error("Recursion camera failed.ResourceTree is null.");
                return;
            }
            if (tiandyCameraDictionary == null)
            {
                tiandyCameraDictionary = new Dictionary<string, TiandyCamera>();
            }
            if (hostDictionary == null)
            {
                hostDictionary = new Dictionary<string, Host>();
            }
            if (mediaServerDictionary == null)
            {
                mediaServerDictionary = new Dictionary<string, MediaServer>();
            }
            foreach (Resource resource in resourceTree)
            {
                //TypeId为5时，表示该节点为通道，对应一个摄像头
                if (((int)NodeType.CAMERA).ToString().Equals(resource.TypeId))
                {
                    TiandyCamera camera = new TiandyCamera(resource.Id, resource.Caption);
                    camera.Channel = resource.Ch;
                    //摄像头的父节点为主机
                    camera.HostNo = resource.ParentId;
                    tiandyCameraDictionary.Add(camera.No, camera);

                    //如果是摄像头节点，不需要递归
                    continue;
                }
                else if (((int)NodeType.HOST).ToString().Equals(resource.TypeId))
                {
                    //将主机加入缓存，主机的父节点为流媒体（中间件）服务器
                    Host host = new Host(
                                resource.Id,
                                resource.Username,
                                resource.Pwd,
                                resource.ParentId);

                    hostDictionary.Add(host.No, host);
                }
                else if (((int)NodeType.MEDIASERVER).ToString().Equals(resource.TypeId))
                {
                    //将流媒体（中间件）服务器加入缓存
                    MediaServer mediaServer = new MediaServer(
                                                resource.Id,
                                                resource.Ip,
                                                resource.Port,
                                                resource.Username,
                                                resource.Pwd);

                    mediaServerDictionary.Add(mediaServer.No, mediaServer);
                }

                //递归处理
                RecursionCamera(resource.items, tiandyCameraDictionary, hostDictionary, mediaServerDictionary);
            }

        }

        /// <summary>
        /// 递归天地伟业返回的自定义设备列表，只获取分组节点（TypeId为1000）和通道（摄像头）节点（TypeId为5），舍弃其他节点（如主机，中间件服务器等）
        /// </summary>
        /// <param name="customTree">天地伟业返回的自定义设备列表树</param>
        /// <param name="pathList">节点路径，如果为跟节点，传null即可，主要是用于递归函数处理</param>
        /// <param name="groupDic">组列表</param>
        /// <param name="nodeRelationDic">组、摄像头关系列表Dic,不能重复</param>
        /// <param name="nodeRelationListT">组、摄像头关系列表，可以重复，解决同一摄像头在不同分组下，融合网关报错的问题</param>
        /// 
        private void RecursionCameraGroup(List<Resource> customTree, List<string> pathList, Dictionary<string, CameraGroup> groupDic, Dictionary<string, NodeRelation> nodeRelationDic, List<NodeRelation> nodeRelationListT)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            if (customTree == null)
            {
                logEx.Error("RecursionCameraGroup failed.CustomTree is null.");
                return;
            }

            if (pathList == null)
            {
                pathList = new List<string>();
            }

            if (groupDic == null)
            {
                groupDic = new Dictionary<string, CameraGroup>();
            }

            if (nodeRelationDic == null)
            {
                nodeRelationDic = new Dictionary<string, NodeRelation>();
            }

            if (nodeRelationListT == null)
            {
                nodeRelationListT = new List<NodeRelation>();
            }

            foreach (Resource custom in customTree)
            {
                //TypeId为5时，表示该节点为通道，对应一个摄像头
                if (((int)NodeType.CAMERA).ToString().Equals(custom.TypeId))
                {
                    //pathList.Add(custom.Id);
                    NodeRelation nodeRelation = new NodeRelation(custom.Id, new List<string>(pathList), CgwMonitorManage.Common.NodeType.CAMERA);
                    if (!nodeRelationDic.ContainsKey(custom.Id))
                    {
                        nodeRelationDic.Add(custom.Id, nodeRelation);
                    }
                    nodeRelationListT.Add(nodeRelation);

                    //获取完路径后，要返回上一级路径
                    //pathList.Remove(custom.Id);
                }
                //TypeId为1000时，表示该节点为分组
                else if (((int)NodeType.GROUP).ToString().Equals(custom.TypeId))
                {
                    //添加组信息
                    CameraGroup cameraGroup = new CameraGroup(custom.Id, custom.Caption);
                    groupDic.Add(custom.Id, cameraGroup);

                    NodeRelation nodeRelation = new NodeRelation(custom.Id, new List<string>(pathList), CgwMonitorManage.Common.NodeType.GROUP);
                    if (!nodeRelationDic.ContainsKey(custom.Id))
                    {
                        nodeRelationDic.Add(custom.Id, nodeRelation);
                    }
                    nodeRelationListT.Add(nodeRelation);

                    //添加分组关系
                    pathList.Add(custom.Id);

                    //如果是组，还需要递归处理，遍历子节点
                    RecursionCameraGroup(custom.items, pathList, groupDic, nodeRelationDic, nodeRelationListT);

                    //获取完路径后，要返回上一级路径
                    pathList.Remove(custom.Id);

                }
                else
                {
                    //其他类型节点不做处理
                }
            }
        }

        /// <summary>
        /// 码流回调处理，用于中转
        /// </summary>
        /// <param name="playfd">预览句柄</param>
        /// <param name="datatype">视频流类型</param>
        /// <param name="buf">码流</param>
        /// <param name="size">码流大小</param>
        /// <param name="usr">用户标识数据</param>
        private void TiandyRealDataCallbackFunc(int playfd, int datatype, byte[] buf, uint size, int usr)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            string cameraNo = null;
            try
            {

                StreamType streamType = StreamType.VIDEO_H264;
                //判断码流类型，因为码流回调访问频频非常高，不单独抽成一个方法，减少方法访问开销
                //对于支持的码流类型，用break退出switch，对于不支持的码流类型直接舍弃，用return返回
                switch (datatype)
                {
                    case (int)TiandyStreamType.STREAM_TYPE_VIDEO_FRAME_I:
                        streamType = StreamType.VIDEO_H264;
                        break;
                    case (int)TiandyStreamType.STREAM_TYPE_VIDEO_FRAME_P:
                        streamType = StreamType.VIDEO_H264;
                        break;
                    case (int)TiandyStreamType.STREAM_TYPE_VIDEO_FRAME_B:
                        streamType = StreamType.VIDEO_H264;
                        break;

                    //音频只接收G711A和G711U，其他舍弃
                    case (int)TiandyStreamType.STREAM_TYPE_AUDIO_G711A:
                        streamType = StreamType.AUDIO_G711A;
                        break;
                    case (int)TiandyStreamType.STREAM_TYPE_AUDIO_G711U:
                        streamType = StreamType.AUDIO_G711U;
                        break;
                    default:
                        //不支持的类型直接舍弃，返回
                        //logEx.Warn("This stream type is not support. Chuck the data.StreamType:{0};Camera no:{1}", datatype, cameraNo);
                        return;
                }

                if (this.handelOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                {
                    try
                    {
                        if (!this.videoHandleCameraDic.ContainsKey(playfd))
                        {
                            logEx.Error("The video data handle is not found.Handle:{0}", playfd);
                            return;
                        }
                        cameraNo = this.videoHandleCameraDic[playfd];
                        if (string.IsNullOrEmpty(cameraNo))
                        {
                            return;
                        }
                    }
                    finally
                    {
                        this.handelOperateLock.ExitReadLock();
                    }
                }

                if (streamType == StreamType.AUDIO_G711A || streamType == StreamType.AUDIO_G711U)
                {
                    //如果是音频流，需要判断mic的状态，开启时才发送音频流
                    if (this.micOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                    {
                        try
                        {
                            if (this.cameraMicStatusDic.ContainsKey(cameraNo))
                            {
                                //如果mic为非开启状态，则不发送音频码流,
                                if (!this.cameraMicStatusDic[cameraNo])
                                {
                                    //logEx.Warn("This data is audio,but the mic is off.Chuck the data.Camera no:{0}", cameraNo);
                                    return;
                                }
                            }
                            else
                            {
                                //默认为关闭状态，因此如果cameraMicStatusDic不包含该摄像头，则认为处于关闭状态，舍弃音频码流
                                //logEx.Warn("This data is audio,but the mic is off.Chuck the data.Camera no:{0}", cameraNo);
                                return;
                            }
                        }
                        finally
                        {
                            this.micOperateLock.ExitReadLock();
                        }
                    }

                }

                MediaData mediaData = new MediaData();
                mediaData.Data = buf;
                mediaData.DataType = MediaDataType.FRAME_DATA;
                mediaData.StreamType = streamType;
                mediaData.Size = size;

                //向回调函数转发码流
                this.dataCallBack(cameraNo, mediaData, sender);
            }
            catch (Exception e)
            {
                logEx.Error("Send the media data failed.The camera No is {0}.Execption message:{1}", cameraNo, e.Message);
            }
        }


        /// <summary>
        /// 刷新监控摄像头列表
        /// </summary>
        /// <returns></returns>
        public SmcErr RefreshMonitorCamera()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.RefreshMonitorCamera");
            SmcErr err = new CgwError();

            if (refreshMonitorCameraOperateLock.TryEnterWriteLock(CgwConst.EFRESH_MONITOR_CAMERA_WAIT_TIME))
            {
                try
                {
                    GetAllCamerasMethod();
                    //重新开始计时
                    updateCameraTimer.Stop();
                    updateCameraTimer.Start();
                }
                finally
                {
                    refreshMonitorCameraOperateLock.ExitWriteLock();
                }
            }
            logEx.Info("RefreshMonitorCamera success.");
            return err;
        }

        /// <summary>
        /// 获取监控摄像头列表刷新状态，返回结果为0是表示刷新完毕，为1是刷新操作中。当查询刷新状态为0时，可调用获取监控摄像头列表接口，获取刷新后监控摄像头列表
        /// </summary>
        /// <param name="refreshStatus">刷新状态</param>
        /// <returns></returns>
        public SmcErr GetRefreshStatus(out SmcErr refreshStatus)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandyVideoMonitor.GetRefreshStatus");
            SmcErr err = new CgwError();
            refreshStatus = new SmcErr(); 
            refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END;

            if (refreshMonitorCameraOperateLock.TryEnterWriteLock(CgwConst.EFRESH_MONITOR_CAMERA_WAIT_TIME))
            {
                try
                {
                    refreshStatus.ErrNo = isRefreshSucess ? CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END : CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED;
                }
                finally
                {
                    refreshMonitorCameraOperateLock.ExitWriteLock();
                }
            }
            else
            {
                refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_EXECUTING;
            }
            logEx.Info("GetRefreshStatus success.");
            return err;
        }
    }
}
