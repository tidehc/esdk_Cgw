/********************************************************************
filename    :     bp_net_factory
author      :     w90004666
created     :     2012/09/20
description :     动态创建网络连接类
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/09/20 初始版本
*********************************************************************/
#ifndef BP_NET_FACTORY_H_
#define BP_NET_FACTORY_H_
#include "bp_namespace.h"
#include "bp_def.h"
#include "bp_net_config.h"
#include "NetElementMgr.h"
#include "tcp_server.h"


NAMESPACE_BP_BEGIN

#pragma  pack(push)
#pragma  pack(1)
    typedef struct stCreateClientInfo
{
    stCreateClientInfo(void)
    {
        HW_MEMSET(NetElementID, 0, sizeof(NetElementID));
        NetElementType = 0;
        HW_MEMSET(PeerIP, 0, sizeof(PeerIP));
        PeerPort  = 0;
        HW_MEMSET(LocalIP, 0, sizeof(LocalIP));
        NeedSwitchId = true;
        uiTimeout = 30;
    }

    char        NetElementID[NET_ELEMENT_ID_LEGTH];      // 本模块的模块ID，NSS协议网元（SDK除外）必须指定
    int         NetElementType;                          // 网元类型，必须
    char        PeerIP[IP_LENGTH];                       // 远端服务器IP
    int         PeerPort;                                // 远端服务器PORT
    char        LocalIP[IP_LENGTH];                      // 本端IP，可以为NULL
    bool        NeedSwitchId;                            // 是否用对端ID更新本端所持有的连接ID，默认为true，SDK需要指定为false
    uint32_t    uiTimeout;                               // 创建连接超时时间，单位为秒，默认为30秒, 若取值为0，则表示无超时时间限制
} CREATE_CLIENT_INFO;

#pragma pack(pop)

class EXPORT_DLL CNetFactory
{
public:
	virtual ~CNetFactory(void) throw();
	
	/**************************************************************************
	 * name       : Instance
	 * description: 单例接口
	 * output     : 
	 * return     : CNetFactory&
	 * remark     :               
	 **************************************************************************/
	static CNetFactory& Instance();

	/**************************************************************************
	 * name       : CreateClientLink
	 * description: 基于某个已创建的网元，动态创建连接
	 * input      : std::string & strNetElementID：本模块的模块ID，NSS协议网元（SDK除外）必须指定
	 * input      : int iNetElementType：网元类型，必须
	 * input      : const char * pszPeerIP：对端IP
	 * input      : int iPeerPort：对端端口
	 * input      : const char * pszLocalIP：本端IP，可以为NULL
     * input      : bool bNeedSwitchId：是否用对端ID更新本端所持有的连接ID，默认为true，SDK需要指定为false
     * input      : uint32_t uiTimeout：创建连接超时时间，单位为秒，默认为0, 表示无超时时间限制
	 * output     : 
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int CreateClientLink(std::string& strNetElementID, int iNetElementType, const char* pszPeerIP, int iPeerPort, const char* pszLocalIP, bool bNeedSwitchId = true,
                         uint32_t uiTimeout = 0);

    /**************************************************************************
	 * name       : CreateClientLinkSyn
	 * description: 采用同步方式动态创建连接
	 * input      : CREATE_CLIENT_INFO& createClientInfo：创建连接参数信息
	 * output     :
	 * return     : int
     * remark     : 同步创建动态连接时，uiTimeout参数值不允许为0
     **************************************************************************/
    int CreateClientLinkSyn(CREATE_CLIENT_INFO& createClientInfo);

	/**************************************************************************
	 * name       : DeleteClientLink
	 * description: 根据网元类型和连接ID删除指定连接
	 * input      : const std::string & strNetElementID
	 * input      : iNetElementType
	 * output     : 
	 * return     : int
	 * remark     : 
	 **************************************************************************/
	int DeleteClientLink(const std::string& strNetElementID, int iNetElementType);

    int DeleteClientLinkByAddr(const char* pszPeerIP, int iPeerPort, int iNetElementType);
    
	/**************************************************************************
	 * name       : CreateElement
	 * description: 基于网元的基本信息创建网元，只负责生成网元，没有加载到通信线程中运行
					注意：
					1、针对不同Client和Server类的网元，需要提供的网元基本信息不同，信息的格式、范围请参考网元配置文件net.xml
					   Client：需要提供ElementName、ElementType、Protocol、ServiceType，不需要提供ClientList
					   Server：除了上述信息，还需要ListenPort、LocalIP、ElementID，可以不提供鉴权IP列表

	 * input      : CNetElementConfig & ElementConfig
	 * input      : INetElement * & pNetElement
	 * output     : 
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int CreateElement(CNetElementConfig& ElementConfig, INetElement*& pNetElement);

	/**************************************************************************
	 * name       : AddValidatedLink
	 * description: 动态增加做为服务端的网元的合法连接IP列表
	 * input      : std::string& strPeerIP
	 *input	   : int netElemType 网元类型
	 * output     : 
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int AddValidatedLink(const std::string& strPeerIP, int netElemType);

	/**************************************************************************
	 * name       : DelValidatedLink
	 * description: 动态删除做为服务端的网元的合法连接IP列表
	 * input      : std::string& strPeerIP
	 *input	   : int netElemType 网元类型
	 * output     : 
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int DelValidatedLink(const std::string& strPeerIP, int netElemType);

	/**************************************************************************
	 * name       : AddInvalidIP
	 * description: 动态新增做为服务端的网元的非法IP列表（黑名单）
	 * input      : strPeerIP   IP地址
	 *				netElemType 网元类型
	 *              lTimeOut    加入黑名单的时间，-1为永久，单位为s
	 * output     : 
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int AddInvalidIP(const std::string& strPeerIP, int netElemType, long lTimeOut);

		/**************************************************************************
	 * name       : DelValidatedLink
	 * description: 动态删除做为服务端的网元的非法IP列表（黑名单）
	 * input      : strPeerIP   IP地址
	 *              netElemType 网元类型
	 * output     : 
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int DelInValidIP(const std::string& strPeerIP, int netElemType);

    /**************************************************************************
	 * name       : SetLinkAuthed
	 * description: 设置连接已鉴权
	 * input      : const std::string & strNetElementID
	 * input      : iNetElementType
	 * output     : 
	 * return     : void
	 * remark     : 
	 **************************************************************************/
    void SetLinkAuthed(const std::string& strNetElementID, int iNetElementType);

private:
	CNetFactory(void);

	CTCPServer * GetTCPServerByNetElementType(int netElemType);

private:
	static CNetFactory m_Instance;
};

NAMESPACE_BP_END

#endif /* BP_NET_FACTORY_H_ */

