#ifndef IVSCBB_SS_SIPSTACK_MSG_HANDLER_HPP
#define IVSCBB_SS_SIPSTACK_MSG_HANDLER_HPP

#include "sipstack_import.hpp" 
#include "sipstack_agent_msg.hpp"

#define SIP_STACK_CMDCODE_OPTION_MSG       "cmdCodeOptionMsgHander"
#define SIP_STACK_CMDCODE_REG_MSG          "cmdCodeRegMsgHander"
#define SIP_STACK_CMDCODE_DEFAULT_SYS      "cmdCodeDefaultMsgHander"

namespace SipStack{

typedef list<string> ListSipCmd;

class SipStackAgentMsgHandler
{
public:	
    SipStackAgentMsgHandler(){ };
    virtual ~SipStackAgentMsgHandler() { };

    virtual ListSipCmd& getCmdList() = 0;
    virtual void handRcvMsg(auto_ptr<SipStackAgentMsg> msgInfo) = 0; 
};


typedef map<string, SipStackAgentMsgHandler*> SipMessageHandlerMap;

}

#endif




