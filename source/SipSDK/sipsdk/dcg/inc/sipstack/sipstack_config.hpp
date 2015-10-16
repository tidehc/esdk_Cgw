#ifndef IVSCBB_SS_SIPSTACK_AGENT_CONFIG_HPP
#define IVSCBB_SS_SIPSTACK_AGENT_CONFIG_HPP

#include "sipstack_import.hpp"
#include "DataType.h"

using namespace std;
namespace SipStack{

#define SS_SERVICE_CFG_IPV4_DFLT                   ("127.0.0.1")
#define SS_SERVICE_CFG_PORT_DFLT                   (5080)

class SipStackAgentCfg
{
public:
    string m_login_ip_str;
    string m_login_port_str;
    string m_login_name_str;
    string m_login_pwd_str;
    string m_login_domain_str;
	string m_ServerID;
	string m_LocalID;
    string m_agent_user_name_str;
    string m_agent_user_version_str;
    string m_local_ip_str;
    string m_local_port_str;
	string m_server_sipname;
	string m_server_sippwd;
	SIP_CallBack m_SIPCallBackFunc;
public:
    SipStackAgentCfg();
    SipStackAgentCfg(const SipStackAgentCfg& obj);
    virtual ~SipStackAgentCfg();

    SipStackAgentCfg& operator=(const SipStackAgentCfg& obj);
};

class CSipStackServiceCfg
{
private:
    std::string    m_strServiceIP;
    unsigned short m_usServicePort;
    std::string    m_strName;
    std::string    m_strDomainName;
    unsigned short m_usLogSwitch;
    unsigned int m_unSipMsgTimeout;
    unsigned long m_uExpires;

public:
    CSipStackServiceCfg();
    CSipStackServiceCfg(const CSipStackServiceCfg& obj);
    virtual ~CSipStackServiceCfg();

    const std::string&   GetServiceIP   () const;
    unsigned short       GetServicePort () const;
    const std::string&   GetName () const;
    const std::string&   GetDomainName  () const;
    unsigned int         GetSipMsgTimeout() const;
    unsigned short       GetLogSwitch() const;
    unsigned long        GetExpires() const;

    void SetServiceIP   ( const std::string& strServiceIP = SS_SERVICE_CFG_IPV4_DFLT );
    void SetServicePort ( const unsigned short usPort     = SS_SERVICE_CFG_PORT_DFLT );
    void SetName        ( const std::string& strName );
    void SetDomainName  ( const std::string& strDomainName );
    void SetLogSwitch   ( const unsigned short usSwitchFlag );
    void SetSipMsgTimeout  ( const unsigned int unTimeout = SIP_STACK_TIMEOUT_VALUE_MESSAGE );
    void SetExpires        ( const unsigned long uExpires );
};

class CSipVar
{
public:
    string m_strRequestUriUserInfo;
    string m_strRequestUriDomain;
    string m_strAgentName;
    string m_strCallID;
    string m_strSubject;
public:
    CSipVar();
};

class CSipStackConfigMgr
{
    static CSipStackConfigMgr m_instance;

    CSipStackServiceCfg m_ServiceCfg;

private:
    CSipStackConfigMgr ();
    ~CSipStackConfigMgr();
    CSipStackConfigMgr ( CSipStackConfigMgr& );
    CSipStackConfigMgr& operator = ( CSipStackConfigMgr& );

public:
    static CSipStackConfigMgr& Instance ();

    bool LoadConfig ( const std::string& strFile );

    const CSipStackServiceCfg& GetServiceCfg () const;
};

#define  SS_CFG_MGR_INS_REF       (CSipStackConfigMgr::Instance())

}

#endif

