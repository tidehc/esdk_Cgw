/********************************************************************
filename    :    ivs_security_sha.h
author      :    yWX150774
created     :    2012/11/06
description :    SHA(256)摘要算法
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/06 初始版本
*********************************************************************/

#ifndef CBB_SECURITY_SHA_H
#define CBB_SECURITY_SHA_H


#include "ivs_namespace.h"

#include "openssl/sha.h"
#include "openssl/hmac.h"

NAMESPACE_CBB_BEGIN

/* 数据经过摘要处理, 再经过十六进制转换, 得到的数据所需占用的缓冲大小 */
const int SHA256_HEX_LENGTH = 2 * SHA256_DIGEST_LENGTH + 1;

/****************************************************************
 * SHA(256)是一种摘要算法, OpenSSL中已经有相应实现, 这里对其进行简易封装
 * pszData  : 输入数据
 * iDataLen : 输入数据长度
 * pszDigest: 保存得到的摘要数据
 * 返回值    : 摘要数据的长度, 
 *            如果输入数据有效, 返回固定值: SHA256_DIGEST_LENGTH
 *            否则为0
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_SHA256(
    const unsigned char * pszData, int iDataLen, 
    unsigned char * pszDigest);

/****************************************************************
 * 先进行SHA256摘要处理, 再进行十六进制转换处理成可打印的数据
 * pszData     : 输入数据
 * iDataLen    : 输入数据长度
 * pszDigestHex: 保存经过十六进制转换得到的摘要数据
 * 返回值       : 摘要数据经过十六进制转换后的长度, 
 *               如果输入数据有效, 返回固定值: SHA256_HEX_LENGTH
 *               否则为0
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_SHA256_Hex(
    const unsigned char * pszData, int iDataLen, 
    char * pszDigestHex);

/****************************************************************
 * SHA(256)是一种摘要算法, OpenSSL中已经有相应实现, 这里对其进行简易封装
 * pszData  : 输入数据
 * iDataLen : 输入数据长度
 * pszDigest: 保存得到的摘要数据
 * 返回值    : 摘要数据的长度, 
 *            如果输入数据有效, 返回固定值: SHA256_DIGEST_LENGTH
 *            否则为0
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_HMAC_SHA256(
    const unsigned char * pszData, int iDataLen, 
    const unsigned char *pszSalt, int iSaltLen,
    char * pszDigest);

/****************************************************************
 * 先进行HMAC SHA256摘要处理, 再进行十六进制转换处理成可打印的数据
 * pszData     : 输入数据
 * iDataLen    : 输入数据长度
 * pszSalt     : 输入数据
 * iSaltLen    : 输入数据长度
 * pszDigestHex: 保存经过十六进制转换得到的摘要数据
 * 返回值       : 摘要数据经过十六进制转换后的长度, 
 *               如果输入数据有效, 返回固定值: SHA256_HEX_LENGTH
 *               否则为0
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_HMAC_SHA256_Hex(const unsigned char * pszData, int iDataLen, 
    const unsigned char *pszSalt, int iSaltLen,
    char * pszDigestHex);

NAMESPACE_CBB_END


#endif // CBB_SECURITY_SHA_H
