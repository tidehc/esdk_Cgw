#ifndef IVSCBB_SS_SIPSTACK_AGENT_SERVER_HPP
#define IVSCBB_SS_SIPSTACK_AGENT_SERVER_HPP

#include <limits>
#include "sipstack_import.hpp"

using namespace std;
namespace SipStack{

class SipStackAgentCfg;
class SipStackAgentClient; 

class SipStackAgentServer
{
public:
    SipStackAgentServer() {};
    virtual ~SipStackAgentServer() {};

    virtual bool Init ( SipStackAgentClient* sipstack_agent_client_p ) = 0;
    virtual void Fini () = 0;

    virtual SipStackAgentCfg & GetAgentConfig() = 0;
};

typedef map<unsigned short, SipStackAgentServer*> SipAgentServerMap_t;

}
 
#endif

