#ifndef IVSCBB_SS_SIPSTACK_AGENT_CLIENT_IMPL_HPP
#define IVSCBB_SS_SIPSTACK_AGENT_CLIENT_IMPL_HPP

#include "sipstack_import.hpp"
#include "sipstack_agent_client.hpp"
#include "sipstack_agent_msg_handler.hpp"

#define TRANS_ID_DEFAULT_LENGTH 20

namespace SipStack{

class SipStackAgentServerImpl;
class SipStackAgentMsg;

class DefaultMsgHandlerImplMiniSip;
class OptionMsgHandlerImplMiniSip;
class RegisterMsgHandlerImplMiniSip;

class SipStackAgentClientImpl: public SipStackAgentClient
{
public:
    SipStackAgentClientImpl(SipStackAgentServerImpl& agentServer);
    virtual ~SipStackAgentClientImpl();

    virtual bool SendRegister(const string& strReqUri,const string& strServer,unsigned short usPort,const string& strCallID,bool bIsRegister); 

    virtual bool SendOption(const string& strReqUri); 

    virtual void SendInviteResp(const string& strReqData, unsigned long seq, unsigned long retCode);

    virtual void SendResp(const unsigned long seq);

    virtual bool SendRegisterResp ( unsigned long ulResponseID, unsigned long resp_code_n, const string& strIPv4, unsigned short usPort );
    virtual bool SendCommonMsgResp( unsigned long ulResponseID, unsigned long resp_code_n, const string& strIPv4, unsigned short usPort );
    virtual bool SendOptionMsgResp ( unsigned long ulResponseID, unsigned long resp_code_n, const string& strIPv4, unsigned short usPort );
    virtual bool AsynSendMsgEx ( const string& strBody, const string& strUri, const string& strIPv4, unsigned short usPort);
    virtual bool SendInviteReq ( const string& strReqData, const string & strReqUri, const string& strCallID,
                                 const string& strSubject, const string& strIP, unsigned short usPort
                               );

    virtual bool SendInviteAck ( unsigned long ulSeq, const string& strIP, unsigned short usPort );
    virtual bool SendByeReq ( unsigned long resp_id_n, const std::string& strIPv4, unsigned short usPort );
    virtual bool SendCancelReq ( unsigned long ulSeq, const string& strIP, unsigned short usPort );
    virtual string SendSubscribeReq ( const string & strBody, const string & strUri, const std::string& strIPv4, unsigned short usPort );
    //////////////////////////////////////////  add by w00207027 2012-11-02 Begin ///////////////////////////////////////////////////////

public:
    //modify cwx148380 in 2012-12-27 22:46:27
    virtual SipStackAgentServer & GetAgentServer();

    //注册与注销Sip消息处理句柄
    virtual void SaveAgentMsgHandler(SipStackAgentMsgHandler* msgHandler);
    virtual void SaveAgentMsgHandlerWithDeviceID(const string & devId, SipStackAgentMsgHandler* msgHandler);
    virtual void DeleteAgentMsgHandler(SipStackAgentMsgHandler* msgHandler);
    virtual void DeleteAgentMsgHandlerWithDeviceID ( const string& device_id_str );

    virtual string GetMsgSeq();

    /*******************************************    异步方法    **************************************************/
    virtual bool AsynSendOption( const unsigned long trans_id,
                                 const unsigned long ret_code,
                                 const string& serverIP,
                                 const short serverPort
                                );
    virtual bool AsynSendAck ( const unsigned long resp_id_n,
                               const string& strIPv4,
                               const unsigned short usPort
                              );
    virtual void AsynSendMsg(const string & strReqData);
    virtual void AsynSendMsg(const string & strReqData, const string & strReqUri);
    virtual void AsynAnsMsg(const string& ans_code, const string& data );
    /*************************************************************************************************************/

    /*******************************************    同步方法    **************************************************/
    virtual auto_ptr<SipStackAgentMsg> SyncSendInviteEx ( const string& strCallID,
                                                     const string& strBody,
                                                     const string& strUri,
                                                     const string& strSubject,
                                                     const string& strIPv4,
                                                     const unsigned short usPort
                                                    );

    virtual string SyncSendMsg(const string & strReqData); 
    virtual string SyncSendMsg(const string & strReqData, const string & strReqUri,const string& strIPv4, unsigned short usPort,const string& strCallID); 
    virtual auto_ptr<SipStackAgentMsg> SyncSendMsgEx(const string & strReqData); 
    virtual auto_ptr<SipStackAgentMsg> SyncSendMsgEx(const string & strReqData, const string & strReqUri); 
    virtual auto_ptr<SipStackAgentMsg> SyncSendMsgEx(const string & strBody, const string & strUri, const string& strIPv4, unsigned short usPort,const string& strCallID);
    virtual string SyncSendMsgExList(const string & strReqData);
    virtual string SyncSendMsgExList(const string & strReqData, const string & strReqUri);
    virtual bool SyncSendMsgNoResp(const string & strReqData, const string & strReqUri);
    virtual bool SyncSendMsgNoResp(const string & strReqData);
    /*******************************************    异步方法    **************************************************/
    //cwx148680


public:
    void HandleRcvMsg(auto_ptr<SipStackAgentMsg> msg_ptr);

private:
    bool ParseUri ( const string& uri, string& uri_name, string& uri_domain );

    string ParseMsgDataSeq(const string &msgBody);

    SipMessageHandlerMap& GetSipMessageHanlderMap(); 

private:
    SipStackAgentServerImpl& m_agent_server;

    ACE_RW_Mutex         m_sip_msg_handler_map_rw_mutex;
    SipMessageHandlerMap m_sip_msg_handler_map;

private:
    //ACE_Thread_Mutex         _lockSendRegister;
    //ACE_Thread_Mutex         _lockSendOption;
    //ACE_Thread_Mutex         _lockSendAck;
    //ACE_Thread_Mutex         _lockSendInvite;
    //ACE_Thread_Mutex         _lockSyncSendMsgEx;
    //ACE_Thread_Mutex         _lockSipStack;

    unsigned long m_sip_msg_seq_ul;
}; 

}

#endif 




