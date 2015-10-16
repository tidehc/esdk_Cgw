/********************************************************************
filename    :     msg_resend_record.h
author      :     h00232170, yWX150774
created     :     2012/10/26
description :     消息重复信息
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/10/26 初始版本
*********************************************************************/

#ifndef BP_MSG_RESEND_RECORD_H
#define BP_MSG_RESEND_RECORD_H

#include "bp_namespace.h"
#include "bp_def.h"

#include "ace_header.h"

#include <map>
#include <list>

NAMESPACE_BP_BEGIN

const int RESEND_BAD_SEQUENCE                        = -1;    // 无效的编号

enum enMsgResendState
{
    RESEND_STATE_INIT,      // 初始化
    RESEND_STATE_SAVE,      // 保存
    RESEND_STATE_SENDED,    // 已发送
    RESEND_STATE_FAILURE,   // 失败
    RESEND_STATE_SUCCESS    // 成功
};

struct MsgResendInfo
{
    MsgResendInfo();

    int                 iRecordSequence;                    // 在消息重发文件的序号;
    uint32_t            uiMsgType;                          // 重发的消息类型;
    int                 iNetType;                           // 重发的目标网元类型;
    char                szLinkID[DEVICE_CODE_LEN];          // 重发的目标网元ID;
    char                szTransactionNo[TRANSACTIONNO_LEN]; // 重发消息的TransID;
    int                 iCurrentResendTimes;                // 已经重发次数;
    int                 iResendInterval;                    // 重发时间间隔
    int                 iMaxResendTimes;                    // 最大重发次数
    time_t              tLastSendMsgTime;                   // 上次重发时间;
    enMsgResendState    enResendState;                      // 重发状态;
};

struct MsgResendRecord : public MsgResendInfo
{
    MsgResendRecord();

    bool HaveDataToSend() const;
    bool LastSendFailed() const;
    bool ResendOverload() const;
    bool ResendTimeCome(time_t tCurrentTime) const;

    ACE_Message_Block  * m_pMsgBlock;
};

typedef std::map<int, MsgResendRecord>  CMsgResendRecordMap;
typedef std::list<int>                  CRecordSequenceList;

NAMESPACE_BP_END

#endif // BP_MSG_RESEND_RECORD_H
