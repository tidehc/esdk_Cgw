#include "tinyxml.h"
#include "sipheaders/ssenums.h"
#include "sipstack_defs.h"
#include "sipstack_msg_ctrl.h"
#include "sipstack_config.hpp"
#include "Log.h"
namespace SipStack{

SipStackAgentCfg::SipStackAgentCfg()
    :m_login_ip_str("")
    ,m_login_port_str("")
    ,m_login_name_str("")
    ,m_login_pwd_str("")
    ,m_login_domain_str("")
	,m_ServerID("")
	,m_LocalID("")
    ,m_agent_user_name_str("")
    ,m_agent_user_version_str("")
    ,m_local_ip_str("")
    ,m_local_port_str("")
	,m_server_sipname("")
	,m_server_sippwd("")
	,m_SIPCallBackFunc(NULL)
{
}

SipStackAgentCfg::SipStackAgentCfg(const SipStackAgentCfg& other )
	:m_login_ip_str(other.m_login_ip_str)
	,m_login_port_str(other.m_login_port_str)
	,m_login_name_str(other.m_login_name_str)
	,m_login_pwd_str(other.m_login_pwd_str)
	,m_login_domain_str(other.m_login_domain_str)
	,m_ServerID(other.m_ServerID)
	,m_LocalID(other.m_LocalID)
	,m_agent_user_name_str(other.m_agent_user_name_str)
	,m_agent_user_version_str(other.m_agent_user_version_str)
	,m_local_ip_str(other.m_local_ip_str)
	,m_local_port_str(other.m_local_port_str)
	,m_server_sipname(other.m_server_sipname)
	,m_server_sippwd(other.m_server_sippwd)
	,m_SIPCallBackFunc(other.m_SIPCallBackFunc)
{
}

SipStackAgentCfg::~SipStackAgentCfg ()
{

}

SipStackAgentCfg& SipStackAgentCfg::operator=(const SipStackAgentCfg& other) 
{
    if (this == &other)
    {
        return *this;
    }
    m_login_ip_str              = other.m_login_ip_str;
    m_login_port_str            = other.m_login_port_str;
    m_agent_user_name_str        = other.m_agent_user_name_str;
    m_agent_user_version_str     = other.m_agent_user_version_str;
    m_login_name_str            = other.m_login_name_str;
    m_login_pwd_str             = other.m_login_pwd_str;
    m_login_domain_str          = other.m_login_domain_str;
    m_local_ip_str              = other.m_local_ip_str;
    m_local_port_str            = other.m_local_port_str;
	m_ServerID				    = other.m_ServerID;
	m_LocalID					= other.m_LocalID;
	m_server_sipname			= other.m_server_sipname;
	m_server_sippwd				= other.m_server_sippwd;
	m_SIPCallBackFunc			= other.m_SIPCallBackFunc;

    return *this;
}

CSipVar::CSipVar ()
    :m_strRequestUriUserInfo("")
    ,m_strRequestUriDomain("")
    ,m_strAgentName("")
    ,m_strCallID("")
    ,m_strSubject("")
{
}

CSipStackServiceCfg::CSipStackServiceCfg ()
                :m_strServiceIP(SS_SERVICE_CFG_IPV4_DFLT)
                ,m_usServicePort(SS_SERVICE_CFG_PORT_DFLT)
                ,m_strName("")
                ,m_strDomainName("")
                ,m_usLogSwitch(0)
                ,m_unSipMsgTimeout(SIP_STACK_TIMEOUT_VALUE_MESSAGE)
                ,m_uExpires(SIP_STACK_EXPIRES)
{
}
CSipStackServiceCfg::CSipStackServiceCfg(const CSipStackServiceCfg& other )
{
    *this = other;
}
CSipStackServiceCfg::~CSipStackServiceCfg ()
{

}

const std::string& CSipStackServiceCfg::GetServiceIP() const
{
    return m_strServiceIP;
}
unsigned short CSipStackServiceCfg::GetServicePort() const
{
    return m_usServicePort;
}
const std::string& CSipStackServiceCfg::GetName() const
{
    return m_strName;
}
const std::string& CSipStackServiceCfg::GetDomainName() const
{
    return m_strDomainName;
}
unsigned int CSipStackServiceCfg::GetSipMsgTimeout() const
{
    return m_unSipMsgTimeout;
}
unsigned short CSipStackServiceCfg::GetLogSwitch() const
{
    return m_usLogSwitch;
}

unsigned long CSipStackServiceCfg::GetExpires() const
{
    return m_uExpires;
}

void CSipStackServiceCfg::SetServiceIP( const std::string& strServiceIP /*= SS_SERVICE_CFG_IPV4_DFLT */ )
{
    m_strServiceIP = strServiceIP;
}
void CSipStackServiceCfg::SetServicePort( const unsigned short usPort /*= SS_SERVICE_CFG_PORT_DFLT */ )
{
    m_usServicePort = usPort;
}
void CSipStackServiceCfg::SetName( const std::string& strName )
{
    m_strName = strName;
}
void CSipStackServiceCfg::SetDomainName( const std::string& strDomainName )
{
    m_strDomainName = strDomainName;
}
void CSipStackServiceCfg::SetLogSwitch(const unsigned short usSwitchFlag)
{
    m_usLogSwitch = usSwitchFlag;
}

void CSipStackServiceCfg::SetExpires(const unsigned long uExpires)
{
    m_uExpires = uExpires;
}

void CSipStackServiceCfg::SetSipMsgTimeout(const unsigned int unTimeout)
{
    m_unSipMsgTimeout = unTimeout;
}

/****************************************************************************/
//lint -e1712
class CSipStackParseCfg
{
    CSipStackServiceCfg&           m_rServiceCfg;//lint !e1725
    SS_CTRL_MSG_MAP_T::key_type&   m_rnQueueLength;//lint !e1725
    SS_CTRL_MSG_MAP_T&             m_rmapMsg;//lint !e1725

public:
    CSipStackParseCfg ( CSipStackServiceCfg&          rServiceCfg,
                        SS_CTRL_MSG_MAP_T::key_type&  rnQueueLength, 
                        SS_CTRL_MSG_MAP_T&            rmapMsg
                      )
        :m_rServiceCfg(rServiceCfg)
        ,m_rnQueueLength(rnQueueLength)
        ,m_rmapMsg(rmapMsg)
    {
    }

    virtual ~CSipStackParseCfg ()
    {
    }

    bool ParseConfigFile ( const std::string& strFilePath )
    {
        if ( strFilePath.empty() )
        {
            ERROR_LOG("sipstack config mgr - parse config file - invalid file");
            return false;
        }

        TiXmlDocument aXmlDoc;
        if ( !aXmlDoc.LoadFile(strFilePath.c_str()) )
        {
            ERROR_LOG("sipstack config mgr - parse config file - failure to read config file");
            return false;
        }

        TiXmlHandle aXmlHandle(&aXmlDoc);
        TiXmlHandle aXmlHandleSS(aXmlHandle.FirstChild("sipstack"));

        if ( !ParseCfgForService( aXmlHandleSS ) )
        {
            ERROR_LOG("sipstack config mgr - parse config file - failure to parse service config");
            return false;
        }

        if ( !ParseCfgForReliability( aXmlHandleSS ) )
        {
            ERROR_LOG("sipstack config mgr - parse config file - failure to parse reliability config");
            return false;
        }

        INFO_LOG("sipstack config mgr - parse config file - success ");
        return true;
    }

    //服务性配置
    bool ParseCfgForService ( const TiXmlHandle& crXmlHandleSS )
    {
        TiXmlElement* pElem   = NULL;
        const char  * pchTemp = NULL;

        //1.解析队列长度
        TiXmlHandle aXmlHandleSvc = crXmlHandleSS.FirstChild("service");

        if (  NULL != ( pElem = aXmlHandleSvc.FirstChild("ip").ToElement() )
           && NULL != ( pchTemp = pElem->GetText() )
           && '\0' != pchTemp[0]
           )//lint !e838
        {
            m_rServiceCfg.SetServiceIP(pchTemp);
        }

        if (  NULL != ( pElem = aXmlHandleSvc.FirstChild("port").ToElement() )
           && NULL != ( pchTemp = pElem->GetText() )
           && '\0' != pchTemp[0]
           )
        {
            m_rServiceCfg.SetServicePort((unsigned short)atoi(pchTemp));
        }

        if (  NULL != ( pElem = aXmlHandleSvc.FirstChild("name").ToElement() )
           && NULL != ( pchTemp = pElem->GetText() )
           && '\0' != pchTemp[0]
        )
        {
            m_rServiceCfg.SetName(pchTemp);
        }

        if (  NULL != ( pElem = aXmlHandleSvc.FirstChild("domain").ToElement() )
           && NULL != ( pchTemp = pElem->GetText() )
           && '\0' != pchTemp[0]
           )
        {
            m_rServiceCfg.SetDomainName(pchTemp);
        }

        if (  NULL != ( pElem = aXmlHandleSvc.FirstChild("logSwitch").ToElement() )
            && NULL != ( pchTemp = pElem->GetText() )
            && '\0' != pchTemp[0]
        )
        {
            m_rServiceCfg.SetLogSwitch((unsigned short)atoi(pchTemp));
        }

        if (  NULL != ( pElem = aXmlHandleSvc.FirstChild("expires").ToElement() )
            && NULL != ( pchTemp = pElem->GetText() )
            && '\0' != pchTemp[0]
            && atol(pchTemp) > 0
        )
        {
            m_rServiceCfg.SetExpires((unsigned long)atol(pchTemp));
        }

        return true;
    }

    //可靠性配置
    bool ParseCfgForReliability ( const TiXmlHandle& crXmlHandleSS )const
    {
		return true;
    }

    void ParseOneTrigger ( const TiXmlElement* pXmlElement, map< int, int>& mapTriggers )const
    {
        int iID    = 0;
        int iScale = 100;
        if (  ( NULL != pXmlElement )
           && ( NULL != pXmlElement->Attribute("id", &iID) )
           && ( NULL != pXmlElement->Attribute("scale", &iScale) )
           && ( 0 < iID )
           && ( 0 <= iScale && 100 >= iScale )
           )
        {
            mapTriggers[iID] = (int)(m_rnQueueLength * (unsigned int)iScale / 100);
        }
    }
    void ParseOneControllers ( TiXmlElement* pXmlElement, const map<int, int>& mapTriggers )
    {
        if ( NULL == pXmlElement )
        {
            ERROR_LOG("parse one controllers - invalid param");
            return;
        }

        int iID = 0;
        if ( NULL == pXmlElement->Attribute("trigger", &iID) || 0 > iID )
        {
            //无法解析出触发器
            ERROR_LOG("parse one controllers - failure to parse attribute out- Attribute=trigger.");
            return;
        }

        std::map<int, int>::const_iterator iter = mapTriggers.find(iID);
        if ( mapTriggers.end() == iter )
        {
            //未配置此ID的触发器
            ERROR_LOG("parse one controllers - failure to find attribute id - ID=%d",iID);
            return;
        }

        CSipStackCtrlMsg* pCtrlMsg  = NULL;

        const char* pchTemp = pXmlElement->Attribute("method");
        if ( NULL == pchTemp || '\0' == pchTemp[0] )
        {
            //未找到此消息方法名
            ERROR_LOG("parse one controllers - failure to parse attribute out- Attribute=method.");
            return ;
        }
        else if ( 0 == strncmp("REGISTER", pchTemp, strlen("REGISTER")) )
        {
            pCtrlMsg = new CSipStackCtrlMsg(SIP_METHOD_REGISTER, "REGISTER");
            m_rmapMsg[(unsigned int)(iter->second)].push_back(pCtrlMsg);
        }
        else if ( 0 == strncmp("OPTIONS", pchTemp, strlen("OPTIONS")) )
        {
            pCtrlMsg = new CSipStackCtrlMsg(SIP_METHOD_OPTIONS, "OPTIONS");
            m_rmapMsg[(unsigned int)(iter->second)].push_back(pCtrlMsg);
        }
        else if ( 0 == strncmp("INVITE", pchTemp, strlen("INVITE")) )
        {
            pCtrlMsg = new CSipStackCtrlMsg(SIP_METHOD_INVITE, "INVITE");
            m_rmapMsg[(unsigned int)(iter->second)].push_back(pCtrlMsg);
        }
        else if ( 0 == strncmp("ACK", pchTemp, strlen("ACK")) )
        {
            pCtrlMsg = new CSipStackCtrlMsg(SIP_METHOD_ACK, "ACK");
            m_rmapMsg[(unsigned int)(iter->second)].push_back(pCtrlMsg);
        }
        else if ( 0 == strncmp("BYE", pchTemp, strlen("BYE")) )
        {
            pCtrlMsg = new CSipStackCtrlMsg(SIP_METHOD_BYE, "BYE");
            m_rmapMsg[(unsigned int)(iter->second)].push_back(pCtrlMsg);
        }
        else if ( 0 == strncmp("MESSAGE", pchTemp, strlen("MESSAGE")) )
        {
            pCtrlMsg = new CSipStackCtrlMsg(SIP_METHOD_MESSAGE, "MESSAGE");
            CSipStackCtrlMsgBody* pCtrlMsgBody = Parse2CreateCtrlMsgBody(pXmlElement->FirstChildElement("body"));
            pCtrlMsg->SetCtrlMsgBody(pCtrlMsgBody);
            m_rmapMsg[(unsigned int)(iter->second)].push_back(pCtrlMsg);
        }
        else
        {
            WARN_LOG("parse one controllers - unsupported method - Method= %s",pchTemp );
        }
    }//lint !e429
    CSipStackCtrlMsgBody* Parse2CreateCtrlMsgBody ( const TiXmlElement* pXmlElement )const
    {
        if ( NULL == pXmlElement )
        {
            ERROR_LOG("parse one body - invalid param");
            return NULL;
        }

        const char* pchTemp = pXmlElement->Attribute("format");
        if ( NULL == pchTemp || '\0' == pchTemp[0] )
        {
            //未找到此消息方法名
            ERROR_LOG("parse one controllers - failure to parse attribute out- Attribute=body.");
            return NULL;
        }
        else if ( 0 == strncmp("XML", pchTemp, strlen("XML")) )
        {
            //消息为XML格式的
            const char* pchElem = pXmlElement->Attribute("element");
            MACRO_CHECK_SZ_POINTER_RETURN(pchElem, NULL);
            const char* pchAttr = pXmlElement->Attribute("attribute");
            pchAttr = ( NULL == pchAttr ) ? "" : pchAttr;
            const char* pchValue = pXmlElement->GetText();
            MACRO_CHECK_SZ_POINTER_RETURN(pchValue, NULL);

            CSipStackCtrlMsgBodyXML* pCtrlMsgBody = new CSipStackCtrlMsgBodyXML;
            pCtrlMsgBody->SetElement(pchElem);
            pCtrlMsgBody->SetAttribute(pchAttr);
            pCtrlMsgBody->SetValue(pchValue);

            return pCtrlMsgBody;
        }
        else
        {
            WARN_LOG("parse one controllers - unsupported body - Body=%s", pchTemp);
            return NULL;
        }
    }
};
//lint +e1712
SipStack::CSipStackConfigMgr CSipStackConfigMgr::m_instance;

CSipStackConfigMgr::CSipStackConfigMgr()
{

}
CSipStackConfigMgr::CSipStackConfigMgr( CSipStackConfigMgr& )
{

}
CSipStackConfigMgr::~CSipStackConfigMgr()
{

}

CSipStackConfigMgr& CSipStackConfigMgr::operator=( CSipStackConfigMgr& )
{
    return *this;
}

CSipStackConfigMgr& CSipStackConfigMgr::Instance()
{
    return m_instance;
}
bool CSipStackConfigMgr::LoadConfig( const std::string& strFile )
{
    CSipStackParseCfg aParseCfg( m_ServiceCfg,
                                 SS_CTRL_MSG_MGR_INS_REF.m_nQueueLength,
                                 SS_CTRL_MSG_MGR_INS_REF.m_mapCtrlMsg
                               );
    bool bNice = aParseCfg.ParseConfigFile(strFile);
    if ( !SS_CTRL_MSG_MGR_INS_REF.m_mapCtrlMsg.empty() )
    {
        SS_CTRL_MSG_MGR_INS_REF.m_itCurrentCondition = SS_CTRL_MSG_MGR_INS_REF.m_mapCtrlMsg.begin();
    }

    return bNice;
}

const CSipStackServiceCfg& CSipStackConfigMgr::GetServiceCfg() const
{
    return m_ServiceCfg;
}

}
