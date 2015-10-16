/****************************************************************************************
filename    :    ivs_aes256_ctr_partial.h
author      :    lianshiguo 00174213
created     :    2012/11/22 (直接用以前版本的, 兼容C30, C60)
description :    AES-CTR加解密, 部分码流AES256加解密
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/22 (从之前版本上除去测试代码)
*****************************************************************************************/

#ifndef CBB_BASE_AES256_CTR_PARTIAL_H
#define CBB_BASE_AES256_CTR_PARTIAL_H





//guodognalinig

#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

/*****************************************************************************************
 * AES-CTR加解密
 * ibuf : 要加/解密的数据流, 以字节为单位 (如果不满一个字节, 可以在后面补足; 加/解密后删除补足的字节即可)
 * len  : 要加/解密的数据流长度 (字节数)
 * obuf : 加/解密后的数据流, 以字节为单位 (如果加/解密前补足过, 加/解密后可删除补足的部分)
 * cbuf : 加/解密用的初始化向量，可自主选定 (16个字节)
 * key  : 加/解密密钥, 可自主选定 (字节数可以是: 16, 24, 32)
 * nbits: 加/解密密钥的比特长度, 与密钥key的字节数匹配 (比特数可以是: 128, 192, 256)
 ****************************************************************************************/
extern "C" void EXPORT_DLL AesCtrCry(
    const unsigned char * ibuf, int len, unsigned char * obuf, 
    unsigned char * cbuf, const unsigned char * key, int nbits);

/*****************************************************************************************
 * 部分码流AES256加密
 * ibuf : 要加密的数据流, 以字节为单位 (如果不满一个字节, 可以在后面补足; 加密后删除补足的字节即可)
 * len  : 要加密的数据流长度 (字节数)
 * obuf : 加密后的数据流, 以字节为单位 (如果加密前补足过, 加密后可删除补足的部分)
 * cbuf : 加密用的初始化向量，可自主选定 (16个字节)
 * key  : 加密密钥, 可自主选定 (字节数可以是: 16, 24, 32)
 * nbits: 加密密钥的比特长度, 与密钥key的字节数匹配 (比特数可以是: 128, 192, 256)
 * selective_enc_ratio: 加密的数据比例, 变化范围为(0, 1). 
 *                      例如: 0.5表示每两个字节中加密一个字节; 0.1表示每10个字节中加密一个字节
 ****************************************************************************************/
extern "C" void EXPORT_DLL AesCtrSelectiveEncry(
    const unsigned char * ibuf, int len, unsigned char * obuf, 
    unsigned char * cbuf, const unsigned char * key, int nbits, 
    double selective_enc_ratio);

/*****************************************************************************************
 * 部分码流AES256解密
 * ibuf : 要解密的数据流, 以字节为单位 (如果不满一个字节, 可以在后面补足; 解密后删除补足的字节即可)
 * len  : 要解密的数据流长度 (字节数)
 * obuf : 解密后的数据流, 以字节为单位 (如果解密前补足过, 解密后可删除补足的部分)
 * cbuf : 解密用的初始化向量，可自主选定 (16个字节)
 * key  : 解密密钥, 可自主选定 (字节数可以是: 16, 24, 32)
 * nbits: 解密密钥的比特长度, 与密钥key的字节数匹配 (比特数可以是: 128, 192, 256)
 * selective_enc_ratio: 解密的数据比例, 变化范围为(0, 1). 
 *                      例如: 0.5表示每两个字节中解密一个字节; 0.1表示每10个字节中解密一个字节
 ****************************************************************************************/
extern "C" void EXPORT_DLL AesCtrSelectiveDecry(
    const unsigned char * ibuf, int len, unsigned char * obuf, 
    unsigned char * cbuf, const unsigned char * key, int nbits, 
    double selective_enc_ratio);

NAMESPACE_CBB_END


#endif // CBB_BASE_AES256_CTR_PARTIAL_H
