// sipsdk.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "sipsdk.h"
#include "Log.h"
#include "UnhandledExceptionFilter.h"

#ifdef WIN32
#pragma comment(linker, "/EXPORT:SIP_SDK_Init=_SIP_SDK_Init@44")
#pragma comment(linker, "/EXPORT:SIP_SDK_UnInit=_SIP_SDK_UnInit@0")
#pragma comment(linker, "/EXPORT:SIP_SDK_REGISTER=_SIP_SDK_REGISTER@0")
#pragma comment(linker, "/EXPORT:SIP_SDK_INVITE=_SIP_SDK_INVITE@16")
#pragma comment(linker, "/EXPORT:SIP_SDK_MESSAGE=_SIP_SDK_MESSAGE@8")
#pragma comment(linker, "/EXPORT:SIP_SDK_BYE=_SIP_SDK_BYE@4")
#pragma comment(linker, "/EXPORT:SIP_SDK_Subscribe=_SIP_SDK_Subscribe@8")
#endif

using namespace SipStack;
//全局初始化标志
static bool g_isInit = false;
//全局注册标志
static bool g_isReg	= false;
//sip协议栈配置路径文件
const char * SIPSTACK_CFG_FILE_PATH = "\\config\\sipstack_config.xml";
//全局协议栈配置，保存用户名和本地ip。服务器ip端口等信息
static SipStack::SipStackAgentCfg g_AgentCfg;
//全局回调函数
static SIP_CallBack g_SIPCB_Func = NULL;

/*
 *	判断指针是否为空 
 */
#define CHECK_POINTER( pPointer, retValue )			\
if ( NULL == ( pPointer ) )							\
{													\
	std::string strContent("#pPointer");			\
	strContent.append(" is Null Point.");			\
	WARN_LOG(strContent.c_str()); 					\
    return retValue;								\
}													
	
/*
 *	回调中转函数
 */
int __stdcall SIP_CallBack_Adapter(int msgType,int eventType,const char* pPara)
{
	if(NULL == g_SIPCB_Func)
	{
		return SIP_RET_FAILURE;
	}

	CHECK_POINTER(pPara,SIP_RET_FAILURE);

	std::string strPara(pPara);

	INFO_LOG("msgType=%d,eventType=%d,pPara=%s",msgType,eventType,pPara);

	return g_SIPCB_Func(msgType,eventType,strPara.c_str());

}

int __SIP_SDK_CALL SIP_SDK_Init(const char* pLocalSipAccount
	,const char* pLocalSipPasswd
	,const char* pLocalID
	,const char* pLocalIP
	,int iLocalPort
	,const char* pServerSipAccount
	,const char* pServerSipPasswd
	,const char* pServerID
	,const char* pServerIP
	,int iServerPort
	,SIP_CallBack pCallBackFunc)
{	
	SetUnhandledExceptionFilter(GPTUnhandledExceptionFilter);

	std::string path;
	eSDKTool::getCurrentPath(path);
	std::string strLogCfgFile = path+"\\eSDKClientLogCfg.ini";
	std::string strLogStorePath = path+"\\siplog";
	unsigned int logLevel[LOG_CATEGORY] = {INVALID_LOG_LEVEL,INVALID_LOG_LEVEL,INVALID_LOG_LEVEL};
	LogInit(PRODUCT_NAME,strLogCfgFile.c_str(),logLevel,strLogStorePath.c_str());


	INFO_TRACE("");

	if(g_isInit)
	{
		WARN_LOG("Init too many times.");
		return SIPSDK_RET_HAS_INIT;
	}

	CHECK_POINTER(pLocalSipAccount,SIP_RET_FAILURE);
	CHECK_POINTER(pLocalSipPasswd,SIP_RET_FAILURE);
	CHECK_POINTER(pLocalID,SIP_RET_FAILURE);
	CHECK_POINTER(pLocalIP,SIP_RET_FAILURE);
	CHECK_POINTER(pServerSipAccount,SIP_RET_FAILURE);
	CHECK_POINTER(pServerSipPasswd,SIP_RET_FAILURE);
	CHECK_POINTER(pServerID,SIP_RET_FAILURE);
	CHECK_POINTER(pServerIP,SIP_RET_FAILURE);
	CHECK_POINTER(pCallBackFunc,SIP_RET_FAILURE);

	INFO_LOG("pLocalSipAccount=%s,pLocalID=%s,pLocalIP=%s,iLocalPort=%d",pLocalSipAccount,pLocalID,pLocalIP,iLocalPort);
	INFO_LOG("pServerSipAccount=%s,pServerID=%s,pServerIP=%s,iServerPort=%d",pServerSipAccount,pServerID,pServerIP,iServerPort);

	g_SIPCB_Func = pCallBackFunc;

	ACE::init();

	g_AgentCfg.m_login_name_str = pLocalSipAccount;
	g_AgentCfg.m_login_pwd_str = pLocalSipPasswd;
	g_AgentCfg.m_local_ip_str = pLocalIP;

	std::string strLoaclPort;
	eSDKTool::Int10ToStr(strLoaclPort,iLocalPort);
	g_AgentCfg.m_local_port_str = strLoaclPort;

	g_AgentCfg.m_login_ip_str = pServerIP;
	std::string strServerPort;
	eSDKTool::Int10ToStr(strServerPort,iServerPort);
	g_AgentCfg.m_login_port_str = strServerPort;

	g_AgentCfg.m_login_domain_str = "";
	g_AgentCfg.m_login_domain_str.append(g_AgentCfg.m_login_ip_str);
	g_AgentCfg.m_login_domain_str.append(":");
	g_AgentCfg.m_login_domain_str.append(strServerPort);
	g_AgentCfg.m_ServerID = pServerID;
	g_AgentCfg.m_LocalID  = pLocalID;
	g_AgentCfg.m_SIPCallBackFunc = SIP_CallBack_Adapter;

	g_AgentCfg.m_agent_user_name_str = "eSDK TP";
	g_AgentCfg.m_agent_user_version_str = "V100R005C10";

	g_AgentCfg.m_server_sipname = pServerSipAccount;
	g_AgentCfg.m_server_sippwd = pServerSipPasswd;

	if(g_AgentCfg.m_LocalID.empty() || g_AgentCfg.m_ServerID.empty() || g_AgentCfg.m_local_ip_str.empty()
		|| g_AgentCfg.m_local_port_str.empty() || g_AgentCfg.m_login_port_str.empty())
	{
		ERROR_LOG("SipStack Init Failed.");
		ACE::fini();		
		LogFini(PRODUCT_NAME);
		return SIPSDK_RET_INVALID_PARAM;
	}

	unsigned long nMaxQueueNum = std::numeric_limits<unsigned long>::max();
	path.append(SIPSTACK_CFG_FILE_PATH);
	bool bRet = SipStack::GetAgentMgr().Init(g_AgentCfg, nMaxQueueNum, path);
	if(!bRet)
	{
		//初始化失败后去初始化
		SipStack::GetAgentMgr().Fini();
		ACE::fini();
		ERROR_LOG("SipStack Init Failed.");
		LogFini(PRODUCT_NAME);
		return SIPSDK_RET_FAILURE;
	}	

	g_isInit = true;

	return SIPSDK_RET_SUCCESS;
}

int __SIP_SDK_CALL SIP_SDK_UnInit(void)
{
	{	
		INFO_TRACE("");

		if(!g_isInit)
		{
			WARN_LOG("Not Init.");
			return SIPSDK_RET_NOT_INIT;
		}

		INFO_LOG("");

		SipStack::GetAgentMgr().Fini();
		int iRet = ACE::fini();
		if(-1 == iRet)
		{
			WARN_LOG("ACE::fini failed.");
		}

		g_isInit = false;
	}
	LogFini(PRODUCT_NAME);
	return SIPSDK_RET_SUCCESS;
}

int __SIP_SDK_CALL SIP_SDK_REGISTER(void)
{
	INFO_TRACE("");

	if(!g_isInit)
	{
		WARN_LOG("Not Init.");
		return SIPSDK_RET_NOT_INIT;
	}

	INFO_LOG("");

	std::string strReqUri = "";
	strReqUri.append(g_AgentCfg.m_ServerID);
	strReqUri.append("@");
	strReqUri.append(g_AgentCfg.m_login_domain_str);
	std::string strServer = g_AgentCfg.m_login_ip_str;
	unsigned short usPort = eSDKTool::StrToInt10(g_AgentCfg.m_login_port_str);
	SipStack::SipStackAgentClient* pSipAgent = SipStack::GetAgentMgr().GetAgentClient();
	
	std::string callID = eSDKTool::CreateGuidStr();
	CHECK_POINTER(pSipAgent,SIP_RET_FAILURE);
	bool bRet =  pSipAgent->SendRegister(strReqUri,strServer,usPort,callID,true);
	if(!bRet)
	{
		ERROR_LOG("Register Failed.");
		return SIPSDK_RET_FAILURE;
	}

	g_isReg = true;
	return SIPSDK_RET_SUCCESS;
}
int __SIP_SDK_CALL SIP_SDK_UNREGISTER(void)
{
	INFO_TRACE("");
	if(!g_isInit)
	{
		WARN_LOG("Not Init.");
		return SIPSDK_RET_NOT_INIT;
	}
	if(!g_isReg)
	{
		WARN_LOG("Not Register.");
		return SIPSDK_RET_NOT_REGISTER;
	}

	INFO_LOG("");

	SipStack::SipStackAgentClient* pSipAgent = SipStack::GetAgentMgr().GetAgentClient();
	CHECK_POINTER(pSipAgent,SIP_RET_FAILURE);

	std::string strReqUri = "";
	strReqUri.append(g_AgentCfg.m_login_name_str);
	strReqUri.append("@");
	strReqUri.append(g_AgentCfg.m_login_domain_str);
	std::string strServer = g_AgentCfg.m_login_ip_str;
	unsigned short usPort = eSDKTool::StrToInt10(g_AgentCfg.m_login_port_str);
	std::string callID = eSDKTool::CreateGuidStr();
	bool bRet =  pSipAgent->SendRegister(strReqUri,strServer,usPort,callID,false);
	g_isReg = false;
	if(!bRet)
	{
		ERROR_LOG("UnRegister Failed.");
		return SIPSDK_RET_FAILURE;
	}

	return SIPSDK_RET_SUCCESS;
}
int __SIP_SDK_CALL SIP_SDK_INVITE(const char* pDevCode,const char* pLocalSdpBody,int& iResponseID,char strRemoteBody[MAX_LENGTH])
{
	INFO_TRACE("");
	if(!g_isInit)
	{
		ERROR_LOG("Not Init.");
		return SIPSDK_RET_NOT_INIT;
	}
	if(!g_isReg)
	{
		WARN_LOG("Not Register.");
		return SIPSDK_RET_NOT_REGISTER;
	}

	CHECK_POINTER(pDevCode,SIP_RET_INVALID_PARAM);
	CHECK_POINTER(pLocalSdpBody,SIP_RET_INVALID_PARAM);

	INFO_LOG("pDevCode=%s,pLocalSdpBody=%s",pDevCode,pLocalSdpBody);

	std::string strDevCode = pDevCode;
	if(strDevCode.empty())
	{
		ERROR_LOG("strDevCode is empty");
		return SIPSDK_RET_INVALID_PARAM;
	}
	std::string strLocalSdp = pLocalSdpBody;
	if(strLocalSdp.empty())
	{
		ERROR_LOG("strLocalSdp is empty");
		return SIPSDK_RET_INVALID_PARAM;
	}	

	SipStack::SipStackAgentClient* pSipAgent = SipStack::GetAgentMgr().GetAgentClient();
	CHECK_POINTER(pSipAgent,SIP_RET_FAILURE);

	strDevCode.append("@");
	strDevCode.append(g_AgentCfg.m_login_domain_str);
	std::string callID = eSDKTool::CreateGuidStr();
	std::string strServer = g_AgentCfg.m_login_ip_str;
	unsigned short usPort = eSDKTool::StrToInt10(g_AgentCfg.m_login_port_str);
	auto_ptr<SipStack::SipStackAgentMsg> pResponeMsg = pSipAgent->SyncSendInviteEx(callID,strLocalSdp,strDevCode,"",strServer,usPort);
	if(NULL == pResponeMsg.get())
	{
		ERROR_LOG("Response is empty");
		return SIPSDK_RET_FAILURE;
	}
	unsigned long ulRetCode = pResponeMsg->GetStatusCode();
	if(SIP_STATUS_SUCC_OK == ulRetCode)
	{
		iResponseID = (int)pResponeMsg->GetResponseID();
		std::string strBody = pResponeMsg->GetBody();
		memset(strRemoteBody,0,MAX_LENGTH);
		strcpy_s(strRemoteBody,MAX_LENGTH-1,strBody.c_str());
		INFO_LOG("iResponseID=%d,strRemoteBody=%s",iResponseID,strRemoteBody);
		return SIPSDK_RET_SUCCESS;
	}
	else
	{
		ERROR_LOG("Response code is %d",ulRetCode);
		return SIPSDK_RET_FAILURE;		
	}
	
}
int __SIP_SDK_CALL SIP_SDK_MESSAGE(const char* pDevCode,const char* pBody)
{
	INFO_TRACE("");
	if(!g_isInit)
	{
		ERROR_LOG("Not Init.");
		return SIPSDK_RET_NOT_INIT;
	}
	if(!g_isReg)
	{
		WARN_LOG("Not Register.");
		return SIPSDK_RET_NOT_REGISTER;
	}
	CHECK_POINTER(pDevCode,SIP_RET_INVALID_PARAM);
	CHECK_POINTER(pBody,SIP_RET_INVALID_PARAM);
	INFO_LOG("pDevCode=%s",pDevCode);
	INFO_LOG("pBody=%s",pBody);

	std::string strDevCode = pDevCode;
	if(strDevCode.empty())
	{
		ERROR_LOG("strDevCode is empty");
		return SIPSDK_RET_INVALID_PARAM;
	}
	std::string strBody = pBody;
	if(strBody.empty())
	{
		ERROR_LOG("strBody is empty.");
		return SIPSDK_RET_INVALID_PARAM;
	}

	SipStack::SipStackAgentClient* pSipAgent = SipStack::GetAgentMgr().GetAgentClient();
	CHECK_POINTER(pSipAgent,SIP_RET_FAILURE);
	std::string strServer = g_AgentCfg.m_login_ip_str;
	unsigned short usPort = eSDKTool::StrToInt10(g_AgentCfg.m_login_port_str);
	std::string strAccount = strDevCode;
	strAccount.append("@");
	strAccount.append(g_AgentCfg.m_login_domain_str);
	std::string callID = eSDKTool::CreateGuidStr();
	auto_ptr<SipStack::SipStackAgentMsg> pResponeMsg = pSipAgent->SyncSendMsgEx(strBody,strAccount,strServer,usPort,callID);
	if(NULL == pResponeMsg.get())
	{
		ERROR_LOG("Response is empty");
		return SIPSDK_RET_FAILURE;
	}
	unsigned long ulRetCode = pResponeMsg->GetStatusCode();
	if(SIP_STATUS_SUCC_OK == ulRetCode)
	{
		std::string strBody = pResponeMsg->GetBody();
		INFO_LOG("strResponseBody=%s",strBody.c_str());
		return SIPSDK_RET_SUCCESS;
	}
	else
	{
		ERROR_LOG("Response code is %d",ulRetCode);
		return SIPSDK_RET_FAILURE;		
	}
}

int __SIP_SDK_CALL SIP_SDK_BYE(int iResponseID)
{
	INFO_TRACE("");
	if(!g_isInit)
	{
		ERROR_LOG("Not Init.");
		return SIPSDK_RET_NOT_INIT;
	}
	if(!g_isReg)
	{
		WARN_LOG("Not Register.");
		return SIPSDK_RET_NOT_REGISTER;
	}

	INFO_LOG("iResponseID=%d",iResponseID);

	SipStack::SipStackAgentClient* pSipAgent = SipStack::GetAgentMgr().GetAgentClient();
	CHECK_POINTER(pSipAgent,SIP_RET_FAILURE);
	std::string strServer = g_AgentCfg.m_login_ip_str;
	unsigned short usPort = eSDKTool::StrToInt10(g_AgentCfg.m_login_port_str);
	bool bRet = pSipAgent->SendByeReq((unsigned long)iResponseID,strServer,usPort);
	if(!bRet)
	{
		ERROR_LOG("Bye Failed.");
		return SIPSDK_RET_FAILURE;
	}

	return SIPSDK_RET_SUCCESS;
}

int __SIP_SDK_CALL SIP_SDK_ACK(int iResponseID)
{
	INFO_TRACE("");
	if(!g_isInit)
	{
		ERROR_LOG("Not Init.");
		return SIPSDK_RET_NOT_INIT;
	}
	if(!g_isReg)
	{
		WARN_LOG("Not Register.");
		return SIPSDK_RET_NOT_REGISTER;
	}

	INFO_LOG("iResponseID=%d",iResponseID);

	SipStack::SipStackAgentClient* pSipAgent = SipStack::GetAgentMgr().GetAgentClient();
	CHECK_POINTER(pSipAgent,SIP_RET_FAILURE);
	std::string strServer = g_AgentCfg.m_login_ip_str;
	unsigned short usPort = eSDKTool::StrToInt10(g_AgentCfg.m_login_port_str);
	bool bRet = pSipAgent->AsynSendAck((unsigned long)iResponseID,strServer,usPort);
	if(!bRet)
	{
		ERROR_LOG("Bye Failed.");
		return SIPSDK_RET_FAILURE;
	}

	return SIPSDK_RET_SUCCESS;

}

int __SIP_SDK_CALL SIP_SDK_Subscribe(const char* pRemoteURI,const char* pBody)
{
	INFO_TRACE("");
	if(!g_isInit)
	{
		ERROR_LOG("Not Init.");
		return SIPSDK_RET_NOT_INIT;
	}
	if(!g_isReg)
	{
		WARN_LOG("Not Register.");
		return SIPSDK_RET_NOT_REGISTER;
	}

	CHECK_POINTER(pRemoteURI,SIPSDK_RET_INVALID_PARAM);
	CHECK_POINTER(pBody,SIPSDK_RET_INVALID_PARAM);

	INFO_LOG("pRemoteURI=%s,pBody=%s",pRemoteURI,pBody);

	SipStack::SipStackAgentClient* pSipAgent = SipStack::GetAgentMgr().GetAgentClient();
	CHECK_POINTER(pSipAgent,SIPSDK_RET_FAILURE);

	std::string strServer = g_AgentCfg.m_login_ip_str;
	unsigned short usPort = eSDKTool::StrToInt10(g_AgentCfg.m_login_port_str);
	std::string strRet = pSipAgent->SendSubscribeReq(pBody,pRemoteURI,strServer,usPort);
	if(strRet.empty())
	{
		ERROR_LOG("Subscribe Failed.");
		return SIPSDK_RET_FAILURE;
	}

	return SIPSDK_RET_SUCCESS;
}


