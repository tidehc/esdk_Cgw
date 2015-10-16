/********************************************************************
filename    :    LinkStatusAlarmInfo.h
author      :    l00193369
created     :    2013/03/11
description :    连接告警管理类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/03/11 初始版本
*********************************************************************/
#ifndef LINK_STATUS_ALARM_MGR_H
#define LINK_STATUS_ALARM_MGR_H


#include "bp_namespace.h"
#include "bp_mem.h"
#include "bp_map.h"
#include "ILink.h"
#include <string>
#include "ace_header.h"

NAMESPACE_BP_BEGIN

typedef struct linkStatusWarnInfo
{
	LINK_STATUS_REPORT_INFO   linkStatusReportInfo;
	time_t                    tTime;
	int                       iNetElementType;
} LINK_STATUS_WARN_INFO;

typedef std::map<string, LINK_STATUS_WARN_INFO*> LSWIMap; //key值为netElement+LinkID
typedef LSWIMap::iterator          LSWIMapIterator;

class CLinkStatusAlarmMgr
{
public:
	~CLinkStatusAlarmMgr();
	static CLinkStatusAlarmMgr& Instance();

	/**************************************************************************
    * name       : AddAlarm
    * description: 往map表加入一条告警
    * input      : iNetElementType        网元类型
	*              pLinkStatusReportInfo  告警信息
    * output     : N/A
    * return     : 成功返回IVS_SUCCESSED,失败返回IVS_FAIL
    * remark     : N/A
    **************************************************************************/
	int AddAlarm(int iNetElementType, LINK_STATUS_REPORT_INFO * pLinkStatusReportInfo);

	/**************************************************************************
    * name       : GetAlarm
    * description: 连接的命令处理
    * input      : pCmd    : 命令对象
    * output     : N/A
    * return     : 成功返回对应的告警信息指针，无告警返回NULL
    * remark     : N/A
    **************************************************************************/
	LINK_STATUS_WARN_INFO * GetAlarm();

	/**************************************************************************
    * name       : Release
    * description: 清空map里的消息
    * input      : pCmd    : 命令对象
    * output     : N/A
    * return     : 成功返回对应的告警信息指针，无告警返回NULL
    * remark     : N/A
    **************************************************************************/
	void Release();

private:
	CLinkStatusAlarmMgr();
	LINK_STATUS_WARN_INFO * NewLinkStatusWarnInfo(int iNetElementType, LINK_STATUS_REPORT_INFO * pLinkStatusReportInfo);


	LSWIMap m_LSWIMapForClose;
	LSWIMap m_LSWIMapForOpen;
	ACE_Thread_Mutex                    m_mutex;  // 连接信息map锁;
};

NAMESPACE_BP_END 

#endif