#ifndef IVSCBB_SS_SIPSTACK_MSG_TYPE_HPP
#define IVSCBB_SS_SIPSTACK_MSG_TYPE_HPP

#include <string>
using namespace std;

namespace SipStack{
typedef int (*SIPHANDLER)(const string &msg, const string &msgBody, const unsigned long retCode);

typedef enum _SIP_STACK_MSG_TYPE
{
    SIP_STACK_TYPE_REGISTER      = 0x0001 | 0x1000,
    SIP_STACK_TYPE_UNREGISTER    = 0x0002 | 0x1000,
    SIP_STACK_TYPE_OPTIONS       = 0x0003 | 0x1000,
    SIP_STACK_TYPE_MESSAGE       = 0x0004 | 0x1000,
    SIP_STACK_TYPE_INVITE        = 0x0005 | 0x1000,
    SIP_STACK_TYPE_BYE           = 0x0006 | 0x1000,
    SIP_STACK_TYPE_CANCEL        = 0x0007 | 0x1000,
    SIP_STACK_TYPE_ACK           = 0x0008 | 0x1000,
    SIP_STACK_TYPE_INVITE_RSP    = 0x0009 | 0x1000,
    SIP_STACK_TYPE_NOTIFY        = 0x0012 | 0x1000,
	SIP_STACK_TYPE_MAX
} SIP_STACK_MSG_TYPE; 

}

#endif //sipstack_agent_msg_type.hpp

