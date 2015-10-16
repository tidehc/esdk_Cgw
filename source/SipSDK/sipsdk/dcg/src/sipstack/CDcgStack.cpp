#include <sstream>
#include <cstdio>

#include <ace_header.h>
#include <MiniSip.h>

#include "sip_namespace.h"
#include "CDcgStack.hpp"
#include "CSipStack.hpp"
#include "sipstack_agent_server_impl.hpp"
#include "Log.h"

//lint -e438
namespace SipStack{

static void HandleRespondEvent(CDcgStack* pStack, SIP::TEXT_MSG_TYPE type_func, SIP_HEADER *sipHeader, const unsigned long retCode)
{
    if (NULL==pStack )
    {
        ERROR_LOG("no handler found");
        return ;
    }

    // cout<< " cout deal event handler \n";

    SipStackAgentServerImpl *pHandler = NULL;
    pHandler = pStack->GetServer();//lint !e838
    if (NULL == pHandler)
    {
        INFO_LOG("deal event handler event handler failed");
        return ;
    }

    pHandler->DispatchRespMsg((SIP_STACK_MSG_TYPE)type_func, sipHeader, retCode);

    return ;
}

static void HandleRequestEvent(CDcgStack* pStack, const SIP::TEXT_MSG_TYPE type_func, SIP_HEADER *sipHeader, const unsigned long retCode)
{
    // cout<<SipStack::genCurTime()<<"|"<<"deal event handler request event handler success: "<<type_func<<endl;

    if (NULL==pStack)
    {
        ERROR_LOG("no handler found");
        return ;
    }

    SipStackAgentServerImpl *pHandler = NULL;
    pHandler = pStack->GetServer();//lint !e838
    if (NULL == pHandler)
    {
        ERROR_LOG("deal event handler event handler failed\n");
        return ;
    }

    pHandler->DispatchReqMsg((SIP_STACK_MSG_TYPE)type_func, sipHeader, retCode);

    return ;
}

/********************************************************************************************************
*                                 SIP STACK CALLBACK FUNCTION                                           *
*********************************************************************************************************/
static long STDCALL HandleNotifyCallBack
    (
    IN SIP::NOTIFY_INFO* pstNotifyInfo, 
    IN void* pUserData
    )
{
    if (NULL == pstNotifyInfo)
    {
        ERROR_LOG("NVS Stack Handle Notify Call Back Failed. Notify Info is Empty.");
        return 1;
    }

    if (NULL == pUserData)
    {
        ERROR_LOG("NVS user data NULL error");
        return 1;
    }

    //参数指针安全转换
    SipStack::CDcgStack* pStackHandler = (SipStack::CDcgStack*)(pUserData);

    long lResult = 0;
    switch (pstNotifyInfo->ulNotifyType)
    {
    case SIP::NOTIFY_TYPE_TEXT_MSG_RESP:  //文本消息响应
        {
            if (NULL == pstNotifyInfo->pNotifyInfo)
            {
                break;
            }

            SIP::NOTIFY_TEXT_MSG_RESP *pMsg = (SIP::NOTIFY_TEXT_MSG_RESP*) pstNotifyInfo->pNotifyInfo;
            SIP_HEADER *pMsgData = static_cast<SIP_HEADER*>(pMsg->stTextMsg.pUserData);

            INFO_LOG("HandleNotifyCallBack response from = %s", pMsgData->from.c_str());
            HandleRespondEvent(pStackHandler, pMsg->stTextMsg.enTextMsgType, pMsgData, pMsg->ulRetCode);

            break;
        }

    case SIP::NOTIFY_TYPE_TEXT_MSG_REQ:    //文本消息请求
        {
            if (NULL == pstNotifyInfo->pNotifyInfo)
            {
                break;
            }

            SIP::NOTIFY_TEXT_MSG_REQ *pMsg = (SIP::NOTIFY_TEXT_MSG_REQ *) pstNotifyInfo->pNotifyInfo;
            SIP_HEADER *pMsgData = static_cast<SIP_HEADER*>(pMsg->pUserData);

            INFO_LOG("HandleNotifyCallBack request from = %s",pMsgData->from.c_str());
            HandleRequestEvent(pStackHandler, pMsg->enTextMsgType, pMsgData, 0);

            break;
        }

    default:
        {
            // error
            INFO_LOG("handle unknown message");
            break;
        }
    }

    INFO_LOG("NVS Stack Handle Notify Call Back Success.");
    return lResult;
}//lint !e818

/********************************************************************************************************
*                                 DCG Stack Class                                                       *
*********************************************************************************************************/
CDcgStack::CDcgStack(): m_pServer(NULL), m_localPort(0),m_serverPort(0)
{
    m_pSipStack = new CSipStack();
};

CDcgStack::~CDcgStack()
{
	if(NULL != m_pSipStack)
	{
		delete m_pSipStack;
		m_pSipStack = NULL;
	}
}

long CDcgStack::Init(SipStackAgentServerImpl *sipImpl)
{
    if (NULL != sipImpl )
    {
        m_pServer = sipImpl;
    }

    long lResult = m_pSipStack->Init((SIP::PCALLBACK_NOTIFY)HandleNotifyCallBack, this, m_localPort);

    g_pSipStack = m_pSipStack;

	return lResult;
}

bool CDcgStack::setRegInfo(const SipStackAgentCfg &agent_cfg)
{
    if (agent_cfg.m_login_ip_str.empty())
    {
        return false;
    }

    SIP::REGISTER_INFO register_inf_st;
    memset (&register_inf_st, 0, sizeof(register_inf_st));

    register_inf_st.ulServerInfoNum = 1;
    register_inf_st.stServerInfoArray[0].usServerPort = (unsigned short)::atoi(agent_cfg.m_login_port_str.c_str());
    (void)strcpy_s(register_inf_st.stServerInfoArray[0].szServerIP, agent_cfg.m_login_ip_str.c_str());
    (void)strcpy_s(register_inf_st.szLoginDomain, agent_cfg.m_login_domain_str.c_str());
    (void)strcpy_s(register_inf_st.szLoginName, agent_cfg.m_login_name_str.c_str());
    (void)strcpy_s(register_inf_st.szPassword, agent_cfg.m_login_pwd_str.c_str());
    (void)strcpy_s(register_inf_st.szProductName, agent_cfg.m_agent_user_name_str.c_str());
    (void)strcpy_s(register_inf_st.szProductVersion, agent_cfg.m_agent_user_version_str.c_str());
    (void)strcpy_s(register_inf_st.szLocalIp, agent_cfg.m_local_ip_str.c_str());
    (void)strcpy_s(register_inf_st.szLocalPort, agent_cfg.m_local_port_str.c_str());
	(void)strcpy_s(register_inf_st.szUriDomain, agent_cfg.m_login_domain_str.c_str());
	(void)strcpy_s(register_inf_st.szSipServerId,agent_cfg.m_ServerID.c_str());
	(void)strcpy_s(register_inf_st.szLocalId,agent_cfg.m_LocalID.c_str());
	(void)strcpy_s(register_inf_st.szRemoteLoginName, agent_cfg.m_server_sipname.c_str());
	(void)strcpy_s(register_inf_st.szRemotePassword, agent_cfg.m_server_sippwd.c_str());
    register_inf_st.ulExpireTime = CSipStackConfigMgr::Instance().GetServiceCfg().GetExpires();
    m_localPort = (unsigned short)atoi(register_inf_st.szLocalPort);
    if (m_localPort == 0)
    {
        m_localPort = 9081;
    }

    if (NULL != m_pSipStack)
    {
        m_pSipStack->SetRegisterInfo(register_inf_st);
    }

    return true;
}

bool CDcgStack::sendRegister(const CSipVar &sipVar)
{
    SIP::TEXT_MSG msg;
    memset(&msg, 0, sizeof(msg));
    msg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_REGISTER;
    msg.ulMsgSeq=0;

    SIP::PEER_URI_INFO *pUriInfo = new SIP::PEER_URI_INFO;
    memset(pUriInfo, 0, sizeof(*pUriInfo));
    memcpy(pUriInfo->szUriUserName, sipVar.m_strRequestUriUserInfo.c_str(), sipVar.m_strRequestUriUserInfo.length());
    memcpy(pUriInfo->szUriHostName, sipVar.m_strRequestUriDomain.c_str(), sipVar.m_strRequestUriDomain.length());
    memcpy(pUriInfo->szUserAgentName, sipVar.m_strAgentName.c_str(), sipVar.m_strAgentName.length());
    msg.pUserData = pUriInfo;

    if (NULL == m_pSipStack)
    {
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendSipMsg(msg,sipVar.m_strCallID))
    {
        // | function | SEND | SUCCESS | 
        ERROR_LOG("CDcgStack::sendRegister failed");
        /*SIP_LOG_ERR_SEND("CDcgStack::sendRegister | SEND | FAILED | send register failed");*/

        delete pUriInfo;

        return false;
    }

    INFO_LOG("CDcgStack::sendRegister success");

    delete pUriInfo;

    return true;
}

bool CDcgStack::sendUnRegister(const CSipVar &sipVar)
{
	SIP::TEXT_MSG msg;
	memset(&msg, 0, sizeof(msg));
	msg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_UNREGISTER;
	msg.ulMsgSeq=0;

	SIP::PEER_URI_INFO *pUriInfo = new SIP::PEER_URI_INFO;
	memset(pUriInfo, 0, sizeof(*pUriInfo));
	memcpy(pUriInfo->szUriUserName, sipVar.m_strRequestUriUserInfo.c_str(), sipVar.m_strRequestUriUserInfo.length());
	memcpy(pUriInfo->szUriHostName, sipVar.m_strRequestUriDomain.c_str(), sipVar.m_strRequestUriDomain.length());
	memcpy(pUriInfo->szUserAgentName, sipVar.m_strAgentName.c_str(), sipVar.m_strAgentName.length());
	msg.pUserData = pUriInfo;

	if (NULL == m_pSipStack)
	{
		throw exception();
	}

	if (SIP::RET_CODE_OK != m_pSipStack->SendSipMsg(msg,sipVar.m_strCallID))
	{
		// | function | SEND | SUCCESS | 
		ERROR_LOG("CDcgStack::sendUnRegister failed");
		/*SIP_LOG_ERR_SEND("CDcgStack::sendRegister | SEND | FAILED | send register failed");*/

		delete pUriInfo;

		return false;
	}

	INFO_LOG("CDcgStack::sendUnRegister success");

	delete pUriInfo;

	return true;

}

bool CDcgStack::sendMsg(const string &message)
{
    SIP::TEXT_MSG msg;
    memset(&msg, 0, sizeof(msg));
    msg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_MESSAGE;
    msg.ulMsgSeq=2;
    msg.ulMsgBodyLen = message.length();
    msg.pszMsgBody = (char *)message.c_str();//lint !e1773

    if (NULL == m_pSipStack)
    {
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendSipMsg(msg))
    {
        return false;
    }

    /* SIP_LOG_INF_SEND("CDcgStack::sendMsg | SEND | SUCCESS | send message success");*/
    return true;
}

bool CDcgStack::sendMsg(const string &message, const CSipVar &sipVar)
{
    SIP::TEXT_MSG msg;
    memset(&msg, 0, sizeof(msg));
    msg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_MESSAGE;
    msg.ulMsgSeq=0;
    msg.ulMsgBodyLen = message.length();
    msg.pszMsgBody = (char *)message.c_str();//lint !e1773

    SIP::PEER_URI_INFO *pUriInfo = new SIP::PEER_URI_INFO;
    memset(pUriInfo, 0, sizeof(*pUriInfo));
    memcpy(pUriInfo->szUriUserName, sipVar.m_strRequestUriUserInfo.c_str(), sipVar.m_strRequestUriUserInfo.length());
    memcpy(pUriInfo->szUriHostName, sipVar.m_strRequestUriDomain.c_str(), sipVar.m_strRequestUriDomain.length());
    memcpy(pUriInfo->szUserAgentName, sipVar.m_strAgentName.c_str(), sipVar.m_strAgentName.length());
	memcpy(pUriInfo->szCallID, sipVar.m_strCallID.c_str(), sipVar.m_strCallID.length());
    msg.pUserData = pUriInfo;

    if (NULL == m_pSipStack)
    {
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendSipMsg(msg,sipVar.m_strCallID))
    {
        delete pUriInfo;

        return false;
    }

    delete pUriInfo;

    return true;
}

bool CDcgStack::sendMsg(const string &message, const CSipVar &sipVar, unsigned long msgSeq)
{
    SIP::TEXT_MSG msg;
    memset(&msg, 0, sizeof(msg));
    msg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_MESSAGE;
    msg.ulMsgSeq=msgSeq;
    msg.ulMsgBodyLen = message.length();
    msg.pszMsgBody = (char *)message.c_str();//lint !e1773

    SIP::PEER_URI_INFO *pUriInfo = new SIP::PEER_URI_INFO;
    memset(pUriInfo, 0, sizeof(*pUriInfo));
    memcpy(pUriInfo->szUriUserName, sipVar.m_strRequestUriUserInfo.c_str(), sipVar.m_strRequestUriUserInfo.length());
    memcpy(pUriInfo->szUriHostName, sipVar.m_strRequestUriDomain.c_str(), sipVar.m_strRequestUriDomain.length());
    memcpy(pUriInfo->szUserAgentName, sipVar.m_strAgentName.c_str(), sipVar.m_strAgentName.length());
    msg.pUserData = pUriInfo;

    if (NULL == m_pSipStack)
    {
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendSipMsg(msg))
    {
        delete pUriInfo;

        return false;
    }

    delete pUriInfo;

    return true;
}

bool CDcgStack::sendInvite (const string &message)
{
    SIP::TEXT_MSG msg;
    memset(&msg, 0, sizeof(msg));
    msg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_INVITE;
    msg.ulMsgSeq=2;
    msg.ulMsgBodyLen = message.length();
    msg.pszMsgBody = (char *)message.c_str();//lint !e1773

    if (NULL == m_pSipStack)
    {
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendSipMsg(msg))
    {
        /*SIP_LOG_ERR_SEND("CDcgStack::sendInvite | SEND | FAILED | send invite failed");*/

        return false;
    }

    /*SIP_LOG_INF_SEND("CDcgStack::sendInvite | SEND | SUCCESS | send invite success");*/

    return true;
}

bool CDcgStack::sendInvite(const string &message, const CSipVar &sipVar)
{
    if (NULL == m_pSipStack)
    {
        INFO_LOG("failure to send invite - sip stack pointer is null");
        return false;
    }

    //SIP文本消息结构体
    SIP::TEXT_MSG msg;
    memset(&msg, 0, sizeof(msg));
    msg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_INVITE;
    msg.ulMsgSeq = getSipSeq();                                       //生成SEQ
    msg.ulMsgBodyLen = message.size();                                //获得消息长度
    msg.pszMsgBody = (char *)message.c_str();//lint !e1773
    msg.ulDialogId = getSipSeq();

    SIP::REQ_HEADER_INVITE* pMsgHeader = new SIP::REQ_HEADER_INVITE;
    memset(pMsgHeader, 0, sizeof(*pMsgHeader));
    (void)strncpy_s(pMsgHeader->szSubject, sipVar.m_strSubject.c_str(), sipVar.m_strSubject.length());
    msg.ulMsgHeaderLen = sizeof(*pMsgHeader);
    msg.pMsgHeader = pMsgHeader;

    SIP::PEER_URI_INFO* uri_info_ptr = new SIP::PEER_URI_INFO;
    memset(uri_info_ptr, 0, sizeof(SIP::PEER_URI_INFO));
    (void)strncpy_s(uri_info_ptr->szUriUserName, sipVar.m_strRequestUriUserInfo.c_str(), sipVar.m_strRequestUriUserInfo.size());
    (void)strncpy_s(uri_info_ptr->szUriHostName, sipVar.m_strRequestUriDomain.c_str(), sipVar.m_strRequestUriDomain.size());
    (void)strncpy_s(uri_info_ptr->szUserAgentName, sipVar.m_strAgentName.c_str(), sipVar.m_strAgentName.size());
    msg.pUserData = uri_info_ptr;

    if (SIP::RET_CODE_OK != m_pSipStack->SendSipMsg(msg, sipVar.m_strCallID))
    {
        delete pMsgHeader;
        pMsgHeader = NULL;
        delete uri_info_ptr;
        uri_info_ptr = NULL;
        ERROR_LOG("failure to send invite - send sip message failed.");
        return false;
    }

    INFO_LOG("success to send invite.");

    delete pMsgHeader;
    pMsgHeader = NULL;
    delete uri_info_ptr;
    uri_info_ptr = NULL;
    return true;
}

bool CDcgStack::sendInviteResp (const string &message, unsigned long seq, unsigned long retCode)
{
    if (NULL == m_pSipStack)
    {
        ERROR_LOG("CDcgStack::sendInviteResp failed.");
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendInviteResp(message, seq, retCode))
    {
        ERROR_LOG("CDcgStack::sendInviteResp failed.");
        return false;
    }

    INFO_LOG("CDcgStack::sendInviteResp success");

    return true;
}

bool CDcgStack::sendOption (const CSipVar &sipVar)
{
    SIP::TEXT_MSG msg;
    memset(&msg, 0, sizeof(msg));
    msg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_OPTIONS;
    msg.ulMsgSeq=0;
    msg.pMsgHeader = NULL;

    SIP::PEER_URI_INFO *pUriInfo = new SIP::PEER_URI_INFO;
    memset(pUriInfo, 0, sizeof(*pUriInfo));
    memcpy(pUriInfo->szUriUserName, sipVar.m_strRequestUriUserInfo.c_str(), sipVar.m_strRequestUriUserInfo.length());
    memcpy(pUriInfo->szUriHostName, sipVar.m_strRequestUriDomain.c_str(), sipVar.m_strRequestUriDomain.length());
    memcpy(pUriInfo->szUserAgentName, sipVar.m_strAgentName.c_str(), sipVar.m_strAgentName.length());
    msg.pUserData = pUriInfo;

    if (NULL == m_pSipStack)
    {
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendSipMsg(msg))
    {
        ERROR_LOG("CDcgStack::sendOption failed");

        delete pUriInfo;

        return false;
    }

    INFO_LOG("CDcgStack::sendOption success");

    delete pUriInfo;

    return true;
}

bool CDcgStack::sendOption (void)
{
    SIP::TEXT_MSG msg;
    memset(&msg, 0, sizeof(msg));
    msg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_OPTIONS;
    msg.ulMsgSeq=0;

    if (NULL == m_pSipStack)
    {
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendSipMsg(msg))
    {
        ERROR_LOG("CDcgStack::sendOption failed");
        return false;
    }

    INFO_LOG("CDcgStack::sendOption success");
    return true;
}

unsigned long CDcgStack::getSipSeq(void)
{
    if (NULL != m_pSipStack)
    {
        return m_pSipStack->GetCSeq();
    }

    return 0;
}

void CDcgStack::Fini()
{
	if(NULL != m_pSipStack)
	{
		m_pSipStack->Fini();
		delete m_pSipStack;
		m_pSipStack = NULL;
		g_pSipStack = NULL;
	}
    //delete m_pServer;
    m_pServer = NULL; //由mgr负责释放
}

SipStackAgentServerImpl *CDcgStack::GetServer(void)
{
    return this->m_pServer;
}

long CDcgStack::sendResp(const unsigned long seq)
{
    if (NULL == m_pSipStack)
    {
        return -1;
    }

    return m_pSipStack->SendResp(seq);
}

bool CDcgStack::ansMsg(const string& ans_code, const string& data)
{
    long lRet = m_pSipStack->AnsMsg(ans_code, data);
    if ( 0 != lRet )
    {
        return false;
    }

    return true;
}

///////////////////////////////////////////  add by w00207012 2012-11-02 Begin //////////////////////////////////////////////////
long CDcgStack::setServerAddrEx(bool bServerAddrEx, const string& serverIP, unsigned short serverPort)
{
    if ((NULL == m_pSipStack))
    {
        ERROR_LOG("CDcgStack::setServerAddrEx failed.");
        return -1;
    }

    return m_pSipStack->SetServerAddrEx(bServerAddrEx, serverIP, serverPort);
}

bool CDcgStack::sendRegisterResp (unsigned long seq, unsigned long retCode)
{
    if (NULL == m_pSipStack)
    {
        ERROR_LOG("CDcgStack::SendRegisterResp | SEND | FAILED | send register resp failed.");
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendRegisterResp(seq, retCode))
    {
        ERROR_LOG("CDcgStack::SendRegisterResp | SEND | FAILED | send register resp failed.");
        return false;
    }

    INFO_LOG("CDcgStack::SendRegisterResp | SEND | SUCCESS | send register resp success.");

    return true;
}

bool CDcgStack::sendCommonMsgResp (unsigned long seq, unsigned long retCode)
{
    if (NULL == m_pSipStack)
    {
        ERROR_LOG("CDcgStack::sendKeepaliveResp | SEND | FAILED | send Keepalive resp failed.");
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendKeepaliveResp(seq, retCode))
    {
        ERROR_LOG("CDcgStack::sendKeepaliveResp | SEND | FAILED | send Keepalive resp failed.");
        return false;
    }

    INFO_LOG("CDcgStack::sendKeepaliveResp | SEND | SUCCESS | send Keepalive resp success.");

    return true;
}

bool CDcgStack::sendOptionMsgResp (unsigned long seq, unsigned long retCode)
{
    if (NULL == m_pSipStack)
    {
        ERROR_LOG("CDcgStack::sendOptionMsgResp | SEND | FAILED | send Option resp failed.");
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendOptionResp(seq, retCode))
    {
        ERROR_LOG("CDcgStack::sendOptionMsgResp | SEND | FAILED | send Option resp failed.");
        return false;
    }

    INFO_LOG("CDcgStack::sendOptionMsgResp | SEND | SUCCESS | send Option resp success.");

    return true;
}

bool CDcgStack::sendInviteAck(unsigned long seq)
{
    if (NULL == m_pSipStack)
    {
        ERROR_LOG("CDcgStack::sendInviteAck | SEND | FAILED | send INVITE ACK failed.");
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendInviteAck(seq))
    {
        ERROR_LOG("CDcgStack::sendInviteAck | SEND | FAILED | send INVITE ACK failed.");
        return false;
    }

    INFO_LOG("CDcgStack::sendInviteAck | SEND | SUCCESS | send INVITE ACK success.");

    return true;
}

bool CDcgStack::sendByeReq(unsigned long seq)
{
    if (NULL == m_pSipStack)
    {
        ERROR_LOG("CDcgStack::sendByeReq | SEND | FAILED | send INVITE Bye failed.");
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendInviteBye(seq))
    {
        ERROR_LOG("CDcgStack::sendByeReq | SEND | FAILED | send INVITE Bye failed.");
        return false;
    }

    INFO_LOG("CDcgStack::sendByeReq | SEND | SUCCESS | send INVITE Bye success.");

    return true;
}

bool CDcgStack::sendCancelReq(unsigned long seq)
{
    if (NULL == m_pSipStack)
    {
        ERROR_LOG("CDcgStack::sendInviteAck | SEND | FAILED | send INVITE ACK failed.");
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendInviteCancel(seq))
    {
        ERROR_LOG("CDcgStack::sendInviteAck | SEND | FAILED | send INVITE ACK failed.");
        return false;
    }

    INFO_LOG("CDcgStack::sendInviteAck | SEND | SUCCESS | send INVITE ACK success.");

    return true;
}

bool CDcgStack::sendSubscribeReq(const string &message, const CSipVar &sipVar)
{
    SIP::TEXT_MSG msg;
    memset(&msg, 0, sizeof(msg));
    msg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_SUBSCRIBE;
    msg.ulMsgSeq = getSipSeq();
    msg.ulMsgBodyLen = message.length();
    msg.pszMsgBody = (char *)message.c_str();//lint !e1773

    SIP::PEER_URI_INFO *pUriInfo = new SIP::PEER_URI_INFO;
    memset(pUriInfo, 0, sizeof(*pUriInfo));
    memcpy(pUriInfo->szUriUserName, sipVar.m_strRequestUriUserInfo.c_str(), sipVar.m_strRequestUriUserInfo.length());
    memcpy(pUriInfo->szUriHostName, sipVar.m_strRequestUriDomain.c_str(), sipVar.m_strRequestUriDomain.length());
    memcpy(pUriInfo->szUserAgentName, sipVar.m_strAgentName.c_str(), sipVar.m_strAgentName.length());
    msg.pUserData = pUriInfo;

    if (NULL == m_pSipStack)
    {
        ERROR_LOG("CDcgStack::sendSubscribeReq failed.");
        throw exception();
    }

    if (SIP::RET_CODE_OK != m_pSipStack->SendSipMsg(msg))
    {
        ERROR_LOG("CDcgStack::sendSubscribeReq failed.");
        delete pUriInfo;
        return false;
    }

    INFO_LOG("CDcgStack::sendSubscribeReq success.");

    delete pUriInfo;

    return true;
}
///////////////////////////////////////////  add by w00207012 2012-11-02 End ////////////////////////////////////////////////////

}

//lint +e438

