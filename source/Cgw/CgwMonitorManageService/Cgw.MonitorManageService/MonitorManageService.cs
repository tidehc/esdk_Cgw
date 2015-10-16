/*-----------------------------------------------------------------------
//文件名：MonitorManageService.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-6-12
//描述：用于对外开放wcf接口
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.ServiceModel.Activation;
using System.ServiceModel;
using System.ServiceModel.Description;
using System.IO;
using CgwMonitorManage.SmcError;
using CgwMonitorManage.VideoMonitor;
using CgwMonitorManage.Common;
using System.ServiceModel.Channels;
using System.Threading;

namespace CgwMonitorManage.Service
{
    [ServiceBehavior(ConcurrencyMode = ConcurrencyMode.Multiple, InstanceContextMode = InstanceContextMode.Single)]
    [AspNetCompatibilityRequirements(RequirementsMode = System.ServiceModel.Activation.AspNetCompatibilityRequirementsMode.Allowed)]
    public class MonitorManageService : ServiceBase, IMonitorManageService, IServiceBehavior
    {
        /// <summary>
        /// 日志
        /// </summary>
        private static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 监控管理服务端口
        /// </summary>
        private ServiceHost serviceHost;

        /// <summary>
        /// 监控平台是否加载成功
        /// </summary>
        private bool isLoadSuccess = false;

        /// <summary>
        /// 单实例
        /// </summary>
        private static MonitorManageService monitorManageService;

        /// <summary>
        /// 单例模式锁
        /// </summary>
        private static object instanceLocker = new object();

        /// <summary>
        /// 连接返回的SessionID，用作各操作前CheckSession的使用
        /// </summary>
        private static string currentSessionID = null;

        /// <summary>
        /// 私有构造函数
        /// </summary>
        private MonitorManageService()
        {

        }

        /// <summary>
        /// 单实例
        /// </summary>
        /// <returns></returns>
        static public MonitorManageService Instance()
        {
            if (monitorManageService == null)
            {
                lock (instanceLocker)
                {
                    monitorManageService = new MonitorManageService();
                }
            }
            return monitorManageService;
        }

        /// <summary>
        /// 设置服务运行宿主
        /// </summary>
        /// <param name="host"></param>
        public void SetHost(ServiceHost host)
        {
            this.serviceHost = host;
        }

        /// <summary>
        /// 检查Session
        /// </summary>
        /// <returns></returns>
        private SmcErr CheckSession(string sessionID)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new CgwError();
            CgwSession CgwSession = null;
            if (OperationContext.Current != null)
            {
                //string strSessionId = OperationContext.Current.SessionId;
                string strSessionId = sessionID;
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
                        //logEx.Trace("Current SessionId is:{0}, LastVisitedTime is :{1}", strSessionId, CgwSession.LastVisitedTime.ToLongTimeString());
                        logEx.Trace("LastVisitedTime is :{0}", CgwSession.LastVisitedTime.ToLongTimeString());

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
        /// 线程加载cgw
        /// </summary>
        /// <returns></returns>
        public SmcErr Load()
        {
            SmcErr err = new SmcErr();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            //初始化监控平台
            err = VideoMonitorManage.Instance().Load();
            VideoMonitorManage.Instance().SetDataCallBackFunc(DataCallBackFunc);
            if (err.IsSuccess())
            {
                logEx.Trace("Load CgwMonitorManage Successful !");
                isLoadSuccess = true;

            }
            else
            {
                logEx.Fatal("Load CgwMonitorManage Failed, ErrNo : {0}!", err.ErrNo);
            }

            return err;
        }


        /// <summary>
        /// 停止服务
        /// </summary>
        protected override void OnStop()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("Enter MonitorManageService.OnStop");

            if (this.serviceHost != null)
            {
                VideoMonitorManage.Instance().Unload();
                this.serviceHost.Close();
                logEx.Info("MonitorManageService Close Success");
            }
        }

        #region  继承IServiceBehavior的接口

        public void AddBindingParameters(ServiceDescription serviceDescription, ServiceHostBase serviceHostBase, System.Collections.ObjectModel.Collection<ServiceEndpoint> endpoints, System.ServiceModel.Channels.BindingParameterCollection bindingParameters)
        {

        }

        public void ApplyDispatchBehavior(ServiceDescription serviceDescription, ServiceHostBase serviceHostBase)
        {

        }

        public void Validate(ServiceDescription serviceDescription, ServiceHostBase serviceHostBase)
        {

        }
        #endregion

        #region 通过wcf对外开放的接口

        /// <summary>
        /// 断开监控管理连接
        /// </summary>
        /// <returns></returns>
        public SmcErr Disconnect()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            SmcErr err = new CgwError();
            if (OperationContext.Current != null)
            {
                //string strSessionId = OperationContext.Current.SessionId;
                string strSessionId = currentSessionID;
                if (!string.IsNullOrEmpty(strSessionId))
                {
                    err = SessionManage.Instance().RemoveSessionBySessionId(strSessionId);
                    logEx.Info("Client DisConnect MonitorManageService Successful,Current.Cgw IP is : {0}", CgwConst.ClientIp);
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
        /// 连接融合网关
        /// </summary>
        /// <param name="password">连接字符串</param>
        /// <returns></returns>
        public SmcErr Connect(out string sessionID)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            sessionID = "";
            if (isLoadSuccess == false)
            {
                err.SetErrorNo(CgwError.ERR_CGW_IS_LOADING);
                return err;
            }

            //获取SessionManage中Session个数
            if (SessionManage.Instance().GetSessionsCount() == 0)
            {
                Guid sessionGuid = Guid.NewGuid();

                sessionID = OperationContext.Current.SessionId;
                currentSessionID = sessionID;
                //保存至CGW的session管理中
                CgwSession CgwSession = new CgwSession(sessionGuid, OperationContext.Current.SessionId);

                MessageProperties properties = OperationContext.Current.IncomingMessageProperties;   //获取传进的消息属性  

                RemoteEndpointMessageProperty endpoint = properties[RemoteEndpointMessageProperty.Name] as RemoteEndpointMessageProperty; //获取消息发送的远程终结点IP和端口 
                if (endpoint != null)
                {
                    CgwConst.ClientIp = endpoint.Address; // 获取发送消息的客户端的 IP 地址。
                    logEx.Info("Client Connect MonitorManageService Successful,Current.Client IP is : {0}", CgwConst.ClientIp);
                }
                else
                {
                    logEx.Info("Client Connect MonitorManageService Successful,Current.Client IP is  null.");
                }
            }
            else
            {
                err.SetErrorNo(CgwError.ERR_CGW_BUSINESS_SESSION_COUNT_MAX);
                logEx.Warn("Session Count is  Top Of Max number !");
            }

            return err;
        }

        /// <summary>
        /// 保持连接
        /// </summary>]
        public SmcErr KeepAliver(string sessionID)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            err = CheckSession(sessionID);
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
        /// 获取摄像头列表及分组信息
        /// </summary>
        /// <param name="fromMonitorSys">如果该值为true，则实时从监控平台获取,否则从融合网关缓存获取</param>
        /// <param name="cameraList">摄像头列表</param>
        /// <param name="groupList">组信息</param>
        /// <param name="nodeRelationList">分组关系</param>
        /// <returns></returns>
        public SmcErr GetAllCameras(PlatformType platformType, PagesParam pageParam, out List<Camera> cameraList, out List<CameraGroup> groupList, out List<NodeRelation> nodeRelationList, out PageInfo pagesInfo, PlatformLicenseInfo licenseInfo = null)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            cameraList = null;
            groupList = null;
            nodeRelationList = null;
            pagesInfo = null;

            err = CheckSession(currentSessionID);
            //调任何接口前 需要判断监控平台是否加载成功
            if (err.IsSuccess())
            {
                err = VideoMonitorManage.Instance().GetAllCameras(pageParam, out cameraList, out groupList, out nodeRelationList, out pagesInfo, licenseInfo);
            }
            if (err.IsSuccess())
            {
                logEx.Info("MonitorManageService QueryMonitorCamera  Successful,Current Cgw IP is : {0}", CgwConst.ClientIp);
            }
            else
            {
                logEx.Error("MonitorManageService QueryMonitorCamera failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// PTZ(Pan/Tilt/Zoom)控制，该接口需要在预览模式下使用
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="ptzCommandType">命令类型</param>
        /// <param name="param">命令参数</param>
        /// <returns>返回错误码</returns>
        public SmcErr StartControlPtz(string cameraNo, PtzCommandType ptzCommandType, int param)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            err = CheckSession(currentSessionID);
            //调任何接口前 需要判断监控平台是否加载成功
            if (err.IsSuccess())
            {
                err = VideoMonitorManage.Instance().StartControlPtz(cameraNo, ptzCommandType, param);
            }
            if (err.IsSuccess())
            {
                logEx.Info("MonitorManageService StartControlPtz  Successful,Current Cgw IP is : {0}", CgwConst.ClientIp);
            }
            else
            {
                logEx.Error("MonitorManageService StartControlPtz failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 停止云台控制，摄像头控制
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="ptzCommandType">命令类型</param>
        /// <returns></returns>
        public SmcErr StopControlPtz(string cameraNo, PtzCommandType ptzCommandType)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            err = CheckSession(currentSessionID);
            //调任何接口前 需要判断监控平台是否加载成功
            if (err.IsSuccess())
            {
                err = VideoMonitorManage.Instance().StopControlPtz(cameraNo, ptzCommandType);
            }
            if (err.IsSuccess())
            {
                logEx.Info("MonitorManageService StopControlPtz  Successful,Current Cgw IP is : {0}", CgwConst.ClientIp);
            }
            else
            {
                logEx.Error("MonitorManageService StopControlPtz failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 重发关键帧
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        public SmcErr MakeIFrame(string cameraNo)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            err = CheckSession(currentSessionID);
            //调任何接口前 需要判断监控平台是否加载成功
            if (err.IsSuccess())
            {
                err = VideoMonitorManage.Instance().MakeIFrame(cameraNo);
            }
            if (err.IsSuccess())
            {
                logEx.Info("MonitorManageService MakeIFrame  Successful,Current Cgw IP is : {0}", CgwConst.ClientIp);
            }
            else
            {
                logEx.Error("MonitorManageService MakeIFrame failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 设置摄像头扬声器状态
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="isOn">扬声器是否开启</param>
        /// <returns></returns>
        public SmcErr SetSpeaker(string cameraNo, bool isOn)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            err = CheckSession(currentSessionID);
            //调任何接口前 需要判断监控平台是否加载成功
            if (err.IsSuccess())
            {
                err = VideoMonitorManage.Instance().SetSpeaker(cameraNo, isOn);
            }
            if (err.IsSuccess())
            {
                logEx.Info("MonitorManageService SetSpeaker  Successful,Current Cgw IP is : {0}", CgwConst.ClientIp);
            }
            else
            {
                logEx.Error("MonitorManageService SetSpeaker failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 设置摄像头麦克风状态
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="isOn">麦克风是否开启</param>
        /// <returns></returns>
        public SmcErr SetMic(string cameraNo, bool isOn)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            err = CheckSession(currentSessionID);
            //调任何接口前 需要判断监控平台是否加载成功
            if (err.IsSuccess())
            {
                err = VideoMonitorManage.Instance().SetMic(cameraNo, isOn);
            }
            if (err.IsSuccess())
            {
                logEx.Info("MonitorManageService SetMic  Successful,Current Cgw IP is : {0}", CgwConst.ClientIp);
            }
            else
            {
                logEx.Error("MonitorManageService SetMic failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }


        /// <summary>
        /// 刷新监控摄像头列表
        /// </summary>
        /// <returns></returns>
        public SmcErr RefreshMonitorCamera()
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            err = CheckSession(currentSessionID);
            //调任何接口前 需要判断监控平台是否加载成功
            if (err.IsSuccess())
            {
                Thread th = new Thread(new ThreadStart(() =>
                {
                    VideoMonitorManage.Instance().RefreshMonitorCamera();
                }));
                th.Start();
            }

            if (err.IsSuccess())
            {
                logEx.Info("MonitorManageService RefreshMonitorCamera  Successful,Current Cgw IP is : {0}", CgwConst.ClientIp);
            }
            else
            {
                logEx.Error("MonitorManageService RefreshMonitorCamera failed,ErrNo :{0}", err.ErrNo);
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
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            refreshStatus = new SmcErr();
            refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END;
            err = CheckSession(currentSessionID);
            //调任何接口前 需要判断监控平台是否加载成功
            if (err.IsSuccess())
            {
                err = VideoMonitorManage.Instance().GetRefreshStatus(out refreshStatus);
            }
            else
            {
                refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED;
            }
            if (err.IsSuccess())
            {
                logEx.Info("MonitorManageService GetRefreshStatus  Successful,Current Cgw IP is : {0}", CgwConst.ClientIp);
            }
            else
            {
                logEx.Error("MonitorManageService GetRefreshStatus failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 预览摄像头视频，使用命名管道方式接收数据
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="pipeName"></param>
        /// <returns></returns>
        public SmcErr StartReceiveVideoByPipe(string cameraNo, out string pipeName)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            pipeName = string.Empty;
            err = CheckSession(currentSessionID);
            //调任何接口前 需要判断监控平台是否加载成功
            if (err.IsSuccess())
            {
                err = VideoMonitorManage.Instance().StartReceiveVideo(cameraNo);

                //开始监控平台成功、开始通过管道发送数据
                if (err.IsSuccess())
                {
                    VideoPipeManage.Instance().DeleteVideoPipe(cameraNo);
                    VideoPipe videoPipe = VideoPipeManage.Instance().AddVideoPipe(cameraNo);
                    if (videoPipe != null)
                    {
                        pipeName = videoPipe.PipeName;
                        videoPipe.Run();

                        logEx.Info("MonitorManageService StartReceiveVideoByPipe  Successful,Current Cgw IP is : {0}", CgwConst.ClientIp);
                    }
                    else
                    {
                        logEx.Error("MonitorManageService AddVideoPipe failed.cameraNo = {0}", cameraNo);
                        err.SetErrorNo(CgwError.START_RECEIVE_VIDEO_FAILED);
                        StopReceiveVideoByPipe(cameraNo);
                    }
                }
                else
                {
                    logEx.Error("MonitorManageService StartReceiveVideoByPipe failed,ErrNo :{0}", err.ErrNo);
                }
            }
            else
            {
                logEx.Error("MonitorManageService StartReceiveVideoByPipe failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 停止预览摄像头视频，并清理命名管道
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <returns></returns>
        public SmcErr StopReceiveVideoByPipe(string cameraNo)
        {
            SmcErr err = new CgwError();
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);

            err = CheckSession(currentSessionID);
            //调任何接口前 需要判断监控平台是否加载成功
            if (err.IsSuccess())
            {
                err = VideoMonitorManage.Instance().StopReceiveVideo(cameraNo);
                VideoPipe videoPipe = VideoPipeManage.Instance().DeleteVideoPipe(cameraNo);
                if (videoPipe == null)
                {
                    logEx.Error("MonitorManageService StopReceiveVideoByPipe failed,VideoPipe is null,cameraNo = {0}", cameraNo);
                    err.SetErrorNo(CgwError.STOP_RECEIVE_VIDEO_FAILED);
                }
                else
                {
                    videoPipe.Stop();
                }
            }
            if (err.IsSuccess())
            {
                logEx.Info("MonitorManageService StopReceiveVideoByPipe  Successful,Current Cgw IP is : {0}", CgwConst.ClientIp);
            }
            else
            {
                logEx.Error("MonitorManageService StopReceiveVideoByPipe failed,ErrNo :{0}", err.ErrNo);
            }
            return err;
        }

        /// <summary>
        /// 码流回调委托
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="mediaData">码流</param>
        /// <param name="sender">调用者,区别哪个平台的回调</param>
        public void DataCallBackFunc(string cameraNo, MediaData mediaData, string sender)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            //logEx.Trace("Enter: MonitorManageService.SetDataCallBackFunc.sender ={0},cameraNo ={1}", sender, cameraNo);

            VideoPipe videoPipe = VideoPipeManage.Instance().GetVideoPipe(cameraNo);
            if (videoPipe != null && videoPipe.HaveConnect)
            {
                videoPipe.SenderData(mediaData);
                //logEx.Trace("Enter: Send MediaData.sender ={0},cameraNo ={1},size ={2},data ={3}", sender, cameraNo, mediaData.Size, mediaData.Data);
            }
        }
    }

        #endregion
}
