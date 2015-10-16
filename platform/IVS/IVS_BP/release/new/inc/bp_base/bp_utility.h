/********************************************************************
    filename    :    bp_utility.h
    author      :    yWX150774
    created     :    2013/03/11
    description :    功能小组件
    copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
    history     :    2013/03/11 初始版本
*********************************************************************/

#ifndef BP_UTILITY_H
#define BP_UTILITY_H


#include "bp_def.h"

NAMESPACE_BP_BEGIN

/* 获取不带路径的文件名 */
extern "C" EXPORT_DLL const char * GetFileNameWithoutPath(const char * pszFilenameWithPath);

/* 检测主机名(IP)是否合法, 只校验每个分隔位是否属于[0, 255] */
extern "C" EXPORT_DLL bool CheckHostName(const char * pszHostName);

/* 检测端口号是否合法, 只校验是否属于[1, 65535] */
extern "C" EXPORT_DLL bool CheckHostPort(const char * pszHostPort);

/* 判断是否为不带符号的合法整数字符串 */
extern "C" EXPORT_DLL bool IsUnsignedInteger(const char * pszValue);

enum enPathCheckMode
{
    PATH_CHECK_DEFAULT_MODE, // 以默认的方式校验路径名
    PATH_CHECK_UNIX_MODE,    // 明确指定为UNIX对应的规则
    PATH_CHECK_WINDOWS_MODE  // 明确指定为WINDOWS对应的规则
};

/******************************************************************
 * 按下面规则判断是否为合法的路径名, 并非判断路径是否存在
 * Unix:
 *          1. 路径:
 *             1>  不能是空串
 *             2>  不能包含 <>\"?:|*
 *             3>  不能包含 \t\r\n
 *             4>  不能包含 \\
 *          2. 每一个子路径(文件名, 文件夹名):
 *             1>  不能以空格开始
 *             2>  不能以空格结束
 * Windows:
 *          1. 路径:
 *             1>  不能是空串
 *             2>  不能包含 <>\"?|*
 *             3>  不能包含 \t\r\n
 *             4>  ':'只能出现在根目录中
 *          2. 每一个子路径(文件名, 文件夹名):
 *             1>  不能以空格开始
 *             2>  不能以空格结束
 *             3>  不能以'.'开始(.和..除外)
 *             4>  不能以\\开始
 *             5>  不能以/开始
 ******************************************************************/
extern "C" EXPORT_DLL bool IsValidPathName(
    const char * pszPathName, 
    enPathCheckMode enMode = PATH_CHECK_DEFAULT_MODE);

NAMESPACE_BP_END


#endif // BP_UTILITY_H
