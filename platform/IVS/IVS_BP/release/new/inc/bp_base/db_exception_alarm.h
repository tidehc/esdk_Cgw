/********************************************************************
    filename    :    db_exception_alarm.h
    author      :    yWX150774
    created     :    2013/03/09
    description :    数据库异常告警
    copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
    history     :    2013/03/09 初始版本
*********************************************************************/

#ifndef DB_EXCEPTION_ALARM_H
#define DB_EXCEPTION_ALARM_H


#include "bp_def.h"
#include "DBInterface.h"

#include "ace/Thread_Mutex.h"

#include <string>

class EXPORT_DLL CDBExceptionAlarm
{
public:
    CDBExceptionAlarm();
    ~CDBExceptionAlarm();

public:
    void SetUser(const std::string & strUser);
    void SetServer(const std::string & strServer);
    void SetExceptionCallBack(DBExceptionCallBack pFuncCallBack);

public:
    void DBConnectFailAlarm();
    void DBConnectResumeAlarm();

    /**
     * 执行SQL语句异常时告警，数据库错误码为iErrorCode。
     * bAlarm为真时总是告警（不论iErrorCode是何值），改参数为了屏蔽oracle, pgsql的区别, 由各种数据库内部判断是否告警。
     */
    void DBExceptionFailAlarm(int iErrorCode, bool bAlarm = false);
    void DBExceptionResumeAlarm();

private:
    enum enLastDBAlarmState  // 数据库告警状态
    {
        ALARM_STATE_UNKNOWN, // 程序刚启动, 未知上一次告警状态
        ALARM_STATE_ALARMED, // 上一次为告警
        ALARM_STATE_RESUMED  // 上一次为恢复
    };

private:
    DBExceptionCallBack m_pFuncCallBack;       // 数据库访问异常回调函数指针

    std::string m_strUser;                     // 用户名
    std::string m_strServer;                   // 主机名
    enLastDBAlarmState m_enConnectAlarmState;  // 连接数据库的告警状态
    enLastDBAlarmState m_enAccessAlarmState;   // 访问数据库的告警状态

    ACE_Thread_Mutex m_aThreadMutex;           // 保护上面变量的线程互斥量
};


#endif // DB_EXCEPTION_ALARM_H
