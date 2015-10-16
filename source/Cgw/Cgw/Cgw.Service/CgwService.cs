using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.ServiceModel;
using System.Text;
using Cgw.SmcError;
using System.Web;
using Cgw.Common;
using System.ServiceModel.Description;
using System.ServiceModel.Channels;
using System.ServiceModel.Activation;
using System.Runtime.Caching;
using Cgw.Channel;
using System.ServiceProcess;
using Cgw.MonitorManageAdapter;


namespace Cgw.Service
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "Service1" in code, svc and config file together.
    [AspNetCompatibilityRequirements(RequirementsMode = AspNetCompatibilityRequirementsMode.Allowed)]
    [ServiceBehavior(ConcurrencyMode = ConcurrencyMode.Multiple, InstanceContextMode = InstanceContextMode.Single)]
    public class CgwService : ICgwService, IServiceBehavior
    {
        //private int licChannelCount = 0;

        /// <summary>
        /// license项是否改变
        /// </summary>
        private bool isChangeLic;
        /// <summary>
        /// 记录日志操作类.
        /// </summary>
        public static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 单实例
        /// </summary>
        private static CgwService cgwServiceInstance;

        //private string strPwd = "";

        private bool isLoadSuccess = false;

        /// <summary>
        /// 构造函数
        /// 单例模式实例
        /// </summary>
        public static CgwService Instance()
        {
            if (cgwServiceInstance == null)
            {
                cgwServiceInstance = new CgwService();
            }
            return cgwServiceInstance;
        }

        /// <summary>
        /// 単例的构造函数
        /// </summary>
        private CgwService()
        {

        }
        /// <summary>
        /// 初始化融合网管服务
        /// </summary>
        public SmcErr InitService()
        {
            SmcErr err = new SmcErr();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            //strPwd = ConfigSettings.ConnectPassword;
            return err;
        }
        /// <summary>
        /// 线程加载cgw
        /// </summary>
        /// <returns></returns>
        public SmcErr Load()
        {
            SmcErr err = new SmcErr();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            //初始化监控平台
            err = MonitorChannelBll.Instance().Load();
            if (err.IsSuccess())
            {
                logEx.Trace("Load CgwService Successful !");
                isLoadSuccess = true;
            }
            else
            {
                logEx.Fatal("Load CgwService Failed, ErrNo : {0}!", err.ErrNo);
            }

            return err;
        }

        /// <summary>
        /// 线程Unload cgw
        /// </summary>
        /// <returns></returns>
        public SmcErr StopAllStream() 
        {
            SmcErr err = new SmcErr();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            //Stop VideoData
            err = MonitorChannelBll.Instance().StopAllStream();
            if (err.IsSuccess())
            {
                logEx.Info("UnLoad CgwService Successful !");           
            }
            else
            {
                logEx.Fatal("UnLoad CgwService Failed, ErrNo : {0}!", err.ErrNo);
            }

            return err;
        }

        /// <summary>
        /// Disconnect monitorPlat
        /// </summary>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr DisconnectMonitor()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new SmcErr();
            err = MonitorChannelBll.Instance().Disconnect();
            if (err.IsSuccess())
            {
                logEx.Info("CgwService.Disconnect Successful !");
            }
            return err;
        }

        /// <summary>
        /// LicenseInit
        /// </summary>
        public void LicenseInit()
        {
            MonitorChannelBll.Instance().LicenseInit();
        }

        /// <summary>
        /// 连接融合网关
        /// </summary>
        /// <param name="cgwPwd">连接字符串</param>
        /// <returns></returns>
        public SmcErr Connect(string cgwPwd)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            if (isLoadSuccess == false)
            {
                err.SetErrorNo(CgwError.ERR_CGW_IS_LOADING);
                return err;
            }
            //if (false == string.IsNullOrEmpty(cgwPwd))
            //{

                ////进行SHA256加密
                //string newPassword = CommonFunction.SHA256Encrypt(strPwd);
                //保存当前会话
                //if (newPassword == cgwPwd)
                //{
                    //获取SessionManage中Session个数
                    if (SessionManage.Instance().GetSessionsCount() == 0)
                    {
                        Guid sessionGuid = Guid.NewGuid();
                        //保存至CGW的session管理中
                        CgwSession CgwSession = new CgwSession(sessionGuid, OperationContext.Current.SessionId);

                        isChangeLic = false;
                        MessageProperties properties = OperationContext.Current.IncomingMessageProperties;   //获取传进的消息属性  

                        RemoteEndpointMessageProperty endpoint = properties[RemoteEndpointMessageProperty.Name] as RemoteEndpointMessageProperty; //获取消息发送的远程终结点IP和端口 
                        if (endpoint != null)
                        {
                            CgwConst.SmcIp = endpoint.Address; // 获取发送消息的客户端的 IP 地址。
                            logEx.Info("SMC Connect CgwService Successful,Current.SMC IP is : {0}", CgwConst.SmcIp);
                        }
                        else
                        {
                            logEx.Info("SMC Connect CgwService Successful,Current.SMC IP is  null.");
                        }
                    }
                    else
                    {
                        err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_SESSION_COUNT_MAX);
                        logEx.Warn("Session Count is  Top Of Max number !");
                    }

                //}
                //else
                //{
                //    err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_PASSWORD_INCORRECT);
                //    logEx.Error("Connect Password is Incorrect !");
                //}
            //}
            //else
            //{
            //    err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_PASSWORD_NULL);
            //    logEx.Error("Connect Password is null !");
            //}
            return err;
        }

        /// <summary>
        /// 断开融合网关
        /// </summary>
        /// <returns></returns>
        public SmcErr Disconnect()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new CgwError();
            if (OperationContext.Current != null)
            {
                string strSessionId = OperationContext.Current.SessionId;
                if (!string.IsNullOrEmpty(strSessionId))
                {
                    err = SessionManage.Instance().RemoveSessionBySessionId(strSessionId);
                    logEx.Info("SMC DisConnect CgwService Successful,Current.SMC IP is : {0}", CgwConst.SmcIp);
                }
                else
                {
                    err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_CURRENTSESSION_NULL);
                    logEx.Warn("Current SessionId is null ! ");
                }
            }
            else
            {
                err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_CURRENTSESSION_NULL);
                logEx.Warn("Current SessionId is null ! ");
            }

            return err;

        }

        /// <summary>
        /// 保持连接
        /// </summary>]
        public SmcErr KeepAliver()
        {

            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            err = CheckSession();
            if (err.IsSuccess())
            {
                logEx.Trace("KeepAlive Successful");
            }
            else
            {
                logEx.Error("KeepAlive failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }





        /// <summary>
        /// 分配监控通道
        /// </summary>
        public SmcErr UpdateMonitorChannel(List<ChannelInfo> channelInfoList)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            err = CheckSession();
            if (err.IsSuccess())
            {
                if (channelInfoList != null)
                {
                    err = MonitorChannelBll.Instance().AssignMonitorChannel(channelInfoList);

                }
                else
                {
                    err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_CHANNELLIST_NULL);
                }

            }
            if (err.IsSuccess())
            {
                logEx.Info("SMC AssignMonitorChannel  Successful,Current SMC IP is : {0}", CgwConst.SmcIp);
            }
            else
            {
                logEx.Error("SMC AssignMonitorChannel failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 同步通道信息
        /// </summary>
        /// <param name="ChannelInfoList">通道列表</param>
        /// <returns></returns>
        public SmcErr SyncMonitorChannel(ref  List<ChannelInfo> channelInfoList)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            err = CheckSession();
            //调任何接口前 需要判断监控平台是否加载成功
            if (err.IsSuccess())
            {
                if (channelInfoList != null)
                {
                    err = MonitorChannelBll.Instance().SyncMonitorChannel(ref channelInfoList);
                }
                else
                {
                    err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_CHANNELLIST_NULL);
                }
            }
            if (err.IsSuccess())
            {
                logEx.Info("SMC SyncMonitorChannel  Successful,Current SMC IP is : {0}", CgwConst.SmcIp);
            }
            else
            {
                logEx.Error("SMC SyncMonitorChannel failed,ErrNo :{0}", err.ErrNo);
            }
            return err;

        }

        /// <summary>
        /// 获取监控设备列表
        /// </summary>
        /// <param name="CameraInfoList">摄像头基本信息</param>
        /// <param name="CameraGroupList">组基本信息</param>
        /// <param name="nodeRelationList">所属分组关系信息</param>
        /// <returns></returns>
        public SmcErr QueryMonitorCamera(PlatformType platformType, PageParam pageParam, out List<Camera> cameraInfoList, out List<CameraGroup> cameraGroupList, out List<NodeRelation> nodeRelationList, out PagesInfo pagesInfo)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            cameraInfoList = null;
            cameraGroupList = null;
            nodeRelationList = null;
            pagesInfo = null;
            err = CheckSession();
            if (err.IsSuccess())
            {
                err = MonitorChannelBll.Instance().QueryMonitorCamera(platformType, pageParam, out cameraInfoList, out cameraGroupList, out nodeRelationList, out pagesInfo);
            }
            if (err.IsSuccess())
            {
                logEx.Info("SMC QueryMonitorCamera  Successful,Current SMC IP is : {0}", CgwConst.SmcIp);
            }
            else
            {
                logEx.Error("SMC QueryMonitorCamera failed,ErrNo :{0}", err.ErrNo);
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
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            err = CheckSession();
            if (err.IsSuccess())
            {
                err = MonitorChannelBll.Instance().SwitchChannelVideoSource(channelLabel, cameraNo);

            }
            if (err.IsSuccess())
            {
                logEx.Info("SMC SwitchChannelVideoSource  Successful,Current SMC IP is : {0}", CgwConst.SmcIp);
            }
            else
            {
                logEx.Error("SMC SwitchChannelVideoSource failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 开始PTZ操作
        /// </summary>
        /// <param name="cameraNo">摄像头NO</param>
        /// <param name="ptzCommand">操作类型</param>
        /// <param name="param">命令参数</param>
        /// <returns></returns>
        public SmcErr StartControlPtz(string cameraNo, PtzCommandType ptzCommand, int param)
        {
            SmcErr err = new CgwError();
            err = CheckSession();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            if (err.IsSuccess())
            {
                err = MonitorChannelBll.Instance().StartControlPtz(cameraNo, ptzCommand, param);
            }
            if (err.IsSuccess())
            {
                logEx.Info("SMC StartControlPtz  Successful,Current SMC IP is : {0}", CgwConst.SmcIp);
            }
            else
            {
                logEx.Error("SMC StartControlPtz failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 关闭PTZ操作
        /// </summary>
        /// <param name="Camerano">摄像头</param>
        /// <returns></returns>
        public SmcErr StopControlPtz(string cameraNo, PtzCommandType ptzCommandType)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            err = CheckSession();
            if (err.IsSuccess())
            {
                err = MonitorChannelBll.Instance().StopControlPtz(cameraNo, ptzCommandType);
            }
            if (err.IsSuccess())
            {
                logEx.Info("SMC StopPTZ  Successful,Current SMC IP is : {0}", CgwConst.SmcIp);
            }
            else
            {
                logEx.Error("SMC StopPTZ failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 设置麦克风
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="isOn"></param>
        /// <returns></returns>
        public SmcErr SetMic(string channelLabel, string cameraNo, bool isOn)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            err = CheckSession();
            if (err.IsSuccess())
            {
                err = MonitorChannelBll.Instance().SetMic(channelLabel, cameraNo, isOn);
            }
            if (err.IsSuccess())
            {
                logEx.Info("SMC SetMic  Successful,Current SMC IP is : {0}", CgwConst.SmcIp);
            }
            else
            {
                logEx.Error("SMC SetMic failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 设置扬声器
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="isOn"></param>
        /// <returns></returns>
        public SmcErr SetSpeaker(string channelLabel, string cameraNo, bool isOn)
        {
            SmcErr err = new CgwError();
            err = CheckSession();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            if (err.IsSuccess())
            {
                err = MonitorChannelBll.Instance().SetSpeaker(channelLabel, cameraNo, isOn);
            }
            if (err.IsSuccess())
            {
                logEx.Info("SMC StartControlPtz  Successful,Current SMC IP is : {0}", CgwConst.SmcIp);
            }
            else
            {
                logEx.Error("SMC StartControlPtz failed,ErrNo :{0}", err.ErrNo);
            }
            return err;

        }
        /// <summary>
        /// 检查Session
        /// </summary>
        /// <returns></returns>
        private SmcErr CheckSession()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new CgwError();
            CgwSession CgwSession = null;
            if (OperationContext.Current != null)
            {
                string strSessionId = OperationContext.Current.SessionId;
                err = SessionManage.Instance().GetSessionsBySessionId(strSessionId, out CgwSession);

                if (err.IsSuccess() == false || CgwSession == null)
                {
                    err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_SESSION_NULL);
                }
                else
                {
                    if (CgwSession != null)
                    {
                        //更新Session最后访问时间
                        CgwSession.LastVisitedTime = DateTime.Now;
                        //logEx.Trace("Current Id is:{0}, LastVisitedTime is :{1}", strSessionId, CgwSession.LastVisitedTime.ToLongTimeString());
                        logEx.Trace("Current LastVisitedTime is :{0}", CgwSession.LastVisitedTime.ToLongTimeString());
                        if (isLoadSuccess == false)
                        {
                            err.SetErrorNo(CgwError.ERR_CGW_IS_LOADING);
                        }
                    }
                }
            }
            else
            {
                err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_CURRENTSESSION_NULL);
            }

            return err;
        }

        /// <summary>
        /// 获取软终端被叫sip端口号
        /// </summary>
        /// <param name="sipPort"></param>
        /// <returns></returns>
        public SmcErr GetSipPort(out uint sipPort)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwService.GetSipPort");

            SmcErr err = new CgwError();

            sipPort = (uint)ConfigSettings.CgwChanelCallPort;
            logEx.Debug("sipPort = {0}", sipPort);

            if (0 == sipPort)
            {
                err.SetErrorNo(CgwError.ERR_CGW_CONFIGURE_FORMAT);
            }
            return err;
        }

        #region WCF必须的契约绑定
        public void AddBindingParameters(ServiceDescription serviceDescription, ServiceHostBase serviceHostBase, System.Collections.ObjectModel.Collection<ServiceEndpoint> endpoints, BindingParameterCollection bindingParameters)
        {

        }

        public void ApplyDispatchBehavior(ServiceDescription serviceDescription, ServiceHostBase serviceHostBase)
        {

        }

        public void Validate(ServiceDescription serviceDescription, ServiceHostBase serviceHostBase)
        {

        }
        #endregion

        /// <summary>
        /// 刷新监控摄像头列表
        /// </summary>
        /// <returns></returns>
        public SmcErr RefreshMonitorCamera()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwService.RefreshMonitorCamera");

            SmcErr err = new CgwError();
            err = CheckSession();
            if (err.IsSuccess())
            {
                err = MonitorChannelBll.Instance().RefreshMonitorCamera();
            }
            if (err.IsSuccess())
            {
                logEx.Info("SMC RefreshMonitorCamera  Successful,Current SMC IP is : {0}", CgwConst.SmcIp);
            }
            else
            {
                logEx.Error("SMC RefreshMonitorCamera failed,ErrNo :{0}", err.ErrNo);
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
            logEx.Trace("Enter: CgwService.GetRefreshStatus");
            refreshStatus = new SmcErr();
            refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END;
            SmcErr err = new CgwError();
            err = CheckSession();
            if (err.IsSuccess())
            {
                err = MonitorChannelBll.Instance().GetRefreshStatus(out refreshStatus);
            }
            else
            {
                refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED;
            }
            if (err.IsSuccess())
            {
                logEx.Info("SMC GetRefreshStatus Success,Current SMC IP is : {0}, refreshStatus : {1}", CgwConst.SmcIp,refreshStatus.ErrNo);
            }
            else
            {
                logEx.Error("SMC GetRefreshStatus Failed,ErrNo :{0}", err.ErrNo);
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
            logEx.Trace("Enter: CgwService.ImportCGWLicense");
            SmcErr err = new CgwError();

            err = CheckSession();
            if (err.IsSuccess())
            {
                err = MonitorChannelBll.Instance().ImportCGWLicense(license);
            }
            if (err.IsSuccess())
            {
                logEx.Info("SMC ImportCGWLicense  Successful,Current SMC IP is : {0}", CgwConst.SmcIp);
            }
            else
            {
                logEx.Error("SMC ImportCGWLicense failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }
    }
}
