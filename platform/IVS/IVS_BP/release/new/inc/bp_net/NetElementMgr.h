/********************************************************************
filename    :    NetElementMgr.h
author      :    j00213308
created     :    2012/04/07
description :    网元管理，网元类的头文件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/04/07 初始版本
*********************************************************************/
#ifndef NET_ELEMENT_MGR_H
#define NET_ELEMENT_MGR_H

#include "net_element.h"

NAMESPACE_BP_BEGIN        // IVS NAMESPACE

// 类型预申明
class CCmd;
class ILink;
class ILinkCreater;

// 类型重定义
typedef std::map<int, INetElement*>              CNetElementMap;     // 网元类型(int) 与网元对象 map
typedef CNetElementMap::iterator    NetElementMapIterator;

//static const int MAX_SELECT_WAIT_TIME  = 20*1000;            // 套接字select的间隔时间，单位:毫秒


// 网元管理器
class EXPORT_DLL CNetElementMgr
{
public:
    CNetElementMgr(void);
    ~CNetElementMgr(void);

    /**************************************************************************
    * name       : HandleCmd
    * description: 消息命令处理
    * input      : pCmd : 待处理的消息命令
    * output     : NA
    * return     : 0 - 消息已被处理, -1 - 消息尚未被处理
    * remark     : NA
    **************************************************************************/
    int HandleCmd(CCmd* pCmd);

    /**************************************************************************
    * name       : Routing
    * description: 列程处理
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    uint32_t Routing(void);

    /**************************************************************************
    * name       : AddElement
    * description: 保存网元对象
    * input      : iElementType: 网元类型
    *            : pElement    : 保存的网元对象
    * output     : NA
    * return     : true - 成功, false - 失败
    * remark     : 1. 线程安全
    *              2. 不允许加入相同类型的网元
    **************************************************************************/
    bool AddElement(int iElementType, INetElement* pElement);

    /**************************************************************************
    * name       : RmvElement
    * description: 移除网元对象
    * input      : iElementType: 网元类型
    * output     : NA
    * return     : 非NULL - 成功, NULL - 失败
    * remark     : 1. 线程安全
    *              2. 不存在指定的网元类型，则返回失败
    *              3. 返回的指针为被移除的对象指针
    **************************************************************************/
    INetElement* RmvElement(int iElementType);

	/**************************************************************************
    * name       : GetElement
    * description: 根据网元类型获取网元对象指针
    * input      : iElementType: 网元类型
    * output     : NA
    * return     : 非NULL - 成功, NULL - 失败
    * remark     : NA
    **************************************************************************/
    INetElement* GetElement(int iElementType);

	/**************************************************************************
    * name       : GetEpollFD()
    * description: 根据网元类型获取网元对象指针
    * input      : iElementType: 网元类型
    * output     : NA
    * return     : 非NULL - 成功, NULL - 失败
    * remark     : NA
    **************************************************************************/
	int GetEpollFD(void){return m_epollFD;};

private:

    /**************************************************************************
    * name       : DoRouting
    * description: 运行所有网元类型
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    uint32_t DoRouting(void);

    /**************************************************************************
    * name       : WaitForEvent
    * description: 等待注册的套接字句柄上的网络事件发生
    * input      : NA
    * output     : NA
    * return     : 返回本次可读写的文件数，用于后面统计
    * remark     : NA
    **************************************************************************/
    uint32_t WaitForEvent(void);

    /**************************************************************************
    * name       : RegisterHandle
    * description: 注册所有网元内连接对象，以确定是否有网络事件
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void RegisterHandle(void);

    /**************************************************************************
    * name       : UnRegisterHandle
    * description: 注销连接对象
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void DeregisterHandle(void);

	/**************************************************************************
    * name       : EpollEvent
    * description: 检查连接事件
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void EpollEvent();

private:
    ACE_Handle_Set m_rdHandleSet;            // 连接对象网络事件检查用
    ACE_Handle_Set m_wtHandleSet;
    ACE_Handle_Set m_exHandleSet;
    bool           m_bMonitorRead;           // 是否select可读事件
    bool           m_bMonitorWrite;          // 是否select可写事件
    bool           m_bMonitorExp;            // 是否select异常事件

    RecursiveMutex m_lock;                   // 同步锁
    CNetElementMap m_mapElement;             // 保存网元
	
	int            m_epollFD;                // epoll所用的句柄

#ifndef WIN32
	int            m_maxevents;
	struct epoll_event *m_events;
#endif //WIN32

};


NAMESPACE_BP_END

#endif // NET_ELEMENT_MGR_H
