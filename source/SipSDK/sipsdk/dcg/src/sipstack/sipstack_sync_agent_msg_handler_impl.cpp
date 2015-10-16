#include "sipstack_import.hpp"
#include "sipstack_agent_msg_impl.hpp"
#include "sipstack_agent_client_impl.hpp"
#include "sipstack_sync_agent_msg_handler_impl.hpp"
#include "sipstack_config.hpp"
#include "Log.h"

namespace SipStack{

SipStackSyncAgentMsgHandlerImpl::SipStackSyncAgentMsgHandlerImpl(SipStackAgentClient& agentClient, const std::string& strCmdCode)
                          :m_bInit(false)
                          ,m_agent_client(agentClient)
                          ,m_condition_p( NULL )
{
    Init();

    m_cmd_list.push_back(strCmdCode);
    if ( !strCmdCode.empty() )
    {
        DEBUG_LOG("sync message handler - cmd_code=%s",strCmdCode.c_str());
    }
    else
    {
        DEBUG_LOG("sync message handler - cmd_code is empty.");
    }
    m_agent_client.SaveAgentMsgHandler(this);
}

SipStackSyncAgentMsgHandlerImpl::~SipStackSyncAgentMsgHandlerImpl()
{
    m_agent_client.DeleteAgentMsgHandler(this);

    Fini();
}

void SipStackSyncAgentMsgHandlerImpl::Init()
{
    m_condition_p = new ACE_Condition<ACE_Recursive_Thread_Mutex >(m_sync_thread_mutex);
    m_bInit = true;
}

void SipStackSyncAgentMsgHandlerImpl::Fini()
{
    if ( NULL != m_condition_p )
    {
        delete m_condition_p;
        m_condition_p = NULL;
    }

    m_bInit = false;
}

void SipStackSyncAgentMsgHandlerImpl::Reset()
{
    m_sipstack_msg_ptr.reset(); 
}

void SipStackSyncAgentMsgHandlerImpl::Acquire ()
{
    if ( !m_bInit || NULL == m_condition_p )
    {
        ERROR_LOG("sync message handler - acquire read - need init or empty condition -");
        return ;
    }

    m_sync_thread_mutex.acquire();
}

void SipStackSyncAgentMsgHandlerImpl::Signal()
{
    if ( !m_bInit || NULL == m_condition_p )
    {
        ERROR_LOG("sync message handler - broadcast - need init or empty condition - ");
        return ;
    }
    //m_condition_p->broadcast();
    m_condition_p->signal();
}

void SipStackSyncAgentMsgHandlerImpl::Release()
{
    if ( !m_bInit || NULL == m_condition_p )
    {
        ERROR_LOG("sync message handler - release - need init or empty condition - ");
        return ;
    }

    m_sync_thread_mutex.release();
}

bool SipStackSyncAgentMsgHandlerImpl::IsRcv()
{
    if ( NULL == m_sipstack_msg_ptr.get() )
    {
        return false;
    }

    return true;
}

void SipStackSyncAgentMsgHandlerImpl::HandleTimeout()
{
    DEBUG_LOG("sync message handler - timeout.");
}

void SipStackSyncAgentMsgHandlerImpl::WaitMsg()
{
    if ( !m_bInit || NULL == m_condition_p )
    {
        ERROR_LOG("sync message handler - wait msg - need init or empty condition");
        return ;
    }

    //ACE_Time_Value timeout(SIP_STACK_TIMEOUT_VALUE_MESSAGE, 0);
    ACE_Time_Value timeout((long long)(CSipStackConfigMgr::Instance().GetServiceCfg().GetSipMsgTimeout()), 0);
    timeout += ACE_OS::gettimeofday();

    INFO_LOG("sync message handler - wait msg - wait a little.");
    if ( -1 == m_condition_p->wait(&timeout) )
    {
        WARN_LOG("sync message handler - wait msg - failure to wait.");
    }

    if (!IsRcv())
    {
        WARN_LOG("sync message handler - wait msg - failure to receive.");
        HandleTimeout();
    }
}

auto_ptr<SipStackAgentMsg> SipStackSyncAgentMsgHandlerImpl::GetMsg()
{
    return m_sipstack_msg_ptr;
}

ListSipCmd& SipStackSyncAgentMsgHandlerImpl::getCmdList()
{
    return m_cmd_list;
}

void SipStackSyncAgentMsgHandlerImpl::handRcvMsg( auto_ptr<SipStackAgentMsg> sipstack_msg_ptr )
{
    Acquire();
    m_sipstack_msg_ptr = sipstack_msg_ptr;
    Signal();
    Release();
}

}

