#ifndef IVSCBB_SS_SIPSTACK_AGENT_HEADER_HPP
#define IVSCBB_SS_SIPSTACK_AGENT_HEADER_HPP

#include "sipstack_agent_msg_type.hpp"
using namespace std;

namespace SipStack{

struct SIP_TOKEN
{
    unsigned short   usTokenId;
    unsigned short   usSpare;
    string      stString;

    SIP_TOKEN(void):
        usTokenId(0), usSpare(0), stString("")
        {}
};

struct SIP_REQUEST_LINE
{
    SIP_TOKEN   stMethod;          /* Request method (EN_SIP_METHOD) */
    string     stRequestURI;
    unsigned short  usSipVersionMajor; /* SIP-Version "2" in "2.0" */
    unsigned short  usSipVersionMinor; /* SIP-Version "0" in "2.0" */

    SIP_REQUEST_LINE(void)
        :stRequestURI(""), usSipVersionMajor(0), usSipVersionMinor(0)
    {}
};

struct SIP_STATUS_LINE
{
    int     iSipStatusCode;
    string  strReasonPhrase;
    unsigned short  usSipVersionMajor; /* SIP-Version "2" in "2.0" */
    unsigned short  usSipVersionMinor; /* SIP-Version "0" in "2.0" */

    SIP_STATUS_LINE(void)
        :iSipStatusCode(0), strReasonPhrase(""),
         usSipVersionMajor(0), usSipVersionMinor(0)
    {}

};

 struct SIP_TIME_STAMP
 {
     unsigned long lTimeSecs;
     long lTimeMilliSecs;
     long lDelaySecs;
     long lDelayMilliSecs;

     SIP_TIME_STAMP(void):lTimeSecs(0), lTimeMilliSecs(0), lDelaySecs(0), lDelayMilliSecs(0)
     {}
 };

struct SIP_URI
{
    string displayName;
    string hostName;
    string tag;

    SIP_URI(void):displayName(""), hostName(""), tag(""){}
};

struct SIP_HEADER
{
    int                   iStatusCode;  /* if enMsgType in SipMsgS is SIP_RESPONSE */
    unsigned long         seq;
    string                sipMsg;  // used for debug
    unsigned long         userSeq;
    char *                pMsgBody;
    _SIP_STACK_MSG_TYPE   msgType;
    bool                  reqMsg;
    string                callID;
    string                subJect;
    string                devIP;
    unsigned short        devPort;
    string                devDomain;
    unsigned long         expires;
    string                from;
    string                digestUserName;
    string                digestRealm;
    string                digestNonce;
    string                digestUri;
    string                digestResponse;
    string                digestOpaque;
    string                digestCNonce;
    string                digestNonceCount;
    string                digestQop;
    string                digestAlgorithm;

    unsigned long         ulDialogId;
    void*                 pUserData;

    SIP_HEADER(void)
        :iStatusCode(0), seq(0), sipMsg(""), userSeq(0), pMsgBody(NULL), msgType(SIP_STACK_TYPE_MESSAGE), reqMsg(false), callID(""), subJect(""),
        devIP(""),devPort(0),devDomain(""),expires(0),from(""),digestUserName(""),digestRealm(""),digestNonce(""),digestUri(""),digestResponse(""),
        digestOpaque(""),	digestCNonce(""),	digestNonceCount(""),digestQop(""),digestAlgorithm(""),ulDialogId(0), pUserData(NULL)
    {
    }
};

}

#endif
