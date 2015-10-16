/********************************************************************
filename    :    ThreadMon.h
author      :    s00191067
created     :    2012/03/28
description :    线程监控类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/28 初始版本
*********************************************************************/
#ifndef THREAD_MON_H
#define THREAD_MON_H

#include "ThreadMgr.h"
#include "ace_header.h"
#include "bp_namespace.h"

NAMESPACE_BP_BEGIN // namespace begin
enum CHECK_INTERVAL
{
    MIN_INTERVAL         = 180,              // 最小间隔
    MAX_INTERVAL         = 3600,           // 最大间隔
    DEFAULT_INTERVAL    = 1800,            // 默认间隔

    DEFAULT_REPORT_INTERVAL        = 10,    // 定义线程状态上报的最大周期,单位秒
    DEFAULT_THREAD_CHECK_INTERVAL  = 10,     // 定义线程检查的最大间隔，单位秒
    DEFAULT_THREAD_CHECK_TIME_PROOFREADING = 10 // 时间超时次数校对值
};

class EXPORT_DLL CThreadEventHandle
{
public:
	CThreadEventHandle(){}
	virtual ~CThreadEventHandle(){}

public:
	virtual void OnThreadTimeOut(CThread*& pThread);
};

class CThreadMon
{
public:
    ~CThreadMon(void);

    static CThreadMon& instance();

    /**************************************************************************
    * name       : CheckMonitor
    * description: 监测线程
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
    int CheckMonitor();

    // 设置线程管理类对象;
    void SetThreadMgr(CThreadMgr* pThreadMgr) { m_pThreadMgr = pThreadMgr; }
	void SetEventHandle(CThreadEventHandle* pEventHandle);

private:
    CThreadMon(void);
    CThreadMon(const CThreadMon &src)
    {
    }

    CThreadMon& operator=(const CThreadMon&)
    {
        return *this;
    }

private:
	static CThreadMon m_Instance;

    time_t            m_tLastCheckTime;                 // 最近一次检查时间;
    unsigned long     m_ulMaxThreadCheckInterval;        // 线程检查间隔;

	CThreadMgr*       m_pThreadMgr;
	CThreadEventHandle* m_pEventHandle;
};

NAMESPACE_BP_END   // namespace end
#endif //THREAD_MON_H
