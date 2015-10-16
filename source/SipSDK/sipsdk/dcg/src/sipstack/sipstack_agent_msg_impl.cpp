#include "sipstack_import.hpp"
#include "sipstack_agent_msg_impl.hpp"


namespace SipStack{

SipStackMsgImplMiniSip::SipStackMsgImplMiniSip()
    :m_port_us(0)
	,m_resp_id_ul(0)
	,m_expires_ul(0)
	,m_dialog_id_ul(0)
	,m_use_data_p(NULL)
{
    // cout<<endl<<"SipStackMsgImplMiniSip::SipStackMsgImplMiniSip..." <<endl;
    Init(); 
}

SipStackMsgImplMiniSip::~SipStackMsgImplMiniSip()
{
    // cout<<endl<<"SipStackMsgImplMiniSip::~SipStackMsgImplMiniSip..." <<endl;
}


void SipStackMsgImplMiniSip::Init()
{
    m_request_flag_b       = true;
    m_msg_type_en        = SIP_STACK_TYPE_MAX;

    m_req_uri_str      = SIP_STRING_NULL;
    m_req_name_str     = SIP_STRING_NULL;
    m_req_domain_str   = SIP_STRING_NULL;

    m_call_id_str      = SIP_STRING_NULL;
    m_subject_str     = SIP_STRING_NULL; 
    m_sequence_ul = SIP_INT_NULL;

    m_body_sequence_str= SIP_STRING_NULL;
    m_body_code_str = SIP_STRING_NULL; 
    m_body_str        = SIP_STRING_NULL;
    m_status_code_ul        = SIP_STACK_SUCCESS;

    return;
}


bool SipStackMsgImplMiniSip::IsRequest()
{
    return m_request_flag_b;
}


SIP_STACK_MSG_TYPE SipStackMsgImplMiniSip::GetType()
{
    return m_msg_type_en;
}


const string& SipStackMsgImplMiniSip::GetReqUri()
{
    return m_req_uri_str;
}


const string& SipStackMsgImplMiniSip::GetReqName()
{
    return m_req_name_str;
}


const string& SipStackMsgImplMiniSip::GetReqDomain()
{
    return m_req_domain_str;
}


const string& SipStackMsgImplMiniSip::GetCallID()
{
    return m_call_id_str;
}


const string& SipStackMsgImplMiniSip::GetSubject()
{
    return m_subject_str;
}


const unsigned long SipStackMsgImplMiniSip::GetSequence()
{
    return m_sequence_ul;
}


const string& SipStackMsgImplMiniSip::GetBodySequence()
{
    return m_body_sequence_str;
}


const string& SipStackMsgImplMiniSip::GetBodyCode()
{
    return m_body_code_str;
}


const string& SipStackMsgImplMiniSip::GetBody()
{
    return m_body_str;
}


const unsigned long SipStackMsgImplMiniSip::GetStatusCode()
{
    return m_status_code_ul;
}


const unsigned long SipStackMsgImplMiniSip::GetResponseID(void)
{
    return m_resp_id_ul;
}

const string& SipStackMsgImplMiniSip::GetIP(void)
{
    return m_ip_str;
}

unsigned short SipStackMsgImplMiniSip::GetPort(void)
{
    return m_port_us;
}

const unsigned long SipStackMsgImplMiniSip::GetExpires(void)
{
    return m_expires_ul;
}

const string& SipStackMsgImplMiniSip::GetFrom( void )
{
    return m_from_str;
}

const string& SipStackMsgImplMiniSip::GetDigestUserName(void) 
{
    return m_digest_user_name_str;
}

const string& SipStackMsgImplMiniSip::GetDigestRealm(void) 
{
    return m_digest_realm_str;
}

const string& SipStackMsgImplMiniSip::GetDigestNonce(void) 
{
    return m_digest_nonce_str;
}

const string& SipStackMsgImplMiniSip::GetDigestCNonce(void)
{
    return m_digest_cnonce_str;
}

const string& SipStackMsgImplMiniSip::GetDigestNonceCount(void)
{
    return m_digest_nonce_count_str;
}

const string& SipStackMsgImplMiniSip::GetDigestQop(void) 
{
    return m_digest_qop_str;
}

const string& SipStackMsgImplMiniSip::GetDigestUri(void)
{
    return m_digest_uri_str;
}

const string& SipStackMsgImplMiniSip::GetDigestResponse(void)
{
    return m_digest_response_str;
}

const string& SipStackMsgImplMiniSip::GetDigestAlgorithm(void)
{
    return m_digest_algorithm_str;
}

const unsigned long SipStackMsgImplMiniSip::GetDialogID(void) 
{
    return m_dialog_id_ul;
}

void* SipStackMsgImplMiniSip::GetUserData(void)
{
    return m_use_data_p;
}

const string& SipStackMsgImplMiniSip::PrintMessage()
{
    return m_message_str;
}

void SipStackMsgImplMiniSip::SetRequestFlag(bool req_flg_b)
{
    m_request_flag_b = req_flg_b;
}


void SipStackMsgImplMiniSip::SetType(SIP_STACK_MSG_TYPE sipstack_msg_type_en)
{
    m_msg_type_en = sipstack_msg_type_en;
}


void SipStackMsgImplMiniSip::SetReqURI(const string& req_uri_str)
{
    m_req_uri_str = req_uri_str;
}


void SipStackMsgImplMiniSip::SetReqName(const string& req_name_str)
{
    m_req_name_str = req_name_str;
}


void SipStackMsgImplMiniSip::SetReqDomain(const string& req_domain_str)
{
    m_req_domain_str = req_domain_str;
}


void SipStackMsgImplMiniSip::SetCallID(const string& call_id_str)
{
    m_call_id_str = call_id_str;
}


void SipStackMsgImplMiniSip::SetSubject(const string& subject_str)
{
    m_subject_str = subject_str;
}


void SipStackMsgImplMiniSip::SetSequence(const unsigned long seq_str)
{
    m_sequence_ul = seq_str;
}


void SipStackMsgImplMiniSip::SetBodySequence(const string& body_seq_str)
{
    m_body_sequence_str = body_seq_str;
}


void SipStackMsgImplMiniSip::SetBodyCode(const string& body_code_str)
{
    m_body_code_str= body_code_str;
}


void SipStackMsgImplMiniSip::SetBody(const string& body_str)
{
    m_body_str= body_str;
}


void SipStackMsgImplMiniSip::SetStatusCode(const unsigned long status_code_ul)
{
    m_status_code_ul= status_code_ul;
}

void SipStackMsgImplMiniSip::SetCmd(const string& cmd_str)
{
    m_cmd_str = cmd_str;
}

void SipStackMsgImplMiniSip::SetRespondID(const unsigned long resp_id_ul)
{
    m_resp_id_ul = resp_id_ul;
}

void SipStackMsgImplMiniSip::SetIP(const string& ip_str)
{
    m_ip_str = ip_str;
}

void SipStackMsgImplMiniSip::SetPort(unsigned short port_us)
{
    m_port_us = port_us;
}

void SipStackMsgImplMiniSip::SetDomain(const string& domain_str)
{
    m_domain_str = domain_str;
}

void SipStackMsgImplMiniSip::SetExpires( const unsigned long expires_ul )
{
    m_expires_ul = expires_ul;
}

void SipStackMsgImplMiniSip::SetFrom( const string& from_str )
{
    m_from_str = from_str;
}

void SipStackMsgImplMiniSip::SetDigestUserName(const string& digest_user_name_str) 
{
    m_digest_user_name_str = digest_user_name_str;
}

void SipStackMsgImplMiniSip::SetDigestRealm(const string& digest_realm_str)
{
    m_digest_realm_str = digest_realm_str;
}

void SipStackMsgImplMiniSip::SetDigestNonce(const string& digest_nonce_str)
{
    m_digest_nonce_str = digest_nonce_str;
}

void SipStackMsgImplMiniSip::SetDigestNonceCount(const string& digest_nouce_count)
{
    m_digest_nonce_count_str = digest_nouce_count;
}

void SipStackMsgImplMiniSip::SetDigestCNonce(const string& digest_cnonce)
{
    m_digest_cnonce_str = digest_cnonce;
}

void SipStackMsgImplMiniSip::SetDigestQop(const string& digest_qop_str) 
{
    m_digest_qop_str = digest_qop_str;
}

void SipStackMsgImplMiniSip::SetDigestUri(const string& digest_uri_str)
{
    m_digest_uri_str = digest_uri_str;
}

void SipStackMsgImplMiniSip::SetDigestResponse(const string& digest_response_str) 
{
    m_digest_response_str = digest_response_str;
}

void SipStackMsgImplMiniSip::SetDigestAlgorithm(const string& digest_algorithm_str) 
{
    m_digest_algorithm_str = digest_algorithm_str;
}
void SipStackMsgImplMiniSip::SetDialogID(const unsigned long  dialog_id_ul) 
{
    m_dialog_id_ul = dialog_id_ul;
}

void SipStackMsgImplMiniSip::SetUserData(void*  user_data_p) 
{
    m_use_data_p = user_data_p;
}

void SipStackMsgImplMiniSip::SetMessage(const string& message_str)
{
    m_message_str = message_str;
}

}

