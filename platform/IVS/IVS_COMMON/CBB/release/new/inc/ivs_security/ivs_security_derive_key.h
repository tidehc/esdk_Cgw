/********************************************************************
filename    :    ivs_security_derive_key.h
author      :    yWX150774
created     :    2013/01/23
description :    密钥生成算法
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/01/23 初始版本
*********************************************************************/

#ifndef CBB_SECURITY_DERIVE_KEY_H
#define CBB_SECURITY_DERIVE_KEY_H


#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

/****************************************************************
 * 密钥生成 (生成的密钥用做循环异或的Key值)
 *    实现说明:
 *       1. 以下设定仍然依据C30的定义
 *       2. 需要兼容C30的实现
 *       3. C30内部的uiIterateCount少计算了一次, 
 *          为了兼容, 此代码也会少算一次
 *       4. 如果不用考虑与C30的兼容, 可以考虑为后三个数值参数设定默认值
 *       5. 与C30版本的一点不同是, 第一个参数的类型改为无符号的
 *          显然, 这个是更合理的
 * pszKeyBuffer  : 用于保存导出的密钥的缓冲区
 * uiKeyBufferLen: 用于保存导出的密钥的缓冲区的大小 (不以'\0'结尾)
 * pszInitKey    : 初始密钥
 * uiInitKeyLen  : 初始密钥的长度, 为0时, 自动取字符串pszInitKey的长度
 * pszSalt       : 干扰信息
 * uiSaltLen     : 干扰信息的长度, 为0时, 自动取字符串pszSalt的长度
 * uiIterateCount: 重复次数, 为0时, 自动取2048
 * 返回值         : 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_Derive_Key(
    unsigned char * pszKeyBuffer, unsigned int uiKeyBufferLen, 
    const char * pszInitKey, unsigned int uiInitKeyLen, 
    const char * pszSalt, unsigned int uiSaltLen, 
    unsigned int uiIterateCount);

NAMESPACE_CBB_END


#endif // CBB_SECURITY_DERIVE_KEY_H
