#ifndef IVSCBB_SS_SIPSTACK_AGENT_MSG_HPP
#define IVSCBB_SS_SIPSTACK_AGENT_MSG_HPP


#include "sipstack_import.hpp" 
#include "sipstack_agent_msg_type.hpp"


namespace SipStack{

class SipStackAgentMsg
{
public:
    SipStackAgentMsg() { };
    virtual ~SipStackAgentMsg() { };

    virtual bool                 IsRequest()               = 0;
    virtual SIP_STACK_MSG_TYPE   GetType()                 = 0;
    virtual const string&        GetReqUri()               = 0;
    virtual const string&        GetReqName()              = 0;
    virtual const string&        GetReqDomain()            = 0;
    virtual const string&        GetCallID()               = 0;
    virtual const string&        GetSubject()              = 0;
    virtual const unsigned long  GetSequence()             = 0;
    virtual const string&        GetBodySequence()         = 0;
    virtual const string&        GetBodyCode()             = 0;
    virtual const string&        GetBody()                 = 0;
    virtual const unsigned long  GetStatusCode()           = 0;
    virtual const unsigned long  GetResponseID(void)       = 0;
    virtual const string&        GetIP(void)               = 0;
    virtual unsigned short       GetPort(void)             = 0;
    virtual const unsigned long  GetExpires(void)          = 0;
    virtual const string&        GetFrom(void)             = 0;
    virtual const string&        GetDigestUserName(void)   = 0;
    virtual const string&        GetDigestRealm(void)      = 0;
    virtual const string&        GetDigestNonce(void)      = 0;
    virtual const string&        GetDigestNonceCount(void) = 0;
    virtual const string&        GetDigestCNonce(void)     = 0;
    virtual const string&        GetDigestQop(void)        = 0;
    virtual const string&        GetDigestUri(void)        = 0;
    virtual const string&        GetDigestResponse(void)   = 0;
    virtual const string&        GetDigestAlgorithm(void)  = 0;
    virtual const unsigned long  GetDialogID(void)         = 0;
    virtual void*                GetUserData(void)         = 0;
    virtual const string&        PrintMessage()            = 0;
};

}
 
 
#endif  

