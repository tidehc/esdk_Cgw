/********************************************************************
filename    :    ivs_security_aes.h
author      :    yWX150774
created     :    2012/11/06
description :    ACE加解密
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/06 初始版本
*********************************************************************/

#ifndef CBB_SECURITY_AES_H
#define CBB_SECURITY_AES_H


#include "ivs_uncopy.hpp"

#include "openssl/aes.h"

NAMESPACE_CBB_BEGIN

/*************************************************************
 * 密码学中的高级加密标准(Advanced Encryption Standard, AES),  
 * 又称Rijndael加密法, 是美国联邦政府采用的一种区块加密标准, 
 * 它是一种对称加密算法, OpenSSL中已经有相应实现, 这里对其进行简易封装
 *
 * OpenSSL函数的一些说明:
 *
 * 1. 构造函数中的bytes指代的是key的长度, 只支持16, 24, 32
 *    bytes越大, 加密数据越难以被破解
 * 2. 不管key是16, 24, 32bytes的, 加解密函数都只会取16字节
 *    如果数据不足, 就会造成数组越界, 
 *    如果过长每次都只能出来16字节 (估计它只把前16字节当成要加密的数据)
 ************************************************************/
class EXPORT_DLL CAES : private CUnCopy
{
public:
    explicit CAES();
    ~CAES();

public:
    /*****************************************************
     * 设置加解密的Key值
     * pKey:       二进制的数据, 用做加解密Key值
     * iBytes:     pszKey的长度, 必须为16, 24, 32中之一
     * 返回值:      iBytes不为16, 24, 32时必定失败
     *****************************************************/
    bool SetKey(const unsigned char * pszKey, int iBytes);

public:
    /*****************************************************
     * AES加密
     * pszInBuff:  明文
     * pszOutBuff: 密文
     * iLen:       数据长度
     *             首先, 必须为AES_BLOCK_SIZE的整数倍
     *             另外, 因为是对称加密算法, 输入输出长度是一样
     * 返回值:      数据长度不是AES_BLOCK_SIZE的整数倍时, 会失败
     *****************************************************/
    bool EncryptBinary(
        const unsigned char * pszInBuff, int iLen, 
        unsigned char * pszOutBuff) const;

    /*****************************************************
     * AES解密
     * pszInBuff:  密文
     * pszOutBuff: 明文
     * iLen:       数据长度
     *             首先, 必须为AES_BLOCK_SIZE的整数倍
     *             另外, 因为是对称加密算法, 输入输出长度是一样
     * 返回值:      数据长度不是AES_BLOCK_SIZE的整数倍时, 会失败
     *****************************************************/
    bool DecryptBinary(
        const unsigned char * pszInBuff, int iLen, 
        unsigned char * pszOutBuff) const;

    /*****************************************************
     * 功能: 先用AES加密, 再用Base64编码成字符串格式数据
     * pszInBuff:   字符串类型的明文 (空串加密还是空串)
     * pszOutBuff:  输出缓冲
     * iOutBuffLen: 输出缓冲的大小, 成功的话, 会保存'\0'
     * 返回值:       是否成功
     *****************************************************/
    bool EncryptString(const char * pszInBuff, 
        char * pszOutBuff, int iOutBuffLen) const;

    /*****************************************************
     * 功能: 先用Base64解码, 再用AES解密出字符串数据
     * pszInBuff:   字符串类型的Base64编码的密文 (空串解密还是空串)
     * pszOutBuff:  输出缓冲
     * iOutBuffLen: 输出缓冲的大小, 成功的话, 会保存'\0'
     * 返回值:       是否成功
     *****************************************************/
    bool DecryptString(const char * pszInBuff, 
        char * pszOutBuff, int iOutBuffLen) const;

    bool EncryptCbcString(const char * pszInBuff, const char* pszIvBuff,
    	 char * pszOutBuff, int iOutBuffLen) const;

    bool DecryptCbcString(const char * pszInBuff, const char* pszIvBuff,
        char * pszOutBuff, int iOutBuffLen) const;

    bool EncodeCbcSaltString(const char * pszInBuff,
    	  char * pszOutBuff, int iOutBuffLen) const;
	
    bool DncodeCbcSaltString(const char * pszInBuff,
         char * pszOutBuff, int iOutBuffLen) const;

    /*****************************************************
     * 功能: 
     *        获知长度为iStringLen(不包括'\0')的字符串经过
     *        EncryptString()处理后的输出缓冲的安全大小(最大值)
     *****************************************************/
    static int GetStringEncryptedBufferLen(int iStringLen);

    /*****************************************************
     * 功能: 
     *        获知长度为iStringLen(不包括'\0')的字符串经过
     *        DecryptString()处理后的输出缓冲的安全大小(最大值)
     *****************************************************/
    static int GetStringDecryptedBufferLen(int iStringLen);

    /*****************************************************
     * 功能: 
     *        获知字符串pszString经过
     *        EncryptString()处理后的输出缓冲的安全大小(最大值)
     *****************************************************/
    static int GetStringEncryptedBufferLen(const char * pszString);

    /*****************************************************
     * 功能: 
     *        获知字符串pszString经过
     *        DecryptString()处理后的输出缓冲的安全大小(最大值)
     *****************************************************/
    static int GetStringDecryptedBufferLen(const char * pszString);

private:
    /*****************************************************
     * 功能: 
     *        检查对象是否初始化, 以及传入参数的合法性(初步检查)
     *        用做加解密函数的子函数, 降低圈复杂度
     *****************************************************/
    bool CheckStateAndArgument(
        const void * pszInBuff, const void * pszOutBuff, int iLen) const;
    bool CheckCbcStateAndArgument(const void * pInBuff, 
    	 const char* pIvBuff, const void * pOutBuff, int iLen) const;

private:
    bool        m_bSetKey;
    AES_KEY     m_aAesEncryptKey;
    AES_KEY     m_aAesDecryptKey;
};

NAMESPACE_CBB_END


#endif // CBB_SECURITY_AES_H
