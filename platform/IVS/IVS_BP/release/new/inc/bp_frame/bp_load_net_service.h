/********************************************************************
filename    :    bp_load_net_service.h
author      :    w90004666
created     :    2012/9/20
description :	 加载网元配置，请启动网元通讯
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/20 初始版本
*********************************************************************/
#ifndef BP_LOAD_NET_SERVICE_H
#define BP_LOAD_NET_SERVICE_H

#include "ivs_xml.h"
#include "Thread.h"
#include "bp_namespace.h"
#include "bp_stdint.h"
#include "NetElementMgr.h"
#include "bp_net_config.h"
#include "bp_config_alarmer.h"

#include <map>
#include <list>
#include <string>

NAMESPACE_BP_BEGIN // namespace begin

class CLoadNetServcieByConfig : private CConfigAlarmer
{
public:
	CLoadNetServcieByConfig(void);
	virtual ~CLoadNetServcieByConfig(void) throw();

	/**************************************************************************
	 * name       : LoadService
	 * description: 从配置文件中加载网元通讯服务
	 * input      : const std::string & strConfigPath
	 * output     : NA
	 * return     : bool
	 * remark     :               
	 **************************************************************************/
	bool LoadService(const std::string& strConfigPath);

	bool CreateLinkCreater(CNetElementConfig* pElementConfig, ILinkCreater*& pLinkCreater, bool bUseDefDllName = false);

	bool CreateNetElement(CNetElementConfig* pElementConfig, CNetElementMgr* pMgr, INetElement*& pNetElement);

private:
	//从xml中加载配置信息
	bool LoadConfig(const std::string& strConfigPath);

	bool LoadAllThreadConfig(void);

	bool LoadAllNetElementConfig(void);

	bool LoadAllProtocolDllConfig(void);

	bool LoadNetElementListByThread(CCommuThreadConfig& ThreadConfig);

	bool LoadProtocolByNetElement(CNetProtocolConfig& ProtoclConfig);

    void LoadProtocolAuthByNetElement(CNetProtocolConfig& ProtoclConfig);

    void LoadProtocolAuthMessageByNetElement(CNetProtocolConfig& ProtoclConfig);

	void LoadProtocolInfoByNetElement(CNetProtocolConfig& ProtoclConfig);

	bool LoadClientListByNetElement(std::list<CNetClientConfig>& ClientConfigList);

	bool LoadClientByNetElement(CNetClientConfig& ClientConfig);

	bool LoadServerByNetElement(CNetServerConfig& ServerConfig);

	bool LoadClientIPListByServer(CNetServerConfig& ServerConfig);

	//根据缓存中的配置信息，启动网元通讯
	bool StartService(void);

	bool StartSingleThread(CCommuThreadConfig& ThreadConfig);

	bool AddNetElementToMgr(const std::string& strElementName, CNetElementMgr* pNetManager, CThread* pThread);

	bool CreateClientLink(CNetElementConfig* pElementConfig, INetElement* pNetElement);

	//根据网元名称获取对应的网元配置对象
	CNetElementConfig* GetNetElementConfigByName(const std::string& strName);

private:
    bool                           m_bAlreadyLoad;          // 是否已加载
    CBB::CXml                      m_NetXml;                // 配置文件对象
    std::list<CCommuThreadConfig>  m_ThreadConfigList;      // 线程配置表
    CProtocolDllConfigMap          m_ProtocolDllConfigMap;  // 协议栈配置表
    CNetElementConfigMap           m_NetElementConfigMap;   // 网元配置表
	unsigned int                   m_uiThreadTimeout;       // 线程超时时间
};

NAMESPACE_BP_END   // namespace end
#endif //BP_LOAD_NET_SERVICE_H
