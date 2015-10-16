/********************************************************************
filename    :    ISession.h 
author      :    b00209723
created     :    2012/04/12
description :    媒体会话接口
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd 
history     :    2012/04/12 初始版本
*********************************************************************/
#ifndef  BP_I_SESSION_H
#define  BP_I_SESSION_H
//#include "cmd.h"
#include "bp_stdint.h"
#include <time.h>
#include "bp_namespace.h"
#include "ace_header.h"

NAMESPACE_BP_BEGIN
class CCmd;

//会话状态，-1，初始化状态，0：结束状态；其他状态自业务定义
const int SessionStatus_Init = -1;
const int SessionStatus_Complete = 0;

//默认会话超时时间
const int SessionTimeoutDef = 30;

class ISession
{
public:
    ISession()
        : m_status(SessionStatus_Init)
        , m_lastCmdTime(ACE_OS::time())
        , m_iTimeout(SessionTimeoutDef)
        , m_uiServiceID(0)
    {

    }

    virtual ~ISession(){};

    /**********************************************************************
    * name           : GetStatus
    * description    : 获取状态
    * input          : 
    * return         : 会话状态值
    * remark         : 获取会话状态
    *********************************************************************/
    int GetStatus() const {return m_status;}
	void SetStatus(int iStatus){m_status = iStatus;}
	void UpdateLastCmdTime(time_t tLastCmdTime){m_lastCmdTime = tLastCmdTime;}

	bool IsComplete(){return (SessionStatus_Complete == m_status);}

    /**********************************************************************
    * name           : HandleCmd
    * description    : 处理命令对象
    * input          : CCmd* 命令对象
    * output         : NA
    * return         : IVS_SUCCEED 成功 IVS_FAIL 失败
    * remark         : 纯虚函数，各子类实现如何处理命令
    *********************************************************************/
    virtual int HandleCmd(CCmd *) = 0;

    /**********************************************************************
    * name           : Routing
    * description    : 例行处理
    * input          : NA
    * return         : NA
    * remark         : 纯虚函数，各子类决定例行处理功能
    *********************************************************************/
	virtual void Routing() = 0;
    
    /**********************************************************************
    * name           : IsTimeout
    * description    : 判断会话是否超时
    * input          : time_t 当前时间
    * return         : True 会话超时，False 会话正常
    *********************************************************************/
	bool IsTimeout(time_t tCurrentTime)
	{
	    return ((tCurrentTime - m_lastCmdTime) >= m_iTimeout);
	}

	void SetTimeout(int iSessionTimeout)
	{
	    m_iTimeout = iSessionTimeout;
	}

    void SetServiceID(uint32_t uiServiceID) { m_uiServiceID = uiServiceID; }

    uint32_t GetServiceID() const { return(m_uiServiceID); }

    /**********************************************************************
    * name           : Timeout
    * description    : 超时后的处理流程 
    * input          : NA
    * return         : NA   
    * remark         : 纯虚函数，各子类实现超时处理事项
    *********************************************************************/
    virtual void Timeout() = 0;

private:
    int       m_status;         // 记录会话内部状态
    time_t    m_lastCmdTime;    // 前次业务处理时间，判断是否超时

	int       m_iTimeout;
    uint32_t  m_uiServiceID;
};

NAMESPACE_BP_END   // namespace end
#endif 
