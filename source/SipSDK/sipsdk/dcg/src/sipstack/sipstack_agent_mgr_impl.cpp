#include "sipstack_msg_ctrl.h"
#include "sipstack_agent_mgr_impl.hpp"
#include "Log.h"

namespace SipStack{

ACE_Recursive_Thread_Mutex g_sip_agent_mgr_thread_mutex;
SipStackAgentMgrImpl* SipStackAgentMgrImpl::m_instance_p = NULL;
unsigned int SipStackAgentMgrImpl::m_ref_n = 0;

SipStackAgentMgrImpl::SipStackAgentMgrImpl()
                       :m_bInit(false)
                       ,m_server_dflt_p(NULL)
                       ,m_client_dflt_p(NULL)
{
}

SipStackAgentMgrImpl::~SipStackAgentMgrImpl()
{
}

bool SipStackAgentMgrImpl::Init ( const SipStackAgentCfg& agent_reg_info, unsigned long max_num_ul, const std::string& strFile )
{
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, obj, g_sip_agent_mgr_thread_mutex, false);

    if (m_bInit)
    {
        m_ref_n++;
        INFO_LOG("sip agent manager - init- init again");
        return true;
    }

    if ( !SS_CFG_MGR_INS_REF.LoadConfig(strFile) )
    {
        ERROR_LOG("sip agent manager - init - failure to load config.");
    }

    //agent_reg_info.m_local_ip_str = SS_CFG_MGR_INS_REF.GetServiceCfg().GetServiceIP();
    //agent_reg_info.m_login_ip_str = SS_CFG_MGR_INS_REF.GetServiceCfg().GetServiceIP();
    //char szPort[10] = "";
    //memset(szPort, 0x0, sizeof(szPort));
    //(void)itoa((int)SS_CFG_MGR_INS_REF.GetServiceCfg().GetServicePort(), szPort, 10);
    //agent_reg_info.m_local_port_str = szPort;
    //agent_reg_info.m_login_port_str = szPort;
    //max_num_ul = 2 * SS_CTRL_MSG_MGR_INS_REF.GetQueueLength();

    m_server_dflt_p = new SipStackAgentServerImpl(agent_reg_info, max_num_ul);
    m_client_dflt_p = new SipStackAgentClientImpl( *m_server_dflt_p );
    if ( !m_server_dflt_p->Init(m_client_dflt_p) )
    {
        ERROR_LOG("sip agent manager - init - failure to init server");
        return false;
    }

    m_ref_n++;
    return (m_bInit = true);
}

void SipStackAgentMgrImpl::Fini()
{
    ACE_GUARD(ACE_Recursive_Thread_Mutex, obj, g_sip_agent_mgr_thread_mutex);
    if (!m_bInit)
    {
        INFO_LOG("sip agent manager - finish- it have not initialized.");
        return ;
    }

    m_ref_n--;
    if ( 0 != m_ref_n )
    {
        INFO_LOG("sip agent manager - finish- decrease reference.");
        return;
    }  

	if ( NULL != m_server_dflt_p )
    {
		m_server_dflt_p->Fini();
        delete m_server_dflt_p;
        m_server_dflt_p = NULL;
    }

	if ( NULL != m_client_dflt_p )
	{
		delete m_client_dflt_p;
		m_client_dflt_p = NULL;
	}

    m_bInit = false;
}

SipStackAgentClient* SipStackAgentMgrImpl::GetAgentClient()
{
    return m_client_dflt_p;
}

SipStackAgentServerImpl* SipStackAgentMgrImpl::GetAgentServer()
{
    return m_server_dflt_p;
}

SipStackAgentMgrImpl* SipStackAgentMgrImpl::Instance()
{
    if(NULL == m_instance_p)
    {
        ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, obj, g_sip_agent_mgr_thread_mutex, NULL);
        if(NULL == m_instance_p)
        {
            ACE_NEW_RETURN(m_instance_p, SipStackAgentMgrImpl(), NULL);
        }
    }

    return m_instance_p;
}

SipStackAgentMgr& GetAgentMgr()
{
    return *(SipStackAgentMgrImpl::Instance());
}

SipStackAgentClient* GetAgentClient()
{
    return SipStackAgentMgrImpl::Instance()->GetAgentClient();
}

}
