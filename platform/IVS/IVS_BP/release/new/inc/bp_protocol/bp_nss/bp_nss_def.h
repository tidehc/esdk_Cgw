/********************************************************************
filename    :    bp_nss_def.h
author      :    h00232170
created     :    2012/9/19
description :	 NSS协议头文件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/19 初始版本
*********************************************************************/
#ifndef BP_NSSP_DEF_H
#define BP_NSSP_DEF_H

#include "nss_mt.h"
#include "bp_def.h"
#include "bp_stdint.h"

// 是否检测心跳的开关
#define _CHECK_HEARTBEAT_

// 登录状态
typedef enum enNSSP_STAT_FIAG
{
    STAT_UNDEF,
	LOGIN_INIT,
	LOGIN_ING,
	LOGIN_ERROR,
	LOGIN_OK,
	LOGIN_TIMEOUT,
	HEARTBEAT_ING,
	HEARTBEAT_OK,
	HEARTBEAT_TIMEOUT
}NSSP_STAT_FIAG;


//TODO：以后放到另一个专用消息定义头文件中
#define NSS_MSG_PROTOCOL_VERSION   0x0100  // NSS消息版本号


//////////////// 通用消息结构体定义 /////////////////////////
/*
 * 消息头结构
 */
//设置为单字节对齐
#pragma pack(push)
#pragma pack(1)

typedef struct NssMsgHeader
{
	uint16_t usiProtocolVersion;             // 协议版本信息，当前版本号为0x0100 
	uint16_t usiMsgType;                     // 消息类型,参见 NSS_MSG_TYPE DELETE_SPACE_TYPE
	uint32_t uiSeqID;                        // 消息序号，要求应答消息与请求消息一致
	uint32_t uiTraceID;                      // 消息跟踪ID
	char szTransactionNo[TRANSACTIONNO_LEN]; // 事务编号
	uint32_t uiRspCode;                      // 响应码。0：成功；其他：具体错误码
	char szReserve[RESERVE_LEN];             // 预留字段 
	uint32_t uiPacketLength;                 // 消息体长度（该长度包括消息头长度）
}TNssMsgHeader;

const int NssMsgTypePos = sizeof(uint16_t);
const int NssTransactionNoPos = sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint32_t) + sizeof(uint32_t);
const int NssRspCodePos = NssTransactionNoPos + TRANSACTIONNO_LEN;
const int NssPacketLengthPos = NssRspCodePos + sizeof(uint32_t) + RESERVE_LEN;


//注册请求消息
typedef struct _NSS_REGISTER_REQ_MSG
{
	TNssMsgHeader Header;
	char           ClientDeviceCode[DEVICE_CODE_LEN];
}NSS_REGISTER_REQ_MSG, *PNSS_REGISTER_REQ_MSG;

//注册响应消息
typedef struct _NSS_REGISTER_RESP_MSG
{
	TNssMsgHeader Header;
	char           ServerDeviceCode[DEVICE_CODE_LEN];
}NSS_REGISTER_RESP_MSG, *PNSS_REGISTER_RESP_MSG;

//心跳请求消息
typedef struct _NSS_KEEPALIVE_REQ_MSG
{
	TNssMsgHeader Header;
}NSS_KEEPALIVE_REQ_MSG, *PNSS_KEEPALIVE_REQ_MSG;

typedef struct _NSS_TEST_REQ_MSG
{
	TNssMsgHeader Header;
}NSS_TEST_REQ_MSG, *PNSS_TEST_REQ_MSG;

typedef struct _NSS_TEST_RESP_MSG
{
	TNssMsgHeader Header;
}NSS_TEST_RESP_MSG, *PNSS_TEST_RESP_MSG;

#pragma pack(pop)

#endif
