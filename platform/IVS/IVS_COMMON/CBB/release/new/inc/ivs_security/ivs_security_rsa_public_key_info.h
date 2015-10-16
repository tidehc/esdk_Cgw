/********************************************************************
filename    :    ivs_security_rsa_public_key_info.h
author      :    yWX150774
created     :    2012/11/05
description :    保存从RSA中提取的的公钥信息所转换出的十进制数据N和E
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/05 初始版本
*********************************************************************/

#ifndef CBB_SECURITY_RSA_PUBLIC_KEY_INFO_H
#define CBB_SECURITY_RSA_PUBLIC_KEY_INFO_H


#include "ivs_uncopy.hpp"

#include "openssl/ossl_typ.h"

NAMESPACE_CBB_BEGIN

/***********************************************************
 * 保存从RSA中提取的的公钥信息所转换出的十进制数据N和E
 * 继承自UnCopy, 是个不能拷贝的类
 **********************************************************/
class EXPORT_DLL CRSA_Public_Key_Info : private CUnCopy
{
public:
    explicit CRSA_Public_Key_Info(const RSA * pRSA);
    ~CRSA_Public_Key_Info();

public:
    /*****************************************************************
     * 获知是否已经初始化
     ****************************************************************/
    bool HasInit() const;

    /*****************************************************************
     * 获取RSA公钥的模数N
     ****************************************************************/
    const char * GetPublicKeyN() const;

    /*****************************************************************
     * 获取RSA公钥的公钥指数E
     ****************************************************************/
    const char * GetPublicKeyE() const;

private:
    bool Init(const BIGNUM * pRSAPubN, const BIGNUM * pRSAPubE);
    void Clean();

private:
    char             * m_pszPubKeyN;
    char             * m_pszPubKeyE;
    bool               m_bInit;
};

NAMESPACE_CBB_END


#endif // CBB_SECURITY_RSA_PUBLIC_KEY_INFO_H
