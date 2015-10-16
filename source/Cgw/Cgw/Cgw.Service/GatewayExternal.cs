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



namespace Cgw.External
{
    // NOTE: You can use the "Rename" command on the "Refactor" menu to change the class name "Service1" in code, svc and config file together.
    [AspNetCompatibilityRequirements(RequirementsMode = AspNetCompatibilityRequirementsMode.Allowed)]
    [ServiceBehavior(ConcurrencyMode = ConcurrencyMode.Multiple, InstanceContextMode = InstanceContextMode.Single)]
    public class GatewayExternal : IGatewayExternal, IServiceBehavior
    {

        /// <summary>
        /// 记录日志操作类.
        /// </summary>
       public static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 单实例
        /// </summary>
        private static GatewayExternal gatewayExternalInstance;

        /// <summary>
        /// 构造函数
        /// 单例模式实例
        /// </summary>
        public static GatewayExternal Instance()
        {
            if (gatewayExternalInstance == null)
                gatewayExternalInstance = new GatewayExternal();
            return gatewayExternalInstance;
        }

        /// <summary>
        /// 初始化融合网管服务
        /// </summary>
        public void InitService()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            //初始化监控平台
            SmcErr err = new SmcErr();
            err = MonitorChannelBll.Instance().LoadMonitor();
            if (err.IsSuccess())
            {
                logEx.Trace("Load VideoMonitor Successful !");
            }
            else
            {
                logEx.Fatal("Load VideoMonitor Failed, ErrNo : {0}!", err.ErrNo);
            }

        }

        /// <summary>
        /// 连接融合网关
        /// </summary>
        /// <param name="password">连接字符串</param>
        /// <returns></returns>
        public SmcErr Connect(string password)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            if (false == string.IsNullOrEmpty(password))
            {
                //获取后台密码字符串
                string s = CommonFunction.GetAppSetting("PassWord");
                //进行SHA256加密
                string newPassword = CommonFunction.SHA256Encrypt(s);
                //保存当前会话
                if (newPassword == password)
                {
                    //获取SessionManage中Session个数
                    if (SessionManage.Instance().GetSessionsCount() == 0)
                    {
                        Guid sessionGuid = Guid.NewGuid();

                        //保存至CGW的session管理中
                        CGWSession CgwSession = new CGWSession(sessionGuid, OperationContext.Current.SessionId);
                        logEx.Trace("Connect Successful,Current.SessionId is : ", OperationContext.Current.SessionId);
                    }
                    else
                    {                        
                        err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_SESSION_COUNT_MAX);
                        logEx.Warn("Session Count is  Top Of Max number !");
                    }

                }
                else
                {                    
                    err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_PASSWORD_INCORRECT);
                    logEx.Error("Connect Password is Incorrect !");
                }
            }
            else
            {
                err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_PASSWORD_NULL);
                logEx.Error("Connect Password is null !");
            }
            return err;
        }

        /// <summary>
        /// 断开融合网关
        /// </summary>
        /// <returns></returns>
        public SmcErr DisConnect()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new CgwError();
            if (OperationContext.Current != null)
            {
                string strSessionId = OperationContext.Current.SessionId;
                if (!string.IsNullOrEmpty(strSessionId))
                {
                    err = SessionManage.Instance().RemoveSessionBySessionId(strSessionId);
                    logEx.Trace("DisConnect Successful,Current.SessionId is : ", strSessionId);
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
                logEx.Error("KeepAlive failed,ErrNo :{0}",err.ErrNo);
            }
            return err;
        }



        /// <summary>
        /// 查询Lisence控制项（暂无）
        /// </summary>
        public SmcErr QueryLisence(out int numLisence)
        {
            SmcErr err = new CgwError();
            numLisence = 0;
            err = CheckSession();
            return err;

        }

        /// <summary>
        /// 分配监控通道
        /// </summary>
        public SmcErr AssignMonitorChannel(List<ChannelInfo> ChannelInfoList)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            err = CheckSession();
            if (err.IsSuccess())
            {
                if (ChannelInfoList != null)
                {
                    err = MonitorChannelBll.Instance().AssignMonitorChannel(ChannelInfoList);
                    
                }
                else
                {
                    err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_CHANNELLIST_NULL);
                }
                
            }
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
        /// 同步通道信息
        /// </summary>
        /// <param name="ChannelInfoList">通道列表</param>
        /// <returns></returns>
        public SmcErr SyncMonitorChannel(List<ChannelInfo> ChannelInfoList)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            err = CheckSession();
            //调任何接口前 需要判断监控平台是否加载成功
            if (err.IsSuccess())
            {
                if (ChannelInfoList != null)
                {
                    err = MonitorChannelBll.Instance().SyncMonitorChannel(ChannelInfoList);
                }
                else
                {
                    err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_CHANNELLIST_NULL);
                }
            }
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
        /// 获取监控设备列表
        /// </summary>
        /// <param name="CameraInfoList">摄像头基本信息</param>
        /// <param name="CameraGroupList">组基本信息</param>
        /// <param name="nodeRelationList">所属分组关系信息</param>
        /// <returns></returns>
        public SmcErr QueryMonitorCamera(out List<Camera> CameraInfoList, out List<CameraGroup> CameraGroupList, out List<NodeRelation> nodeRelationList)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            CameraInfoList = null;
            CameraGroupList = null;
            nodeRelationList = null;
            err = CheckSession();
            if (err.IsSuccess())
            {
                err = MonitorChannelBll.Instance().QueryMonitorCamera(out CameraInfoList, out CameraGroupList, out nodeRelationList);
            }
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
                logEx.Trace("SwitchChannelVideoSource Successful");
            }
            else
            {
                logEx.Error("SwitchChannelVideoSource failed,ErrNo :{0}", err.ErrNo);
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
                logEx.Trace("StartControlPtz Successful");
            }
            else
            {
                logEx.Error("StartControlPtz failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 关闭PTZ操作
        /// </summary>
        /// <param name="Camerano">摄像头</param>
        /// <returns></returns>
        public SmcErr StopPTZ(string Camerano)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            err = CheckSession();
            if (err.IsSuccess())
            {
                err = MonitorChannelBll.Instance().StopPTZ(Camerano);
            }
            if (err.IsSuccess())
            {
                logEx.Trace("StopPTZ Successful");
            }
            else
            {
                logEx.Error("StopPTZ failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 摄像头闭音（暂无）
        /// </summary>
        public SmcErr MuteCamera()
        {
            SmcErr err = new CgwError();
            err = CheckSession();
            if (err.IsSuccess())
            {


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
            CGWSession CgwSession = null;
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
                    //更新Session最后访问时间
                    CgwSession.LastVisitedTime = DateTime.Now;
                    logEx.Trace("Current SessionId is:{0}, LastVisitedTime is :{1}", strSessionId, CgwSession.LastVisitedTime.ToLongTimeString());
                }
            }
            else
            {
                err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_CURRENTSESSION_NULL);
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
    }
}
