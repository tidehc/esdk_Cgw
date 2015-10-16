/********************************************************************
filename		: 	eLTE_Types.h
author			:	zWX229156
created		    :	2015/1/6
description		:	进行数据类型以及回调函数的重定义
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/6 初始版本
*********************************************************************/
#ifndef __ELTE_TYPES_H__
#define __ELTE_TYPES_H__

#ifdef WIN32
#define __SDK_CALL __stdcall
#else
#define __SDK_CALL
#endif

typedef int               ELTE_INT32;
typedef unsigned int      ELTE_UINT32;
typedef char              ELTE_CHAR;
typedef void              ELTE_VOID;
typedef unsigned long     ELTE_ULONG;
typedef unsigned short    ELTE_USHORT;

// 消息定义(消息上报)
typedef enum
{
	ELTE_Event_NotifyUserStatus = 0,			// 设备状态变化事件通知
	ELTE_Event_NotifyP2pvideocallStatus,		// 开始实况事件通知
	ELTE_Event_NotifyResourceStatus,			// 群组关系状态变化事件通知
	ELTE_Event_NotifyProvisionChange,			// 设备属性配置变更通知事件
	ELTE_Event_NotifyProvisionAllResync,		// 自动下载配置数据通知事件
	ELTE_Event_NotifyP2pcallStatus,				// 点呼状态变化事件
	ELTE_Event_NotifyGroupStatus,				// 组呼状态变化事件
	ELTE_Event_NotifyModuleStatus				// 模块组件状态事件
} eLTE_SDK_EVENT_TYPE;

/**************************************************************************
* name			: EventCallBack
* description   : 事件回调函数(事件通知，异常回调等)
* input			: N/A
* output		: iEventType		事件类型
				  pEventBuf		    事件数据
				  uiBufSize         Buf长度
				  pUserData         用户数据
* return		: N/A
**************************************************************************/
typedef ELTE_VOID (__SDK_CALL * EventCallBack)(ELTE_INT32 iEventType, ELTE_VOID* pEventBuf, ELTE_UINT32 uiBufSize, ELTE_VOID* pUserData);

#endif //__ELTE_TYPES_H__
