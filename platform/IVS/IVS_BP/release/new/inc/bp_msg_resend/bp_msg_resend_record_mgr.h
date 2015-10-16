/********************************************************************
filename    :     bp_msg_resend_record_mgr.h
author      :     h00232170, yWX150774
created     :     2012/10/26
description :     消息重发文件
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/10/26 初始版本
*********************************************************************/

#ifndef BP_MSG_RESEND_RECORD_MGR_H
#define BP_MSG_RESEND_RECORD_MGR_H

#include "bp_namespace.h"
#include "bp_msg_resend_record.h"

#include "ace/Thread_Mutex.h"

NAMESPACE_BP_BEGIN

class CMsgResendRecordMgr
{
public:
    CMsgResendRecordMgr();
    ~CMsgResendRecordMgr();

public:
    bool AddRecord(MsgResendRecord & rMsgResendRecord);
    bool RemoveRecord(const MsgResendRecord & rMsgResendRecord);
    bool ModifyRecord(const MsgResendRecord & rMsgResendRecord);
    int GetRecord(MsgResendRecord * pMsgResendRecord, int iMaxRecordCount);

    int GetRecordCount();

private:
    int ApplySequence();
    bool GetNextRecord(MsgResendRecord & rMsgResendRecord);

private:
    CMsgResendRecordMgr(const CMsgResendRecordMgr &);
    CMsgResendRecordMgr & operator = (const CMsgResendRecordMgr &);

private:
    CMsgResendRecordMap            m_mapMsgResendInfo;     // 重发消息内存缓存
    CMsgResendRecordMap::iterator  m_iterLastHandle;       // 当前处理的迭代器GetNextRecord中使用
    int                            m_iRecordCount;         // 有效记录数
    int                            m_iMaxRecordSequence;   // 当前文件最大记录数, 包括里面的空闲记录;
    CRecordSequenceList            m_listIdleSequence;     // 空闲记录的序号列表
    ACE_Thread_Mutex               m_aThreadMutex;         // 保护上面变量的线程互斥量
};

NAMESPACE_BP_END

#endif // BP_MSG_RESEND_RECORD_MGR_H
