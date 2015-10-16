/********************************************************************
filename    :    bp_net_config.h
author      :    w90004666
created     :    2012/9/20
description :	 网元配置信息类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/20 初始版本
*********************************************************************/
#ifndef BP_NET_CONFIG_H
#define BP_NET_CONFIG_H
#include <list>
#include <map>
#include "Thread.h"
#include "bp_namespace.h"
#include "bp_stdint.h"
#include "bp_def.h"


NAMESPACE_BP_BEGIN // namespace begin

class CProtocolDllConfig;
class CNetElementConfig;

typedef std::map<std::string, CProtocolDllConfig>  CProtocolDllConfigMap; 
typedef std::map<std::string, CNetElementConfig> CNetElementConfigMap; 


class EXPORT_DLL CProtocolDllConfig
{
public:
	CProtocolDllConfig(){};
	virtual ~CProtocolDllConfig(){};

	const std::string& GetProtocolName(void) { return this->m_strProtocolName; };
	void SetProtocolName(const std::string& strName) { this->m_strProtocolName = strName; };

	const std::string& GetDllName(void) { return this->m_strDllName; };
	void SetDllName(const std::string& strName) { this->m_strDllName = strName; };

private:
	std::string m_strProtocolName;
	std::string m_strDllName;
};

class EXPORT_DLL CNetClientConfig
{
public:
	CNetClientConfig() : m_uPeerPort ( 0 ), m_uLocalPort ( 0 ) {};
	virtual ~CNetClientConfig(){};

	const std::string& GetPeerIP(void) { return this->m_PeerIP; };
	void SetPeerIP(const std::string& strName) { this->m_PeerIP = strName; };

	const std::string& GetLocalIP(void) { return this->m_LocalIP; };
	void SetLocalIP(const std::string& strName) { this->m_LocalIP = strName; };

	uint32_t GetPeerPort(void) { return this->m_uPeerPort; };
	void SetPeerPort(uint32_t uPort) { this->m_uPeerPort = uPort; };

	uint32_t GetLocalPort(void) { return this->m_uLocalPort; };
	void SetLocalPort(uint32_t uPort) { this->m_uLocalPort = uPort; };

	const std::string& GetNetElementID(void) { return this->m_strNetElementID; };
	void SetNetElementID(const std::string& strID) { this->m_strNetElementID = strID; };

private:
	std::string m_PeerIP;						//对端IP
	uint32_t m_uPeerPort;						//对端Port
	std::string m_LocalIP;						//本机IP
	uint32_t m_uLocalPort;						//本机Port
	std::string m_strNetElementID;				//本机网元ID，给对端使用
};

class EXPORT_DLL CNetServerConfig
{
public:
    CNetServerConfig() : m_uListenPort ( 0 ) {};
    virtual ~CNetServerConfig(){};

    const std::string& GetLocalIP(void) { return this->m_LocalIP; };
    void SetLocalIP(const std::string& strName) { this->m_LocalIP = strName; };

    uint32_t GetListenPort(void) { return this->m_uListenPort; };
    void SetListenPort(uint32_t uPort) { this->m_uListenPort = uPort; };

    const std::string& GetNetElementID(void) { return this->m_strNetElementID; };
    void SetNetElementID(const std::string& strID) { this->m_strNetElementID = strID; };

    void AddClientIP(const std::string& strIP);

    const std::list<std::string>& GetClientIPList() { return this->m_ClientIPList; };

private:
    std::string m_LocalIP;						//本机IP
    uint32_t m_uListenPort;						//本机Port
    std::string m_strNetElementID;				//本机网元ID，给对端使用
    std::list<std::string> m_ClientIPList;		//允许连接的客户端列表
};


class EXPORT_DLL CNetProtocolConfig
{
public:
    CNetProtocolConfig() : m_eTransProtocol ( LTYPE_TCP ), m_uConnectionType ( 0 ),
        m_uHeartbeatInterval ( 10 ), m_uHeartbeatTimeout ( 30 ), m_bSecuritySocket ( false ), m_enAuthType(AUTH_NONE) {};
    virtual ~CNetProtocolConfig(){};

    const std::string& GetProtocolName(void) { return this->m_strProtocolName; };
    void SetProtocolName(const std::string& strName) { this->m_strProtocolName = strName; };

    LINK_TYPE GetTransProtocol(void) { return this->m_eTransProtocol; };
    void SetTransProtocol(const std::string& strProtocol);

    uint32_t GetConnectionType(void) { return this->m_uConnectionType; };
    void SetConnectionType(uint32_t uType);

    uint32_t GetHeartbeatInterval(void) { return this->m_uHeartbeatInterval; };
    void SetHeartbeatInterval(uint32_t uInterval);

    uint32_t GetHeartbeatTimeout(void) { return this->m_uHeartbeatTimeout; };
    void SetHeartbeatTimeout(uint32_t uTimeout);

    bool GetSecuritySocket(void) const { return this->m_bSecuritySocket; }
    void SetSecuritySocket(bool bSecuritySocket);

    const std::string & GetPriPemFileKey(void) const { return this->m_strPriPemFileKey; }
    void SetPriPemFileKey(const std::string & strPriPemFileKey);

    ProtocolAuthType GetAuthType(void) const { return m_enAuthType; }
    void SetAuthType(ProtocolAuthType enAuthType);

    const std::string& GetAuthKey(void) const { return m_strAuthKey; }
    void SetAuthKey(const std::string& strAuthKey);

    const std::list<uint32_t>& GetAuthMessageList() const;
    void AddAuthMessage(uint32_t uiAuthMessage);


private:
    std::string m_strProtocolName;				//协议名称
    LINK_TYPE m_eTransProtocol;				//传输协议
    uint32_t m_uConnectionType;					//连接类型
    uint32_t m_uHeartbeatInterval;				//发心跳间隔
    uint32_t m_uHeartbeatTimeout;				//心跳超时时间

    bool         m_bSecuritySocket;             // 是否需要使用SSL协议传输数据(只支持TCP)
    std::string  m_strPriPemFileKey;            // SSL服务端的私钥文件的key(加密过的)

    ProtocolAuthType m_enAuthType;
    std::string  m_strAuthKey;              
    std::list<uint32_t> m_authMessageList;
};

class EXPORT_DLL CNetElementConfig
{
public:
    CNetElementConfig() : m_uElementType(0), m_bServiceType(false) {};
    virtual ~CNetElementConfig(){};

    const std::string& GetElementName(void) { return this->m_strElementName; };
    void SetElementName(const std::string& strName) { this->m_strElementName = strName; };

    uint32_t GetElementType(void) { return this->m_uElementType; };
    void SetElementType(uint32_t uType) { this->m_uElementType = uType; };

    bool GetServiceType(void) { return this->m_bServiceType; };
    void SetServiceType(const std::string& strType);

    CNetProtocolConfig& GetNetProtocolConfig(void) { return this->m_Protocol; };

    CNetServerConfig& GetNetServerConfig(void) { return this->m_ServerConfig; };

    std::list<CNetClientConfig>& GetNetClientConfigList(void) { return this->m_ClientConfigList; };

private:
    std::string m_strElementName;               //网元名称
    uint32_t m_uElementType;                    //网元类型

    CNetProtocolConfig m_Protocol;              //协议配置

    bool m_bServiceType;                        //是否是Server

    CNetServerConfig m_ServerConfig;            //当作为服务器时，服务器配置

    std::list<CNetClientConfig> m_ClientConfigList;			//当作为客户端时，客户端配置表
};


/**
 * @class 通信线程配置类
 * @brief 保存从net.xml中读取的通信线程配置参数。该类不负责创建线程。
 */
class EXPORT_DLL CCommuThreadConfig
{
public:
	CCommuThreadConfig() : m_bConnectDb ( false ), m_uiCmdQueueLength( DEFAULT_CMD_QUEUE_LENGTH * 10 ){};
	virtual ~CCommuThreadConfig(){};

	/**************************************************************************
    * name       : IsConnectDatabase
    * description: 判断此线程是否需要访问数据库
    * input      : NA 
    * output     : NA
    * return     : true - 需要访问数据库, false - 不需要访问数据库
    * remark     : NA    
    **************************************************************************/
    bool IsConnectDatabase() const { return this->m_bConnectDb; };
    void SetConnectDatabase(const bool bConnectDb) { this->m_bConnectDb = bConnectDb; };

    const std::string& GetThreadName(void) { return this->m_strThreadName; };
    void SetThreadName(const std::string& strName) { this->m_strThreadName = strName; };

    unsigned int GetCmdQueueLength(){return m_uiCmdQueueLength;};
    void SetCmdQueueLength(unsigned int uiCmdQueueLength){m_uiCmdQueueLength = uiCmdQueueLength;};

    std::list<std::string>& GetNetElementList(void) { return this->m_NetElementList; };

    void AddNetElement(const std::string& strName);

private:
    bool m_bConnectDb;                              //是否创建数据库连接
    std::string m_strThreadName;                    //线程名称
    unsigned int m_uiCmdQueueLength;                //线程消息队列长度
    std::list<std::string> m_NetElementList;        //与线程绑定的网元列表
};

NAMESPACE_BP_END   // namespace end
#endif //BP_NET_CONFIG_H
