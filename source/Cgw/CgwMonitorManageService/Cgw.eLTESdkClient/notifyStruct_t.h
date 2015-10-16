//===================================================================
//	Copyright(C)2013-2020, Huawei Tech. Co., Ltd.
//
//	文件名	：notifyStruct_t.h
//	作  者	：w00206574
//	完成日期：2014-04-08
//	说  明	：实现eLTE接口C++ 和C#之间转换
//	其  它	：
//	历史记录：
//===================================================================
#include "StdAfx.h"
#include <string>

/**
   定义群组显示数据结构
**/
typedef enum {
    PTT_IND_STATUS_UNKNOWN=1000,
    PTT_IND_STATUS_RECEIVED,
    PTT_IND_STATUS_REQUEST,
    PTT_IND_STATUS_ACCEPT,
    PTT_IND_STATUS_WAITING,
    PTT_IND_STATUS_REJECT,
    PTT_IND_STATUS_SNATCH,
    PTT_IND_STATUS_HANGUP_OK,
    PTT_IND_STATUS_CLOSE_OK,
    PTT_IND_STATUS_CANCEL_OK,
    PTT_IND_STATUS_PREMPTED,
    /*--------媒体面变化的通知-------------*/
    PTT_IND_STATUS_TX_BEGIN,        //组呼话权变化
    PTT_IND_STATUS_TX_IDLE,         //组呼话权空闲
    PTT_IND_STATUS_MEDIAPLANE_ERROR,
    PTT_IND_STATUS_EMERGENCY_BEGIN,
    PTT_IND_STATUS_EMERGENCY_UPDATE,
    PTT_IND_STATUS_LICENSE_LIMIT,
    PTT_IND_STATUS_SIGNAL_ERROR
} Speaker_Status_t;

typedef  struct  strGrpDspInfo
{
	int _grpId;
	Speaker_Status_t _sp_status;
	int _speaker;
	int theLocalPort;
	int theRemotePort;
	unsigned long theRemoteIP;
};
typedef void (*notifyGroupStatus_t)(strGrpDspInfo *pGrpInfo);

typedef enum
{
	/*-------------------以下信息元素用于点呼，包括视频点呼；见p2pcallstatus_indicator.cpp-----------*/
	CALLSTATUS =0, //点呼状态
	CALLERINFO =1, //主叫人
	CALLEEINFO =2, //被叫人

	/*--------------------以下信息元素用于组呼，见grpcallstatus_indicator.cpp----------------*/
	GRPCALLSTATUS =3, //组呼状态
	GRPCALLSPERKER =4, //组呼主讲人
	FLOORSTATUS =5,    //话权状态指示

	/*--------------------以下信息元素用于紧急呼叫， 见emergency_indicator.cpp----------------*/
	EMERGENCYCALL =6,  //紧急呼叫指示
	EMERGENCYCALLER =7,  //紧急呼叫人

	/*--------------------以下信息元素用于资源状态管理,见resourcestatus_indicator.cpp----------------*/
	RESORCEID =8,              //资源ID
	DEVICEID =9,               //视频分栏时的窗口号
	RESSELECTSTATUS =10,        //资源选中状态指示， 用于多GUI联动
	RESASSIGNSTATUS =11,        //资源指派状态指示， 即调度台是否加入该资源组
	RESTXSTATUS =12,            //资源占用状态指示
	RESLOGGINGSTATUS =13,       //资源录音状态指示
	RESDLSTATUS =14,            //资源监听状态指示
	RESREGSTATUS_PROXY =15,     //资源注册状态指示, 代理注册(所有有线用户，终端集群注册)
	RESREGSTATUS_DIRECT =16,    //资源注册状态指示, 直接注册(终端业务注册，如视频)
	ATTACHINGGROUP =17,         //资源加入的组号

	/*--------------------以下信息元素用于用户状态管理;见userstatus_indicator.cpp----------------*/
	USERSTATUS =18,		//用户状态指示， 一般由BCC通知
	USERGPSSTATUS =19,       //用户GPS状态指示
	USERDLSTATUS =20,            //用户登录状态指示
	USERDGNASTATUS =21,      //用户动态重组状态
	VIDEODISPATCHSTATUS =22,     //视频分发状态
	MUTESTATUS =23,          //静音状态
	GRPPATCHSTATUS =24  //派接操作状态
}BundleKey;

/**
   定义资源状态指示数据结构
**/
typedef  BundleKey resource_status_t;

typedef enum {
    RESOURCE_STATUS_UNKNOWN = 4000,
    STATUS_SELECTED = 4001,    //资源选中状态
    STATUS_UNSELECTED = 4002,  //资源去选中状态

    STATUS_ASSIGNED = 4003,    //资源指派状态-
    STATUS_DEASSIGNED = 4004,  //资源去指派状态-

    STATUS_LISTENING = 4005,   //资源监听中状态
    STATUS_LISTENEND = 4006,   //资源监听结束状态

    STATUS_LOGGING = 4007,      //资源录音中状态
    STATUS_NOLOGGING = 4008,    //资源未设置录音

    STATUS_INPATCH = 4009,      //资源被派接
    STATUS_OUTPATCH = 4010,     //资源解除派接

    STATUS_REGOK = 4011,        //资源注册
    STATUS_REGFAIL = 4012,      //资源去注册
    STATUS_FORBIDEN = 4013,     //资源未授权 500
    STATUS_NOT_FOUND = 4014,    //资源不存在 404
    STATUS_TEMP_UNAVAILABLE = 4015, //资源暂时不可用 480
    STATUS_RES_CONFILCT = 4016, //资源冲突 450
    STATUS_UNAUTHORIZED = 4017, //未鉴权 401
    STATUS_LICENSE_LIMIT = 4018, //License受限 503
    STATUS_PASSWORD_WRONG = 4019, //密码错误 403

    //下面四个状态的顺序很重要， 因为 isValidTransition（）需要参考顺序
    STATUS_INVITING = 4020,    //the resource is initializing a call
    STATUS_RINGING = 4021,      //the resource is ringing
    STATUS_TXMTING = 4022,     //the resource is talking/transimitting in a call
    STATUS_TXMTIDLE = 4023,    //the resource is idle

    STATUS_DGNAOK = 4024,       //dynamic regrouping OK
    STATUS_DGNAFAIL = 4025,      //dynamic regrouping fail
    STATUS_DGNA_UEFAIL = 4026,   //dynamic regrouping one UE fail
    STATUS_GROUP_ACTIVATED = 4027,
    STATUS_GROUP_DEACTIVATED = 4028,
    STATUS_MUTE = 4029,        //sound mute
    STATUS_UNMUTE = 4030,       //sound unmute
    STATUS_GRPPATCH_CREATEOK = 4031,  //派接组创建OK
    STATUS_GRPPATCH_CREATEFAIL = 4032, //派接组创建失败
    STATUS_GRPPATCH_CANCELOK = 4033, //派接组取消OK
    STATUS_GRPPATCH_CANCELFAIL = 4034, //派接组取消失败
    STATUS_GRPPATCH_ADDOK = 4035, //派接组添加成员OK
    STATUS_GRPPATCH_ADDFAIL = 4036, //派接组添加成员FAIL
    STATUS_GRPPATCH_DELOK = 4037, //派接组删除成员OK
    STATUS_GRPPATCH_DELFAIL = 4038,  //派接组删除成员FAIL

    STATUS_ACCOUNT_LOCKED = 4060 // 账号被锁定

} res_status_value_t;

typedef  struct  strResourceStatusIndicator
{
	int ResId;
	resource_status_t status_type;      //status type
	res_status_value_t status_value;    //status value for this type
	int attaching_grp;
};
typedef void (*notifyResourceStatus_t)(strResourceStatusIndicator *resStaIndi);
typedef void (*notifyUserStatus_t)(strResourceStatusIndicator *resStaIndi);

/**
   定义群组显示数据结构
**/
typedef enum {
    P2P_IND_STATUS_UNKNOWN = 2000,
    /*--------信令面状态--------*/
    P2P_IND_STATUS_INITIATED = 2001,
    P2P_IND_STATUS_RECEIVED = 2002,
    P2P_IND_STATUS_ANSWERED = 2003,
    P2P_IND_STATUS_EMERGENCY = 2004,
    P2P_IND_STATUS_PROCEEDING = 2005,
    P2P_IND_STATUS_RINGING = 2006,
    P2P_IND_STATUS_ACKED = 2007,
    P2P_IND_STATUS_RELEASED = 2008,
    P2P_IND_STATUS_HANGUPED = 2009,
    P2P_IND_STATUS_HANGUPED_ACTIVE = 2010,
    P2P_IND_STATUS_CANCELLED = 2011,
    P2P_IND_STATUS_PREMPTED = 2012,
    P2P_IND_STATUS_RELEASED_BUSY = 2013,
    P2P_IND_STATUS_BREAKIN_FAIL = 2014,
    P2P_IND_STATUS_BREAKIN_SUCCESS = 2015,
    P2P_IND_STATUS_PROHIBITED = 2016,
    P2P_IND_STATUS_REMOTE_NOANSWER = 2017,
    P2P_IND_STATUS_REMOTE_NOTFOUND = 2018,
    P2P_IND_STATUS_LICENSE_LIMIT = 2019,
    /*--------媒体面状态--------*/
    P2P_IND_STATUS_MEDIAPLANE_ERROR = 2020,
    P2P_IND_STATUS_MEDIAPLANE_MUTE = 2021,
    P2P_IND_STATUS_MEDIAPLANE_UNMUTE = 2022,
    P2P_IND_STATUS_REMOTE_NOTSUPPORTED = 2023,
    P2P_IND_STATUS_REMOTE_NOTCONNECTED = 2024,
    P2P_IND_STATUS_TRANSFER_SUCCESS = 2025,
    P2P_IND_STATUS_TRANSFER_FAIL = 2026,
    P2P_IND_STATUS_TRANSFER = 2027,
    P2P_IND_STATUS_SIGNAL_ERROR = 2028,
    P2P_IND_STATUS_EMERGENCY_BEGIN = 2029,
    P2P_IND_STATUS_RING_START_180 = 2030,         //introduced from TTR3.0
    P2P_IND_STATUS_RING_START_183 = 2031,         //introduced from TTR3.0
    P2P_IND_STATUS_RING_STOP = 2032               //introduced from TTR3.0
} P2pcall_Status_t;

typedef  struct  strP2pcallStatusIndicator
{
	P2pcall_Status_t theStatus;
	int theCaller;
	int  theCallee;
	int  theInserter;//抢权者
	int  theTargeter;//抢权对应对象

	int theLocalPort;
	int theRemotePort;
	unsigned long theRemoteIP;
};
typedef void (*notifyP2pcallStatus_t)(strP2pcallStatusIndicator *p2pStaIndi);

//短消息的函数定义
typedef void (*notifySMS2_t)(char *content,char *from,int getSdsContent);

typedef enum {
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
} dc_module_t;

typedef enum {
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
} module_status_t;

typedef  struct  strDCModuleStatusIndicator
{
	dc_module_t m_module_type;      //模块组件类型
	module_status_t m_status_value;    //状态值
};
typedef void (*notifyModuleStatus_t)(strDCModuleStatusIndicator *modSta);


/**
   定义群组显示数据结构
**/
typedef enum {
        P2Pvideocall_IND_STATUS_UNKNOWN = 3000,
/*------------信令面状态-----------*/
    P2Pvideocall_IND_STATUS_INITIATED = 3001,
    P2Pvideocall_IND_STATUS_RECEIVED = 3002,
    P2Pvideocall_IND_STATUS_ANSWERED = 3003,
    P2Pvideocall_IND_STATUS_PROCEEDING = 3004,
    P2Pvideocall_IND_STATUS_RINGING = 3005,
    P2Pvideocall_IND_STATUS_ACKED = 3006,
    P2Pvideocall_IND_STATUS_RELEASED = 3007,
    P2Pvideocall_IND_STATUS_HANGUPED = 3008,
    P2Pvideocall_IND_STATUS_HANGUPED_ACTIVE = 3009,
    P2Pvideocall_IND_STATUS_CANCELLED = 3010,
    P2Pvideocall_IND_STATUS_MONITOR = 3011,
    P2Pvideocall_IND_STATUS_PREMPTED = 3012,
    P2Pvideocall_IND_STATUS_RELEASED_BUSY = 3013,
    P2Pvideocall_IND_STATUS_PROHIBITED = 3014,
    P2Pvideocall_IND_STATUS_REMOTE_NOANSWER = 3015,
    P2Pvideocall_IND_STATUS_REMOTE_NOTFOUND = 3016,
    P2Pvideocall_IND_STATUS_LICENSE_LIMIT = 3017,
    P2Pvideocall_IND_STATUS_EMERGENCY = 3018,
	/*------------媒体面状态-----------*/
    P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_ERROR = 3019,  //视频RTP流建立失败
    P2Pvideocall_IND_STATUS_ACKED_MEDIAPLANE_NOAUDIO = 3020, //视频伴音流建立失败
    P2Pvideocall_IND_STATUS_REMOTE_NOTCONNECTED = 3021,
    P2Pvideocall_IND_STATUS_REMOTE_NOTSUPPORTED = 3022,
    P2Pvideocall_IND_STATUS_SIGNAL_ERROR = 3023,            //SIP error for GW-mode
    P2Pvideocall_IND_STATUS_VIDEO_CONTROL = 3024            //PTZ control for GW-mode
} P2pvideocall_Status_t;

//视频格式类型
typedef enum
{
	V_CIF,
	V_QCIF,
	V_D1,
	V_720P,
	V_1080P,
	INVALID_FMT
}Fmt_type;

//音频格式类型
typedef enum {
	invalid_ptype = -1,
	amr_475_ptype,
	amr_1220_ptype,
	pcmu_ptype,
	pcma_ptype
} Sound_t;

typedef  struct  strP2pvideocallStatusIndicator
{
	int Caller;
	int Callee;
	int CalAudioPort;
	int LocalVideoPort;
	int RemoteAudioPort;
	int RemoteVideoPort;
	unsigned long RemoteIp;
	int SoundMute;
	int Channel;
	int Uri;
	int UserCfm;
	int Camera;
	Sound_t SoundPtype;
	Fmt_type Fmt;
	P2pvideocall_Status_t Status;
};
typedef void (*notifyP2pvideocallStatus_t)(strP2pvideocallStatusIndicator *P2pvideocallSta);

typedef void (*notifyProvisionAllResync_t)();