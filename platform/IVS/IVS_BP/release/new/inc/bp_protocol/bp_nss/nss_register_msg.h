/********************************************************************
filename    :     nss_register_msg.h
author      :     h00232170
created     :     2012/10/24
description :     nss-xml 消息解析
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/10/24 初始版本
*********************************************************************/

#ifndef NSS_REGISTER_MSG_H
#define NSS_REGISTER_MSG_H

#include <ace_header.h>
#include "bp_stdint.h"
#include "bp_def.h"
#include "nss_msg_header.h"

//最长的摘要长度
const uint32_t MAX_REG_DIGEST = 256;

/////////////////NSS  TAG 定义 ///////////////////////////////
const uint16_t NssTag_XMLContent = 0x0001;


USING_NAMESPACE_BP

typedef struct NssXMLMsg
{
	uint16_t usiXMLTag;
	uint32_t uiXMLLength;
	char* pszXMLValue;
}TNssXMLMsg;

//const int NssHeaderLength = sizeof(TNssMsgHeader);

class EXPORT_DLL CNssRegisterMsg: public CNssMsgBase
{
public:
	CNssRegisterMsg(void);
        virtual ~CNssRegisterMsg(void);
	CNssRegisterMsg(uint16_t usiMsgType,uint32_t uiSeqID, const char* szTransactionNo,uint32_t uiRspCode, uint32_t uiTraceID = 0);

	// 将消息头解析到m_header解构中，并进行了网络字节序转换；
	virtual uint32_t decodeStream(const char* pszMsg, int iMsgLength);

	// 将m_header解构中的字段，编到pMsgBlock中，pMsgBlock 会根据GetMsgLength方法统一申请内存；
	virtual uint32_t encode(ACE_Message_Block* pMsgBlock);

	// 计算消息长度，子类需要重新实现该方法，计算该消息的实际长度;
	virtual uint32_t GetMsgLength(){return (sizeof(TNssMsgHeader) + DEVICE_CODE_LEN + m_uiDigestLen);}

	void SetDeviceCode(const char* pszDeviceCode);
	const char* GetDeviceCode(){return m_szDeviceCode;}

    void SetDigest(const char* pszDigest, uint32_t uiLen);
    const char* GetDigest(){return m_szDigest;}

private:
	char m_szDeviceCode[DEVICE_CODE_LEN + 1];
    char m_szDigest[MAX_REG_DIGEST + 1];
    uint32_t m_uiDigestLen;
};


#endif // NSS_REGISTER_MSG_H
