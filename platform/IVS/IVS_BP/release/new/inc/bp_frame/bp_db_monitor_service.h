/********************************************************************
filename    :    bp_db_monitor_service.h
author      :    x00196467
created     :    2014/07/29
description :    db连接的检测线程
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2014/07/29 初始版本
*********************************************************************/

#ifndef BP_DB_MONITOR_SERVICE_H
#define BP_DB_MONITOR_SERVICE_H

#include "IService.h"

class EXPORT_DLL CDBMonitorService : public BP::IService
{
public:
    CDBMonitorService ();
    virtual ~CDBMonitorService(void);

public:
    /**************************************************************************
    * name       : HandleCmd
    * description: 播发命令，具体任务实现类需要处理命令
    * input      : pCmd 命令指针
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
    virtual int HandleCmd(BP::CCmd* pCmd);

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

    time_t m_tLastCheckTime;
};

#endif