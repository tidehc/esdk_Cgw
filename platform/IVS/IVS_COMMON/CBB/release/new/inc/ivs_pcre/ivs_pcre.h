/********************************************************************
filename    :    ivs_pcre.h
author      :    l00193369
created     :    2012.11.3
description :    正则表达式
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012.11.3 初始版本
*********************************************************************/

#ifndef CBB_PCRE_H
#define CBB_PCRE_H


#include "ivs_namespace.h"

#include "pcre.h"

NAMESPACE_CBB_BEGIN

/************************************************************************
 * 编译正则表达式 
 * pattern    正则表达式
 * options    为0，或者其他参数选项
 * errptr     出错消息
 * erroffset  出错位置
 * tableptr   指向一个字符数组的指针，可以设置为空NULL
 * 返回值      成功返回 pcre*, 失败返回 NULL
 ***********************************************************************/
extern "C"  EXPORT_DLL pcre * ivs_pcre_compile(const char *pattern, int options, const char **errptr, 
                                               int *erroffset, const unsigned char *tableptr);

/************************************************************************
 * 使用编译好的模式进行匹配，采用的是一种非传统的方法DFA，只是对匹配串扫描一次 
 * code      编译好的模式
 * extra     指向一个pcre_extra结构体，可以为NULL
 * subject   需要匹配的字符串
 * length    匹配的字符串长度（Byte
 * startoffset        匹配的开始位置
 * options   选项位
 * ovector   指向一个结果的整型数组
 * ovecsize  数组大小
 * 返回值     成功返回 返回匹配串的偏移位置, 失败返回 -1
 ***********************************************************************/
extern "C"  EXPORT_DLL int ivs_pcre_exec(const pcre *code, const pcre_extra *extra, const char *subject, 
                                         int length, int startoffset, int options, int *ovector, int ovecsize);

/************************************************************************
 * 释放编译生成的pcre
 * void *    释放的指针
 * 返回值     void
 ***********************************************************************/
extern "C"  EXPORT_DLL void ivs_pcre_free(void * pattern);

NAMESPACE_CBB_END

#endif // CBB_PCRE_H
