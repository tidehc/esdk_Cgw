/*****************************************************************************
*               Copyright (C) 2013, Huawei Tech. Co., Ltd.               
*                           ALL RIGHTS RESERVED                               
* FileName: soft_terminal_type.h                                                          
* Version: 1.0                                                                
* Description:  呼叫模块的RTP处理.     
*                                                                             
* History:                                                                    
* 1. 2013-03-12,y00186060 Create this file.                                 
******************************************************************************/
#ifndef __SOT_TYPE_H__
#define __SOT_TYPE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>

#ifdef __cplusplus
	extern "C"{
#endif

#undef const
typedef void             SOT_VOID;     /**< 基本数据类型定义 */
typedef const void       SOT_CVOID;    /**< 基本数据类型定义 */
typedef char      SOT_INT8;     /**< 基本数据类型定义 */
typedef unsigned char    SOT_UINT8;    /**< 基本数据类型定义 */
typedef signed short     SOT_INT16;    /**< 基本数据类型定义 */
typedef unsigned short   SOT_UINT16;   /**< 基本数据类型定义 */
typedef signed long      SOT_INT32;    /**< 基本数据类型定义 */
typedef unsigned long    SOT_UINT32;   /**< 基本数据类型定义 */
typedef unsigned char    SOT_BOOL;     /**< 基本数据类型定义 */
typedef int              SOT_INT;      /**< 基本数据类型定义 */
typedef unsigned int     SOT_UINT;     /**< 基本数据类型定义 */
typedef float            SOT_FLOAT;    /* 32 bits float */


#define SOT_FALSE 0
#define SOT_TRUE 1

#define SOT_SUCCESS    0   /**< 成功 */
#define SOT_FAIL       1   /**< 失败 */

/** 无效值定义 */
#define SOT_INVALID8       0xFF        /**< 8位无效值定义 */
#define SOT_INVALID16      0xFFFF      /**< 16位无效值定义 */
#define SOT_INVALID32      0xFFFFFFFF  /**< 32位无效值定义 */
#define SOT_INVALID_VALUE  -1          /**< 位无效值定义 */

/*****************宏定义*****************/
#define SOT_MAX_IPADDR_HEX_LEN 16  /**< ipv6地址的16进制表示的字节长度 */

#define SOT_PRODUCT_INFO_LEN 64 /** 产品信息长度*/

//呼叫号，即通道号的最大长度
#define  MAX_CALL_NUM_LEN  256 /** 与SIPC_MAX_NAME_LEN保持一致*/

//会议号的最大长度
#define  MAX_CONF_NUM_LEN 256 /** 与SIPC_MAX_NAME_LEN保持一致*/

typedef enum tagSOT_IP_VER_E
{
	SOT_IP_V4 = 0, /**< IP协议栈版本:v4 */
	SOT_IP_V6 = 1  /**< IP协议栈版本:v6 */

}SOT_IP_VER_E;


//呼叫支持的音频能力：G711a和G711u
typedef enum tagSOT_CAP_AUDIO_E
{
	SOT_CAP_AUDIO_G711_A, //G711a
	SOT_CAP_AUDIO_G711_U, //G711u
	SOT_CAP_AUDIO_END,
}SOT_CAP_AUDIO_E;


//媒体类型：音频和视频
typedef enum 
{
	SOT_MEDIA_AUDIO,    //音频，暂不支持，方便后续扩展
	SOT_MEDIA_VIDEO,   //视频，只支持H.264
	SOT_MEDIA_END
}SOT_MEDIA_TYPE_E;


//发送的数据类型
typedef enum
{
	RTP_H264_DATA,    //监控平台已经打好RTP包
	RAW_H264_DATA     //一帧的裸数据
}SOT_H264_DATA_TYPE_E;


//发送音频数据类型
typedef enum
{
	G711_A_AUDIO_DATA,    //G711a
	G711_U_AUDIO_DATA     //G711u
}SOT_AUDIO_DATA_TYPE_E;


typedef struct tagSOT_IP_ADDR_S
{
	SOT_IP_VER_E eIpVesion;          /**< ip 协议版本 */
	SOT_UINT32  ipv4;                /**< ipV4地址值 */
	SOT_UINT8   ipv6[SOT_MAX_IPADDR_HEX_LEN];/**< ipV6地址值 */

}SOT_IP_ADDR_S;


/** media chan rtp info*/
typedef struct tagSOT_MEDIA_RTP_INFO_S
{
	SOT_IP_ADDR_S  stRtpAddr;  /**< RTP 地 址结构 */
	SOT_UINT16     usRtpPort;  /**< RTP 端口号 */
	SOT_UINT16     usRtcpPort; /**< RTCP 端口号  */
}SOT_MEDIA_RTP_INFO_S;


//日志级别
typedef enum tagSOT_LOG_LEVEL_E
{
	SOT_LOG_ERROR,      //错误
	SOT_LOG_WARNING,    //报警
	SOT_LOG_INFO,       //信息
	SOT_LOG_TRACE       //Trace信息
}SOT_LOG_LEVEL_E;


//使用软终端实体，vsd还是cgw
typedef enum tagSOT_ENTITY_TYPE_E
{
	SOT_ENTITY_VSD,      //VSD
	SOT_ENTITY_CGW    //CGW
}SOT_ENTITY_TYPE_E;


/** 媒体收发模式*/
typedef enum tagSOT_MEDIA_MODE_E
{
	SOT_MEDIA_MODE_BUTT = 0x00, 
	SOT_AUDIO_SILENCE = 0x01,  /*音频，静默，不收也不发*/
	SOT_AUDIO_RECVONLY = 0x02, /*音频，只收*/
	SOT_AUDIO_SENDONLY = 0x04, /*音频，只发*/
	SOT_AUDIO_SENDRECV = 0x06, /*音频，收发*/

	SOT_VIDEO_SILENCE = 0x10,  /*视频，静默，不收也不发*/
	SOT_VIDEO_RECVONLY = 0x20, /*视频，只收*/
	SOT_VIDEO_SENDONLY = 0x40, /*视频，只发*/
	SOT_VIDEO_SENDRECV = 0x60, /*视频，收发*/
}SOT_MEDIA_MODE_E;


/** 通道状态*/
typedef enum tagSOT_CHANNEL_STATUS_E
{
	SOT_CHANNEL_IDLE,   /*通道空闲*/
	SOT_CHANNEL_ACTIVE, /*通道占用*/
	SOT_CHANNEL_BUTT    /*非法，未分配对应的通道*/
}SOT_CHANNEL_STATUS_E;


//接收呼叫是否成功
typedef enum tagSOT_REC_CALL_STAT_E
{
	SOT_RECV_CALL_SUCCESS,	//接收呼叫成功
	SOT_RECV_CALL_FAIL     //接收呼叫失败  

}SOT_RECV_CALL_STAT_E;

//创建session是否成功
typedef enum tagSOT_CREATE_SESSION_STAT_E
{
	SOT_CREATE_SESSION_FAIL,      //创建session失败
	SOT_CREATE_SESSION_SUCCESS    //创建session成功

}SOT_CREATE_SESSION_STAT_E;

//错误报告类型
typedef enum tagSOT_ERROR_CODE_E
{
	SOT_INIT_STACK_ERROR,			/*初始化协议栈失败*/
	SOT_RTP_SESSION_CREATE_FAILED,   /*创建RTP会话失败*/
}SOT_ERROR_CODE_E;

/** 系统配置参数*/
typedef struct tagSOT_SYSCFG_S
{
	/*本端SIP配置*/
	SOT_IP_ADDR_S  stLocalAddr; /*本端地址*/   
	SOT_UINT16 usLocalPort;      /*本端SIP端口号*/
	SOT_IP_ADDR_S stLocalAddr2;	/*本端地址2*/   
	SOT_UINT16 usLocalPort2;      /*本端端口号2*/

	SOT_UINT16 usLocalRtpBasePort; /*本端RTP基础端口，请设置在6000~60000之间*/
	SOT_UINT16 usLocalRtpPortNum; /*本端RTP端口个数*/

	/*收发模式*/
	SOT_UINT16 usMediaMode; /*媒体收发模式*/

	/*调试信息开关*/
	SOT_LOG_LEVEL_E  eLogLevel; /*级别控制*/

	SOT_CAP_AUDIO_E eCapAudio; //呼叫的音频能力

	SOT_ENTITY_TYPE_E eEntityType;//使用软终端的实体，vsd还是cgw

	/*产品信息配置*/
	SOT_INT8 acProductInfo[SOT_PRODUCT_INFO_LEN+1];
	SOT_UINT32 ulProductInfoLen;

}SOT_SYSCFG_S;

//创建通道时需要的参数
typedef struct tagSOT_CHANNEL_INFO_S
{
	SOT_INT8 * pcConfNumber;     //会议号
	SOT_UINT32    ulConfLen;     //会议号的长度
	SOT_INT8 * pcCallNumber; //通道号
	SOT_UINT32    ulCallLen;     //通道号的长度
}SOT_CHANNEL_INFO_S;

typedef struct tagSOT_START_CALL_S
{
	SOT_UINT16		usSotNum; /*通道号*/
	SOT_UINT32     ulLocalUserNameLen;
	SOT_UINT8      aucLocalUserName[MAX_CALL_NUM_LEN + 1]; /**< 本端号码 */
	SOT_UINT32     ulRmtUserNameLen;/**< 远端号码长度 */
	SOT_UINT8      aucRmtUserName[MAX_CONF_NUM_LEN + 1];  /**< 远端号码 */

	SOT_IP_ADDR_S  stRmtAddr;      /**< 远端ip */
	SOT_UINT16     usRmtPort;      /**< 远端端口号 */

	SOT_UINT32		ulCallRate;     /* call bandwidth kbps*/
}SOT_START_CALL_S;

typedef struct tagSOT_END_CALL_S
{
	SOT_UINT16 usSotNum;
	SOT_UINT32 ulReason; /*挂断原因*/
}SOT_END_CALL_S;


/*上层应用传入，日志相关回调函数*/
typedef SOT_UINT32 (* pFnSotLog)(SOT_LOG_LEVEL_E eLevel,SOT_INT8 * pcLog,SOT_UINT32 lLen);


/** 请求I帧重传(注：呼叫管理不关注I帧请求结果，如需失败重试，请回调函数内部保证) */
typedef SOT_UINT32 (* pFnSotIFrameReq)(IN SOT_INT8 * pcSotNum);


/** 建立接收呼叫是否成功的通知回调函数（注：现只有在接收呼叫成功才调用该回调函数） */
typedef SOT_VOID (*pFnSotReceiveCallNotify)(SOT_INT8 * pcSotNum,SOT_RECV_CALL_STAT_E eRecStat);


/** 创建session是否成功的通知回调函数（注：现只有在创建session成功才调用该回调函数） */
typedef SOT_VOID (*pFnSotCreateSessionNotify)(SOT_INT8 * pcSotNum,SOT_CREATE_SESSION_STAT_E eCreSessionStat,SOT_MEDIA_TYPE_E ucMediaType, SOT_UINT8 ucPayloadType,SOT_UINT16 usReceivePort);


/** 呼叫挂断的回调函数 */
typedef SOT_VOID (*pFnSotCallHangup)(SOT_INT8 * pcSotNum);

/** 错误上报函数*/
typedef SOT_VOID (*pFnSotErrorReport)(SOT_INT8 * pcSotNum, SOT_ERROR_CODE_E eErrorCode);


/*需上层传入的回调函数*/
typedef struct  tagSOT_APP_REG_FN_S
{
	/*日志相关*/
	pFnSotLog				pfSotCallLogFunc;          /**< log输出回调接口 */

	/*I帧重传*/
	pFnSotIFrameReq			pfSotIFrameReq;     /**< 媒体控制请求上报 */

	/*呼叫建立*/
	pFnSotReceiveCallNotify	pfSotReceiveCallNotify;  /**< 建立接收呼叫是否成功的通知回调函数 */

	/*session建立*/
	pFnSotCreateSessionNotify	pfSotCreateSessionNotify;  /**< 创建session是否成功的通知回调函数 */

	/*呼叫挂断*/
	pFnSotCallHangup		pfSotCallHangup;    /**< 呼叫挂断回调函数 */

	pFnSotErrorReport		pfSotErrorReport; /**< 错误报告*/	
}SOT_APP_REG_FN_S;

#ifdef __cplusplus
	}
#endif

#endif /*__SOT_TYPE_H__*/
