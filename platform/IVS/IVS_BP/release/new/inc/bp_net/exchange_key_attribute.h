/********************************************************************
filename    :    exchange_key_attribute.h
author      :    yWX150774
created     :    2012/12/20
description :    交换密钥的属性
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/12/20 初始版本
*********************************************************************/

#ifndef BP_EXCHANGE_KEY_ATTRIBUTE_H
#define BP_EXCHANGE_KEY_ATTRIBUTE_H


#include "ivs_error.h"
#include "bp_stdint.h"
#include "bp_nss_def.h"

NAMESPACE_BP_BEGIN

const int EXCHANGE_KEY_LENGTH  = 32; // 交换密钥的长度
const int EXCHANGE_KEY_TIMEOUT = 60; // 交换密钥的有效时长
const unsigned int RSA_DECRYPT_FAIL = IVS_SUCCEED + IVS_FAIL + 1; // RSA解密失败 (服务器换了一组RSA公私钥对)
const unsigned int SECURITY_MESSAGE_MIN_LENGTH = sizeof(TNssMsgHeader) +  sizeof(uint16_t) + sizeof(uint32_t);

enum DOMAIN_TYPE
{
    DOMAIN_NONE    = 0, // 无效值
    DOMAIN_PILE    = 1, // 堆叠
    DOMAIN_CLUSTER = 2, // 集群
    DOMAIN_SINGLE  = 3, // 单机
    DOMAIN_MAX
};

NAMESPACE_BP_END


#endif // BP_EXCHANGE_KEY_ATTRIBUTE_H
