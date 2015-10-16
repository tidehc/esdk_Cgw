/********************************************************************
    filename    :    bp_alarm_storage.h
    author      :    yWX150774
    created     :    2013/03/23
    description :    BP内部告警仓库, 缓存BP还未注册处理事务能力之前的告警
    copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
    history     :    2013/03/23 初始版本
*********************************************************************/

#ifndef BP_ALARM_STORAGE_H
#define BP_ALARM_STORAGE_H


#include "bp_def.h"
#include "Cmd.h"

#include "ace/Thread_Mutex.h"

#include <list>

class EXPORT_DLL CAlarmMsgStorage
{
public:
    static CAlarmMsgStorage & GetInstance();

public:
    void Init();
    void Release();

public:
    void StopStoreAlarmMsg();
    bool TryStoreAlarmMsg(BP::CCmd *& pCmd);
    void GetAlarmMsg(std::list<BP::CCmd *> & listAlarmMsg);
    void CleanAlarmMsg();

private:
    CAlarmMsgStorage();
    ~CAlarmMsgStorage();

private:
    CAlarmMsgStorage(const CAlarmMsgStorage &);
    CAlarmMsgStorage & operator = (const CAlarmMsgStorage &);

private:
    bool                    m_bNeedStoreAlarmMsg; // 是否需要缓存告警信息
    std::list<BP::CCmd *>   m_listAlarmMsg;       // 缓存告警信息的链表
    ACE_Thread_Mutex        m_aThreadMutex;       // 保护上面变量的线程互斥量
};


#endif // BP_ALARM_STORAGE_H
