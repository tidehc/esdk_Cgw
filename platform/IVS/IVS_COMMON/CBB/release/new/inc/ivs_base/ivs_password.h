/********************************************************************
filename    :    ivs_password.h
author      :    yWX150774
created     :    2012.11.20
description :    输入校验头文件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012.11.20 初始版本
*********************************************************************/

#ifndef CBB_BASE_PASSWORD_H
#define CBB_BASE_PASSWORD_H


#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

enum KEY_RELIABILITY
{
    KEY_WEAK,
    KEY_MIDDLE,
    KEY_STRONG,
    KEY_STRONGER
};

/************************************************************************
 * 获知口令的强度
 * pszPassword : 口令
 * 返回值       : 口令的强度
 ***********************************************************************/
extern "C" KEY_RELIABILITY EXPORT_DLL GetPasswordReliability(
    const char * pszPassword);

/************************************************************************
 * 口令自动生成函数 (自动生成(iBufferLength - 1)位的口令)
 * pszPasswordBuffer : 保存口令的缓冲区
 * iBufferLength     : 缓冲区的大小 (密码长度必须>=6, 所以缓冲大小必须>=7)
 * iSymbolOption     : 字符选项
 *     (0 != (iSymbolOption & 0x08)) : 必须有大写
 *     (0 != (iSymbolOption & 0x04)) : 必须有小写
 *     (0 != (iSymbolOption & 0x02)) : 必须有数字
 *     (0 != (iSymbolOption & 0x01)) : 必须有特殊字符
 * 返回值             : 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL GeneratePassword(
    char * pszPasswordBuffer, int iBufferLength, int iSymbolOption);

/************************************************************************
 * 口令校验函数
 *                 被校验的口令必须满足iSymbolOption的要求, 
 *                 如果口令中含有不是下面四种类型的字符, 也会返回失败
 * pszPassword   : 待校验的口令
 * iSymbolOption : 字符选项
 *     (0 != (iSymbolOption & 0x08)) : 必须有大写
 *     (0 != (iSymbolOption & 0x04)) : 必须有小写
 *     (0 != (iSymbolOption & 0x02)) : 必须有数字
 *     (0 != (iSymbolOption & 0x01)) : 必须有特殊字符
 * iMinLength    : 密码的最小长度 (默认为8)
 * iMaxLength    : 密码的最大长度 (默认为-1, -1代表无要求)
 * 返回值         : 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL CheckPassword(
    const char * pszPassword, int iSymbolOption, 
    int iMinLength = 8, int iMaxLength = -1);

NAMESPACE_CBB_END


#endif // CBB_BASE_PASSWORD_H
