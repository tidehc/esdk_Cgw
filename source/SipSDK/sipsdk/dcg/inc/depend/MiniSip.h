/**
* @file  MiniSip.h
* @brief 包含MiniSip所有头文件  
*
* Copyright (c) 2010
* Huawei Tech.Co.,Ltd
*
* @author   Li GengQiang/l00124479
* @date     2010/12/06
* @version  NVS V100R002C01
*
*/

#ifndef IVSCBB_SS_MINISIP_H
#define IVSCBB_SS_MINISIP_H

#define SS_MAINTAIN

#ifdef CUMW_SIP_NAMESPACE

//去掉map告警
#pragma warning(disable : 4786 4503)
#endif

#include "ssincludeall.h"
#include "CConnMgr.h"
#include "sip_namespace.h"
//SIP的UDP的Socket
extern unsigned long g_ulSipUdpSocket;
namespace SipStack{

namespace SIP
{
    //SIP返回值的显示字符串最大长度
    const unsigned long STRING_LEN_SIP_RET_RESULT = sizeof("SIP_RET_ERR_MANDATORY_HDR");

    //SIP返回值的显示字符串数组定义
    const char STR_ARR_EN_SIP_RESULT[][STRING_LEN_SIP_RET_RESULT]
    = {
        "SIP_RET_SUCCESS",
        "SIP_RET_FAILURE",
        "SIP_RET_MALLOC_FAILED",
        "SIP_RET_INVALID_PARAM",
        "SIP_RET_INVALID_STATE",
        "SIP_RET_ERR_MANDATORY_HDR",    //STRING_LEN_SIP_RET_RESULT为该字符串的长度
        "SIP_RET_ERR_BRANCH_CLASH",
        "SIP_RET_NOT_HEADER_LIST",
        "SIP_RET_RPORT_ERROR"
    };

    //SIP请求方法的字符串最大长度
    const unsigned long STRING_LEN_SIP_METHOD = sizeof("UNSUBSCRIBE");
    //SIP请求方法的字符串数组定义
    const char STR_ARR_SIP_METHOD[][STRING_LEN_SIP_METHOD]
    = {
        "EXT",
        "INVITE",
        "ACK",
        "OPTIONS",
        "BYE",
        "CANCEL",
        "REGISTER",
        "INFO",
        "PRACK",
        "SUBSCRIBE",
        "NOTIFY",
        "UPDATE",
        "MESSAGE",
        "UNSUBSCRIBE",
        "REFER",
        "PUBLISH"
    };

    typedef enum
    {
        EX_HDR_ID_MAX_FORWARDS  = SIP_BASIC_HDR_ID_BUTT,    //Max-Forwords头域
        EX_HDR_ID_USER_AGENT,                               //User-Agent头域
        EX_HDR_ID_AUTHORIZATION,                            //Authorization头域
        EX_HDR_ID_EXPIRES,                                  //Expires头域
        EX_HDR_ID_CONTENT_TYPE,                             //Content-Type头域
        EX_HDR_ID_WWW_AUTHENTICATE,                         //WWW-Authenticate头域
        EX_HDR_ID_AUTHENTICATION_INFO,                      //Authentication-Info头域
        EX_HDR_ID_SUBJECT,                                  //Subject头域
        EX_HDR_ID_REASON,                                   //
        EX_HDR_ID_DATE,                                     //
        EX_HDR_ID_SERVER,                                   //add by cwx148380
        EX_HDR_ID_MAX,                                      //扩展头域最大值
    }EX_HDR_ID;

    const unsigned long MAX_NUM_EX_HEADER = EX_HDR_ID_MAX - EX_HDR_ID_MAX_FORWARDS;   //扩展头域个数
	
	typedef enum
	{
		INIT  = 0,                   // 
		OCCUPIED,            // 使用中
	}REQ_MSG_STATE;

    //请求消息信息
    typedef struct
    {
        SipMsgS*    pstSipReqMsg;   //SIP请求消息指针
        SS_UINT32   ulTuObjId;      //对应的TU对象ID
        SS_UINT32   ulTxnObjId;     //对应的事务对象ID
        unsigned long ulDialogId;
        // added by dhong
        unsigned long secs;        // added by dhong
		REQ_MSG_STATE state;
    }REQ_MSG_INFO;
}

namespace SIP
{
    /**
    * Description:  CreateReqMsg(). 创建SIP请求消息，并且设置CSeq头域
    * @param  [in]  enSipMethod SIP请求类型 
    * @param  [out] pstSipMsg   SIP消息指针
    * @return       long.       返回码
    */
    long CreateReqMsg
    (
        EN_SIP_METHOD   enSipMethod, 
        SipMsgS*&       pstSipMsg
    );

    /**
    * Description:  InitMiniSip().  初始化MiniSip
    * @return       long.       返回码
    */
    long InitMiniSip();

    /**
    * Description:  RegExHeaders(). 注册扩展头域
    * @return       long.       返回码
    */
    long RegExHeaders();

    /**
    * Description:  SetSipTptIpPort().   设置SipTptIpPort
    * @param  [out] stTptIpPort 待设置的SipTptIpPortS引用
    * @param  [in]  objAddr     网路地址
    * @return       void.
    */
    void SetSipTptIpPort
    (
        SipTptIpPortS&      stTptIpPort, 
        const CNetworkAddr& objAddr
    );

    /**
    * Description:  SetSipTptNwInfo().  设置SipTptNwInfo
    * @param  [out] stTptNwInfo 待设置的SipTptNwInfoS引用
    * @param  [in]  objDstAddr  目的网路地址
    * @param  [in]  objSrcAddr  源网路地址
    * @return       long.       返回码
    */
    void SetSipTptNwInfo
    (
        SipTptNwInfoS&      stTptNwInfo, 
        const CNetworkAddr& objDstAddr, 
        const CNetworkAddr& objSrcAddr
    );

    /**
    * Description:  CreateSipString().  创建SipString
    * @param  [in]  hdlMemCp        内存控制块指针
    * @param  [out] pstSipString    待创建的SipString
    * @param  [in]  szContent       内容
    * @return       long.           返回码
    */
    long CreateSipString
    (
        SipMemCpHdl     hdlMemCp, 
        VppStringS*&    pstSipString,
        const char*     szContent
    );

    /**
    * Description:  CreateQuoteString().  创建带引号的SipString
    * @param  [in]  hdlMemCp        内存控制块指针
    * @param  [out] pstSipString    待创建的SipString
    * @param  [in]  szContent       内容
    * @return       long.           返回码
    */
    long CreateQuoteString
    (
        SipMemCpHdl     hdlMemCp, 
        VppStringS*&    pstSipString,
        const char*     szContent
    );

    /**
    * Description:  CreateSipStringData().  创建SipString的数据
    * @param  [in]  hdlMemCp        内存控制块指针
    * @param  [out] stSipString     待创建的SipString的数据
    * @param  [in]  szContent       内容
    * @return       long.           返回码
    */
    long CreateSipStringData
    (
        SipMemCpHdl     hdlMemCp, 
        VppStringS&     stSipString, 
        const char*     szContent
    );

    /**
    * Description:  SetSipString(). 设置SipString
    * @param  [in]  stSipString     待创建的SipString
    * @param  [in]  szContent       内容
    * @return       void.
    */
    void SetSipString
    (
        SipString&  stSipString, 
        const char* szContent
    );

    /**
    * Description:  SetSipHostPort().   设置SipHostPort
    * @param  [out] stHostPort  待设置的HostPort引用
    * @param  [in]  objAddr     网路地址
    * @return       void.
    */
    void SetSipHostPort
    (
        SipHostPort&        stHostPort, 
        const CNetworkAddr& objAddr
    );

    void SetSipHostPort_New
        (
        SipHostPort&         sip_host_port_st_r, 
        const string&   ip_str_r,
        const unsigned short port_us
        );

    /**
    * Description:  SetSipHostName().    设置HostName
    * @param  [in]  hdlMemCp        内存控制块指针
    * @param  [out] stHostPort      待设置的HostPort引用
    * @param  [in]  pszHostName     主机名
    * @return       long.   返回值
    */
    long SetSipHostName
    (
        SipMemCpHdl     hdlMemCp, 
        SipHostPort&    stHostPort, 
        const char*     pszHostName
    );

    /**
    * Description:  SetURI().   设置URI
    * @param  [in]  hdlMemCp    内存控制块指针
    * @param  [out] stURI       待设置的URI引用
    * @param  [in]  pszUriName  URI名
    * @return       long.       返回码
    */
    long SetURI
    (
        SipMemCpHdl hdlMemCp, 
        URI&        stURI, 
        const char* pszUriName
    );

    /**
    * Description:  SetUriByDomain().   设置域名URI
    * @param  [in]  hdlMemCp        内存控制块指针
    * @param  [out] stURI           待设置的URI引用
    * @param  [in]  pszUriName      URI名
    * @param  [in]  pszUriDomain    URI域
    * @return       long.       返回码
    */
    long SetUriByDomain
    (
        SipMemCpHdl     hdlMemCp, 
        URI&            stURI, 
        const char*     pszUriName, 
        const char*     pszUriDomain
    );

    /**
    * Description:  SetUriByAddr(). 设置地址型URI
    * @param  [in]  hdlMemCp        内存控制块指针
    * @param  [out] stURI           待设置的URI引用
    * @param  [in]  pszUriName      URI名
    * @param  [in]  objAddr         网路地址
    * @return       long.       返回码
    */
    long SetUriByAddr
    (
        SipMemCpHdl         hdlMemCp, 
        URI&                stURI, 
        const char*         pszUriName, 
        const CNetworkAddr& objAddr
    );

    /**
    * Description:  SetRequestUri().    设置Request-URI
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  pszUriName      URI名
    * @param  [in]  pszUriDomain    URI域
    * @return       long.       返回码
    */
    long SetRequestUri
    (
        SipMsgS&        stSipMsg,
        const char*     pszUriName, 
        const char*     pszUriDomain
    );

    /**
    * Description:  SetCallId().  设置Call-ID头域
    * @param  [in]          stSipMsg        Sip消息结构体引用
    * @param  [in/out]  strCallId       CallID字符串的值，为空时将随机生成
    * @return       long.       返回码
    */
    long SetCallId
    (
        SipMsgS&    stSipMsg,
        string&     strCallId
    );

    /**
    * Description:  SetFrom().  设置From头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  pszUriName      URI名
    * @param  [in]  pszUriDomain    URI域
    * @return       long.       返回码
    */
    long SetFrom
    (
        SipMsgS&        stSipMsg,
        const char*     pszUriName, 
        const char*     pszUriDomain
    );

    /**
    * Description:  SetTo().    设置To头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  pszUriName      URI名
    * @param  [in]  pszUriDomain    URI域
    * @return       long.       返回码
    */
    long SetTo
    (
        SipMsgS&        stSipMsg,
        const char*     pszUriName, 
        const char*     pszUriDomain
    );

    /**
    * Description:  SetToTag().    设置To的Tag
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @return       long.       返回码
    */
    long SetToTag
    (
        SipMsgS& stSipMsg
    );

    /**
    * Description:  SetVia().   设置Via头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  objAddr         网路地址
    * @return       long.       返回码
    */
    long SetVia
    (
        SipMsgS&             sip_msg_st_r,
        const string&   ip_str_r,
        const unsigned short port_us
    );


    /**
    * Description:  AddExHeader().  添加扩展头域
    * @param  [in]  stSipMsg    Sip消息结构体引用
    * @param  [in]  usHdrId     扩展头域ID
    * @param  [in]  pvHdrVal    扩展头域值指针
    * @return       long.       返回码
    */
    long AddExHeader
    (
        SipMsgS&        stSipMsg,
        unsigned short  usHdrId,
        void*           pvHdrVal
    );

    /**
    * Description:  AddAuthorizationHeader().  添加扩展头域
    * @param  [in]  stSipMsg                    Sip消息结构体引用
    * @param  [in]  stSipAuthorization  Sip鉴权信息
    * @return       long.       返回码
    */
    long AddAuthorizationHeader
    (
        SipMsgS&            stSipMsg,
        SipAuthorization&   stSipAuthorization
    );

   /**
    * Description:  AddWWWAuthorizationHeader().  添加扩展头域
    * @param  [in]  stSipMsg                    Sip消息结构体引用
    * @param  [in]  stSipAuthorization  Sip鉴权信息
    * @return       long.       返回码
    */
    long AddWWWAuthorizationHeader
    (
        SipMsgS&            stSipMsg,
        SipWWWAuthenticate&   stSipAuthorization
    );

    /**
    * Description:  SetMaxForwords().   设置Max-Forwards头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  nMaxForwards    Max-Forwards值
    * @return       long.       返回码
    */
    long SetMaxForwards
    (
        SipMsgS&    stSipMsg,
        SS_UINT32   nMaxForwards
    );

    /**
    * Description:  SetUserAgent(). 设置User-Agent头域
    * @param  [in]  stSipMsg            Sip消息结构体引用
    * @param  [in]  pszProductName      产品名称
    * @param  [in]  pszProductVersion   产品版本
    * @return       long.       返回码
    */
    long SetUserAgent
    (
        SipMsgS&    stSipMsg,
        const char* pszProductName,
        const char* pszProductVersion
    );

    /**
    * Description:  SetRoute(). 设置Route头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  objRouteAddr    路由地址
    * @param  [in]  pszDisplayName  显示名字
    * @return       long.       返回码
    */
    long SetRoute
    (
        SipMsgS&            stSipMsg,
        const CNetworkAddr& objRouteAddr,
        const char*         pszDisplayName
    );

    /**
    * Description:  SetAuthorization(). 设置Authorization头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  pszUserName     用户名
    * @param  [in]  pszPassword     密码
    * @param  [in]  stPlatAuthInfo  平台鉴权信息
    * @return       long.       返回码
    */
    long SetAuthorization
    (
        SipMsgS&                stSipMsg,
        char*                   pszUserName,
        char*                   pszPassword,
        const PLAT_AUTH_INFO&   stPlatAuthInfo
    );

    /**
    * Description:  CreateAuthDigestUri().  创建Authorization的Digest的URI
    * @param  [in]      stReqUri        SipURI结构体引用
    * @param  [out]     pszDigestUri    DigestUri字符串输出
    * @return       long.       返回码
    */
    long CreateAuthDigestUri
    (
        const SipURI&   stReqUri,
        char*&          pszDigestUri
    );

    /**
    * Description:  SetContact(). 设置Contact头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  objContactAddr  Contact地址
    * @param  [in]  pszDisplayName  显示名字
    * @return       long.       返回码
    */
    long SetContact
    (
        SipMsgS&            stSipMsg,
        const CNetworkAddr& objContactAddr,
        const char*         pszDisplayName
    );

    /**
    * Description:  SetExpires().   设置Expires头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  nExpireTime     超时时间
    * @return       long.       返回码
    */
    long SetExpires
    (
        SipMsgS&    stSipMsg,
        SS_UINT32   nExpireTime
    );

    /**
    * Description:  SetDate(). 设置Date头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  pszDate          时间
    * @return       long.       返回码
    */
    long SetDate
    (
        SipMsgS&            stSipMsg,
        const char*         pszDate
    );

    long SetContentEncoding
        (
        SipMsgS&            stSipMsg,
        const char*         pszEncoding
        );

    /**
    * Description:  SetDate()       设置Date头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  pszServer       域服务器
    * @return       long.           返回码
    */
    long SetServer
    (
        SipMsgS&            stSipMsg,
        const char*         pszServer
    );

    /**
    * Description:  SetEvent(). 设置自定义Event头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  pszEvent         事件
    * @return       long.       返回码
    */
    long SetEvent
        (
        SipMsgS&         stSipMsg,
        const char*         pszEvent
        );

    /**
    * Description:  SetExpires().   设置Expires头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  szSubMediaType  子媒体类型
    * @return       long.       返回码
    */
    long SetContentType
    (
        SipMsgS&    stSipMsg,
        const char* szSubMediaType
    );

    /**
    * Description:  SetSubject().   设置Subject头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  strSubject      Subject内容字符串
    * @return       long.       返回码
    */
    long SetSubject
    (
        SipMsgS&        stSipMsg,
        const string&   strSubject
    );

    /**
    * Description:  ResetVarHdr().  重置可变头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  ulSequence      CSeq值
    * @param  [in]  pszFromTag      From的Tag
    * @param  [in]  pszToTag        To的Tag
    * @return       long.       返回码
    */
    long ResetVarHdr
    (
        SipMsgS&        stSipMsg,   
        unsigned long   ulSequence,
        const std::string& strFromTag,
        const char*     pszToTag    = VOS_NULL
    );

    /**
    * Description:  SetMsgBody().   设置消息体
    * @param  [in]  stSipMsg            ip消息结构体引用
    * @param  [in]  szSubMediaType      子媒体类型
    * @param  [in]  ulMsgBodyLen        消息体长度
    * @param  [in]  szMsgBody           消息体字符串
    * @return       long.       返回码
    */
    long SetMsgBody
    (
        SipMsgS&        stSipMsg,
        const char*     szSubMediaType,
        unsigned long   ulMsgBodyLen,
        char*           szMsgBody
    );

    /**
    * Description:  GetPlatAuthInfo().   获取平台鉴权信息
    * @param  [in]  stSipMsg        SIP消息引用
    * @param  [out] stPaltAuthInfo  获取的平台鉴权信息
    * @return       long.       返回码
    */
    long GetPlatAuthInfo
    (
        SipMsgS&        stSipMsg,
        PLAT_AUTH_INFO& stPaltAuthInfo
    );

    /**
    * Description:  GetRedirectInfo().  获取重定向信息
    * @param  [in]  stSipMsg            SIP消息引用
    * @param  [out] vectorServerInfo    服务器信息向量
    * @return       long.       返回码
    */
    long GetRedirectInfo
    (
        SipMsgS&            stSipMsg,
        VECTOR_SERVER_INFO& vectorServerInfo
    );

    /**
    * Description:  GetExpires().   获取注册超时时间
    * @param  [in]  stSipMsg        SIP消息引用
    * @param  [out] ulExpires       获取的注册超时时间
    * @return       long.       返回码
    */
    long GetExpires
    (
        SipMsgS&        stSipMsg,
        unsigned long&  ulExpires
    );

	/**
	* Description:  GetContact().   获取设备网络地址信息
	* @param  [in]  stSipMsg        SIP消息引用
	* @param  [out] devIP              获取设备IP
	* @param  [out] devPort          获取设备Port
	* @return       long.       返回码
	*/
	long GetContact
	(
	    SipMsgS&        stSipMsg,
	    string&          devIP,
	    unsigned short&  devPort
	);

   /**
    * Description:  GetDate().   获取Date时间
    * @param  [in]  stSipMsg        SIP消息引用
    * @param  [out] dateTime          时间
    * @return       long.       返回码
    */
    long GetDate
    (
        SipMsgS&        stSipMsg,
		string&       dateTime
    );

   /**
    * Description:  GetCurrentTime().   获取系统当前时间
    * @return       string.       返回系统当前时间
    */
	string GetCurrentTime();

    /**
    * Description:  GetNextNonce().     获取NextNonce
    * @param  [in]  stSipMsg        SIP消息引用
    * @param  [out] strNextNonce       获取的NextNonce
    * @return       long.       返回码
    */
    long GetNextNonce
    (
        SipMsgS&    stSipMsg,
        string&     strNextNonce
    );

    /**
    * Description:  GetMsgBody().   获取消息体内容
    * @param  [in]  stSipMsg        SIP消息引用
    * @param  [out] pMsgBody       消息体指针
    * @param  [out] ulMsgBodyLen       消息体内容长度
    * @return       long.       返回码
    */
    long GetMsgBody
    (
        SipMsgS&        stSipMsg,
        char*&          pMsgBody,
        unsigned long&  ulMsgBodyLen
    );

    /**
    * Description:  GetSubject().   获取Subject头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  szSubject       Subject内容字符串，必须带字符串结束符
    * @param  [in]  ulSubjectLen    Subject内容字符串长度，包含字符串结束符
    * @return       long.       返回码
    */
    long GetSubject
    (
        SipMsgS&        stSipMsg,
        char*&          pSubject,
        unsigned long&  ulSubjectLen
    );

    /**
    * Description:  GetReason().   获取Reason头域
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [in]  ulReason        ulReason 里面text的错误码
    * @return       long.       返回码
    */
    long GetReason
    (
        SipMsgS&        stSipMsg,
        unsigned long&  ulReason
    );

    /**
    * Description:  CopyHeader().   从SIP消息中拷贝头域
    * @param  [in]  sHeaderId       头域ID
    * @param  [in]  stInSipMsg      拷出的SIP消息引用
    * @param  [out] stOutSipMsg     拷入的SIP消息引用
    * @return       long.       返回码
    */
    long CopyHeader
    (
        unsigned short  usHeaderId,
        SipMsgS&        stInSipMsg,
        SipMsgS&        stOutSipMsg
    );

    /**
    * Description:  GetDialogInfo().  获取对话信息
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [out]  stDiagInfo     输出的对话信息
    * @param  [in]  bFromLocal      消息是否来自本地
    * @return       long.       返回码
    */
    long GetDialogInfo
    (
        SipMsgS&        stSipMsg,
        DIALOG_INFO&    stDiagInfo,
        VOS_BOOLEAN     bFromLocal
    );

    /**
    * Description:  GetFromTag().  获取From的Tag
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [out]  strFromTag     输出的From的Tag
    * @return       long.       返回码
    */
    long GetFromTag
    (
        SipMsgS&    stSipMsg,
        string&     strFromTag
    );

    /**
    * Description:  GetFromUriInfo().  获取From的Uri信息
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [out]  stFromUri     输出的From的Uri信息
    * @return       long.       返回码
    */
    long GetFromUriInfo
    (
        SipMsgS&        stSipMsg,
        PEER_URI_INFO&  stFromUri
    );

    /**
    * Description:  GetFromValue().  获取From的值
    * @param  [in]  stSipMsg        Sip消息结构体引用
    * @param  [out]  pszFromValue   输出的From的Value，里面会分配该内存
    * @return       long.       返回码
    */
    /*
    long GetFromValue
    (
        SipMsgS&    stSipMsg,
        char*&      pszFromValue
    );
    */
}
}//end namespace
#endif //_MINI_SIP_H_HEADER_INCLUDED_

