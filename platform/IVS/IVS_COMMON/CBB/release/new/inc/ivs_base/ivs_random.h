/********************************************************************
filename    :    ivs_random.h
author      :    s90003596, cWX153028, yWX150774
created     :    2012.11.3
description :    安全随机数
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012.11.3 初始版本
*********************************************************************/

#ifndef CBB_BASE_RANDOM_H
#define CBB_BASE_RANDOM_H


#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

/************************************************************************
 * 生成随机数
 *      (先尝试调用MakeRandomFromSys获取, 如果不成功, 再调用MakeRandomFromLib)
 * pszRandomBuffer : 保存输出数据的缓冲区
 * iBufferLength   : 缓冲区的大小 (不包括'\0', 如有需要, 调用者得自己加'\0')
 * 返回值           : 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL GenerateRandom(
    unsigned char * pszRandomBuffer, int iBufferLength);

/************************************************************************
 * 生成随机的可打印字符数组 (调用GenerateRandom, 再转成64种可打印字符)
 * pszRandomBuffer : 保存输出数据的缓冲区
 * iBufferLength   : 缓冲区的大小 (不包括'\0', 如有需要, 调用者得自己加'\0')
 * 返回值           : 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL GenerateRandomChars(
    unsigned char * pszRandomBuffer, int iBufferLength);

/************************************************************************
 * 调用系统函数生成随机数 (调用系统函数, 速度较慢, 安全性较高)
 * pszRandomBuffer : 保存输出数据的缓冲区
 * iBufferLength   : 缓冲区的大小 (不包括'\0', 如有需要, 调用者得自己加'\0')
 * 返回值           : 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL MakeRandomFromSys(
    unsigned char * pszRandomBuffer, int iBufferLength);

/************************************************************************
 * 生成随机的可打印字符数组 (先调用MakeRandomFromSys, 再转成64种可打印字符)
 * pszRandomBuffer : 保存输出数据的缓冲区
 * iBufferLength   : 缓冲区的大小 (不包括'\0', 如有需要, 调用者得自己加'\0')
 * 返回值           : 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL MakeRandomCharsFromSys(
    unsigned char * pszRandomBuffer, int iBufferLength);

/************************************************************************
 * 调用库函数生成随机数 (调用库函数, 速度较快, 安全性较低)
 * pszRandomBuffer : 保存输出数据的缓冲区
 * iBufferLength   : 缓冲区的大小 (不包括'\0', 如有需要, 调用者得自己加'\0')
 * 返回值           : 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL MakeRandomFromLib(
    unsigned char * pszRandomBuffer, int iBufferLength);

/************************************************************************
 * 生成随机的可打印字符数组 (先调用MakeRandomFromLib, 再转成64种可打印字符)
 * pszRandomBuffer : 保存输出数据的缓冲区
 * iBufferLength   : 缓冲区的大小 (不包括'\0', 如有需要, 调用者得自己加'\0')
 * 返回值           : 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL MakeRandomCharsFromLib(
    unsigned char * pszRandomBuffer, int iBufferLength);

NAMESPACE_CBB_END


#endif // CBB_BASE_RANDOM_H
