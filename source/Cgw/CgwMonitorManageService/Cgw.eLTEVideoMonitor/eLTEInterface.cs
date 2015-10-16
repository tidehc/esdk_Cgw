/*-----------------------------------------------------------------------
//文件名：eLTEInterface.cs
//版权：Copyright 2011-2012 Huawei Tech. Co. Ltd. All Rights Reserved. 
//作者：w00206574
//日期：2014-3-06
//描述：用于申明eLTE接口,向上提供接口供T28182模块使用
//主要功能接口包括：登录、注销、获取设备列表、实况等
//---------------------------------------------------------------------*/
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace CgwMonitorManage.eLTE
{
    public delegate void callbackSMS2Delegate(string _content, string _from, int length);
    public delegate void callbackP2pStatusDelegate(ref strP2pcallStatusIndicator structResStaIndi);
    public delegate void callbackResStatusDelegate(ref strResourceStatusIndicator structResStaIndi);
    public delegate void callbackGroupStatusDelegate(ref strGrpDspInfo structGrpStaInfo);
    public delegate void callbackP2pvideocallStatusDelegate(ref strP2pvideocallStatusIndicator structP2pvideocallStaIndi);
    public delegate void callbackUserStatus(ref strResourceStatusIndicator structResStaIndi);
    public delegate void callbackProvisionAllResyncDelegate();
    public delegate void callbackModuleStatusDelegate(ref strDCModuleStatusIndicator modSta);
    
    [StructLayout(LayoutKind.Sequential)]
    public struct strGrpDspInfo
    {
        public int _grpId;
        public _sp_status_t _sp_status;
        public int _speaker;
        public int theLocalPort;
        public int theRemotePort;
        public int theRemoteIP;
        public enum _sp_status_t
        {
            PTT_IND_STATUS_ACCEPT = 0x3eb,
            PTT_IND_STATUS_CANCEL_OK = 0x3f1,
            PTT_IND_STATUS_CLOSE_OK = 0x3f0,
            PTT_IND_STATUS_EMERGENCY_BEGIN = 0x3f6,
            PTT_IND_STATUS_EMERGENCY_UPDATE = 0x3f7,
            PTT_IND_STATUS_HANGUP_OK = 0x3ef,
            PTT_IND_STATUS_MEDIAPLANE_ERROR = 0x3f5,
            PTT_IND_STATUS_PREMPTED = 0x3f2,
            PTT_IND_STATUS_RECEIVED = 0x3e9,
            PTT_IND_STATUS_REJECT = 0x3ed,
            PTT_IND_STATUS_REQUEST = 0x3ea,
            PTT_IND_STATUS_SNATCH = 0x3ee,
            PTT_IND_STATUS_TX_BEGIN = 0x3f3,
            PTT_IND_STATUS_TX_IDLE = 0x3f4,
            PTT_IND_STATUS_UNKNOWN = 0x3e8,
            PTT_IND_STATUS_WAITING = 0x3ec
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct strResourceStatusIndicator
    {
        public int ResId;
        public resource_status_t status_type;
        public res_status_value_t status_value;
        public int attaching_grp;
    }
    public enum res_status_value_t
    {
        RESOURCE_STATUS_UNKNOWN = 4000,
        STATUS_SELECTED,    //资源选中状态
        STATUS_UNSELECTED,  //资源去选中状态

        STATUS_ASSIGNED,    //资源指派状态-
        STATUS_DEASSIGNED,  //资源去指派状态-

        STATUS_LISTENING,   //资源监听中状态
        STATUS_LISTENEND,   //资源监听结束状态

        STATUS_LOGGING,      //资源录音中状态
        STATUS_NOLOGGING,    //资源未设置录音

        STATUS_INPATCH,      //资源被派接
        STATUS_OUTPATCH,     //资源解除派接

        STATUS_REGOK,        //资源注册成功
        STATUS_REGFAIL,      //资源注册失败(general)
        STATUS_FORBIDEN,     //资源未授权 500
        STATUS_NOT_FOUND,    //资源不存在 404
        STATUS_TEMP_UNAVAILABLE, //资源暂时不可用 480
        STATUS_RES_CONFILCT, //资源冲突 450
        STATUS_UNAUTHORIZED, //未鉴权 401
        STATUS_LICENSE_LIMIT, //License受限 503
        STATUS_PASSWORD_WRONG, //密码错误 403

        //下面四个状态的顺序很重要， 因为 isValidTransition（）需要参考顺序
        STATUS_INVITING,    //the resource is initializing a call
        STATUS_RINGING,      //the resource is ringing
        STATUS_TXMTING,     //the resource is talking/transimitting in a call
        STATUS_TXMTIDLE,    //the resource is idle

        STATUS_DGNAOK,       //dynamic regrouping OK
        STATUS_DGNAFAIL,      //dynamic regrouping fail
        STATUS_DGNA_UEFAIL,   //dynamic regrouping one UE fail
        STATUS_GROUP_ACTIVATED,
        STATUS_GROUP_DEACTIVATED
    }

    public enum resource_status_t
    {
        /*-------------------以下信息元素用于点呼，包括视频点呼；见p2pcallstatus_indicator.cpp-----------*/
        CALLSTATUS, //点呼状态
        CALLERINFO, //主叫人
        CALLEEINFO, //被叫人

        /*--------------------以下信息元素用于组呼，见grpcallstatus_indicator.cpp----------------*/
        GRPCALLSTATUS, //组呼状态
        GRPCALLSPERKER, //组呼主讲人
        FLOORSTATUS,    //话权状态指示

        /*--------------------以下信息元素用于紧急呼叫， 见emergency_indicator.cpp----------------*/
        EMERGENCYCALL,  //紧急呼叫指示
        EMERGENCYCALLER,  //紧急呼叫人

        /*--------------------以下信息元素用于资源状态管理,见resourcestatus_indicator.cpp----------------*/
        RESORCEID,              //资源ID
        DEVICEID,               //视频分栏时的窗口号
        RESSELECTSTATUS,        //资源选中状态指示， 用于多GUI联动
        RESASSIGNSTATUS,        //资源指派状态指示， 即调度台是否加入该资源组
        RESTXSTATUS,            //资源占用状态指示
        RESLOGGINGSTATUS,       //资源录音状态指示
        RESDLSTATUS,            //资源监听状态指示
        RESREGSTATUS_PROXY,     //资源注册状态指示, 代理注册(所有有线用户，终端集群注册)
        RESREGSTATUS_DIRECT,    //资源注册状态指示, 直接注册(终端业务注册，如视频)
        ATTACHINGGROUP,         //资源加入的组号

        /*--------------------以下信息元素用于用户状态管理;见userstatus_indicator.cpp----------------*/
        USERSTATUS,		//用户状态指示， 一般由BCC通知
        USERGPSSTATUS,       //用户GPS状态指示
        USERDLSTATUS,            //用户登录状态指示
        USERDGNASTATUS,      //用户动态重组状态
        VIDEODISPATCHSTATUS     //视频分发状态
    }


    [StructLayout(LayoutKind.Sequential)]
    public struct strP2pcallStatusIndicator
    {
        public P2pcall_Status_t theStatus;
        public int theCaller;
        public int theCallee;
        public int theInserter;
        public int theTargeter;
        public int theLocalPort;
        public int theRemotePort;
        public int theRemoteIP;
        public enum P2pcall_Status_t
        {
            P2P_IND_STATUS_ACKED = 0x7d7,
            P2P_IND_STATUS_ANSWERED = 0x7d3,
            P2P_IND_STATUS_BREAKIN_FAIL = 0x7de,
            P2P_IND_STATUS_BREAKIN_SUCCESS = 0x7df,
            P2P_IND_STATUS_CANCELLED = 0x7db,
            P2P_IND_STATUS_EMERGENCY = 0x7d4,
            P2P_IND_STATUS_HANGUPED = 0x7d9,
            P2P_IND_STATUS_HANGUPED_ACTIVE = 0x7da,
            P2P_IND_STATUS_INITIATED = 0x7d1,
            P2P_IND_STATUS_LICENSE_LIMIT = 0x7e3,
            P2P_IND_STATUS_MEDIAPLANE_ERROR = 0x7e4,
            P2P_IND_STATUS_MEDIAPLANE_MUTE = 0x7e5,
            P2P_IND_STATUS_MEDIAPLANE_UNMUTE = 0x7e6,
            P2P_IND_STATUS_PREMPTED = 0x7dc,
            P2P_IND_STATUS_PROCEEDING = 0x7d5,
            P2P_IND_STATUS_PROHIBITED = 0x7e0,
            P2P_IND_STATUS_RECEIVED = 0x7d2,
            P2P_IND_STATUS_RELEASED = 0x7d8,
            P2P_IND_STATUS_RELEASED_BUSY = 0x7dd,
            P2P_IND_STATUS_REMOTE_NOANSWER = 0x7e1,
            P2P_IND_STATUS_REMOTE_NOTFOUND = 0x7e2,
            P2P_IND_STATUS_RINGING = 0x7d6,
            P2P_IND_STATUS_UNKNOWN = 0x7d0
        }
    }


    public enum Operation_t
    {
        DC_LOGIN, //调度员登入
        DC_LOGOUT, //调度员注销
        DC_EXIT, //调度台退出
        PTT_DIAL, //发起组呼/PTT抢权
        PTT_RELEASE, //发起PTT放权
        PTT_HANGUP, //组呼业务强拆
        SIP_INIT, //Sip初始化
        P2P_DIALOUT, //点对点拨打
        P2P_RECV, //点对点接听
        P2P_HANGUP, //点对点挂断
        P2P_REJECT, //点对点拒绝
        P2P_VIDEO_DIAL,//点对点视频拨打
        P2P_VIDEO_RECV,//点对点视频接听
        P2P_VIDEO_HANGUP,//点对点视频挂断
        P2P_VIDEO_MONITOR,//视频监控
        P2P_VIDEO_DISPATCH,//视频分发
        P2P_VIDEO_STARTCIRCDISPLAY,//发起视频轮询
        P2P_VIDEO_STOPCIRCDISPLAY,//停止视频轮询
        GRP_SUB, //群组订阅
        GRP_UNSUB, //群组去订阅
        GRP_JOIN, //加入群组
        GRP_LEAVE, //退出群组
        GRP_BREAKOFF, //强拆组呼
        GRP_QUERY, //用户当前群组查询
        P2P_BREAKOFF, //强拆点呼
        P2P_BREAKIN, //点呼抢权
        DC_PZT_CONCTROL, //PZT控制
        VOL_MUTE, //静音控制
        VOL_UNMUTE, //关闭静音控制
        BATCH_CFG,   //系统启动批配置操作
        SDS_SEND,   //发送短数据
        SDS_EXPORT,   //短数据导出
        REC_START,   //开始录音\录像
        REC_STOP,    //停止录音\录像
        VWALL_START,   //启动视频上墙
        VWALL_STOP,   //停止上墙
        DL_START,   //开始Discreet Listen
        DL_STOP,    //停止DL
        GIS_SUBSCRIBE, //GIS订阅/去订阅
        DGNA_CREATE,    //创建动态重组
        DGNA_CANCEL,     //取消动态重组
        TIC_DIALOUT,    //Telephone-Interconnect-Call dialout
        TIC_HANGUP,      //Telephone-Interconnect-Call hangup
        TEMPGRP_CREATE,  //创建临时组
        TEMP_PTT,        //发起临时组呼
        P2P_VIDEO_REJECT,//视频呼叫拒接
        P2P_VIDEO_PREVIEW, //视频预览，使用bypass模式
        P2P_VIDEO_SWITCHBACK, //视频切换，恢复non-bypass模式
        P2P_ENVIRONMENT_LISTEN, //个呼环境监听
        P2P_VIDEO_RECV_PARA, //点对点视频接听,带参数(fmt,mute),
        TEMPGRP_DELETE
    }

    //函数返回结果
    public enum ELTE_RESULT
    {
        RET_FAILURE = -1,	//失败
        RET_SUCCESS = 0,	//成功
        RET_PARSE_XML_FAILURE = 1,   //解析xml失败
    };

    //视频格式类型
    public enum Fmt_type
    {
        V_CIF,
        V_QCIF,
        V_D1,
        V_720P,
        V_1080P,
        INVALID_FMT
    };

    //音频格式类型
    public enum Sound_t
    {
        invalid_ptype = -1,
        amr_475_ptype,
        amr_1220_ptype,
        pcmu_ptype,
        pcma_ptype
    } ;

    //定义群组显示数据结构
    public enum P2pvideocall_Status_t
    {
        P2Pvideocall_IND_STATUS_UNKNOWN = 3000,
        /*------------信令面状态-----------*/
        P2Pvideocall_IND_STATUS_INITIATED,
        P2Pvideocall_IND_STATUS_RECEIVED,
        P2Pvideocall_IND_STATUS_ANSWERED,
        P2Pvideocall_IND_STATUS_PROCEEDING,
        P2Pvideocall_IND_STATUS_RINGING,
        P2Pvideocall_IND_STATUS_ACKED,
        P2Pvideocall_IND_STATUS_RELEASED,
        P2Pvideocall_IND_STATUS_HANGUPED,
        P2Pvideocall_IND_STATUS_HANGUPED_ACTIVE,
        P2Pvideocall_IND_STATUS_CANCELLED,
        P2Pvideocall_IND_STATUS_MONITOR,
        P2Pvideocall_IND_STATUS_PREMPTED,
        P2Pvideocall_IND_STATUS_RELEASED_BUSY,
        P2Pvideocall_IND_STATUS_PROHIBITED,
        P2Pvideocall_IND_STATUS_REMOTE_NOANSWER,
        P2Pvideocall_IND_STATUS_REMOTE_NOTFOUND,
        P2Pvideocall_IND_STATUS_LICENSE_LIMIT,
        P2Pvideocall_IND_STATUS_EMERGENCY,
        /*------------媒体面状态-----------*/
        P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_ERROR,  //视频RTP流建立失败
        P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_NOAUDIO, //视频伴音流建立失败
        P2Pvideocall_IND_STATUS_REMOTE_NOTCONNECTED,
        P2Pvideocall_IND_STATUS_REMOTE_NOTSUPPORTED
    } ;

    [StructLayout(LayoutKind.Sequential)]
    public struct strP2pvideocallStatusIndicator
    {
        public int Caller;
        public int Callee;
        public int CalAudioPort;
        public int LocalVideoPort;
        public int RemoteAudioPort;
        public int RemoteVideoPort;
        public UInt32 RemoteIp;
        public int SoundMute;
        public int Channel;
        public int Uri;
        public int UserCfm;
        public int Camera;
        public Sound_t SoundPtype;
        public Fmt_type Fmt;
        public P2pvideocall_Status_t Status;
    };

    public enum dc_module_t
    {
        SDS_MODULE = 1,   //短信业务模块
	    SMS_MODULE,       //彩信业务模块
	    AUDIO_MODULE,    //语音业务模块
	    VIDEO_MODULE,  //视频业务模块
	    GIS_MODULE,  //GIS业务模块
	    SIP_MODULE,  //信令通道模块
	    RTP_MODULE,   //媒体通道模块
	    RING_MODULE,   //响铃 add by lwj
	    SVC_MODULE, //调度机进程
	    UNKOWN_MODULE = 999
    };

    public enum module_status_t
    {
        RESOURCE_STATUS_NORMAL = 0, //状态正常
	    SDS_OVERFLOW,    //短信满
	    SDS_DISCONNECT,  //DC和短信服务器断链
	    SDS_CONNECT,     //DC和短信服务器连接
	    SDS_SEND_SUCCESS, //短信发送成功
	    SDS_SEND_FAIL, //短信发送失败
	    FOLDER_IS_EMPTY,  //导出文件夹为空
	    GIS_LICESNCE_UPDATE,  //GIS License变更
	    GIS_LICENSCE_EXPIREDAY, //license 即将过期提醒，只剩7天时通知，正数为还剩n天，负数为已经过期n天
	    SIP_DISCONNECT,       //DC和BCC断链
	    SIP_RECONNECT,       //DC和BCC恢复
	    SIP_KEEPALIVE_REJECT, //BCC拒绝DC的心跳请求
	    ATTACH_UPLOAD_FAIL,   //附件上传失败
	    ATTACH_DOWNLOAD_FAIL,  //附件下载失败
	    SIP_START_RUNNING,     //BCC 启动
	    KICK_OFF,      //BCC 踢出调度台通知,因为有相同账号登录
	    ASYN_CALLBACK_RETURN,   //异步调用返回值
	    PASSWORD_CHANGE,    //调度员密码修改
	    MODULE_DISCONNECT,
	    MODULE_RECONNECT,
	    SIP_KA_FAILONCE,       //DC和BCC断链一次
	    SIP_KA_OKONCE,       //DC和BCC恢复一次
	    USER_DELETE,
	    UNKOWN_STATUS = 999
    };

    [StructLayout(LayoutKind.Sequential)]
    public struct strDCModuleStatusIndicator
    {
        public dc_module_t m_module_type; //模块组件类型
        public module_status_t m_status_value; //状态值
    }

    public class eLTEInterface
    {
        /// <summary>
        /// 实现组呼的回调函数
        /// </summary>
        /// <param name="callback"></param>
        [DllImport(@"CgwMonitorManage.eLTE.eLTESdkClient.dll", EntryPoint = "SetGrpStaCallBack", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetGrpStaCallBack(callbackGroupStatusDelegate callback);

        /// <summary>
        /// 通知用户和群组资源的状态变更信息
        /// </summary>
        /// <param name="callback"></param>
        [DllImport(@"CgwMonitorManage.eLTE.eLTESdkClient.dll", EntryPoint = "SetResStaCallBack", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetResStaCallBack(callbackResStatusDelegate callback);

        /// <summary>
        /// 点呼回调业务
        /// </summary>
        /// <param name="callback"></param>
        [DllImport(@"CgwMonitorManage.eLTE.eLTESdkClient.dll", EntryPoint = "SetP2pStaCallBack", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetP2pStaCallBack(callbackP2pStatusDelegate callback);

        /// <summary>
        /// 通知用户普通短消息的服务接收实现
        /// </summary>
        /// <param name="callback"></param>
        [DllImport(@"CgwMonitorManage.eLTE.eLTESdkClient.dll", EntryPoint = "SetSMSCallBack2", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetSMSCallBack2(callbackSMS2Delegate callback);

        /// <summary>
        /// 处理视频回传并建立RTP传输通道
        /// </summary>
        /// <param name="callback"></param>
        [DllImport(@"CgwMonitorManage.eLTE.eLTESdkClient.dll", EntryPoint = "SetP2pVideoCallStaCallBack", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetP2pVideoCallStaCallBack(callbackP2pvideocallStatusDelegate callback);

        /// <summary>
        /// 通知用户和群组资源的状态变更信息,需要YC实现
        /// </summary>
        /// <param name="callback"></param>
        [DllImport(@"CgwMonitorManage.eLTE.eLTESdkClient.dll", EntryPoint = "SetUserStaCallBack", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetUserStaCallBack(callbackUserStatus callback);

        /// <summary>
        /// 通知下载配置信息完成，下载完成后再注册事件和查询设备
        /// </summary>
        /// <param name="callback"></param>
        [DllImport(@"CgwMonitorManage.eLTE.eLTESdkClient.dll", EntryPoint = "SetProvisionAllResyncCallBack", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetProvisionAllResyncCallBack(callbackProvisionAllResyncDelegate callback);

        /// <summary>
        /// 通知服务状态信息
        /// </summary>
        /// <param name="callback"></param>
        [DllImport(@"CgwMonitorManage.eLTE.eLTESdkClient.dll", EntryPoint = "SetModStaCallBack", CallingConvention = CallingConvention.StdCall)]
        public static extern void SetModStaCallBack(callbackModuleStatusDelegate callback);

    }
}
