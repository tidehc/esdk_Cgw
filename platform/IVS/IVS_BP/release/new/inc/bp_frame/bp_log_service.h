/********************************************************************
filename    :    Thread.h
author      :    cwx148380
created     :    2012/11/23
description :    日志服务类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/23 初始版本
*********************************************************************/
#ifndef BP_LOG_SERVICE_H
#define BP_LOG_SERVICE_H

#include "IService.h"
#include "bp_memoutput.h"
#include "db_connect.h"

NAMESPACE_BP_BEGIN //lint !e129 !e10

class CLogService : public IService
{
public:
    virtual int HandleCmd(CCmd* pCmd){ return(0); }
    virtual uint32_t Routing(void){ return(0); }
};

class COperLogService : public IService
{
public:
    COperLogService ( CMemOutput& aMemOutput );
    virtual ~COperLogService(void);

public:
    /**************************************************************************
    * name       : HandleCmd
    * description: 播发命令，具体任务实现类需要处理命令
    * input      : pCmd 命令指针
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
    virtual int HandleCmd(CCmd* pCmd);

    /**************************************************************************
    * name       : Routing
    * description: 定期检查，具体任务实现类处理定期检查任务
				   注意：一定要统计本次routing完成的事件数，线程的轮询依赖于此统计！！！
    * input      : NA
    * output     : NA
    * return     : 返回本次rouint完成的事件数，便于后面统计
    * remark     : NA
    **************************************************************************/
    virtual uint32_t Routing(void);

private:
    CMemOutput & m_MemOutput;
    IDBConnect * m_connect;                   // 数据库连接
    const char * m_szDbSql;                   // 入库SQL
};


class CSecLogService : public IService
{
public:
    CSecLogService ( CMemOutput& aMemOutput );
    virtual ~CSecLogService(void);

public:
    /**************************************************************************
    * name       : HandleCmd
    * description: 播发命令，具体任务实现类需要处理命令
    * input      : pCmd 命令指针
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
    virtual int HandleCmd(CCmd* pCmd);

    /**************************************************************************
    * name       : Routing
    * description: 定期检查，具体任务实现类处理定期检查任务
				   注意：一定要统计本次routing完成的事件数，线程的轮询依赖于此统计！！！
    * input      : NA
    * output     : NA
    * return     : 返回本次rouint完成的事件数，便于后面统计
    * remark     : NA
    **************************************************************************/
    virtual uint32_t Routing(void);

private:
    CMemOutput & m_MemOutput;
    IDBConnect * m_connect;                   // 数据库连接
    const char * m_szDbSql;                   // 入库SQL
};

NAMESPACE_BP_END

#endif //bp_log_service.h
