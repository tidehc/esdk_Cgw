#include <ace_header.h>
#include <tinyxml.h>
#include "sipstack_task.h"
#include "CDcgStack.hpp"
#include "sipstack_agent_server_impl.hpp"
#include "sipstack_agent_msg_handler.hpp"
#include "sipstack_agent_msg_impl.hpp"
#include "sipstack_sync_agent_msg_handler_impl.hpp"
#include "sipstack_agent_client.hpp" 
#include "sipstack_exception.hpp"
#include "sipstack_agent_client_impl.hpp"
#include "Log.h"
//lint -e438
//lint -e429

namespace SipStack {

string GenCurTime(void)
{
    time_t oCurrTime;
    struct tm *oTmCurrTime;

    ACE_OS::time( &oCurrTime );
    struct tm t_tmTmp;
    oTmCurrTime = ACE_OS::localtime_r( &oCurrTime,&t_tmTmp );

    // szCurrTime 保存当前时间的数组 形式是:2005-06-07 15:36:41
	char szCurrTime[32] = {0};
    ACE_OS::strftime( szCurrTime, 32,"%Y-%m-%d %H:%M:%S", oTmCurrTime );

    return string(szCurrTime);
}

SipStackAgentClientImpl::SipStackAgentClientImpl(SipStackAgentServerImpl& agentServer)
                          :SipStackAgentClient(), m_agent_server(agentServer), m_sip_msg_seq_ul(1) 
{
}

SipStackAgentClientImpl::~SipStackAgentClientImpl()
{
}

SipStackAgentServer & SipStackAgentClientImpl::GetAgentServer()
{
    return m_agent_server;
}

void SipStackAgentClientImpl::SaveAgentMsgHandler(SipStackAgentMsgHandler* msgHandler)
{
    if ( NULL == msgHandler )
    {
        ERROR_LOG("save agent message handler - invalid param - handler is null");
        return ;
    }

    ACE_Write_Guard<ACE_RW_Mutex> guard(m_sip_msg_handler_map_rw_mutex);

    //此处修改为：发现相同的cmd，则使原先的cmd释放？
    ListSipCmd& listCmd = msgHandler->getCmdList();
    list<string>::iterator iterCmd = listCmd.begin();
    for (; iterCmd != listCmd.end(); iterCmd++)
    {
        (GetSipMessageHanlderMap())[*iterCmd] = msgHandler;
    }
    DEBUG_LOG("after regist map size= %d",GetSipMessageHanlderMap().size());
}//lint !e1788

void SipStackAgentClientImpl::SaveAgentMsgHandlerWithDeviceID(const string & devId, SipStackAgentMsgHandler* msgHandler)
{
    if ( devId.empty() || NULL == msgHandler )
    {
        ERROR_LOG("save agent message handler with device id - invalid param - %s",devId.c_str());
        return ;
    }

    ACE_Write_Guard<ACE_RW_Mutex> guard(m_sip_msg_handler_map_rw_mutex);

    DEBUG_LOG("device_id= %s",devId.c_str());
    (GetSipMessageHanlderMap())[devId] = msgHandler;
}//lint !e1788

void SipStackAgentClientImpl::DeleteAgentMsgHandler(SipStackAgentMsgHandler* msgHandler)
{
    ListSipCmd& listCmd = msgHandler->getCmdList();

    ACE_Write_Guard<ACE_RW_Mutex> guard(m_sip_msg_handler_map_rw_mutex);

    for ( list<string>::iterator iterCmd = listCmd.begin();
          iterCmd != listCmd.end();
          iterCmd++
        )
    {
        SipMessageHandlerMap::iterator iterHandler = m_sip_msg_handler_map.begin();
        while ( iterHandler != m_sip_msg_handler_map.end() )
        {
            if ( iterHandler->first == *iterCmd && iterHandler->second == msgHandler )
            {
                m_sip_msg_handler_map.erase(iterHandler++);
            }
            else
            {
                iterHandler++;
            }
        }
    }
    DEBUG_LOG("after unregiste map size= %d",GetSipMessageHanlderMap().size());
}//lint !e1788

void SipStackAgentClientImpl::DeleteAgentMsgHandlerWithDeviceID( const string& device_id_str )
{
    if ( device_id_str.empty() )
    {
        return ;
    }

    ACE_Write_Guard<ACE_RW_Mutex> guard(m_sip_msg_handler_map_rw_mutex);
    SipMessageHandlerMap::iterator iterHit = m_sip_msg_handler_map.find(device_id_str);
    if ( iterHit != m_sip_msg_handler_map.end() )
    {
        m_sip_msg_handler_map.erase(iterHit);
    }
}//lint !e1788

string SipStackAgentClientImpl::GetMsgSeq()
{
    ostringstream ossData;
    ossData <<  (++m_sip_msg_seq_ul)%0xffffffff;
    return ossData.str();
}

bool SipStackAgentClientImpl::AsynSendOption( const unsigned long resp_id_n,
                                                const unsigned long resp_code_n,
                                                const string&       strIPv4,
                                                const short         sPort
                                              )
{
    ERROR_LOG("client - asyn send option - unused.");
    return false;
}

bool SipStackAgentClientImpl::AsynSendAck ( const unsigned long resp_id_n,
                                             const string& strIPv4,
                                             const unsigned short usPort
                                            )
{
    INFO_LOG("client asyn-request ack - begin");

    CRequestAckToPu* req_p = new CRequestAckToPu(strIPv4, usPort, resp_id_n);
    if ( !req_p->IsValid() )
    {
        delete req_p;
        req_p = NULL;
        ERROR_LOG("client asyn-request ack - invalid request data");
        return false;
    }

    if ( !CSipStackTask::Instance().RequestToPu(req_p) ) //投入消息队列，若成功则由使用方释放。
    {
        delete req_p;
        req_p = NULL;
        ERROR_LOG("client asyn-request ack - failure to request to pu");
        return false;
    }

    INFO_LOG("client asyn-request ack - end");

    return true;
}

void SipStackAgentClientImpl::AsynSendMsg(const string & strReqData)
{
}

void SipStackAgentClientImpl::AsynSendMsg(const string & strReqData, const string & strReqUri)
{
}

void SipStackAgentClientImpl::AsynAnsMsg(const string& ans_code, const string& data )
{
}

auto_ptr<SipStackAgentMsg> SipStackAgentClientImpl::SyncSendInviteEx ( const string& strCallID,
                                                                    const string& strBody,
                                                                    const string& strUri,
                                                                    const string& strSubject,
                                                                    const string& strIPv4,
                                                                    const unsigned short usPort
                                                                  )
{
    INFO_LOG("client syn-request invite - begin");

    std::string uri_user_info_str = "";
    std::string uri_hostport_str = "";

    if ( SS_SUCCESS != CRequestToPu::ParseUri( strUri, uri_user_info_str, uri_hostport_str ) )
    {
        ERROR_LOG("client syn-request invite - failure to parse uri.");
        throw SipStackException("client syn-request invite - failure to parse uri.");
    }

    CRequestInviteToPu* req_p = new CRequestInviteToPu(strIPv4, usPort, uri_user_info_str, uri_hostport_str, strCallID, strSubject, strBody);
    if ( !req_p->IsValid() )
    {
        delete req_p;
        req_p = NULL;
        ERROR_LOG("client syn-request invite - invalid request data.");
        throw SipStackException("client syn-request invite - invalid request data.");
    }
	/*lint -save -e1036*/
    SipStackSyncAgentMsgHandlerImpl aSyncHdlr(*this, strCallID);
	/*lint -restore*/
    aSyncHdlr.Acquire();

    if ( !CSipStackTask::Instance().RequestToPu(req_p) ) //投入消息队列，若成功则由使用方释放。
    {
        delete req_p;
        req_p = NULL;
        aSyncHdlr.Release();
        ERROR_LOG("client syn-request invite - failure to request to pu.");
        throw SipStackException("client syn-request invite - failure to request to pu.");
    }

    aSyncHdlr.WaitMsg();
    aSyncHdlr.Release();

    INFO_LOG("client syn-request invite - end.");

    return aSyncHdlr.GetMsg();
}


string SipStackAgentClientImpl::SyncSendMsg(const string & strReqData)
{
    return "";
}

string SipStackAgentClientImpl::SyncSendMsg(const string & strBody, const string & strReqUri,const string& strIPv4, unsigned short usPort,const string& strCallID)
{
	INFO_LOG("client syn-request message - begin");

	std::string uri_user_info_str = "";
	std::string uri_hostport_str = "";

	if ( SS_SUCCESS != CRequestToPu::ParseUri( strReqUri, uri_user_info_str, uri_hostport_str ) )
	{
		ERROR_LOG("client syn-request message - failure to parse uri.");
		throw SipStackException("client syn-request invite - failure to parse uri.");
	}

	CRequestMessageToPu* req_p = new CRequestMessageToPu(strIPv4, usPort, uri_user_info_str, uri_hostport_str, strBody,strCallID);
	if ( !req_p->IsValid() )
	{
		delete req_p;
		req_p = NULL;
		ERROR_LOG("client syn-request message - invalid request data.");
		throw SipStackException("client syn-request message - invalid request data.");
	}

	if ( !CSipStackTask::Instance().RequestToPu(req_p) ) //投入消息队列，若成功则由使用方释放。
	{
		delete req_p;
		req_p = NULL;
		ERROR_LOG("client syn-request message - failure to request to pu.");
		throw SipStackException("client syn-request message - failure to request to pu.");
	}

	INFO_LOG("client syn-request message - end.");

	return "";
}

auto_ptr<SipStackAgentMsg> SipStackAgentClientImpl::SyncSendMsgEx(const string & strReqData)
{
    return std::auto_ptr<SipStackAgentMsg>();
}

auto_ptr<SipStackAgentMsg> SipStackAgentClientImpl::SyncSendMsgEx(const string & strReqData, const string & strReqUri)
{
    return std::auto_ptr<SipStackAgentMsg>();
}

auto_ptr<SipStackAgentMsg> SipStackAgentClientImpl::SyncSendMsgEx(const string & strBody, const string & strUri, const string& strIPv4, unsigned short usPort,const string& strCallID)
{
    INFO_LOG("client syn-request message - begin");

    std::string uri_user_info_str = "";
    std::string uri_hostport_str = "";

    if ( SS_SUCCESS != CRequestToPu::ParseUri( strUri, uri_user_info_str, uri_hostport_str ) )
    {
        ERROR_LOG("client syn-request message - failure to parse uri.");
        throw SipStackException("client syn-request invite - failure to parse uri.");
    }

    CRequestMessageToPu* req_p = new CRequestMessageToPu(strIPv4, usPort, uri_user_info_str, uri_hostport_str, strBody,strCallID);
    if ( !req_p->IsValid() )
    {
        delete req_p;
        req_p = NULL;
        ERROR_LOG("client syn-request message - invalid request data.");
        throw SipStackException("client syn-request invite - invalid request data.");
    }
	/*lint -save -e1036*/
    SipStackSyncAgentMsgHandlerImpl aSyncHdlr(*this, strCallID);
	/*lint -restore*/
    aSyncHdlr.Acquire();

    if ( !CSipStackTask::Instance().RequestToPu(req_p) ) //投入消息队列，若成功则由使用方释放。
    {
        delete req_p;
        req_p = NULL;
        aSyncHdlr.Release();
        ERROR_LOG("client syn-request message - failure to request to pu.");
        throw SipStackException("client syn-request message - failure to request to pu.");
    }

    aSyncHdlr.WaitMsg();
    aSyncHdlr.Release();

    INFO_LOG("client syn-request message - end.");

    return aSyncHdlr.GetMsg();
}

string SipStackAgentClientImpl::SyncSendMsgExList(const string & strReqData)
{
    return "";
}

string SipStackAgentClientImpl::SyncSendMsgExList(const string & strReqData, const string & strReqUri)
{
    return "";
}

bool SipStackAgentClientImpl::SyncSendMsgNoResp(const string & strReqData)
{
    return false;
}

bool SipStackAgentClientImpl::SyncSendMsgNoResp(const string & strReqData, const string & strReqUri)
{ 
    return false;
}

void SipStackAgentClientImpl::HandleRcvMsg ( auto_ptr<SipStackAgentMsg> apMsg )
{
    INFO_LOG("SipAgentClientImplMiniSip::handRcvMsg, Msg_Type=%d, Dev_ID=%s",apMsg->GetType(),apMsg->GetFrom().c_str());

    if ( NULL == apMsg.get() )
    {
        ERROR_LOG("SipAgentClientImplMiniSip::handRcvMsg... msg point is null");
        return;
    } 

    SipMessageHandlerMap& handler_map           = GetSipMessageHanlderMap();
    SipMessageHandlerMap::iterator handler_iter = handler_map.end();

    ACE_Read_Guard<ACE_RW_Mutex> guard(m_sip_msg_handler_map_rw_mutex);
    do
    {
        //1.sync msg no resp
        ostringstream oss;
        oss<< "cseq_"<< apMsg->GetSequence();
        handler_iter= handler_map.find(oss.str());
        if (handler_iter != handler_map.end())
        {
            INFO_LOG("SipAgentClientImplMiniSip::handRcvMsg...find handler by Msg_Sequence=%s",oss.str());
            break;
        }

        //2.sync xml
        handler_iter = handler_map.find(apMsg->GetBodySequence());
        if (handler_iter != handler_map.end())
        {
            INFO_LOG("SipAgentClientImplMiniSip::handRcvMsg...find handler by Data_Sequence=%s",apMsg->GetBodySequence().c_str());
            break;
        }

		//3.register
		_SIP_STACK_MSG_TYPE emType = apMsg->GetType();
		if( SIP_STACK_TYPE_UNREGISTER == emType || SIP_STACK_TYPE_REGISTER == emType || SIP_STACK_TYPE_INVITE == emType || SIP_STACK_TYPE_MESSAGE == emType)
		{
			INFO_LOG("SipAgentClientImplMiniSip::handRcvMsg...find handler by CallID=%s",apMsg->GetCallID().c_str());
			handler_iter = handler_map.find(apMsg->GetCallID());
			if (handler_iter != handler_map.end())
			{
				INFO_LOG("SipAgentClientImplMiniSip::handRcvMsg...find handler by CallID=%s",apMsg->GetCallID().c_str());
				break;
			}
		}

        //4.设备
        handler_iter= handler_map.find(apMsg->GetFrom());
        if (handler_iter != handler_map.end())
        {
            INFO_LOG("SipAgentClientImplMiniSip::handRcvMsg...find handler by Devid=%s",apMsg->GetFrom().c_str());
            break;
        }

        INFO_LOG("SipAgentClientImplMiniSip::handRcvMsg...default...find default hander fail, unknown msg");
        return ;
    }
    while (false);

    SipStackAgentMsgHandler* handler_p = (handler_iter->second);
    if ( NULL == handler_p)
    {
        WARN_LOG("client handle receive message - handler is null");
        return ;
    }

    INFO_LOG("SipAgentClientImplMiniSip::handRcvMsg...Handler_Tag=%s",handler_iter->first.c_str());
    handler_p->handRcvMsg(apMsg);

    return;
}//lint !e1788

bool SipStackAgentClientImpl::ParseUri ( const string& uri, string& uri_name, string& uri_domain )
{
    if ( uri.empty() )
    {
        return false;
    }

    string::size_type posAt = uri.find('@');
    if ( string::npos == posAt )
    {
        return false;
    }

    uri_name = uri.substr ( 0, posAt );
    uri_domain = uri.substr ( posAt + 1 );
    return true;
}

SipMessageHandlerMap& SipStackAgentClientImpl::GetSipMessageHanlderMap()
{
    return m_sip_msg_handler_map;
}

std::string SipStackAgentClientImpl::ParseMsgDataSeq(const string &msgBody)
{
    std::string targetDataSequence = SIP_STRING_NULL;

    TiXmlDocument doc;
    doc.Parse(msgBody.c_str(),0,TIXML_ENCODING_LEGACY);

    if (doc.Error())
    {
        return SIP_STRING_NULL;
    }

    TiXmlElement *root = NULL;
    try
    {
        root = doc.RootElement();
        if (NULL == root)
        {
            return SIP_STRING_NULL;
        }
    }
    catch (...)
    {
        return SIP_STRING_NULL;
    }

    // CV_HEADER
    TiXmlElement *pList = root->FirstChildElement("CV_HEADER");
    if (NULL == pList)
    {
        return SIP_STRING_NULL;
    }

    const char *pSeq = pList->Attribute("MsgSeq");
    if (NULL == pSeq)
    {
        TiXmlElement *pSeqElement = pList->FirstChildElement("MSG_SEQ");
        if (NULL != pSeqElement)
        {   
            pSeq = pSeqElement->GetText();
        }
    }
	if(NULL == pSeq)
	{
		return SIP_STRING_NULL;
	}

    targetDataSequence = pSeq;

    return targetDataSequence.c_str();
}

/**********************************************************************************/

bool SipStackAgentClientImpl::SendRegisterResp ( unsigned long resp_id_n, unsigned long resp_code_n, const string& strIPv4, unsigned short usPort )
{
    INFO_LOG("client respond register - begin");

    CRepondRegisterToPu* resp_p = new CRepondRegisterToPu(resp_id_n, resp_code_n, strIPv4, usPort);
    if ( !resp_p->IsValid() )
    {
        delete resp_p;
        resp_p = NULL;
        ERROR_LOG("client respond register - invalid respond data");
        return false;
    }

    if ( !CSipStackTask::Instance().RespondToPu(resp_p) ) //投入消息队列，若成功则由使用方释放。
    {
        delete resp_p;
        resp_p = NULL;
        ERROR_LOG("client respond register - failure to respond to pu");
        return false;
    }

    INFO_LOG("client respond register - end");

    return true;
}

bool SipStackAgentClientImpl::SendCommonMsgResp( unsigned long resp_id_n, unsigned long resp_code_n, const string& strIPv4, unsigned short usPort )
{
    INFO_LOG("client respond message - begin");

    CRepondRegisterToPu* resp_p = new CRepondRegisterToPu(resp_id_n, resp_code_n, strIPv4, usPort);
    if ( !resp_p->IsValid() )
    {
        delete resp_p;
        resp_p = NULL;
        ERROR_LOG("client respond message - invalid respond data");
        return false;
    }

    if ( !CSipStackTask::Instance().RespondToPu(resp_p) ) //投入消息队列，若成功则由使用方释放。
    {
        delete resp_p;
        resp_p = NULL;
        ERROR_LOG("client respond message - failure to respond to pu");
        return false;
    }

    INFO_LOG("client respond message - end");

    return true;
}

bool SipStackAgentClientImpl::SendOptionMsgResp ( unsigned long resp_id_n, unsigned long resp_code_n, const string& strIPv4, unsigned short usPort )
{
    INFO_LOG("client respond options - begin");

    CRepondOptionsToPu* resp_p = new CRepondOptionsToPu(resp_id_n, resp_code_n, strIPv4, usPort);
    if ( !resp_p->IsValid() )
    {
        delete resp_p;
        resp_p = NULL;
        ERROR_LOG("client respond options - invalid respond data");
        return false;
    }

    if ( !CSipStackTask::Instance().RespondToPu(resp_p) ) //投入消息队列，若成功则由使用方释放。
    {
        delete resp_p;
        resp_p = NULL;
        ERROR_LOG("client respond options - failure to respond to pu");
        return false;
    }

    INFO_LOG("client respond options - end");

    return true;
}

bool SipStackAgentClientImpl::SendInviteReq ( const string& strReqData, const string& strReqUri, const string& strCallID,
                                                const string& strSubject, const string& strIP, unsigned short usPort
                                              )
{
    return false;
}

bool SipStackAgentClientImpl::SendInviteAck ( unsigned long ulSeq, const string& strIP, unsigned short usPort )
{
    return false;
}

bool SipStackAgentClientImpl::SendByeReq ( unsigned long resp_id_n, const std::string& strIPv4, unsigned short usPort )
{
    INFO_LOG("client asyn-request bye - begin");

    CRequestByeToPu* req_p = new CRequestByeToPu(strIPv4, usPort, resp_id_n);
    if ( !req_p->IsValid() )
    {
        delete req_p;
        req_p = NULL;
        ERROR_LOG("client asyn-request bye - invalid request data");
        return false;
    }

    if ( !CSipStackTask::Instance().RequestToPu(req_p) ) //投入消息队列，若成功则由使用方释放。
    {
        delete req_p;
        req_p = NULL;
        ERROR_LOG("client asyn-request bye - failure to request to pu");
        return false;
    }

    INFO_LOG("client asyn-request bye - end");

    return true;
}

bool SipStackAgentClientImpl::SendCancelReq ( unsigned long ulSeq, const string& strIP, unsigned short usPort )
{
    return false;
}

string SipStackAgentClientImpl::SendSubscribeReq ( const string & strBody, const string & strUri, const std::string& strIPv4, unsigned short usPort ) 
{
    INFO_LOG("client asyn-request subscribe - begin");

    std::string uri_user_info_str = "";
    std::string uri_hostport_str = "";

    if ( SS_SUCCESS != CRequestToPu::ParseUri( strUri, uri_user_info_str, uri_hostport_str ) )
    {
        ERROR_LOG("client asyn-request subscribe - failure to parse uri.");
        return "";
    }

    CRequestSubScribeToPu* req_p = new CRequestSubScribeToPu(strIPv4, usPort, uri_user_info_str, uri_hostport_str, strBody);
    if ( !req_p->IsValid() )
    {
        delete req_p;
        req_p = NULL;
        ERROR_LOG("client asyn-request subscribe - invalid request data");
        return "";
    }

    if ( !CSipStackTask::Instance().RequestToPu(req_p) ) //投入消息队列，若成功则由使用方释放。
    {
        delete req_p;
        req_p = NULL;
        ERROR_LOG("client asyn-request subscribe - failure to request to pu");
        return "";
    }

    INFO_LOG("client asyn-request subscribe - end");

    return "1";
}

bool SipStackAgentClientImpl::SendRegister(const string& strReqUri,const string& strServer,unsigned short usPort,const string& strCallID,bool bIsRegister)
{
	INFO_LOG("client asyn-request register - begin");

	std::string uri_user_info_str = "";
	std::string uri_hostport_str = "";

	if ( SS_SUCCESS != CRequestToPu::ParseUri( strReqUri, uri_user_info_str, uri_hostport_str ) )
	{
		ERROR_LOG("client asyn-request register - failure to parse uri.");
		return false;
	}

	CRequestToPu* req_p = NULL;
	if(bIsRegister)
	{
		req_p = new CRequestRegisterToPu(strServer, usPort, uri_user_info_str, uri_hostport_str,strCallID);
	}
	else
	{
		req_p = new CRequestUnRegisterToPu(strServer, usPort, uri_user_info_str, uri_hostport_str,strCallID);
	}
	
	if ( !req_p->IsValid() )
	{
		delete req_p;
		req_p = NULL;
		ERROR_LOG("client asyn-request register - invalid request data");
		return false;
	}

	if(bIsRegister)//注册的消息需要同步返回
	{
		/*lint -save -e1036*/
		SipStackSyncAgentMsgHandlerImpl aSyncHdlr(*this, strCallID);
		/*lint -restore*/
		aSyncHdlr.Acquire();

		if ( !CSipStackTask::Instance().RequestToPu(req_p) ) //投入消息队列，若成功则由使用方释放。
		{
			delete req_p;
			req_p = NULL;
			aSyncHdlr.Release();
			ERROR_LOG("client asyn-request register - failure to request to pu");
			return false;
		}

		aSyncHdlr.WaitMsg();
		aSyncHdlr.Release();

		INFO_LOG("client asyn-request register - end");
		auto_ptr<SipStackAgentMsg>  pMsg = aSyncHdlr.GetMsg();
		if(NULL == pMsg.get())
		{
			ERROR_LOG("response msg is null.");
			return false;
		}
		const unsigned long ulStatusCode = pMsg->GetStatusCode();
		if(SIP_STATUS_SUCC_OK == ulStatusCode)
		{
			return true;
		}
		else
		{
			ERROR_LOG("response code is %d",ulStatusCode);
			return false;
		}
	}
	else
	{
		if ( !CSipStackTask::Instance().RequestToPu(req_p) ) //投入消息队列，若成功则由使用方释放。
		{
			delete req_p;
			req_p = NULL;
			ERROR_LOG("client asyn-request unregister - failure to request to pu");
			return false;
		}

		INFO_LOG("client asyn-request unregister - end");
		return true;
	}

	 
}

bool SipStackAgentClientImpl::SendOption(const string& strReqUri)
{
    return false;
}

void SipStackAgentClientImpl::SendInviteResp(const string& strReqData, unsigned long ulResponseID, unsigned long ulStatusCode)
{
}

void SipStackAgentClientImpl::SendResp (const unsigned long ulResponseID)
{
}

bool SipStackAgentClientImpl::AsynSendMsgEx ( const string& strBody, const string& strUri, const string& strIPv4, unsigned short usPort)
{
    INFO_LOG("client asyn-request message - begin");

    std::string uri_user_info_str = "";
    std::string uri_hostport_str = "";

    if ( SS_SUCCESS != CRequestToPu::ParseUri( strUri, uri_user_info_str, uri_hostport_str ) )
    {
        ERROR_LOG("client asyn-request message - failure to parse uri.");
        return false;
    }

    CRequestMessageToPu* req_p = new CRequestMessageToPu(strIPv4, usPort, uri_user_info_str, uri_hostport_str, strBody,"");
    if ( !req_p->IsValid() )
    {
        delete req_p;
        req_p = NULL;
        ERROR_LOG("client asyn-request message - invalid request data");
        return false;
    }

    if ( !CSipStackTask::Instance().RequestToPu(req_p) ) //投入消息队列，若成功则由使用方释放。
    {
        delete req_p;
        req_p = NULL;
        ERROR_LOG("client asyn-request message - failure to request to pu");
        return false;
    }

    INFO_LOG("client asyn-request message - end");

    return true;
}

}
//lint +e429
//lint +e438
