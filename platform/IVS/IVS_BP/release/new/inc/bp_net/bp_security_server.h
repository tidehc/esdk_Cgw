/********************************************************************
filename    :    bp_security_server.h
author      :    yWX150774
created     :    2012/11/08
description :    信息安全服务器 (密钥交换响应方)
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/08 初始版本
*********************************************************************/

#ifndef BP_SECURITY_SERVER_H
#define BP_SECURITY_SERVER_H


#include "bp_namespace.h"
#include "Cmd.h"

#include "ivs_xml.h"
#include "ivs_uncopy.hpp"
#include "ivs_security_aes.h"
#include "ivs_security_rsa_key.h"
#include "ivs_security_rsa_private_key.h"
#include "ivs_security_rsa_public_key_info.h"

#include "ace/Thread_Mutex.h"

#include <string>
#include <map>
#include <list>
#include <utility>

NAMESPACE_BP_BEGIN

/********************************************************
 * 
 * 信息安全服务器 (密钥交换响应方)
 * 
 *******************************************************/
class EXPORT_DLL CSecurityServer : private CBB::CUnCopy
{
public:
    CSecurityServer();
    ~CSecurityServer();

public:
    /*****************************************************
     * 初始化
     *****************************************************/
    bool Init();

public: 
    /*****************************************************
     * 处理RSA公钥请求
     * 返回值: 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
     *****************************************************/
    int HandlePublicKeyRequest(CCmd * pReqPubKeyCmd);

    /*****************************************************
     * 处理密钥交换请求 (pAes用于返回一个AES对象的副本)
     * 返回值: 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
     *****************************************************/
    int HandleExchangeKeyRequest(
        CCmd * pReqExchKeyCmd, CBB::CAES * pAes = NULL);

public:
    /*****************************************************
     * 删除过期的交换密钥
     *****************************************************/
    void RemoveTimeoutExchangeKey();

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
        const std::string & strTransID, 
        const unsigned char * pszInBuff, int iLen, 
        unsigned char * pszOutBuff);

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
        const std::string & strTransID, 
        const unsigned char * pszInBuff, int iLen, 
        unsigned char * pszOutBuff);

    /*****************************************************
     * 功能: 先用AES加密, 再用Base64编码成字符串格式数据
     * pszInBuff:   字符串类型的明文
     * pszOutBuff:  输出缓冲
     * iOutBuffLen: 输出缓冲的大小, 成功的话, 会保存'\0'
     * 返回值:       是否成功
     *****************************************************/
    bool EncryptString(
        const std::string & strTransID, 
        const char * pszInBuff, 
        char * pszOutBuff, int iOutBuffLen);

    /*****************************************************
     * 功能: 先用Base64解码, 再用AES解密出字符串数据
     * pszInBuff:   字符串类型的Base64编码的密文
     * pszOutBuff:  输出缓冲
     * iOutBuffLen: 输出缓冲的大小, 成功的话, 会保存'\0'
     * 返回值:       是否成功
     *****************************************************/
    bool DecryptString(
        const std::string & strTransID, 
        const char * pszInBuff, 
        char * pszOutBuff, int iOutBuffLen);

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
     * 从请求交换密钥的XML文件中解析出交换密钥和TransID
     *****************************************************/
    int ParseReqExchangeKeyXml(
        CBB::CXml & rReqExchKeyXml, 
        std::string & strTransID, 
        unsigned char *& pExchKey);

private:
    /*****************************************************
     * 真正处理RSA公钥请求的实现函数
     *****************************************************/
    int HandlePublicKeyRequestImplement(CCmd & rReqPubKeyCmd);
    
    /*****************************************************
     * 真正处理密钥交换请求的实现函数
     *****************************************************/
    int HandleExchangeKeyRequestImplement(
        CCmd & rReqExchKeyCmd, CBB::CAES * pAes);

    /*****************************************************
     * 依据TransID组装相应的AES对象
     *****************************************************/
    bool GetAES(const std::string & strTransID, CBB::CAES & rAes);

    /*****************************************************
     * 获取保存交换密钥的缓冲区(32字节)
     *****************************************************/
    unsigned char * GetExchangeKeyBuffer();

    /*****************************************************
     * 回收保存交换密钥的缓冲区(32字节)
     *****************************************************/
    void ReleaseExchangeKeyBuffer(unsigned char * pExchKey);

private:
    bool                                     m_bInit;                    // 是否已正常初始化
    const CBB::CRSA_Key                    & m_rRSA;                     // RSA单件实例的常引用
    CBB::CRSA_Private_Key                  * m_pRSAPriKey;               // RSA私钥信息
    CBB::CRSA_Public_Key_Info              * m_pRSAPubKeyInfo;           // RSA公钥的N和E的信息
    std::list<unsigned char *>               m_listExchKeyBuffer;        // 保存交换密钥的缓冲区的内存池
    ACE_Thread_Mutex                         m_aExchKeyBufferListThreadMutex; // 保存交换密钥的内存池对应的线程互斥量
    std::map<
             std::string, 
             std::pair<
                       unsigned char *, 
                       time_t
                      >
            >                                m_mapTransID2ExchKeyCTime;  // TransID到交换密钥及其创建时间的一一映射
    ACE_Thread_Mutex                         m_aExchKeyMapThreadMutex;   // 保存交换密钥的map对应的线程互斥量

    typedef std::map<
                     std::string, 
                     std::pair<
                               unsigned char *, 
                               time_t
                              >
                    >::iterator              TransID2ExchKeyCTimeMapIterator;
};

NAMESPACE_BP_END


#endif // BP_SECURITY_SERVER_H
