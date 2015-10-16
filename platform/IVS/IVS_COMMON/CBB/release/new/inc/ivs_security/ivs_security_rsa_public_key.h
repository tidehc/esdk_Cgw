/********************************************************************
filename    :    ivs_security_rsa_public_key.h
author      :    yWX150774
created     :    2012/11/05
description :    RSA公钥加密数据
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/05 初始版本
*********************************************************************/

#ifndef CBB_SECURITY_RSA_PUBLIC_KEY_H
#define CBB_SECURITY_RSA_PUBLIC_KEY_H


#include "ivs_uncopy.hpp"

#include "openssl/ossl_typ.h"

NAMESPACE_CBB_BEGIN

/*********************************************************************
 * 负责用RSA公钥加密数据, 
 * 加密后的数据的大小是固定的, 
 * 继承自CUnCopy, 是个不能拷贝的类
 ********************************************************************/
class EXPORT_DLL CRSA_Public_Key : private CUnCopy
{
public:
    explicit CRSA_Public_Key(RSA * pRSA);

    explicit CRSA_Public_Key(
        const char * pszPubKeyN, 
        const char * pszPubKeyE);

    ~CRSA_Public_Key();

public:
    /*****************************************************************
     * 确定需要多大的加密输出缓冲大小
     ****************************************************************/
    int EncryptedBufferLength() const;

    /*****************************************************************
     * 对pInBuff中的数据用公钥进行加密, 加密后的数据保存在pOutBuff中, 要求
     * pOutBuff的缓冲大小足够大, 用户可先调用EncryptedBufferLength()
     * 来确定需要多大的输出缓冲大小, iOutLen作为输入参数时, 为pOutBuff的缓冲大小
     * 必须 >= EncryptedBufferLength(), 否则, 失败返回! 另外, iOutLen作为
     * 输出参数, 还会保存加密后的数据的大小, 理应==EncryptedBufferLength()
    *****************************************************************/
    bool Encrypt(
        const void * pInBuff, int iInLen, 
        void * pOutBuff, int & iOutLen) const;

private:
    RSA     * m_pRSA;
    int       m_iEncryptedBufferLen;
};

NAMESPACE_CBB_END


#endif // CBB_SECURITY_RSA_PUBLIC_KEY_H
