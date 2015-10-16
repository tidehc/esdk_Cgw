/**
* @file  CSipStack.h
* @brief Sip协议栈类定义  
*
* Copyright (c) 2010
* Huawei Tech.Co.,Ltd
*
* @author   Li GengQiang/l00124479
* @date     2010/12/06
* @version  NVS V100R002C01
*
*/

#ifndef IVSCBB_SS_SIPSTACK_HPP
#define IVSCBB_SS_SIPSTACK_HPP

#include <map>
#include "MiniSip.h"
#include "ace_header.h"
#include "sipstack_agent_header.hpp"

namespace SipStack{
class CSipStack;

//全局SIP协议栈指针
extern CSipStack* g_pSipStack;

/**
* Class name:   CSipTimerTrigger
* @brief        SIP定时触发器
*
* Description:  
*/
class CSipStackTimerTrigger : public ITrigger
{
public:
    /**
    * Description:    CSipTimerTrigger(). Default constructor
    */
    CSipStackTimerTrigger()
    {
    };

    /**
    * Description:    CSipTimerTrigger(). Destructor
    */
    virtual ~CSipStackTimerTrigger()
    {
    };

    /**
    * Description:  触发定时事件处理
    * @param  [in]  pArg        具体处理类
    * @param  [in]  ullScales   定时器指针
    * @param  [in]  enStyle     触发类型
    */
    void OnTrigger
    (
        void *pArg, 
        ULONGLONG ullScales, 
        TriggerStyle enStyle
    );
};

class CSipStackMsgTimerTrigger : public ITrigger
{
public:
    /**
    * Description:    CSipTimerTrigger(). Default constructor
    */
    CSipStackMsgTimerTrigger()
    {
    };

    /**
    * Description:    CSipTimerTrigger(). Destructor
    */
    virtual ~CSipStackMsgTimerTrigger()
    {
    };

    /**
    * Description:  触发定时事件处理
    * @param  [in]  pArg        具体处理类
    * @param  [in]  ullScales   定时器指针
    * @param  [in]  enStyle     触发类型
    */
    void OnTrigger
    (
        void *pArg, 
        ULONGLONG ullScales, 
        TriggerStyle enStyle
    );
};

/**
* Class name:   CSipUdpHandle
* @brief        Sip的UDP处理类
*
* Description:  
*/
class CSipUdpHandle : public CUdpSockHandle
{
public:
    /**
    * Description:    CSipUdpHandle(). Default constructor
    */
    CSipUdpHandle();

    /**
    * Description:    ~CSipUdpHandle(). Destructor
    */
    ~CSipUdpHandle();

    /**
    * Description:  handle_recv(). 当有数据到达时调用此函数
    * @return       void.
    */
    void handle_recv(void);

    /**
    * Description:  handle_send(). 当可以发送数据时调用此函数
    * @return       void.
    */
    void handle_send(void);
};

/**
* Class name:   CSipStack
* @brief        Sip协议栈类
*
* Description:
*/
class CSipStack
{
    friend class CSipStackMsgWrap;
    friend class CSipStackMsgWrapRigister;
    friend class CSipStackMsgWrapUnRigister;
    friend class CSipStackMsgWrapMessage;
    friend class CSipStackMsgWrapInvite;
    friend class CSipStackMsgWrapBye;
    friend class CSipStackMsgWrapRedirect;
    friend class CSipStackMsgWrapAck;
    friend class CSipStackMsgWrapResp;
    friend class CSipStackMsgWrapInviteResp;
    friend class CSipStackMsgWrapSubscribe;

public:
    /**
    * Description:    CSipStack(). Default constructor
    */
    CSipStack();

    /**
    * Description:    ~CSipStack(). Destructor
    */
    virtual ~CSipStack();

    /**
    * Description:  InitSipStack(). 初始化SipStack
    * @param  [in]  pFunNotifyCallBack  通知回调函数指针
    * @param  [in]  pCallBackUserData   通知回调函数的用户数据指针
    * @return       long.       返回码
    */
    long Init(SIP::PCALLBACK_NOTIFY pfnNotify, void* pNotifyFnParams, const unsigned short local_port );

    /**
    * Description:  ReleaseSipStack(). 释放SipStack
    * @return       long.       返回码
    */
    long Fini();

    /**
    * Description:  SetRegisterInfo().  设置注册信息
    * @param  [in]  stRegisterInfo  注册信息
    * @return       long.           返回码
    */
    long SetRegisterInfo(const SIP::REGISTER_INFO& stRegisterInfo);

    ///////////////////////////////////////////  add by w00207012 2012-11-02 Begin //////////////////////////////////////////////////
    /**
    * Description:  SetRegisterInfo().  设置注册信息
    * @param  [in]  bServerAddrEx  是否使用服务器地址
    * @param  [in]  serverIP              服务器IP
    * @param  [in]  serverPort          服务器Port
    * @return       long.           返回码
    */
    long SetServerAddrEx(bool bServerAddrEx, const string& serverIP, unsigned short serverPort);

    /**
    * Description:  isServerAddrEx().  判断是否与前端交互，便于封装消息
    * @return       bool.
    */
    bool IsServerAddrEx(void);

    /**
    * Description:  SendRegisterResp().  返回注册结果信息
    * @param  [in]  seq              序列号
    * @param  [in]  retCode      结果码
    * @return       long.           返回码
    */
    long SendRegisterResp(const unsigned long seq, const unsigned long retCode);

    /**
    * Description:  SendKeepaliveResp().  返回心跳结果信息
    * @param  [in]  seq              序列号
    * @param  [in]  retCode      结果码
    * @return       long.           返回码
    */
    long SendKeepaliveResp(const unsigned long seq, const unsigned long retCode);

    /**
    * Description:  SendOptionResp().  返回option 信息（千里眼心跳回包）
    * @param  [in]  seq              序列号
    * @param  [in]  retCode      结果码
    * @return       long.           返回码
    */
    long SendOptionResp(const unsigned long seq, const unsigned long retCode);

    /**
    * Description:  SendInviteAck().  返回Invite Ack信息
    * @param  [in]  seq              序列号
    * @return       long.           返回码
    */
    long SendInviteAck(const unsigned long seq);

    /**
    * Description:  SendInviteBye().  返回Invite Bye信息
    * @param  [in]  seq              序列号
    * @return       long.           返回码
    */
    long SendInviteBye(const unsigned long seq);

    /**
    * Description:  SendInviteCancel().  返回Invite Cancel信息
    * @param  [in]  seq              序列号
    * @return       long.           返回码
    */
    long SendInviteCancel(const unsigned long seq);	
    ///////////////////////////////////////////  add by w00207012 2012-11-02 End ////////////////////////////////////////////////////

    /**
    * Description:  GetTuObjId().   获取TU对象ID
    * @return       unsigned long.           TU对象ID
    */
    unsigned long GetTuObjId();

    /**
    * Description:  SendSipMsg().   发送Sip消息
    * @param  [in]  stTextMsg   文本消息
    * @return       long.       返回码
    */
    long SendSipMsg(const SIP::TEXT_MSG& stTextMsg, const string& callID = "");

    /**
    * Description:  HandleSipTxnHiSfRspInd().   处理有状态Sip响应消息
    * @param  [in]  ulTuObjId   对应的TU对象ID
    * @param  [in]  ulTxnObjId  对应的事务对象ID
    * @param  [in]  stSipMsg    收到的SIP响应消息
    * @return       void.
    */
    void HandleSipTxnHiSfRspInd
    (
        SS_UINT32            ulTuObjId,
        SS_UINT32            ulTxnObjId,
        const SipTptNwInfoS& stTptNwInf,
        SipMsgS&             stSipMsg
    );

    /**
    * Description:  HandleSipTxnHiSfReqInd().   处理有状态Sip请求消息
    * @param  [in]  ulTuObjId   对应的TU对象ID
    * @param  [in]  ulTxnObjId  对应的事务对象ID
    * @param  [in]  stSipMsg    收到的SIP请求消息
    * @return       void.
    */
    void HandleSipTxnHiSfReqInd
    (
        SS_UINT32            ulTuObjId,
        SS_UINT32            ulTxnObjId,
        const SipTptNwInfoS& stTptNwInf,
        SipMsgS&             stSipMsg
    );

    /**
    * Description:  HandleSipTxnHiSlReqInd().   处理无状态Sip请求消息
    * @param  [in]  stSipMsg    收到的SIP请求消息
    * @return       void.
    */
    void HandleSipTxnHiSlReqInd
    (
        const SipTptNwInfoS& stTptNwInf,
        SipMsgS&             stSipMsg
    );

    /**
    * Description:  AddEvent(). 添加事件
    * @param  [in]  stEventInfo     事件信息引用
    * @return       long.   返回码
    */
    long AddEvent
    (
        const SIP::EVENT_INFO& stEventInfo
    );

    /**
    * Description:  GetTimerId().   获取定时器ID
    * @return       unsigned long.  定时器ID
    */
    unsigned long GetTimerID();

    //BEGIN R002C01SPC001 ADD 2011-08-24 ligengqiang 00124479 for 增加线程锁，MiniSIP只支持单线程
    /**
    * Description:  GetThreadMutex().   获取线程锁
    * @return       VOS_Mutex*.     锁指针
    */
    VOS_Mutex* GetThreadMutex();
    //END   R002C01SPC001 ADD 2011-08-24 ligengqiang 00124479 for 增加线程锁，MiniSIP只支持单线程

    /**
    * Description:  GetCSeq().   获取SIP消息的CSeq
    * @return       unsigned long.  SIP消息的CSeq
    */
    unsigned long GetCSeq();

    long SendInviteResp(const string &msg, const unsigned long seq, const unsigned long retCode);
    long SendResp(const unsigned long seq);

    long AnsMsg( const string& ans_code, const string& data);
private:
    /**
    * Description:  ProcEventThreadEntry().  处理事件线程入口
    * @param  [in]  pPara   线程入口参数
    * @return       long.   返回码
    */
    static unsigned long STDCALL ProcEventThreadEntry(void* pPara);

    /**
    * Description:  ProcEventThread().  处理事件线程
    * @return       void.
    */
    void ProcEventThread();

    /**
    * Description:  ClearAllMsgInfo().  清理所有消息信息
    * @return       void.
    */
    void ClearAllMsgInfo();

    /**
    * Description:  NotifyCallBack(). 调用回调函数
    * @param  [in]  stInfo  通知信息结构体
    * @return       long.   返回码
    */
    long NotifyCallBack(SIP::NOTIFY_INFO& stInfo);

    /**
    * Description:  NotifyTextMsgResp(). 通知文本响应消息
    * @param  [in]  stNotifyInfo    文本响应消息通知信息结构体
    * @return       long.   返回码
    */
    long NotifyTextMsgResp(SIP::NOTIFY_TEXT_MSG_RESP& stNotifyInfo);

    /**
    * Description:  NotifyTextMsgReq(). 通知文本请求消息
    * @param  [in]  stNotifyInfo        文本请求消息通知信息结构体
    * @return       long.   返回码
    */
    long NotifyTextMsgReq(SIP::NOTIFY_TEXT_MSG_REQ& stNotifyInfo);

    /**
    * Description:  SetLocalAddr().  设置本地地址
    * @return       void.
    */
    void SetLocalAddr(const unsigned short local_port);

    /**
    * Description:  HandleEventTimerTrigger().  处理定时器触发事件
    * @return       void.
    */
    void HandleEventTimerTrigger();

    /**
    * Description:  HandleEventTimerRegister(). 处理定时器注册事件
    * @param  [in]  pEventInfo      事件信息指针
    * @param  [in]  ulEventInfoLen  事件信息内容长度
    * @return       long.       返回码
    */
    long HandleEventTimerRegister
    (
        void*           pEventInfo, 
        unsigned long   ulEventInfoLen
    );

    /**
    * Description:  HandleEventTimerCancel().   处理定时器取消事件
    * @param  [in]  pEventInfo      事件信息指针
    * @param  [in]  ulEventInfoLen  事件信息内容长度
    * @return       long.       返回码
    */
    long HandleEventTimerCancel
    (
        void*           pEventInfo, 
        unsigned long   ulEventInfoLen
    );

    /**
    * Description:  HandleEventRedirect().  处理重定向事件
    * @param  [in]  pEventInfo      事件信息指针
    * @param  [in]  ulEventInfoLen  事件信息内容长度
    * @return       void.
    */
    void HandleEventRedirect
    (
        void*           pEventInfo, 
        unsigned long   ulEventInfoLen
    );

    /**
    * Description:  GetDialogIdBySipMsg().    根据SIP消息找到对话ID
    * @param  [in]      stSipMsg    SIP消息结构体引用
    * @param  [out] ulDialogId      对话ID
    * @param  [in]      bErase      是否删除
    * @return       long.       返回码
    */
    long GetDialogIdBySipMsg
    (
        SipMsgS&        stSipMsg,
        unsigned long&  ulDialogId,
        VOS_BOOLEAN     bErase = VOS_FALSE
    );

    /**
    * Description:  GetDialogIdBySipMsg().    根据曰靶畔⒄业蕉曰癐D
    * @param  [in]      stDialogInfo    对话信息
    * @param  [out] ulDialogId      对话ID
    * @param  [in]      bErase      是否删除
    * @return       long.       返回码
    */
    long GetDialogIdByInfo
    (
        const SIP::DIALOG_INFO& stDialogInfo,
        unsigned long&          ulDialogId,
        VOS_BOOLEAN             bErase = VOS_FALSE
    );

    /**
    * Description:  GetDialogInfoById().      根据对话ID找到对话信息
    * @param  [in]      ulDialogId      对话ID
    * @param  [out]     stDialogInfo    对话信息
    * @return       long.       返回码
    */
    long GetDialogInfoById
    (
        unsigned long       ulDialogId,
        SIP::DIALOG_INFO&   stDialogInfo
    );

    ////////////////////////////////// add by wx153027 2012-12-18 Begin /////////////////////////////////
    /**
    * Description:  HandleReqOption().  处理Option请求消息
    * @param  [in]  ulTuObjId   对应的TU对象ID
    * @param  [in]  ulTxnObjId  对应的事务对象ID
    * @param  [in]      stSipMsg            SIP消息引用
    * @param  [out] stNotifyReq     通知文本请求结构体引用
    * @return       long.       返回码
    */
    long HandleReqOption
    (
        SS_UINT32                   ulTuObjId,
        SS_UINT32                   ulTxnObjId,
        const SipTptNwInfoS&        stTptNwInf,
        SipMsgS&                    stSipMsg,
        SIP::NOTIFY_TEXT_MSG_REQ&   stNotifyReq
    );
    ////////////////////////////////// add by wx153027 2012-12-18 end /////////////////////////////////

    ////////////////////////////////// add by w00207027 2012-10-27 Begin /////////////////////////////////
    /**
    * Description:  HandleReqRegister().  处理Register请求消息
    * @param  [in]  ulTuObjId   对应的TU对象ID
    * @param  [in]  ulTxnObjId  对应的事务对象ID
    * @param  [in]      stSipMsg            SIP消息引用
    * @param  [out] stNotifyReq     通知文本请求结构体引用
    * @return       long.       返回码
    */
    long HandleReqRegister
    (
        SS_UINT32                   ulTuObjId,
        SS_UINT32                   ulTxnObjId,
        const SipTptNwInfoS&        stTptNwInf,
        SipMsgS&                    stSipMsg,
        SIP::NOTIFY_TEXT_MSG_REQ&   stNotifyReq
    );

    /**
    * Description:  CheckRegistAuthInfo().  校验Regist请求的Authenticate信息
    * @param  [in]      stSipMsg                    SIP消息引用
    * @param  [out]    pstSipAuthorization   Regist请求的Authenticate信息
    * @return       long.       返回码
    */
    long CheckRegistAuthInfo
    (
        SipMsgS&                    stSipMsg,
        SipAuthorization* &         pstSipAuthorization
    );

    /**
    * Description:  getStringFromQuoteString().  校验Regist请求的Authenticate信息
    * @param  [in]      sipString           SIP消息引用
    * @param  [out]    str                      提出后Quote的string
    * @return    long  .       返回码
    */
    long GetStringFromQuoteString
    (
        const SipString &sipString, 
        string &str
    );

    /**
    * Description:  HandleReqNotify().  处理Notify请求消息
    * @param  [in]  ulTuObjId   对应的TU对象ID
    * @param  [in]  ulTxnObjId  对应的事务对象ID
    * @param  [in]      stSipMsg            SIP消息引用
    * @param  [out] stNotifyReq     通知文本请求结构体引用
    * @return       long.       返回码
    */
    long HandleReqNotify
    (
        SS_UINT32                   ulTuObjId,
        SS_UINT32                   ulTxnObjId,
        const SipTptNwInfoS&        stTptNwInf,
        SipMsgS&                    stSipMsg,
        SIP::NOTIFY_TEXT_MSG_REQ&   stNotifyReq
    );

    /**
    * Description:  isRespDirect().     判断是否直接会响应消息
    * @param  [in]      pMsgBody            SIP消息体
    * @return   bool.       返回码
    */
    long IsRespDirect
    ( 
        const char* pMsgBody
    );
    ////////////////////////////////// add by w00207027 2012-10-27 End /////////////////////////////////

    /**
    * Description:  HandleReqInvite().  处理Invite请求消息
    * @param  [in]  ulTuObjId   对应的TU对象ID
    * @param  [in]  ulTxnObjId  对应的事务对象ID
    * @param  [in]      stSipMsg            SIP消息引用
    * @param  [out] stNotifyReq     通知文本请求结构体引用
    * @return       long.       返回码
    */
    long HandleReqInvite
    (
        SS_UINT32                   ulTuObjId,
        SS_UINT32                   ulTxnObjId,
        SipMsgS&                    stSipMsg,
        SIP::NOTIFY_TEXT_MSG_REQ&   stNotifyReq
    );

    /**
    * Description:  HandleReqMessage().     处理Message请求消息
    * @param  [in]  ulTuObjId   对应的TU对象ID
    * @param  [in]  ulTxnObjId  对应的事务对象ID
    * @param  [in]      stSipMsg            SIP消息引用
    * @param  [out] stNotifyReq     通知文本请求结构体引用
    * @return       long.       返回码
    */
    long HandleReqMessage
    (
        SS_UINT32                   ulTuObjId,
        SS_UINT32                   ulTxnObjId,
        const SipTptNwInfoS&        stTptNwInf,
        SipMsgS&                    stSipMsg,
        SIP::NOTIFY_TEXT_MSG_REQ&   stNotifyReq
    );

    /**
    * Description:  HandleReqBye().     处理Bye请求消息
    * @param  [in]  ulTuObjId   对应的TU对象ID
    * @param  [in]  ulTxnObjId  对应的事务对象ID
    * @param  [in]      stSipMsg            SIP消息引用
    * @param  [out] stNotifyReq     通知文本请求结构体引用
    * @return       long.       返回码
    */
    long HandleReqBye
    (
        SS_UINT32                   ulTuObjId,
        SS_UINT32                   ulTxnObjId,
        SipMsgS&                    stSipMsg,
        SIP::NOTIFY_TEXT_MSG_REQ&   stNotifyReq
    );

private:
      int GetAddrWithSipTptIpPortS( IN  const SipTptIpPortS& stTptAddr,
                                    OUT std::string&         strIPv4,
                                    OUT unsigned short&      usPort
                                  );
private:
    //SIP消息信息结构体
    typedef struct _SIP_MSG_INFO
    {
        unsigned long       ulMsgSeq;       //上层传下来的消息序号，回调通知时需要
        SIP::TEXT_MSG_TYPE  enTextMsgType;  //文本消息类型
        SS_UINT32           ulTxnObjId;     //事务对象ID
        SS_UINT32           ulTuObjId;      //TU对象ID
        unsigned long       ulDialogId;     //INVITE请求的对话ID
        SipMsgS*            pstSipMsg;      //SIP消息指针

        _SIP_MSG_INFO()
        {
            ulMsgSeq        = 0;
            enTextMsgType   = SIP::TEXT_MSG_TYPE_SIP_REGISTER;
            ulTxnObjId      = 0;
            ulTuObjId       = 0;
            ulDialogId      = 0;
            pstSipMsg       = NULL;
        }
    }SIP_MSG_INFO;

    class NoCmpDialogInfo
    {
    public:
        bool operator()
            (
            const SIP::DIALOG_INFO&, 
            const SIP::DIALOG_INFO&
            ) const
        {
            return true;
        }
    };

    //CSeq号与SIP消息信息的映射表类型
    typedef map<unsigned long, SIP_MSG_INFO*> MAP_C_SEQ_TO_SIP_MSG_INFO;

    //对话ID与对话信息的映射表类型
    typedef map<unsigned long, SIP::DIALOG_INFO> MAP_DIALOG_ID_TO_INFO;

    //对话信息与对话ID的映射表类型
    typedef map<SIP::DIALOG_INFO, unsigned long, NoCmpDialogInfo> MAP_DIALOG_INFO_TO_ID;

    //定时器ID与定时器信息的映射表类型
    typedef map<unsigned long, SIP::TIMER_INFO> MAP_TIMER_ID_TO_INFO;

    //服务器信息列表表类型
    typedef list<SIP::SERVER_INFO> LIST_SVR_INFO;

    //服务器信息查找比较方法类
    class FindServerInfo
    {
    public:
        FindServerInfo (const SIP::SERVER_INFO& stSvrInfo) : m_stSvrInfo(stSvrInfo)
        {
        }

        bool operator()(const SIP::SERVER_INFO& stSvrInfo)      
        {
            if (0 == strncmp(m_stSvrInfo.szServerIP, stSvrInfo.szServerIP, sizeof(m_stSvrInfo.szServerIP))
                && m_stSvrInfo.usServerPort == stSvrInfo.usServerPort)
            {
                return true;
            }

            return false;
        }

    private:
        const SIP::SERVER_INFO m_stSvrInfo;
    };

    //服务器侧对话信息查找比较方法类
    class FindDialogInfo
    {
    public:
        FindDialogInfo (const SIP::DIALOG_INFO& stDialogInfo) : m_stDialogInfo(stDialogInfo)
        {
        }

        bool operator()(const MAP_DIALOG_INFO_TO_ID::value_type& objValueType)      
        {
            if ((objValueType.first.strCallId     == m_stDialogInfo.strCallId)
                && (objValueType.first.strLocalTag == m_stDialogInfo.strLocalTag)
                && (objValueType.first.strRemoteTag   == m_stDialogInfo.strRemoteTag))
            {
                return true;
            }

            return false;
        }

    private:
        const SIP::DIALOG_INFO m_stDialogInfo;
    };

private:
    /**
    * Description:  GetSipMsgInfoByCSeq().      根据CSeq找到SIP消息的信息
    * @param  [in]      ulCSeq              SIP消息的CSeq
    * @param  [out] pstSipMsgInfo    SIP消息信息指针
    * @param  [in]  bErase      是否删除
    * @return       long.       返回码
    */
    long GetSipMsgInfoByCSeq
    (
        unsigned long   ulCSeq,
        SIP_MSG_INFO*&  pstSipMsgInfo,
        VOS_BOOLEAN     bErase = VOS_FALSE
    );

    /**
    * Description:  HasRedirect().  检查是否有重定向事件
    * @param  [in]  stSipMsgInfo    SIP消息信息引用
    * @return       VOS_BOOLEAN.    是否  有重定向
    */
    VOS_BOOLEAN HasRedirectEvent
    (
        const SIP_MSG_INFO& stSipMsgInfo
    );

    /**
    * Description:  HandleUnAuthResp().     处理未授权响应
    * @param  [in]  stSipMsg            SIP消息引用
    * @param  [in]  stReqMsgInfo  请求消息信息引用
    * @return       long.       返回码
    */
    long HandleUnAuthResp
    (
        SipMsgS&        stSipMsg,
        SIP_MSG_INFO&   stReqMsgInfo,
        bool            bFirst=true
    );

public:
    /**
    * Description:  HandleUnAuthResp().     重新注册，added by dhong
    * @return       long.       返回码
    */
    unsigned long InsertReqMsg(SipMsgS &sipMsg, const SS_UINT32 &ulTuObjId, const SS_UINT32 &ulTxnObjId, const unsigned long &ulDialogId);
    void CheckReqMsg(void);
    long HandleReRegister(void);
    SIP::REQ_MSG_INFO* GetReqMsg(unsigned long seq, bool bGetAndDelete = true);

protected:
    SIP_HEADER *GetSipHeader(const SipMsgS &sipMsg, bool bReq);
    void       GetFromSipString(const SipString &sipString, string &str);

private:
    /**
    * Description:  HandleUnAuthResp().     处理未授权响应
    * @param  [in]  stSipMsg            SIP消息引用
    * @param  [in]  stReqMsgInfo  请求消息信息引用
    * @return       long.       返回码
    */
    long HandleAuthOnLineResp
    (
        const SipMsgS&        stSipMsg,
        SIP_MSG_INFO&   stReqMsgInfo
    );

    /**
    * Description:  HandleSuccessResp().     处理成功响应
    * @param  [in]      stSipMsg            SIP消息引用
    * @param  [in]  stReqMsgInfo  请求消息信息引用
    * @param  [out] stNotifyResp    通知文本响应结构体引用
    * @return       long.       返回码
    */
    long HandleSuccessResp
    (
        SipMsgS&                    stSipMsg,
        const SIP_MSG_INFO&         stReqMsgInfo,
        SIP::NOTIFY_TEXT_MSG_RESP&  stNotifyResp
    );

    /**
    * Description:  HandleSuccessRespRegister().     处理Register成功响应
    * @param  [in]      stSipMsg            SIP消息引用
    * @param  [out] stNotifyResp    通知文本响应结构体引用
    * @return       long.       返回码
    */
    long HandleSuccessRespRegister
    (
        SipMsgS&                    stSipMsg,
        SIP::NOTIFY_TEXT_MSG_RESP&  stNotifyResp
    );

    /**
    * Description:  HandleSuccessRespInvite().     处理Invite成功响应
    * @param  [in]  stSipMsg        SIP消息引用
    * @param  [in]  stReqMsgInfo    请求消息信息引用
    * @param  [out] stNotifyResp    通知文本响应结构体引用
    * @return       long.       返回码
    */
    long HandleSuccessRespInvite
    (
        SipMsgS&                    stSipMsg,
        const SIP_MSG_INFO&         stReqMsgInfo,
        SIP::NOTIFY_TEXT_MSG_RESP&  stNotifyResp
    );

private:
    SIP::PCALLBACK_NOTIFY   m_pfnNotify;        //回调函数指针
    void*                   m_pNotifyFnParams;  //用户数据指针

    //处理事件线程
    VOS_Thread* m_pProceEventThread;
    //处理事件线程是否运行
    VOS_BOOLEAN m_bProcEventThreadRun;
    //同步事件队列
    SIP::CEventSyncQueue m_objEventSyncQueue;

    //定时器ID
    unsigned long m_ulTimerId;
    //定时器ID与定时器信息的映射表
    MAP_TIMER_ID_TO_INFO       m_mapTimerIdToInfo;
    ACE_Recursive_Thread_Mutex m_mutexTimerMap;

    //定时器
    CSipStackTimerTrigger* m_pTimerTrigger;
    // Message 定时器
    CSipStackMsgTimerTrigger *m_pMsgTimerTrigger;
    //是否已注册
    volatile VOS_BOOLEAN m_bRegister;

    //注册信息
    SIP::REGISTER_INFO m_stRegisterInfo;
    //密码的MD5的16进制字符串
    char m_szPwdMd5[SIP::MD5_HEX_LEN];
    //使用过的服务器信息列表
    LIST_SVR_INFO m_listSvrInfoUsed;
    //当前重定向服务器列表
    SIP::VECTOR_SERVER_INFO m_vectorServerInfo;
    //是否有重定向
    VOS_BOOLEAN m_bHasRedirect;
    //是否重定向过，用于上报重定向列表
    VOS_BOOLEAN m_bRedirected;
    //是否为临时重定向
    VOS_BOOLEAN m_bTempRedirect;

    //SIP的UDP处理器
    CSipUdpHandle m_objSipUdpHandle;
    //本地地址，网络序
    CNetworkAddr  m_objLocalAddr;
    //服务器地址，网络序
    CNetworkAddr  m_objServerAddr;

    ////////////////////////////// add by w00207027 2012-11-02 Begin ////////////////////////////////////////////
    //服务器地址，网络序
    CNetworkAddr  m_objServerAddrEx;

    bool      m_bServerAddrEx;
    ////////////////////////////// add by w00207027 2012-11-02 End //////////////////////////////////////////////

    //modify-cwx148380-begin
    string m_strIP;
    unsigned short m_usPort;
    //modify-cwx148380-end

    //TU对象ID
    unsigned long m_ulTuObjId;

    //Map锁
    VOS_Mutex* m_pMapMutex;
    //BEGIN R002C01SPC001 ADD 2011-08-24 ligengqiang 00124479 for 增加线程锁，MiniSIP只支持单线程
    //MiniSIP线程锁
    VOS_Mutex* m_pThreadMutex;
    //END   R002C01SPC001 ADD 2011-08-24 ligengqiang 00124479 for 增加线程锁，MiniSIP只支持单线程

    //对话ID到对话信息的映射表
    MAP_DIALOG_ID_TO_INFO m_mapDialogIdToInfo;
    //对话信息到对话ID的映射表
    MAP_DIALOG_INFO_TO_ID m_mapDialogInfoToId;

    //保存Register的Call-ID
    string m_strRegisterCallId;

    //CSeq号与SIP消息信息的映射表
    MAP_C_SEQ_TO_SIP_MSG_INFO m_mapCSeqToSipMsgInfo;

    //SIP消息的CSeq
    unsigned long m_ulCSeq;

    //平台鉴权信息
    SIP::PLAT_AUTH_INFO m_stPlatAuthInfo;

    //平台鉴权信息变更次数
    unsigned long m_ulPlatAuthInfoChangeNum;

    // 保存401返回来的SipMsgS消息, 用于重新注册时使用
    //SipMsgS    m_authMsg;
    SIP_MSG_INFO m_msgInfo;

    // 用于请求消息内容保存
    VOS_Mutex* m_pReqMsgMutex;
    map<unsigned long, SIP::REQ_MSG_INFO*>  m_msgMap;
    map<unsigned long, SIP::REQ_MSG_INFO*>::iterator m_msgItertor;
    unsigned long m_mapKey;

    unsigned long m_reserved;

    // 频繁调用SetRegisterInfo,对链表加锁保护
    // for m_listSvrInfoUsed
    VOS_Mutex* m_pThreadlistSvrInfoMutex;
    // for m_vectorServerInfo
    VOS_Mutex* m_pThreadServerInfoMutex;


    //用于处理前端重发SIP报文的数据结构。
    int HandleReqResend( const unsigned long enHeader, const SipTptNwInfoS& stTptNwInf, const unsigned long nCSeq );
    int HandleRspResend( const unsigned long enHeader, const SipTptNwInfoS& stTptNwInf, const unsigned long nCSeq );
    class CDealResendData
    {
    public:
        CDealResendData ();
        CDealResendData ( const std::string& strIPv4, unsigned short usPort,  unsigned long nCSeq );


    public:
        const std::string& GetIPv4() const;
        unsigned short GetPort() const;
        unsigned long GetCSeq() const;

        bool IsTheSame ( const CDealResendData& other );
        bool IsTimeout ();

    private:
        const std::string    m_strIPv4;
        const unsigned short m_usPort;
        const unsigned long  m_nCSeq;


        time_t               m_tResendTime;
    };
    typedef std::list<CDealResendData*>         DEAL_RESEND_LIST;
    typedef std::map<int, DEAL_RESEND_LIST>    DEAL_RESEND_HEADER_MAP;

    DEAL_RESEND_HEADER_MAP m_mapReqResend;
    DEAL_RESEND_HEADER_MAP m_mapRspResend;
};

}//end namespace

#endif //_C_SIP_STACK_H_HEADER_INCLUDED_

