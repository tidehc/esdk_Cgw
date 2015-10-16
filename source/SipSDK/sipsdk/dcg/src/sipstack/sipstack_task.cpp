#include <time.h>
#include "tinyxml.h"
#include "sipstack_config.hpp"
#include "sipstack_agent_mgr_impl.hpp"
#include "sipstack_msg_ctrl.h"
#include "sipstack_task.h"
#include "Log.h"

namespace SipStack{

#define TASK_TOO_MANY_MESSIONS_COUT_DFLT (500)

#define CLEAN_ONE_POST_TO_APP_MESSAGE_ST(one_st_p)                                     \
{                                                                                      \
    if ( NULL != one_st_p )                                                            \
    {                                                                                  \
        if ( NULL != one_st_p->message_string_st.pcData )                              \
        {                                                                              \
            char* data_buf_p = static_cast<char*>(one_st_p->message_string_st.pcData); \
            delete [] data_buf_p;                                                      \
            data_buf_p = NULL;                                                         \
            one_st_p->message_string_st.pcData = NULL;                                 \
        }                                                                              \
        delete one_st_p;                                                               \
        one_st_p = NULL;                                                               \
    }                                                                                  \
}

#define SVC_LOCK_IN_LOOP(locker, ace_time_value, action)                               \
if ( SS_ZERO == locker.locked() )                                                      \
{                                                                                      \
    ERROR_LOG("finish - failure to lock respond queue");                               \
    ACE_SLEEP(ace_time_value);                                                         \
    action;                                                                            \
}

CPostToPu::CPostToPu ( const std::string&   ipv4_str,
                       const unsigned short port_us
                     )
          :m_strIPv4(ipv4_str.c_str())
          ,m_usPort(port_us)
{
}
CPostToPu::~CPostToPu ()
{
}
const std::string& CPostToPu::GetIPv4 () const
{
    return m_strIPv4;
}
unsigned short     CPostToPu::GetPort () const
{
    return m_usPort;
}
bool CPostToPu::IsValid ()
{
    if ( m_strIPv4.empty() )
    {
        ERROR_LOG("post to pu - ipv4 is null.");
        return false;
    }

    if ( SS_MAX_PORT_NUM <= m_usPort )
    {
        ERROR_LOG("post to pu - port is invalid - port= %d",m_usPort);
        return false;
    }

    return true;
}
int CPostToPu::Execute()
{
    ERROR_LOG("post to pu - failure to execute.");
    return SS_FAILURE;
}

CRequestToPu::CRequestToPu ( const std::string& strIPv4, const unsigned short usPort,
                             const std::string& strUriUserInfo, const std::string& strUriHostport
                           )
             :CPostToPu(strIPv4, usPort)
             ,m_bSync(false)
             ,m_strUriUserInfo(strUriUserInfo.c_str())
			 ,m_strUriHostport(strUriHostport.c_str())
             ,m_strAgentName(SS_AGENT_NAME_DFLT)
{
}
CRequestToPu::~CRequestToPu ()
{
}
const std::string& CRequestToPu::GetUriUserInfo () const
{
    return m_strUriUserInfo;
}
const std::string& CRequestToPu::GetUriHostport () const
{
    return m_strUriHostport;
}
const std::string& CRequestToPu::GetAgentName () const
{
    return m_strAgentName;
}
void CRequestToPu::SetSync ( bool bSync )
{
    m_bSync = bSync;
}
bool CRequestToPu::IsSync () const
{
    return m_bSync;
}
bool CRequestToPu::IsValid ()
{
    if ( m_strUriUserInfo.empty() || m_strUriHostport.empty() )
    {
        ERROR_LOG("request to pu - uri is invalid.");
        return false;
    }

    if ( m_strAgentName.empty() )
    {
        ERROR_LOG("request to pu - agent name is invalid.");
        return false;
    }

    return CPostToPu::IsValid();
}
int CRequestToPu::ParseUri ( const std::string& strUri,
                             std::string& strUriUserInfo, std::string& strUriHostport
                           )
{
    if ( strUri.empty() )
    {
        ERROR_LOG("request to pu - failure to parse uri - invalid param.");
        return SS_FAILURE;
    }

    std::string::size_type nAtPos = string::npos;
    if ( std::string::npos == ( nAtPos = strUri.find('@') ) )//lint !e838
    {
        ERROR_LOG("request to pu - failure to parse uri - can not find '@'.");
        return SS_FAILURE;
    }

    strUriUserInfo = strUri.substr ( SS_ZERO, nAtPos );
    strUriHostport = strUri.substr ( nAtPos + 1 );

    return SS_SUCCESS;
}

CRequestInviteToPu::CRequestInviteToPu( const std::string& strIPv4, const unsigned short usPort,
                                        const std::string& strUriUserInfo, const std::string& strUriHostport,
                                        const std::string& strCallID, const std::string& strSubject, const std::string& strBody
                                      )
                   :CRequestToPu(strIPv4, usPort, strUriUserInfo, strUriHostport)
                   ,m_strCallID(strCallID.c_str())
                   ,m_strSubject(strSubject.c_str())
                   ,m_strBody(strBody.c_str())
{
}
CRequestInviteToPu::~CRequestInviteToPu()
{
}
const std::string& CRequestInviteToPu::GetCallID()  const
{
    return m_strCallID;
}
const std::string& CRequestInviteToPu::GetSubject() const
{
    return m_strSubject;
}
const std::string& CRequestInviteToPu::GetBody()    const
{
    return m_strBody;
}
bool CRequestInviteToPu::IsValid ()
{
    if ( m_strCallID.empty() )
    {
        ERROR_LOG("request invite to pu - call-id is emtpy.");
        return false;
    }

	//去除subject字段，以防服务器校验失败
    //if ( m_strSubject.empty() )
    //{
    //    ERROR_LOG("request invite to pu - subject is emtpy.");
    //    return false;
    //}

    if ( m_strBody.empty() )
    {
        ERROR_LOG("request invite to pu - body is emtpy.");
        return false;
    }

    return CRequestToPu::IsValid();
}
int CRequestInviteToPu::Execute()
{
    if ( !IsValid() )
    {
        ERROR_LOG("request invite to pu - invalid param.");
        return SS_FAILURE;
    }
	INFO_LOG("request invite to pu - begin to execute - Uri=%s:%s,Call_ID=%s,Subject=%s,AgentName=%s,Body=%s.",
		GetUriUserInfo().c_str(), GetUriHostport().c_str(),
		GetCallID().c_str(), GetSubject().c_str(), GetAgentName().c_str(),
		GetBody().c_str());

    CSipVar aSipVar;
    aSipVar.m_strRequestUriUserInfo   = GetUriUserInfo();
    aSipVar.m_strRequestUriDomain     = GetUriHostport();
    aSipVar.m_strCallID               = GetCallID();
    aSipVar.m_strSubject              = GetSubject();
    aSipVar.m_strAgentName            = GetAgentName();

    SipStackAgentServerImpl* sipstack_agent_server_p = SipStackAgentMgrImpl::Instance()->GetAgentServer();
    sipstack_agent_server_p->GetDcgStack().sendInvite(GetBody(), aSipVar);

    INFO_LOG("post to pu - end.");

    return SS_SUCCESS;
}

CRequestAckToPu::CRequestAckToPu( const std::string& strIPv4, const unsigned short usPort,
                                  const unsigned long nRespID
                                )
                :CRequestToPu(strIPv4, usPort, "", "")
                ,m_nRespID(nRespID)
{
}
CRequestAckToPu::~CRequestAckToPu()
{
}
unsigned int CRequestAckToPu::GetRespID () const
{
    return m_nRespID;
}
bool CRequestAckToPu::IsValid ()
{
    if ( GetAgentName().empty() )
    {
        ERROR_LOG("request ack to pu - agent name is invalid.");
        return false;
    }

    if ( GetIPv4().empty() )
    {
        ERROR_LOG("request ack to pu - ipv4 is null.");
        return false;
    }

    if ( SS_MAX_PORT_NUM <= GetPort() )
    {
        ERROR_LOG("request ack to pu - port is invalid - port= %d",GetPort());
        return false;
    }

    return true;
}
int CRequestAckToPu::Execute()
{
    if ( !IsValid() )
    {
        ERROR_LOG("request ack to pu - invalid param.");
        return SS_FAILURE;
    }

	INFO_LOG("request ack to pu - begin to execute - Repond_ID=%d,IPv4=%s,Port=%d.",
		GetRespID(), GetIPv4().c_str(), GetPort());
    SipStackAgentServerImpl* sipstack_agent_server_p = SipStackAgentMgrImpl::Instance()->GetAgentServer();
    sipstack_agent_server_p->GetDcgStack().sendInviteAck(GetRespID());

    INFO_LOG("request ack to pu - end.");

    return SS_SUCCESS;
}

CRequestByeToPu::CRequestByeToPu( const std::string& strIPv4, const unsigned short usPort,
                                  const unsigned long nRespID
                                )
                :CRequestToPu(strIPv4, usPort, "", "")
                ,m_nRespID(nRespID)
{
}
CRequestByeToPu::~CRequestByeToPu()
{
}
unsigned int CRequestByeToPu::GetRespID () const
{
    return m_nRespID;
}
bool CRequestByeToPu::IsValid ()
{
    if ( GetAgentName().empty() )
    {
        ERROR_LOG("request bye to pu - agent name is invalid.");
        return false;
    }

    if ( GetIPv4().empty() )
    {
        ERROR_LOG("request bye to pu - ipv4 is null.");
        return false;
    }

    if ( SS_MAX_PORT_NUM <= GetPort() )
    {
        ERROR_LOG("request bye to pu - port is invalid - port= %d",GetPort());
        return false;
    }

    return true;
}
int CRequestByeToPu::Execute()
{
    if ( !IsValid() )
    {
        ERROR_LOG("request bye to pu - invalid param.");
        return SS_FAILURE;
    }

	INFO_LOG("request bye to pu - begin to execute - Repond_ID=%d,IPv4=%s,Port=%d.",
		GetRespID(), GetIPv4().c_str(), GetPort());
	SipStackAgentServerImpl* sipstack_agent_server_p = SipStackAgentMgrImpl::Instance()->GetAgentServer();
    sipstack_agent_server_p->GetDcgStack().sendByeReq(GetRespID());

    INFO_LOG("request bye to pu - end.");

    return SS_SUCCESS;
}

CRequestMessageToPu::CRequestMessageToPu( const std::string& strIPv4, const unsigned short usPort,
                                          const std::string& strUriUserInfo, const std::string& strUriHostport,
                                          const std::string& strBody,const std::string& strCallID
                                         )
                    :CRequestToPu(strIPv4, usPort, strUriUserInfo, strUriHostport)
                    ,m_strBody(strBody.c_str()),m_strCallID(strCallID.c_str())
{
}
CRequestMessageToPu::~CRequestMessageToPu()
{
}
const std::string& CRequestMessageToPu::GetBody () const
{
    return m_strBody;
}
bool CRequestMessageToPu::IsValid ()
{
    if ( m_strBody.empty() )
    {
        ERROR_LOG("request message to pu - body is empty.");
        return false;
    }

    return CRequestToPu::IsValid();
}
int CRequestMessageToPu::Execute()
{
    if ( !IsValid() )
    {
        ERROR_LOG("request message to pu - invalid param.");
        return SS_FAILURE;
    }

	INFO_LOG("request message to pu - begin to execute - Uri=%s:%s - Body=%s.",
		GetUriUserInfo().c_str(), GetUriHostport().c_str(),
		GetBody().c_str());


    CSipVar aSipVar;
    aSipVar.m_strRequestUriUserInfo   = GetUriUserInfo();
    aSipVar.m_strRequestUriDomain     = GetUriHostport();
    aSipVar.m_strAgentName            = "HUAWEI eSDK";
	if(!m_strCallID.empty())
	{
		aSipVar.m_strCallID           = m_strCallID;
	}
	

    SipStackAgentServerImpl* sipstack_agent_server_p = SipStackAgentMgrImpl::Instance()->GetAgentServer();
    if ( !sipstack_agent_server_p->GetDcgStack().sendMsg(GetBody(), aSipVar) )
    {
        ERROR_LOG("request message to pu - failure to send msg.");
        return SS_FAILURE;
    }

    INFO_LOG("request message to pu - end.");

    return SS_SUCCESS;
}

CRequestSubScribeToPu::CRequestSubScribeToPu( const std::string& strIPv4, const unsigned short usPort,
                                              const std::string& strUriUserInfo, const std::string& strUriHostport,
                                              const std::string& strBody
                                            )
                      :CRequestToPu(strIPv4, usPort, strUriUserInfo, strUriHostport)
                      ,m_strBody(strBody.c_str())
{
}
CRequestSubScribeToPu::~CRequestSubScribeToPu()
{
}
const std::string CRequestSubScribeToPu::GetBody() const
{
    return m_strBody.c_str();
}
bool CRequestSubScribeToPu::IsValid ()
{
    if ( m_strBody.empty() )
    {
        ERROR_LOG("request subscribe to pu - body is empty.");
        return false;
    }

    return CRequestToPu::IsValid();
}
int CRequestSubScribeToPu::Execute()
{
    if ( !IsValid() )
    {
        ERROR_LOG("request subscribe to pu - invalid param.");
        return SS_FAILURE;
    }
	INFO_LOG("request subscribe to pu - begin to execute - Uri=%s:%s - Body=%s.",
		GetUriUserInfo().c_str(), GetUriHostport().c_str(),
		GetBody().c_str());
    
    CSipVar aSipVar;
    aSipVar.m_strRequestUriUserInfo   = GetUriUserInfo();
    aSipVar.m_strRequestUriDomain     = GetUriHostport();
    aSipVar.m_strAgentName            = "HUAWEI eSDK";

    SipStackAgentServerImpl* sipstack_agent_server_p = SipStackAgentMgrImpl::Instance()->GetAgentServer();
    if ( !sipstack_agent_server_p->GetDcgStack().sendSubscribeReq(GetBody(), aSipVar) )
    {
        ERROR_LOG("request subscribe to pu - failure to send msg.");
        return SS_FAILURE;
    }

    INFO_LOG("request subscribe to pu - end.");

    return SS_SUCCESS;
}

/*start register*/
CRequestRegisterToPu::CRequestRegisterToPu( const std::string& strIPv4, const unsigned short usPort,
	const std::string& strUriUserInfo, const std::string& strUriHostport,const std::string& strCallID)
	:CRequestToPu(strIPv4, usPort, strUriUserInfo, strUriHostport),m_strCallID(strCallID.c_str())
{
}
CRequestRegisterToPu::~CRequestRegisterToPu()
{
}
bool CRequestRegisterToPu::IsValid ()
{
	return CRequestToPu::IsValid();
}
int CRequestRegisterToPu::Execute()
{
	if ( !IsValid() )
	{
		ERROR_LOG("request register to pu - invalid param.");
		return SS_FAILURE;
	}

	INFO_LOG("request register to pu - begin to execute - Uri=%s:%s.",
		GetUriUserInfo().c_str(), GetUriHostport().c_str());

	CSipVar aSipVar;
	aSipVar.m_strRequestUriUserInfo   = GetUriUserInfo();
	aSipVar.m_strRequestUriDomain     = GetUriHostport();
	aSipVar.m_strAgentName            = "HUAWEI eSDK";
	aSipVar.m_strCallID               = GetCallID();

	SipStackAgentServerImpl* sipstack_agent_server_p = SipStackAgentMgrImpl::Instance()->GetAgentServer();
	if ( !sipstack_agent_server_p->GetDcgStack().sendRegister(aSipVar) )
	{
		ERROR_LOG("request register to pu - failure to send msg.");
		return SS_FAILURE;
	}

	INFO_LOG("request register to pu - end.");

	return SS_SUCCESS;
}
const std::string& CRequestRegisterToPu::GetCallID()  const
{
	return m_strCallID;
}
/*end register*/
/*start unregister*/
CRequestUnRegisterToPu::CRequestUnRegisterToPu( const std::string& strIPv4, const unsigned short usPort,
	const std::string& strUriUserInfo, const std::string& strUriHostport,const std::string& strCallID)
	:CRequestToPu(strIPv4, usPort, strUriUserInfo, strUriHostport),m_strCallID(strCallID.c_str())
{
}
CRequestUnRegisterToPu::~CRequestUnRegisterToPu()
{
}
bool CRequestUnRegisterToPu::IsValid ()
{
	return CRequestToPu::IsValid();
}
int CRequestUnRegisterToPu::Execute()
{
	if ( !IsValid() )
	{
		ERROR_LOG("request register to pu - invalid param.");
		return SS_FAILURE;
	}

	INFO_LOG("request register to pu - begin to execute - Uri=%s@%s.",GetUriUserInfo().c_str() ,GetUriHostport().c_str());

	CSipVar aSipVar;
	aSipVar.m_strRequestUriUserInfo   = GetUriUserInfo();
	aSipVar.m_strRequestUriDomain     = GetUriHostport();
	aSipVar.m_strAgentName            = "HUAWEI eSDK";
	aSipVar.m_strCallID               = GetCallID();

	SipStackAgentServerImpl* sipstack_agent_server_p = SipStackAgentMgrImpl::Instance()->GetAgentServer();
	if ( !sipstack_agent_server_p->GetDcgStack().sendUnRegister(aSipVar) )
	{
		ERROR_LOG("request register to pu - failure to send msg.");
		return SS_FAILURE;
	}

	INFO_LOG("request register to pu - end.");

	return SS_SUCCESS;
}
const std::string& CRequestUnRegisterToPu::GetCallID()  const
{
	return m_strCallID;
}

/*end unregister*/
CRespondToPu::CRespondToPu ( const unsigned long  resp_id_ui,
                             const unsigned int   resp_code_ui,
                             const std::string&   ipv4_str,
                             const unsigned short port_us
                           )
                :CPostToPu(ipv4_str, port_us)
                ,m_uiRespID(resp_id_ui)
                ,m_uiRespCode(resp_code_ui)
				,m_usPort(port_us)
{
}

CRespondToPu::~CRespondToPu ()
{
}

unsigned long CRespondToPu::GetRespID () const
{
    return m_uiRespID;
}

unsigned int CRespondToPu::GetRespCore () const
{
    return m_uiRespCode;
}

CRepondRegisterToPu::CRepondRegisterToPu ( const unsigned long  resp_id_ui,
                                           const unsigned int   resp_code_ui,
                                           const std::string&   ipv4_str,
                                           const unsigned short port_us
                                         )
                    : CRespondToPu ( resp_id_ui, resp_code_ui, ipv4_str, port_us )
{

}

CRepondRegisterToPu::~CRepondRegisterToPu()
{
}

int CRepondRegisterToPu::Execute()
{
    DEBUG_LOG("respond to pu - send register respond - ipv4= %s",GetIPv4().c_str());
    SipStackAgentServerImpl* sipstack_agent_server_p = SipStackAgentMgrImpl::Instance()->GetAgentServer();

    try
    {
        if ( !sipstack_agent_server_p->GetDcgStack().sendRegisterResp(GetRespID(), GetRespCore()) )
        {
            ERROR_LOG("respoind to pu - failure to send register respond.");
            return SS_FAILURE;
        }
    }
    catch (...)
    {
        ERROR_LOG("respoind to pu - send register respond throw exception");
        return SS_FAILURE;
    }

    return SS_SUCCESS;
}

CRepondMessageToPu::CRepondMessageToPu ( const unsigned long  resp_id_ui,
                                         const unsigned int   resp_code_ui,
                                         const std::string&   ipv4_str,
                                         const unsigned short port_us
                                       )
                   : CRespondToPu ( resp_id_ui, resp_code_ui, ipv4_str, port_us )
{

}
CRepondMessageToPu::~CRepondMessageToPu()
{
}
int CRepondMessageToPu::Execute()
{
    DEBUG_LOG("respond to pu - send message respond - ipv4 = %s",GetIPv4().c_str());
    SipStackAgentServerImpl* sipstack_agent_server_p = SipStackAgentMgrImpl::Instance()->GetAgentServer();
    try
    {
        if ( !sipstack_agent_server_p->GetDcgStack().sendCommonMsgResp(GetRespID(), GetRespCore()) )
        {
            ERROR_LOG("respond to pu - failure to send message respond.");
            return SS_FAILURE;
        }
    }
    catch (...)
    {
        ERROR_LOG("respond to pu - send message respond throw exception");
        return SS_FAILURE;
    }

    return SS_SUCCESS;
}

CRepondOptionsToPu::CRepondOptionsToPu ( const unsigned long  resp_id_ui,
                                         const unsigned int   resp_code_ui,
                                         const std::string&   ipv4_str,
                                         const unsigned short port_us
                                       )
                   : CRespondToPu ( resp_id_ui, resp_code_ui, ipv4_str, port_us )
{

}
CRepondOptionsToPu::~CRepondOptionsToPu()
{
}
int CRepondOptionsToPu::Execute()
{
    DEBUG_LOG("respond to pu - send options respond - ipv4= %s",GetIPv4().c_str());
    SipStackAgentServerImpl* sipstack_agent_server_p = SipStackAgentMgrImpl::Instance()->GetAgentServer();
    try
    {
        if ( !sipstack_agent_server_p->GetDcgStack().sendOptionMsgResp(GetRespID(), GetRespCore()) )
        {
            ERROR_LOG("respoind to pu - failure to send options respond.");
            return SS_FAILURE;
        }
    }
    catch (...)
    {
        ERROR_LOG("respoind to pu - send options respond throw exception");
        return SS_FAILURE;
    }

    return SS_SUCCESS;
}

bool g_work_b        = true;
bool g_finish_work_b = false;

CSipStackTask CSipStackTask::m_instance;
ACE_Recursive_Thread_Mutex CSipStackTask::m_minisip_mutex;

CSipStackTask::CSipStackTask ()
                :m_init_b(false)
                ,m_thd_id(SS_ZERO)
                ,m_work_b(false)
                ,m_req_to_pu_q_read_ui(SS_ZERO)
                ,m_resp_to_pu_q_read_ui(SS_ZERO)
				,m_post_to_app_q_ui(SS_ZERO)
{
	m_post_to_app_q.clear();
}

CSipStackTask::~CSipStackTask ()
{
}
//lint -e1538
CSipStackTask::CSipStackTask(const CSipStackTask& )
	:m_init_b(false)
	,m_thd_id(SS_ZERO)
	,m_work_b(false)
	,m_req_to_pu_q_read_ui(SS_ZERO)
	,m_resp_to_pu_q_read_ui(SS_ZERO)
	,m_post_to_app_q_ui(SS_ZERO)
{
	m_post_to_app_q.clear();
}
//lint +e1538
CSipStackTask& CSipStackTask::operator=(CSipStackTask& )
{
    return *this;
}
CSipStackTask& CSipStackTask::Instance ()
{
    return m_instance;
}

ACE_Recursive_Thread_Mutex& CSipStackTask::MiniSipMutex()
{
    return m_minisip_mutex;
}

bool CSipStackTask::Init()
{
    if ( m_init_b )
    {
        DEBUG_LOG("worker init - success - init again.");
        return true;
    }

    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, obj, m_init_mutex, false);

    m_work_b        = true;
    //开启协议栈工作主线程
    if (ACE_SPAWN_FAIL == activate(THR_NEW_LWP  | THR_JOINABLE) )//lint !e835
    {
        ERROR_LOG("worker init - success - spawn thread.");
        return (m_work_b = false);
    }

    return (m_init_b = true);
}

void CSipStackTask::Fini()
{
    ACE_GUARD(ACE_Recursive_Thread_Mutex, obj, m_init_mutex);

    m_work_b = false;
    wait();

    const ACE_Time_Value locker_waiting_tv((long long)ACE_TIME_ONE_SEC, 0);//1秒，防止CPU过高
    int count_n = SS_ZERO;

    for ( int i =  SS_ZERO; i < SS_TWO; i ++ )
    {
        //清除发送给PU的请求数据
        do
        {
            ACE_Guard<ACE_Recursive_Thread_Mutex> tempobj(m_req_to_pu_q_mutex);
            SVC_LOCK_IN_LOOP(tempobj, locker_waiting_tv, continue);

            count_n = (int)m_req_to_pu_q[i].size();

            CRequestToPu* trash_p = NULL;
            for ( int j = SS_ZERO; j < count_n; j ++ )
            {
                trash_p = m_req_to_pu_q[i].front();
                m_req_to_pu_q[i].pop_front();
                if ( NULL != trash_p)
                {
                    delete trash_p;
                    trash_p = NULL;
                }
            }
        }
        while(false);

        //清除发送给PU的响应数据
        do
        {
            ACE_Guard<ACE_Recursive_Thread_Mutex> tempobj(m_resp_to_pu_q_mutex);
            SVC_LOCK_IN_LOOP(tempobj, locker_waiting_tv, continue);

            count_n = (int)m_resp_to_pu_q[i].size();

            CRespondToPu* trash_p = NULL;
            for ( int j = SS_ZERO; j < count_n; j ++ )
            {
                trash_p = m_resp_to_pu_q[i].front();
                m_resp_to_pu_q[i].pop_front();
                if ( NULL != trash_p)
                {
                    delete trash_p;
                    trash_p = NULL;
                }
            }
        }
        while(false);
    }

    do
    {
        ACE_Guard<ACE_Recursive_Thread_Mutex> tempobj(m_post_to_app_mutex);
        SVC_LOCK_IN_LOOP(tempobj, locker_waiting_tv, continue);

        count_n = (int)m_post_to_app_q.size();

        POST_TO_APP_ST* trash_p = NULL;
        for ( int j = SS_ZERO; j < count_n; j ++ )
        {
            trash_p = m_post_to_app_q.front();
            m_post_to_app_q.pop_front();
			//lint -e438
            CLEAN_ONE_POST_TO_APP_MESSAGE_ST(trash_p);
			//lint +e438
        }
    }while(false);

    m_init_b = false;
}

int CSipStackTask::svc()
{
    CRequestToPu* req_to_pu_msg_p  = NULL;
    CRespondToPu* resp_to_pu_msg_p = NULL;
    POST_TO_APP_ST* post_msg_st_p  = NULL;

    int  mession_ui                = SS_ZERO;

    const ACE_Time_Value locker_waiting_tv((long long)0, 100 * 1000);
    const ACE_Time_Value none_mession_sleeping_tv((long long)0, 100 * 1000);
    const ACE_Time_Value too_many_messions_sleeping_tv((long long)0, 50 * 1000);

    while(m_work_b)
    {
        mession_ui = SS_ZERO;

        //处理发送请求任务
        do
        {
            if ( m_req_to_pu_q[m_req_to_pu_q_read_ui].empty() ) //当前消息队列为空，则切换读写队列
            {
                ACE_Write_Guard<ACE_Recursive_Thread_Mutex> obj(m_req_to_pu_q_mutex);
                SVC_LOCK_IN_LOOP(obj, locker_waiting_tv, break);
                m_req_to_pu_q_read_ui = (SS_ZERO != m_req_to_pu_q_read_ui ? SS_ZERO : SS_ONE);
            }

            //BEGIN : 以下代码为单线执行，即不能加锁，也不应改成多线程去访问。
            if ( m_req_to_pu_q[m_req_to_pu_q_read_ui].empty() ) //当前消息对了若为空，则无工作任务。
            {
                break;
            }

            mession_ui++;    //工作任务+1

            req_to_pu_msg_p = m_req_to_pu_q[m_req_to_pu_q_read_ui].front();
            m_req_to_pu_q[m_req_to_pu_q_read_ui].pop_front();
            HandleRequestMession(req_to_pu_msg_p);

            if ( NULL != req_to_pu_msg_p )
            {
                delete req_to_pu_msg_p;
                req_to_pu_msg_p = NULL;
            }
            //END
        } while (false);

        //处理发送响应任务
        do 
        {
            if ( m_resp_to_pu_q[m_resp_to_pu_q_read_ui].empty() ) //当前消息队列为空，则切换读写队列
            {
                ACE_Guard<ACE_Recursive_Thread_Mutex> obj(m_resp_to_pu_q_mutex);
                SVC_LOCK_IN_LOOP(obj, locker_waiting_tv, break);
                m_resp_to_pu_q_read_ui = (SS_ZERO != m_resp_to_pu_q_read_ui ? SS_ZERO : SS_ONE);
            }

            if ( m_resp_to_pu_q[m_resp_to_pu_q_read_ui].empty() ) //当前消息对了若为空，则无工作任务。
            {
                break;
            }

            mession_ui++;    //工作任务+1

            //BEGIN : 以下代码为单线执行，即不能加锁，也不应改成多线程去访问。
            resp_to_pu_msg_p = m_resp_to_pu_q[m_resp_to_pu_q_read_ui].front();
            m_resp_to_pu_q[m_resp_to_pu_q_read_ui].pop_front();
            HandleRespondMession(resp_to_pu_msg_p);

            if ( NULL != resp_to_pu_msg_p )
            {
                delete resp_to_pu_msg_p;
                resp_to_pu_msg_p = NULL;
            }
            //END
        } while (false);

        //处理接收任务
        do
        {
            ACE_Guard<ACE_Recursive_Thread_Mutex> obj(m_post_to_app_mutex);
            SVC_LOCK_IN_LOOP(obj, locker_waiting_tv, break);

            if ( m_post_to_app_q.empty() )
            {
                break;
            }

            mession_ui++;
            post_msg_st_p = m_post_to_app_q.front();
            m_post_to_app_q.pop_front();
            m_post_to_app_q_ui--;

            SS_CTRL_MSG_MGR_INS_REF.UpdateCondition(m_post_to_app_q_ui);

            HandleRecvMession(post_msg_st_p);
			//lint -e438
            CLEAN_ONE_POST_TO_APP_MESSAGE_ST(post_msg_st_p);
			//lint +e438
        } while (false);

        if ( SS_ZERO == mession_ui )
        {
            //INFO_LOG("task - sleep because nothing to do.");
            ACE_SLEEP(none_mession_sleeping_tv);                                   //休息
        }
        else if ( TASK_TOO_MANY_MESSIONS_COUT_DFLT <= mession_ui )
        {
            INFO_LOG("task - just complete %d mission",mession_ui);
            mession_ui = SS_ZERO;
            ACE_SLEEP(too_many_messions_sleeping_tv);                              //长时间工作的休息
        }
    }

    g_finish_work_b = true;
    return 0;
}

bool CSipStackTask::RequestToPu( IN CRequestToPu* request_to_pu_msg_p )
{
    if ( !m_init_b )
    {
        ERROR_LOG("request to pu - fail - it must init first.");
        return false;
    }

    if ( NULL == request_to_pu_msg_p )
    {
        ERROR_LOG("request to pu - fail - invalid param. ") ;
        return false;
    }

    //if ( SS_MAX_QUEUE_COUNT <= m_post_to_pu_num )
    //{
    //    ERROR_LOG("request to pu - fail - too many request - requst cout is approximate to %u.",
    //               m_post_to_pu_num
    //             );
    //    return false;
    //}

    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, obj, m_req_to_pu_q_mutex, false);

    //取得当前写队列
    const unsigned int read_ui = (SS_ZERO != m_req_to_pu_q_read_ui ? SS_ZERO : SS_ONE);
    m_req_to_pu_q[read_ui].push_back(request_to_pu_msg_p);

    return true;
}

void CSipStackTask::HandleRequestMession( IN CRequestToPu* request_to_pu_msg_p )
{
    if ( NULL == request_to_pu_msg_p )
    {
        ERROR_LOG("handle request mission- fail - invalid param.");
        return ;
    }

    INFO_LOG("handle request mission - begin.");

    SipStackAgentServerImpl* sipstack_agent_server_p = SipStackAgentMgrImpl::Instance()->GetAgentServer();

    sipstack_agent_server_p->GetDcgStack().setServerAddrEx(true, request_to_pu_msg_p->GetIPv4(), request_to_pu_msg_p->GetPort());

    ACE_GUARD(ACE_Recursive_Thread_Mutex, obj, m_minisip_mutex); //暂时先放此处，将来放在距离MINISIP近处。
    if ( SS_SUCCESS != request_to_pu_msg_p->Execute() )
    {
        ERROR_LOG("handle request mission - failure to execute.");
        sipstack_agent_server_p->GetDcgStack().setServerAddrEx(false, "", 0);
        return ;
    }

    sipstack_agent_server_p->GetDcgStack().setServerAddrEx(false, "", 0);

    INFO_LOG("handle request mission - end.");
}

bool CSipStackTask::RespondToPu( IN CRespondToPu* respond_to_pu_msg_p )
{
    if ( !m_init_b )
    {
        ERROR_LOG("respond to pu - fail - it must init first.");
        return false;
    }

    if ( NULL == respond_to_pu_msg_p )
    {
        ERROR_LOG("respond to pu - fail - invalid param.");
        return false;
    }

    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, obj, m_resp_to_pu_q_mutex, false);

    //取得当前写队列
    const unsigned int read_ui = (SS_ZERO != m_resp_to_pu_q_read_ui ? SS_ZERO : SS_ONE);
    m_resp_to_pu_q[read_ui].push_back(respond_to_pu_msg_p);

    return true;
}

void CSipStackTask::HandleRespondMession( IN CRespondToPu* respond_to_pu_msg_p )
{
    if ( NULL == respond_to_pu_msg_p )
    {
        ERROR_LOG("handle respond mission- fail - invalid param. ");
        return ;
    }

    INFO_LOG("handle respond mission - begin.");

    SipStackAgentServerImpl* sipstack_agent_server_p = SipStackAgentMgrImpl::Instance()->GetAgentServer();

    sipstack_agent_server_p->GetDcgStack().setServerAddrEx(true, respond_to_pu_msg_p->GetIPv4(), respond_to_pu_msg_p->GetPort());

    ACE_GUARD(ACE_Recursive_Thread_Mutex, obj, m_minisip_mutex); //暂时先放此处，将来放在距离MINISIP近处。
    if ( SS_SUCCESS != respond_to_pu_msg_p->Execute() )
    {
        ERROR_LOG("handle respond mission - failure to execute.");
        sipstack_agent_server_p->GetDcgStack().setServerAddrEx(false, "", 0);
        return ;
    }

    sipstack_agent_server_p->GetDcgStack().setServerAddrEx(false, "", 0);

    INFO_LOG("handle respond mission - end.");
}

void CSipStackTask::HandleRecvMession( IN POST_TO_APP_ST* post_msg_p )
{
    if ( NULL == post_msg_p || NULL == post_msg_p->message_string_st.pcData )
    {
        ERROR_LOG("handle receive mission- fail - invalid param. ") ;
        return ;
    }

    DEBUG_LOG("handle receive mission - ready to call SipTptLiStrRecvInd - message=\n %s",
               std::string(post_msg_p->message_string_st.pcData, post_msg_p->message_string_st.ulLen).c_str());
             

    //上报响应消息
    ACE_GUARD(ACE_Recursive_Thread_Mutex, obj, m_minisip_mutex); //暂时先放此处，将来放在距离MINISIP近处。
    //此处的出口可能为SipTptLiStrSend
    EN_SIP_RESULT enResult = SipTptLiStrRecvInd(&(post_msg_p->message_string_st), &(post_msg_p->tpt_network_info_st), NULL);
    if (SIP_RET_SUCCESS != enResult)
    {
        ERROR_LOG("handle receive mission - failure to call SipTptLiStrRecvInd - Error= %d",enResult) ;
        return ;
    }
}

bool CSipStackTask::PostToApp( POST_TO_APP_ST* post_msg_p )
{
    if ( !m_init_b )
    {
        ERROR_LOG("post to app - fail - it must init first.");
        return false;
    }

    if ( NULL == post_msg_p || NULL == post_msg_p->message_string_st.pcData )
    {
        ERROR_LOG("post to app - fail - invalid param.");
        return false;
    }

    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, obj, m_post_to_app_mutex, false);

    m_post_to_app_q.push_back(post_msg_p);
    m_post_to_app_q_ui++;

    return true;
}

}
