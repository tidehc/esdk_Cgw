/********************************************************************
filename    :     bp_msg_resend_service.h
author      :     h00232170, yWX150774
created     :     2012/10/26
description :     消息重发service
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/10/26 初始版本
*********************************************************************/

#ifndef BP_MSG_RESEND_SERVICE_H
#define BP_MSG_RESEND_SERVICE_H

#include "bp_msg_resend_record.h"
#include "bp_msg_resend_record_mgr.h"
#include "bp_def.h"
#include "IService.h"
#include "bp_session_mgr.h"

NAMESPACE_BP_BEGIN

extern "C" EXPORT_DLL IService * CreateBusinessService();

class EXPORT_DLL CMsgResendService : public IService
{
public:
    CMsgResendService();
    virtual ~CMsgResendService();

private:
    CMsgResendService(const CMsgResendService &src)
    {
    }

    CMsgResendService& operator=(const CMsgResendService&)
    {
        return *this;
    }

public:
    virtual int Init();
    virtual int HandleCmd(CCmd * pCmd);
    virtual uint32_t Routing();

public:
    CMsgResendRecordMgr * GetMsgResendRecordMgr();

private:
    ISession * NewSession(int iReqType, int iResendInterval, int iResendTimes);
    ISession * NewSession(int iReqType, const MsgResendRecord & rMsgResendRecord);
    CCmd * CreateResendCmd(MsgResendRecord & rMsgResendRecord);

    int SendMsg(CCmd * pCmd);
    int ResendMsg();

    void AdjustResendConfig();

private:
    CMsgResendRecordMgr  * m_pMsgResendRecordMgr;
    time_t                 m_tLastResendTime;
    CSessionMgr            m_aSessionMgr;
};

NAMESPACE_BP_END

#endif // BP_MSG_RESEND_SERVICE_H
