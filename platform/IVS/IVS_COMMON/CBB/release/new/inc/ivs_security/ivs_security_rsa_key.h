/********************************************************************
filename    :    ivs_security_rsa_key.h
author      :    yWX150774
created     :    2012/11/05
description :    生成配对RSA公私钥, 公私钥可被分别备份
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/05 初始版本
*********************************************************************/

#ifndef CBB_SECURITY_RSA_KEY_H
#define CBB_SECURITY_RSA_KEY_H


#include "ivs_uncopy.hpp"

#include "openssl/ossl_typ.h"

NAMESPACE_CBB_BEGIN

class CRSA_Private_Key;
class CRSA_Public_Key;
class CRSA_Public_Key_Info ;

/*************************************************************
 * 负责生成配对RSA公私钥
 * 继承自CUnCopy, 是个不能拷贝的类
 * 另外, 实现为单件模式
 ************************************************************/
class EXPORT_DLL CRSA_Key : private CUnCopy
{
private:
    explicit CRSA_Key(int iBits = 2048);
    ~CRSA_Key();

public:
    /*********************************************************
     * 获取实例
     ********************************************************/
    static CRSA_Key & GetInstance();

public:
    /*********************************************************
     * 拷贝出私钥
     ********************************************************/
    CRSA_Private_Key * DupPrivateKey() const;

    /*********************************************************
     * 拷贝出公钥
     ********************************************************/
    CRSA_Public_Key * DupPublicKey() const;

    /*********************************************************
     * 拷贝出公钥信息(N和E)
     ********************************************************/
    CRSA_Public_Key_Info * DupPublicKeyInfo() const;

private:
    /*********************************************************
     * 重新生成配对的RSA公私钥, 暂时不提供此接口, 故定义为private函数
     ********************************************************/
    bool ReInit(int iBits = 2048);

private:
    static CRSA_Key s_aRSA;

private:
    RSA * m_pRSA;
};

NAMESPACE_CBB_END


#endif // CBB_SECURITY_RSA_KEY_H
