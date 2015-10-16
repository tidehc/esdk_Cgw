/********************************************************************
filename    :    ivs_string_conversion.h
author      :    yWX150774
created     :    2012/11/22
description :    字符转换公用小组件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/22 初始版本
*********************************************************************/

#ifndef CBB_BASE_STRING_CONVERSION_H
#define CBB_BASE_STRING_CONVERSION_H


#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

/***********************************************************************
 * 异或处理
 * pData        : 待异或处理的输入数据, 也是异或处理后数据的缓冲区
 * iDataLen     : 输入数据长度
 * pSecretKey   : 用做异或处理的密钥
 * iSecretKeyLen: 密钥长度
 * 返回值        : 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL IVS_XOR(
    void * pData, int iDataLen, 
    const void * pSecretKey, int iSecretKeyLen);

/************************************************************************
 * 将二进制字符数组转换为可打印的字符数组 ('0'-'9', 'A'-'Z', 'a'-'z', '+', '-')
 * 注意: 这个转换是不可逆的
 * pszData  : 输入数据, 也是输出数据(不会自动加'\0')
 * iDataLen : 输入数据长度 (不包括'\0')
 * 返回值    : 无
 ***********************************************************************/
extern "C" void EXPORT_DLL IVS_BinaryToChars(
    unsigned char * pszData, int iDataLen);

/************************************************************************
 * 将二进制字符(unsigned char) 转换为 可打印的十六进制字符数组 ('0'-'9', 'a'-'f')
 * 这个转换是可逆的, 与IVS_HexesToUChar是互逆的
 * ucIn  : 输入数据 (单个二进制字符)
 * szOut : 输出数据 (缓冲区至少是2, 不会自动加'\0')
 * 返回值 : 无
 ***********************************************************************/
extern "C" void EXPORT_DLL IVS_UCharToHexes(
    unsigned char ucIn, char szOut[2]);

/************************************************************************
 * 将可打印的十六进制字符数组 ('0'-'9', 'a'-'f') 转换为 二进制字符(unsigned char)
 * 这个转换是可逆的, 与IVS_UCharToHexes是互逆的
 * szIn   : 输入数据 (缓冲区至少是2)
 * pszOut : 输出数据 (单个二进制字符, 不会自动加'\0')
 * 返回值  : 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL IVS_HexesToUChar(
    const char szIn[2], unsigned char * pszOut);

/************************************************************************
 * 将二进制字符数组转换为可打印的十六进制字符数组 ('0'-'9', 'a'-'f')
 * 这个转换是可逆的, 与IVS_HexesToBinary是互逆的
 * pszInData  : 输入数据
 * iInDataLen : 输入数据长度 (不包括'\0')
 * pszOutData : 输出数据 (缓冲区至少是iInDataLen*2, 不会自动加'\0')
 * 返回值      : 无
 ***********************************************************************/
extern "C" void EXPORT_DLL IVS_BinaryToHexes(
    const unsigned char * pszInData, int iInDataLen, char * pszOutData);

/************************************************************************
 * 将可打印的十六进制字符数组 ('0'-'9', 'a'-'f') 转换为 二进制字符数组
 * 这个转换是可逆的, 与IVS_BinaryToHexes是互逆的
 * pszInData  : 输入数据
 * iInDataLen : 输入数据长度 (iInDataLen必须是偶数, 不包括'\0')
 * pszOutData : 输出数据 (缓冲区至少是iInDataLen/2, 不会自动加'\0')
 * 返回值      : 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL IVS_HexesToBinary(
    const char * pszInData, int iInDataLen, unsigned char * pszOutData);

NAMESPACE_CBB_END


#endif // CBB_BASE_STRING_CONVERSION_H
