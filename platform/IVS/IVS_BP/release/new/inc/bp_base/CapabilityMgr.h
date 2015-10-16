/********************************************************************
filename    :    CapabilityMgr.h
author      :    s00191067
created     :    2012/03/27
description :    能力管理
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/27 初始版本
*********************************************************************/
#ifndef BP_CAPABILITY_MGR_H
#define BP_CAPABILITY_MGR_H

#include <vector>
#include <map>
#include <list>
#include "ace_header.h"
#include "bp_def.h"
#include "bp_stdint.h"

using std::vector;

NAMESPACE_BP_BEGIN // namespace begin

typedef struct stCapabilityInfo
{
    uint32_t        ReqID;      // 请求ID,对应消息类型
    uint32_t        ServiceID;  // 任务ID
	std::string     ServiceName; //任务的名称
    ACE_thread_t    ThreadID;   // 线程ID
    int     ModuleType; // 模块类型

    stCapabilityInfo()
        : ReqID(0)
        , ServiceID(0)
        , ServiceName("")
        , ThreadID(0)
        , ModuleType(0)
    {
    }

    stCapabilityInfo& operator = (const stCapabilityInfo& info)
    {
        this->ReqID = info.ReqID;
        this->ServiceID = info.ServiceID;
        this->ThreadID = info.ThreadID;
		this->ServiceName = info.ServiceName;
        this->ModuleType = info.ModuleType;
        return *this;
    }

}CAPABILITY_INFO;

typedef std::list<CAPABILITY_INFO*> CCapabilityList;
typedef CCapabilityList::iterator   CapabilityListIter;

class EXPORT_DLL CCapabilityMgr
{
public:
    virtual ~CCapabilityMgr(void);

    static CCapabilityMgr& instance();

    void close();

    /**************************************************************************
    * name       : RegCapability
    * description: 注册能力
    * input      : enReqID 请求ID 
                   enServiceID 任务ID
                   threadID 线程ID
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : 如果某Service能处理10个请求，该Service需要注册10次
    **************************************************************************/
    int RegCapability(uint32_t enReqID, uint32_t enServiceID, ACE_thread_t threadID, int moduleType = 0);

    /**************************************************************************
    * name       : GetCapability
    * description: 根据负载情况获取最优的能力对象
    * input      : enReqID 请求ID 
    * output     : stCapabilityInfo 负载最优的能力对象
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : 线程的负载均衡实现方案：遍历list表获取消息队列长度最短的线程。
    **************************************************************************/
    int GetCapabiltybyLoad(uint32_t enReqID, CAPABILITY_INFO& stCapabilityInfo);

    int GetModuleType(uint32_t enReqID, int& outModuleType);

   /**************************************************************************
    * name       : GetCapabilityAll
    * description: 获取能处理请求的所有能力对象
    * input      : enReqID 请求ID 
    * output     : vecCapabilityInfo 所有能力对象
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : 线程的负载均衡实现方案：遍历list表获取消息队列长度最短的线程。
    **************************************************************************/
    int GetCapabilityAll(uint32_t enReqID, std::vector<CAPABILITY_INFO>& vecCapabilityInfo);

private:
    CCapabilityMgr(void);

    /**************************************************************************
    * name       : GetCapability
    * description: 获取能力表
    * input      : enReqID 请求ID 
    * output     : NA
    * return     : CCapabilityList* 根据请求ID获取能处理该请求的能力List表
    * remark     : 线程的负载均衡实现方案：遍历list表获取消息队列长度最短的线程。
    **************************************************************************/
    CCapabilityList* GetCapability(uint32_t enReqID);

private:
	static CCapabilityMgr m_Instance;

    typedef std::map<uint32_t, CCapabilityList*> CCapabilityMap;     //key为请求ID
    typedef CCapabilityMap::iterator    CapabilityMapIter;
    CCapabilityMap                      m_mapCapability;
    ACE_Recursive_Thread_Mutex                    m_mutex;  // 线程负载列表锁;
};

NAMESPACE_BP_END   // namespace end
#endif //BP_CAPABILITY_MGR_H
