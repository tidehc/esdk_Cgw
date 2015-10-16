/********************************************************************
filename    :    ivs_alarm_status.h
author      :    
created     :    2012.11.3
description :    正则表达式
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012.11.3 初始版本
*********************************************************************/

#ifndef CBB_ALARM_STATUS_H
#define CBB_ALARM_STATUS_H

#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

#define ALARM_TYPE_LENGTH (10)
#define ALARM_INFO_LENGTH (256)

typedef struct
{
    char AlarmType[ALARM_TYPE_LENGTH];
    char AlarmInfo[ALARM_INFO_LENGTH];
} Alarm_Item;

/************************************************************************
 * 初始化接口，用于初始化缓存文件所属路径
 * pszFilePath    缓存文件所属路径
 * 返回值         成功返回IVS_SUCCEED，失败返回IVS_FAIL
 ***********************************************************************/
extern "C"  EXPORT_DLL int IVS_AlarmInit(const char * pszFilePath);

/************************************************************************
 * 释放资源接口
 * 返回值    
 ***********************************************************************/
extern "C"  EXPORT_DLL void IVS_AlarmRelease();

/************************************************************************
 * 新增缓存记录接口
 * pAlarmItem    记录信息指针
 * 返回值        若添加成功，返回IVS_SUCCEED
 *               若添加失败或记录已存在，返回IVS_FAIL
 ***********************************************************************/
extern "C"  EXPORT_DLL int IVS_AddAlarmItem(const Alarm_Item * pAlarmItem);

/************************************************************************
* 删除缓存记录接口
* pAlarmItem    记录信息指针
* 返回值        若删除成功或记录不存在，返回IVS_SUCCEED
*               若删除失败，返回IVS_FAIL
***********************************************************************/
extern "C"  EXPORT_DLL int IVS_DelAlarmItem(const Alarm_Item * pAlarmItem);

/************************************************************************
* 获取缓存记录接口
* pAlarmItem    记录信息指针
* pCount        记录计数
* 返回值        若成功，返回IVS_SUCCEED
*               若失败，返回IVS_FAIL
* 备注          当GetAllAlarmItem接口第一个参数为空时，表示获取告警记录总数，在第二个参数返回
*               当GetAllAlarmItem接口第一个参数不为空，表示告警记录数组首地址，第二个参数为数组长度
*               即，应用层先以第一种调用方式，获取总数，再根据总数分配内存空间，
*               第二次调用的时候，将先前分配的内存空间首地址、数组长度作为参数传入
***********************************************************************/
extern "C"  EXPORT_DLL int IVS_GetAllAlarmItem(Alarm_Item * pAlarmItem, int * pCount);

NAMESPACE_CBB_END

#endif // CBB_ALARM_STATUS_H
