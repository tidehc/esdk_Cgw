/*-----------------------------------------------------------------------
//文件名：IvsVideoMonitor.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-3-28
//描述：用于调用IVS接口，解析返回数据
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using CgwMonitorManage.SmcError;
using CgwMonitorManage.Common;
using System.Threading;
using CgwMonitorManage.Ivs;
using System.Runtime.InteropServices;

namespace CgwMonitorManage.Ivs
{
    public class IvsVideoMonitor : IVideoMonitor
    {
        /// <summary>
        /// ip
        /// </summary>
        string ip = string.Empty;

        /// <summary>
        /// 端口
        /// </summary>
        string port = string.Empty;

        /// <summary>
        /// 用户
        /// </summary>
        string user = string.Empty;

        /// <summary>
        /// 加密密码 
        /// </summary>
        //string password = string.Empty;
        byte[] pwdByte = null;

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
        /// ivs 日志根路径
        /// </summary>
        private static string ivsLogPath = AppDomain.CurrentDomain.BaseDirectory + "IVSSDK/ivs_log";

        /// <summary>
        /// 向上码流回调函数
        /// </summary>
        private DataCallBack dataCallBack;

        /// <summary>
        /// 向下码流回调，为了防止回调函数被销毁导致码流回调失败，定义为成员变量 
        /// </summary>
        private IvsRealPlayCallBackRawFunc sdkCallbackFunc;

        /// <summary>
        /// sdk事件回调，用于监控重连
        /// </summary>
        private IvsEventCallBackFunc ivsEventCallBackFunc;

        /// <summary>
        /// 码流回调者
        /// </summary>
        private string sender;

        /// <summary>
        /// 监控平台ID
        /// </summary>
        private string monitorId;

        /// <summary>
        /// 保存摄像头跟预览句柄的关系，key为摄像头编号，value为预览句柄，一对一的关系
        /// </summary>
        private Dictionary<string, int> cameraVideoHandeDic = new Dictionary<string, int>();

        /// <summary>
        /// 保存预览句柄跟摄像头的关系，key为预览句柄，value为摄像头编号，一对一的关系
        /// </summary>
        private Dictionary<int, MediaDataSender> videoHandleCameraDic = new Dictionary<int, MediaDataSender>();

        /// <summary>
        /// 预览句柄操作锁，用于cameraVideoHandeDic、videoHandleCameraDic的操作
        /// </summary>
        private ReaderWriterLockSlim handelOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 保存摄像头和mic状态，key为camera编号，value为mic是否开启标记
        /// </summary>
        private Dictionary<string, bool> cameraMicStatusDic = new Dictionary<string, bool>();

        /// <summary>
        /// mic状态dictionary操作锁
        /// </summary>
        private ReaderWriterLockSlim micOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 摄像头操作锁
        /// </summary>
        private ReaderWriterLockSlim cameraOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 摄像头列表
        /// </summary>
        private List<Camera> cameraList = new List<Camera>();

        /// <summary>
        /// 自定义组列表
        /// </summary>
        private List<CameraGroup> groupList = new List<CameraGroup>();

        /// <summary>
        /// 分组管理列表
        /// </summary>
        private List<NodeRelation> nodeRelationList = new List<NodeRelation>();

        private IvsSdkClient ivsSdkClient;

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

        /// <summary>
        /// 连接登录线程判断是否成功
        /// </summary>
        bool loadResult = false;

        public IvsVideoMonitor()
        {
            this.ivsSdkClient = new IvsSdkClient();
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

        /// <summary>
        /// 初始化IVS监控平台
        /// </summary>
        /// <param name="monitorConfigElement">监控平台配置节点</param>
        /// <returns></returns>
        public SmcErr Load(XmlElement monitorConfigElement)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("Enter: IvsVideoMonitor.Load().");
            SmcErr err = new CgwError();

            try
            {
                //解析xml节点，获取所需参数    
                this.monitorId = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.ID_TAG);
                ip = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.IP_TAG);
                port = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.PORT_TAG);
                user = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.USER_TAG);

                //password = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.PASSWORD_TAG);
                pwdByte = CommonFunction.EncryptStr2Byte(CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.PASSWORD_TAG),CgwConst.PASSWORD_TAG);

                //从配置文件读取的参数要简单判断一下合法性

                //初始化之前设置日志路径
                logEx.Trace("Call SetLogPath:{0}", ivsLogPath);
                this.ivsSdkClient.SetLogPath(ivsLogPath);

                logEx.Trace("Call ivsSdkClient.Init().");
                //初始化
                int result = this.ivsSdkClient.Init();
                if (result != CgwConst.IVS_SDK_SUCCESS_TAG)
                {
                    //直接将IVS的错误码返回
                    err.SetErrorNo(CgwError.MONITOR_LOAD_FAILED);
                    logEx.Error("Init ivs monitor failed.Monitor id:{0}.ivs sdk error code:{1}.", this.monitorId, result);
                    return err;
                }
                else
                {
                    logEx.Info("Init ivs sdk success.Monitor id:{0}", this.monitorId);
                }

                //设置监控平台id
                this.ivsSdkClient.SetId(this.monitorId);

                this.sdkCallbackFunc = IvsRealPlayCallBackRaw;
                this.ivsSdkClient.SetRealPlayCBRawCallBackFunc(this.sdkCallbackFunc);

                //sdk事件回调
                this.ivsEventCallBackFunc = IvsEventCallBackFunc;
                this.ivsSdkClient.SetEventCallBackFunc(this.ivsEventCallBackFunc);

                IvsLoginInfo loginInfo = new IvsLoginInfo();
                loginInfo.Ip = ip;
                loginInfo.Port = int.Parse(port);
                loginInfo.User = user;
                //loginInfo.Pwd = password;
                loginInfo.Pwd = CommonFunction.DecryptByte2Str(pwdByte,CgwConst.PASSWORD_TAG);

                loadResult = false;
                //使用线程轮询加载监控平台
                Thread loaderThread = new Thread(LoginThread);
                loaderThread.IsBackground = true;
                loaderThread.Start(loginInfo);
            }
            catch (Exception e)
            {
                err.SetErrorNo(CgwError.MONITOR_CONFIG_FILE_INVALID);
                logEx.Error("Load ivs monitor failed.Execption message:{0}.", e.Message);
                return err;
            }

            logEx.Info("Load video monitor success.Monitor id:{0}.", this.monitorId);
            return err;
        }

        /// <summary>
        /// 监控平台加载器，轮询加载，直到成功
        /// </summary>
        /// <param name="loginInfo"></param>
        private void LoginThread(Object loginInfoObj)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: IvsVideoMonitor.LoginThread().");
            try
            {

                IvsLoginInfo loginInfo = loginInfoObj as IvsLoginInfo;
                
                while (!loadResult)
                {
                    if (loginInfo == null)
                    {
                        logEx.Error("loginInfoObj is Null.");
                        return;
                    }
                    //等待ivs sdk事件回调结束，再调用sdk接口，否则调用失败
                    Thread.Sleep(1000);
                    logEx.Trace("Call ivsSdkClient.Login({0},{1},{2},******).", loginInfo.Ip, loginInfo.Port, loginInfo.User);
                    //登陆
                    int result = this.ivsSdkClient.Login(loginInfo.Ip, loginInfo.Port, loginInfo.User, loginInfo.Pwd);
                    if (result != CgwConst.IVS_SDK_SUCCESS_TAG)
                    {
                        logEx.Error("Login to ivs monitor failed.Monitor id:{0}.ip:{1}, port:{2}, user:{3}.ivs sdk error code:{4}.",
        this.monitorId, loginInfo.Ip, loginInfo.Port, loginInfo.User, result);

                        //登陆失败，等待1分钟后再次尝试登陆
                        Thread.Sleep(CgwConst.RELOAD_MONITOR_WAIT_TIME);
                        loadResult = false;
                        continue;
                    }
                    else
                    {
                        logEx.Info("Login to ivs monitor success.Monitor id:{0}.ip:{1}, port:{2}, user:{3}.",
        this.monitorId, loginInfo.Ip, loginInfo.Port, loginInfo.User);
                        loadResult = true;
                    }

                    this.GetAllCamerasTimer(null, null);

                    //启动定时器
                    this.updateCameraTimer.Start();
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("Load IVS monitor failed. Exception message:{0}.", ex.Message);
            }
        }

        /// <summary>
        /// 注销IVS监控平台资源
        /// </summary>
        /// <returns></returns>
        public SmcErr Unload()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("Enter: IvsVideoMonitor.Unload().");
            SmcErr err = new CgwError();

            loadResult = true;
            //停止定时器
            this.updateCameraTimer.Stop();
            logEx.Trace("Call ivsSdkClient.Logout().");
            int result = this.ivsSdkClient.Logout();
            logEx.Trace("Call ivsSdkClient.Cleanup().");
            result = this.ivsSdkClient.Cleanup();

            if (result == CgwConst.IVS_SDK_SUCCESS_TAG)
            {
                logEx.Info("Unload video monitor success.Monitor id:{0}.", this.monitorId);
            }
            else
            {
                err.SetErrorNo(CgwError.MONITOR_UDLOAD_FAILED);
                logEx.Error("Unload video monitor failed.Monitor id:{0}.", this.monitorId);
            }
            return err;
        }

        /// <summary>
        /// 指定码流回调函数
        /// </summary>
        /// <param name="videoDataCallBack"></param>
        /// <param name="sender"></param>
        public void SetDataCallBackFunc(DataCallBack videoDataCallBack, string sender)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: IvsVideoMonitor.SetVideoDataCallBackFunc().");
            this.dataCallBack = videoDataCallBack;
            this.sender = sender;
            logEx.Info("Set VideoDataCallBackFunc success. Monitor id:{0}", this.monitorId);
        }

        /// <summary>
        /// 获取摄像头列表及分组信息
        /// </summary>
        /// <param name="isRealTime">是否实时获取，融合网关有个缓存，间隔一段时间获取，默认是从融合网关获取列表，如果该值为true，则实时获取</param>
        /// <param name="cameraList">摄像头列表</param>
        /// <param name="groupList">组信息</param>
        /// <param name="nodeRelationList">分组关系</param>
        /// <returns></returns>
        public SmcErr GetAllCameras(out List<Camera> cameraList, out List<CameraGroup> groupList, out List<NodeRelation> nodeRelationList)
        {
            monitorManageServiceGetCameraList.Stop();
            isGetDevicesFinish = false;

            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: IvsVideoMonitor.GetAllCameras().");
            SmcErr err = new CgwError();
            cameraList = new List<Camera>();
            groupList = new List<CameraGroup>();
            nodeRelationList = new List<NodeRelation>();

            if (this.cameraOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    #region 深度克隆数据
                    foreach (Camera ivsCamera in this.cameraList)
                    {
                        //从缓存获取                        
                        Camera camera = new Camera(ivsCamera.No, ivsCamera.Name);
                        camera.Status = ivsCamera.Status;
                        cameraList.Add(camera);
                    }
                    foreach (CameraGroup cameraGroup in this.groupList)
                    {
                        CameraGroup cameraGroupTemp = new CameraGroup(cameraGroup.No, cameraGroup.Name);
                        groupList.Add(cameraGroupTemp);
                    }
                    foreach (NodeRelation nodeRelation in this.nodeRelationList)
                    {
                        NodeRelation nodeRelationTemp = new NodeRelation(nodeRelation.No, nodeRelation.Path, nodeRelation.Type);
                        nodeRelationList.Add(nodeRelationTemp);
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

        void monitorManageServiceGetCameraList_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            isGetDevicesFinish = true;
        }

        /// <summary>
        /// 启动摄像头预览
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        public SmcErr StartReceiveVideo(string cameraNo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: IvsVideoMonitor.StartReceiveVideo({0}).", cameraNo);
            SmcErr err = new CgwError();
            int handle;
            logEx.Trace("Call ivsSdkClient.StartRealPlayCBRaw({0}).", cameraNo);
            int result = this.ivsSdkClient.StartRealPlayCBRaw(cameraNo, out handle);

            //如果不为0，表示预览失败
            if (result != CgwConst.IVS_SDK_SUCCESS_TAG)
            {
                err.SetErrorNo(CgwError.START_RECEIVE_VIDEO_FAILED);
                logEx.Error("Start Receive camera video data failed.Camera No:{0}.Ivs sdk error code:{1}", cameraNo, result);
                return err;
            }
            else
            {
                logEx.Info("Start Receive camera video data success.Camera No:{0},Handle:{1}.", cameraNo, handle);
            }

            //预览成功，需要停止原来的预览，并将预览句柄添加到缓存
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
                    this.cameraVideoHandeDic.Add(cameraNo, handle);
                    MediaDataSender mediaDataSender = new MediaDataSender(cameraNo, this.dataCallBack);
                    this.videoHandleCameraDic.Add(handle, mediaDataSender);
                }
                finally
                {
                    this.handelOperateLock.ExitWriteLock();
                }
            }

            //重新预览后，更新了预览句柄，需要将原来的预览停止，放在handelOperateLock外面，防止长时间占用锁
            if (needToStopHandel != CgwConst.START_RECEIVE_VIDEO_DATA_FAILED)
            {
                result = this.ivsSdkClient.StopRealPlay(needToStopHandel);
                //如果不为0，表示停止原来的预览失败，只记录日志，不返回错误，不设置错误码
                if (result != CgwConst.IVS_SDK_SUCCESS_TAG)
                {
                    logEx.Error("Get a new preview success. But stop old preview failed.CameraNo:{0},Ivs sdk error code:{0}", cameraNo, result);
                    return err;
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
            logEx.Trace("Enter: IvsVideoMonitor.StopReceiveVideo({0}).", cameraNo);
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
                        logEx.Warn("Stop Receive camera video data failed. Don't need to end the preview.Camera No:{0}.", cameraNo);
                        //如果预览句柄不存在，不需要处理，直接返回
                        return err;
                    }
                }
                catch (Exception ex)
                {
                    err.SetErrorNo(CgwError.STOP_RECEIVE_VIDEO_FAILED);
                    logEx.Error("Stop Receive camera video data failed.Execption message:{0}", ex.Message);
                    return err;
                }
                finally
                {
                    this.handelOperateLock.ExitWriteLock();
                }
            }

            //调用sdk的停止方法，放在handelOperateLock外面，防止长时间占用锁
            if (needToStopHandel != CgwConst.START_RECEIVE_VIDEO_DATA_FAILED)
            {
                int result = this.ivsSdkClient.StopRealPlay(needToStopHandel);
                //如果不为0，表示预览失败
                if (result != CgwConst.IVS_SDK_SUCCESS_TAG)
                {
                    err.SetErrorNo(CgwError.STOP_RECEIVE_VIDEO_FAILED);
                    logEx.Error("Stop Receive camera video data failed.Ivs sdk error code:{0}", result);
                    return err;
                }
                logEx.Info("Stop Receive camera video data success.Camera No:{0},Handle:{1}.", cameraNo, needToStopHandel);
            }

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
            logEx.Trace("Enter: IvsVideoMonitor.StartControlPtz({0},{1},{2}).", cameraNo, ptzCommand, param);
            SmcErr err = new CgwError();

            //将ptz命令转换成IVS的命令
            string name = Enum.GetName(typeof(PtzCommandType), ptzCommand);
            if (Enum.IsDefined(typeof(IvsPtzCommandType), name))
            {
                IvsPtzCommandType ivsPtzCommandType = (IvsPtzCommandType)Enum.Parse(typeof(IvsPtzCommandType), name);

                logEx.Trace("Call ivsSdkClient.StartPtzControl({0},{1},{2}).", cameraNo, (int)ivsPtzCommandType, param);
                int result = this.ivsSdkClient.StartPtzControl(cameraNo, (int)ivsPtzCommandType, param.ToString());

                //如果为0，表示成功
                if (result == CgwConst.IVS_SDK_SUCCESS_TAG)
                {
                    logEx.Info("Start control ptz success.Camera No:{0}.", cameraNo);
                }
                else
                {
                    err.SetErrorNo(CgwError.START_CONTROL_PTZ_FAILED);
                    logEx.Error("Start control ptz failed.Camera No:{0}.Ivs sdk error code：{1}.", cameraNo, result);
                    return err;
                }

            }
            else
            {
                err.SetErrorNo(CgwError.START_CONTROL_PTZ_FAILED);
                logEx.Error("The ivs monitor is not support the command.ptzCommand:{0}.", ptzCommand);
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
            logEx.Trace("Enter: IvsVideoMonitor.StopControlPtz({0}).", cameraNo);
            SmcErr err = new CgwError();
            logEx.Trace("Call ivsSdkClient.StopPtzControl({0}).", cameraNo);
            int result = this.ivsSdkClient.StopPtzControl(cameraNo);
            //如果为0，表示成功
            if (result == CgwConst.IVS_SDK_SUCCESS_TAG)
            {
                logEx.Info("Stop control ptz success.Camera No:{0}.", cameraNo);
            }
            else
            {
                //直接将IVS的错误码返回
                err.SetErrorNo(CgwError.STOP_CONTROL_PTZ_FAILED);
                logEx.Error("Stop control ptz failed.Camera No:{0}.Ivs sdk error code：{1}.", cameraNo, result);
                return err;
            }
            return err;
        }

        /// <summary>
        /// 重发I帧（暂不支持）
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        public SmcErr MakeIFrame(string cameraNo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: IvsVideoMonitor.MakeIFrame({0}).", cameraNo);
            SmcErr err = new CgwError();
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
        /// 设置麦克风状态，非物理状态，通过软件控制，该状态只针对该融合网关。软件重启，状态丢失
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="isOn">麦克风是否开启</param>
        /// <returns></returns>
        public SmcErr SetMic(string cameraNo, bool isOn)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: IvsVideoMonitor.SetMic({0}，{1}).", cameraNo, isOn);
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
            logEx.Trace("Enter: IvsVideoMonitor.ClearCamera().");
            if (this.cameraOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    this.cameraList = new List<Camera>();
                    this.groupList = new List<CameraGroup>();
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
            logEx.Trace("Enter: IvsVideoMonitor.GetAllCamerasMethod().");
            try
            {
                //1、获取系统中所有的域
                List<IvsDomainRoute> ivsDomainRouteList;

                logEx.Trace("Call ivsSdkClient.GetDomainRoute().");
                int result = this.ivsSdkClient.GetDomainRoute(out ivsDomainRouteList);

                if (result == CgwConst.IVS_SDK_SUCCESS_TAG)
                {
                    logEx.Info("GetDomainRoute success.List count:{0}", ivsDomainRouteList.Count);
                }
                else
                {
                    logEx.Error("GetDomainRoute failed.Ivs sdk error code:{0}", result);
                    ClearCamera();
                    isRefreshSucess = false;
                    return;
                }

                List<Camera> cameraListTemp = new List<Camera>();
                List<CameraGroup> groupListTemp = new List<CameraGroup>();
                List<NodeRelation> nodeRelationListTemp = new List<NodeRelation>();
                Dictionary<string, NodeRelation> nodeRelationDicTemp = new Dictionary<string, NodeRelation>();

                foreach (IvsDomainRoute route in ivsDomainRouteList)
                {
                    //加上此判断条件的话，子域将不会作查目录处理，不合理，故注释掉
                    //if (route.ParentDomain != "")
                    //{
                    //    continue;
                    //}
                    Dictionary<string, IvsCameraGroup> ivsCameraGroupDic;

                    logEx.Trace("Call ivsSdkClient.GetDeviceGroupList({0},{1}，out groupCount, out ivsCameraGroupDic).",
                                route.DomainCode,
                                CgwConst.IVS_SDK_ROOTGROUP_TAG);

                    result = this.ivsSdkClient.GetDeviceGroupList(route.DomainCode,
                                                             CgwConst.IVS_SDK_ROOTGROUP_TAG,
                                                             out ivsCameraGroupDic);

                    if (result == CgwConst.IVS_SDK_SUCCESS_TAG)
                    {
                        logEx.Info("GetDeviceGroupList success.Current group count:{0}", ivsCameraGroupDic.Count);
                    }
                    else
                    {
                        logEx.Error("GetDeviceGroupList failed.Ivs sdk error code:{0}", result);
                        ClearCamera();
                        isRefreshSucess = false;
                        return;
                    }

                    //域也当做分组使用
                    string encodeDomainNo = CgwConst.IVS_SDK_ROOTGROUP_TAG + CgwConst.IVS_SDK_DOMAINCODE_SEPARATOR_TAG + route.DomainCode;
                    CameraGroup domainGroup = new CameraGroup(encodeDomainNo, route.DomainName);
                    groupListTemp.Add(domainGroup);

                    List<string> pathDomainList = new List<string>();
                    RecursionGroupPath(route.DomainCode, ivsDomainRouteList,ref pathDomainList);
                    NodeRelation nodeDomainRelation = new NodeRelation(encodeDomainNo, pathDomainList, CgwMonitorManage.Common.NodeType.GROUP);
                    nodeRelationDicTemp.Add(encodeDomainNo, nodeDomainRelation);

                    foreach (KeyValuePair<string, IvsCameraGroup> ivsCameraGroupKeyValue in ivsCameraGroupDic)
                    {
                        IvsCameraGroup group = ivsCameraGroupKeyValue.Value;
                        string encodeGroupNo = group.GroupNo + CgwConst.IVS_SDK_DOMAINCODE_SEPARATOR_TAG + group.DomainCode;
                        //添加组信息
                        CameraGroup cameraGroup = new CameraGroup(encodeGroupNo, group.GroupName);
                        groupListTemp.Add(cameraGroup);

                        List<string> pathList = new List<string>();
                        RecursionPath(group.GroupNo, ivsCameraGroupDic, ref pathList);

                        NodeRelation nodeRelation = new NodeRelation(encodeGroupNo, pathList, CgwMonitorManage.Common.NodeType.GROUP);
                        nodeRelationDicTemp.Add(encodeGroupNo, nodeRelation);
                    }
                }
                //添加所有组节点
                nodeRelationListTemp.AddRange(nodeRelationDicTemp.Values);

                List<IvsCamera> ivsCameraPageList = new List<IvsCamera>();
                int cameraCount = 0;
                logEx.Trace("Call ivsSdkClient.GetDeviceList");

                //查询第一页记录，同时获取摄像头个数
                result = this.ivsSdkClient.GetDeviceList(CgwConst.PAGE_FIRST_INDEX, CgwConst.PAGE_LAST_INDEX, out cameraCount, out ivsCameraPageList);

                List<IvsCamera> ivsCameraLeaveList = new List<IvsCamera>();
                //如果总记录大于一页的总记录数
                if (cameraCount > CgwConst.PAGE_LAST_INDEX)
                {
                    //一次将剩下所有记录查询出来
                    result = this.ivsSdkClient.GetDeviceList(CgwConst.PAGE_LAST_INDEX + 1, cameraCount, out cameraCount, out ivsCameraLeaveList);
                }

                if (result == CgwConst.IVS_SDK_SUCCESS_TAG)
                {
                    logEx.Info("GetDeviceList success.Current group count:{0}", cameraCount);
                    ivsCameraPageList.AddRange(ivsCameraLeaveList);
                }
                else
                {
                    logEx.Error("GetDeviceList failed.Ivs sdk error code:{0}", result);
                    ClearCamera();
                    isRefreshSucess = false;
                    return;
                }

                foreach (IvsCamera ivsCamera in ivsCameraPageList)
                {
                    Camera camera = new Camera(ivsCamera.No, ivsCamera.Name);
                    camera.Status = ivsCamera.Status;

                    List<string> cameraPathList = new List<string>();
                    //string encodeGroupNo = ivsCamera.GroupNo + CgwConst.IVS_SDK_DOMAINCODE_SEPARATOR_TAG + ivsCamera.DomainCode;//摄像头所属组号错误（与群组组号混淆了）。
                    string encodeGroupNo = ivsCamera.GroupNo;
                    if (nodeRelationDicTemp.ContainsKey(encodeGroupNo))
                    {
                        //如果自定义分组里面包含该摄像头的父节点，需要设置分组路径                    
                        cameraPathList.AddRange(nodeRelationDicTemp[encodeGroupNo].Path);
                        cameraPathList.Add(encodeGroupNo);
                    }

                    NodeRelation nodeRelation = new NodeRelation(camera.No, cameraPathList, CgwMonitorManage.Common.NodeType.CAMERA);

                    //解决问题单DTS2013080201001，规避因IVS服务器存在摄像头重复的bug导致融合网关异常的问题
                    if (!nodeRelationDicTemp.ContainsKey(camera.No))
                    {
                        cameraListTemp.Add(camera);
                        nodeRelationDicTemp.Add(camera.No, nodeRelation);
                    }

                    nodeRelationListTemp.Add(nodeRelation);
                }
                //nodeRelationListTemp.AddRange(nodeRelationDicTemp.Values);

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
                //将实时获取的值放到缓存
                if (this.cameraOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                {
                    try
                    {
                        this.cameraList = cameraListTemp;
                        this.groupList = groupListTemp;
                        this.nodeRelationList = nodeRelationListTemp;
                        isRefreshSucess = true;
                    }
                    catch (Exception ex)
                    {
                        isRefreshSucess = false;
                        logEx.Error("Set the list to the buffer failed. ", ex.Message);
                    }
                    finally
                    {
                        this.cameraOperateLock.ExitWriteLock();
                    }
                }
                else
                {
                    isRefreshSucess = false;
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("GetAllCamerasMethod failed.Exception message:{0}", ex.Message);
                isRefreshSucess = false;
            }
        }
        /// <summary>
        /// 获取摄像头列表及分组信息定时器
        /// 1、获取系统中所有的域
        /// 2、循环所有的域，查询域下面的分组，递归处理，获取节点关系
        /// 3、查询设备列表
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GetAllCamerasTimer(object sender, System.Timers.ElapsedEventArgs e)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: IvsVideoMonitor.GetAllCamerasTimer().");

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
        /// 递归处理，获取节点路径
        /// 通过当前节点，递归逐级获取父节点，将父节点编号插入到list前面
        /// </summary>
        /// <param name="groupNo">组编号</param>
        /// <param name="ivsCameraGroupDic">所有分组列表</param>
        /// <param name="pathList">节点路径</param>
        private void RecursionPath(string groupNo, Dictionary<string, IvsCameraGroup> ivsCameraGroupDic, ref List<string> pathList)
        {
            IvsCameraGroup group = ivsCameraGroupDic[groupNo];

            //如果父节点是根节点，结束递归
            if (CgwConst.IVS_SDK_ROOTGROUP_TAG.Equals(group.ParentGroupNo))
            {
                //增加域为顶级组
                pathList.Insert(0, CgwConst.IVS_SDK_ROOTGROUP_TAG + CgwConst.IVS_SDK_DOMAINCODE_SEPARATOR_TAG + group.DomainCode);
                return;
            }
            else
            {
                //插入父节点
                pathList.Insert(0, group.ParentGroupNo + CgwConst.IVS_SDK_DOMAINCODE_SEPARATOR_TAG + group.DomainCode);
                //如果为非根节点，需要递归，获取节点层级路径
                RecursionPath(group.ParentGroupNo, ivsCameraGroupDic, ref pathList);
            }

        }

        /// <summary>
        /// 递归处理，循环加入设备域
        /// </summary>
        /// <param name="DomainCode"></param>
        /// <param name="ivsDomainRouteList"></param>
        /// <param name="pathDomainList"></param>
        private void RecursionGroupPath(string DomainCode, List<IvsDomainRoute> ivsDomainRouteList, ref List<string> pathDomainList)
        {
            IvsDomainRoute domainRoute=ivsDomainRouteList.Find((x) =>
            {
                return x.DomainCode == DomainCode;
            });
            if (domainRoute != null)
            {
                if (string.IsNullOrEmpty(domainRoute.ParentDomain))
                {
                    return;
                }
                else
                {
                    pathDomainList.Insert(0, CgwConst.IVS_SDK_ROOTGROUP_TAG + CgwConst.IVS_SDK_DOMAINCODE_SEPARATOR_TAG + domainRoute.ParentDomain);
                    RecursionGroupPath(domainRoute.ParentDomain, ivsDomainRouteList, ref pathDomainList);
                }
            }
        }

        /// <summary>
        /// 码流回调函数,
        /// 1、根据预览句柄handle从videoHandleCameraDic中获取摄像头编号，
        /// 2、根据摄像头编号，获取mic状态
        /// 3、判断码流是否是音频，如果是音频，根据mic状态判断是否需要转发
        /// </summary>
        /// <param name="handle">播放句柄</param>
        /// <param name="pRawFrameInfo">实况参数</param>
        /// <param name="pBuf">码流</param>
        /// <param name="uiBufSize">大小</param>
        /// <param name="cameraNo">摄像头编号</param>
        private void IvsRealPlayCallBackRaw(int handle, IvsRawFrameInfo pRawFrameInfo, byte[] pBuf, UInt32 uiBufSize, string pUserData)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            //logEx.Trace("Enter:IvsRealPlayCallBackRaw(),{0}", pUserData);

            MediaDataSender mediaDataSender = null;
            try
            {
                if (this.handelOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                {
                    try
                    {
                        if (!this.videoHandleCameraDic.ContainsKey(handle))
                        {
                            logEx.Error("The video data handle is not found.Handle:{0}", handle);
                            return;
                        }
                        mediaDataSender = this.videoHandleCameraDic[handle];
                    }
                    finally
                    {
                        this.handelOperateLock.ExitReadLock();
                    }
                }

                if (mediaDataSender == null)
                {
                    return;
                }

                StreamType streamType = StreamType.VIDEO_H264;
                //判断码流类型，因为码流回调访问频频非常高，不单独抽成一个方法，减少方法访问开销
                //对于支持的码流类型，用break退出switch，对于不支持的码流类型直接舍弃，用return返回
                switch (pRawFrameInfo.StreamType)
                {
                    //对于音频只接收G711A和G711U，其他舍弃
                    case (int)IvsStreamType.PAY_LOAD_TYPE_PCMU:
                        streamType = StreamType.AUDIO_G711U;
                        break;
                    case (int)IvsStreamType.PAY_LOAD_TYPE_PCMA:
                        streamType = StreamType.AUDIO_G711A;
                        break;

                    //只接收H264的视频码流
                    case (int)IvsStreamType.PAY_LOAD_TYPE_H264:
                        //H264的标准视频流，作为视频流处理
                        streamType = StreamType.VIDEO_H264;
                        break;
                    default:
                        //不支持的类型,直接舍弃，返回
                        //logEx.Warn("This stream type is not support. Chuck the data.StreamType:{0};Camera no:{1}",
                        //            Enum.GetName(typeof(IvsStreamType), pRawFrameInfo.StreamType),
                        //            cameraNo);
                        return;
                }

                if (streamType == StreamType.AUDIO_G711A || streamType == StreamType.AUDIO_G711U)
                {
                    //如果是音频流，需要判断mic的状态，开启时才发送音频流
                    if (this.micOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                    {
                        try
                        {
                            if (this.cameraMicStatusDic.ContainsKey(mediaDataSender.CameraNo))
                            {
                                //如果mic为非开启状态，则不发送音频码流,
                                if (!this.cameraMicStatusDic[mediaDataSender.CameraNo])
                                {
                                    //logEx.Warn("This data is audio,but the mic is off.Chuck the data.Camera no:{0}", mediaDataSender.CameraNo);
                                    return;
                                }
                            }
                            else
                            {
                                //默认为关闭状态，因此如果cameraMicStatusDic不包含该摄像头，则认为处于关闭状态，舍弃音频码流
                                //logEx.Warn("This data is audio,but the mic is off.Chuck the data.Camera no:{0}", mediaDataSender.CameraNo);
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
                mediaData.Data = pBuf;
                //IVS 目前为裸码流
                mediaData.DataType = MediaDataType.FRAME_DATA;
                mediaData.StreamType = streamType;
                mediaData.Size = uiBufSize;

                //将Ivs帧类型转换成各融合网关统一的帧类型
                string name = Enum.GetName(typeof(IvsH264NaluType), pRawFrameInfo.FrameType);

                //帧类型判断，偶现IVS返回的帧类型，在融合网关中未记录，返回值为null
                if (string.IsNullOrEmpty(name))
                {
                    logEx.Warn("IvsRealPlayCallBackRaw FrameType Is Not Defined ,FrameType:{0}", pRawFrameInfo.FrameType);
                    mediaData.FrameType = FrameDataType.H264_NALU_TYPE_UNDEFINED;
                }
                else
                {
                    if (Enum.IsDefined(typeof(FrameDataType), name))
                    {
                        FrameDataType frameDataType = (FrameDataType)Enum.Parse(typeof(FrameDataType), name);
                        mediaData.FrameType = frameDataType;
                    }
                    else
                    {
                        mediaData.FrameType = FrameDataType.H264_NALU_TYPE_UNDEFINED;
                    }
                }            

                //向回调函数转发码流
                //this.videoDataCallBack(cameraNo, mediaData, this.sender);

                //logEx.Debug("FrameDataCallBackFun.mediaData.DataType={0},FrameType = {1},StreamType = {2},Size = {3}", Enum.GetName(typeof(MediaDataType), mediaData.DataType),
                //         Enum.GetName(typeof(FrameDataType), mediaData.FrameType), Enum.GetName(typeof(StreamType), mediaData.StreamType), mediaData.Size);
                mediaDataSender.SendData(mediaData, this.sender);
            }
            catch (Exception e)
            {
                logEx.Error("Send the media data failed.Execption message:{0}", e.ToString());
            }
        }


        /// <summary>
        /// 刷新监控摄像头列表
        /// </summary>
        /// <returns></returns>
        public SmcErr RefreshMonitorCamera()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: IvsVideoMonitor.RefreshMonitorCamera");
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
            logEx.Trace("Enter: IvsVideoMonitor.GetRefreshStatus");
            SmcErr err = new CgwError();
            refreshStatus = new SmcErr();
            refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END;

            if (refreshMonitorCameraOperateLock.TryEnterWriteLock(CgwConst.EFRESH_MONITOR_CAMERA_WAIT_TIME))
            {
                try
                {
                    if (loadResult)
                    {
                        refreshStatus.ErrNo = isRefreshSucess ? CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END : CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED;
                    }
                    else
                    {
                        refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_EXECUTING;
                    }
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

        /// <summary>
        /// sdk回调函数，用于重连
        /// </summary>
        /// <param name="iEventType">事件类型</param>
        /// <param name="pEventBuf"> 数据</param>
        /// <param name="uiBufSize">buf长度</param>
        /// <param name="pUserData">用户数据，自定义，相应回调函数被调用时返回该参数</param>
        public void IvsEventCallBackFunc(int iEventType, IvsUserOfflineOff pEventBuf, UInt32 uiBufSize, string pUserData)
        {
            if (10019 == iEventType || 10301 == iEventType)//用户下线通知、客户端登录失败
            {
                NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

                IvsLoginInfo loginInfo = new IvsLoginInfo();
                loginInfo.Ip = ip;
                loginInfo.Port = int.Parse(port);
                loginInfo.User = user;
                //loginInfo.Pwd = password;
                loginInfo.Pwd = CommonFunction.DecryptByte2Str(pwdByte,CgwConst.PASSWORD_TAG);

                logEx.Info("IvsEventCallBackFunc.user ={0}.", user);
                //使用线程轮询加载监控平台
                Thread loaderThread = new Thread(LoginThread);
                loaderThread.IsBackground = true;
                loaderThread.Start(loginInfo);
            }
        }
    }
}
