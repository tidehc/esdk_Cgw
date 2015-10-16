/********************************************************************
filename    :     monitor_fork_ctrl.h
author      :     s69859
created     :     2012/09/20
description :     监控进程僵死控制类
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/09/20 初始版本
*********************************************************************/

#ifndef MONITOR_FORKCTRL_H_
#define MONITOR_FORKCTRL_H_

#include "MainCtrl.h"
#include "Cmd.h"
#include "Thread.h"
#define CHECK_STATFILE_INTERVAL 5


NAMESPACE_BP_BEGIN
class CMonMainctrl : public IService
{
public:
	CMonMainctrl(void);
    virtual ~CMonMainctrl(void);

    virtual int HandleCmd(CCmd* pCmd);

    virtual uint32_t Routing(void);  //定期检查;

private:
	void checkProcess();

    bool killprocess(const char *pfile, pid_t currentPid);

private:
	time_t m_iLastCheckStatTime; //上次进程僵死监控检查时间
	std::string m_szPidFilePath;    //放pid文件的路径

};
NAMESPACE_BP_END

#endif /* MONITOR_FORKCTRL_H_ */

