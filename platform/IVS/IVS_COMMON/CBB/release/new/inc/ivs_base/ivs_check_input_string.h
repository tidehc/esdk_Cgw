/********************************************************************
filename    :    ivs_check_input_string.h
author      :    yWX150774
created     :    2012.11.20
description :    输入校验头文件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012.11.20 初始版本
*********************************************************************/

#ifndef CBB_BASE_CHECK_INPUT_STRING_H
#define CBB_BASE_CHECK_INPUT_STRING_H


#include "ivs_namespace.h"

#include <vector>
#include <string>

/************************************************************************
 * 默认的特殊字符黑名单: (不能包含的特殊字符集)
 *    01. 单引号 ( ' )         02. 反斜杠+单引号 ( \' )
 *    03. 双引号 ( " )         04. 反斜杠+双引号 ( \" )
 *    05. 后圆括号 ( ) )       06. 分号 ( ; )
 *    07. 前尖括号 ( < )       08. 后尖括号 ( > )
 *    09. 美元符号 ( $ )       10. 百分符号 ( % )
 *    11. at符号 ( @ )        12. 双中划线 ( -- )
 ***********************************************************************/
static const std::string s_aDefaultInvalidStringArray[] = 
{
    std::string("\'"), 
    std::string("\\\'"), 
    std::string("\""), 
    std::string("\\\""), 
    std::string(")"), 
    std::string(";"), 
    std::string("<"), 
    std::string(">"), 
    std::string("$"), 
    std::string("%"), 
    std::string("@"), 
    std::string("--")
};

/************************************************************************
 * 默认的特殊字符黑名单 (不能包含的特殊字符集)
 ***********************************************************************/
static const std::vector<std::string> s_aDefaultInvalidStringSet
(
    s_aDefaultInvalidStringArray, 
    s_aDefaultInvalidStringArray + 
        sizeof(s_aDefaultInvalidStringArray) / sizeof(s_aDefaultInvalidStringArray[0])
);

/************************************************************************
 * 默认的特殊字符白名单 (忽略检查的特殊字符集), 用于抵消黑名单的功能
 ***********************************************************************/
static const std::vector<std::string> s_aDefaultIgnoreStringSet;

NAMESPACE_CBB_BEGIN

/************************************************************************
 * 校验特殊字符
 * pszString        : 待校验的字符串
 * rIgnoreStringSet : 需要忽略检查的特殊字符集 (特殊字符白名单) 用于抵消黑名单的功能
 * rInvalidStringSet: 不能包含的特殊字符集    (特殊字符黑名单)
 * 返回值            : 字符串合法 返回 IVS_SUCCEED, 否则返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL CheckSpecialString(
    const char * pszString, 
    const std::vector<std::string> & rIgnoreStringSet = s_aDefaultIgnoreStringSet, 
    const std::vector<std::string> & rInvalidStringSet = s_aDefaultInvalidStringSet);

/************************************************************************
 * 校验数字字符
 * pszString    : 待校验的字符串
 *                合法字符集为 { pszIgnoreSet + pszValidSet }
 *                即:
 *                如果pszString中含有不在pszIgnoreSet和pszValidSet字符集中的字符
 *                则认为pszString不合法
 *                当pszIgnoreSet和pszValidSet都为NULL, pszString也不合法
 *                当pszString为NULL或"", 也不合法
 * pszIgnoreSet : 需要忽略检查的字符集 (默认忽略字符集为千位分隔符(逗号: ",") )
 *                pszIgnoreSet可以为NULL, 此时相当于""
 * pszValidSet  : 认为有效的字符集 (默认为: ".0123456789+E-e")
 *                pszValidSet可以为NULL, 此时相当于""
 * 返回值        : 字符串合法 返回 IVS_SUCCEED, 否则返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL CheckNumberString(
    const char * pszString, 
    const char * pszIgnoreSet = ",", 
    const char * pszValidSet = ".0123456789+E-e");

/************************************************************************
 * 校验一般字符
 * pszString    : 待校验的字符串
 *                下列64种字符是一定合法的:
 *                   ('a'-'z') + ('A'-'Z') + ('0'-'9') + ('_') + ('-')
 * pszIgnoreSet : 需要忽略检查的字符集 (默认忽略字符集为空)
 *                pszIgnoreSet可以为NULL, 此时相当于""
 * 返回值        : 字符串合法 返回 IVS_SUCCEED, 否则返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL CheckNormalString(
    const char * pszString, 
    const char * pszIgnoreSet = NULL);

/************************************************************************
 * 校验字符串长度
 * pszString  : 待校验的字符串
 * uiMaxLength: 字符串长度的上限 (字符串长度必须 <= uiMaxLength)
 * uiMinLength: 字符串长度的下限 (字符串长度必须 >= uiMinLength), 默认为0
 * 返回值        : 字符串合法 返回 IVS_SUCCEED, 否则返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL CheckStringLength(
    const char * pszString, 
    unsigned int uiMaxLength, 
    unsigned int uiMinLength = 0);

NAMESPACE_CBB_END


#endif // CBB_BASE_CHECK_INPUT_STRING_H
