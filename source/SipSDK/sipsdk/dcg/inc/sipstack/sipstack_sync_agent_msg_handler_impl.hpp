#ifndef IVSCBB_SS_SIPSTACK_SYNC_MSG_HANDLER_HPP
#define IVSCBB_SS_SIPSTACK_SYNC_MSG_HANDLER_HPP

#include "sipstack_import.hpp"
#include "sipstack_agent_msg_handler.hpp"

namespace SipStack{

class SipStackAgentMsg;
class SipStackAgentClient;

class SipStackSyncAgentMsgHandlerImpl: public SipStackAgentMsgHandler
{
public:
    SipStackSyncAgentMsgHandlerImpl(SipStackAgentClient& agentClient, const std::string& strCmdCode);
    virtual ~SipStackSyncAgentMsgHandlerImpl();

    virtual ListSipCmd& getCmdList(); 
    virtual void handRcvMsg(auto_ptr<SipStackAgentMsg> sipstack_msg_ptr); 

public:
    void Reset();

    void WaitMsg();
    auto_ptr<SipStackAgentMsg> GetMsg();

    void Acquire();
    void Signal();
    void Release ();

private:
    void Init();
    void Fini();
    bool IsRcv();
    void HandleTimeout();

private:
    bool m_bInit;

    SipStackAgentClient&             m_agent_client;
    ListSipCmd                  m_cmd_list; 

    ACE_Recursive_Thread_Mutex  m_sync_thread_mutex;
    ACE_Condition<ACE_Recursive_Thread_Mutex > *m_condition_p;

    auto_ptr<SipStackAgentMsg>       m_sipstack_msg_ptr;

};


}
 
#endif
 
