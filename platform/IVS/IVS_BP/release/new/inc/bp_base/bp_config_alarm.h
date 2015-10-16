/********************************************************************
    filename    :    bp_config_alarm.h
    author      :    yWX150774
    created     :    2013/03/11
    description :    BP内部配置告警
    copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
    history     :    2013/03/11 初始版本
*********************************************************************/

#ifndef BP_CONFIG_ALARM_H
#define BP_CONFIG_ALARM_H


#include "bp_def.h"

typedef void (*ConfigErrorCallBack)(int, const char *, const char *, const char *, const char *);

const unsigned int CONFIG_ALARM_PATH_NAME_LENGTH = 256;
const unsigned int CONFIG_ALARM_FILE_NAME_LENGTH = 32;
const unsigned int CONFIG_ALARM_ITEM_NAME_LENGTH = 128;
const unsigned int CONFIG_ALARM_INVALID_VALUE_LENGTH = 64;
const unsigned int CONFIG_ALARM_DEFAULT_VALUE_LENGTH = 64;

typedef struct ConfigAlarmInfo
{
    uint32_t      uiErrorCode;                                       // 错误码
    char          szPath[CONFIG_ALARM_PATH_NAME_LENGTH];             // 配置文件名
    char          szFile[CONFIG_ALARM_FILE_NAME_LENGTH];             // 配置文件名
    char          szItem[CONFIG_ALARM_ITEM_NAME_LENGTH];             // 配置项名称
    char          szInvalidValue[CONFIG_ALARM_INVALID_VALUE_LENGTH]; // 配置项错误值
    char          szDefaultValue[CONFIG_ALARM_DEFAULT_VALUE_LENGTH]; // 配置项默认值
} CONFIG_ALARM_INFO;

enum
{
    CONFIG_FAIL_ALARM_FOR_CANNOT_FIND_FILE = 700, // 找不到配置文件
    CONFIG_FAIL_ALARM_FOR_INVALID_VALUE           // 节点的值无效
};

class EXPORT_DLL CConfigAlarm
{
public:
    CConfigAlarm();
    ~CConfigAlarm();

public:
    void SetConfigCallBack(ConfigErrorCallBack pFuncCallBack);

public:
    void Alarm(int iErrCode, const char * pszFile, const char * pszItem, 
               const char * pszInvalidValue, const char * pszDefaultValue) const;

private:
    CConfigAlarm(const CConfigAlarm &);
    CConfigAlarm & operator = (const CConfigAlarm &);

private:
    ConfigErrorCallBack m_pFuncCallBack;  // 数据库访问异常回调函数指针
};


#endif // BP_CONFIG_ALARM_H
