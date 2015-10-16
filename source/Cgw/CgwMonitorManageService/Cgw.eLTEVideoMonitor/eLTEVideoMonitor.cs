/*-----------------------------------------------------------------------
//文件名：eLTEVideoMonitor.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-4-06
//描述：用于对接eLTE平台，解析返回数据
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using CgwMonitorManage.Common;
using CgwMonitorManage.SmcError;
using System.Threading;
using System.Net;
using System.Runtime.InteropServices;
using CgwMonitorManage.eLTE;
using System.Net.Sockets;
using System.Reflection;
using System.IO;

namespace CgwMonitorManage.eLTE
{
#pragma warning disable 0618

    public class eLTEVideoMonitor : IVideoMonitor
    {

        /// <summary>
        /// 刷新设备成功
        /// </summary>
        bool isRefreshSuccess = true;

        bool isLoginSuccess = false;

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
        /// 云台控制速度放大倍数
        /// </summary>
        int iPTZScale = 20;

        /// <summary>
        /// PTT用户
        /// </summary>
        bool bPttUser = false;

        /// <summary>
        /// 固定摄像头
        /// </summary>
        bool bFixedCamera = false;

        /// <summary>
        /// 视频格式，取值为“D1”、“CIF”、“QCIF”、“1080P”、“720P”
        /// </summary>
        string fmtValue = string.Empty;

        /// <summary>
        /// “0”表示前置； “1”表示后置
        /// </summary>
        string cameraType = string.Empty;

        /// <summary>
        /// “0”表示不需要用户确认；“1”表示需要用户确认
        /// </summary>
        string userConfirmType = string.Empty;

        /// <summary>
        /// “0”表示需要伴音；“1”表示不需要伴音
        /// </summary>
        string muteType = string.Empty;

        /// <summary>
        /// eLTE服务端IP
        /// </summary>
        string ip = string.Empty;

        /// <summary>
        /// 端口
        /// </summary>
        int iPort = 0;

        /// <summary>
        /// local端口
        /// </summary>
        int ilocalPort = 0;

        /// <summary>
        /// 用户名
        /// </summary>
        string user = string.Empty;

        /// <summary>
        /// 加密密码
        /// </summary>
        //string password = string.Empty;
        byte[] pwdByte = null;

        /// <summary>
        /// eLTESdkClient变量，
        /// </summary>
        private eLTESdkClient lteSdkClient;

        //定时器器，用于定时更新摄像头
        private System.Timers.Timer updateCameraTimer = new System.Timers.Timer();

        /// <summary>
        /// 日志
        /// </summary>
        private static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

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

        /// <summary>
        /// 视频播放类型数组
        /// </summary>
        private string[] fmtValueArrays = { "QCIF", "CIF", "D1", "720P","1080P" };

        /// <summary>
        /// 视频播放摄像头，1：后置，0：前置
        /// </summary>
        private string[] cameraTypeArrays = { "1", "0" };

        /// <summary>
        /// 播放是否需要用户确认，0：不需要用户确认，1：需要用户确认
        /// </summary>
        private string[] userConfirmTypeArrays = { "0", "1" };

        /// <summary>
        /// 伴音，1：无伴音,0：伴音
        /// </summary>
        private string[] muteTypeArrays = { "1", "0" };

        /// <summary>
        /// 布尔值类型判断
        /// </summary>
        private string[] boolArrays = { "TRUE", "FALSE" };

        /// <summary>
        /// 监控平台ID
        /// </summary>
        private string monitorId;

        /// <summary>
        /// 向上码流回调函数
        /// </summary>
        private DataCallBack dataCallBack;

        /// <summary>
        /// 在线用户列表
        /// </summary>
        private List<string> onlineUser = new List<string>();

        /// <summary>
        /// 码流回调者
        /// </summary>
        private string sender;

        /// <summary>
        /// 保存摄像头和mic状态，key为camera编号，value为mic是否开启标记
        /// </summary>
        private Dictionary<string, bool> cameraMicStatusDic = new Dictionary<string, bool>();

        /// <summary>
        /// 用户状态操作锁
        /// </summary>
        private ReaderWriterLockSlim cameraStatusOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 摄像头操作锁
        /// </summary>
        private ReaderWriterLockSlim cameraOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 预览句柄操作锁，用于cameraVideoHandeDic、videoHandleCameraDic的操作
        /// </summary>
        private ReaderWriterLockSlim handelOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 预览摄像头，等待预览状态结果锁
        /// </summary>
        private ReaderWriterLockSlim videoAutOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 保存摄像头跟预览通道的关系，key为摄像头编号，value为预览通道，一对一的关系
        /// </summary>
        private Dictionary<string, UInt32> cameraVideoChannelDic = new Dictionary<string, UInt32>();

        /// <summary>
        /// 保存预览通道跟码流发送类对象的关系，key为预览通道，value为码流发送类对象，一对一的关系
        /// </summary>
        private Dictionary<UInt32, MediaDataSender> videoChannelDataSenderDic = new Dictionary<UInt32, MediaDataSender>();

        /// <summary>
        /// 保存播放通道跟用户数据指针的关系，key为通道，value为用户数据指针，一对一的关系
        /// </summary>
        private Dictionary<UInt32, IntPtr> channelInPtrDic = new Dictionary<UInt32, IntPtr>();

        /// <summary>
        /// rtp数据包解析类
        /// </summary>
        static RtpAdapter rtpAdapter = new RtpAdapter();

        /// <summary>
        /// 解析rtp包回调函数
        /// </summary>
        static FrameDataCallBack frameDataCallBack;

        /// <summary>
        /// 设置实况rtp数据包回调，向上层传递rtp数据包
        /// </summary>
        static NET_DATA_CALLBACK realPlayCallback = new NET_DATA_CALLBACK(RealPlayCallBackRawFun);

        /// <summary>
        /// rtp数据包接收异常
        /// </summary>
        static NET_EXCEPTION_CALLBACK netExceptionCallBack = new NET_EXCEPTION_CALLBACK(NetExceptionCallBackFun);

        /// <summary>
        /// 发起视频回传成功后，在状态变化事件中处理视频接收
        /// </summary>
        static callbackP2pvideocallStatusDelegate callP2pvideo;

        /// <summary>
        /// 通知用户和群组资源的状态变更信息
        /// </summary>
        static callbackResStatusDelegate callResStatus;

        /// <summary>
        /// 通知用户和群组资源的状态变更信息,需要YC实现
        /// </summary>
        static callbackUserStatus callUserStatus;

        /// <summary>
        /// 自动下载配置文件成功回调，下载成功才进行查找设备和注册Trigger
        /// </summary>
        static callbackProvisionAllResyncDelegate callProvisionAllResync;

        /// <summary>
        /// 用户登录状态，SIP通道状态返回
        /// </summary>
        static callbackModuleStatusDelegate callModuleStatus;

        /// <summary>
        /// mic状态dictionary操作锁
        /// </summary>
        private ReaderWriterLockSlim micOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 刷新设备列表操作锁
        /// </summary>
        private ReaderWriterLockSlim refreshMonitorCameraOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 定时器器，用于获取设备列表
        /// </summary>
        private System.Timers.Timer monitorManageServiceGetCameraList = new System.Timers.Timer();

        /// <summary>
        /// 2015/2/5  保存播放的等待状态返回的摄像头,key为摄像头编号，Value为播放超时等待的信号量类
        /// </summary>
        private Dictionary<string, eLTEAutoReset> videoAutoEvents = new Dictionary<string, eLTEAutoReset>();

        /// <summary>
        /// 2015/2/5  播放视频状态返回的时间,35s
        /// </summary>
        private const int start_Wait_Time = 35000;

        /// <summary>
        /// 判断是否需要登录eLTE服务
        /// </summary>
        private bool IsNeedLogin = false;

        private string localIP = null;

        /// <summary>
        /// 构造函数，设置查询摄像机线程属性
        /// </summary>
        public eLTEVideoMonitor()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: eLTEVideoMonitor.");

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
            lteSdkClient = new eLTESdkClient();

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
        /// 通知资源状态信息,登录用户回调状态
        /// </summary>
        /// <param name="structResStaIndi"></param>
        private void CallbackResStatusDelegateFun(ref strResourceStatusIndicator structResStaIndi)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CallbackResStatusDelegateFun.status_type = {0},status_value = {1}", Enum.GetName(typeof(resource_status_t), structResStaIndi.status_type),
                         Enum.GetName(typeof(res_status_value_t), structResStaIndi.status_value));
            if (structResStaIndi.status_type == resource_status_t.RESREGSTATUS_PROXY && structResStaIndi.status_value == res_status_value_t.STATUS_REGOK)
            {
                logEx.Trace("{0} Login eLTE Success", structResStaIndi.ResId);

                ////休息2秒，等缓存回调上来后查设备，否则设备为0
                //Thread.Sleep(2000);

                //Thread th = new Thread(new ThreadStart(() =>
                //{
                //    //开始获取摄像机 
                //    this.GetAllCamerasTimer(null, null);

                //    //启动定时器
                //    this.updateCameraTimer.Start();
                //}));
                //th.Start();
            }
            else if (structResStaIndi.status_type == resource_status_t.RESREGSTATUS_PROXY && structResStaIndi.status_value == res_status_value_t.STATUS_NOT_FOUND)//密码不正确，不进行重连
            {
                logEx.Info("{0} Login eLTE Failed,STATUS_NOT_FOUND",structResStaIndi.ResId);
            }
            else if (structResStaIndi.status_type == resource_status_t.RESREGSTATUS_PROXY && structResStaIndi.status_value != res_status_value_t.STATUS_UNAUTHORIZED)
            {
                logEx.Trace("{0} Login eLTE Failed", structResStaIndi.ResId);
                if (IsNeedLogin)//判断是否需要登录
                {
                    eLTELoginInfo loginInfo = new eLTELoginInfo();
                    loginInfo.Ip = ip;
                    loginInfo.Port = iPort;
                    loginInfo.User = user;
                    //loginInfo.Pwd = password;
                    loginInfo.Pwd = CommonFunction.DecryptByte2Str(pwdByte, CgwConst.PASSWORD_TAG);
                    loginInfo.PTTUSER = bPttUser;
                    loginInfo.FIXEDCAMERA = bFixedCamera;
                    loginInfo.NotFirstLogin = true;

                    //使用线程轮询加载监控平台
                    Thread loaderThread = new Thread(LoginThread);
                    loaderThread.IsBackground = true;
                    loaderThread.Start(loginInfo);
                    loaderThread.Join(20000);
                }              
            }
        }

        /// <summary>
        /// 服务用户状态信息回调
        /// </summary>
        /// <param name="modSta"></param>
        private void CallModuleStatusDelegateFun(ref strDCModuleStatusIndicator modSta)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("Enter:CallModuleStatusDelegateFun, Type : {0},Status : {1}. ", Enum.GetName(typeof(dc_module_t), modSta.m_module_type), Enum.GetName(typeof(module_status_t), modSta.m_status_value));
            if (dc_module_t.SIP_MODULE == modSta.m_module_type)
            {
                if (module_status_t.SIP_RECONNECT == modSta.m_status_value ||
                    module_status_t.KICK_OFF == modSta.m_status_value ||
                    module_status_t.SIP_KEEPALIVE_REJECT==modSta.m_status_value)
                {
                    eLTELoginInfo loginInfo = new eLTELoginInfo();
                    loginInfo.Ip = ip;
                    loginInfo.Port = iPort;
                    loginInfo.User = user;
                    //loginInfo.Pwd = password;
                    loginInfo.Pwd = CommonFunction.DecryptByte2Str(pwdByte, CgwConst.PASSWORD_TAG);
                    loginInfo.PTTUSER = bPttUser;
                    loginInfo.FIXEDCAMERA = bFixedCamera;
                    loginInfo.NotFirstLogin = true;

                    //使用线程轮询加载监控平台
                    Thread loaderThread = new Thread(LoginThread);
                    loaderThread.IsBackground = true;
                    loaderThread.Start(loginInfo);
                    loaderThread.Join(20000);
                }
                else if (module_status_t.PASSWORD_CHANGE == modSta.m_status_value ||
                         module_status_t.USER_DELETE == modSta.m_status_value)
                {
                    logEx.Error("User Status Exception,Not ReConnect");
                }
            }            
        }

        /// <summary>
        /// 自动下载配置文件成功，下载成功后，才进行查找设备和注册Trigger
        /// </summary>
        private void CallProvisionAllDelegateFun()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("Enter:ProvisionAllResyncDelegateFun");
            //注册返回设备状态回调
            ELTE_RESULT iRet = (ELTE_RESULT)lteSdkClient.TriggerStatusReport(true);

            if (iRet == ELTE_RESULT.RET_SUCCESS)//注册成功，查找设备
            {
                Thread th = new Thread(new ThreadStart(() =>
                {
                    //开始获取摄像机 
                    this.GetAllCamerasTimer(null, null);

                    //启动定时器
                    this.updateCameraTimer.Start();
                }));
                th.Start();
            }
            else
            {
                logEx.Info("eLTE TriggerStatusReport Failed");
            }
        }

        /// <summary>
        /// 通知用户和群组资源的状态变更信息,需要YC实现,更新设备列表状态
        /// </summary>
        /// <param name="structResStaIndi"></param>
        private void CallUserStatusDelegateFun(ref strResourceStatusIndicator structResStaIndi)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: CallUserStatusDelegateFun.status_type = {0},status_value = {1},ResId = {2}", Enum.GetName(typeof(resource_status_t), structResStaIndi.status_type),
                         Enum.GetName(typeof(res_status_value_t), structResStaIndi.status_value), structResStaIndi.ResId);
            if (structResStaIndi.status_type == resource_status_t.RESREGSTATUS_PROXY && structResStaIndi.status_value == res_status_value_t.STATUS_REGOK)
            {
                try
                {
                    if (this.cameraStatusOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                    {
                        string str = structResStaIndi.ResId.ToString();
                        if (onlineUser.Exists((x) =>
                        {
                            if (x == str)
                            {
                                return true;
                            }
                            else
                            {
                                return false;
                            }
                        }))
                        {
                            return;
                        }

                        onlineUser.Add(str);
                    }
                }
                finally
                {
                    this.cameraStatusOperateLock.ExitWriteLock();
                }

            }
            else if (structResStaIndi.status_type == resource_status_t.RESREGSTATUS_PROXY && structResStaIndi.status_value == res_status_value_t.STATUS_REGFAIL)
            {
                try
                {
                    if (this.cameraStatusOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                    {
                        string str = structResStaIndi.ResId.ToString();
                        if (onlineUser.Exists((x) =>
                        {
                            if (x == str)
                            {
                                return true;
                            }
                            else
                            {
                                return false;
                            }
                        }))
                        {
                            onlineUser.Remove(str);
                        }
                    }
                }
                finally
                {
                    this.cameraStatusOperateLock.ExitWriteLock();
                }
            }

        }

        /// <summary>
        /// 发起视频回传成功后，在状态变化事件中处理视频接收、P2Pvideocall_IND_STATUS_ANSWERED时开始接收视频数据
        /// </summary>
        /// <param name="structResStaIndi"></param>
        private void P2pStatusDelegateFun(ref strP2pvideocallStatusIndicator structP2pvideocallStaIndi)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("Enter: P2pStatusDelegateFun.strP2pvideocallStatusIndicator.Status = {0},Callee = {1}", Enum.GetName(typeof(P2pvideocall_Status_t),
                         structP2pvideocallStaIndi.Status), structP2pvideocallStaIndi.Callee);

            //P2Pvideocall_IND_STATUS_ANSWERED，表示视频回传请求被成功处理处理
            //视频网关需要提取其中的RTP port和对端IP，并负责与摄像头建立RTP连接和读取RTP中的H.264的视频流，然后再调用上层的视频管理系统或视频监控系统中的API
            //视频从摄像头回传，通过视频网关传入到上层的视频管理系统或视频监控系统
            if (structP2pvideocallStaIndi.Status == P2pvideocall_Status_t.P2Pvideocall_IND_STATUS_ANSWERED)
            {
                //摄像机编码
                int cameraNo = structP2pvideocallStaIndi.Callee;
                //源端发送码流IP;
                string pRemoteSendStreamIp = ip;
                //源端发送视频码流RTP端口号
                uint uiRemoteSendStreamVideoPort = (uint)structP2pvideocallStaIndi.RemoteVideoPort;
                //源端发送音频端口号
                uint uiRemoteSendStreamAudioPort = (uint)structP2pvideocallStaIndi.RemoteAudioPort;

                //本地视频、音频端口
                uint uiLocalAudioPort = (uint)structP2pvideocallStaIndi.CalAudioPort;
                uint uiLocalVideoPort = (uint)structP2pvideocallStaIndi.LocalVideoPort;

                //发起视频回传，建立成功
                logEx.Trace("P2pStatusDelegateFun.Camera = {0},pRemoteSendStreamIp ={1},uiRemoteSendStreamVideoPort = {2},uiRemoteSendStreamAudioPort ={3},uiLocalAudioPort ={4} ,uiLocalVideoPort ={5} P2Pvideocall_IND_STATUS_ANSWERED OK!",
                             cameraNo, pRemoteSendStreamIp, uiRemoteSendStreamVideoPort, uiRemoteSendStreamAudioPort, uiLocalAudioPort, uiLocalVideoPort);

                if (uiRemoteSendStreamVideoPort == 0)
                {
                    logEx.Warn("P2pvideocallStatus uiRemoteSendStreamVideoPort is NULL");
                    return;
                }

                //打开通道，开始接收实况RTP数据流
                UInt32 channel = StartRecvStream(cameraNo.ToString(), pRemoteSendStreamIp, uiRemoteSendStreamVideoPort, uiRemoteSendStreamAudioPort == 0 ?
                                                 uiRemoteSendStreamVideoPort : uiRemoteSendStreamAudioPort, uiLocalVideoPort,
                                                 uiLocalAudioPort == 0 ? uiLocalVideoPort : uiLocalAudioPort);

                //如果为0，表示预览失败
                if (channel == CgwConst.T28181_ERROR_HANDLE)
                {
                    logEx.Error("Start Receive camera video data failed.Camera No:{0}.", cameraNo);
                    return;
                }
                else
                {
                    logEx.Info("Start Receive camera video data success.Camera No:{0}", cameraNo);
                }
                if (this.videoAutOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                {
                    try
                    {
                        if (videoAutoEvents.ContainsKey(cameraNo.ToString()))
                        {
                            videoAutoEvents[cameraNo.ToString()].VideoSuccess = true;
                            videoAutoEvents[cameraNo.ToString()].eLTEVideoEvent.Set();
                            logEx.Trace("P2pStatusDelegateFun AutoResetEvent State Set,StartVideo Success");
                        }
                    }
                    finally
                    {
                        this.videoAutOperateLock.ExitWriteLock();
                    }
                }

                //设置rtp解析回调函数
                rtpAdapter.ESDK_RTP_OpenESChannel(frameDataCallBack, channel);

                if (this.handelOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                {
                    try
                    {
                        this.cameraVideoChannelDic.Add(cameraNo.ToString(), channel);
                        MediaDataSender mediaDataSender = new MediaDataSender(cameraNo.ToString(), this.dataCallBack);
                        this.videoChannelDataSenderDic.Add(channel, mediaDataSender);
                    }
                    finally
                    {
                        this.handelOperateLock.ExitWriteLock();
                    }
                }
            }
            else if (P2pvideocall_Status_t.P2Pvideocall_IND_STATUS_HANGUPED == structP2pvideocallStaIndi.Status
            || P2pvideocall_Status_t.P2Pvideocall_IND_STATUS_RELEASED == structP2pvideocallStaIndi.Status
            || P2pvideocall_Status_t.P2Pvideocall_IND_STATUS_CANCELLED == structP2pvideocallStaIndi.Status)
            {
                //2015/3/6 设置信号量不阻塞
                if (videoAutOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                {
                    try
                    {
                        int cameraNo = structP2pvideocallStaIndi.Callee;
                        if (videoAutoEvents.ContainsKey(cameraNo.ToString()))
                        {
                            videoAutoEvents[cameraNo.ToString()].VideoSuccess = true;
                            videoAutoEvents[cameraNo.ToString()].eLTEVideoEvent.Set();

                            logEx.Trace("P2pStatusDelegateFun AutoResetEvent State Set,StartVideo End");
                        }
                    }
                    finally
                    {
                        this.videoAutOperateLock.ExitWriteLock();
                    }
                }

                //收到呼叫被结束事件、需要停止RTP传输并关闭RTP连接
                StopReceiveVideo(structP2pvideocallStaIndi.Callee.ToString());
            }
            //2015/2/5 设备播放失败的错误码状态
            else if (structP2pvideocallStaIndi.Status == P2pvideocall_Status_t.P2Pvideocall_IND_STATUS_REMOTE_NOTCONNECTED ||
                     structP2pvideocallStaIndi.Status == P2pvideocall_Status_t.P2Pvideocall_IND_STATUS_REMOTE_NOTSUPPORTED ||
                     structP2pvideocallStaIndi.Status == P2pvideocall_Status_t.P2Pvideocall_IND_STATUS_REMOTE_NOANSWER ||
                     structP2pvideocallStaIndi.Status == P2pvideocall_Status_t.P2Pvideocall_IND_STATUS_REMOTE_NOTFOUND ||
                     structP2pvideocallStaIndi.Status == P2pvideocall_Status_t.P2Pvideocall_IND_STATUS_RELEASED_BUSY )
            {
                if (this.videoAutOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                {
                    try
                    {
                        //摄像机编码
                        int cameraNo = structP2pvideocallStaIndi.Callee;
                        if (videoAutoEvents.ContainsKey(cameraNo.ToString()))
                        {
                            videoAutoEvents[cameraNo.ToString()].VideoSuccess = false;
                            videoAutoEvents[cameraNo.ToString()].eLTEVideoEvent.Set();

                            logEx.Trace("P2pStatusDelegateFun AutoResetEvent State Set,StartVideo Remote Failed");
                        }
                    }
                    finally
                    {
                        this.videoAutOperateLock.ExitWriteLock();
                    }
                }
            }
        }

        /// <summary>
        /// 获取摄像头列表及分组信息
        /// </summary>
        private void GetAllCamerasMethod()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("Enter:GetAllCamerasMethod");
            try
            {
                List<Camera> cameraListTemp = new List<Camera>();
                List<CameraGroup> groupListTemp = new List<CameraGroup>();
                List<NodeRelation> nodeRelationListTemp = new List<NodeRelation>();

                List<eLTEGroupInfo> lsteLTEGroupInfo = new List<eLTEGroupInfo>();
                List<eLTEGrpUserInfo> lsteLTEGrpUserInfo = new List<eLTEGrpUserInfo>();

                List<eLTEUserInfo> dcUsersList = new List<eLTEUserInfo>();
                //获得本DC管理的所有User
                ELTE_RESULT iRet = (ELTE_RESULT)lteSdkClient.GetAllDcUsers(0, out dcUsersList);
                if (ELTE_RESULT.RET_SUCCESS != iRet)
                {
                    isRefreshSuccess = false;
                    logEx.Error("GetAllDcUsers Failed,ELTE_Result:{0}",iRet.ToString());
                    return;
                }

                //获得本DC管理的所有Group，返回值为一份克隆的数据，由调用者负责操纵并最终销毁、dcid：本调度台的用户号，如果 Dcid==0，则返回全网所有的Group
                iRet = (ELTE_RESULT)lteSdkClient.GetAllGroups(0, out lsteLTEGroupInfo);
                if (ELTE_RESULT.RET_SUCCESS != iRet)
                {
                    isRefreshSuccess = false;
                    logEx.Error("GetAllGroups Failed,ELTE_Result:{0}",iRet.ToString());
                    return;
                }

                foreach (eLTEGroupInfo group in lsteLTEGroupInfo)
                {
                    //保存分组信息
                    CameraGroup cameraGroup = new CameraGroup(group.grpid.ToString(), group.grpname);
                    groupListTemp.Add(cameraGroup);

                    List<eLTEGrpUserInfo> groupUserList = new List<eLTEGrpUserInfo>();
                    //获得某个组里的所有User，返回值为一份克隆的数据，由调用者负责操纵并最终销毁、grpid：组号;如果该组不存在，则返回NULL
                    iRet=(ELTE_RESULT)lteSdkClient.GetGroupUsers(group.grpid, out groupUserList);
                    if (ELTE_RESULT.RET_SUCCESS != iRet)
                    {
                        logEx.Error("GetGroupUsers Failed,GroupID:{0},ELTE_Result:{1}", group.grpid,iRet.ToString());
                        break;
                    }

                    foreach (eLTEGrpUserInfo user in groupUserList)
                    {
                        //动态重组中的组成员
                        if (user.memberType == eLTEGroup_Member_t.MEMBER_GROUP)
                        {
                            eLTEUserInfo userInfo = new eLTEUserInfo();
                            iRet=(ELTE_RESULT)lteSdkClient.GetUserInfo(user.userid, out userInfo);
                            if (ELTE_RESULT.RET_SUCCESS != iRet)
                            {
                                logEx.Error("GetUserInfo Failed,UserID:{0},ELTE_Result:{1}", user.userid,iRet.ToString());
                                break;
                            }
                            //保存分组信息
                            if (!groupListTemp.Exists(x => x.No == group.grpid.ToString()))
                            {
                                cameraGroup = new CameraGroup(userInfo.userid.ToString(), userInfo.username);
                                cameraGroup.ParentID = group.grpid.ToString();
                                groupListTemp.Add(cameraGroup);
                            }
                        }
                        //普通用户成员
                        else if (user.memberType == eLTEGroup_Member_t.MEMBER_USER)
                        {
                            eLTEUserInfo userInfo = new eLTEUserInfo();
                            userInfo = dcUsersList.Find(x =>
                            {
                                return x.userid == user.userid;
                            });

                            if (userInfo != null)
                            {
                                if ((userInfo.usercategory == eLTEUser_Category_t.PTTUSER && bPttUser) ||
                                    (userInfo.usercategory == eLTEUser_Category_t.FIXEDCAMERA && bFixedCamera))
                                {
                                    //保存用户信息
                                    Camera camera = new Camera(userInfo.userid.ToString(), userInfo.username);
                                    camera.ParentID = group.grpid.ToString();

                                    try
                                    {
                                        if (this.cameraStatusOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                                        {
                                            camera.Status = onlineUser.Exists((x) =>
                                            {
                                                if (x == camera.No)
                                                {
                                                    return true;
                                                }
                                                else
                                                {
                                                    return false;
                                                }
                                            }) == true ? CameraStatus.Online : CameraStatus.Offline;
                                        }
                                        cameraListTemp.Add(camera);
                                    }
                                    finally
                                    {
                                        this.cameraStatusOperateLock.ExitReadLock();
                                    }

                                }
                            }
                            else
                            {
                                logEx.Error("UserInfo is null,UserID:{0}", user.userid);
                            }
                        }
                    }
                }

                logEx.Trace("GetElteCameras cameraListTemp:{0}", cameraListTemp.Count);
                logEx.Trace("GetElteCameras groupListTemp:{0}", groupListTemp.Count);

                //获取摄像头和组之间的关联
                GetCameraAndGroupRelation(cameraListTemp, groupListTemp, nodeRelationListTemp);

                ////增加组外的设备信息
                //List<eLTEUserInfo> dcUsersList = new List<eLTEUserInfo>();
                ////获得本DC管理的所有User
                //lteSdkClient.GetAllDcUsers(0, out dcUsersList);

                foreach (eLTEUserInfo userInfo in dcUsersList)
                {
                    if ((userInfo.usercategory == eLTEUser_Category_t.PTTUSER && bPttUser) ||
                                (userInfo.usercategory == eLTEUser_Category_t.FIXEDCAMERA && bFixedCamera))
                    {
                        if (cameraListTemp.Exists(x => x.No == userInfo.userid.ToString()))
                        {
                            continue;
                        }
                        else
                        {
                            Camera camera = new Camera(userInfo.userid.ToString(), userInfo.username);

                            try
                            {
                                if (this.cameraStatusOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                                {
                                    camera.Status = onlineUser.Exists((x) =>
                                    {
                                        if (x == camera.No)
                                        {
                                            return true;
                                        }
                                        else
                                        {
                                            return false;
                                        }
                                    }) == true ? CameraStatus.Online : CameraStatus.Offline;
                                }
                                cameraListTemp.Add(camera);
                            }
                            finally
                            {
                                this.cameraStatusOperateLock.ExitReadLock();
                            }

                            List<string> pathList = new List<string>();
                            NodeRelation nodeRelation = new NodeRelation(camera.No, pathList, NodeType.CAMERA);
                            nodeRelationListTemp.Add(nodeRelation);
                        }
                    }
                }

                DateTime dtStart = DateTime.Now;
                DateTime dtNow = new DateTime();
                while (!isGetDevicesFinish)
                {
                    dtNow = DateTime.Now;

                    if ((dtNow - dtStart).TotalSeconds > refreshDeviceListOverTime)
                    {
                        isRefreshSuccess = false;
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

                        isRefreshSuccess = true;
                    }
                    catch (Exception ex)
                    {
                        isRefreshSuccess = false;
                        logEx.Error("Set the list to the buffer failed. ", ex.Message);
                    }
                    finally
                    {
                        this.cameraOperateLock.ExitWriteLock();
                    }
                }
            }
            catch (System.Exception ex)
            {
                isRefreshSuccess = false;
                logEx.Error("GetAllCamerasMethod failed.Exception message:{0}", ex.Message);
            }

            logEx.Debug("GetAllCameras eLTE cameraList:{0}", cameraList.Count);
            logEx.Debug("GetAllCameras eLTE groupList:{0}", groupList.Count);
            logEx.Debug("GetAllCameras eLTE nodeRelationList:{0}", nodeRelationList.Count);
        }
        /// <summary>
        /// 获取摄像头列表及分组信息定时器
        /// 1、获取系统中所有的域
        /// 2、循环所有的域，查询域下面的分组，递归处理，获取节点关系
        /// 3、查询设备列表
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void GetAllCamerasTimer(object sender, System.Timers.ElapsedEventArgs e)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: eLTEVideoMonitor.GetAllCamerasTimer().");

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
        /// 获取摄像头和组之间的关联
        /// </summary>
        /// <param name="cameraListTemp">摄像机列表</param>
        /// <param name="groupListTemp">分组列表</param>
        /// <param name="nodeRelationListTemp">组关系列表</param>
        private void GetCameraAndGroupRelation(List<Camera> cameraListTemp, List<CameraGroup> groupListTemp, List<NodeRelation> nodeRelationListTemp)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: eLTEVideoMonitor.GetCameraAndGroupRelation().");

            try
            {
                //查询摄像机的父节点,保存摄像机父节点关系列表
                foreach (Camera ca in cameraListTemp)
                {
                    //设备没有父节点
                    if (string.IsNullOrEmpty(ca.ParentID))
                    {
                        NodeRelation nodeRelation = new NodeRelation(ca.No, new List<string>(), NodeType.CAMERA);
                        nodeRelationListTemp.Add(nodeRelation);
                    }
                    else
                    {
                        string parentID = ca.ParentID;
                        //获取所有父节点路径
                        List<string> pathList = new List<string>();
                        FindNodeRelationPath(parentID, groupListTemp, ref pathList);

                        if (pathList.Count > 1)
                        {
                            //按照从顶到底排序
                            pathList.Reverse();
                        }
                        if (pathList == null)
                        {
                            pathList = new List<string>();
                        }

                        //设备组之间关系
                        NodeRelation nodeRelation = new NodeRelation(ca.No, pathList, NodeType.CAMERA);
                        nodeRelationListTemp.Add(nodeRelation);

                    }
                }
                logEx.Trace("GetCameraAndGroupRelation.CameraListTemp:{0}", cameraListTemp.Count);

                //比较返回序列中的非重复元素
                List<Camera> noduplicates = new List<Camera>();
                foreach (var camera in cameraListTemp)
                {
                    if (!noduplicates.Exists(x => x.No == camera.No))
                    {
                        noduplicates.Add(camera);
                    }
                }

                cameraListTemp.Clear();
                foreach (var camera in noduplicates)
                {
                    cameraListTemp.Add(camera);
                }

                //查询设备组的父节点,保存设备组父节点关系列表
                foreach (CameraGroup cg in groupListTemp)
                {
                    //设备组没有父节点
                    if (string.IsNullOrEmpty(cg.ParentID))
                    {
                        NodeRelation nodeRelation = new NodeRelation(cg.No, new List<string>(), NodeType.GROUP);
                        nodeRelationListTemp.Add(nodeRelation);
                    }
                    else
                    {
                        string parentID = cg.ParentID;
                        //获取分组所有父节点路径
                        List<string> pathList = new List<string>();
                        FindNodeRelationPath(parentID, groupListTemp, ref pathList);

                        if (pathList.Count > 1)
                        {
                            //按照从顶到底排序
                            pathList.Reverse();
                        }
                        //保存分组的父节点列表
                        NodeRelation nodeRelation = new NodeRelation(cg.No, pathList, NodeType.GROUP);
                        nodeRelationListTemp.Add(nodeRelation);
                    }
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("GetCameraAndGroupRelation failed. {0} ", ex.Message);
            }
        }

        /// <summary>
        /// 获取所有父节点路径
        /// </summary>
        /// <param name="parentID">父节点ID</param>
        /// <param name="groupListTemp">组列表</param>
        /// <param name="pathList">返回父节点路径</param>
        private void FindNodeRelationPath(string parentID, List<CameraGroup> groupListTemp, ref List<string> pathList)
        {
            string newParentID = string.Empty;
            bool exists = groupListTemp.Exists((x)
                =>
            {
                if (x.No == parentID)
                {
                    //保存新的父节点
                    newParentID = x.ParentID;
                    return true;
                }
                else
                {
                    return false;
                }
            });
            if (exists == false)
            {
                return;
            }
            else
            {
                //增加父节点路径
                pathList.Add(parentID);

                //开始迭代查询父节点
                FindNodeRelationPath(newParentID, groupListTemp, ref  pathList);
            }
        }

        /// <summary>
        /// 加载eLTE播放的配置参数，做异常判断并设定默认值.zWX231378. 2015/6/16
        /// </summary>
        private string LoadElTEParamters(System.Xml.XmlElement monitorConfigElement,string nodeName,string[] tempArrays)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            string tempValue=null;
            //eLTE实况参数获取
            try
            {
                tempValue = CommonFunction.GetSingleNodeValue(monitorConfigElement, nodeName);
                if (!tempArrays.Contains(tempValue.ToUpper()))
                {
                    tempValue = tempArrays[0];
                    logEx.Error("eLTE {0} is not Valid,Set default Type '{1}'", nodeName, tempValue);                   
                }
            }
            catch
            {
                if (tempArrays != null)
                {
                    tempValue = tempArrays[0];
                    logEx.Error("eLTE {0} is not Contained,Set default Type '{1}'", nodeName, tempValue);
                }
            }
            return tempValue;
           
        }

        /// <summary>
        /// 初始化eLTE台
        /// </summary>
        /// <param name="monitorConfigElement">监控平台配置节点</param>
        /// <returns></returns>
        public SmcError.SmcErr Load(System.Xml.XmlElement monitorConfigElement)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("Enter: eLTEVideoMonitor.Load().");
            SmcErr err = new CgwError();

            try
            {
                //解析xml节点，获取所需参数    
                this.monitorId = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.ID_TAG);
                string port = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.PORT_TAG);
                string localPort = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.LOCAL_ELTE_PORT);
                user = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.USER_TAG);

                pwdByte = CommonFunction.EncryptStr2Byte(CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.PASSWORD_TAG),CgwConst.PASSWORD_TAG);
                ip = CommonFunction.GetSingleNodeValue(monitorConfigElement, CgwConst.IP_TAG);

                bPttUser = bool.Parse(LoadElTEParamters(monitorConfigElement, CgwConst.PTT_USER, boolArrays));
                bFixedCamera = bool.Parse(LoadElTEParamters(monitorConfigElement, CgwConst.FIXED_CAMERA, boolArrays));

                fmtValue = LoadElTEParamters(monitorConfigElement, CgwConst.FMT_VALUE, fmtValueArrays);
                cameraType = LoadElTEParamters(monitorConfigElement, CgwConst.CAMERA_TYPE, cameraTypeArrays);

                userConfirmType = LoadElTEParamters(monitorConfigElement, CgwConst.USER_CONFIRM_TYPE,userConfirmTypeArrays);
                muteType = LoadElTEParamters(monitorConfigElement, CgwConst.MUTE_TYPE,muteTypeArrays);

                iPort = 0;
                if (!int.TryParse(port, out iPort))
                {
                    err.SetErrorNo(CgwError.MONITOR_CONFIG_FILE_INVALID);
                    logEx.Error("Load eLTE monitor failed.Execption PORT_TAG:{0}.", port);
                    return err;
                }

                ilocalPort = 0;
                if (!int.TryParse(localPort, out ilocalPort))
                {
                    err.SetErrorNo(CgwError.MONITOR_CONFIG_FILE_INVALID);
                    logEx.Error("Load eLTE monitor failed.Execption PORT_TAG:{0}.", localPort);
                    return err;
                }
                //用户名是数值
                int iUserName = 0;
                if (!int.TryParse(user, out iUserName))
                {
                    err.SetErrorNo(CgwError.MONITOR_CONFIG_FILE_INVALID);
                    logEx.Error("Load eLTE monitor failed.Execption PORT_TAG:{0}.", user);
                    return err;
                }

                eLTELoginInfo loginInfo = new eLTELoginInfo();
                loginInfo.Ip = ip;
                loginInfo.Port = iPort;
                loginInfo.User = user;
                loginInfo.Pwd = CommonFunction.DecryptByte2Str(pwdByte,CgwConst.PASSWORD_TAG);
                loginInfo.PTTUSER = bPttUser;
                loginInfo.FIXEDCAMERA = bFixedCamera;

                IsNeedLogin = true;
                //使用线程轮询加载监控平台
                Thread loaderThread = new Thread(LoginThread);
                loaderThread.IsBackground = true;
                loaderThread.Start(loginInfo);

            }
            catch (Exception e)
            {
                err.SetErrorNo(CgwError.MONITOR_CONFIG_FILE_INVALID);
                logEx.Error("Load eLTEVideoMonitor failed.Execption message:{0}.", e.Message);
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
            logEx.Trace("Enter: eLTEVideoMonitor.LoginThread().");

            try
            {
                eLTELoginInfo loginInfo = loginInfoObj as eLTELoginInfo;

                if (loginInfo == null)
                {
                    logEx.Error("loginInfoObj is Null.");
                    return;
                }
                if (loginInfo.NotFirstLogin == true)
                {
                    //登陆失败，等待一分后再次尝试登陆
                    Thread.Sleep(5000);
                }

                //本机IP 
                //string localIP = GetLocalIP(ip, iPort, ilocalPort);
                if (string.IsNullOrEmpty(localIP))
                {
                    localIP = GetLocalIP(ip, iPort, ilocalPort);
                }

                logEx.Trace("Call eLTEVideoMonitor.Login({0},{1},{2},{3}).", loginInfo.Ip, loginInfo.Port, loginInfo.User, localIP);

                //设置登录服务器IP、本地IP、用户名、密码、sip端口等信息
                ELTE_RESULT result = (ELTE_RESULT)lteSdkClient.Set_Login_Info(loginInfo.Ip, localIP, loginInfo.User, loginInfo.Pwd, loginInfo.Port.ToString(), Convert.ToInt32(loginInfo.User));
                if (result != ELTE_RESULT.RET_SUCCESS)
                {
                    logEx.Error("Set_Login_Info failed.Monitor id:{0}.ip:{1}, port:{2}, user:{3}", this.monitorId, loginInfo.Ip, loginInfo.Port, loginInfo.User);
                }

                //发起视频回传成功后，在状态变化事件中处理视频接收  
                if (callP2pvideo == null)
                {
                    callP2pvideo = P2pStatusDelegateFun;
                    eLTEInterface.SetP2pVideoCallStaCallBack(callP2pvideo);

                    callResStatus = CallbackResStatusDelegateFun;
                    eLTEInterface.SetResStaCallBack(callResStatus);

                    callUserStatus = CallUserStatusDelegateFun;
                    eLTEInterface.SetUserStaCallBack(callUserStatus);

                    callProvisionAllResync = CallProvisionAllDelegateFun;
                    eLTEInterface.SetProvisionAllResyncCallBack(callProvisionAllResync);

                    callModuleStatus = CallModuleStatusDelegateFun;
                    eLTEInterface.SetModStaCallBack(callModuleStatus);

                    //设置rtp转码回调函数
                    frameDataCallBack = FrameDataCallBackFun;

                    //初始化rtp转码模块
                    rtpAdapter.ESDK_RTP_Init();

                    //设置NETSOURCE日志目录
                    IVS_NETSOURCE_RESULT iNet = NetSourcedInterface.IVS_NETSOURCE_Init(AppDomain.CurrentDomain.BaseDirectory + "\\logs");
                    if (iNet != IVS_NETSOURCE_RESULT.SUCCESS)
                    {
                        logEx.Error("SipStackAdapter.IVS_NETSOURCE_Init Failed.");
                    }
                }

                //初始化上下文，启动SDK 
                result = (ELTE_RESULT)lteSdkClient.SDK_START();
                if (result != ELTE_RESULT.RET_SUCCESS)
                {
                    logEx.Fatal("eLTEVideoMonitor.SDK_START failed.Monitor id:{0}.ip:{1}, port:{2}, user:{3},ErrorNo:{4}", this.monitorId, loginInfo.Ip, loginInfo.Port, loginInfo.User,(int)result);
                    //服务器端返回错误, 操作被调度机拒绝
                    if ((int)(result) == -40097)
                    {
                        logEx.Fatal("Login to eLTEVideoMonitor failed,elte remote failed,elte refuse to connect.");
                    }
                }
                else
                {
                    isLoginSuccess = true;
                    logEx.Info("eLTEVideoMonitor.SDK_START success.Monitor id:{0}.ip:{1}, port:{2}, user:{3}.", this.monitorId, loginInfo.Ip, loginInfo.Port, loginInfo.User);
                }

            }
            catch (System.Exception ex)
            {
                logEx.Error("Load eLTEVideoMonitor failed. Exception message:{0}.", ex.Message);
            }
        }

        /// <summary>
        /// 注销eLTE平台资源
        /// </summary>
        /// <returns></returns>
        public SmcError.SmcErr Unload()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Info("Enter: eLTEVideoMonitor.Unload().");

            IsNeedLogin = false;
            SmcErr err = new CgwError();
            //停止定时器
            this.updateCameraTimer.Stop();
            //SDK停止处理业务
            logEx.Trace("Call eLTEVideoMonitor SDK_STOP");
            ELTE_RESULT result = (ELTE_RESULT)this.lteSdkClient.SDK_STOP();

            //释放所有实况通道,释放NETSOURCE资源
            //IVS_NETSOURCE_RESULT iNet = NetSourcedInterface.IVS_NETSOURCE_UnInit();

            logEx.Trace("Call eLTEVideoMonitor IVS_NETSOURCE_UnInit");
            NetSourcedInterface.IVS_NETSOURCE_UnInit();
            logEx.Trace("Call eLTEVideoMonitor ESDK_RTP_UnInit");
            int iNet = rtpAdapter.ESDK_RTP_UnInit();

            //if (result == ELTE_RESULT.RET_SUCCESS || iNet != IVS_NETSOURCE_RESULT.SUCCESS)
            if (result == ELTE_RESULT.RET_SUCCESS && iNet == 0)
            {
                logEx.Info("Unload eLTEVideoMonitor monitor success.Monitor id:{0}.", this.monitorId);
            }
            else
            {
                err.SetErrorNo(CgwError.MONITOR_UDLOAD_FAILED);
                logEx.Error("Unload eLTEVideoMonitor monitor failed.Monitor id:{0}.", this.monitorId);
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
            logEx.Trace("Enter: eLTEVideoMonitor.SetVideoDataCallBackFunc().");
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
        public SmcError.SmcErr GetAllCameras(out List<Camera> cameraList, out List<CameraGroup> groupList, out List<NodeRelation> nodeRelationList)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: eLTEVideoMonitor.GetAllCameras().");
            SmcErr err = new CgwError();
            cameraList = new List<Camera>();
            groupList = new List<CameraGroup>();
            nodeRelationList = new List<NodeRelation>();

            isGetDevicesFinish = false;
            if (this.cameraOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    #region 深度克隆数据
                    foreach (Camera ivsCamera in this.cameraList)
                    {
                        //从缓存获取                        
                        Camera camera = new Camera(ivsCamera.No, ivsCamera.Name);

                        try
                        {
                            if (this.cameraStatusOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                            {
                                camera.Status = onlineUser.Exists((x) =>
                                {
                                    if (x == camera.No)
                                    {
                                        return true;
                                    }
                                    else
                                    {
                                        return false;
                                    }
                                }) == true ? CameraStatus.Online : CameraStatus.Offline;
                            }
                        }
                        finally
                        {
                            this.cameraStatusOperateLock.ExitReadLock();
                        }
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

            logEx.Info("Get all cameras success.");
            return err;
        }

        /// <summary>
        /// 启动摄像头预览
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        public SmcError.SmcErr StartReceiveVideo(string cameraNo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: eLTEVideoMonitor.StartReceiveVideo({0}).", cameraNo);
            SmcErr err = new CgwError();

            //预览成功，需要停止原来的预览，并将预览句柄添加到缓存
            //需要停止的预览句柄
            UInt32 needToStopChannel = CgwConst.T28181_ERROR_HANDLE;
            if (this.handelOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    //如果预览句柄已经存在，删除掉原来的句柄,用新的句柄替换
                    if (this.cameraVideoChannelDic.ContainsKey(cameraNo.ToString()))
                    {
                        needToStopChannel = this.cameraVideoChannelDic[cameraNo.ToString()];
                        this.videoChannelDataSenderDic.Remove(needToStopChannel);
                        this.cameraVideoChannelDic.Remove(cameraNo.ToString());

                        //用户参数,4字节整数
                        IntPtr pUser = Marshal.AllocHGlobal(4);
                        NetSourcedInterface.IVS_NETSOURCE_SetDataCallBack(needToStopChannel, null, pUser);

                        NetSourcedInterface.IVS_NETSOURCE_CloseNetStream(needToStopChannel);
                        //释放NETSOURCE通道资源
                        NetSourcedInterface.IVS_NETSOURCE_FreeChannel(needToStopChannel);
                        //关闭rtp回调
                        rtpAdapter.ESDK_RTP_CloseChannel(needToStopChannel);

                        //释放用户数据内存 
                        foreach (KeyValuePair<UInt32, IntPtr> kvp in channelInPtrDic)
                        {
                            if (kvp.Key == needToStopChannel)
                            {
                                Marshal.FreeHGlobal(kvp.Value);
                                channelInPtrDic.Remove(kvp.Key);
                                break;
                            }
                        }
                    }
                }
                finally
                {
                    this.handelOperateLock.ExitWriteLock();
                }
            }

            //重新预览后，更新了预览句柄，需要将原来的预览停止
            if (needToStopChannel != CgwConst.T28181_ERROR_HANDLE)
            {
                ELTE_RESULT result = (ELTE_RESULT)this.lteSdkClient.StopP2PVideo(Convert.ToInt32(cameraNo));
                //如果不为0，表示停止原来的预览失败，只记录日志，不返回错误，不设置错误码
                if (result != ELTE_RESULT.RET_SUCCESS)
                {
                    logEx.Error("Get a new preview success. But stop old preview failed.CameraNo:{0},Ivs sdk error code:{0}", cameraNo, result);
                }
            }
            //设置实况参数
            eLTEVideoParameter para = new eLTEVideoParameter();
            para.cameratype = cameraType;
            para.fmtvalue = fmtValue;
            para.mutetype = muteType;
            para.user_confirm_type = userConfirmType;

            if (this.videoAutOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (!videoAutoEvents.ContainsKey(cameraNo))
                    {
                        AutoResetEvent startAutoEvent = new AutoResetEvent(false);
                        startAutoEvent.Reset();

                        eLTEAutoReset elTEAutoEvent = new eLTEAutoReset();
                        elTEAutoEvent.eLTEVideoEvent = startAutoEvent;
                        elTEAutoEvent.VideoSuccess = false;

                        logEx.Trace("StartReceiveVideo add CameraNo");
                        videoAutoEvents.Add(cameraNo, elTEAutoEvent);
                    }
                    //else
                    //{
                    //    cameraNoExists = true;
                    //    logEx.Info("StartReceiveVideo Contains CameraNo");
                    //}
                }
                finally
                {
                    this.videoAutOperateLock.ExitWriteLock();
                }
            }         

            ELTE_RESULT iRet = (ELTE_RESULT)lteSdkClient.StartP2PVideoMonitor(Convert.ToInt32(cameraNo), para);

            if (iRet != ELTE_RESULT.RET_SUCCESS)
            {
                err.SetErrorNo(CgwError.START_RECEIVE_VIDEO_FAILED);
                //logEx.Error("StartReceiveVideo.StartP2PVideoMonitor failed.CameraNo:{0},ELTE_RESULT:{1}", cameraNo, Enum.GetName(typeof(ELTE_RESULT), iRet));
                logEx.Error("StartReceiveVideo.StartP2PVideoMonitor failed.CameraNo:{0},ELTE_RESULT:{1}", cameraNo, iRet.ToString());
            }
            else
            {
                if (!videoAutoEvents[cameraNo].eLTEVideoEvent.WaitOne(start_Wait_Time))
                {
                    //等待播放超时，设置错误码
                    err.SetErrorNo(CgwError.START_RECEIVE_VIDEO_FAILED);
                }
                else
                {
                    if (!videoAutoEvents[cameraNo].VideoSuccess)//未超时，但播放失败
                    {
                        err.SetErrorNo(CgwError.START_RECEIVE_VIDEO_FAILED);
                    }
                }
            }

            if (this.videoAutOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (videoAutoEvents.ContainsKey(cameraNo))
                    {
                        videoAutoEvents.Remove(cameraNo);
                        logEx.Trace("StartReceiveVideo Remove CameraNo:{0} Success", cameraNo);
                    }
                }
                finally
                {
                    this.videoAutOperateLock.ExitWriteLock();
                }
            }

            return err;
        }

        /// <summary>
        /// 停止预览
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns>成功返回0，失败返回错误码</returns>
        public SmcError.SmcErr StopReceiveVideo(string cameraNo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: eLTEVideoMonitor.StopReceiveVideo({0}).", cameraNo);
            SmcErr err = new CgwError();
            //需要停止的预览句柄
            uint needToStopChannel = CgwConst.T28181_ERROR_HANDLE;
            if (this.handelOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (this.cameraVideoChannelDic.ContainsKey(cameraNo))
                    {
                        needToStopChannel = this.cameraVideoChannelDic[cameraNo];
                        this.videoChannelDataSenderDic.Remove(needToStopChannel);
                        this.cameraVideoChannelDic.Remove(cameraNo);

                        //释放用户数据内存 
                        foreach (KeyValuePair<UInt32, IntPtr> kvp in channelInPtrDic)
                        {
                            if (kvp.Key == needToStopChannel)
                            {
                                Marshal.FreeHGlobal(kvp.Value);
                                channelInPtrDic.Remove(kvp.Key);
                                break;
                            }
                        }

                        //用户参数,4字节整数
                        IntPtr pUser = Marshal.AllocHGlobal(4);
                        NetSourcedInterface.IVS_NETSOURCE_SetDataCallBack(needToStopChannel, null, pUser);
                        //释放NETSOURCE通道资源
                        IVS_NETSOURCE_RESULT iNet = NetSourcedInterface.IVS_NETSOURCE_CloseNetStream(needToStopChannel);
                        if (iNet != IVS_NETSOURCE_RESULT.SUCCESS)
                        {
                            logEx.Error("IVS_NETSOURCE_CloseNetStream failed channel={0}", needToStopChannel);
                            err.SetErrorNo(CgwError.STOP_RECEIVE_VIDEO_FAILED);
                        }

                        iNet = NetSourcedInterface.IVS_NETSOURCE_FreeChannel(needToStopChannel);
                        if (iNet != IVS_NETSOURCE_RESULT.SUCCESS)
                        {
                            logEx.Error("IVS_NETSOURCE_FreeChannel failed channel={0}", needToStopChannel);
                            err.SetErrorNo(CgwError.STOP_RECEIVE_VIDEO_FAILED);
                        }

                        //关闭rtp回调
                        rtpAdapter.ESDK_RTP_CloseChannel(needToStopChannel);
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
            if (needToStopChannel != CgwConst.T28181_ERROR_HANDLE)
            {
                ELTE_RESULT result = (ELTE_RESULT)this.lteSdkClient.StopP2PVideo(Convert.ToInt32(cameraNo));
                //如果不为0，表示预览失败
                if (result != ELTE_RESULT.RET_SUCCESS)
                {
                    err.SetErrorNo(CgwError.STOP_RECEIVE_VIDEO_FAILED);
                    logEx.Error("Stop Receive camera video data failed.eLTE sdk error code:{0}", result);
                    return err;
                }
                logEx.Info("Stop Receive camera video data success.Camera No:{0},Handle:{1}.", cameraNo, needToStopChannel);
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
        public SmcError.SmcErr StartControlPtz(string cameraNo, PtzCommandType ptzCommand, int param)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: eLTEVideoMonitor.StartControlPtz.cameraNo:{0},ptzCommand:{1},param:{2}", cameraNo, Enum.GetName(typeof(PtzCommandType), ptzCommand), param);
            SmcErr err = new CgwError();

            //将ptz命令转换成elte的命令
            eLTEPTZControlCode ptzControl = eLTEPTZControlCode.STOP_SEQ;

            switch (ptzCommand)
            {
                case PtzCommandType.PTZ_CMD_DOWN:
                    ptzControl = eLTEPTZControlCode.TILT_DOWN;
                    break;
                case PtzCommandType.PTZ_CMD_FOCUS_IN:
                    ptzControl = eLTEPTZControlCode.FOCUS_NEAR;
                    break;
                case PtzCommandType.PTZ_CMD_FOCUS_OUT:
                    ptzControl = eLTEPTZControlCode.FOCUS_FAR;
                    break;
                case PtzCommandType.PTZ_CMD_IRIS_ENLARGE:
                    ptzControl = eLTEPTZControlCode.IRIS_OPEN;
                    break;
                case PtzCommandType.PTZ_CMD_IRIS_SHRINK:
                    ptzControl = eLTEPTZControlCode.IRIS_CLOSE;
                    break;
                case PtzCommandType.PTZ_CMD_LEFT:
                    ptzControl = eLTEPTZControlCode.PAN_LEFT;
                    break;
                case PtzCommandType.PTZ_CMD_RIGHT:
                    ptzControl = eLTEPTZControlCode.PAN_RIGHT;
                    break;
                case PtzCommandType.PTZ_CMD_UP:
                    ptzControl = eLTEPTZControlCode.TILT_UP;
                    break;
                case PtzCommandType.PTZ_CMD_ZOOM_IN:
                    ptzControl = eLTEPTZControlCode.ZOOM_IN;
                    break;
                case PtzCommandType.PTZ_CMD_ZOOM_OUT:
                    ptzControl = eLTEPTZControlCode.ZOOM_OUT;
                    break;

            }

            ELTE_RESULT result = (ELTE_RESULT)lteSdkClient.PTZControl(Convert.ToInt32(cameraNo), (uint)ptzControl, (uint)(param * iPTZScale));//eLTE云台速度慢，放大20倍

            if (result != ELTE_RESULT.RET_SUCCESS)
            {
                err.SetErrorNo(CgwError.START_CONTROL_PTZ_FAILED);
                logEx.Error("StartControlPtz failed.eLTE sdk error code:{0}", result);
                return err;
            }
            else
            {
                logEx.Debug("StartControlPtz Success");
            }

            return err;
        }

        /// <summary>
        /// 停止云台控制，摄像头控制
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <param name="ptzCommand">命令类型</param>
        /// <returns></returns>
        public SmcError.SmcErr StopControlPtz(string cameraNo, PtzCommandType ptzCommandType)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: eLTEVideoMonitor.StopControlPtz({0}).", cameraNo);
            SmcErr err = new CgwError();
            ELTE_RESULT result = (ELTE_RESULT)lteSdkClient.PTZControl(Convert.ToInt32(cameraNo), (uint)eLTEPTZControlCode.STOP_SEQ, 1);

            if (result != ELTE_RESULT.RET_SUCCESS)
            {
                err.SetErrorNo(CgwError.START_CONTROL_PTZ_FAILED);
                logEx.Error("StopControlPtz failed.eLTE sdk error code:{0}", result);
                return err;
            }
            else
            {
                logEx.Debug("StopControlPtz Success");
            }

            return err;
        }

        /// <summary>
        /// 重发I帧（暂不支持）
        /// </summary>
        /// <param name="cameraNo">摄像头编号</param>
        /// <returns></returns>
        public SmcError.SmcErr MakeIFrame(string cameraNo)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: eLTEVideoMonitor.MakeIFrame({0}).", cameraNo);
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
            logEx.Trace("Enter: eLTEVideoMonitor.SetMic({0}，{1}).", cameraNo, isOn);
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
        /// 开始实况
        /// </summary>
        /// <param name="cameraNo">摄像机编码</param>
        /// <returns></returns>
        private UInt32 StartRecvStream(string cameraCode, string pRemoteSendStreamIp, uint uiRemoteSendStreamVideoPort, uint uiRemoteSendStreamAudioPort, uint uiLocalVideoPort, uint uiLocalAudioPort)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: eLTEVideoMonitor.StartRecvStream  cameraCode :{0},pRemoteSendStreamIp:{1},uiRemoteSendStreamVideoPort:{2},uiRemoteSendStreamAudioPort:{3},uiLocalVideoPort = {4},uiLocalAudioPort ={5}",
                          cameraCode, pRemoteSendStreamIp, uiRemoteSendStreamVideoPort, uiRemoteSendStreamAudioPort, uiLocalVideoPort, uiLocalAudioPort);

            //用户参数,4字节整数
            IntPtr pUser = Marshal.AllocHGlobal(4);
            try
            {
                //接收视频码流RTP端口号
                uint uiLocalRecvStreamVideoRtpPort = uiLocalVideoPort;
                //接收音频码流RTP端口号
                uint uiLocalRecvStreamAudioRtpPort = uiLocalAudioPort;

                //通道号
                UInt32 uiChannel = 0;
                //string strLocal = GetLocalIP(ip, iPort, ilocalPort);

                if (string.IsNullOrEmpty(localIP))
                {
                    localIP = GetLocalIP(ip, iPort, ilocalPort);
                }

                //获取可用通道
                IVS_NETSOURCE_RESULT iRet = NetSourcedInterface.IVS_NETSOURCE_GetChannel(ref uiChannel);
                if (iRet != IVS_NETSOURCE_RESULT.SUCCESS)
                {
                    logEx.Error("eLTEVideoMonitor.IVS_NETSOURCE_GetChannel failed. cameraCode :{0}", cameraCode);
                    return CgwConst.T28181_ERROR_HANDLE;
                }

                Marshal.Copy(new int[1] { Convert.ToInt32(uiChannel) }, 0, pUser, 1);

                //设置媒体流回调函数
                NetSourcedInterface.IVS_NETSOURCE_SetDataCallBack(uiChannel, realPlayCallback, pUser);
                //异常回调函数
                NetSourcedInterface.IVS_NETSOURCE_SetExceptionCallBack(netExceptionCallBack, pUser);


                //协议类型 UDP
                iRet = NetSourcedInterface.IVS_NETSOURCE_SetProtocolType(uiChannel, 1);
                if (iRet != IVS_NETSOURCE_RESULT.SUCCESS)
                {
                    logEx.Error("eLTEVideoMonitor.IVS_NETSOURCE_SetProtocolType failed. cameraCode :{0}", cameraCode);
                    Marshal.FreeHGlobal(pUser);
                    return CgwConst.T28181_ERROR_HANDLE;
                }

                iRet = NetSourcedInterface.IVS_NETSOURCE_SetLocalRecvStreamIP(uiChannel, localIP);
                if (iRet != IVS_NETSOURCE_RESULT.SUCCESS)
                {
                    logEx.Error("eLTEVideoMonitor.IVS_NETSOURCE_SetLocalRecvStreamIP failed. cameraCode :{0}", cameraCode);
                    Marshal.FreeHGlobal(pUser);
                    return CgwConst.T28181_ERROR_HANDLE;
                }

                iRet = NetSourcedInterface.IVS_NETSOURCE_SetLocalRecvStreamPort(uiChannel, uiLocalRecvStreamVideoRtpPort, uiLocalRecvStreamAudioRtpPort);
                if (iRet != IVS_NETSOURCE_RESULT.SUCCESS)
                {
                    logEx.Error("eLTEVideoMonitor.IVS_NETSOURCE_GetLocalRecvStreamPort failed. cameraCode :{0}", cameraCode);
                    Marshal.FreeHGlobal(pUser);
                    return CgwConst.T28181_ERROR_HANDLE;
                }

                iRet = NetSourcedInterface.IVS_NETSOURCE_OpenNetStream(uiChannel, pRemoteSendStreamIp, uiRemoteSendStreamVideoPort, uiRemoteSendStreamAudioPort);
                if (iRet != IVS_NETSOURCE_RESULT.SUCCESS)
                {
                    logEx.Error("eLTEVideoMonitor.IVS_NETSOURCE_OpenNetStream failed. cameraCode :{0}", cameraCode);
                    Marshal.FreeHGlobal(pUser);
                    return CgwConst.T28181_ERROR_HANDLE;
                }

                iRet = NetSourcedInterface.IVS_NETSOURCE_StartRecvStream(uiChannel);
                if (iRet == IVS_NETSOURCE_RESULT.SUCCESS)
                {
                    channelInPtrDic.Add(uiChannel, pUser);
                    logEx.Info("eLTEVideoMonitor.IVS_NETSOURCE_StartRecvStream pChannel: {0}, cameraCode :{1} ", uiChannel, cameraCode);
                    return uiChannel;
                }
                else
                {
                    logEx.Error("eLTEVideoMonitor.StartRecvStream failed. cameraCode :{0}", cameraCode);
                    this.lteSdkClient.StopP2PVideo(Convert.ToInt32(cameraCode));
                    Marshal.FreeHGlobal(pUser);
                    return CgwConst.T28181_ERROR_HANDLE;
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("StartRecvStream failed.Exception message:{0},cameraCode :{0}", ex.Message, cameraCode);
                Marshal.FreeHGlobal(pUser);
                return CgwConst.T28181_ERROR_HANDLE;
            }
        }

        /// <summary>
        /// 获取本机ip
        /// </summary>
        /// <returns></returns>
        static private string GetLocalIP(string serverIP, int serverPort, int localPort)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: eLTEVideoMonitor.GetLocalIP().");

            try
            {
                IPAddress serverAddress = IPAddress.Parse(serverIP);
                //IPAddress[] arrIPAddresses = Dns.GetHostAddresses(Dns.GetHostName());
                IPAddress[] arrIPAddresses = Dns.GetHostAddresses(System.Environment.MachineName);
                foreach (IPAddress ip in arrIPAddresses)
                {
                    if (serverAddress.AddressFamily.Equals(AddressFamily.InterNetwork) && ip.AddressFamily.Equals(serverAddress.AddressFamily))
                    {
                        #region IPv4
                        IPEndPoint serverEndpoint = new IPEndPoint(serverAddress, serverPort);
                        Socket socketClient = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
                        IPEndPoint clientEndpoint = new IPEndPoint(ip, localPort);
                        socketClient.Bind(clientEndpoint);
                        try
                        {
                            socketClient.Connect(serverEndpoint);
                            if (socketClient.Connected)
                            {
                                logEx.Trace("eLTEVideoMonitor.try GetLocalIP ={0}.", ip.ToString());
                                return ip.ToString();
                            }
                            else
                            {
                                logEx.Info("socketClient is not Connected.");
                            }
                        }
                        catch (SocketException e)
                        {
                            logEx.Warn("eLTEVideoMonitor.GetLocalIP failed,e.ErrorCode:{0}", e.ErrorCode);

                            if (e.ErrorCode == 10061 || e.ErrorCode == 10048)   //Even No connection could be made because the target machine actively refused it ,but IP is valid.
                            {
                                logEx.Trace("eLTEVideoMonitor.catch GetLocalIP ={0}.", ip.ToString());
                                return ip.ToString();
                            }
                        }
                        finally
                        {
                            socketClient.Close();
                            logEx.Info("eLTEVideoMonitor.GetLocalIP finally,Close the socketClient success");
                        }
                        #endregion
                    }
                    //如果serverIP为IPv6
                    else if (serverAddress.AddressFamily.Equals(AddressFamily.InterNetworkV6) && ip.AddressFamily.Equals(serverAddress.AddressFamily))
                    {
                        #region IPV6
                        IPEndPoint serverEndpoint = new IPEndPoint(serverAddress, serverPort);
                        Socket socketClient = new Socket(AddressFamily.InterNetworkV6, SocketType.Stream, ProtocolType.Tcp);
                        IPEndPoint clientEndpoint = new IPEndPoint(ip, localPort);
                        socketClient.Bind(clientEndpoint);
                        try
                        {
                            socketClient.Connect(serverEndpoint);
                            if (socketClient.Connected)
                            {
                                logEx.Trace("SipStackAdapter.try GetLocalIP ={0}.", ip.ToString());
                                return ip.ToString();
                            }
                        }
                        catch (SocketException e)
                        {
                            //logEx.Warn("SipStackAdapter.GetLocalIP failed,e.ErrorCode:{0}", e.ErrorCode);

                            if (e.ErrorCode == 10061 || e.ErrorCode == 10048)   //Even No connection could be made because the target machine actively refused it ,but IP is valid.
                            {
                                logEx.Trace("SipStackAdapter.catch GetLocalIP ={0}.", ip.ToString());
                                return ip.ToString();
                            }

                            logEx.Error("SipStackAdapter.GetLocalIP failed.catch Execption message:{0}", e.Message);
                        }
                        finally
                        {
                            socketClient.Close();
                            logEx.Info("SipStackAdapter.GetLocalIP finally,Close the socketClient success");
                        }
                        #endregion
                    }
                }
                return null;
            }
            catch (System.Exception ex)
            {
                logEx.Error("eLTEVideoMonitor.GetLocalIP failed.serverIP={0},serverPort={1},localPort={2},Execption message:{0}", serverIP, serverPort, localPort, ex.Message);
                return null;
            }
        }

        /// <summary>
        /// 异常回调函数
        /// </summary>
        /// <param name="ulChannel">通道号</param>
        /// <param name="iMsgType">异常消息类型</param>
        /// <param name="pParam">对应异常的信息，可扩展为结构体指针</param>
        /// <param name="pUser">用户数据</param>
        static private void NetExceptionCallBackFun(UInt32 ulChannel, UInt32 iMsgType, IntPtr pParam, IntPtr pUser)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Error("NetExceptionCallBack error, ulChannel = {0}", ulChannel);
        }

        /// <summary>
        /// eLTE实况回调，获取得到实况的RTP包、处理获取到的rtp数据包
        /// </summary>
        /// <param name="pEventBuf">RTP字节数组包</param>
        /// <param name="uiSize">RTP包的大小</param>
        /// <param name="pUser">用户数据,存储rtp收流通道</param>
        static private void RealPlayCallBackRawFun(IntPtr pEventBuf, UInt32 uiSize, IntPtr pUser)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            try
            {
                //获取播放通道
                int[] pChannel = new int[1];
                Marshal.Copy(pUser, pChannel, 0, 1);
                uint channel = (uint)pChannel[0];

                //logEx.Debug("RealPlayCallBackRawFun uiSize = {0},channel = {1}", uiSize, channel);
                //进行rtp包转码
                rtpAdapter.ESDK_RTP_ProcessPacket(pEventBuf, uiSize, channel);
            }
            catch (System.Exception ex)
            {
                logEx.Error("RealPlayCallBackRawFun failed.Execption message:{0}", ex.Message);
            }
        }

        /// <summary>
        /// rtp码流回调处理
        /// </summary>
        /// <param name="pBuf">帧数据字节数组</param>
        /// <param name="pFrameData">帧数据类型</param>
        /// <param name="uiChannel">通道</param>
        /// <param name="uiBufSize">帧数据字节数组长度</param>
        private void FrameDataCallBackFun(IntPtr pBuf, uint uiBufSize, ref ST_FRAME_DATA pFrameData, uint uiChannel)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            ST_FRAME_DATA frameData = pFrameData;

            MediaDataSender mediaDataSender = null;
            if (this.handelOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (this.videoChannelDataSenderDic.ContainsKey(uiChannel))
                    {
                        mediaDataSender = this.videoChannelDataSenderDic[uiChannel];
                    }
                }
                finally
                {
                    this.handelOperateLock.ExitReadLock();
                }
            }

            if (mediaDataSender == null)
            {
                logEx.Warn("FrameDataCallBackFun mediaDataSender = NULL");
                return;
            }

            StreamType streamType = StreamType.VIDEO_H264;
            //对于支持的码流类型，用break退出switch，对于不支持的码流类型直接舍弃，用return返回
            switch (frameData.iStreamType)
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
                    logEx.Warn("FrameDataCallBackFun.iStreamType is not valid");
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
                                // logEx.Warn("This data is audio,but the mic is off.Chuck the data.Camera no:{0}", mediaDataSender.CameraNo);
                                return;
                            }
                        }
                        else
                        {
                            //默认为关闭状态，因此如果cameraMicStatusDic不包含该摄像头，则认为处于关闭状态，舍弃音频码流
                            // logEx.Warn("This data is audio,but the mic is off.Chuck the data.Camera no:{0}", mediaDataSender.CameraNo);
                            return;
                        }
                    }
                    finally
                    {
                        this.micOperateLock.ExitReadLock();
                    }
                }
            }

            try
            {
                MediaData mediaData = new MediaData();

                //获取非托管的数据 
                byte[] datagram = new byte[uiBufSize];
                Marshal.Copy(pBuf, datagram, 0, (int)uiBufSize);

                //视频数据增加头信息
                if (!(streamType == StreamType.AUDIO_G711A || streamType == StreamType.AUDIO_G711U))
                {
                    //头部增加四个四节的开始表实0x000001
                    byte[] newDatagram = new byte[uiBufSize + 4];
                    datagram.CopyTo(newDatagram, 4);
                    newDatagram[3] = 1;
                    mediaData.Data = newDatagram;
                    mediaData.Size = (uint)(uiBufSize + 4);
                }
                else
                {
                    mediaData.Data = datagram;
                    mediaData.Size = (uint)(uiBufSize);
                }
                //裸码流
                mediaData.DataType = MediaDataType.FRAME_DATA;
                mediaData.StreamType = streamType;


                //将帧类型转换成各融合网关统一的帧类型
                string name = Enum.GetName(typeof(IvsH264NaluType), frameData.iFrameDataType);
                if (Enum.IsDefined(typeof(FrameDataType), name))
                {
                    FrameDataType frameDataType = (FrameDataType)Enum.Parse(typeof(FrameDataType), name);
                    mediaData.FrameType = frameDataType;
                }
                else
                {
                    mediaData.FrameType = FrameDataType.H264_NALU_TYPE_UNDEFINED;
                    logEx.Warn("eLTE FrameDataCallBackFun FrameType is Not Defined, FrameType:{0}", frameData.iFrameDataType);
                }

                //logEx.Debug("FrameDataCallBackFun.mediaData.DataType={0},FrameType = {1},StreamType = {2},Size = {3}", Enum.GetName(typeof(MediaDataType), mediaData.DataType),
                //             Enum.GetName(typeof(FrameDataType), mediaData.FrameType), Enum.GetName(typeof(StreamType), mediaData.StreamType), mediaData.Size);
                //向回调函数转发码流
                mediaDataSender.SendData(mediaData, this.sender);
            }
            catch (System.Exception ex)
            {
                logEx.Error("FrameDataCallBackFun failed.Execption message:{0}", ex.Message);
            }
        }

        /// <summary>
        /// 刷新监控摄像头列表
        /// </summary>
        /// <returns></returns>
        public SmcErr RefreshMonitorCamera()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: eLTEVideoMonitor.RefreshMonitorCamera");
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
            else
            {
                logEx.Info("eLTEVideoMonitor.RefreshMonitorCamera Wait Lock");
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
            logEx.Trace("Enter: eLTEVideoMonitor.GetRefreshStatus");
            SmcErr err = new CgwError();
            refreshStatus = new SmcErr();
            refreshStatus.ErrNo = CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END;

            if (refreshMonitorCameraOperateLock.TryEnterWriteLock(CgwConst.EFRESH_MONITOR_CAMERA_WAIT_TIME))
            {
                try
                {
                    if (isLoginSuccess)
                    {
                        refreshStatus.ErrNo = isRefreshSuccess ? CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_END : CgwError.ERR_DEVICE_LIST_REFRESH_STATUS_FAILED;
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
    }
}
