#ifndef _tcp_server_h
#define _tcp_server_h

#include <map>
#include "net_element.h"
#include "ILink.h"
#include "openssl/ssl.h"

NAMESPACE_BP_BEGIN

#define INVALIDIP_FOREVER -1
typedef struct invalidIPInfo
{
	time_t        startTime;
	long lTimeOut;
}INVALID_IP_INFO;

// TCP服务器网元
class EXPORT_DLL CTCPServer : public INetElement
{
public:
    CTCPServer(void);
    virtual ~CTCPServer(void);

    // 命令处理
    virtual int HandleCmd(CCmd* pCmd);

    // 列程处理
    virtual uint32_t Routing(void);    

    // 启动网元
    virtual bool Startup(void);

    // 停止网元
    virtual void Setdown(void);

    // 判断是否客户端
    virtual bool IsClient(void);

    // 注册检查网络事件的IO
    virtual void RegisterHandle(ACE_Handle_Set& rdHandleSet, ACE_Handle_Set& wtHandleSet, ACE_Handle_Set& exHandleSet);

	void SetValidatedList(const std::list<std::string>& ValidateIpList);

	//向白名单中增加IP
	void AddValidatedLink(const std::string & validateIp);

	//删除白名单中的IP
	void DelValidatedLink(const std::string & validateIp);

	//向黑名单中增加IP
	void AddInvalidIP(const std::string & strInvalidIp, long lTimeOut);

	//删除黑名单中的IP
	void DelInValidIP(const std::string & strInvalidIp);

	//设置epoll的结果
	void SetEpollResult(int iEvent);

private:
    // 服务器接入连接处理
    uint32_t DoAccept(void);	

	//检查是否在白名单中
	bool CheckIPValidated(const char* pszClientIP);

	//检查是否在黑名单中
	bool CheckIPInvalid(std::string strClientIP);

private:
    ACE_SOCK_Acceptor m_sockAcceptor;   // 服务器接受连接器

	//ip白名单
	std::list<std::string> m_ValidatedIPList;

	//ip黑名单
	typedef std::map<const std::string, INVALID_IP_INFO*> InvalidIpInfoMap;
	typedef InvalidIpInfoMap::iterator InvalidIpInfoMapIter;

	InvalidIpInfoMap m_invalidIpInfoMap;

    bool InitServerSSLCtx();
    SSL * CreateServerSSL(ACE_HANDLE aAceHandle);
    bool ServerSSLConnect(ILink *& pLink);

    // SSL协议server所需
    SSL_METHOD * m_pServerSSLMethod;    // SSL协议中使用的加解密、摘要计算等方法
    SSL_CTX    * m_pServerSSLCtx;       // SSL上下文信息
    bool         m_bInitServerSSLCtx;   // 是否成功初始化SSL上下文
    bool         m_bHasTryToInitCtx;    // 是否尝试过去初始化SSL上下文

	EPOLL_DATA   m_stEpollData;         // 用于epoll的数据
	bool         m_bIsReadable;         // socket是否有可读数据
	bool         m_bIsException;        // socket是否有异常数据
};

NAMESPACE_BP_END

#endif 
