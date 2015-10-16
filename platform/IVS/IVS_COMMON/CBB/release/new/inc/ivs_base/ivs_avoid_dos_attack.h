/********************************************************************
filename    :    ivs_avoid_dos_attack.h
author      :    yWX150774
created     :    2012/12/04
description :    防止DoS攻击的单件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/12/04 初始版本
*********************************************************************/

#ifndef CBB_BASE_AVOID_DOS_ATTACK_H
#define CBB_BASE_AVOID_DOS_ATTACK_H


#include "ivs_namespace.h"
#include "ivs_uncopy.hpp"

#include "bp_stdint.h"

#include "ace/Thread_Mutex.h"

#include <map>
#include <utility>

NAMESPACE_CBB_BEGIN

/*************************************************************
 *
 * 防止DoS攻击的类, 是一个不可拷贝的类, 另外, 实现为单件模式
 *
 ************************************************************/
class EXPORT_DLL CAvoidDoSAttack : private CBB::CUnCopy
{
private:
    CAvoidDoSAttack();
    ~CAvoidDoSAttack();

public:
    /*********************************************************
     * 获取实例
     ********************************************************/
    static CAvoidDoSAttack & GetInstance();

public:
    /*********************************************************
     * 检测此访问IP是否是安全的
     * 频繁访问的IP会加入黑名单, 经过一段时间后会从黑名单移除
     * 参数为检测的IP, 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
     ********************************************************/
    int Check(uint32_t uiCheckIP);

protected:
    static const uint32_t  LOCK_UP_TIME_SLICE;               // 拉入黑名单的最长时间
    static const uint32_t  MAX_ACCESS_TIMES_PER_TIME_SLICE;  // 单位时间片内最大的访问次数

    static CAvoidDoSAttack s_aAvoidDoSAttack;

private:
    std::map<
             uint32_t, 
             std::pair<
                       time_t, 
                       uint32_t
                      >
            >                    m_mapIP2CheckTimeAndTimes;  // IP到检测时间及检测次数的一一映射

    ACE_Thread_Mutex             m_aMapThreadMutex;          // 上面的map对应的线程互斥量

    typedef std::map<
                     uint32_t, 
                     std::pair<
                               time_t, 
                               uint32_t
                              >
                    >::iterator  IP2TimeAndTimesMapIterator;
};

NAMESPACE_CBB_END


#endif // CBB_BASE_AVOID_DOS_ATTACK_H
