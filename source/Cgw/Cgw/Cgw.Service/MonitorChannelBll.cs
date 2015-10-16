using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using Cgw.SmcError;
using System.Timers;
using Cgw.Common;
using Cgw.Channel;
using LicenseCLR;
using Cgw.NLogEx;
using System.Text;
using System.IO;
using System.Threading;
using Cgw.MonitorManageAdapter;
using System.ServiceProcess;
using Smc20.Common.Public;


namespace Cgw.Service
{
    public class MonitorChannelBll
    {
        MonitorServiceControl serviceControl = new MonitorServiceControl();

        /// <summary>
        /// MonitorChannelBll实例
        /// </summary>
        private static MonitorChannelBll instance;

        /// <summary>
        /// 监控通道实例化
        /// </summary>
        private MonitorChannelRelationManager manageChannel;

        /// <summary>
        /// 记录日志操作类.
        /// </summary>
        public static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// License模块加载线程
        /// </summary>
        private System.Threading.Thread licenseThread;

        /// <summary>
        /// lisence中融合网关通道个数
        /// </summary>
        private int licCount = 0;

        /// <summary>
        /// lisence失效文件路径
        /// </summary>
        private string strPathPersistent = AppDomain.CurrentDomain.BaseDirectory.ToString() + "persistentSafe.dat";

        /// <summary>
        /// lisence路径
        /// </summary>
        private string strPathLic = AppDomain.CurrentDomain.BaseDirectory.ToString() + "license_smc.dat";

        /// <summary>
        /// 文件监视类
        /// </summary>
        private FileSystemWatcher fileWatcher;

        /// <summary>
        /// 定时器器，用于定时连接监控平台
        /// </summary>
        private Thread connectMonitorManageServiceThread = null;

        /// <summary>
        /// 定时器器，用于保活接监控平台
        /// </summary>
        private System.Timers.Timer monitorManageServiceKeepLiveTimer = new System.Timers.Timer();

        /// <summary>
        /// license_smc操作锁
        /// </summary>
        private ReaderWriterLockSlim licenseFileOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 需要重新连接监控服务
        /// </summary>
        private bool isNeedConnect = true;

        /// <summary>
        /// 需要开始保活处理
        /// </summary>
        private bool isNeedKeepAliver = false;

        /// <summary>
        /// 是否有监控平台、应急平台权限
        /// </summary>
        private PlatformLicenseInfo licenseInfo = new PlatformLicenseInfo();

        /// <summary>
        /// MonitorChannelBll 单实例
        /// </summary>
        /// <returns></returns>
        public static MonitorChannelBll Instance()
        {
            if (instance == null)
            {
                instance = new MonitorChannelBll();
            }
            return instance;
        }

        /// <summary>
        /// 
        /// </summary>
        private MonitorChannelBll()
        {

        }

        /// <summary>
        /// 加载监控平台，加载通道管理模块，加载license管理模块
        /// </summary>
        /// <returns></returns>
        public SmcErr Load()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new SmcErr();

            //开启连接监控平台线程
            this.connectMonitorManageServiceThread = new Thread(new ThreadStart(ConnectMonitorManageServiceTimer));
            this.connectMonitorManageServiceThread.Start();
            logEx.Info("ConnectMonitorManageServiceTimer Run");
            //初始话定时器
            this.monitorManageServiceKeepLiveTimer.AutoReset = true;
            this.monitorManageServiceKeepLiveTimer.Elapsed += new ElapsedEventHandler(MonitorManageServiceKeepLiveTimer_Elapsed);
            //保活时间间隔，5秒
            this.monitorManageServiceKeepLiveTimer.Interval = CgwConst.MONITOR_MANAGE_KEEPLIVE_WAIT_TIME;
            this.monitorManageServiceKeepLiveTimer.Start();

            manageChannel = new MonitorChannelRelationManager();
            try
            {
                IPAddressEx ip = new IPAddressEx(ConfigSettings.CgwServiceIp);
            }
            catch (System.Exception ex)
            {
                logEx.Error("CgwService configuration files is error ,The IP address  format  is error,Please Check this configuration files! "+ex.ToString());
                err.SetErrorNo(CgwError.ERR_CGW_CONFIGURE_FORMAT);
            }
            if (err.IsSuccess())
            {
                InitSotParam param = new InitSotParam();
                param.cgwIp = ConfigSettings.CgwServiceIp;
                param.audioType = ConfigSettings.CgwAudioProtocol;
                param.channelCallPort = ConfigSettings.CgwChanelCallPort;
                param.rtpStartPort = ConfigSettings.CgwChannelRTPStartPort;
                param.rtpPortCount = ConfigSettings.CgwChannelRTPPortCount;
                err = manageChannel.Init(param);
            }

            if (err.IsSuccess())
            {
                logEx.Info("CgwServiceLoad Successful !");
            }
            else
            {
                logEx.Error("CgwServiceLoad failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 停止管道中所有的流
        /// </summary>
        /// <returns></returns>
        public SmcErr StopAllStream()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new SmcErr();
            logEx.Info("Enter:CgwService UnLoad:");
            manageChannel.StopChannelData();
            return err;
        }

        /// <summary>
        /// Disconnect monitorPlat
        /// </summary>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr Disconnect()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new SmcErr();

            //2015/2/13  停止服务，将状态重新赋值
            isNeedKeepAliver = false;
            isNeedConnect = false;
            logEx.Info("Bll.Disconnect set isNeedKeepAliver=false,isNeedConnect=false.");

            err = CgwMonitorManageAdapter.Instance().Disconnect();
            if (err.IsSuccess())
            {
                logEx.Info("Bll.Disconnect Successful !");
            }

            return err;
        }

        /// <summary>
        /// 保活监控平台服务
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void MonitorManageServiceKeepLiveTimer_Elapsed(object sender, ElapsedEventArgs e)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new SmcErr();
            try
            {
                if (isNeedKeepAliver)
                {
                    //Connect Monitor
                    if (!serviceControl.MonitorServiceRun())
                    {
                        isNeedConnect = true;

                        //2015/2/13 Monitor服务不在线，将KeepAliver置为false，避免线程不断保活，待测试
                        isNeedKeepAliver = false;
                        err.SetErrorNo(CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                    }
                    else
                    {
                        err = CgwMonitorManageAdapter.Instance().KeepAliver();
                    }
                    if (err.IsSuccess())
                    {
                        logEx.Trace("MonitorManageServiceKeepLive Successful !");
                    }
                    else
                    {
                        isNeedConnect = true;
                        isNeedKeepAliver = false;
                        logEx.Warn("MonitorManageServiceKeepLive Failed !");
                    }
                }
            }
            catch (System.Exception ex)
            {
                isNeedConnect = true;
                isNeedKeepAliver = false;
                logEx.Error("MonitorManageServiceKeepLive.Exception is {0}", ex.ToString());
            }

        }

        /// <summary>
        /// 连接监控通道
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void ConnectMonitorManageServiceTimer()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new SmcErr();

            while (true)
            {
                try
                {
                    if (isNeedConnect)
                    {
                        //err = CgwMonitorManageAdapter.Instance().Connect(CommonFunction.SHA256Encrypt(ConfigSettings.ConnectMonitorManagePassword));
                        err = CgwMonitorManageAdapter.Instance().Connect(null);

                        if (err.IsSuccess())
                        {
                            logEx.Info("Load MonitorManageService Successful !");
                            isNeedConnect = false;
                            isNeedKeepAliver = true;

                            Thread th = new Thread(new ThreadStart(() =>
                            {
                                while (true)
                                {
                                    //建立连接后,需要查看监控通道关系中是否存在开启的视频,如果存在,需要重新打开视频
                                    SmcErr refreshStatus = new SmcErr();
                                    GetRefreshStatus(out refreshStatus);

                                    if (refreshStatus.ErrNo == Cgw.SmcError.CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END)
                                    {
                                        Thread.Sleep(1500);
                                        manageChannel.ReChannelVideo();
                                        break;
                                    }
                                    Thread.Sleep(1000);
                                }
                            }));
                            th.Start();
                        }
                    }
                }
                catch (System.Exception ex)
                {
                    logEx.Error("Load MonitorManageService.Exception = {0}", ex.ToString());
                }
                finally
                {
                    Thread.Sleep(CgwConst.CONNECT_MONITOR_MANAGE_WAIT_TIME);
                }
            }
        }

        /// <summary>
        /// License初始化
        /// </summary>
        public void LicenseInit()
        {
            fileWatcher = new FileSystemWatcher();
            fileWatcher.Path = AppDomain.CurrentDomain.BaseDirectory.ToString();
            fileWatcher.Filter = "license_smc.dat";
            fileWatcher.Changed += new FileSystemEventHandler(fileWatcher_Changed);

            fileWatcher.Created += new FileSystemEventHandler(fileWatcher_Created);
            fileWatcher.Deleted += new FileSystemEventHandler(fileWatcher_Deleted);

            fileWatcher.EnableRaisingEvents = true;
            licenseThread = new System.Threading.Thread(new System.Threading.ThreadStart(LoadLicenseInfo));
            licenseThread.Start();

        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void fileWatcher_Created(object sender, FileSystemEventArgs e)
        {
            if (this.licenseFileOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                LoggerEx logEx = new LoggerEx(log);
                try
                {
                    fileWatcher.EnableRaisingEvents = false;
                    if (e.ChangeType == WatcherChangeTypes.Created)
                    {
                        uint resultErr = 0;
                        byte[] bteLic;
                        if (File.Exists(strPathLic))
                        {
                            Thread.Sleep(1000);
                            bteLic = File.ReadAllBytes(strPathLic);

                        }
                        else
                        {
                            byte[] b = new byte[0];
                            bteLic = b;
                        }
                        CLicenseManagedAdapter licMan = new CLicenseManagedAdapter();
                        SmcErr smcErr = Convert2ErrNo(licMan.ActivateLicenseKey(bteLic, ref resultErr));
                    }
                    fileWatcher.EnableRaisingEvents = true;
                }
                catch (Exception ex)
                {
                    logEx.Error("fileWatcher_Created Exception:{0}", ex.ToString());
                }
                finally
                {
                    this.licenseFileOperateLock.ExitWriteLock();
                }
            }
        }
        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void fileWatcher_Deleted(object sender, FileSystemEventArgs e)
        {
            if (this.licenseFileOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                LoggerEx logEx = new LoggerEx(log);
                try
                {
                    fileWatcher.EnableRaisingEvents = false;
                    if (e.ChangeType == WatcherChangeTypes.Deleted)
                    {
                        uint resultErr = 0;
                        byte[] bteLic;
                        if (File.Exists(strPathLic))
                        {
                            bteLic = File.ReadAllBytes(strPathLic);

                        }
                        else
                        {
                            byte[] b = new byte[0];
                            bteLic = b;
                            int licCountNew = 0;
                            if (licCount != licCountNew)
                            {
                                SessionManage.Instance().RemoveAllSession();
                                logEx.Trace("License have changed! the New Count is:{0}, the Old Count:{1} ", licCountNew.ToString(), licCount.ToString());
                                licCount = licCountNew;
                            }
                        }
                        CLicenseManagedAdapter licMan = new CLicenseManagedAdapter();
                        SmcErr smcErr = Convert2ErrNo(licMan.ActivateLicenseKey(bteLic, ref resultErr));

                    }
                    fileWatcher.EnableRaisingEvents = true;
                }

                catch (Exception ex)
                {
                    logEx.Error("fileWatcher_Deleted Exception:{0}", ex.ToString());
                }
                finally
                {
                    this.licenseFileOperateLock.ExitWriteLock();
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void fileWatcher_Changed(object sender, FileSystemEventArgs e)
        {
            if (this.licenseFileOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                LoggerEx logEx = new LoggerEx(log);
                try
                {
                    fileWatcher.EnableRaisingEvents = false;
                    try
                    {
                        if (e.ChangeType == WatcherChangeTypes.Changed)
                        {
                            uint resultErr = 0;

                            byte[] bteLic;
                            if (File.Exists(strPathLic))
                            {
                                Thread.Sleep(1000);
                                FileStream files = File.Open(strPathLic, FileMode.Open, FileAccess.Read, FileShare.Read);
                                if (files.Length != 0)
                                {
                                    byte[] btemp = new byte[files.Length];
                                    files.Read(btemp, 0, btemp.Length);
                                    bteLic = btemp;
                                }
                                else
                                {
                                    bteLic = new byte[0];
                                }
                                files.Close();
                            }
                            else
                            {
                                byte[] b = new byte[0];
                                bteLic = b;
                            }
                            CLicenseManagedAdapter licMan = new CLicenseManagedAdapter();
                            SmcErr smcErr = Convert2ErrNo(licMan.ActivateLicenseKey(bteLic, ref resultErr));

                        }
                        GetControlLicItem(null);
                    }
                    catch (System.Exception ex)
                    {
                        logEx.Error("fileWatcher_Changed Exception:{0}", ex.ToString());
                    }
                    finally
                    {
                        fileWatcher.EnableRaisingEvents = true;
                    }

                }
                finally
                {
                    this.licenseFileOperateLock.ExitWriteLock();
                }
            }
        }
        #region License模块读取
        /// <summary>
        /// 调用托管方法
        /// </summary>
        /// <returns></returns>
        public void LoadLicenseInfo()
        {
            SmcErr smcErr = new SmcErr();
            CLicenseManagedAdapter LicMan = new CLicenseManagedAdapter();
            ProcLMMsgFunc CallFunc = new ProcLMMsgFunc(this.ReceiveLicMsg);

            smcErr = Convert2ErrNo(LicMan.AddCallbackToLM(CallFunc));

            uint i = LicMan.ManagerLicenseInit(SMCConst.LICENSE_KEY_VALUES, LicProductKeyType.LicSMC20ProductKEY);
            smcErr = Convert2ErrNo(i);
        }

        /// <summary>
        /// 把License错误码转换成Smc错误码
        /// </summary>
        /// <param name="errNo"></param>
        /// <returns></returns>
        private SmcErr Convert2ErrNo(uint errNo)
        {
            LicenseError err = new LicenseError(errNo);
            return err;
        }

        /// <summary>
        /// 读取license信息
        /// </summary>
        /// <param name="licMsg"></param>
        /// <returns></returns>
        public uint ReceiveLicMsg(ref LicMsg licMsg)
        {
            CLicenseManagedAdapter licMan = new CLicenseManagedAdapter();
            SmcErr smcErr = new SmcErr();
            LoggerEx logEx = new LoggerEx(log);

            if (null == licMsg)
            {
                smcErr.ErrNo = SmcErr.SMC_INVALID_PARAM;
                logEx.Warn("Perform ReceiveLicMsg's licMsg is null msg!");
                return smcErr.ErrNo;
            }
            else
            {
                logEx.Trace("ReceiveLicMsg licMsg msg: {0}", licMsg.GetType());
            }

            if (LicCallBackType.LicLKReadRevokeTicket == licMsg.MsgID)
            {
                LicMsg license = new LicMsg();
                if (LicPersisFileType.LicPersistentSafemem == licMsg.FileType)
                {

                    if (File.Exists(strPathPersistent))
                    {
                        license.MsgParam = File.ReadAllBytes(strPathPersistent);
                        logEx.Info("LoadLicPersistentSafemem is successful!");
                    }
                    else
                    {
                        //string str = "0x";
                        //byte[] b = Encoding.Default.GetBytes(str);
                        byte[] b = new byte[0];
                        license = new LicMsg();
                        license.MsgParam = b;
                    }

                }
                else if (LicPersisFileType.LicPersistentInfo == licMsg.FileType)
                {
                    if (this.licenseFileOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                    {
                        try
                        {
                            try
                            {

                                if (File.Exists(strPathLic))
                                {

                                    license.MsgParam = File.ReadAllBytes(strPathLic);
                                    logEx.Info("LoadLicenseInfo is successful!");
                                }
                                else
                                {
                                    byte[] b = new byte[0];
                                    license = new LicMsg();
                                    license.MsgParam = b;
                                }
                            }
                            catch (System.Exception ex)
                            {
                                logEx.Error(ex, "LicLKReadRevokeTicket faild!");
                                //记录系统加载License失败
                                SmcErr recordErr = new SmcErr(SmcErr.LICENSE_LOAD_ERR);
                                recordErr.ErrorInfoParam = new ErrorInfo();
                                recordErr.ErrorInfoParam.AddSmcErrInfo(smcErr);

                                logEx.Error(recordErr);
                            }
                        }
                        finally
                        {
                            this.licenseFileOperateLock.ExitWriteLock();
                        }
                    }

                }
                licMsg = license;
            }
            //License变更
            else if (LicCallBackType.LicLKStateChange == licMsg.MsgID)
            {
                GetControlLicItem(null);
            }
            //保存Licsense失效信息
            else if (LicCallBackType.LicLKWriteRevokeTicket == licMsg.MsgID)
            {
                File.WriteAllBytes(strPathPersistent, licMsg.MsgParam);
            }

            return smcErr.ErrNo;
        }
        /// <summary>
        /// 获取控制项信息
        /// </summary>
        /// <param name="err"></param>
        /// <param name="licMan"></param>
        private SmcErr GetControlLicItem(List<ConfigItem> configList)
        {
            LoggerEx logEx = new LoggerEx(log);
            SmcErr smcErr = new SmcErr();
            CLicenseManagedAdapter licMan = new CLicenseManagedAdapter();
            List<LKItemValue> licItemTypeList = new List<LKItemValue>();
            smcErr = Convert2ErrNo(licMan.GetLicenseInfo(ref licItemTypeList));
            if (smcErr.IsSuccess())
            {
                licenseInfo.IsMonitorPlatform = false;
                licenseInfo.IsEltePlatform = false;
                foreach (LKItemValue item in licItemTypeList)
                {
                    if (LicItemfType.LicItemMaxChannelNum == item.ItemID)
                    {
                        int licCountNew = Convert.ToInt32(item.ItemValue);
                        if (licCount != licCountNew)
                        {

                            SessionManage.Instance().RemoveAllSession();
                            logEx.Trace("License have changed! the New Count is:{0}, the Old Count:{1} ", licCountNew.ToString(), licCount.ToString());
                            licCount = licCountNew;
                        }
                    }
                    if (item.ItemID.ToString() == "LicItemMonitorEnable" && item.ItemValue.ToString() == "1")
                    {
                        licenseInfo.IsMonitorPlatform = true;
                    }
                    if (item.ItemID.ToString() == "LicItemELTEEnable" && item.ItemValue.ToString() == "1")
                    {
                        licenseInfo.IsEltePlatform = true;
                    }

                    logEx.Info("GetControlLicItem item:{0},{1}", item.ItemID, item.ItemValue);
                }
            }
            else
            {
                logEx.Warn("GetControlLicItem errNo:{0}", smcErr.ErrNo);
            }

            return smcErr;
        }
        #endregion
        /// <summary>
        /// 分配监控通道
        /// </summary>
        /// <param name="ChannelInfoList"></param>
        /// <returns></returns>
        public SmcErr AssignMonitorChannel(List<Common.ChannelInfo> ChannelInfoList)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new SmcErr();
            if (serviceControl.MonitorServiceRun())
            {
                err = manageChannel.AssignChannel(ChannelInfoList);
            }
            else
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
            }

            //日志记录
            if (err.IsSuccess())
            {
                logEx.Trace("AssignMonitorChannel Successful");
            }
            else
            {
                logEx.Error("AssignMonitorChannel failed,ErrNo :{0}", err.ErrNo);
            }
            return err;

        }

        /// <summary>
        /// 同步监控通道
        /// </summary>
        /// <param name="ChannelInfoList"></param>
        /// <returns></returns>
        public SmcErr SyncMonitorChannel(ref List<Common.ChannelInfo> ChannelInfoList)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new SmcErr();
            //if (serviceControl.MonitorServiceRun())
            //{
                err = manageChannel.SyncChannelInfo(ref ChannelInfoList, licCount);
            //}
            //else
            //{
            //    err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
            //}
            //日志记录
            if (err.IsSuccess())
            {
                logEx.Trace("SyncMonitorChannel Successful");
            }
            else
            {
                logEx.Error("SyncMonitorChannel failed,ErrNo :{0}", err.ErrNo);
            }
            return err;

        }

        /// <summary>
        /// 获取监控设备列表,返回摄像头基本信息，组基本信息，所属分组关系信息
        /// </summary>
        public SmcErr QueryMonitorCamera(Cgw.Common.PlatformType platformType, Cgw.Common.PageParam pageParam, out List<Common.Camera> CameraInfoList, out List<Common.CameraGroup> CameraGroupList, out List<Common.NodeRelation> nodeRelationList, out Cgw.Common.PagesInfo pagesInfo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new SmcErr();

            CameraInfoList = null;
            CameraGroupList = null;
            nodeRelationList = null;
            pagesInfo = null;
            if (serviceControl.MonitorServiceRun())
            {
                try
                {
                    err = CgwMonitorManageAdapter.Instance().GetAllCameras(platformType, pageParam, out CameraInfoList, out CameraGroupList, out nodeRelationList, out pagesInfo, licenseInfo);
                }
                catch (Exception ex)
                {
                    logEx.Trace("QueryMonitorCamera.Exception = {0}", ex.ToString());
                }
            }
            else
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
            }
            //日志记录
            if (err.IsSuccess())
            {
                logEx.Trace("QueryMonitorCamera Successful");
            }
            else
            {
                logEx.Error("QueryMonitorCamera failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 设置监控通道视频源
        /// </summary>
        /// <param name="channelLabel">通道label</param>
        /// <param name="cameraNo">摄像头NO</param>
        /// <returns></returns>
        public SmcErr SwitchChannelVideoSource(string channelLabel, string cameraNo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new SmcErr();
            if (serviceControl.MonitorServiceRun())
            {
                if (!string.IsNullOrEmpty(channelLabel))
                {
                    //切换通道关联
                    err = manageChannel.SwitchChannelRelation(channelLabel, cameraNo);

                }
                else
                {
                    //监控通道号不能为空
                    err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_CHANNELNO_NULL);
                }
            }
            else
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
            }

            //日志记录
            if (err.IsSuccess())
            {
                logEx.Trace("SwitchChannelVideoSource Successful");
            }
            else
            {
                logEx.Error("SwitchChannelVideoSource failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 开启PTZ控制
        /// </summary>
        /// <param name="cameraNo">摄像头NO</param>
        /// <param name="ptzCommand">操作类型</param>
        /// <param name="param">命令参数</param>
        /// <returns></returns>
        public SmcErr StartControlPtz(string cameraNo, Common.PtzCommandType ptzCommand, int param)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new CgwError();

            if (serviceControl.MonitorServiceRun())
            {
                err = CgwMonitorManageAdapter.Instance().StartControlPtz(cameraNo, ptzCommand, param);

            }
            else
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
            }


            //日志记录
            if (err.IsSuccess())
            {
                logEx.Trace("StartControlPtz Successful");
            }
            else
            {
                logEx.Error("StartControlPtz failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 关闭PTZ控制,
        /// </summary>
        /// <param name="Camerano">摄像头NO.</param>
        /// <returns></returns>
        public SmcErr StopControlPtz(string cameraNo, Common.PtzCommandType ptzCommandType)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new CgwError();
            if (serviceControl.MonitorServiceRun())
            {
                err = CgwMonitorManageAdapter.Instance().StopControlPtz(cameraNo, ptzCommandType);

            }
            else
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
            }
            //日志记录
            if (err.IsSuccess())
            {
                logEx.Trace("StopControlPtz Successful");
            }
            else
            {
                logEx.Error("StopControlPtz failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="channelLabel"></param>
        /// <param name="cameraNo"></param>
        /// <param name="isOn"></param>
        /// <returns></returns>
        public SmcErr SetMic(string channelLabel, string cameraNo, bool isOn)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new CgwError();
            if (serviceControl.MonitorServiceRun())
            {
                err = CgwMonitorManageAdapter.Instance().SetMic(cameraNo, isOn);

            }
            else
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
            }
            //日志记录
            if (err.IsSuccess())
            {
                logEx.Trace("SetMic Successful");
            }
            else
            {
                logEx.Error("SetMic failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }


        public SmcErr SetSpeaker(string channelLabel, string cameraNo, bool isOn)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new CgwError();
            if (serviceControl.MonitorServiceRun())
            {
                err = CgwMonitorManageAdapter.Instance().SetSpeaker(cameraNo, isOn);
            }
            else
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
            }
            //日志记录
            if (err.IsSuccess())
            {
                logEx.Trace("SetMic Successful");
            }
            else
            {
                logEx.Error("SetMic failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 刷新监控摄像头列表
        /// </summary>
        /// <returns></returns>
        public SmcErr RefreshMonitorCamera()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new CgwError();

            if (serviceControl.MonitorServiceRun())
            {
                err = CgwMonitorManageAdapter.Instance().RefreshMonitorCamera();
            }
            else
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
            }
            //日志记录
            if (err.IsSuccess())
            {
                logEx.Trace("RefreshMonitorCamera Successful");
            }
            else
            {
                logEx.Error("RefreshMonitorCamera failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 获取监控摄像头列表刷新状态，返回结果为0是表示刷新完毕，为1是刷新操作中。当查询刷新状态为0时，可调用获取监控摄像头列表接口，获取刷新后监控摄像头列表
        /// </summary>
        /// <param name="refreshStatus"></param>
        /// <returns></returns>
        public SmcErr GetRefreshStatus(out SmcErr refreshStatus)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new CgwError();
            refreshStatus = new SmcErr();
            refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END;
            if (serviceControl.MonitorServiceRun())
            {
                err = CgwMonitorManageAdapter.Instance().GetRefreshStatus(out refreshStatus);
            }
            else
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
            }
            //日志记录
            if (err.IsSuccess())
            {
                logEx.Trace("GetRefreshStatus Successful");
            }
            else
            {
                logEx.Error("GetRefreshStatus failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 导入网关license文件
        /// </summary>
        /// <param name="license"></param>
        /// <returns></returns>
        public SmcErr ImportCGWLicense(string license)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new CgwError();

            if (this.licenseFileOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    FileOperation file = new FileOperation();
                    file.WriteFile(strPathLic, license);
                }
                catch (Exception ex)
                {
                    logEx.Error("ImportCGWLicense Exception:{0}", ex.ToString());
                }
                finally
                {
                    this.licenseFileOperateLock.ExitWriteLock();
                }
            }
            return err;
        }

    }
}