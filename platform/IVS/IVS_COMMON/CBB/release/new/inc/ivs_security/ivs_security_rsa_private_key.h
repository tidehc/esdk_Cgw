/********************************************************************
filename    :    ivs_security_rsa_private_key.h
author      :    yWX150774
created     :    2012/11/05
description :    RSA私钥解密数据
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/05 初始版本
*********************************************************************/

#ifndef CBB_SECURITY_RSA_PRIVATE_KEY_H
#define CBB_SECURITY_RSA_PRIVATE_KEY_H


#include "ivs_uncopy.hpp"

#include "openssl/ossl_typ.h"

NAMESPACE_CBB_BEGIN

/****************************************************************
 * 负责用RSA私钥解密数据, 
 * 解密后的数据的大小是不确定的, 
 * 继承自CUnCopy, 是个不能拷贝的类
 ***************************************************************/
class EXPORT_DLL CRSA_Private_Key : private CUnCopy
{
public:
    explicit CRSA_Private_Key(RSA * pRSA);
    ~CRSA_Private_Key();

public:
    /************************************************************
     * 对pInBuff中的数据用私钥进行解密, 解密后的数据保存在pOutBuff中, 
     * 要求pOutBuff的缓冲大小iOutLen必须足够大, 否则, 可能会溢出! 
     * 具体得有多大, 还没看到相关的底层介绍, iOutLen保存解密后的数据大小
    ************************************************************/
    bool Decrypt(
        const void * pInBuff, int iInLen, 
        void * pOutBuff, int & iOutLen) const;

private:
    RSA * m_pRSA;
};

NAMESPACE_CBB_END


#endif // CBB_SECURITY_RSA_PRIVATE_KEY_H
