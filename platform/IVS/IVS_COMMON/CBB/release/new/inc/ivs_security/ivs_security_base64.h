/********************************************************************
filename    :    ivs_security_base64.h
author      :    yWX150774
created     :    2012/11/10
description :    Base64编解码
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/10 初始版本
*********************************************************************/

#ifndef CBB_SECURITY_BASE64_H
#define CBB_SECURITY_BASE64_H


#include "ivs_uncopy.hpp"

#include "openssl/evp.h"

NAMESPACE_CBB_BEGIN

/*********************************************************************
 * Base64编解码
 ********************************************************************/
class EXPORT_DLL CBase64
{
public:
    /*****************************************************************
     * 二进制数据的Base64编码
     * pszInBuff:  二进制输入数据
     * iInLen:     输入数据的长度
     * pszOutBuff: 经过Base64编码过后的数据, 会自动格式化(换行之类)输出数据
     * iOutLen:    编码过后的数据的长度
     * 返回值:      编码是否成功
     ****************************************************************/
    static bool EncodeBinaryFormat(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /*****************************************************************
     * 二进制数据的Base64解码
     * pszInBuff:  经过Base64编码过后的格式化(有换行之类)输入数据
     * iInLen:     输入数据的长度
     * pszOutBuff: 经过Base64解码过后的数据
     * iOutLen:    解码过后的数据的长度
     * 返回值:      解码是否成功
     *****************************************************************/
    static bool DecodeBinaryFormat(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /*****************************************************************
     * 二进制数据的Base64编码
     * pszInBuff:  二进制输入数据
     * iInLen:     输入数据的长度
     * pszOutBuff: 经过Base64编码过后的数据 (不会自动格式化)
     * iOutLen:    编码过后的数据的长度
     * 返回值:      编码是否成功
     ****************************************************************/
    static bool EncodeBinary(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /*****************************************************************
     * 二进制数据的Base64解码
     * pszInBuff:  二进制输入数据
     * iInLen:     输入数据的长度
     * pszOutBuff: 经过Base64解码过后的数据
     * iOutLen:    解码过后的数据的长度
     * 返回值:      解码是否成功
     *****************************************************************/
    static bool DecodeBinary(
        const unsigned char * pszInBuff, int iInLen, 
        unsigned char * pszOutBuff, int & iOutLen);

    /******************************************************************
     * 字符串数据的Base64编码
     * pszInBuff:  字符串输入数据
     * pszOutBuff: 经过Base64编码过后的数据 (字符串)
     * iOutLen:    保存输出数据的缓冲大小
     * 返回值:      编码是否成功
     *****************************************************************/
    static bool EncodeString(
        const char * pszInBuff, 
        char * pszOutBuff, int iOutLen);

    /******************************************************************
     * 二进制数据的Base64解码
     * pszInBuff:  字符串输入数据
     * pszOutBuff: 经过Base64解码过后的数据 (字符串)
     * iOutLen:    保存输出数据的缓冲大小
     * 返回值:      解码是否成功
     *****************************************************************/
    static bool DecodeString(
        const char * pszInBuff, 
        char * pszOutBuff, int iOutLen);

    /******************************************************************
     * 长度为inLen的二进制输入数据, 经过Base64编码后的输出数据所需占用的最大长度
     *****************************************************************/
    static int GetEncodeBinarySize(int iInLen);

    /******************************************************************
     * 长度为inLen的二进制输入数据, 经过Base64解码后的输出数据所需占用的最大长度
     *****************************************************************/
    static int GetDecodeBinarySize(int iInLen);

    /******************************************************************
     * 字符串输入数据, 经过Base64编码后的输出数据所需占用的最大长度(包括'\0')
     *****************************************************************/
    static int GetEncodeStringSize(const char * pszInBuff);

    /******************************************************************
     * 字符串输入数据, 经过Base64解码后的输出数据所需占用的最大长度(包括'\0')
     *****************************************************************/
    static int GetDecodeStringSize(const char * pszInBuff);
};

NAMESPACE_CBB_END


#endif // CBB_SECURITY_BASE64_H
