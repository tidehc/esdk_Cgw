#ifndef IVSCBB_SS_SIPSTACK_AGENT_MSG_IMPL_HPP
#define IVSCBB_SS_SIPSTACK_AGENT_MSG_IMPL_HPP

#include "sipstack_import.hpp" 
#include "sipstack_agent_msg.hpp"

namespace SipStack{

class  SipStackMsgImplMiniSip: public SipStackAgentMsg
{
public:
    SipStackMsgImplMiniSip();
    virtual ~SipStackMsgImplMiniSip();

    virtual bool                 IsRequest();
    virtual SIP_STACK_MSG_TYPE   GetType();
    virtual const string&        GetReqUri();
    virtual const string&        GetReqName();
    virtual const string&        GetReqDomain();
    virtual const string&        GetCallID();
    virtual const string&        GetSubject();
    virtual const unsigned long  GetSequence();
    virtual const string&        GetBodySequence();
    virtual const string&        GetBodyCode();
    virtual const string&        GetBody();
    virtual const unsigned long  GetStatusCode();
    virtual const unsigned long  GetResponseID(void);
    virtual const string&        GetIP(void);
    virtual unsigned short       GetPort(void);
    virtual const unsigned long  GetExpires(void);
    virtual const string&        GetFrom(void);
    virtual const string&        GetDigestUserName(void);
    virtual const string&        GetDigestRealm(void);
    virtual const string&        GetDigestNonce(void);
    virtual const string&        GetDigestNonceCount(void);
    virtual const string&        GetDigestCNonce(void);
    virtual const string&        GetDigestQop(void);
    virtual const string&        GetDigestUri(void);
    virtual const string&        GetDigestResponse(void);
    virtual const string&        GetDigestAlgorithm(void);
    virtual const unsigned long  GetDialogID(void);
    virtual void*                GetUserData(void);
    virtual const string&        PrintMessage();

public:
    void SetRequestFlag(bool req_flg_b = true);
    void SetType(const SIP_STACK_MSG_TYPE sipstack_msg_type_en);
    void SetReqURI(const string& req_uri_str);
    void SetReqName(const string& req_name_str);
    void SetReqDomain(const string& req_domain_str);
    void SetCallID(const string& call_id_str);
    void SetSubject(const string& subject_str);
    void SetSequence(const unsigned long seq_ul);
    void SetBodySequence(const string& body_seq_str);
    void SetBodyCode(const string& body_code_str);
    void SetBody(const string& body_str);
    void SetStatusCode(const unsigned long status_code_ul);
    void SetCmd(const string& cmd_str);
    void SetRespondID(const unsigned long resp_id_ul);
    void SetIP(const string& ip_str);
    void SetPort(unsigned short port_us);
    void SetDomain(const string& domain_str);
    void SetExpires(const unsigned long expires_ul);
    void SetFrom(const string& from_str);
    void SetDigestUserName(const string& digest_user_name_str);
    void SetDigestRealm(const string& digest_realm_str);
    void SetDigestNonce(const string& digest_nonce_str);
    void SetDigestNonceCount(const string& digest_nonce_count_str);
    void SetDigestCNonce(const string& digest_cnonce);
    void SetDigestQop(const string& digest_qop_str);
    void SetDigestUri(const string& digest_uri_str);
    void SetDigestResponse(const string& digest_response_str);
    void SetDigestAlgorithm(const string& digest_algorithm_str);
    void SetDialogID(const unsigned long  dialog_id_ul);
    void SetUserData(void*  user_data_p);
    void SetMessage(const string& message_str);

private:
    void Init();

private:
    bool                  m_request_flag_b;
    SIP_STACK_MSG_TYPE    m_msg_type_en;

    string           m_req_uri_str;
    string           m_req_name_str;
    string           m_req_domain_str;

    string           m_call_id_str;
    string           m_subject_str;
    unsigned long    m_sequence_ul;

    string           m_body_sequence_str;
    string           m_body_code_str;
    string           m_body_str;
    unsigned long    m_status_code_ul; 

    string           m_cmd_str;

    unsigned long    m_resp_id_ul;

    string           m_ip_str;
    unsigned short   m_port_us;
    string           m_domain_str;
    unsigned long    m_expires_ul;
    string           m_from_str;
    string           m_digest_user_name_str;
    string           m_digest_realm_str;
    string           m_digest_nonce_str;
    string           m_digest_nonce_count_str;
    string           m_digest_cnonce_str;
    string           m_digest_qop_str;
    string           m_digest_uri_str;
    string           m_digest_response_str;
    string           m_digest_algorithm_str;
    unsigned long    m_dialog_id_ul;
    void*            m_use_data_p;
    string           m_message_str;
};


}


#endif
