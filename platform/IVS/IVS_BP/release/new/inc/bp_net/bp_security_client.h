/********************************************************************
filename    :    bp_security_client.h
author      :    yWX150774
created     :    2012/11/08
description :    信息安全客户端 (密钥交换请求方)
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/08 初始版本
*********************************************************************/

#ifndef BP_SECURITY_CLIENT_H
#define BP_SECURITY_CLIENT_H


#include "Cmd.h"

#include "exchange_key_attribute.h"

#include "ivs_xml.h"
#include "ivs_uncopy.hpp"
#include "ivs_security_aes.h"
#include "ivs_security_rsa_public_key.h"

#include <string>

NAMESPACE_BP_BEGIN

/*********************************************
 * 
 * 信息安全客户端 (密钥交换请求方)
 * 
 ********************************************/
class EXPORT_DLL CSecurityClient : private CBB::CUnCopy
{
public:
    CSecurityClient();
    ~CSecurityClient();

public:
    /*****************************************************
     * 请求密钥交换
     * 参数:      组成CCmd需要的信息
     * 返回值:    成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
     *****************************************************/
    int RequestExchangeKey(
        uint32_t uiServiceID, int iNetElemType, 
        const char * pszNetLinkID, 
        const char * pszCameraCode = "", 
        const char * pszDomainCode = "", 
        const char * pszDestModule = "", 
        const char * pszDeviceCode = "", 
        const char * pszNVRCode = "", 
        enum DOMAIN_TYPE enDomainType = DOMAIN_NONE);

private: // 下面函数都放在RequestExchangeKey中调用, 不对外提供
    /*****************************************************
     * 初始化
     *****************************************************/
    bool Init();

    /*****************************************************
     * 请求RSA公钥
     * 参数:      组成CCmd需要的信息
     * 返回值:    成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
     *****************************************************/
    int RequestPublicKey(
        uint32_t uiServiceID, int iNetElemType, 
        const char * pszNetLinkID, 
        const char * pszCameraCode = "", 
        const char * pszDomainCode = "", 
        const char * pszDestModule = "", 
        const char * pszDeviceCode = "", 
        const char * pszNVRCode = "", 
        enum DOMAIN_TYPE enDomainType = DOMAIN_NONE);

private: // 因为是同步处理消息的方式, 故设置为私有函数
    /*****************************************************
     * 处理RSA公钥请求的响应
     ****************************************************/
    int HandlePublicKeyResponse(CCmd * pRespPubKeyCmd);

    /*****************************************************
     * 处理密钥交换请求的响应
     ****************************************************/
    int HandleExchangeKeyResponse(CCmd * pRespExchKeyCmd);

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
     * pszInBuff:   字符串类型的明文
     * pszOutBuff:  输出缓冲
     * iOutBuffLen: 输出缓冲的大小, 成功的话, 会保存'\0'
     * 返回值:       是否成功
     *****************************************************/
    bool EncryptString(const char * pszInBuff, 
        char * pszOutBuff, int iOutBuffLen) const;

    /*****************************************************
     * 功能: 先用Base64解码, 再用AES解密出字符串数据
     * pszInBuff:   字符串类型的Base64编码的密文
     * pszOutBuff:  输出缓冲
     * iOutBuffLen: 输出缓冲的大小, 成功的话, 会保存'\0'
     * 返回值:       是否成功
     *****************************************************/
    bool DecryptString(const char * pszInBuff, 
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

    /*****************************************************
     * 功能: 
     *        获知传输AES加密数据时需要的特定的TransID
     *****************************************************/
    const std::string & GetSecurityDataTransID() const;

    /*****************************************************
     * 功能: 
     *        获知传输AES加密数据时需要的特定的TransID
     *        nLen需足够大, 否则会失败
     *        成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
     *****************************************************/
    int GetSecurityDataTransID(char * szTransactionNo, size_t nLen) const;

    /*****************************************************
     * 功能: 
     *        得到内部AES对象的副本
     *        成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
     *****************************************************/
    int DupAes(CBB::CAES * pAes) const;

private:
    /*****************************************************
     * 创建保存目的地址信息的XML
     ****************************************************/
    int BuildDestInfoXml(
        CBB::CXml & rDestInfoXml, 
        const char * pszCameraCode, 
        const char * pszDomainCode, 
        const char * pszDestModule, 
        const char * pszDeviceCode, 
        const char * pszNVRCode, 
        enum DOMAIN_TYPE enDomainType);

    /*****************************************************
     * 从XML文件中解析出RSA的公钥
     ****************************************************/
    int ParseRSAPublicKeyFromXML(CBB::CXml & rRespPubKeyXml);

    /*****************************************************
     * 创建密钥交换请求的XML
     ****************************************************/
    int BuildRequestExchangeKeyXml(
        CBB::CXml & rReqExchKeyXml, 
        const char * pszCameraCode, 
        const char * pszDomainCode, 
        const char * pszDestModule, 
        const char * pszDeviceCode, 
        const char * pszNVRCode, 
        enum DOMAIN_TYPE enDomainType);

private:
    bool                    m_bInit;                       // 是否已正常初始化
    uint32_t                m_uiSeqID;
    std::string             m_strSecurityDataTransID;      // 传输AES加密数据时所需的特定TransID
    unsigned char         * m_pRSAEncryptBuff;             // 保存RSA公钥加密所得数据的缓冲
    int                     m_iRSAEncryptLen;              // 保存RSA公钥加密所得数据的缓冲大小
    unsigned char         * m_pRSAEncryptBase64EncodeBuff; // 保存RSA公钥加密再被Base64编码所得数据的缓冲
    int                     m_iRSAEncryptBase64EncodeLen;  // 保存RSA公钥加密再被Base64编码所得数据的缓冲大小
    CBB::CRSA_Public_Key  * m_pRSAPubKey;                  // 请求到的RSA公钥
    CBB::CAES             * m_pAES;                        // AES加解密对象
    bool                    m_bGetPubKey;                  // 是否获得了RSA公钥
    unsigned char           m_szExchKey[EXCHANGE_KEY_LENGTH]; // 保存工作密钥
};

NAMESPACE_BP_END


#endif // BP_SECURITY_CLIENT_H
