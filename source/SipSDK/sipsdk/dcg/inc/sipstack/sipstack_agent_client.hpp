#ifndef IVSCBB_SS_SIPSTACK_AGENT_CLIENT_HPP
#define IVSCBB_SS_SIPSTACK_AGENT_CLIENT_HPP

#include "sipstack_import.hpp"

using namespace std;
namespace SipStack{

class SipStackAgentServer;
class SipStackAgentMsgHandler;
class SipStackAgentMsg;

class SipStackAgentClient
{
public:
    SipStackAgentClient(){ };
    virtual ~SipStackAgentClient() { };

    //used
    virtual bool SendRegister(const string& strReqUri,const string& strServer,unsigned short usPort,const string& strCallID,bool bIsUnregister) = 0;

    //unused
    virtual bool SendOption(const string& strReqUri) = 0; 

    //down virtual void asynSendMsg(const string& strReqData, const string& strReqUri) = 0;

    //down virtual string syncSendMsg(const string& strReqData, const string& strReqUri) = 0;

    //down virtual auto_ptr<SipStackMsg> syncSendMsgEx(const string& strReqData, const string& strReqUri) = 0;

    //down virtual auto_ptr<SipStackMsg> syncSendMsgEx(const string& strReqData, const string& strReqUri, const string& devIP, unsigned short devPort) = 0;

    //down virtual string syncSendMsgExList(const string& strReqData) = 0;

    //down virtual string syncSendMsgExList(const string& strReqData, const string& strReqUri) = 0;

    //down virtual bool sendRegister() = 0; 

    //down virtual bool sendOption() = 0; 

    //down virtual void asynSendMsg(const string& strReqData) = 0; 

    //unused
    virtual void SendInviteResp(const string& strReqData, unsigned long ulResponseID, unsigned long ulStatusCode) = 0;

    //unused
    virtual void SendResp(const unsigned long ulResponseID) = 0;

    //down virtual string syncSendMsg(const string& strReqData) = 0; 

    //down virtual auto_ptr<SipStackMsg> syncSendMsgEx(const string& strReqData) = 0; 
  
    //down virtual string getMsgSeq() = 0;

    //down virtual SipAgentServer & getAgentServer() = 0; 

    //down virtual bool syncSendMsgNoResp(const string& strReqData, const string& strReqUri)=0;

    //down virtual bool syncSendMsgNoResp(const string& strReqData)=0;

    //////////////////////////////////////////  add by w00207027 2012-11-02 Begin ///////////////////////////////////////////////////////
    //int setServerAddrEx(bool bServerAddrEx, string& serverIP, unsigned short serverPort) = 0;
    virtual bool SendRegisterResp ( unsigned long ulResponseID, unsigned long resp_code_n, const string& strIPv4, unsigned short usPort ) = 0;
    virtual bool SendCommonMsgResp( unsigned long ulResponseID, unsigned long resp_code_n, const string& strIPv4, unsigned short usPort ) = 0;
    virtual bool SendOptionMsgResp ( unsigned long ulResponseID, unsigned long resp_code_n, const string& strIPv4, unsigned short usPort ) = 0;
    virtual bool AsynSendMsgEx ( const string& strBody, const string& strUri, const string& strIPv4, unsigned short usPort ) = 0;
    //unused
    virtual bool SendInviteReq ( const string& strReqData, const string & strReqUri, const string& strCallID,
                                 const string& strSubject, const string& strIP, unsigned short usPort ) = 0;
    //unused
    virtual bool SendInviteAck ( unsigned long ulSeq, const string& strIP, unsigned short usPort ) = 0;
    virtual bool SendByeReq ( unsigned long ulResponseID, const std::string& strIPv4, unsigned short usPort) = 0;
    //unused
    virtual bool SendCancelReq ( unsigned long ulSeq, const string& strIP, unsigned short usPort ) = 0;
    virtual string SendSubscribeReq ( const string & strBody, const string & strUri, const std::string& strIPv4, unsigned short usPort ) = 0;
    //////////////////////////////////////////  add by w00207027 2012-11-02 Begin ///////////////////////////////////////////////////////


    //modify cwx148380 in 2012-12-27 22:46:27
    //virtual SipAgentServer & getAgentServer() = 0;

    //注册与注销Sip消息处理句柄
    virtual void SaveAgentMsgHandler(SipStackAgentMsgHandler* msg_handler_p) = 0;
    virtual void DeleteAgentMsgHandler(SipStackAgentMsgHandler* msg_handler_p) = 0;
    virtual void SaveAgentMsgHandlerWithDeviceID (const string & device_id_str, SipStackAgentMsgHandler* msg_handler_p) = 0;
    virtual void DeleteAgentMsgHandlerWithDeviceID ( const string& device_id_str ) = 0;

    virtual string GetMsgSeq() = 0;

    /*******************************************    异步方法    **************************************************/
    //unused
    virtual bool AsynSendOption( const unsigned long trans_id,
                                 const unsigned long ret_code,
                                 const string& serverIP,
                                 const short serverPort
                                ) = 0;
    virtual bool AsynSendAck ( const unsigned long resp_id_n,
                               const string& strIPv4,
                               const unsigned short usPort
                             ) = 0;
    //unused
    virtual void AsynSendMsg(const string& strReqData) = 0;
    //unused
    virtual void AsynSendMsg(const string & strReqData, const string & strReqUri) = 0;
    //unused
    virtual void AsynAnsMsg(const string& ans_code, const string& data ) = 0;
    /*************************************************************************************************************/

    /*******************************************    同步方法    **************************************************/
    virtual auto_ptr<SipStackAgentMsg> SyncSendInviteEx ( const string& strCallID,
                                                     const string& strBody,
                                                     const string& strUri,
                                                     const string& strSubject,
                                                     const string& strIPv4,
                                                     const unsigned short usPort
                                                   ) = 0;
    //unused
    virtual string SyncSendMsg(const string & strReqData) = 0;
    //unused
    virtual string SyncSendMsg(const string & strReqData, const string & strReqUri,const string& strIPv4, unsigned short usPort,const string& strCallID) = 0;
    //unused
    virtual auto_ptr<SipStackAgentMsg> SyncSendMsgEx(const string & strReqData) = 0;
    //unused
    virtual auto_ptr<SipStackAgentMsg> SyncSendMsgEx(const string & strReqData, const string & strReqUri) = 0; 
    virtual auto_ptr<SipStackAgentMsg> SyncSendMsgEx(const string & strBody, const string & strUri, const string& strIPv4, unsigned short usPort,const string& strCallID) = 0;
    //unused
    virtual string SyncSendMsgExList(const string & strReqData) = 0;
    //unused
    virtual string SyncSendMsgExList(const string & strReqData, const string & strReqUri) = 0;
    //unused
    virtual bool SyncSendMsgNoResp(const string & strReqData, const string & strReqUri)=0;
    //unused
    virtual bool SyncSendMsgNoResp(const string & strReqData)=0;
    /*******************************************    异步方法    **************************************************/
    //cwx148680
};


//typedef map<string, SipStackAgentClient*> MapURI2AgentClient;

}


#endif


