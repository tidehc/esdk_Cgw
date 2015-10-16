/********************************************************************
filename    :     nss_xml_msg.h
author      :     h00232170
created     :     2012/10/24
description :     nss-xml 消息解析
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/10/24 初始版本
*********************************************************************/

#ifndef NSS_XML_MSG_H_
#define NSS_XML_MSG_H_

#include <ace_header.h>
#include "bp_stdint.h"
#include "bp_def.h"
#include "nss_msg_header.h"

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

class EXPORT_DLL CNssXMLMsg: public CNssMsgBase
{
public:
	CNssXMLMsg(void);
    virtual ~CNssXMLMsg(void);
	CNssXMLMsg(uint16_t usiMsgType,uint32_t uiSeqID, const char* szTransactionNo,uint32_t uiRspCode, uint32_t uiTraceID = 0);

	// 将消息头解析到m_header解构中，并进行了网络字节序转换；
	virtual uint32_t decodeStream(const char* pszMsg, int iMsgLength);
	// 将m_header解构中的字段，编到pMsgBlock中，pMsgBlock 会根据GetMsgLength方法统一申请内存；
	virtual uint32_t encode(ACE_Message_Block* pMsgBlock);
	// 计算消息长度，子类需要重新实现该方法，计算该消息的实际长度;
	virtual uint32_t GetMsgLength(){return (sizeof(TNssMsgHeader) + sizeof(uint16_t) + sizeof(uint32_t) + m_nssXML.uiXMLLength);}

	void SetXML(const char* pszXML, uint32_t iXMLLength);
	const char* GetXML(){return m_nssXML.pszXMLValue;}
	uint32_t GetXMLLength(){return m_nssXML.uiXMLLength;}
private:
	TNssXMLMsg m_nssXML;
};


#endif /* #define NSS_XML_MSG_H_
 */
