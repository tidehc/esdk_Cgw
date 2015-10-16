/*-----------------------------------------------------------------------
//文件名：CgwMonitorManageAdapter.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-6-12
//描述：用于调用监控管理服务接口
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Cgw.Common;
using Cgw.SmcError;
using Cgw.MonitorManageAdapter.CgwMonitorManageServiceReference;
using System.ServiceModel;
using System.IO.Pipes;
using System.ServiceProcess;
using System.Threading;
using System.Timers;
namespace Cgw.MonitorManageAdapter
{
    public class CgwMonitorManageAdapter
    {
        /// <summary>
        /// 监控服务控制
        /// </summary>
        MonitorServiceControl serviceControl = new MonitorServiceControl();

        /// <summary>
        /// 单实例
        /// </summary>
        private static CgwMonitorManageAdapter monitorManageAdapter = null;

        /// <summary>
        /// 单例模式锁
        /// </summary>
        private static object instanceLocker = new object();

        /// <summary>
        /// 监控管理服务
        /// </summary>
        private static MonitorManageServiceClient client = null;

        /// <summary>
        /// 记录日志操作类.
        /// </summary>
        private static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 码流回调函数，向上回调，平台管理接到码流后，解析，判断是哪个平台的回调，然后去掉平台标识，再执行回调转发码流
        /// </summary>
        private Common.DataCallBack dataCallBack;

        /// <summary>
        /// 监控平台组名
        /// </summary>
        private string monitorPlatformCameraGroupName = string.Empty;

        /// <summary>
        /// eLTE组名
        /// </summary>
        private string eLTEPlatformCameraGroupName = string.Empty;

        /// <summary>
        /// Connect返回的SessionID，用作KeepAlive
        /// </summary>
        private string sessionID = "";

        /// <summary>
        /// 私有构造函数
        /// </summary>
        private CgwMonitorManageAdapter()
        {
            monitorPlatformCameraGroupName = ConfigSettings.MonitorPlatformCameraGroupName;
            eLTEPlatformCameraGroupName = ConfigSettings.eLTEPlatformCameraGroupName;
        }

        /// <summary>
        /// 单实例
        /// </summary>
        /// <returns></returns>
        static public CgwMonitorManageAdapter Instance()
        {
            if (monitorManageAdapter == null)
            {
                lock (instanceLocker)
                {
                    monitorManageAdapter = new CgwMonitorManageAdapter();
                    //client = new MonitorManageServiceClient();
                }
            }
            return monitorManageAdapter;
        }

        /// <summary>
        /// 连接监控平台管理服务
        /// </summary>
        /// <param name="monitorPwd">加密后的密码字符串</param>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr Connect(string monitorPwd)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("Enter: CgwMonitorManageAdapter.Connect");
            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();
            try
            {
                if (serviceControl.MonitorServiceRun())
                {
                    logEx.Info("CgwMonitorManageAdapter.Connect Opened");
                    client = new MonitorManageServiceClient();
                    CgwMonitorManageServiceReference.SmcErr serviceErr = client.Connect(out monitorPwd);

                    sessionID = monitorPwd;
                    client.Close();
                    if (serviceErr.ErrNo != Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_SUCCESS)
                    {
                        logEx.Error("CgwMonitorManageAdapter.Connect failed. ErrNo = {0} ", serviceErr.ErrNo);
                        err = SetCgwErrNo(serviceErr);
                    }
                }
                else
                {
                    logEx.Info("CgwMonitorManageAdapter.Connect MonitorServiceStart");
                    err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                    serviceControl.MonitorServiceStart();//自动重连监控服务

                }
            }
            catch (System.Exception ex)
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                logEx.Error("CgwMonitorManageServiceReference.Connect failed. Exception is {0} ", ex.ToString());
            }
            return err;

        }

        /// <summary>
        /// 断开监控平台管理服务
        /// </summary>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr Disconnect()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.Disconnect");
            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();

            try
            {
                //判断Monitor服务是否在线，在线调用DisConnect方法
                if (serviceControl.MonitorServiceRun())
                {
                    client = new MonitorManageServiceClient();
                    CgwMonitorManageServiceReference.SmcErr serviceErr = client.Disconnect();
                    client.Close();
                    if (serviceErr.ErrNo != Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_SUCCESS)
                    {
                        logEx.Error("CgwMonitorManageAdapter.Disconnect failed. ErrNo = {0} ", serviceErr.ErrNo);
                        err = SetCgwErrNo(serviceErr);
                    }
                }
                else //不在线，无需调用DisConnect方法
                {
                    logEx.Info("CgwMonitorManageAdapter.Disconnect,MonitorManage Stopped");
                }
            }
            catch (System.Exception ex)
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                logEx.Error("CgwMonitorManageServiceReference.Disconnect failed. Exception is {0} ", ex.ToString());
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
        public Cgw.SmcError.SmcErr GetAllCameras(Cgw.Common.PlatformType platformType, Common.PageParam pageParam, out List<Cgw.Common.Camera> cameraList, out List<Cgw.Common.CameraGroup> groupList, out List<Cgw.Common.NodeRelation> nodeRelationList, out PagesInfo pageInfo, Cgw.Common.PlatformLicenseInfo licenseInfo = null)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.GetAllCameras");

            cameraList = new List<Cgw.Common.Camera>();
            groupList = new List<Common.CameraGroup>();
            nodeRelationList = new List<Common.NodeRelation>();
            pageInfo = new PagesInfo();

            CgwMonitorManageServiceReference.Camera[] cameraListTemp = null;
            CgwMonitorManageServiceReference.CameraGroup[] groupListTemp = null;
            CgwMonitorManageServiceReference.NodeRelation[] nodeRelationListTemp = null;
            CgwMonitorManageServiceReference.PageInfo pagesInfoTemp = null;

            CgwMonitorManageServiceReference.PlatformLicenseInfo info = new CgwMonitorManageServiceReference.PlatformLicenseInfo();
            if (licenseInfo != null)
            {
                info.IsEltePlatform = licenseInfo.IsEltePlatform;
                info.IsMonitorPlatform = licenseInfo.IsMonitorPlatform;
            }

            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();

            try
            {
                //if (client.State == CommunicationState.Opened)
                if(serviceControl.MonitorServiceRun())
                {
                    CgwMonitorManageServiceReference.PlatformType platformTypeTemp = (CgwMonitorManageServiceReference.PlatformType)platformType;
                    CgwMonitorManageServiceReference.PagesParam pageParamTemp = new CgwMonitorManageServiceReference.PagesParam();
                    pageParamTemp.CurrentPage = pageParam.CurrentPage;
                    pageParamTemp.NumberPerPage = pageParam.NumberPerPage;

                    client = new MonitorManageServiceClient();
                    CgwMonitorManageServiceReference.SmcErr serviceErr = client.GetAllCameras(out cameraListTemp, out groupListTemp, out nodeRelationListTemp, out pagesInfoTemp, platformTypeTemp, pageParamTemp, info);
                    client.Close();

                    if (serviceErr.ErrNo != Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_SUCCESS)
                    {
                        logEx.Error("CgwMonitorManageAdapter.GetAllCameras failed. ErrNo = {0} ", serviceErr.ErrNo);
                        err = SetCgwErrNo(serviceErr);
                    }
                    else
                    {
                        if (pagesInfoTemp != null)
                        {
                            pageInfo.CurrentPage = pagesInfoTemp.CurrentPage;
                            pageInfo.TotalPages = pagesInfoTemp.TotalPages;
                            pageInfo.TotalRecords = pagesInfoTemp.TotalRecords;
                        }
                        
                        if (cameraListTemp != null)
                        {
                            foreach (CgwMonitorManageServiceReference.Camera ca in cameraListTemp)
                            {
                                Common.Camera temp = new Common.Camera(ca.No, ca.Name, (Common.CameraStatus)ca.Status);
                                cameraList.Add(temp);
                            }
                        }

                        if (groupListTemp != null)
                        {
                            foreach (CgwMonitorManageServiceReference.CameraGroup cg in groupListTemp)
                            {
                                Common.CameraGroup temp = new Common.CameraGroup(cg.No, cg.Name);
                                groupList.Add(temp);
                            }
                        }

                        if (nodeRelationList != null && nodeRelationListTemp != null)
                        {
                            foreach (CgwMonitorManageServiceReference.NodeRelation no in nodeRelationListTemp)
                            {
                                Common.NodeRelation temp = new Common.NodeRelation(no.No, new List<String>(no.Path), (Common.NodeType)no.Type);
                                nodeRelationList.Add(temp);
                            }
                        }

                        //增加顶层分组
                        if (string.IsNullOrEmpty(monitorPlatformCameraGroupName))
                        {
                            monitorPlatformCameraGroupName = "MonitorPlatform";
                        }
                        if (string.IsNullOrEmpty(eLTEPlatformCameraGroupName))
                        {
                            eLTEPlatformCameraGroupName = "eLTEPlatform";
                        }

                        //查询第一页数据时增加顶层分组信息
                        if (pageParam.CurrentPage == 1)
                        {
                            //if (licenseInfo != null && licenseInfo.IsEltePlatform)
                            if(licenseInfo!=null)
                            {
                                groupList.Add(new Common.CameraGroup(eLTEPlatformCameraGroupName, eLTEPlatformCameraGroupName));
                            }
                            //if (licenseInfo != null && licenseInfo.IsMonitorPlatform)
                            if(licenseInfo!=null)
                            {
                                groupList.Add(new Common.CameraGroup(monitorPlatformCameraGroupName, monitorPlatformCameraGroupName));
                            }
                        }

                        if (nodeRelationList.Count > 0)
                        {
                            List<Common.NodeRelation> nodeRelationTemp = new List<Common.NodeRelation>(nodeRelationList);
                            nodeRelationList.Clear();

                            foreach (Common.NodeRelation no in nodeRelationTemp)
                            {
                                List<String> path = new List<String>(no.Path);
                                if (path.Count > 1)
                                {
                                    //按照从底到顶排序
                                    path.Reverse();
                                }
                                if (no.No.IndexOf("eLTE") > -1)
                                {
                                    path.Add(eLTEPlatformCameraGroupName);
                                }
                                else
                                {
                                    path.Add(monitorPlatformCameraGroupName);
                                }

                                if (path.Count > 1)
                                {
                                    //按照从顶到底排序
                                    path.Reverse();
                                }
                                Common.NodeRelation temp = new Common.NodeRelation(no.No, path, (Common.NodeType)no.Type);
                                nodeRelationList.Add(temp);
                            }
                           
                        }

                        if (pageParam.CurrentPage == 1)
                        {
                            //if (licenseInfo != null && licenseInfo.IsEltePlatform)
                            if(licenseInfo!=null)
                            {
                                Common.NodeRelation tempELTE = new Common.NodeRelation(eLTEPlatformCameraGroupName, new List<String>(), Common.NodeType.GROUP);
                                nodeRelationList.Add(tempELTE);
                            }
                            //if (licenseInfo != null && licenseInfo.IsMonitorPlatform)
                            if(licenseInfo !=null)
                            {
                                Common.NodeRelation tempMonitor = new Common.NodeRelation(monitorPlatformCameraGroupName, new List<String>(), Common.NodeType.GROUP);
                                nodeRelationList.Add(tempMonitor);
                            }
                        }
                    }
                }
                else
                {
                    err.ErrNo = Cgw.SmcError.CgwError.GET_ALL_CAMERAS_FAILED;
                }
            }
            catch (System.Exception ex)
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                logEx.Error("CgwMonitorManageServiceReference.GetAllCameras failed. Exception is {0} ", ex.ToString());
            }
            return err;
        }

        /// <summary>
        /// 刷新监控摄像头列表
        /// </summary>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr RefreshMonitorCamera()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.RefreshMonitorCamera");
            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();

            try
            {
                //if (serviceControl.MonitorServiceRun() && client.State == CommunicationState.Opened)
                if(serviceControl.MonitorServiceRun())
                {
                    client = new MonitorManageServiceClient();
                    CgwMonitorManageServiceReference.SmcErr serviceErr = client.RefreshMonitorCamera();
                    client.Close();
                    if (serviceErr.ErrNo != Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_SUCCESS)
                    {
                        logEx.Error("CgwMonitorManageAdapter.RefreshMonitorCamera failed. ErrNo = {0} ", serviceErr.ErrNo);
                        err = SetCgwErrNo(serviceErr);
                    }
                }
                else
                {
                    err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                }
            }
            catch (System.Exception ex)
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                logEx.Error("CgwMonitorManageServiceReference.RefreshMonitorCamera failed. Exception is {0} ", ex.ToString());
            }
            return err;
        }

        /// <summary>
        /// 获取监控摄像头列表刷新状态，返回结果为0是表示刷新完毕，为1是刷新操作中。当查询刷新状态为0时，可调用获取监控摄像头列表接口，获取刷新后监控摄像头列表
        /// </summary>
        /// <param name="refreshStatus">返回值，刷新状态</param>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr GetRefreshStatus(out SmcError.SmcErr refreshStatus)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.GetRefreshStatus");

            refreshStatus = new Cgw.SmcError.SmcErr();
            refreshStatus.ErrNo = Cgw.SmcError.CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END;

            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();
            try
            {
                //if (serviceControl.MonitorServiceRun() && client.State == CommunicationState.Opened)
                if(serviceControl.MonitorServiceRun())
                {
                    CgwMonitorManageServiceReference.SmcErr i_refreshStatus = new CgwMonitorManageServiceReference.SmcErr();

                    client = new MonitorManageServiceClient();
                    CgwMonitorManageServiceReference.SmcErr serviceErr = client.GetRefreshStatus(out i_refreshStatus);
                    client.Close();

                    refreshStatus.ErrNo = i_refreshStatus.ErrNo;
                    if (serviceErr.ErrNo != Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_SUCCESS)
                    {
                        logEx.Error("CgwMonitorManageAdapter.GetRefreshStatus failed. ErrNo = {0} ", serviceErr.ErrNo);
                        err = SetCgwErrNo(serviceErr);
                    }
                    else
                    {
                        logEx.Info("CgwMonitorManageAdapter.GetRefreshStatus Success. ");
                    }
                }
                else
                {
                    err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                }
            }
            catch (System.Exception ex)
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                logEx.Error("CgwMonitorManageServiceReference.GetRefreshStatus failed. Exception is {0} ", ex.ToString());
            }
            return err;
        }

        /// <summary>
        /// 预览摄像头视频，使用命名管道方式接收数据
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="pipeName"></param>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr StartReceiveVideoByPipe(string cameraNo, out string pipeName)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.StartReceiveVideoByPipe");
            pipeName = "";
            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();

            try
            {
                //if (serviceControl.MonitorServiceRun() && client.State == CommunicationState.Opened)
                if (serviceControl.MonitorServiceRun())
                {
                    client = new MonitorManageServiceClient();
                    CgwMonitorManageServiceReference.SmcErr serviceErr = client.StartReceiveVideoByPipe(out pipeName, cameraNo);
                    client.Close();

                    if (serviceErr.ErrNo != Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_SUCCESS)
                    {
                        logEx.Error("CgwMonitorManageAdapter.StartReceiveVideoByPipe failed. ErrNo = {0} ", serviceErr.ErrNo);
                        err = SetCgwErrNo(serviceErr);
                    }
                    else
                    {
                        //将摄像头通道删除。避免重启服务后，VideoPipe未清除，导致调用停止播放。by zWX231378 2015/7/16
                        VideoPipe delVideoPipe = VideoPipeManage.Instance().DeleteVideoPipe(cameraNo);
                        if (delVideoPipe != null)
                        {
                            logEx.Info("CgwMonitorManageAdapter.StartReceiveVideoByPipe delete VideoPipe. cameraNo = {0}", cameraNo);
                            delVideoPipe.Stop();
                        }

                        VideoPipe videoPipe = VideoPipeManage.Instance().AddVideoPipe(cameraNo, pipeName);
                        if (videoPipe != null)
                        {
                            try
                            {
                                videoPipe.Run();
                                if (videoPipe.HaveConnect)
                                {
                                    videoPipe.ReadData(this.dataCallBack);

                                    logEx.Info("CgwMonitorManageAdapter.StartReceiveVideoByPipe Success. pipeName = {0} ", pipeName);
                                }
                                else
                                {
                                    logEx.Error("CgwMonitorManageAdapter.StartReceiveVideoByPipe failed. cameraNo = {0} ", cameraNo);
                                }
                            }
                            catch (System.Exception ex)
                            {
                                logEx.Error("CgwMonitorManageAdapter.StartReceiveVideoByPipe Exception= {0} ", ex.ToString());
                            }
                        }
                        else
                        {
                            logEx.Error("CgwMonitorManageAdapter.StartReceiveVideoByPipe failed. cameraNo = {0} ", cameraNo);
                        }
                    }
                }
                else
                {
                    //当监控服务未启动,则关闭管道。
                    //StopReceiveVideoByPipe(cameraNo);
                    err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                }
            }
            catch (System.Exception ex)
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                logEx.Error("CgwMonitorManageServiceReference.StartReceiveVideoByPipe failed. Exception is {0} ", ex.ToString());
            }
            return err;
        }

        /// <summary>
        /// 停止预览摄像头视频，并清理命名管道
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr StopReceiveVideoByPipe(string cameraNo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.StopReceiveVideoByPipe");
            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();

            try
            {
                //if (serviceControl.MonitorServiceRun() && client.State == CommunicationState.Opened)
                if(serviceControl.MonitorServiceRun())
                {
                    client = new MonitorManageServiceClient();
                    CgwMonitorManageServiceReference.SmcErr serviceErr = client.StopReceiveVideoByPipe(cameraNo);
                    client.Close();
                    if (serviceErr.ErrNo != Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_SUCCESS)
                    {
                        logEx.Error("CgwMonitorManageAdapter.StopReceiveVideoByPipe failed. ErrNo = {0} ", serviceErr.ErrNo);
                        err = SetCgwErrNo(serviceErr);
                    }
                    else
                    {
                        VideoPipe videoPipe = VideoPipeManage.Instance().DeleteVideoPipe(cameraNo);
                        if (videoPipe != null)
                        {
                            videoPipe.Stop();
                            logEx.Info("CgwMonitorManageAdapter.StopReceiveVideoByPipe Success. cameraNo = {0} ", cameraNo);
                        }
                        else
                        {
                            logEx.Error("CgwMonitorManageAdapter.StopReceiveVideoByPipe failed. ErrNo = {0} ", serviceErr.ErrNo);
                        }
                    }
                }
                else
                {
                    err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                }
            }
            catch (System.Exception ex)
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                logEx.Error("CgwMonitorManageServiceReference.StopReceiveVideoByPipe failed. Exception is {0} ", ex.ToString());
                logEx.Error("CameraNo:" + cameraNo);
            }
            return err;
        }

        /// <summary>
        /// 开始摄像头PTZ操作
        /// </summary>
        /// <param name="cameraNo">摄像头NO</param>
        /// <param name="ptzCommand">操作类型</param>
        /// <param name="param">命令参数</param>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr StartControlPtz(string cameraNo, Common.PtzCommandType ptzCommand, int param)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.StartControlPtz({0},{1},{2}).", cameraNo, ptzCommand, param);
            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();

            try
            {
                //if (serviceControl.MonitorServiceRun() && client.State == CommunicationState.Opened)
                if(serviceControl.MonitorServiceRun())
                {
                    client = new MonitorManageServiceClient();
                    CgwMonitorManageServiceReference.PtzCommandType ptzCommandType = (CgwMonitorManageServiceReference.PtzCommandType)ptzCommand;  //将平台的枚举转换成服务中的枚举

                    CgwMonitorManageServiceReference.SmcErr serviceErr = client.StartControlPtz(cameraNo, ptzCommandType, param);
                    client.Close();
                    if (serviceErr.ErrNo != Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_SUCCESS)
                    {
                        logEx.Error("Start control ptz failed.Camera no:{0},ptzCommandType:{1},param:{2},Error Code:{3} ", cameraNo, ptzCommand, param, err.ErrNo);
                        err = SetCgwErrNo(serviceErr);
                    }
                    else
                    {
                        logEx.Info("Start control ptz success.Camera no:{0},ptzCommandType:{1},param:{2}.", cameraNo, ptzCommand, param);
                    }
                }
                else
                {
                    err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                }
            }
            catch (System.Exception ex)
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                logEx.Error("CgwMonitorManageServiceReference.StartControlPtz failed. Exception is {0} ", ex.ToString());
            }
            return err;
        }

        /// <summary>
        /// 关闭PTZ操作
        /// </summary>
        /// <param name="Camerano">摄像头</param>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr StopControlPtz(string cameraNo, Common.PtzCommandType ptzCommandType)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.StopControlPtz");
            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();

            try
            {
                //if (serviceControl.MonitorServiceRun() && client.State == CommunicationState.Opened)
                if(serviceControl.MonitorServiceRun())
                {
                    client = new MonitorManageServiceClient();
                    CgwMonitorManageServiceReference.PtzCommandType ptzCommand = (CgwMonitorManageServiceReference.PtzCommandType)ptzCommandType;  //将平台的枚举转换成服务中的枚举

                    CgwMonitorManageServiceReference.SmcErr serviceErr = client.StopControlPtz(cameraNo, ptzCommand);
                    client.Close();
                    if (serviceErr.ErrNo != Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_SUCCESS)
                    {
                        logEx.Error("Stop control ptz failed.Camera no:{0},ptzCommandType:{1},Error Code:{2} ", cameraNo, ptzCommand, err.ErrNo);
                        err = SetCgwErrNo(serviceErr);
                    }
                    else
                    {
                        logEx.Info("Stop control ptz success.Camera no:{0},ptzCommandType:{1}.", cameraNo, ptzCommand);
                    }
                }
                else
                {
                    err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                }
            }
            catch (System.Exception ex)
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                logEx.Error("CgwMonitorManageServiceReference.StopControlPtz failed. Exception is {0} ", ex.ToString());
            }
            return err;
        }


        /// <summary>
        /// 设置麦克风
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="isOn"></param>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr SetMic(string cameraNo, bool isOn)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.SetMic");
            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();

            try
            {
                //if (serviceControl.MonitorServiceRun() && client.State == CommunicationState.Opened)
                if(serviceControl.MonitorServiceRun())
                {
                    client = new MonitorManageServiceClient();
                    CgwMonitorManageServiceReference.SmcErr serviceErr = client.SetMic(cameraNo, isOn);
                    client.Close();
                    if (serviceErr.ErrNo == Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_SUCCESS)
                    {
                        logEx.Info("Cgw SetMic  Successful,Current Cgw IP is : {0}", CgwConst.CGWSERVICE_IP_TAG);
                    }
                    else
                    {
                        logEx.Error("Cgw SetMic failed,ErrNo :{0}", err.ErrNo);
                        err = SetCgwErrNo(serviceErr);
                    }
                }
                else
                {
                    err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                }
            }
            catch (System.Exception ex)
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                logEx.Error("CgwMonitorManageServiceReference.SetMic failed. Exception is {0} ", ex.ToString());
            }
            return err;
        }

        /// <summary>
        /// 设置扬声器
        /// </summary>
        /// <param name="cameraNo"></param>
        /// <param name="isOn"></param>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr SetSpeaker(string cameraNo, bool isOn)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.SetSpeaker");

            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();
            try
            {
                //if (serviceControl.MonitorServiceRun() && client.State == CommunicationState.Opened)
                if(serviceControl.MonitorServiceRun())
                {
                    client = new MonitorManageServiceClient();
                    CgwMonitorManageServiceReference.SmcErr serviceErr = client.SetSpeaker(cameraNo, isOn);
                    client.Close();
                    if (serviceErr.ErrNo == Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_SUCCESS)
                    {
                        logEx.Info("Cgw SetSpeaker  Successful,Current Cgw IP is : {0}", CgwConst.CGWSERVICE_IP_TAG);
                    }
                    else
                    {
                        logEx.Error("Cgw SetSpeaker failed,ErrNo :{0}", err.ErrNo);
                        err = SetCgwErrNo(serviceErr);
                    }
                }
                else
                {
                    err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                }
            }
            catch (System.Exception ex)
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                logEx.Error("CgwMonitorManageServiceReference.SetSpeaker failed. Exception is {0} ", ex.ToString());
            }
            return err;
        }

        /// <summary>
        /// 重发I帧（暂不支持）
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr MakeIFrame(string cameraNo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.MakeIFrame");
            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();

            //MakeIFrame不存在重发I帧的效果，注销代码  zWX231378  2015/7/14
            //try
            //{
            //    //if (serviceControl.MonitorServiceRun() && client.State == CommunicationState.Opened)
            //    if(serviceControl.MonitorServiceRun())
            //    {
            //        client = new MonitorManageServiceClient();
            //        CgwMonitorManageServiceReference.SmcErr serviceErr = client.MakeIFrame(cameraNo);
            //        client.Close();
            //        if (serviceErr.ErrNo == Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_SUCCESS)
            //        {
            //            logEx.Info("Cgw MakeIFrame  Successful,Current Cgw IP is : {0}", CgwConst.CGWSERVICE_IP_TAG);
            //        }
            //        else
            //        {
            //            logEx.Error("Cgw MakeIFrame failed,ErrNo :{0}", err.ErrNo);
            //            err = SetCgwErrNo(serviceErr);
            //        }
            //    }
            //    else
            //    {
            //        err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
            //    }
            //}
            //catch (System.Exception ex)
            //{
            //    err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
            //    logEx.Error("CgwMonitorManageServiceReference.MakeIFrame failed. Exception is {0} ", ex.ToString());
            //}
            return err;
        }

        /// <summary>
        /// 保持连接
        /// </summary>
        /// <returns></returns>
        public Cgw.SmcError.SmcErr KeepAliver()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.KeepAliver");
            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();

            try
            {
                client = new MonitorManageServiceClient();
                CgwMonitorManageServiceReference.SmcErr serviceErr = client.KeepAliver(sessionID);
                client.Close();
                if (serviceErr.ErrNo != Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_SUCCESS)
                {
                    logEx.Error("CgwMonitorManageAdapter.KeepAliver failed. ErrNo = {0} ", serviceErr.ErrNo);
                    err = SetCgwErrNo(serviceErr);
                }
            }
            catch (System.Exception ex)
            {
                err.SetErrorNo(Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING);
                logEx.Error("CgwMonitorManageServiceReference.KeepAliver failed. Exception is {0} ", ex.ToString());
            }
            return err;
        }

        /// <summary>
        /// 指定码流回调函数
        /// </summary>
        /// <param name="dataCallBack">回调函数</param>
        public void SetDataCallBackFunc(Common.DataCallBack dataCallBack)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.SetVideoDataCallBackFunc({0})", dataCallBack);
            //向上回调，平台管理接到码流后，对摄像头编号加上平台标识，再执行回调转发码流
            this.dataCallBack = dataCallBack;
        }

        /// <summary>
        /// 指定码流数据异常回调函数
        /// </summary>
        /// <param name="dataExceptionCallBack">回调函数</param>
        public void SetDataExceptionCallBackFunc(Common.DataExceptionCallBack dataExceptionCallBack)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CgwMonitorManageAdapter.SetDataExceptionCallBackFunc({0})", dataExceptionCallBack);
           
          
            VideoPipeManage.Instance().SetDataExceptionCallBackFunc(dataExceptionCallBack);

        }


        /// <summary>
        /// 设置返回smc的错误码
        /// </summary>
        /// <param name="monitorSmcErr"></param>
        /// <returns></returns>
        private Cgw.SmcError.SmcErr SetCgwErrNo(CgwMonitorManageServiceReference.SmcErr monitorSmcErr)
        {
            Cgw.SmcError.SmcErr err = new Cgw.SmcError.SmcErr();

            if (monitorSmcErr.ErrNo == Cgw.SmcError.CgwError.ERR_CGW_IS_LOADING
               || monitorSmcErr.ErrNo == Cgw.SmcError.CgwError.ERR_CGW_BUSINESS_CURRENTSESSION_NULL
                || monitorSmcErr.ErrNo == Cgw.SmcError.CgwError.ERR_CGW_BUSINESS_SESSION_NULL
              )
            {
                err.ErrNo = Cgw.SmcError.CgwError.ERR_MONITOR_MANAGE_SERVICE_RESTARTING;
            }
            else
            {
                err.ErrNo = monitorSmcErr.ErrNo;
            }
            return err;
        }
    }
}
