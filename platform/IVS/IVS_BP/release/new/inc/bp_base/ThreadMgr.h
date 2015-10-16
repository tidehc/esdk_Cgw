/********************************************************************
filename    :    ThreadMgr.h
author      :    s00191067
created     :    2012/03/28
description :    线程管理类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/28 初始版本
*********************************************************************/
#ifndef BP_THREAD_MGR_H
#define BP_THREAD_MGR_H
#include <map>
#include "Thread.h"
#include "bp_namespace.h"

NAMESPACE_BP_BEGIN // namespace begin
#define DEFAULT_MUTEX_TIMEOUT 3

#ifndef WIN32
#define MUTEX_THREAD_REACTION(TIMEOUT_SEC, RETURN)               \
    ACE_Time_Value tv = ACE_OS::gettimeofday();                  \
    tv.sec(tv.sec() + (TIMEOUT_SEC));                            \
    if ( BP_FAIL == m_mutexThread.acquire(tv) )                  \
    {                                                            \
        BP_RUN_LOG_WAR("in ThreadMgr", "acquire mutex timeout"); \
        RETURN;                                                  \
    }

#else
#define MUTEX_THREAD_REACTION(TIMEOUT_SEC, RETURN)               \
    if ( BP_FAIL == m_mutexThread.acquire() )                    \
    {                                                            \
        BP_RUN_LOG_WAR("in ThreadMgr", "acquire mutex fail");    \
        RETURN;                                                  \
    }
#endif

#define MUTEX_THREAD_ACQUIRE_RETURN(TIMEOUT_SEC, RETURN)         \
    MUTEX_THREAD_REACTION(TIMEOUT_SEC, return RETURN)

#define MUTEX_THREAD_ACQUIRE(TIMEOUT_SEC)                        \
    MUTEX_THREAD_REACTION(TIMEOUT_SEC, return)

#define MUTEX_THREAD_RELEASE()                                   \
    m_mutexThread.release();

typedef std::map<ACE_thread_t, CThread*> CThreadMap;
typedef CThreadMap::iterator             ThreadMapIter;
class EXPORT_DLL CThreadMgr
{
public:
    virtual ~CThreadMgr(void);

    static CThreadMgr& instance();

    void close();

    /**************************************************************************
    * name       : AddThread
    * description: 向线程管理类中增加线程
    * input      : pThread 线程对象指针
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : 注意：主线程不能加入到ThreadMgr中管理，因为ThreadMgr析构时会
                   将所有注册的线程销毁，而主线程需要单独销毁
    **************************************************************************/
    int AddThread(CThread* pThread);

    /**************************************************************************
    * name       : DelThread
    * description: 删除线程
    * input      : threadID 线程ID
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
	int DelThread(ACE_thread_t threadID);

    /**************************************************************************
    * name       : GetThreadMap
    * description: 获取线程map表
    * input      : NA
    * output     : NA
    * return     : CThreadMap* 线程map表
    * remark     : NA
    **************************************************************************/
    CThreadMap* GetThreadMap();

	/**************************************************************************
    * name       : GetThread
    * description: 获取线程
    * input      : threadID
    * output     : NA
    * return     : CThread* 线程
    * remark     : NA
    **************************************************************************/
    CThread* GetThread(ACE_thread_t threadID);

	/**************************************************************************
    * name       : GetThreadByName
    * description: 获取线程
    * input      : strThreadName
    * output     : NA
    * return     : CThread* 线程
    * remark     : NA
    **************************************************************************/
    CThread* GetThreadByName(std::string strThreadName);


    /**************************************************************************
    * name       : AddCommuThread
    * description: 注册网元通信线程
    * input      : pThread 网元线程对象指针
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void AddCommuThread(CThread* pThread);

    /**************************************************************************
    * name       : Notify
    * description: 根据业务线程ID，向业务线程中增加命令消息
    * input      : threadID 线程ID pCmd命令对象
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : 将消息播放给业务线程
    **************************************************************************/
    int Notify(ACE_thread_t threadID, CCmd* pCmd);

    /**************************************************************************
    * name       : NotifyEx
    * description: 根据业务线程ID，向业务线程中增加命令消息
    * input      : threadID 线程ID pCmd命令对象, lVal 私有值
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : 将消息播放给业务线程
    **************************************************************************/
    int NotifyEx(ACE_thread_t threadID, CCmd* pCmd, long lVal);

    /**************************************************************************
    * name       : Notify2CommuThread
    * description: 播发到网元通信线程
    * input      : cmd 命令对象
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : 将消息播发给网元线程
    **************************************************************************/
    //int Notify2CommuThread(CCmd* pCmd);

    /**************************************************************************
    * name       : NotifyCommu
    * description: 播发到网元通信线程
    * input      : cmdType   : 命令类型
	*              netType   : 命令对象
	*              ulLinkId  : 连接号
    *              strTransID: 事务id
	*              pPkgData  : 数据
	*              iDataLen  : 数据长度
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : 将消息播发给网元线程
    **************************************************************************/
	//int NotifyCommu(enCmdType cmdType, enNetElemType netType, unsigned long ulLinkId, const std::string& strTransID, const char* pPkgData, unsigned int iDataLen);

    /**************************************************************************
    * name       : GetPayLoad
    * description: 获取指定线程负载权值
    * input      : threadID 线程ID
    * output     : iPayLoad 线程负载权值
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 线程不存在
    * remark     : 非网元线程
    **************************************************************************/
    int GetPayLoad(ACE_thread_t threadID, unsigned int& iPayLoad);
	void PrintSessionStatInfo();
	void PrintCmdQueueStatInfo();
private:
    CThreadMgr(void);

private:
	static CThreadMgr m_Instance;

    CThreadMap              m_mapThread;

    ACE_Recursive_Thread_Mutex        m_mutexThread;      // 线程锁
};

NAMESPACE_BP_END   // namespace end
#endif  //BP_THREAD_MGR_H
