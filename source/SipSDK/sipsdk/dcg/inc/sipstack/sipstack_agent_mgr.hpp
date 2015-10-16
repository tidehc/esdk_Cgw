#ifndef IVSCBB_SS_SIPSTACK_AGENT_MGR_HPP
#define IVSCBB_SS_SIPSTACK_AGENT_MGR_HPP

#include <limits>
#include "sipstack_import.hpp"
#include "sipstack_config.hpp"
#include "sipstack_agent_msg.hpp"
#include "sipstack_agent_msg_type.hpp"
#include "sipstack_agent_msg_handler.hpp"
#include "sipstack_agent_client.hpp"

namespace SipStack{

class SipStackAgentMgr
{
public:
    SipStackAgentMgr() {};
    virtual ~SipStackAgentMgr() {};

    virtual bool Init ( const SipStackAgentCfg& agent_reg_info, unsigned long max_num_ul, const std::string& strFile ) = 0;
    virtual void Fini () = 0;

    virtual SipStackAgentClient* GetAgentClient() = 0;
};

SipStackAgentMgr&    GetAgentMgr();
SipStackAgentClient* GetAgentClient();

}

#endif



