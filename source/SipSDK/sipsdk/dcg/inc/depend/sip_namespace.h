/**
* @file  sip_namespace.h
* @brief SIP命名空间定义  
*
* Copyright (c) 2010
* Huawei Tech.Co.,Ltd
*
* @author   Li GengQiang/l00124479
* @date     2010/12/01
* @version  NVS V100R002C01
*
*/

#ifndef IVSCBB_SS_SIP_NAME_SPACE_H
#define IVSCBB_SS_SIP_NAME_SPACE_H

#ifndef BOOL
#define BOOL  bool
#endif

#ifdef WIN32
    //去掉map告警
#define STDCALL __stdcall
    #pragma warning(disable : 4786 4503)

    #pragma comment(lib, "coresip.lib")
    #pragma comment(lib, "sipext.lib")
    #pragma comment(lib, "sipsdp.lib")
    #pragma comment(lib, "cbb.lib")
#else

#define STDCALL
#define SOCKET_ERROR (-1)

#endif

//标准头文件
#include <string>
#include <map>
#include <vector>
#include <list>
using namespace std;

//VOS头文件
#include "vos.h"
#include "vos_common.h"
#include "CConnMgr.h"
#include "CSyncQueue.h"
#include "CNVSTimer.h"
#include "MiniSip.h"

//获得连接管理器实例引用
#define CONN_MGR CConnMgr::instance()

//获得全局定时器实例引用
#define NVS_TIMER SipStack::CNVSTimer::instance()

#pragma pack(push,1)

namespace SipStack{
//常量定义
namespace SIP
{
const unsigned long MAX_NUM_AUTH_HEADER     = 1; 

const unsigned long MAX_NUM_TRANSACTIONS    = 3000; //最大事务数
const unsigned long MAX_FORWARDS            = 70;   //SIP消息的最大转发数

const unsigned long MAX_LEN_UDP_PACKET      = 1024 * 8; //UDP包的最大长度

const unsigned long MAX_LEN_LOGIN_DOMAIN    = 128 + 4;  //登录域的最大长度
const unsigned long MAX_LEN_LOGIN_NAME      = 32  + 4;  //登录名的最大长度
const unsigned long MAX_LEN_LOGIN_PWD       = 64  + 4;  //登录密码的最大长度
const unsigned long MAX_LEN_PRODUCT_NAME    = 128 + 4;  //产品名称的最大长度
const unsigned long MAX_LEN_PRODUCT_VERSION = 64  + 4;  //产品版本的最大长度
const unsigned long MAX_LEN_CALL_ID         = 64  + 4;  //Call-ID的最大长度
const unsigned long MAX_LEN_REQUEST_URI     = 256;      //Request-URI的最大长度
const unsigned long MAX_LEN_SUBJECT         = 256;      //Subject头域的最大长度
const unsigned long MAX_LEN_OPAQUE          = 256;      //Opaque的最大长度

const unsigned long MD5_HEX_LEN             = 32  + 4;  //MD5值的长度

const unsigned long MAX_SIP_SERVER_ID       = 32;   //SIP服务器ID的最大长度 

const unsigned long MAX_LEN_IP              = 16;   //IP的最大长度
const unsigned long MAX_LEN_PORT            = 5;
const unsigned long MAX_LEN_FILE_PATH       = 256;  //文件路径最大长度

const unsigned long REGISTER_EXPIRES_TIME   = 365 * 24 * 3600; //注册超时时间: 一年

const char SIP_URI_SCHEME[]             = "sip:";   //SIP的URI的方案字符串
const char MEDIA_TYPE_APPLICATION[]     = "application";    //媒体类型
const char SUB_MEDIA_TYPE_SDP[]         = "sdp";    //SDP子媒体类型
const char SUB_MEDIA_TYPE_XML[]         = "cv+xml"; //XML子媒体类型
const char SUB_MEDIA_TYPE_T28181_SUBSCRIBE_XML[]         = "MANSCDP+xml"; //XML子媒体类型
const char SUB_MEDIA_TYPE_T28181_XML[]  = "xml";

const unsigned long MAX_LEN_AUTH_STRING = 8 + 1;    //www摘要鉴权的qop
const unsigned long MAX_NUM_AUTH_INFO_CHANGE    = 3;    //同一次鉴权信息变更最大次数

const unsigned long TIMER_INTERVAL          = 1;    //100毫秒
const unsigned long EVENT_SYNC_QUEUE_NUM    = 1024; //事件同步队列数

const unsigned long TIMER_USER_RIGHT_SHIFT  = 28;   //定时器参数的用户信息右移位数

const unsigned long INVALID_DIALOG_ID = 0;  //无效的业务ID
}

//枚举类型定义
namespace SIP
{
//返回码
enum RET_CODE
{
    //通用错误码
    RET_CODE_OK = 0x0000,           //成功
    RET_CODE_FAIL,                  //系统错误
    RET_CODE_PARA_INVALIDATE,       //无效参数
    RET_CODE_ALLOC_MEM_FAIL,        //分配内存失败
    RET_CODE_MEMORY_NULL,           //指针为空
    RET_CODE_INIT_SYS_ERROR,        //初始化时系统错误
    RET_CODE_AUTHENTICATE_ERROR,    //鉴权失败
    RET_CODE_CREATE_SIP_MSG_FAIL,   //创建SIP消息失败
    RET_CODE_SEND_SIP_MSG_FAIL,     //发送SIP消息失败
    RET_CODE_REQUEST_TIME_OUT,      //请求超时
    RET_CODE_NO_TO_TAG,             //没有TO的Tag
    RET_CODE_NO_REGISTER,           //没有注册，需重注册
    RET_CODE_REGISTER_UNAUTH,       // ....401..
    RET_CODE_ERROR_ONLINE,          // ..............
};

//自定义的SIP状态码
enum SIP_REASON_CODE
{
    SIP_REASON_CODE_NO_REGISTER = 1101,  //没有注册，需重注册
};

//事件类型
enum EVENT_TYPE
{
    EVENT_TYPE_EXIT_THREAD,             ///<退出线程
        //对应的pEventInfo为: NULL

    EVENT_TYPE_STACK_TIMER_REGISTER,    ///<定时器注册事件
        //对应的pEventInfo为: EVENT_INFO_TIMER_REGISTER
    EVENT_TYPE_STACK_TIMER_CANCEL,      ///<定时器取消事件
        //对应的pEventInfo为: EVENT_INFO_TIMER_CANCEL
    EVENT_TYPE_STACK_TIMER_TRIGGER,     ///<定时器触发事件
        //对应的pEventInfo为: NULL
    EVENT_TYPE_STACK_REDIRECT,          ///<重定向事件
        //对应的pEventInfo为: SIP_MSG_INFO

    EVENT_TYPE_STACK_INVALID,   //无效事件
};

//事件类型的显示字符串最大长度
const unsigned long STRING_LEN_EVENT_TYPE = sizeof("Event Type is Timer Register");
//事件类型的显示字符串数组定义
const char STR_ARR_EVENT_TYPE[][STRING_LEN_EVENT_TYPE] =
{
    "exit thread event",
    "registe timer event", //STRING_LEN_EVENT_TYPE为该字符串的长度
    "cancel timer event",
    "trigger timer event",
    "redirect event",
    "invalid event",
};

const unsigned int MAX_STR_ARR_EVENT_TYPE_COUNT = 5;

//文本消息类型
typedef enum
{
    //掩码位定义
    TEXT_MSG_TYPE_MASK_MASK_BIT     = 0xF000,   ///<掩码位的掩码
    TEXT_MSG_TYPE_MASK_SIP          = 0x1000,   ///<SIP掩码
    TEXT_MSG_TYPE_MASK_HTTP         = 0x2000,   ///<HTTP掩码

    //SIP文本消息类型
    TEXT_MSG_TYPE_SIP_MIN           = 0x0000 | TEXT_MSG_TYPE_MASK_SIP,  //SIP消息类型最小值
    TEXT_MSG_TYPE_SIP_REGISTER      = 0x0001 | TEXT_MSG_TYPE_MASK_SIP,  ///<SIP REGISTER
        //对应的pMsgHeader为: RESP_HEADER_REGISTER
    TEXT_MSG_TYPE_SIP_UNREGISTER    = 0x0002 | TEXT_MSG_TYPE_MASK_SIP,  ///<SIP REGISTER 用于注销
    TEXT_MSG_TYPE_SIP_OPTIONS       = 0x0003 | TEXT_MSG_TYPE_MASK_SIP,  ///<SIP OPTIONS
    TEXT_MSG_TYPE_SIP_MESSAGE       = 0x0004 | TEXT_MSG_TYPE_MASK_SIP,  ///<SIP MESSAGE
        //发送消息中，对应的pMsgHeader为: Request-URI的Name部分，带字符串
        //发送消息中，对应的ulMsgHeaderLen为: Request-URI的Name部分，包含结束符
    TEXT_MSG_TYPE_SIP_INVITE        = 0x0005 | TEXT_MSG_TYPE_MASK_SIP,  ///<SIP INVITE
        //发送消息中，对应的pMsgHeader为: @ref SIP::REQ_HEADER_INVITE
        //发送消息中，对应的ulMsgHeaderLen为: @SIP::REQ_HEADER_INVITE的长度
        //接收消息中，对应的pMsgHeader为: Subject字符串长度，包含结束符
        //接收消息中，对应的ulMsgHeaderLen为: Subject字符串长度，包含结束符
    TEXT_MSG_TYPE_SIP_BYE           = 0x0006 | TEXT_MSG_TYPE_MASK_SIP,  ///<SIP BYE
    TEXT_MSG_TYPE_SIP_CANCEL        = 0x0007 | TEXT_MSG_TYPE_MASK_SIP,  ///<SIP CANCEL
    TEXT_MSG_TYPE_SIP_ACK           = 0x0008 | TEXT_MSG_TYPE_MASK_SIP,  ///<SIP ACK

    TEXT_MSG_TYPE_SIP_SUBSCRIBE   = 0x0009 | TEXT_MSG_TYPE_MASK_SIP,  ///<SIP SUBSCRIBE
    TEXT_MSG_TYPE_SIP_UNSUBSCRIBE   = 0x0010 | TEXT_MSG_TYPE_MASK_SIP,  ///<SIP UNSUBSCRIBE
    TEXT_MSG_TYPE_SIP_INVITE_RSP   = 0x0011 | TEXT_MSG_TYPE_MASK_SIP,  ///<SIP 

    TEXT_MSG_TYPE_SIP_MAX,                                              //SIP消息类型最大值

    //HTTP文本消息类型
    TEXT_MSG_TYPE_HTTP_POST         = 0x0001 | TEXT_MSG_TYPE_MASK_HTTP, ///<HTTP POST
}TEXT_MSG_TYPE;

//文本类型到SIP请求类型的转换
const EN_SIP_METHOD ARR_TEXT_TYPE_TO_SIP_METHOD[] =
{
    SIP_METHOD_EXT,
    SIP_METHOD_REGISTER,
    SIP_METHOD_REGISTER,
    SIP_METHOD_OPTIONS,
    SIP_METHOD_MESSAGE,
    SIP_METHOD_INVITE,
    SIP_METHOD_BYE,
    SIP_METHOD_CANCEL,
    SIP_METHOD_ACK,

    SIP_METHOD_SUBSCRIBE,
    SIP_METHOD_UNSUBSCRIBE,
};

//SIP请求类型到文本类型转换
const TEXT_MSG_TYPE ARR_SIP_METHOD_TO_TEXT_TYPE[] =
{
    TEXT_MSG_TYPE_SIP_MIN,
    TEXT_MSG_TYPE_SIP_INVITE,
    TEXT_MSG_TYPE_SIP_ACK,
    TEXT_MSG_TYPE_SIP_OPTIONS,
    TEXT_MSG_TYPE_SIP_BYE,
    TEXT_MSG_TYPE_SIP_CANCEL,
    TEXT_MSG_TYPE_SIP_REGISTER,
    TEXT_MSG_TYPE_SIP_MAX,      //SIP_METHOD_INFO
    TEXT_MSG_TYPE_SIP_MAX,      //SIP_METHOD_PRACK
    TEXT_MSG_TYPE_SIP_SUBSCRIBE,      //SIP_METHOD_SUBSCRIBE
    TEXT_MSG_TYPE_SIP_MAX,      //SIP_METHOD_NOTIFY
    TEXT_MSG_TYPE_SIP_MAX,      //SIP_METHOD_UPDATE
    TEXT_MSG_TYPE_SIP_MESSAGE,         
    TEXT_MSG_TYPE_SIP_UNSUBSCRIBE,      //SIP_METHOD_UNSUBSCRIBE
    TEXT_MSG_TYPE_SIP_MAX,      //SIP_METHOD_REFER
    TEXT_MSG_TYPE_SIP_MAX,      //SIP_METHOD_PUBLISH
    TEXT_MSG_TYPE_SIP_MAX,      //SIP_METHOD_BUTT
};
}

//结构体定义
namespace SIP
{
//定时器信息
typedef struct 
{
    unsigned long ulTimerId;        //定时器ID
    unsigned long ulTimerLength;    //定时器时长
    unsigned long ulTimerPara;      //定时器参数
    unsigned long ulStartTimeTick;  //开始时间戳
        //BEGIN R002C01SPC100 ADD 2011-08-15 ligengqiang 00124479 for 超时后SipTxnTimeoutHandler里崩溃问题，防止重复停止定时器
    BOOL          bStop;            //是否已停止
        //END   R002C01SPC100 ADD 2011-08-15 ligengqiang 00124479 for 超时后SipTxnTimeoutHandler里崩溃问题，防止重复停止定时器
}TIMER_INFO;

//定时器注册事件信息
    //对应的ulEventType为: EVENT_TYPE_STACK_TIMER_REGISTER
typedef TIMER_INFO EVENT_INFO_TIMER_REGISTER;

//定时器取消事件信息
    //对应的ulEventType为: EVENT_INFO_TIMER_CANCEL
typedef struct
{
    unsigned long ulTimerId;    //定时器ID
}EVENT_INFO_TIMER_CANCEL;

//事件信息
typedef struct _EVENT_INFO
{
    EVENT_TYPE      enEventType;        //事件类型
    void*           pEventInfo;         //事件信息指针，其意义取决于ulEventType
    unsigned long   ulEventInfoLen;     //事件信息内容长度，校验用

    //构造函数
    _EVENT_INFO(EVENT_TYPE enType = EVENT_TYPE_STACK_INVALID, void* pInfo = NULL, unsigned long ulInfoLen = 0)
        {
            enEventType     = enType;
            pEventInfo      = pInfo;
            ulEventInfoLen  = ulInfoLen;
        }
}EVENT_INFO;

//事件同步队列
typedef CSyncQueue<EVENT_INFO> CEventSyncQueue;

//服务器信息
typedef struct _SERVER_INFO
{
    char            szServerIP[MAX_LEN_IP+1]; //服务器IP
    unsigned short  usServerPort;           //服务器端口
    unsigned short  usReserve;              //是否使用过

    _SERVER_INFO()
        {
            memset(szServerIP, 0, sizeof(szServerIP));
            usServerPort    = 0;
            usReserve       = 0;
        }
}SERVER_INFO;

//服务器信息向量类型
typedef vector<SERVER_INFO> VECTOR_SERVER_INFO;

//注册信息
typedef struct
{
    char            szLoginDomain[MAX_LEN_LOGIN_DOMAIN];        //登录域
    char            szLoginName[MAX_LEN_LOGIN_NAME];            //登录名
    char            szPassword[MAX_LEN_LOGIN_PWD];              //登录密码
    char            szLocalIp[MAX_LEN_IP];                      //本地IP
    char            szLocalPort[MAX_LEN_PORT];                  //本地端口
    char            szUriDomain[MAX_LEN_LOGIN_DOMAIN];          //网络通信时候的UriDomain域
    char            szSipServerId[MAX_SIP_SERVER_ID];           //SIP服务器ID
	char            szRemoteLoginName[MAX_LEN_LOGIN_NAME];      //远端SIP登录名
	char            szRemotePassword[MAX_LEN_LOGIN_PWD];        //远端SIP登录密码
	char            szLocalId[MAX_LEN_LOGIN_NAME];				//本方ID
    char            szProductName[MAX_LEN_PRODUCT_NAME];        //产品名称
    char            szProductVersion[MAX_LEN_PRODUCT_VERSION];  //产品版本
    unsigned long   ulExpireTime;                               //超时时间长度
    unsigned long   ulServerInfoNum;                            //服务器信息个数
    SERVER_INFO     stServerInfoArray[1];                       //服务器信息可变数组，其元素个数为ulServerInfoNum
}REGISTER_INFO;


//文本消息
typedef struct
{
    unsigned long   ulMsgSeq;       //文本消息序号
    TEXT_MSG_TYPE   enTextMsgType;  //文本消息类型
    unsigned long   ulDialogId;     //Invite建立的对话Id。Invite的200 OK里返回。BYE时需携带
    unsigned long   ulMsgBodyLen;   //文本消息体长度
    char*           pszMsgBody;     //文本消息体
    void*           pMsgHeader;     //消息头域的结构体指针，其意义取决于enMsgType
    unsigned long   ulMsgHeaderLen; //消息头域长度
    void*           pUserData;      //用户数据，原值返回
}TEXT_MSG;

//注册响应消息的头域
typedef struct
{
    unsigned long   ulExpires;                  //超时时长
    VOS_BOOLEAN     bRedirect;                  //是否重定向
    char            szOpaque[MAX_LEN_OPAQUE];   //注册成功的鉴权挑战信息中的Opaque
    unsigned long   ulServerInfoNum;            //服务器信息个数
    SERVER_INFO     stServerInfoArray[1];       //服务器信息可变数组，其元素个数为ulServerInfoNum
}RESP_HEADER_REGISTER;

///////////////////////////////////////////////////////////////////////////
    /// @brief INVITE请求消息的头域
    ///
    /// 对应的enTextMsgType为: @ref SIP::TEXT_MSG_TYPE_SIP_INVITE
    ///////////////////////////////////////////////////////////////////////////
struct REQ_HEADER_INVITE
{
    char szRequestUri[MAX_LEN_REQUEST_URI];  //Request-URI，根据不同业务，可以是设备ID或者内容ID
    char szSubject[MAX_LEN_SUBJECT];         //Subject头域的值
};

///////////////////////////////////////////////////////////////////////////
    /// @brief  对端的URI信息，从请求消息中的From头域提取
    ///
    /// 对应的enTextMsgType为: @ref SIP::TEXT_MSG_TYPE_SIP_MESSAGE
    ///////////////////////////////////////////////////////////////////////////
struct PEER_URI_INFO
{
    char szUriUserName[MAX_LEN_REQUEST_URI];    //URI的名字部分
    char szUriHostName[MAX_LEN_REQUEST_URI];    //URI的域名部分
    char szUserAgentName[MAX_LEN_REQUEST_URI];  //userAgent的域名部分
    char szCallID[MAX_LEN_REQUEST_URI];			//userAgent的域名部分
};

//平台鉴权信息
typedef struct _PLAT_AUTH_INFO
{
    string strRealm;    //用户域
    string strNonce;    //Nonce
    string strOpaque;   //Opaque
    string stAlgorithm; //Algorithm
	string strQop;		//Qop="auth","auth-sess"
}PLAT_AUTH_INFO;

//对话信息
typedef struct _DIALOG_INFO
{
    unsigned long   ulCSeq;               // Invite的CSeq，ACK和Cancel中使用
    VOS_BOOLEAN     bRecvOK;              //是否已经收到OK
    string          strLocalTag;
    string          strRemoteTag;
    string          strCallId;            //接收Invite时有效
    string          strRequestUriName;    //Invite RequestUri
    string          strRequestUriDomain;  //Invite RequestDomain

    _DIALOG_INFO()
        {
            ulCSeq  = 0;
            bRecvOK = VOS_FALSE;
        }

    bool operator < ( _DIALOG_INFO& other )
        {
            return (strCallId < other.strCallId);
        }

}DIALOG_INFO;
}

//回调函数定义
namespace SIP
{
//通知类型定义
typedef enum
{
    NOTIFY_TYPE_INTERFACE_MIN   = 0x8000,   //SIP定义最小值

    NOTIFY_TYPE_TEXT_MSG_RESP   = 0x8001,   //文本消息响应通知
    //对应pNotifyInfo结构体为: NOTIFY_TEXT_MSG_RESP
    NOTIFY_TYPE_TEXT_MSG_REQ    = 0x8002,   //文本消息请求通知
    //对应pNotifyInfo结构体为: NOTIFY_TEXT_MSG_REQ

    NOTIFY_TYPE_SIP_MAX         = 0x9000,   ///<SIP定义最大值
}NOTIFY_TYPE;

//文本消息响应通知
//对应的NOTIFY_TYPE为: NOTIFY_TYPE_TEXT_MSG_RESP
typedef struct
{
    unsigned long   ulRetCode;  //返回码
    unsigned long   ulReason;
    TEXT_MSG        stTextMsg;  //返回的文本消息
}NOTIFY_TEXT_MSG_RESP;

// added by dhong
typedef struct 
{
char *pRcvData;
char *pSendBody;
}Sip_Rcv_Data;

//文本消息请求通知
//对应的NOTIFY_TYPE为: NOTIFY_TYPE_TEXT_MSG_REQ
typedef TEXT_MSG NOTIFY_TEXT_MSG_REQ;

//通知回调函数的通知信息结构体
typedef struct
{
    unsigned long   ulNotifyType;       //通知类型，取值见NOTIFY_TYPE
    void*           pNotifyInfo;        //通知信息，其意义决定于ulNotifyType，取值见NOTIFY_TYPE
    unsigned long   ulNotifyInfoLen;    //pNotifyInfo内容的实际长度，必须正确
}NOTIFY_INFO;

//通知回调函数类型
//参数pstNotifyInfo为通知信息结构体指针
//参数pUserData为用户数据指针
typedef long (STDCALL *PCALLBACK_NOTIFY)(NOTIFY_INFO* pstNotifyInfo, void* pUserData);
}

}//end namespace
#pragma pack(pop)

#endif //_SIP_NAME_SPACE_H_HEADER_INCLUDED_

