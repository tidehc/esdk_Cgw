/********************************************************************
filename		: 	eLTE_SDK.h
author			:	zWX229156
created		    :	2015/1/5
description		:	定义 DLL 应用程序的导出函数
copyright		:	Copyright (C) 2015-2017
history			:	2015/1/5 初始版本
*********************************************************************/

#ifndef __ELTE_SDK_H__
#define __ELTE_SDK_H__

#include "eLTE_Types.h"

// 这里提供DLL导出接口
#ifdef WIN32
#ifdef  ELTE_SDK_EXPORTS
#define ELTE_SDK_API __declspec(dllexport)
#else
#define ELTE_SDK_API __declspec(dllimport)
#endif
#else
#define ELTE_SDK_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	/**************************************************************************
	* name			: ELTE_SDK_SetLogPath
	* description   : 设置日志路径，请使用绝对路径方便查看日志文件
	* input			: pLogFilePath 日志保存路径
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_SetLogPath(const ELTE_CHAR* pLogFilePath);

	/**************************************************************************
	* name			: ELTE_SDK_SetLogLevel	
	* description   : 设置日志级别
	* input			: iLogLevel 日志级别
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_SetLogLevel(ELTE_UINT32 iLogLevel);

	/**************************************************************************
	* name			: ELTE_SDK_ReleaseBuffer
	* description   : 释放SDK内部申请的内存
	* input			: pBuffer 内存指针
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL  ELTE_SDK_ReleaseBuffer(ELTE_CHAR* pBuffer);

	/**************************************************************************
	* name			: ELTE_SDK_GetVersion
	* description   : 获取SDK版本号
	* input			: pVersion 获取SDK版本号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetVersion(ELTE_CHAR** pVersion);

	/**************************************************************************
	* name			: ELTE_SDK_Init
	* description   : 初始化SDK
	* input			: N/A
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_Init();

	/**************************************************************************
	* name			: ELTE_SDK_Cleanup
	* description   : 释放SDK
	* input			: N/A
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_Cleanup();

	/**************************************************************************
	* name			: ELTE_SDK_SetEventCallBack
	* description   : 设置消息回调函数
	* input			: fEventCallBack 用户设置的事件回调函数，为空表示取消设置的回调函数
	                  pUserData      用户数据
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_SetEventCallBack(EventCallBack fEventCallBack, ELTE_VOID* pUserData);

	/**************************************************************************
	* name			: ELTE_SDK_Login
	* description   : 登录
	* input			: pUserID         调度员账号
					  pPWD            调度员密码
					  pServerIP       调度机IP地址
	                  pLocalIP        本地主机IP地址
					  uiServerSIPPort 调度机SIP服务端口
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_Login(const ELTE_CHAR* pUserID, const ELTE_CHAR* pPWD, const ELTE_CHAR* pServerIP, const ELTE_CHAR* pLocalIP, ELTE_UINT32 uiServerSIPPort);

	/**************************************************************************
	* name			: ELTE_SDK_Logout
	* description   : 注销
	* input			: pUserID 调度员账号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_Logout(const ELTE_CHAR* pUserID);

	/**************************************************************************
	* name			: ELTE_SDK_TriggerStatusReport
	* description   : 触发状态上报
	* input			: iEnableStatusReport 是否开启状态上报
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_TriggerStatusReport(ELTE_INT32 iEnableStatusReport);

	/**************************************************************************
	* name			: ELTE_SDK_ProvisionManagerInit
	* description   : 配置管理初始化
	* input			: pServerIP 调度机IP地址
	                : pUserID   调度员账号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_ProvisionManagerInit(const ELTE_CHAR* pServerIP,const ELTE_CHAR* pUserID);

	/**************************************************************************
	* name			: ELTE_SDK_ProvisionManagerExit
	* description   : 退出配置管理
	* input			: N/A
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_ProvisionManagerExit();

	/**************************************************************************
	* name			: ELTE_SDK_GetDcGroups
	* description   : 获取调度台群组列表
	* input			: pUserID 调度台的用户号
	* output		: pDcGroups 该调度台所属的所有群组列表
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetDcGroups(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcGroups);

	/**************************************************************************
	* name			: ELTE_SDK_GetDcUsers
	* description   : 获取调度台用户列表
	* input			: pUserID 调度台的用户号
	* output		: pDcUsers 该调度台所管理的所有用户列表
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetDcUsers(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcUsers);

	/**************************************************************************
	* name			: ELTE_SDK_GetGroupUsers
	* description   : 获取一个群组成员列表
	* input			: pGroupID 群组号
	* output		: pGroupUsers 该群组的所有成员列表
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetGroupUsers(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupUsers);

	/**************************************************************************
	* name			: ELTE_SDK_GetGroupInfo
	* description   : 获取一个群组详细信息
	* input			: pGroupID 群组号
	* output		: pGroupInfo 该群组的详细信息
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetGroupInfo(const ELTE_CHAR* pGroupID, ELTE_CHAR** pGroupInfo);

	/**************************************************************************
	* name			: ELTE_SDK_GetUserInfo
	* description   : 获取一个用户详细信息
	* input			: pUserID 调度员账号
	* output		: pUserInfo 该用户的详细信息
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetUserInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pUserInfo);

	/**************************************************************************
	* name			: ELTE_SDK_GetDcInfo
	* description   : 获取一个用户详细信息
	* input			: pUserID 调度台的用户号
	* output		: pDcInfo 该调度台详细信息
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetDcInfo(const ELTE_CHAR* pUserID, ELTE_CHAR** pDcInfo);

	/**************************************************************************
	* name			: ELTE_SDK_SubscribeGroup
	* description   : 订阅群组
	* input			: pGroupID              群组号
	                  iEnableSubscribeGroup 是否订阅
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_SubscribeGroup(const ELTE_CHAR* pGroupID, ELTE_INT32 iEnableSubscribeGroup);

	/**************************************************************************
	* name			: ELTE_SDK_CreateDynamicGroup
	* description   : 创建动态群组
	* input			: pDGNAParam 动态组参数信息
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_CreateDynamicGroup(const ELTE_CHAR* pDGNAParam);

	/**************************************************************************
	* name			: ELTE_SDK_CancelDynamicGroup
	* description   : 删除动态群组
	* input			: pResourceID 动态组组号
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_CancelDynamicGroup(const ELTE_CHAR* pResourceID);

	/**************************************************************************
	* name			: ELTE_SDK_GetTempGroupID
	* description   : 获取临时组号
	* input			: N/A
	* output		: iGroupID 临时组组号
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetTempGroupID(ELTE_INT32* iGroupID);

	/**************************************************************************
	* name			: ELTE_SDK_CreateTempGroup
	* description   : 创建临时组
	* input			: pTEMPGParam 临时组参数信息
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_CreateTempGroup(const ELTE_CHAR* pTEMPGParam);

	/**************************************************************************
	* name			: ELTE_SDK_GetUserRECFileInfoList
	* description   : 获取录像文件列表
	* input			: pQueryXml 查询录像条件Xml
	* output		: pRspXml   查询录像文件结果列表
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_GetUserRECFileInfoList(const ELTE_CHAR* pQueryXml, ELTE_CHAR** pRspXml);

	/**************************************************************************
	* name			: ELTE_SDK_StartRealPlay
	* description   : 开始播放实时视频
	* input			: pResourceID 资源ID
	                  pVideoParam 媒体参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_StartRealPlay(const ELTE_CHAR* pResourceID, const ELTE_CHAR* pVideoParam);

	/**************************************************************************
	* name			: ELTE_SDK_PTZControl
	* description   : 开始云台控制
	* input			: pResourceID      带云台的摄像机资源ID
	                  iPTZControlCode  云台控制命令
					  iPTZControlValue 云台控制命令对应的参数
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_PTZControl(const ELTE_CHAR* pResourceID, ELTE_UINT32 iPTZControlCode, ELTE_UINT32 iPTZControlValue);

	/**************************************************************************
	* name			: ELTE_SDK_StopRealPlay
	* description   : 停止播放实时视频
	* input			: pResourceID      带云台的摄像机资源ID
	* output		: N/A
	* return		: 成功返回0，失败返回错误码
	**************************************************************************/
	ELTE_SDK_API ELTE_INT32 __SDK_CALL ELTE_SDK_StopRealPlay(const ELTE_CHAR* pResourceID);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif
