/********************************************************************
filename    :    ILink.h
author      :    j00213308
created     :    2012/04/06
description :    连接对象头文件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/04/06 初始版本
*********************************************************************/
#ifndef ILINK_H
#define ILINK_H

#include <list>
#include "ace_header.h"
#include "NetElementMgr.h"
#include "bp_def.h"
#include "bp_stdint.h"

#include "openssl/ssl.h"

NAMESPACE_BP_BEGIN        // IVS NAMESPACE


// 连接状态
enum enLinkState
{	
	LSTATE_INIT,			// 初始状态
	LSTATE_OPENING,			// 正打开状态
	LSTATE_OPEN,			// 已打开状态
	LSTATE_DELAY,			// 延迟状态
	LSTATE_COMPLETE,		// 完成状态
    LSTATE_TIMEOUT,         // 超时状态（仅适用于动态创建Link）
};


// 发送缓冲
typedef struct
{
	char         * pBuffer;
	unsigned int   uiLen;
    time_t         tTime;
} SEND_BUFFER;


typedef struct 
{
	ILink * pLink;
	INetElement * pTCPServer;
}EPOLL_DATA;

typedef std::list<SEND_BUFFER* >             SendBuffList;
typedef std::list<SEND_BUFFER* >::iterator   SendBuffListIter;

#pragma pack(push, 1)

typedef struct tagLinkStatusReportInfo
{
    enLinkState LinkStatus;  //连接状态，参见enLinkState
    char LinkId[DEVICE_CODE_LEN];
    char DevId[DEVICE_CODE_LEN];        
	char PeerIPAddr[16];
	uint32_t PeerPort;
	char LocalIPAddr[16];
	uint32_t LocalPort;
	uint32_t DevNameLen;
	char DevName[1];
}LINK_STATUS_REPORT_INFO;

#pragma pack(pop)

// 预声明
class CCmd;
class IProtocol;

// 网络连接的基类
class ILink
{
public:
    ILink(void);
    virtual ~ILink(void);

    /**************************************************************************
    * name       : HandleCommand
    * description: 连接的命令处理
    * input      : pCmd    : 命令对象
    * output     : N/A
    * return     : N/A
    * remark     : N/A
    **************************************************************************/
    virtual void HandleCmd(CCmd* pCmd);

    /**************************************************************************
    * name       : Routing
    * description: 列行处理
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual uint32_t Routing(void);

    /**************************************************************************
    * name       : Open
    * description: 打开连接
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : 1. 若为TCP客户端，则与远端建立连接
    *               2. 若为UDP客户端，则打开UDP套接字
    **************************************************************************/
    virtual bool Open(void) = 0;

    /**************************************************************************
    * name       : Close
    * description: 关闭连接
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : 1. 若为TCP客户端，则与远端断开连接
    *               2. 若为UDP客户端，则关闭UDP套接字
    **************************************************************************/
    virtual void Close(void) = 0;

    /**************************************************************************
    * name       : Send
    * description: 发送数据
    * input      : pBuffer : 发送数据
    *              uiLen   : 数据长度
    * output     : NA
    * return     : 发送完毕的数据长度
    * remark     : 1. 把发送数据加入发送缓存队列
    *              2. 参数异常则返回0
    **************************************************************************/
    virtual int Send(const char* pBuffer, unsigned int uiLen, bool bIsHeartBeatMsg = false);

    /**************************************************************************
    * name       : Recv
    * description: 接收数据
    * input      : pBuffer : 存放接收数据的缓存
    *              uiLen   : 缓存长度
    * output     : NA
    * return     : 接收到的数据长度
    * remark     : 1. 接收过程，连接断开，则会通知协议栈对象连接断开
    *               2. 正常接收，则返回接收到的数据长度
    *              3. 参数异常则返回0
    **************************************************************************/
    virtual int Recv(char* pBuffer, unsigned int uiLen) = 0;

    /**************************************************************************
    * name       : GetId
    * description: 获取Link的ID
    * input      : NA
    * output     : NA
    * return     : Link的ID
    * remark     : 务必在连接建立后再调用本函数
    **************************************************************************/
    virtual const std::string& GetId(void);

    /**************************************************************************
    * name       : GetHandle
    * description: 获取套接字句柄
    * input      : NA
    * output     : NA
    * return     : 套接字句柄
    * remark     : 务必在连接建立后再调用本函数
    **************************************************************************/
    virtual ACE_HANDLE GetHandle(void) = 0;

	// 获取本地地址
	virtual string GetLocalIP(void) = 0;

    // 处理连接的Init状态
	void HandleStateInit(void);

	// 处理连接的Opening状态
	void HandleStateOpening(void);

	// 处理连接的Open状态
	uint32_t HandleStateOpen(void);

	// 处理连接的Delay状态
	void HandleStateDelay(void);

	// 处理打开事件
	virtual void OnOpen(void) {};

	// 处理关闭事件
	virtual void OnClose(void){};

	// 处理读取事件
	virtual uint32_t OnReadEvent(void);

	// 处理可发送事件
	virtual uint32_t OnSendEvent(void);

	// 处理写事件
	virtual void OnWriteEvent(void){};

	// 处理异常事件
	virtual void OnExpEvent(void){};

	// 通知上层建立连接
	bool NotifyCreateLink(void);

	// 通知上层建立连接
	bool NotifyCreateLinkSync(const CCmd * pSrcCmd, bool bSuccess);

	static bool NotifyDestoryLinkSync(const CCmd* pSrcCmd, bool bSuccess);

    // 测试Link 是否打开状态
    inline bool IsOpen(void) { return m_iState == LSTATE_OPEN; }

    // 测试Link是否为客户端
    inline bool IsClient(void) { return m_pElement->IsClient(); }

    // 测试Link是否结束状态
	inline bool IsCompleted(void) { return m_iState == LSTATE_COMPLETE; }

    // 测试是否动态创建的Link
	inline bool IsTransLink(void) { return m_bTransLink; }

    // 设置LinkID
    inline void SetId(const std::string& strId) { m_strLinkId = strId; }

    // 设置Link绑定的协议
    inline void SetProtocol(IProtocol* pProtocol) { m_pProtocol = pProtocol; }

    // 设置Link本地地址
    inline void SetLocalAddr(const ACE_INET_Addr& addr) { m_localAddr.set(addr); }

    // 设置Link对端地址
    inline void SetPeerAddr(const ACE_INET_Addr& addr) { m_peerAddr.set(addr); }

    // 设置句柄集
    inline void SetHandleSet(ACE_Handle_Set* pHandleSet) { m_pHandleSet = pHandleSet; }

	// 获取Link关联的网元
	inline INetElement* GetElement(void) { return m_pElement; }

	// 设置Link关联的网元
    inline void SetElement(INetElement* pNetElement) { m_pElement = pNetElement; }

    // 设置Link的事务ID
    inline void SetTransId(const string& strTransId) { m_strTransId = strTransId; }

    // 设置Link 动态创建属性
	inline void SetTransLink(bool bTransLink) { m_bTransLink = bTransLink; }

    // 获取Link关联的协议接口
    inline IProtocol* GetProtocol(void) const { return m_pProtocol; }

    // 获取Link关联的本地地址
    inline ACE_INET_Addr GetLocalAddr(void) const { return m_localAddr; }

    // 获取Link关联的对端地址
    inline ACE_INET_Addr GetPeerAddr(void) const { return m_peerAddr; }

    // 获取Link关联的网元类型
    inline int GetElementType(void) const { return m_pElement->GetType(); }

    // 获取Link关联的事务ID
    inline string GetTransId(void) const { return m_strTransId; }

    // 获取Link关联的流式套接字（TCP）对象引用
    inline ACE_SOCK_Stream& GetSockStream(void) { return m_sockStream; }

    // 获取Link关联的数据报套接字（UDP）对象引用
    inline ACE_SOCK_Dgram& GetSockDgram(void) { return m_sockDgram; }

    // 测试 Link当前errno 是否正常
	inline bool IsLinkOk(int iErrNo) { return EAGAIN == iErrNo || EWOULDBLOCK == iErrNo || ETIME == iErrNo || EINTR == iErrNo; }

    // Link是否正在打开
	inline bool IsOpening(void) { return m_iState == LSTATE_OPENING; }

    // 获取发送缓冲的大小
	inline size_t GetSendBuffListSize(void) { return m_listSendBuff.size(); }

	// 获取本机网元对应的LinkID
	inline const std::string& GetLocalLInkId(void) { return this->m_strLocalLinkId; }

	// 设置本机网元对应的LinkID
	inline void SetLocalLInkId(const std::string& strLocalLinkId) { this->m_strLocalLinkId = strLocalLinkId; }

	//查询是否进行ID交换
	inline bool IsNeedSwitchId(void) { return m_bNeedSwitchId; }

	//设置是否进行ID交换
	inline void SetNeedSwitchId(bool bNeedSwitchId) { this->m_bNeedSwitchId = bNeedSwitchId; }

    //设置安全套接字SSL
    inline void SetSSL(SSL * pSSL) { this->m_pSSL = pSSL; }

    //设置创建连接超时时间，单位为秒
    inline void SetTimeout(uint32_t uiTimeout) { this->m_timeout = ((LOOP_RETRY == uiTimeout)? LOOP_RETRY : ( ACE_OS::gettimeofday().sec() + uiTimeout)); }

    //是否异步方式创建连接
    inline bool IsAsyncCreateLink(void) { return m_bAsyncCreateLink; }

    //设置是否异步方式创建连接
    inline void SetAsyncCreateLink(bool bAsyncCreateLink) { m_bAsyncCreateLink = bAsyncCreateLink; }

	// 获取epoll的数据，用于放到epoll的ev里，epoll_wait的时候会带出来
	EPOLL_DATA* GetEpollData(){return &m_stEpollData;};

	// 设置epoll的结果
	void SetEpollResult(int iEvent);

    inline void sockStreamClose() { if (m_sockStream.close()!=0) { BP_RUN_LOG_WAR("tcp socket close exception.","errno=%d", errno); } }

    inline void sockDgramClose() { if (m_sockDgram.close()!=0) { BP_RUN_LOG_WAR("udp socket close exception.","errno=%d", errno); } }

    void SetLinkAuthed();

protected:
    // 更新Link时间
	void UpdateTimeStamp(void);

    // 释放发送缓存
	void ReleaseSendBuff(void);

    // 执行发送
	uint32_t DoSend(void);

	virtual int ExcuteSend(char* pData, unsigned int uiLen) = 0;
	
	// epoll事件
	void RegisterEpollAdd();
	void RegisterEpollMod(int events);
	void DeleteEpoll();

protected:
	bool			m_bNeedSwitchId;	//是否用对端ID更新本端所持有的连接ID

	bool            m_bTransLink;	    // 事务ID，用于动态创建link
	int             m_iState;           // 连接当前的状态
	time_t          m_timeStamp;        // 连接时间戳

	//WTK：这里的linkId类型根据机器位数变化，32位时为4字节，64位时为8字节，等于指针的长度
    std::string     m_strLinkId;        // 对端网元返回的id，本机使用
    std::string     m_strLocalLinkId;   // 本机网元的id，对端使用
    ACE_Handle_Set* m_pHandleSet;       // 检查网络IO的句柄集
    ACE_SOCK_Stream m_sockStream;       // TCP流对象
    ACE_SOCK_Dgram  m_sockDgram;        // UDP对象
    ACE_INET_Addr   m_localAddr;        // TCP,UDP客户端本地地址
	std::string     m_strLocalAddr;     // 字符串格式，用于打印
    ACE_INET_Addr   m_peerAddr;         // TCP,UDP远端地址
	std::string     m_strPeerAddr;      // 字符串格式，用于打印

    IProtocol*      m_pProtocol;        // 协议栈对象指针
    INetElement*    m_pElement;         // 连接所属网元对象
    
    string          m_strTransId;       // 连接正处理的事物ID

	SendBuffList    m_listSendBuff;		// 发送缓存队列
	unsigned int    m_uiSendOffset;		// 已发送数据长度
	int             m_iPerPkgCnt;	    // 单次发送的数据包个数

    SSL           * m_pSSL;             // 安全套接字 Security Socket
    time_t          m_timeout;          // 创建连接超时时间，单位为秒
    bool            m_bAsyncCreateLink; // 是否异步方式创建连接，默认为true，若为false，则表示采用同步方式创建连接

	EPOLL_DATA      m_stEpollData;      // 用于epoll的数据
	bool            m_bIsReadable;      // socket是否有可读数据
	bool            m_bIsWriteable;     // socket是否有可写数据
	bool            m_bIsException;     // socket是否有异常数据

    unsigned long   m_sockErrCount;       // socket连接错误计数，用于防止日志打印太多
};

// 网络连接TCP类
class CTCPLink : public ILink
{
public:
    CTCPLink(void);
    virtual ~CTCPLink(void);

    // 列行处理
    virtual uint32_t Routing(void);

    // 打开连接
    virtual bool Open(void);

    // 关闭连接
    virtual void Close(void);
   
    // 接收数据
    virtual int Recv(char* pBuffer, unsigned int uiLen);

	// 处理打开事件
	virtual void OnOpen(void);

	// 处理关闭事件	
	virtual void OnClose(void);

	// 处理可写事件
	virtual void OnWriteEvent(void);

	// 处理异常事件
	virtual void OnExpEvent(void);

	// 获取连接句柄
	virtual ACE_HANDLE GetHandle(void);

    // 获取本地Ip地址
	virtual string GetLocalIP(void);

protected:
	virtual int ExcuteSend(char* pData, unsigned int uiLen);

private:
	ACE_SOCK_Connector m_sockConnector;

private:
    bool InitClientSSLCtx();
    SSL * CreateClientSSL();
    bool ClientSSLConnect();

protected:
    // SSL协议client所需
    SSL_METHOD * m_pClientSSLMethod;    // SSL协议中使用的加解密、摘要计算等方法
    SSL_CTX    * m_pClientSSLCtx;       // SSL上下文信息
    bool         m_bInitClientSSLCtx;   // 是否成功初始化SSL上下文
    bool         m_bHasTryToInitCtx;    // 是否尝试过去初始化SSL上下文
};

// 网络连接UDP类
class CUDPLink : public ILink
{
public:
    CUDPLink(void);
    virtual ~CUDPLink(void);

    // 列行处理
    virtual uint32_t Routing(void);

    // 打开连接
    virtual bool Open(void);
    
    // 关闭连接
    virtual void Close(void);

    // 接收数据
    virtual int Recv(char* pBuffer, unsigned int uiLen);

	// 处理打开事件
	virtual void OnOpen(void);

	// 处理关闭事件	
	virtual void OnClose(void);

	// 获取连接句柄
	virtual ACE_HANDLE GetHandle(void);

    // 获取连接本地地址
	virtual string GetLocalIP(void);

protected:
	virtual int ExcuteSend(char* pData, unsigned int uiLen);
};

NAMESPACE_BP_END

#endif // ILINK_H
