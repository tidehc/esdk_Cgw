/*-----------------------------------------------------------------------
//文件名：SipStackAdapter.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-3-06
//描述：用于适配底层Sip Stack,向上提供接口供T28182模块使用
//主要功能接口包括：连接、注册、发送消息、邀请等
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net;
using System.Runtime.InteropServices;
using CgwMonitorManage.Common;
using System.Net.Sockets;
using System.Threading;
using System.Xml;

namespace CgwMonitorManage.T28181
{
    public class SipStackAdapter
    {
        /// <summary>
        /// 刷新设备成功
        /// </summary>
        public bool isRefreshSucess = true;

        /// <summary>
        /// 计时器，用于保活
        /// </summary>
        private System.Timers.Timer keepLiveTimer = new System.Timers.Timer();

        /// <summary>
        /// 日志
        /// </summary>
        private static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        /// <summary>
        /// 保存播放通道跟ResponseID的关系，key为通道，value为ResponseID，一对一的关系
        /// </summary>
        private Dictionary<UInt32, int> channelResponseIDDic = new Dictionary<UInt32, int>();

        /// <summary>
        /// 保存播放通道跟用户数据指针的关系，key为通道，value为用户数据指针，一对一的关系
        /// </summary>
        private Dictionary<UInt32, IntPtr> channelInPtrDic = new Dictionary<UInt32, IntPtr>();

        /// <summary>
        /// 设备列表操作锁
        /// </summary>
        static private ReaderWriterLockSlim deviceItemOperateLock = new ReaderWriterLockSlim();

        /// <summary>
        /// 接收到所有设备事件
        /// </summary>
        static private event EventHandler ReceivedAllDevice;

        /// <summary>
        /// 设备列表总数
        /// </summary>
        static private int deviceNum = 0;

        /// <summary>
        /// 设备列表
        /// </summary>
        static List<DeviceItem> deviceList = new List<DeviceItem>();
        public List<DeviceItem> DeviceList
        {
            get
            {
                return deviceList;
            }
            set { deviceList = value; }
        }

        /// <summary>
        /// 设置实况rtp数据包回调，向上层传递rtp数据包
        /// </summary>
        static NET_DATA_CALLBACK cbDataCallBack;

        /// <summary>
        /// rtp数据包接收异常
        /// </summary>
        static NET_EXCEPTION_CALLBACK cbNetExceptionCallBack;

        /// <summary>
        /// sip消息回调,定义为静态，否则sipDLL异常
        /// </summary>
        static SIP_CallBack sip_CallBack;

        /// <summary>
        /// sip invite消息catelog SN
        /// </summary>
        private int iCateLogSN = 1;

        /// <summary>
        /// sip invite消息 keepliver SN
        /// </summary>
        private int KeepaliveSN = 1;

        /// <summary>
        /// sip invite消息 ptz SN
        /// </summary>
        private int DeviceControlSN = 1;

        /// <summary>
        /// 获取本地IP地址
        /// </summary>
        private string localIP = null;

        /// <summary>
        /// 初始化SIP协议栈
        /// </summary>
        public EM_SIP_RESULT SIP_SDK_Init(string pSipAccount, string pSipPasswd, string pLocalID, int iLocalPort, string pServerSipAccount, string pServerSipPasswd, string pServerID, string pServerIP, int iServerPort, EventHandler receivedAllDevice)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.SIP_SDK_Init().");
            EM_SIP_RESULT iRet = EM_SIP_RESULT.RET_FAILURE;
            try
            {
                //string pLocalIP = GetLocalIP(pServerIP, iServerPort, iLocalPort);
                if (string.IsNullOrEmpty(localIP))
                {
                    localIP = GetLocalIP(pServerIP, iServerPort, iLocalPort);
                }
                ReceivedAllDevice = receivedAllDevice;

                sip_CallBack = SIP_CallBackMethod;

                iRet = SipSDKInterface.SIP_SDK_Init(pSipAccount, pSipPasswd, pLocalID, localIP, iLocalPort, pServerSipAccount, pServerSipPasswd, pServerID, pServerIP, iServerPort, sip_CallBack);
                if (iRet != EM_SIP_RESULT.RET_SUCCESS)
                {
                    logEx.Error("SipStackAdapter.SIP_SDK_Init Failed ,pAccount:{0}", pSipAccount);
                }

                //设置NETSOURCE日志目录
                IVS_NETSOURCE_RESULT iNet = NetSourcedInterface.IVS_NETSOURCE_Init(AppDomain.CurrentDomain.BaseDirectory + "\\logs");
                if (iNet != IVS_NETSOURCE_RESULT.SUCCESS)
                {
                    logEx.Error("SipStackAdapter.IVS_NETSOURCE_Init Failed,pAccount:{0}", pSipAccount);
                    iRet = EM_SIP_RESULT.RET_FAILURE;
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.SIP_SDK_Init ,Exception:{0}", ex.ToString());
                iRet = EM_SIP_RESULT.RET_FAILURE;
            }
            return iRet;
        }

        /// <summary>
        /// 消息通知回调函数结果
        /// </summary>
        /// <param name="msgType"></param>
        /// <param name="eventType"></param>
        /// <param name="pPara"></param>
        /// <returns></returns>
        private EM_SIP_RESULT SIP_CallBackMethod(int msgType, int eventType, StringBuilder pPara)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.SIP_CallBackMethod");

            try
            {
                //收到的消息类型
                EM_SIP_MSG_TYPE sipMsgType = (EM_SIP_MSG_TYPE)msgType;
                EM_SIP_REQUEST_EVENT sipEventType = (EM_SIP_REQUEST_EVENT)eventType;

                if (sipMsgType == EM_SIP_MSG_TYPE.SIP_REQUEST_MSG && sipEventType == EM_SIP_REQUEST_EVENT.SIP_REQ_MESSAGE)
                {
                    ParameterizedThreadStart para = new ParameterizedThreadStart(SIP_CallBackThreadMethod);
                    Thread thread = new Thread(para);
                    thread.Priority = ThreadPriority.Highest;
                    thread.Start(pPara.ToString());
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.SIP_CallBackMethod ,Exception:{0}", ex.ToString());
                return EM_SIP_RESULT.RET_FAILURE;
            }
            return EM_SIP_RESULT.RET_SUCCESS;
        }

        /// <summary>
        /// 线程解析返回的xml
        /// </summary>
        /// <param name="oPara">xml消息</param>
        private void SIP_CallBackThreadMethod(object oPara)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            //logEx.Trace("Enter: SipStackAdapter.SIP_CallBackThreadMethod.pPara = {0}", oPara);

            try
            {
                string strBody = oPara as string;
                if (strBody == null)
                {
                    return;
                }
                if (strBody.IndexOf("Catalog") > -1)
                {
                    //logEx.Info("SipStackAdapter.SIP_CallBackThreadMethod strBody ={0}", strBody);

                    try
                    {
                        if (deviceItemOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                        {
                            //首次返回需要获取设备总数 
                            if (deviceNum == 0)
                            {
                                deviceNum = GetAllDeviceNum(strBody);
                                if (deviceNum == 0)
                                {
                                    logEx.Trace("SipStackAdapter.GetDeviceItem Start ReceivedAllDevice");
                                    // 接收完所有设备，通知上层处理单元
                                    ReceivedAllDevice(null, null);
                                    return;
                                }
                            }
                            if (deviceNum <= deviceList.Count)
                            {
                                return;
                            }

                            //获取返回的数据
                            GetDeviceItem(strBody);

                        }
                    }
                    finally
                    {
                        if (deviceItemOperateLock.IsWriteLockHeld)
                        {
                            deviceItemOperateLock.ExitWriteLock();
                        }                       
                    }
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.SIP_CallBackThreadMethod ,Exception:{0}", ex.ToString());
            }
        }
        /// <summary>
        /// 该函数用于去初始化SIP协议栈
        /// </summary>
        public EM_SIP_RESULT SIP_SDK_UnInit()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.SIP_SDK_UnInit().");
            EM_SIP_RESULT iRet = EM_SIP_RESULT.RET_FAILURE;
            try
            {
                iRet = SipSDKInterface.SIP_SDK_UnInit();
                if (iRet != EM_SIP_RESULT.RET_SUCCESS)
                {
                    logEx.Error("SipStackAdapter.SIP_SDK_UnInit,Error = {0}.", Enum.GetName(typeof(EM_SIP_RESULT), iRet));
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.SIP_SDK_UnInit,Exception:{0}", ex.ToString());
                iRet = EM_SIP_RESULT.RET_FAILURE;
            }
            return iRet;
        }

        /// <summary>
        /// 该函数用于发送SIP Register消息
        /// </summary>
        public EM_SIP_RESULT SIP_SDK_REGISTER()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.SIP_SDK_REGISTER().");
            EM_SIP_RESULT iRet = EM_SIP_RESULT.RET_FAILURE;
            try
            {
                iRet = SipSDKInterface.SIP_SDK_REGISTER();

                if (iRet != EM_SIP_RESULT.RET_SUCCESS)
                {
                    logEx.Error("SipStackAdapter.SIP_SDK_REGISTER Error = {0}.", Enum.GetName(typeof(EM_SIP_RESULT), iRet));
                }
                else
                {
                    logEx.Info("SipStackAdapter.SIP_SDK_REGISTER Success");
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.SIP_SDK_REGISTER ,Exception:{0}", ex.ToString());
                iRet = EM_SIP_RESULT.RET_FAILURE;
            }
            return iRet;
        }

        /// <summary>
        /// sip保活
        /// </summary>
        /// <param name="localSignalGateway">网关对接T28181监控平台时，网关视为监控平台外域，该编码是标识网关的域编码，即是监控平台配置界面添加网关时的外域编码。</param>
        public void StartKeepalive(string serverID, string localID)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: T28181VideoMonitor.StartConnectRegisterSip().");
            EM_SIP_RESULT iRet = EM_SIP_RESULT.RET_FAILURE;
            //初始话定时器
            this.keepLiveTimer.AutoReset = true;
            this.keepLiveTimer.Elapsed += new System.Timers.ElapsedEventHandler((x, y)
                =>
                {
                    //sip保活,deviceID为用户名
                    string strBody = @"<?xml version='1.0'?><Notify><CmdType>Keepalive</CmdType><SN>" + KeepaliveSN + "</SN><DeviceID>" + localID + "</DeviceID><Status>OK</Status></Notify>";
                    KeepaliveSN++;
                    iRet = SipSDKInterface.SIP_SDK_MESSAGE(serverID, strBody);
                    if (iRet != EM_SIP_RESULT.RET_SUCCESS)
                    {
                        logEx.Warn("SipStackAdapter.Keepalive Failed");
                        //保活失败，重新注册. zWX231378 .2015/6/24
                        SIP_SDK_REGISTER();
                    }
                    else
                    {
                        logEx.Trace("SipStackAdapter.Keepalive Success");
                    }
                });
            //保活时间为10秒
            this.keepLiveTimer.Interval = CgwConst.Thread_Sleep_Time * 10;
            keepLiveTimer.Start();
        }
        /// <summary>
        /// 该函数用于发送SIP UNRegister消息，有效期为0
        /// </summary>
        public EM_SIP_RESULT SIP_SDK_UNREGISTER()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.SIP_SDK_UNREGISTER().");
            EM_SIP_RESULT iRet = EM_SIP_RESULT.RET_FAILURE;
            try
            {
                iRet = SipSDKInterface.SIP_SDK_UNREGISTER();
                //停止定时器
                this.keepLiveTimer.Stop();

                if (iRet != EM_SIP_RESULT.RET_SUCCESS)
                {
                    logEx.Error("SipStackAdapter.SIP_SDK_UNREGISTER Error = {0}.", Enum.GetName(typeof(EM_SIP_RESULT), iRet));
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.SIP_SDK_UNREGISTER ,Exception:{0}", ex.ToString());
                iRet = EM_SIP_RESULT.RET_FAILURE;
            }
            return iRet;
        }

        /// <summary>
        /// 该函数用于发送SIP Invite消息
        /// </summary>
        /// <param name="pAccount">对方账号</param>
        /// <param name="pSdpBody">SDP描述字符串</param>
        public EM_SIP_RESULT SIP_SDK_INVITE(string pAccount, string pSdpBody, ref int iResponseID, StringBuilder strRemoteBody)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.SIP_SDK_INVITE().");
            iResponseID = 0;
            EM_SIP_RESULT iRet = EM_SIP_RESULT.RET_FAILURE;
            try
            {
                iRet = SipSDKInterface.SIP_SDK_INVITE(pAccount, pSdpBody, ref iResponseID, strRemoteBody);
                SIP_SDK_ACK(iResponseID);
                if (iRet != EM_SIP_RESULT.RET_SUCCESS)
                {
                    logEx.Error("SipStackAdapter.SIP_SDK_INVITE ,pSdpBody:{0}", pSdpBody);
                }
                else
                {
                    logEx.Info("SipStackAdapter.SIP_SDK_INVITE ,iResponseID={0}", iResponseID);

                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.SIP_SDK_INVITE ,Exception:{0}", ex.ToString());
                iRet = EM_SIP_RESULT.RET_FAILURE;
            }
            return iRet;
        }

        /// <summary>
        /// 该函数用于发送SIP ACK消息
        /// </summary>
        /// <param name="iResponseID">iResponseID</param>
        public EM_SIP_RESULT SIP_SDK_ACK(int iResponseID)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.SIP_SDK_ACK().");
            EM_SIP_RESULT iRet = EM_SIP_RESULT.RET_FAILURE;
            try
            {
                iRet = SipSDKInterface.SIP_SDK_ACK(iResponseID);

                if (iRet != EM_SIP_RESULT.RET_SUCCESS)
                {
                    logEx.Error("SipStackAdapter.SIP_SDK_ACK ,iResponseID:{0}", iResponseID);
                }
                else
                {
                    logEx.Info("SipStackAdapter.SIP_SDK_ACK ,iResponseID={0}", iResponseID);
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.SIP_SDK_ACK ,Exception:{0}", ex.ToString());
                iRet = EM_SIP_RESULT.RET_FAILURE;
            }
            return iRet;
        }

        /// <summary>
        /// 云台控制
        /// </summary>
        /// <param name="deviceID">设备编码</param>
        /// <param name="PTZCmd">云台控制命令</param>
        /// <param name="controlPriority">优先级</param>
        /// <returns></returns>
        public EM_SIP_RESULT PtzControl(string deviceID, string ptzCmd, string controlPriority)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.PtzControl deviceID = {0}.", deviceID);
            EM_SIP_RESULT iRet = EM_SIP_RESULT.RET_FAILURE;
            try
            {
                //设备目录查询消息体
                string xmlstr = @"<?xml version='1.0'?><Control><CmdType>DeviceControl</CmdType><SN>" + DeviceControlSN + "</SN><DeviceID>" + deviceID + @"</DeviceID>
                                    <PTZCmd>" + ptzCmd + @"</PTZCmd>
                                    <Info>
                                    <ControlPriority>" + controlPriority + @"</ControlPriority>
                                    </Info></Control>";
                DeviceControlSN++;
                iRet = SipSDKInterface.SIP_SDK_MESSAGE(deviceID, xmlstr);
                if (iRet != EM_SIP_RESULT.RET_SUCCESS)
                {
                    logEx.Error("SipStackAdapter.PtzControl Error = {0}.", Enum.GetName(typeof(EM_SIP_RESULT), iRet));
                }
                logEx.Trace("Leave: SipStackAdapter.PtzControl deviceID = {0}.", deviceID);
            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.PtzControl Error = {0}.", ex.ToString());
                iRet = EM_SIP_RESULT.RET_FAILURE;
            }
            return iRet;
        }

        /// <summary>
        /// 该函数用于发送SIP BYE消息
        /// </summary>
        public EM_SIP_RESULT SIP_SDK_BYE(int responseID)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.SIP_SDK_BYE() responseID ={0}.", responseID);
            EM_SIP_RESULT iRet = EM_SIP_RESULT.RET_FAILURE;
            try
            {
                iRet = SipSDKInterface.SIP_SDK_BYE(responseID);

                if (iRet != EM_SIP_RESULT.RET_SUCCESS)
                {
                    logEx.Error("SipStackAdapter.SIP_SDK_BYE Error = {0}.", Enum.GetName(typeof(EM_SIP_RESULT), iRet));
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.SIP_SDK_BYE,Exception:{0}", ex.ToString());
                iRet = EM_SIP_RESULT.RET_FAILURE;
            }
            return iRet;
        }

        /// <summary>
        /// 该函数用于发送subscribe消息
        /// </summary>
        public EM_SIP_RESULT SIP_SDK_Subscribe(string pBody)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.SIP_SDK_Subscribe().");
            EM_SIP_RESULT iRet = EM_SIP_RESULT.RET_FAILURE;
            try
            {
                iRet = SipSDKInterface.SIP_SDK_Subscribe(pBody);

                if (iRet != EM_SIP_RESULT.RET_SUCCESS)
                {
                    logEx.Error("SipStackAdapter.SIP_SDK_Subscribe ,pBody:{0}", pBody);
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.SIP_SDK_Subscribe ,Exception:{0}", ex.ToString());
                iRet = EM_SIP_RESULT.RET_FAILURE;
            }
            return iRet;
        }

        /// <summary>
        /// 获取本机ip
        /// </summary>
        /// <returns></returns>
        private string GetLocalIP(string serverIP, int serverPort, int localPort)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.GetLocalIP().");

            try
            {
                IPAddress serverAddress = IPAddress.Parse(serverIP);
                IPAddress[] arrIPAddresses = Dns.GetHostAddresses(System.Environment.MachineName);
                foreach (IPAddress ip in arrIPAddresses)
                {
                    //如果serverIP为IPv4
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
                logEx.Error("SipStackAdapter.GetLocalIP failed.Execption message:{0}", ex.Message);
                return null;
            }
        }

        /// <summary>
        /// 根据消息体解析设备
        /// </summary>
        /// <param name="strBody">查询返回的消息</param>
        private static void GetDeviceItem(string strBody)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.GetDeviceItem().");

            //解析返回的XML数据返回数据对象
            XmlNode node = XmlUtil.ProcessResponseMsg(strBody, "DeviceList");
            List<DeviceItem> list = XmlUtil.DeserializeToList<DeviceItem>(node);

            if (list == null)
            {
                return;
            }

            try
            {
                foreach (DeviceItem item in list)
                {
                    if (!deviceList.Contains(item, new DeviceItemCompare()))
                    {
                        deviceList.Add(item);
                    }
                    else
                    {
                        logEx.Warn("SipStackAdapter.GetDeviceItem DeviceItem.DeviceID = {0} duplicate", item.DeviceID);
                        return;
                    }
                }

                //获取数据完成
                if (deviceNum == deviceList.Count)
                {
                    if (ReceivedAllDevice != null)
                    {
                        logEx.Trace("SipStackAdapter.GetDeviceItem Start ReceivedAllDevice");
                        // 接收完所有设备，通知上层处理单元
                        ReceivedAllDevice(null, null);

                        //回收系统内存
                        GC.Collect();
                        GC.WaitForFullGCComplete(CgwConst.Thread_Sleep_Time);
                    }
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter Get all cameras failed.Execption message:{0}", ex.Message);
            }
        }
        /// <summary>
        /// 获取查询出的设备总数
        /// </summary>
        /// <param name="strBody">消息体</param>
        /// <returns>返回设备数量</returns>
        private static int GetAllDeviceNum(string strBody)
        {
            string str = XmlUtil.GetNodeValue(strBody, "SumNum");
            if (str != string.Empty)
            {
                return Convert.ToInt32(str);
            }
            else
            {
                return 0;
            }
        }


        /// <summary>
        /// 打开通道，开始接收实况RTP数据流
        /// </summary>
        /// <param name="cameraCode">摄像机编码</param>
        /// <returns></returns>
        public UInt32 StartRecvStream(string cameraCode, string domain, string iSipPort, string localPort)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.StartRecvStream  cameraCode :{0},domain = {1},iSipPort ={2}", cameraCode, domain, iSipPort);

            try
            {
                //接收视频码流RTP端口号
                uint uiLocalRecvStreamVideoRtpPort = 0;
                //接收音频码流RTP端口号
                uint uiLocalRecvStreamAudioRtpPort = 0;
                //通道号
                UInt32 uiChannel = 0;
                //string strLocal = GetLocalIP(domain, Convert.ToInt32(iSipPort), Convert.ToInt32(localPort));
                
                if (string.IsNullOrEmpty(localIP))
                {
                    localIP = GetLocalIP(domain, Convert.ToInt32(iSipPort), Convert.ToInt32(localPort));
                }
                string strLocal = localIP;

                //获取可用通道
                NetSourcedInterface.IVS_NETSOURCE_GetChannel(ref uiChannel);

                //用户参数,4字节整数
                IntPtr pUser = Marshal.AllocHGlobal(4);
                Marshal.Copy(new int[1] { Convert.ToInt32(uiChannel) }, 0, pUser, 1);

                //设置媒体流回调函数
                NetSourcedInterface.IVS_NETSOURCE_SetDataCallBack(uiChannel, cbDataCallBack, pUser);
                //异常回调函数
                IVS_NETSOURCE_RESULT iRet = NetSourcedInterface.IVS_NETSOURCE_SetExceptionCallBack(cbNetExceptionCallBack, pUser);

                //协议类型 UDP
                iRet = NetSourcedInterface.IVS_NETSOURCE_SetProtocolType(uiChannel, 1);
                if (iRet != IVS_NETSOURCE_RESULT.SUCCESS)
                {
                    logEx.Error("SipStackAdapter.StartRecvStream.IVS_NETSOURCE_SetProtocolType failed. cameraCode :{0},domain = {1},iSipPort ={2}", cameraCode, domain, iSipPort);
                    return CgwConst.T28181_ERROR_HANDLE;
                }

                iRet = NetSourcedInterface.IVS_NETSOURCE_SetLocalRecvStreamIP(uiChannel, localIP);
                if (iRet != IVS_NETSOURCE_RESULT.SUCCESS)
                {
                    logEx.Error("SipStackAdapter.StartRecvStream.IVS_NETSOURCE_SetLocalRecvStreamIP failed. cameraCode :{0},domain = {1},iSipPort ={2}", cameraCode, domain, iSipPort);
                    return CgwConst.T28181_ERROR_HANDLE;
                }

                iRet = NetSourcedInterface.IVS_NETSOURCE_GetLocalRecvStreamPort(uiChannel, ref  uiLocalRecvStreamVideoRtpPort, ref  uiLocalRecvStreamAudioRtpPort);
                if (iRet != IVS_NETSOURCE_RESULT.SUCCESS)
                {
                    logEx.Error("SipStackAdapter.StartRecvStream.IVS_NETSOURCE_GetLocalRecvStreamPort failed. cameraCode :{0},domain = {1},iSipPort ={2}", cameraCode, domain, iSipPort);
                    return CgwConst.T28181_ERROR_HANDLE;
                }

                //Invite消息体
                string strRealPlay = "v=0\r\n"
                                  + "o=" + cameraCode + @" 0 0 IN IP4 " + strLocal + "\r\n"
                                  + "s=Play\r\n"
                                  + "c=IN IP4 " + strLocal + "\r\n"
                                  + "t=0 0\r\n"
                                  + "m=video " + uiLocalRecvStreamVideoRtpPort + " RTP/AVP 96 98 97\r\n"
                                  + "a=recvonly\r\n"
                                  + "a=rtpmap:96 PS/90000\r\n"
                                  + "a=rtpmap:98 H264/90000\r\n"
                                  + "a=rtpmap:97 MPEG4/90000\r\n";

                //发送SIP Invite消息
                int iResponseID = 0;
                //制定字符串长度为1024
                StringBuilder strRemoteBody = new StringBuilder(1024);
                //发送SIP Invite消息
                EM_SIP_RESULT iSip = SIP_SDK_INVITE(cameraCode, strRealPlay, ref iResponseID, strRemoteBody);
                if (iSip != EM_SIP_RESULT.RET_SUCCESS)
                {
                    logEx.Error("SipStackAdapter.SIP_SDK_INVITE failed. cameraCode :{0},domain = {1},iSipPort ={2}", cameraCode, domain, iSipPort);
                    return CgwConst.T28181_ERROR_HANDLE;
                }

                //源端发送码流IP;
                string pRemoteSendStreamIp = "";
                //源端发送视频码流RTP端口号
                uint uiRemoteSendStreamVideoPort = 0;
                GetInviteRemoteIPPort(strRemoteBody.ToString(), ref pRemoteSendStreamIp, ref uiRemoteSendStreamVideoPort);

                iRet = NetSourcedInterface.IVS_NETSOURCE_OpenNetStream(uiChannel, pRemoteSendStreamIp, uiRemoteSendStreamVideoPort, 0);//无音频，传入0
                if (iRet != IVS_NETSOURCE_RESULT.SUCCESS)
                {
                    logEx.Error("SipStackAdapter.StartRecvStream failed. cameraCode :{0},domain = {1},iSipPort ={2}", cameraCode, domain, iSipPort);
                    return CgwConst.T28181_ERROR_HANDLE;
                }

                iRet = NetSourcedInterface.IVS_NETSOURCE_StartRecvStream(uiChannel);
                if (iRet == IVS_NETSOURCE_RESULT.SUCCESS)
                {
                    channelResponseIDDic.Add(uiChannel, iResponseID);
                    channelInPtrDic.Add(uiChannel, pUser);
                    logEx.Info("SipStackAdapter.StartRecvStream pChannel: {0}, cameraCode :{1},domain = {2},iSipPort ={3} ", uiChannel, cameraCode, domain, iSipPort);
                    return uiChannel;
                }
                else
                {
                    logEx.Error("SipStackAdapter.StartRecvStream failed. cameraCode :{0},domain = {1},iSipPort ={2}", cameraCode, domain, iSipPort);
                    StopRecvStream(uiChannel);
                    Marshal.FreeHGlobal(pUser);
                    return CgwConst.T28181_ERROR_HANDLE;
                }
            }
            catch (System.Exception ex)
            {
                logEx.Error("StartRecvStream failed.Exception message:{0},cameraCode :{0},domain = {1},iSipPort ={2}", ex.Message, cameraCode, domain, iSipPort);
                return CgwConst.T28181_ERROR_HANDLE;
            }
        }

        /// <summary>
        /// 获取收流的IP、端口
        /// </summary>
        /// <param name="strRemoteBody">消息体</param>
        /// <param name="pRemoteSendStreamIp">IP</param>
        /// <param name="uiRemoteSendStreamVideoPort">端口</param>
        private void GetInviteRemoteIPPort(string strRemoteBody, ref string pRemoteSendStreamIp, ref uint uiRemoteSendStreamVideoPort)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.GetInviteRemoteIPPort strRemoteBody = {0}.", strRemoteBody);

            try
            {
                //获取返回值字符串的第六个字段，收流的IP
                string[] strList = strRemoteBody.Split(new string[] { "\r\n" }, StringSplitOptions.None); ;
                foreach (string str in strList)
                {
                    if (str.IndexOf("c=") > -1)
                    {
                        string[] strListIP = str.Split(' ');
                        if (strListIP.Length > 0)
                        {
                            pRemoteSendStreamIp = strListIP[strListIP.Length - 1];
                            pRemoteSendStreamIp = pRemoteSendStreamIp.Trim('\r');
                        }
                    }
                    if (str.IndexOf("m=") > -1)
                    {
                        if (str.IndexOf("video") > -1)
                        {
                            string[] mVideo = str.Split(' ');
                            uint a = 0;
                            if (uint.TryParse(mVideo[1], out a) == true) //判断port是否可以转换为uint
                            {
                                uiRemoteSendStreamVideoPort = Convert.ToUInt32(mVideo[1]);
                                if (uiRemoteSendStreamVideoPort < 0)
                                {
                                    logEx.Error("when m=video ,the port is wrong,it can not be smaller than 0");
                                }
                            }
                            else
                            {
                                logEx.Trace("the port is {0}", a);
                                logEx.Error("when m=video ,the port is wrong,it is not number");
                            }
                        }
                    }
                }
                logEx.Trace("SipStackAdapter.RemoteIP = {0}.", pRemoteSendStreamIp);
                logEx.Trace("Leave: SipStackAdapter.GetInviteRemoteIPPort");
                logEx.Trace("the RemoteIPPort is {0}", uiRemoteSendStreamVideoPort);
            }
            catch (System.Exception ex)
            {
                pRemoteSendStreamIp = "";
                uiRemoteSendStreamVideoPort = 0;
                logEx.Error("GetInviteRemoteIPPort failed.Exception message:{0}", ex.Message);
            }
        }
        /// <summary>
        /// 设置实况rtp数据包回调 
        /// </summary>
        /// <param name="dataCallBack"></param>
        public void SetNetDataCallBack(NET_DATA_CALLBACK dataCallBack, NET_EXCEPTION_CALLBACK netExceptionCallBack)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.SetNetDataCallBack().");
            cbDataCallBack = dataCallBack;
            cbNetExceptionCallBack = netExceptionCallBack;
            logEx.Info("Set SetNetDataCallBack success");
        }

        /// <summary>
        /// 停止接收rtp数据包
        /// </summary>
        /// <param name="cameraCode"></param>
        public EM_SIP_RESULT StopRecvStream(UInt32 channel)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.StopRecvStream channel = {0}.", channel);

            try
            {
                //停止实况
                int responseID = 0;
                foreach (KeyValuePair<UInt32, int> kvp in channelResponseIDDic)
                {
                    if (kvp.Key == channel)
                    {
                        responseID = kvp.Value;
                        channelResponseIDDic.Remove(kvp.Key);
                        break;
                    }
                }
                EM_SIP_RESULT iRet = SIP_SDK_BYE(responseID);
                if (iRet != EM_SIP_RESULT.RET_SUCCESS)
                {
                    logEx.Error("StopRecvStream failed channel={0}", channel);
                }
                //释放用户数据内存 
                foreach (KeyValuePair<UInt32, IntPtr> kvp in channelInPtrDic)
                {
                    if (kvp.Key == channel)
                    {
                        Marshal.FreeHGlobal(kvp.Value);
                        channelInPtrDic.Remove(kvp.Key);
                        break;
                    }
                }

                //释放NETSOURCE通道资源
                IVS_NETSOURCE_RESULT iNet = IVS_NETSOURCE_FreeChannel(channel);
                if (iNet != IVS_NETSOURCE_RESULT.SUCCESS)
                {
                    logEx.Error("IVS_NETSOURCE_FreeChannel failed channel={0}", channel);
                    return EM_SIP_RESULT.RET_FAILURE;
                }
                return iRet;
            }
            catch (System.Exception ex)
            {
                logEx.Error("StopRecvStream failed.Exception message:{0}", ex.Message);
                return EM_SIP_RESULT.RET_FAILURE;
            }
        }

        /// <summary>
        /// 获取系统设备列表
        /// </summary>
        /// <param name="serverSignalGateway">要查询的域编码</param>
        /// <param name="deviceID">要查询的根设备编码</param>
        public void GetDeviceList(string serverSignalGateway, string deviceID)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.GetDeviceList deviceID = {0}.", deviceID);

            try
            {
                //设备总数清空
                if (deviceItemOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                {
                    deviceNum = 0;
                    deviceList.Clear();
                }
            }
            finally
            {
                deviceItemOperateLock.ExitWriteLock();
            }

            try
            {
                //设备目录查询消息体
                string xmlstr = "<?xml version='1.0'?>" +
                    "<Query><CmdType>Catalog</CmdType><SN>" + iCateLogSN + "</SN><DeviceID>" + deviceID + "</DeviceID></Query>";

                iCateLogSN++;
                EM_SIP_RESULT iRet = SipSDKInterface.SIP_SDK_MESSAGE(serverSignalGateway, xmlstr);
                if (iRet != EM_SIP_RESULT.RET_SUCCESS)
                {
                    logEx.Error("SipStackAdapter.GetDeviceList Error = {0}.", Enum.GetName(typeof(EM_SIP_RESULT), iRet));
                    isRefreshSucess = false;
                }
                logEx.Trace("Leave: SipStackAdapter.GetDeviceList deviceID = {0}.", deviceID);
            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.GetDeviceList Error = {0}.", ex.ToString());
                isRefreshSucess = false;
            }
        }

        /// <summary>
        /// 释放所有实况通道,释放NETSOURCE资源
        /// </summary>
        public IVS_NETSOURCE_RESULT IVS_NETSOURCE_UnInit()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.IVS_NETSOURCE_UnInit.");
            //IVS_NETSOURCE_RESULT iRet = IVS_NETSOURCE_RESULT.FAILURE;
            IVS_NETSOURCE_RESULT iRet = IVS_NETSOURCE_RESULT.SUCCESS;
            try
            {
                foreach (KeyValuePair<UInt32, int> kvp in channelResponseIDDic)
                {
                    UInt32 channel = kvp.Key;
                    iRet = NetSourcedInterface.IVS_NETSOURCE_CloseNetStream(channel);
                    if (iRet != IVS_NETSOURCE_RESULT.SUCCESS)
                    {
                        logEx.Error("SipStackAdapter.IVS_NETSOURCE_CloseNetStream channelResponseIDDic = {0}.", channel);
                        //注销return，避免返回，不实现IVS_NETSOURCE_UnInit
                        //return iRet;
                    }
                    iRet = NetSourcedInterface.IVS_NETSOURCE_FreeChannel(channel);
                    if (iRet != IVS_NETSOURCE_RESULT.SUCCESS)
                    {
                        logEx.Error("SipStackAdapter.IVS_NETSOURCE_FreeChannel channelResponseIDDic = {0}.", channel);
                        //注销return，避免返回，不实现IVS_NETSOURCE_UnInit
                        //return iRet;
                    }
                }
                //iRet = NetSourcedInterface.IVS_NETSOURCE_UnInit();
                //if (iRet != IVS_NETSOURCE_RESULT.SUCCESS)
                //{
                //    logEx.Error("SipStackAdapter.IVS_NETSOURCE_UnInit failed.");
                //}

                NetSourcedInterface.IVS_NETSOURCE_UnInit();
                logEx.Info("Leave: SipStackAdapter.IVS_NETSOURCE_UnInit.");

            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.IVS_NETSOURCE_UnInit Error = {0}.", ex.ToString());
                iRet = IVS_NETSOURCE_RESULT.FAILURE;
            }
            return iRet;
        }

        /// <summary>
        /// 释放NETSOURCE通道资源
        /// </summary>
        private IVS_NETSOURCE_RESULT IVS_NETSOURCE_FreeChannel(UInt32 channel)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: SipStackAdapter.IVS_NETSOURCE_FreeChannel.");
            IVS_NETSOURCE_RESULT iRet = IVS_NETSOURCE_RESULT.FAILURE;
            try
            {
                if (channel == 0)
                {
                    return iRet;
                }
                //关闭网络流
                iRet = NetSourcedInterface.IVS_NETSOURCE_CloseNetStream(channel);
                if (iRet != CgwConst.IVS_SDK_SUCCESS_TAG)
                {
                    logEx.Error("SipStackAdapter.IVS_NETSOURCE_CloseNetStream ErrorCode = {0}.", iRet);
                }
                //释放指定通道
                iRet = NetSourcedInterface.IVS_NETSOURCE_FreeChannel(channel);
                if (iRet != CgwConst.IVS_SDK_SUCCESS_TAG)
                {
                    logEx.Error("SipStackAdapter.IVS_NETSOURCE_FreeChannel ErrorCode = {0}.", iRet);
                }

            }
            catch (System.Exception ex)
            {
                logEx.Error("SipStackAdapter.IVS_NETSOURCE_FreeChannel Error = {0}.", ex.ToString());
                iRet = IVS_NETSOURCE_RESULT.FAILURE;
            }
            return iRet;
        }
    }
}
