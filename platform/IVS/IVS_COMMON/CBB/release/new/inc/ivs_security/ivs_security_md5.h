/********************************************************************
filename    :    ivs_security_md5.h
author      :    w00207027, yWX150774
created     :    2013/03/18
description :    MD5摘要算法 + HTTP的MD5摘要计算
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/03/18 初始版本
*********************************************************************/

#ifndef CBB_SECURITY_MD5_H
#define CBB_SECURITY_MD5_H


#include "ivs_namespace.h"

#include "openssl/md5.h"

NAMESPACE_CBB_BEGIN

/* 数据经过摘要处理, 再经过十六进制转换, 得到的数据所需占用的缓冲大小 */
const int MD5_HEX_LENGTH = 2 * MD5_DIGEST_LENGTH + 1;

/****************************************************************
 * MD5是一种摘要算法, OpenSSL中已经有相应实现, 这里对其进行简易封装
 * pszData  : 输入数据
 * iDataLen : 输入数据长度
 * pszDigest: 保存得到的摘要数据
 * 返回值    : 摘要数据的长度, 
 *            如果输入数据有效, 返回固定值: MD5_DIGEST_LENGTH
 *            否则为0
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_MD5(
    const unsigned char * pszData, int iDataLen, 
    unsigned char * pszDigest);

/****************************************************************
 * 先进行MD5摘要处理, 再进行十六进制转换处理成可打印的数据
 * pszData     : 输入数据
 * iDataLen    : 输入数据长度
 * pszDigestHex: 保存经过十六进制转换得到的摘要数据
 * 返回值       : 摘要数据经过十六进制转换后的长度, 
 *               如果输入数据有效, 返回固定值: MD5_HEX_LENGTH
 *               否则为0
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_MD5_Hex(
    const unsigned char * pszData, int iDataLen, 
    char * pszDigestHex);

/****************************************************************
 * calculate H(A1) as per HTTP Digest spec
 * 输出缓冲pszDigestHex的大小至少为MD5_HEX_LENGTH
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_Http_MD5_DigestCalcHA1(
    const char * pszAlg, 
    const char * pszUserName, 
    const char * pszRealm, 
    const char * pszPassword, 
    const char * pszNonce, 
    const char * pszCNonce, 
    char * pszDigestHex);

/******************************************************************
 * calculate request-digest/response-digest as per HTTP Digest spec
 * HA1, pszHEntity是十六进制的摘要数据, 长度必须为2 * MD5_DIGEST_LENGTH,
 * 输出缓冲pszDigestHex的大小至少为MD5_HEX_LENGTH
 ******************************************************************/
extern "C" int EXPORT_DLL IVS_Security_Http_MD5_DigestCalcResponse(
    const char * pszHA1,        /* H(A1) */
    const char * pszNonce,      /* nonce from server */
    const char * pszNonceCount, /* 8 hex digits */
    const char * pszCNonce,     /* client nonce */
    const char * pszQop,        /* qop-value: "", "auth", "auth-int" */
    const char * pszMethod,     /* method from the request */
    const char * pszDigestUri,  /* requested URL */
    const char * pszHEntity,    /* H(entity body) if qop="auth-int" */
    char * pszDigestHex);       /* request-digest or response-digest */

NAMESPACE_CBB_END


#endif // CBB_SECURITY_MD5_H
