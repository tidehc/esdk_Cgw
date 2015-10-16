#ifndef IVSCBB_SS_SIPSTACK_WRAP_HPP
#define IVSCBB_SS_SIPSTACK_WRAP_HPP

#include "CSipStack.hpp"

namespace SipStack{

class CSipStackMsgWrap
{
public:
    /**
    * Description:    CSipMsg(). Default constructor
    */
    CSipStackMsgWrap(CSipStack& objSipStack, const SIP::TEXT_MSG& stTextMsg);

    /**
    * Description:  SetNwAddrInf  设置网络地址信息
    * @return       int.         返回码
    */
    int SetNwAddrInf( const string& ip, const unsigned short port );

	    /**
    * Description:  SetlcAddrInf  设置本机地址信息
    * @return       int.         返回码
    */
    int SetLcAddrInf( const string& ip, const unsigned short port );

    /**
    * Description:    ~CSipMsg(). Destructor
    */
    virtual ~CSipStackMsgWrap();

    /**
    * Description:  SendMsg().  发送消息
    * @return       long.       返回码
    */
    virtual long SendMsg();

protected:
    /**
    * Description:  CreateSipReqMsg().  创建SIP请求消息
    * @param  [in]  enSipMethod SIP请求类型
    * @param  [out] pstSipMsg   SIP消息指针
    * @return       long.       返回码
    */
    long CreateSipReqMsg
    (
        EN_SIP_METHOD           enSipMethod, 
        SipMsgS*&               pstSipMsg
    );

    /**
    * Description:  SetSpecialHeader().     设置特殊头域
    * @param  [in]  stSipMsg   SIP消息引用
    * @return       long.       返回码
    */
    virtual long SetSpecialHeader
    (
        SipMsgS& stSipMsg
    );

    /**
    * Description:  SetVarHeader().     设置可变头域
    * @param  [in]  stSipMsg   SIP消息引用
    * @return       long.       返回码
    */
    long SetVarHeader
    (
        SipMsgS& stSipMsg
    );

    /**
    * Description:  SendSipMsg().   发送SIP消息
    * @param  [in]  stSipMsg   SIP消息引用
    * @param  [in]  objServerAddr    接收SIP消息服务器地址  // add by w00207027 2012-11-02
    * @return       long.       返回码
    */
    virtual long SendSipMsg
    (
        SipMsgS& stSipMsg
        //CNetworkAddr& objServerAddr
    );

    /**
    * Description:  SendSipReqMsgWithVarHdr().  发送带可变头域的SIP请求消息
    * @param  [in]  stSipMsg    SIP消息引用
    * @return       long.       返回码
    */
    long SendSipReqMsgWithVarHdr
    (
        SipMsgS& stSipMsg
    );

    /**
    * Description:  PostSendSuccess().  发送成功后处理
    * @return       void. 
    */
    virtual void PostSendSuccess();

    /**
    * Description:  SipTxnHiSend().  调用MiniSIP的发送接口
    * @param  [in]  stSipSdu             SIP的消息数据单元
    * @param  [in]  objServerAddr    接收SIP消息服务器地址  // add by w00207027 2012-11-02
    * @param  [out] ulTuObjId            对应的TU对象ID
    * @param  [out] ulTxnObjId          对应的事务对象ID
    * @return       long.       返回码
    */
    long SipTxnHiSend
    (
        SipDataUnitS&   stSipSdu,
        SS_UINT32&      ulTxnObjId,
        SS_UINT32&      ulTuObjId
    );

    friend class CSipStack;

protected:
    //SIP协议栈对象引用
    CSipStack& m_objSipStack;
    //文本消息结构体引用
    const SIP::TEXT_MSG& m_stTextMsg;

    //Request-URI的Name
    const char* m_pszRequestUriName;
    //Request-URI的Domain
    const char* m_pszRequestUriDomain;
    // User-Agent的Name
    const char* m_pszUserAgentName;
    //SIP消息的CSeq
    unsigned long m_ulCSeq;

    //CallID
    string m_strCallId;
    //From的Tag
    string m_strFromTag;
    //To的Tag
    const char* m_pszToTag;

    //modify-cwx148380-begin 2013-1-15 14:25:34
    string m_strIP;
    unsigned short m_usPort;
    //modify-cwx148380-end 2013-1-15 14:25:34
	string m_strLocalIP;
	unsigned short m_usLocalPort;
};

/**
* Class name:   CSipMsgRegister
* @brief        SIP Register消息
*
* Description:  
*/
class CSipStackMsgWrapRigister : public CSipStackMsgWrap
{
public:
    /**
    * Description:    CSipMsgRegister(). Default constructor
    */
    CSipStackMsgWrapRigister(CSipStack& objSipStack, const SIP::TEXT_MSG& stTextMsg, const string& callID);

protected:
    /**
    * Description:  SetSpecialHeader().     设置特殊头域
    * @param  [in]  stSipMsg   SIP消息引用
    * @return       long.       返回码
    */
    virtual long SetSpecialHeader
    (
        SipMsgS&                stSipMsg
    );

    /**
    * Description:  PostSendSuccess().  发送成功后处理
    * @return       void. 
    */
    virtual void PostSendSuccess();

protected:
    //超时时间
    unsigned long m_ulExpireTime;
};

/**
* Class name:   CSipMsgUnRegister
* @brief        SIP Register消息，用于注销
*
* Description:  
*/
class CSipStackMsgWrapUnRigister : public CSipStackMsgWrapRigister
{
public:
    /**
    * Description:    CSipMsgUnRegister(). Default constructor
    */
    CSipStackMsgWrapUnRigister(CSipStack& objSipStack, const SIP::TEXT_MSG& stTextMsg,const string& callID);

protected:
    /**
    * Description:  PostSendSuccess().  发送成功后处理
    * @return       void. 
    */
    virtual void PostSendSuccess();
};

/**
* Class name:   CSipMsgMessage
* @brief        SIP Message消息
*
* Description:  
*/
class CSipStackMsgWrapMessage : public CSipStackMsgWrap
{
public:
    /**
    * Description:    CSipMsgMessage(). Default constructor
    */
    CSipStackMsgWrapMessage(CSipStack& objSipStack, const SIP::TEXT_MSG& stTextMsg, const string& callID);

protected:
    /**
    * Description:  SetSpecialHeader().     设置特殊头域
    * @param  [in]  stSipMsg   SIP消息引用
    * @return       long.       返回码
    */
    virtual long SetSpecialHeader
    (
        SipMsgS&                stSipMsg
    );

    /**
    * Description:  PostSendSuccess().  发送成功后处理
    * @return       void. 
    */
    virtual void PostSendSuccess();
};

/**
* Class name:   CSipMsgMessage
* @brief        SIP Message消息
*
* Description:  
*/
class CSipStackMsgWrapOptions : public CSipStackMsgWrap
{
public:
    /**
    * Description:    CSipMsgMessage(). Default constructor
    */
    CSipStackMsgWrapOptions(CSipStack& objSipStack, const SIP::TEXT_MSG& stTextMsg);

protected:
    /**
    * Description:  PostSendSuccess().  发送成功后处理
    * @return       void. 
    */
    virtual void PostSendSuccess();
};

/**
* Class name:   CSipMsgInvite
* @brief        SIP Invite消息
*
* Description:  
*/
class CSipStackMsgWrapInvite : public CSipStackMsgWrap
{
public:
    /**
    * Description:    CSipMsgInvite(). Default constructor
    */
    CSipStackMsgWrapInvite ( CSipStack& objSipStack, const SIP::TEXT_MSG& stTextMsg, const string& callID );

protected:
    /**
    * Description:  SetSpecialHeader().     设置特殊头域
    * @param  [in]  stSipMsg   SIP消息引用
    * @return       long.       返回码
    */
    virtual long SetSpecialHeader
    (
        SipMsgS&                stSipMsg
    );

    /**
    * Description:  SendMsg().  发送消息
    * @return       long.       返回码
    */
    virtual long SendMsg();

    /**
    * Description:  PostSendSuccess().  发送成功后处理
    * @return       void. 
    */
    virtual void PostSendSuccess();

private:
    //INVITE的请求头域参数
    SIP::REQ_HEADER_INVITE m_stReqHdrInvite;    
};

/**
* Class name:   CSipMsgAck
* @brief        SIP Invite的Ack响应消息
*
* Description:  
*/
class CSipStackMsgWrapAck : public CSipStackMsgWrap
{
public:
    /**
    * Description:    CSipMsgAck(). Default constructor
    */
    CSipStackMsgWrapAck(CSipStack& objSipStack, const SIP::TEXT_MSG& stTextMsg);

protected:
    /**
    * Description:  SendSipMsg().   发送SIP消息
    * @param  [in]  stSipMsg   SIP消息引用
    * @return       long.       返回码
    */
    long SendSipMsg
    (
        SipMsgS& stSipMsg
    );
};

/**
* Class name:   CSipMsgBye
* @brief        SIP BYE消息
*
* Description:  
*/
class CSipStackMsgWrapBye : public CSipStackMsgWrap
{
public:
    /**
    * Description:    CSipMsgBye(). Default constructor
    */
    CSipStackMsgWrapBye(CSipStack& objSipStack, const SIP::TEXT_MSG& stTextMsg);

    /**
    * Description:  SendMsg().  发送消息
    * @return       long.       返回码
    */
    virtual long SendMsg();

protected:
	/**
	* Description:  SipTxnHiSend().  调用MiniSIP的发送接口
	* @param  [in]  stSipSdu    SIP的消息数据单元
	* @param  [in]  objServerAddr    接收SIP消息服务器地址  // add by w00207027 2012-11-02
	* @param  [out] ulTuObjId   对应的TU对象ID
	* @param  [out] ulTxnObjId  对应的事务对象ID
	* @return       long.       返回码
	*/
	long SipTxnHiSend
	(
		SipDataUnitS&   stSipSdu,
		SS_UINT32&      ulTxnObjId,
		SS_UINT32&      ulTuObjId
	);

    /**
    * Description:  PostSendSuccess().  发送成功后处理
    * @return       void. 
    */
    virtual void PostSendSuccess();

protected:
    //事务层对象ID
    unsigned long m_ulTxnObjId;
    //事务用户对象ID
    unsigned long m_ulTuObjId;

    //是否为Cancel消息
    VOS_BOOLEAN m_bCancel;
};

/**
* Class name:   CSipMsgSubscribe
* @brief        SIP SUBSCRIBE请求消息
*
* Description:  
*/
class CSipStackMsgWrapSubscribe : public CSipStackMsgWrap
{
public:
    /**
    * Description:    CSipMsgSubscribe(). Default constructor
    */
    CSipStackMsgWrapSubscribe(CSipStack& objSipStack, const SIP::TEXT_MSG& stTextMsg);

protected:
    /**
    * Description:  SetSpecialHeader().     设置特殊头域
    * @param  [in]  stSipMsg   SIP消息引用
    * @return       long.       返回码
    */
    virtual long SetSpecialHeader
    (
        SipMsgS&                stSipMsg
    );

    /**
    * Description:  PostSendSuccess().  发送成功后处理
    * @return       void. 
    */
    virtual void PostSendSuccess();

private:
    //超时时间
    unsigned long m_ulExpireTime;
};

/**
* Class name:   CSipMsgInviteResp
* @brief        SIP Invite的响应消息
*
* Description:  
*/
class CSipStackMsgWrapInviteRespEx : public CSipStackMsgWrap
{
public:
    /**
    * Description:    CSipMsgInviteResp(). Default constructor
    */
    CSipStackMsgWrapInviteRespEx(CSipStack& objSipStack, const SIP::TEXT_MSG& stTextMsg, unsigned long retCode=200);

    /**
    * Description:  SendMsg().  发送消息
    * @return       long.       返回码
    */
    virtual long SendMsg();

private:
    unsigned long m_retCode;
};

/**
* Class name:   CSipMsgRedirect
* @brief        SIP 重定向消息
*
* Description:  
*/
class CSipStackMsgWrapRedirect : public CSipStackMsgWrap
{
public:
    /**
    * Description:    CSipMsgRedirect(). Default constructor
    */
    CSipStackMsgWrapRedirect(CSipStack& objSipStack, const SIP::TEXT_MSG& stTextMsg);

    /**
    * Description:  SendMsg().  发送消息
    * @return       long.       返回码
    */
    virtual long SendMsg();
};

/**
* Class name:   CSipMsgReAuth
* @brief        SIP 重鉴权消息
*
* Description:  
*/
class CSipStackMsgWrapReAuth : public CSipStackMsgWrap
{
public:
    /**
    * Description:    CSipMsgReAuth(). Default constructor
    */
    CSipStackMsgWrapReAuth(CSipStack& objSipStack, const SIP::TEXT_MSG& stTextMsg);

    /**
    * Description:  SendMsg().  发送消息
    * @return       long.       返回码
    */
    virtual long SendMsg();
};

/**
* Class name:   CSipRspMsg
* @brief        SIP响应消息
*
* Description:
*/
class CSipStackMsgWrapResp
{
public:
    /**
    * Description:    CSipRspMsg(). Default constructor
    */
    CSipStackMsgWrapResp(CSipStack& objSipStack, SipMsgS& stSipReqMsg, const string& data = "");

    /**
    * Description:    ~CSipRspMsg(). Destructor
    */
    virtual ~CSipStackMsgWrapResp();

    /**
    * Description:  SendRspMsg().  发送响应消息
    * @param  [in]  ulRespCode  响应码
    * @param  [in]  ulTuObjId   对应的TU对象ID
    * @param  [in]  ulTxnObjId  对应的事务对象ID
    * @return       long.       返回码
    */
    virtual long SendRspMsg
    (
        unsigned long   ulRespCode,
        unsigned long   ulTuObjId,
        unsigned long   ulTxnObjId
    );

protected:
    /**
    * Description:  SetSpecialHeader().     设置特殊头域
    * @param  [in]  stRspSipMsg   SIP响应消息引用
    * @return       long.       返回码
    */
    virtual long SetSpecialHeader
    (
        SipMsgS& stRspSipMsg
    );

protected:
    //SIP协议栈对象引用
    CSipStack& m_objSipStack;
    //SIP请求消息引用
    SipMsgS& m_stSipReqMsg;

    string m_SipMsgData;
};

/**
* Class name:   CSipRspMsgInvite
* @brief        SIP INVITE的响应消息
*
* Description:
*/
class CSipStackMsgWrapInviteResp : public CSipStackMsgWrapResp
{
public:
    /**
    * Description:    CSipRspMsgInvite(). Default constructor
    * @param  [in]  objSipStack     SipStack引用
    * @param  [in]  stSipReqMsg     SIP请求消息引用
    * @param  [in]  ulDialogId          对话ID
    * @param  [in]  szMsgBody       消息体字符串
    * @param  [in]  ulMsgBodyLen    消息体长度
    */
    CSipStackMsgWrapInviteResp
    (
        CSipStack&      objSipStack, 
        SipMsgS&        stSipReqMsg,
        unsigned long   ulDialogId,
        unsigned long   ulMsgBodyLen,
        char*           pszMsgBody
    );

    /**
    * Description:    ~CSipStackMsgWrapInviteResp(). Destructor
    */
    virtual ~CSipStackMsgWrapInviteResp();

protected:
    /**
    * Description:  SetSpecialHeader().     设置特殊头域
    * @param  [in]  stRspSipMsg     SIP响应消息引用
    * @return       long.       返回码
    */
    virtual long SetSpecialHeader
    (
        SipMsgS& stRspSipMsg
    );

private:
    unsigned long   m_ulDialogId;   //对话ID
    unsigned long   m_ulMsgBodyLen; //消息体的长度
    char*           m_pszMsgBody;   //消息体内容
};

/**
* Class name:   CSipRspMsgRegist
* @brief        SIP REGIST的响应消息
*
* Description:  
*/
class CSipStackMsgWrapRegisterResp : public CSipStackMsgWrapResp
{
public:
    /**
    * Description:    CSipRspMsgRegist(). Default constructor
    * @param  [in]  objSipStack     SipStack引用
    * @param  [in]  stSipReqMsg     SIP请求消息引用
    * @param  [in]  wwwAuth            响应带WWW_AUTHENTICATE头域
    */
    CSipStackMsgWrapRegisterResp
    (
        CSipStack&      objSipStack, 
        SipMsgS&        stSipReqMsg,
		bool                   wwwAuth
    );

    /**
    * Description:    ~CSipRspMsgRegist(). Destructor
    */
    virtual ~CSipStackMsgWrapRegisterResp();

protected:
    /**
    * Description:  SetSpecialHeader().     设置特殊头域
    * @param  [in]  stRspSipMsg     SIP响应消息引用
    * @return       long.       返回码
    */
    virtual long SetSpecialHeader
    (
        SipMsgS& stRspSipMsg
    );

private:

	bool m_bwwwAuth;

	//超时时间
	unsigned long m_ulExpireTime;
    //unsigned long   m_ulDialogId;   //对话ID
    //unsigned long   m_ulMsgBodyLen; //消息体的长度
    //char*           m_pszMsgBody;   //消息体内容
};

/**
* Class name:   CSipRspMsgOption
* @brief        SIP OPTION的响应消息
*
* Description:  
*/
class CSipStackMsgWrapOptionsResp : public CSipStackMsgWrapResp
{
public:
    /**
    * Description:    CSipRspMsgOption(). Default constructor
    * @param  [in]  objSipStack     SipStack引用
    * @param  [in]  stSipReqMsg     SIP请求消息引用
    */
    CSipStackMsgWrapOptionsResp
    (
        CSipStack&      objSipStack, 
        SipMsgS&        stSipReqMsg
    );

    /**
    * Description:    ~CSipRspMsgOption(). Destructor
    */
    virtual ~CSipStackMsgWrapOptionsResp();

protected:
    /**
    * Description:  SetSpecialHeader().     设置特殊头域
    * @param  [in]  stRspSipMsg     SIP响应消息引用
    * @return       long.       返回码
    */
    virtual long SetSpecialHeader
    (
        SipMsgS& stRspSipMsg
    );

private:
	//超时时间
	//unsigned long m_ulExpireTime;
};

}//end namespace

#endif //_C_SIP_MSG_H_HEADER_INCLUDED_

