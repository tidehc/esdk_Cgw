/********************************************************************
filename    :    Thread.h
author      :    s00191067
created     :    2012/03/27
description :    线程封装类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/27 初始版本
*********************************************************************/
#ifndef BP_THREAD_H
#define BP_THREAD_H


#include <map>
#include <queue>
#include "ace_header.h"
#include "IService.h"
#include "bp_namespace.h"
#include "DBInterface.h"
#include "FluxControl.h"

//extern ACE_TSS<IDBConnect* > g_db_conn;

NAMESPACE_BP_BEGIN // namespace begin

typedef struct fluxControlWarnInfo
{
	uint32_t      uiThreadID;               // 线程ID
	bool          bIsFluxControlWarn;       // 是否是流控告警，true是流控产生告警，false是流控恢复告警
	uint32_t      uiLevel;                  // 告警级别
	uint32_t      uiNameLength;             // 线程名称长度
	char          pThreadName[1];           // 线程名称
}FLUX_CONTROL_WARN_INFO;

class EXPORT_DLL CThread : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CThread(void);
    virtual ~CThread(void);
    static void SetMainThread(CThread* pThread);
    static CThread* GetMainThread();

    /**************************************************************************
    * name       : open
    * description: 启动线程，调用ACE的activate启动一个线程
    * input      : args 参数指针，传入线程名称 bIsCreateThread是否创建线程,false不需要创建，直接使用主线程
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
    virtual int open(const void *args, bool bIsCreateThread = true);

    /**************************************************************************
    * name       : svc
    * description: 线程处理函数，循环执行流程
    * input      : args 参数指针，传入线程名称
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : 继承于ACE的Task
    **************************************************************************/
    virtual int svc();

    /**************************************************************************
    * name       : close
    * description: 结束线程，设置标志位为false，通知线程结束循环。
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
    int close();

    /**************************************************************************
    * name       : kill
    * description: 杀死线程
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
    int kill(); 

    /**************************************************************************
    * name       : AddService
    * description: 向线程中增加任务
    * input      : pService 任务
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void AddService(IService* pService); 

    /**************************************************************************
    * name       : DelService
    * description: 删除线程中的任务
    * input      : pService 任务
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    int DelService(IService*& pService);

	    /**************************************************************************
    * name       : FindService
    * description: 通过serviceID查找IService
    * input      : serviceID
    * output     : NA
    * return     : IService*
    * remark     : NA
    **************************************************************************/
    IService* FindService(uint32_t serviceID);

    /**************************************************************************
    * name       : Notify
    * description: 向线程中增加命令消息
    * input      : pCmd 命令消息
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
    int Notify(CCmd* pCmd);

    /**************************************************************************
    * name       : NotifyEx
    * description: 向线程中增加命令消息
    * input      : pCmd 命令消息， lVal 私有数据
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : CThread 类中直接返回失败，由继承类实现
    **************************************************************************/
    virtual int NotifyEx(CCmd* pCmd, long lVal);

    /**************************************************************************
    * name       : GetThreadPayload
    * description: 获取当前线程负载
    * input      : NA
    * output     : NA
    * return     : 当前线程负载权值
    * remark     : 先查找线程中所有service的负载权值，和值为0，则取cmdQueue长度
    **************************************************************************/
    unsigned int GetThreadPayload();

    void SetIdleMode(bool bIdleMode);

    void setTimeout(unsigned int uiTimeout);

    bool IsTimeout(time_t tCurrent);

    void SetThreadName(const std::string& strName);
    std::string GetThreadName();

    ACE_thread_t GetThreadID(void)const;
    ACE_hthread_t GetThreadHandle()const;

    time_t GetUpdateTime(void) const;

    void SetUpdateTime(time_t time);

	void SetCmdQueueLength(unsigned int unQueueMaxLength);
	unsigned int GetCmdQueueLength();

    /**************************************************************************
    * name       : setConnectDatabase
    * description: 设置线程是否创建数据库连接
    * input      : bDbConnection - 是否创建数据库连接的逻辑开关
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void SetConnectDatabase(const bool bDbConnection);//daxie

    ///**************************************************************************
    //* name       : ConnectDatabase
    //* description: 创建数据库连接
    //* input      : NA
    //* output     : NA
    //* return     : BP_OK - 连接成功 BP_FAIL - 连接失败
    //* remark     : NA
    //**************************************************************************/
    //int ConnectDatabase( void ) throw();

    /**************************************************************************
    * name       : ProcessSyncCmd()
    * description: 处理同步命令，目前只处理同步连接, 其他同步命令不要用此接口
    * input      : NA
    * output     : NA
    * return     : BP_OK - 连接成功 BP_FAIL - 连接失败
    * remark     : NA
    **************************************************************************/
    int ProcessSyncCmd(CCmd &cmd);
	uint32_t GetCmdCount();
	uint32_t GetSessionCount();

	/**************************************************************************
    * name       : ProcessSyncCmd()
    * description: 处理同步命令，目前只处理同步连接, 其他同步命令不要用此接口
    * input      : NA
    * output     : NA
    * return     : BP_OK - 连接成功 BP_FAIL - 连接失败
    * remark     : NA
    **************************************************************************/
	void SetIsCommuThread(bool bIsCommuThread){m_bIsCommuThread = bIsCommuThread;};
	bool IsCommuThread(){return m_bIsCommuThread;};

protected:
    /**************************************************************************
    * name       : NotifyUpdateTime
    * description: 刷新最新执行时间
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void NotifyUpdateTime(void);

    /**************************************************************************
    * name       : DestroyCmd
    * description: 销毁命令队列
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void DestroyCmd(void);

    /**************************************************************************
    * name       : ProcessCmd
    * description: 处理命令队列
    * input      : NA
    * output     : NA
    * return     : 处理的命令个数
    * remark     : NA
    **************************************************************************/
    virtual unsigned int ProcessCmd(void);

    /**************************************************************************
    * name       : GetQueueCmd
    * description: 从队列中获取一个命令
    * input      : NA
    * output     : NA
    * return     : CCmd* 返回一个命令
    * remark     : NA
    **************************************************************************/
    CCmd* GetQueueCmd(void);

    /**************************************************************************
    * name       : PopQueueCmd
    * description: 命令队列出栈，弹出一个命令
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void PopQueueCmd(void);

    /**
     * 连接数据库
     */
    void ConnectDb();

private:
	/**************************************************************************
    * name       : SendFluxControlWarn
    * description: 发送流控告警
    * input      : bIsWarn  true 表示是流控告警，false表示是流控恢复告警
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void SendFluxControlWarn(bool bIsWarn, unsigned short usLevel);

protected:
    unsigned int                 m_uiTimeout;        // 超时时间
    time_t                       m_tLastUpdateTime;  // 最后一次会话更新时间，用于判断超时使用;
    ACE_thread_t                 m_threadID;         // 线程ID
    ACE_hthread_t                m_hThread;          // 线程句柄,windows 使用句柄管理线程;
    std::string                  m_strName;          // 线程名
    bool                         m_bRunning;         // 运行状态标记位
    bool                         m_bIdleMode;        // 线程运行方式，默认false以轮询的方式运行，为true，则以Idle方式运行

    CServiceSet                  m_setService;       // 任务集
    typedef std::queue<CCmd*>    CCmdQueue;          // 命令集
    CCmdQueue                    m_queueCmd;         // 命令队列
	unsigned int				 m_uiCmdQueueLength; // 队列最大长度
    ACE_Recursive_Thread_Mutex   m_mutexCmd;         // 命令负载列表锁;
    ACE_Auto_Event               m_evWait;

    time_t m_nLastConnectDbTime; //上次连接数据库的时间, 等于0时，表示连接数据库成功, 初始值位-1 -- 表示要连接数据库
    bool                         m_bDbConnection;   //是否创建数据库连接标记

	bool                         m_szAlreadyWarned[LEVEL_NUMBER];  //是否被流控的标志，用于告警产生与恢复
	bool                         m_bIsCommuThread;  //是否是通讯线程
};

NAMESPACE_BP_END   // namespace end
#endif //BP_THREAD_H
