/*-----------------------------------------------------------------------
//文件名：TiandySdkClient.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：kf70422
//日期：2013-2-27
//描述：跟天地伟业SDK接口转换成C#接口
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using CgwMonitorManage.NLogEx;
using CgwMonitorManage.Common;
using System.Threading;

namespace CgwMonitorManage.Tiandy
{
    #region 结构体定义

    /// <summary>
    /// 云台控制命令结构体，用于跟C接口传递,参数名跟天地伟业接口参数名保持一致
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    internal struct Client_PTZ_Command
    {
        /// <summary>
        /// 命令
        /// </summary>
        public int cmd;

        public int param;
        public bool start;
    }

    [StructLayout(LayoutKind.Sequential)]
    internal struct RealPlayInfo
    {
        /// <summary>
        /// 通道号 
        /// </summary>
        public int ch;

        /// <summary>
        /// 码流类型 0主码流 1子码流
        /// </summary>               
        public int data_type;

        /// <summary>
        /// 播放窗口的句柄, 为NULL表示不播放图象
        /// </summary>
        public int? hPlayWnd;

    }
    #endregion

    internal class TiandySdkClient
    {
        /// <summary>
        /// 日志
        /// </summary>
        private static NLog.Logger log = NLog.LogManager.GetCurrentClassLogger();

        #region C接口引用，转成C#接口
        /// <summary>
        /// 设置本地ID，
        /// 向上接口，直接跟SDK接口对接
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_set_local_id(string localId);

        /// <summary>
        /// 设置解码库，
        /// 向上接口，直接跟SDK接口对接
        /// </summary>
        /// <param name="type">播放器类型(天地、海康、大华)</param>
        /// <param name="lib_name">解码库名称</param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_player_set_lib(int type, string lib_name);

        /// <summary>
        /// 初始化SDK ，
        /// 向上接口，直接跟SDK接口对接
        /// </summary>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_startup();

        /// <summary>
        /// 释放SDK资源
        /// 向上接口，直接跟SDK接口对接
        /// </summary>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_cleanup();

        /// <summary>
        /// 码流回调函数
        /// 向上接口，直接跟SDK接口对接
        /// </summary>
        /// <param name="playfd">实时预览句柄</param>
        /// <param name="datatype">视频流类型 </param>
        /// <param name="buf">数据缓冲</param>
        /// <param name="size">数据缓冲大小</param>
        /// <param name="usr">用户数据</param>
        public delegate void RealDataCallBackFunc(int playfd, int datatype,
                                                  [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)]
                                                  byte[] buf,
                                                  uint size,
                                                  int usr);

        /// <summary>
        /// 启动实时预览(不需要登录设备,不需要建立长连接 )
        /// 向上接口，直接跟SDK接口对接
        /// </summary>
        /// <param name="dev_id">主机ID </param>
        /// <param name="username">用户名,目前无效,传入NULL即可</param>
        /// <param name="password">密码,目前无效,传入NULL即可</param>
        /// <param name="client_sup_id">客户端接入服务器ID </param>
        /// <param name="dev_sup_id">设备端接入服务器ID </param>
        /// <param name="client_sup_ip">客户端接入服务器IP</param>
        /// <param name="client_sup_port">客户端接入服务器端口</param>
        /// <param name="info">实时播放信息</param>
        /// <param name="func">实时流回调</param>
        /// <param name="usr">实时流回调用户数据</param>
        /// <returns>-1 失败，其他则是返回预览句柄</returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern int client_sdk_realplay2_ex(string dev_id, string username, string password, string client_sup_id, string dev_sup_id, string client_sup_ip, int client_sup_port, ref RealPlayInfo info, RealDataCallBackFunc func, int usr);

        /// <summary>
        /// 停止预览，
        /// 向上接口，直接跟SDK接口对接
        /// </summary>
        /// <param name="playfd">预览句柄</param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_stop_realplay(int playfd);

        /// <summary>
        /// 云台控制(需要预览视频) ，针对监控融合，主要是摄像头控制，
        /// 向上接口，直接跟SDK接口对接
        /// </summary>
        /// <param name="playfd">预览句柄</param>
        /// <param name="cmd">控制命令</param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_ptz_control(int playfd, ref Client_PTZ_Command cmd);

        /// <summary>
        /// 云台控制(需要预览视频) ，针对监控融合，主要是摄像头控制，(带访问级别)
        /// 向上接口，直接跟SDK接口对接
        /// </summary>
        /// <param name="playfd">预览句柄</param>
        /// <param name="cmd">控制命令</param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_ptz_control_ex(int playfd, ref Client_PTZ_Command cmd, int _iRightLevel);

        /// <summary>
        /// 重发关键帧
        /// </summary>
        /// <param name="loginfd">登录句柄</param>
        /// <param name="ch">通道号</param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_make_key_frame(int loginfd, int ch);

        /// <summary>
        /// 设置码流回调函数(Tiandy私有码流)
        /// </summary>
        /// <param name="playfd">预览句柄</param>
        /// <param name="func">回调函数</param>
        /// <param name="usr">用户数据</param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_set_realdata_callback(int playfd, RealDataCallBackFunc func, int usr);

        /// <summary>
        /// 设置码流回调函数（H264标准裸码流）
        /// </summary>
        /// <param name="playfd">预览句柄</param>
        /// <param name="func">回调函数</param>
        /// <param name="usr">用户数据</param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_set_h264_callback(int playfd, RealDataCallBackFunc func, int usr);

        /// <summary>
        /// 设置播放缓冲
        /// </summary>
        /// <param name="playfd">预览句柄</param>
        /// <param name="buf">缓冲区大小（1－50），缓冲区越大，播放越流畅；缓冲区越小，实时性越好。若是复合流建议最小值设置成6 </param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_set_player_buf(int playfd, int buf);

        /// <summary>
        /// 抛B帧
        /// </summary>
        /// <param name="playfd">预览句柄</param>
        /// <param name="num">B帧个数(0-2)</param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_throw_b_frame(int playfd, int num);

        /// <summary>
        /// 设置句柄所带的用户数据
        /// </summary>
        /// <param name="user">用户数据</param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_set_fd_udata(int udata);

        /// <summary>
        /// 通过设备ID信息登录设备(不需要建立长连接) 
        /// </summary>
        /// <param name="dev_id">设备ID</param>
        /// <param name="username">用户名,目前无效,传入NULL即可</param>
        /// <param name="password">密码,目前无效,传入NULL即可</param>
        /// <param name="client_sup_id">客户端接入服务器ID </param>
        /// <param name="dev_sup_id">设备端接入服务器ID </param>
        /// <param name="client_sup_ip">客户端接入服务器IP </param>
        /// <param name="client_sup_port">客户端接入服务器端口</param>
        /// <param name="module_s">目标模块ID</param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern int client_sdk_login2_by_id(string dev_id, string username, string password, string client_sup_id, string dev_sup_id, string client_sup_ip, int client_sup_port, int module_s);

        /// <summary>
        /// 消息回调委托
        /// </summary>
        /// <param name="fd">触发消息的句柄</param>
        /// <param name="command">消息类型</param>
        /// <param name="buf">消息数据</param>
        /// <param name="len">消息数据大小</param>
        /// <param name="usr">用户数据</param>
        public delegate void MessCallBackFunc(int fd, int command, string buf, int len, int usr);

        /// <summary>
        /// 设置消息回调
        /// </summary>
        /// <param name="func">回调函数</param>
        /// <param name="usr">用户数据</param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_set_mess_callback(MessCallBackFunc func, int usr);

        /// <summary>
        /// 设置消息回调（带句柄）
        /// </summary>
        /// <param name="fd">触发消息的句柄</param>
        /// <param name="func">回调函数</param>
        /// <param name="usr">用户数据</param>
        /// <returns></returns>
        [DllImport(@".\TiandySdk\dll\win32_clientsdk.dll", CharSet = CharSet.Ansi)]
        private static extern bool client_sdk_set_mess_callback_ex(int fd, MessCallBackFunc func, int usr);


        #endregion


        #region 向下接口实现

        /// <summary>
        /// 接口适配层回调函数，主要过码流进行转换。
        /// 为了防止回调函数被销毁导致码流回调失败，定义为成员变量 
        /// </summary>
        private RealDataCallBackFunc cgwRealDataCallBackFunc;

        /// <summary>
        /// 注册到tiandy SDK的回调函数，主用判断是否接收到码流，判断是否启动预览成功。
        /// 为了防止回调函数被销毁导致码流回调失败，定义为成员变量 
        /// </summary>
        private RealDataCallBackFunc sdkRealDataCallBackFunc;

        /// <summary>
        /// 成功接收码流的句柄列表
        /// </summary>
        private List<int> receiveSuccHandle = new List<int>();

        /// <summary>
        /// 预览句柄操作锁，用于receiveSuccHandel的操作
        /// </summary>
        private ReaderWriterLockSlim handelOperateLock = new ReaderWriterLockSlim();

        private MediaServer sdkMediaServer;

        internal MediaServer SdkMediaServer
        {
            get { return sdkMediaServer; }
            set { sdkMediaServer = value; }
        }

        //PTZ控制级别，默认为1
        private int ptzControlLevel = CgwConst.PTZ_CONTROL_LEVEL;

        public int PtzControlLevel
        {
            get { return ptzControlLevel; }
            set { ptzControlLevel = value; }
        }

        /// <summary>
        /// 初始化天地伟业SDK客户端
        /// </summary>
        /// <param name="localId">客户端本地ID，必须唯一</param>
        /// <returns></returns>
        public bool Load(string localId)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandySdkClient.Load({0}).", localId);
            try
            {

                //1、设置本地ID
                bool result = client_sdk_set_local_id(localId + DateTime.Now.ToString());
                if (!result)
                {
                    logEx.Error("Load video monitor failed when to call the function client_sdk_set_local_id().Monitor id:{0}.", localId);
                    //重复加载的时候，client_sdk_set_local_id会返回失败，但不影响sdk使用，不返回错误
                    //return false;
                }
                else
                {
                    logEx.Info("Call Tiandy sdk function client_sdk_set_local_id success.Monitor id:{0}.", localId);
                }
                //2、设置解码库,如果不需要解码，则不用加载
                //client_sdk_set_local_id
                //bool result1 = client_player_set_lib(1, "win32_player_hik.dll");
                //bool result2 = client_player_set_lib(2, "win32_player_dh.dll");
                //bool result3 = client_player_set_lib(3, "win32_player_td.dll");

                //3、初始化SDK
                result = client_sdk_startup();
                if (!result)
                {
                    logEx.Error("Call Tiandy sdk function client_sdk_startup failed.Monitor id:{0}.", localId);
                    return false;
                }
                else
                {
                    logEx.Info("Call Tiandy sdk function client_sdk_startup success.Monitor id:{0}.", localId);
                    //设置消息回调
                    //client_sdk_set_mess_callback(this.messCallBackFunc, 0);
                    return true;
                }

            }
            catch (Exception e)
            {
                logEx.Error("Load Tiandy monitor failed. Monitor id:{0},Exception message:{1}.", localId, e.Message);
                return false;
            }


        }

        /// <summary>
        /// 注销SDK资源
        /// </summary>
        /// <returns></returns>
        public bool Unload()
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandySdkClient.Unload().");
            try
            {
                //释放SDK资源
                if (client_sdk_cleanup())
                {
                    logEx.Info("Call Tiandy sdk function client_sdk_cleanup success.");
                    return true;
                }
                else
                {
                    logEx.Error("Call Tiandy sdk function client_sdk_cleanup failed.");
                    return false;
                }
            }
            catch (Exception e)
            {
                logEx.Error("Call Tiandy sdk function client_sdk_cleanup failed. Exception message:{0}.", e.Message);
                return false;
            }
        }

        /// <summary>
        /// 指定码流回调函数
        /// </summary>
        /// <param name="videoDataCallBack"></param>
        /// <param name="sender"></param>
        public void SetDataCallBackFunc(RealDataCallBackFunc dataCallBack)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandySdkClient.SetDataCallBackFunc().");
            this.cgwRealDataCallBackFunc = dataCallBack;
            this.sdkRealDataCallBackFunc = CallbackFunc;
            logEx.Trace("TiandySdkClient SetDataCallBackFunc success.");
        }

        /// <summary>
        /// 码流回调处理，用于判断是否接收到码流，如果接收到码流，标识预览成功，否则失败
        /// </summary>
        /// <param name="playfd">预览句柄</param>
        /// <param name="datatype">视频流类型</param>
        /// <param name="buf">码流</param>
        /// <param name="size">码流大小</param>
        /// <param name="usr">用户标识数据</param>
        private void CallbackFunc(int playfd, int datatype, byte[] buf, uint size, int usr)
        {
            if (this.handelOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
            {
                try
                {
                    if (!this.receiveSuccHandle.Contains(playfd))
                    {
                        this.receiveSuccHandle.Add(playfd);
                    }
                }
                finally
                {
                    this.handelOperateLock.ExitWriteLock();
                }
            }

            this.cgwRealDataCallBackFunc(playfd, datatype, buf, size, usr);
        }



        /// <summary>
        /// 启动实时预览
        /// </summary>
        /// <param name="dev_id">主机ID </param>
        /// <param name="cameraMediaServer">设备端接入的流媒体服务器信息 </param>
        /// <param name="info">实时播放信息</param>
        /// <returns>-1 失败，其他则是返回预览句柄</returns>
        public int StartReceiveVideo(string dev_id, MediaServer cameraMediaServer, ref RealPlayInfo info)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandySdkClient.StartReceiveVideo({0},{1},{2}).", dev_id, cameraMediaServer.No, info);

            int iPlayFd = CgwConst.START_RECEIVE_VIDEO_DATA_FAILED;

            //如果摄像机的流媒体服务器为空，那么启动码流失败
            if (cameraMediaServer == null)
            {
                logEx.Error("cameraMediaServer can't be null");
                return CgwConst.START_RECEIVE_VIDEO_DATA_FAILED;

            }

            MediaServer clientMediaServer = this.sdkMediaServer;
            if (clientMediaServer == null)
            {
                clientMediaServer = cameraMediaServer;
            }





            try
            {

                iPlayFd = client_sdk_realplay2_ex(
                                    dev_id,
                                    clientMediaServer.User,
                                    clientMediaServer.Pwd,
                                    clientMediaServer.No,
                                    cameraMediaServer.No,
                                    clientMediaServer.Ip,
                                    clientMediaServer.Port,
                                    ref info,
                                    null,
                                    0);

                if (iPlayFd == CgwConst.START_RECEIVE_VIDEO_DATA_FAILED)
                {
                    logEx.Error("Call Tiandy sdk function client_sdk_realplay2_ex failed.");
                    return iPlayFd;
                }
                else
                {
                    logEx.Info("Call Tiandy sdk function client_sdk_realplay2_ex success.dev_id:{0},username:{1},client_sup_id:{2},dev_sup_id{3},client_sup_ip:{4},client_sup_port:{5}.",
                                    dev_id,
                                    clientMediaServer.User,
                                    clientMediaServer.No,
                                    cameraMediaServer.No,
                                    clientMediaServer.Ip,
                                    clientMediaServer.Port);
                }

                //预览成功设置码流回调
                //if (!client_sdk_set_realdata_callback(iPlayFd, func, 0))
                if (!client_sdk_set_h264_callback(iPlayFd, this.sdkRealDataCallBackFunc, 0))
                {
                    logEx.Error("Start Receive camera video data failed.Call Tiandy sdk function client_sdk_set_h264_callback failed.playfd:{0}", iPlayFd);
                    return CgwConst.START_RECEIVE_VIDEO_DATA_FAILED;
                }
                else
                {
                    logEx.Info("Call Tiandy sdk function client_sdk_set_h264_callback success.playfd:{0}", iPlayFd);
                }
                //因天地伟业的预览接口是异步的，还需要通过消息回调判断是否成功
                int count = 0;
                bool isSuccess = false;
                //循环5次
                while (count < 5)
                {
                    //等待500ms
                    Thread.Sleep(CgwConst.CHECK_RECEIVE_STATUS_WAIT_TIME);
                    if (this.handelOperateLock.TryEnterReadLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                    {
                        try
                        {
                            if (this.receiveSuccHandle.Contains(iPlayFd))
                            {
                                logEx.Info("Start Receive camera video data success.");
                                isSuccess = true;
                                break;
                            }
                        }
                        finally
                        {
                            this.handelOperateLock.ExitReadLock();
                        }
                    }
                    //等待500ms
                    Thread.Sleep(CgwConst.CHECK_RECEIVE_STATUS_WAIT_TIME);
                    count++;
                }

                if (!isSuccess)
                {
                    logEx.Error("Set callback function success. But not receive any video data.Try to stop receive.");

                    client_sdk_set_h264_callback(iPlayFd, null, 0);
                    client_sdk_stop_realplay(iPlayFd);
                    iPlayFd = CgwConst.START_RECEIVE_VIDEO_DATA_FAILED;
                }

            }
            catch (Exception e)
            {

                iPlayFd = CgwConst.START_RECEIVE_VIDEO_DATA_FAILED;
                logEx.Error("Start Receive camera video data failed.Exception message:{0}.", e.Message);
            }

            return iPlayFd;
        }

        /// <summary>
        /// 消息回调函数
        /// </summary>
        /// <param name="fd"></param>
        /// <param name="command"></param>
        /// <param name="buf"></param>
        /// <param name="len"></param>
        /// <param name="usr"></param>
        private void MessCallBack(int fd, int command, string buf, int len, int usr)
        {
            if (command == CgwConst.MESS_VIDEO_ACCEPT)
            {
                if (this.handelOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                {
                    try
                    {
                        this.receiveSuccHandle.Add(fd);
                    }
                    finally
                    {
                        this.handelOperateLock.ExitWriteLock();
                    }
                }
            }
        }

        /// <summary>
        /// 停止预览
        /// </summary>
        /// <param name="playfd">设备ID</param>
        /// <returns>成功返回0，失败返回错误码</returns>
        public bool StopReceiveVideo(int playfd)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandySdkClient.StopReceiveVideo({0}).", playfd);
            try
            {
                //调用SDK接口，停止预览,回调设置为空
                client_sdk_set_h264_callback(playfd, null, 0);
                if (client_sdk_stop_realplay(playfd))
                {
                    if (this.handelOperateLock.TryEnterWriteLock(CgwConst.ENTER_LOCK_WAIT_TIME))
                    {
                        try
                        {
                            this.receiveSuccHandle.Remove(playfd);
                        }
                        finally
                        {
                            this.handelOperateLock.ExitWriteLock();
                        }
                    }
                    logEx.Info("Call Tiandy sdk function client_sdk_stop_realplay success.playfd:{0}.", playfd);
                    return true;
                }
                else
                {
                    logEx.Error("Call Tiandy sdk function client_sdk_stop_realplay failed. playfd:{0}.", playfd);
                    return false;
                }
            }
            catch (Exception e)
            {
                logEx.Error("Stop Receive camera video data failed.Exception message:{0}.", e.Message);
                return false;
            }

        }

        /// <summary>
        /// PTZ(Pan/Tilt/Zoom)控制，该接口需要在预览模式下使用
        /// </summary>
        /// <param name="playfd">预览句柄</param>
        /// <param name="cmd">控制命令</param>
        /// <returns>成功返回0，失败返回错误码</returns>
        public bool ControlPtz(int playfd, Client_PTZ_Command ptzCommand)
        {
            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandySdkClient.StopReceiveVideo({0}).", playfd);

            try
            {
                if (client_sdk_ptz_control_ex(playfd, ref ptzCommand, this.ptzControlLevel))
                {
                    logEx.Info("Call Tiandy sdk function client_sdk_ptz_control success. playfd:{0},Command:{1} {2} {3}.",
                                playfd,
                                ptzCommand.cmd,
                                ptzCommand.param,
                                ptzCommand.start);
                    return true;
                }
                else
                {
                    logEx.Error("Call Tiandy sdk function client_sdk_ptz_control failed.playfd:{0},Command:{1} {2} {3}.",
                                playfd,
                                ptzCommand.cmd,
                                ptzCommand.param,
                                ptzCommand.start);
                    return false;
                }
            }
            catch (Exception e)
            {
                logEx.Error("Call Tiandy sdk function client_sdk_ptz_control failed.Exception message:{0}", e.Message);
                return false;
            }
        }

        /// <summary>
        /// 重发I帧
        /// </summary>
        /// <param name="hostNo">主机编号</param>
        /// <param name="channel">摄像头对应的主机通道</param>
        /// <param name="devMediaServerId">设备接入服务器id</param>
        /// <returns></returns>
        public bool MakeIFrame(string hostNo, int channel, MediaServer cameraMediaServer)
        {


            MediaServer clientMediaServer = this.sdkMediaServer;
            if (clientMediaServer == null)
            {
                clientMediaServer = cameraMediaServer;
            }
          

            NLogEx.LoggerEx logEx = new NLogEx.LoggerEx(log);
            logEx.Trace("Enter: TiandySdkClient.MakeIFrame({0},{1},{2}).", hostNo, channel, cameraMediaServer.No);
            try
            {
                //调用SDK接口，登陆主机（编码器）()
                int loginResult = client_sdk_login2_by_id(hostNo,
                                                          clientMediaServer.User,
                                                          clientMediaServer.Pwd,
                                                          clientMediaServer.No,
                                                          cameraMediaServer.No,
                                                          clientMediaServer.Ip,
                                                          clientMediaServer.Port,
                                                          0);

                if (loginResult == CgwConst.START_RECEIVE_VIDEO_DATA_FAILED)
                {
                    logEx.Error("Call Tiandy sdk function client_sdk_login2_by_id failed.dev_id:{0},username:{1},client_sup_id:{2},dev_sup_id:{3},client_sup_ip:{4},client_sup_port:{5}.",
                        hostNo,
                        clientMediaServer.User,
                        clientMediaServer.No,
                        cameraMediaServer.No,
                        clientMediaServer.Port);
                    return false;
                }
                else
                {
                    logEx.Info("Call Tiandy sdk function client_sdk_login2_by_id success.");
                }

                //生成I帧
                if (client_sdk_make_key_frame(loginResult, channel))
                {
                    logEx.Info("Call Tiandy sdk function client_sdk_make_key_frame success.");
                    return true;
                }
                else
                {
                    logEx.Error("Call Tiandy sdk function client_sdk_make_key_frame failed.channel:{0}", channel);
                    return false;
                }

                //发完关键帧后，要退出登录

            }
            catch (Exception e)
            {
                logEx.Error("Maker I frame failed.channel:{0},Exception message:{1}.", channel, e.Message);
                return false;
            }

        }

        #endregion
    }
}
