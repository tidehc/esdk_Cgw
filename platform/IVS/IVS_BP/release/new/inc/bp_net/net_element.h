/********************************************************************
filename    :    net_element.h
author      :    j00213308
created     :    2012/04/07
description :    网元管理，网元类的头文件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/04/07 初始版本
*********************************************************************/
#ifndef _net_element_h
#define _net_element_h

#include <map>
#include "ace_header.h"
#include "bp_def.h"
#include <list>

#ifndef WIN32
//Linux的socket头文件
#include <sys/epoll.h>
#else
#define EPOLLIN 0
#define EPOLLOUT 0
#endif

NAMESPACE_BP_BEGIN        // IVS NAMESPACE

// 类型预申明
class CCmd;
class ILink;
class ILinkCreater;
class CNetElementMgr;

// 类型重定义
typedef std::map<std::string, ILink*>            CLinkMap;          // Link对象与ACE socket句柄对应map
typedef CLinkMap::iterator  LinkMapIterator;
typedef std::map<string, ILink*>                 CTransLinkMap;     // Link对象与事务ID(string) map
typedef CTransLinkMap::iterator       TransLinkMapIterator;
typedef ACE_Recursive_Thread_Mutex               RecursiveMutex;

//如果是windows，定义空的结构体
#ifdef WIN32
struct epoll_event
{
};
#endif

//网元类型区分IPV6和IPV4
#define IPV6_MIN_NET_ELEMENT_TYPE 60000
#define IPV4_MIN_NET_ELEMENT_TYPE 40000



// 网元接口
class EXPORT_DLL INetElement
{
public:
    INetElement(void);
    virtual ~INetElement(void);

    /**************************************************************************
    * name       : HandleCmd
    * description: 消息命令处理
    * input      : pCmd : 待处理的消息命令
    * output     : NA
    * return     : 0 - 消息命令已处理, -1 - 消息命令尚未处理
    * remark     : 若已处理消息命令，则无需继续调用其他模块的命令处理方法
    **************************************************************************/
    virtual int HandleCmd(CCmd* pCmd);

    /**************************************************************************
    * name       : Routing
    * description: 列程处理
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual uint32_t Routing(void) = 0;

    /**************************************************************************
    * name       : Startup
    * description: 启动网元
    * input      : NA
    * output     : NA
    * return     : true - 成功, false - 失败
    * remark     : 1. 使用网元前，调用此函数，表示网元的启动
    *              2. 若是TCP客户端网元,必返回true
    *              3. 若是TCP服务器网元,会打开端口
    *              4. 若是UDP网元,必返回true
    **************************************************************************/
    virtual bool Startup(void);

    /**************************************************************************
    * name       : Setdown
    * description: 停止网元
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : 1. 停止网元，同时关闭其内所有连接
    *              2. 若是TCP客户端网元,关闭期内所有连接
    *              3. 若是TCP服务器网元,关闭期内所有连接，且停止查看端口
    *              4. 若是UDP网元,关闭期内所有UDP套接字
    **************************************************************************/
    virtual void Setdown(void);

    // 网元是否是客户端
    virtual bool IsClient(void) = 0;
    
    // 注册Select句柄集
    virtual void RegisterHandle(ACE_Handle_Set& rdHandleSet, ACE_Handle_Set& wtHandleSet, ACE_Handle_Set& exHandleSet) = 0;

    // 检查句柄是否可读
    bool PollReadEvent(ACE_HANDLE handle);

    // 检查句柄是否可写
    bool PollWriteEvent(ACE_HANDLE handle);

    // 检查句柄是否发生异常
    bool PollExpEvent(ACE_HANDLE handle);

    // 注册固定连接句柄到select句柄集
    void RegisterLinkIdHandle();

    // 注册动态创建的连接句柄到select句柄集
    void RegisterTransLinkIdHandle();

    // 获取连接对象
    ILink* GetLink(const std::string& strLinkId);

    // 获取连接对象。返回IP端口相同的连接
    ILink* GetLinkByPeerAddr(const ACE_INET_Addr* peerAddr);

    // 根据LinkId 添加Link对象
    bool AddLink(const std::string& strLinkId, ILink* pLink);

    // 根据LinkId 删除link对象
    ILink* RmvLink(const std::string& strLinkId);

    int UpdateLink(const std::string& strOldLinkId, const std::string& strNewLinkId);

    void SetLinkAuthed(const std::string& strLinkId);

    // 根据事务ID，添加Link对象(动态)
    bool AddLinkByTransId(const string& strTransId, ILink* pLink);

    // 根据事务ID  删除Link对象
    ILink* RmvLinkByTransId(const string& strTransId);

    // 设置网元类型
    void SetType(int iType);

    // 设置网元KeepAlive属性
    void SetKeepAlive(bool bKeepAlive);

    // 获取网元KeepAlive书香
    bool GetKeepAlive(void);

    // 设置网元的连接创建器
    ILinkCreater* GetLinkCreater(void);

    // 设置网元的连接创建器
    void SetLinkCreater(ILinkCreater* plinkCreater);

    // 设置网元本地地址
    void SetLocalAddr(const ACE_INET_Addr & addr);
    
    // 设置网元对端地址
    void SetPeerAddr(const ACE_INET_Addr & addr);

    // 设置网元管理实例地址
    void SetElementManger(CNetElementMgr* pMgr);

    // 获取网元类型
    int  GetType(void) const;

    // 获取网元本地地址
    const ACE_INET_Addr& GetLocalAddr(void) const;

    // 获取网元对端地址
    const ACE_INET_Addr& GetPeerAddr(void) const;

    // 获取本机网元对应的LinkID
    const std::string& GetLocalLinkId(void);

    // 设置本机网元对应的LinkID
    void SetLocalLinkId(const std::string& strLocalLinkId);

    // 获知是否需创建安全套接字
    bool GetSecuritySocket(void) const;

    // 获取epoll的FD
    int GetEpollFD(void);

protected:
    // 处理发送指令
    int HandleSendCmd(CCmd* pCmd);

    // 处理创建连接指令
    int HandleCreateLinkCmd(CCmd* pCmd);

    // 处理创建连接指令
    int HandleDestroyLinkCmd(const CCmd* pCmd);

    // 处理同步创建连接指令
    int HandleCreateLinkSyncCmd(CCmd* pCmd);

    // 处理销毁连接指令
    int HandleDestroyLinkSyncCmd(CCmd* pCmd);

    // 处理连接已鉴权的消息
    int HandleSetLinkAuthedCmd(CCmd* pCmd);

    // 创建连接
    ILink* CreateLink(void);

    // 遍历所有连接的routing
    uint32_t DoLinksRouting(void);

    uint32_t DoLinkIdRouting(void);

    uint32_t DoTransLinkIdRouting(void);

    // 获知SSL服务端的私钥文件的key(加密过的)
    const std::string & GetPriPemFileKey(void) const;

protected:
    CLinkMap        m_mapLink;           // 保存长连接对象
    CTransLinkMap   m_mapTransLink;      // 与transaction相关链接
    
    ACE_Time_Value  m_timeout;           // 网元超时事件
    ACE_INET_Addr   m_localAddr;         // 网元本地地址
    ACE_INET_Addr   m_peerAddr;          // 网元远端地址
    RecursiveMutex  m_lock;              // 同步锁
    ACE_Handle_Set* m_pRdHandleSet;      // 检查网络input事件
    ACE_Handle_Set* m_pWtHandleSet;      // 检查网络output事件
    ACE_Handle_Set* m_pExHandleSet;      // 检查网络exception事件


    CNetElementMgr* m_pElementMgr;       // 网元所属管理器
    ILinkCreater*   m_pLinkCreater;      // 连接创建器

    int             m_iType;             // 网元类型
    bool            m_bKeepAlive;        // 连接方式（长短连接)

    std::string     m_strLocalLinkId;   // 本机网元的id，对端使用，只有当网元为Server时，这个字段才是有效的！！！	
};


NAMESPACE_BP_END

#endif //
