/********************************************************************
filename    :     bp_msg_resend_session.h
author      :     h00232170, yWX150774
created     :     2012/10/26
description :     消息重发session
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/10/26 初始版本
*********************************************************************/

#ifndef BP_MSG_RESEND_SESSION_H
#define BP_MSG_RESEND_SESSION_H

#include "ISession.h"
#include "bp_def.h"
#include "bp_msg_resend_record.h"
#include "bp_msg_resend_service.h"

#include <string>

NAMESPACE_BP_BEGIN

class CMsgResendSession : public ISession
{
public:
    CMsgResendSession();
    virtual ~CMsgResendSession();

public:
    virtual int HandleCmd(CCmd * pCmd);
    virtual void Routing();
    virtual void Timeout();

public:
    void Init(const MsgResendRecord & rMsgResendRecord);
    void SetService(CMsgResendService * pService);

    void SetResendInterval(int iResendInterval);
    void SetResendTimes(int iResendTimes);

private:
    int SaveMsg(CCmd * pCmd);
    void SendMsg(CCmd * pCmd);
    void HandleRsp(CCmd * pCmd);

    int RealSendMsg(uint32_t serviceID, int iNetElemType, const char * pszNetLinkID, 
                    const char * pszTransID, uint32_t uiReqID, ACE_Message_Block * pMsgBlock);

    void ModifySendFailState();

private:
    CMsgResendService   * m_pService;
    MsgResendRecord       m_aMsgResendRecord;
    int                   m_iResendInterval;
    int                   m_iResendTimes;
};

NAMESPACE_BP_END

#endif // BP_MSG_RESEND_SESSION_H
