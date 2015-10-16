/********************************************************************
filename	: 	IVSSDK.cpp
author		:	z00201790
created		:	2012/10/23	
description	:	定义 DLL 应用程序的导出函数;
copyright	:	Copyright (C) 2011-2015
history		:	2012/10/23 初始版本;
*********************************************************************/

#ifndef IVS_SDK_H
#define IVS_SDK_H

#include "hwsdk.h"
#include "ivs_error.h"

#ifdef WIN32
#ifdef  IVS_SDK_EXPORTS
#define IVS_SDK_API __declspec(dllexport)
#else
#define IVS_SDK_API __declspec(dllimport)
#endif
#else
#define IVS_SDK_API __attribute__((visibility("default")))
#endif

#ifdef WIN32
#define __SDK_CALL __stdcall
#else
#define __SDK_CALL
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	/**************************************************************************
	* name			: IVS_SDK_Init
	* description	: 初始化SDK
	* input			: NA
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Init();

	/**************************************************************************
	* name			: IVS_SDK_Cleanup()
	* description	: 释放SDK
	* input			: NA
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Cleanup();

	/**************************************************************************
	* name			: IVS_SDK_GetVersion()
	* description	: 获取SDK版本号，当前版本号为2.2.0.1
	* input			: NA
	* output		: NA
	* return		: 成功返回SDK版本号，高16位：25位~32位表示主版本号，17~24位表示子版本号。低16位：9位~16位表示修正版本号，1~8位表示预留版本号;
	* remark		: 返回SDK版本号以16进制表示，比如2.2.0.1返回值为0x02020001
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetVersion();

	/**************************************************************************
	* name			: IVS_SDK_SetEventCallBack()
	* description	: 设置事件回调函数
	* input			: iSessionID        登录成功后的会话ID
	*					  fEventCallBack    用户设置的事件回调函数，为空表示取消设置的回调函数;
	*					  pUserData		 用户数据，自定义，相应回调函数被调用时返回该参数
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetEventCallBack(IVS_INT32 iSessionID, EventCallBack fEventCallBack = NULL, void *pUserData = NULL);

	/**************************************************************************
	* name			: IVS_SDK_Login
	* description	: 用户登录
	* input			: pLoginReqInfo 登录信息
    * output		: iSessionID 会话ID，SessionID由SDK内部产生，0-127的整数，作为其他接口的入参，用来标识用户的一次登录，最大支持的登录数为128	
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	**************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Login(IVS_LOGIN_INFO* pLoginReqInfo, IVS_INT32* iSessionID);
	
	/**************************************************************************
	* name       : IVS_SDK_LoginByTicket
	* description: 用户单点登录;
	* input      : IVS_LOGIN_INFO_EX 登陆入参结构体;
					(cTicketID		用户的票据，和用户唯一对应;
					stIP			服务器IP地址;
					uiPort			服务器端口;
					uiLoginType		登陆类型: 0 –基本身份验证;
					cDomainName		登录域名信息;
					cMachineName	客户端登录的机器名;
					uiClientType	客户端类型：参考 IVS_CLIENT_TYPE 0-PC客户端, 1-web浏览器, 
						2-移动客户端, 3-PC客户端子进程，4-电视墙进程;);
    * output     : pSessionId  返回sessionId SessionID由SDK内部产生，大于0的整数，
                        作为其他接口的入参，用来区分访问的服务器，最多登录128个服务器;
	* return     : 成功返回0，失败返回错误码;
	* remark     : NA
	**************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LoginByTicket(IVS_LOGIN_INFO_EX* pLoginReqInfoEx, IVS_INT32* pSessionId);
	
    /**************************************************************************
	* name			: IVS_SDK_WinUserLogin
	* description	: 用户登录
	* input			: pLoginReqInfo 登录信息
    * output		: iSessionID 会话ID，SessionID由SDK内部产生，0-127的整数，作为其他接口的入参，用来标识用户的一次登录，最大支持的登录数为128
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	**************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_WinUserLogin(IVS_LOGIN_INFO* pLoginReqInfo, IVS_INT32* iSessionID);

	/**************************************************************************
	* name			: IVS_SDK_Logout
	* description	: 用户注销
	* input			: iSessionID 登录成功后的会话ID
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Logout(IVS_INT32 iSessionID);

	/**************************************************************************
	* name			: IVS_SDK_ChangePWD
	* description	: 修改密码
	* input			: iSessionID 登录成功后的会话ID
						  pOldPWD    旧密码
						  pNewPWD    新密码
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	**************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ChangePWD(IVS_INT32 iSessionID, const IVS_CHAR* pOldPWD, const IVS_CHAR* pNewPWD);

	/******************************************************************
    function		: IVS_SDK_ResetPWD
    description	: 重置密码
    input			: iSessionID      登录成功后的会话ID
					  pDomainCode    域编码
					  iUserID     用户ID
					  pNewPWD      用户新密码
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ResetPWD(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_UINT32 uiUserID, const IVS_CHAR* pNewPWD);

    /******************************************************************
    function		: IVS_SDK_CheckPWDByRole
    description	: 校验密码是否满足账号规则
    input			: iSessionID     登录成功后的会话ID
					  pDomainCode    域编码
					  uiRoleID   角色ID
					  pLoginName  用户登录名
					  pPWD     用户设置的密码
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_CheckPWDByRole(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_UINT32 uiRoleID, const IVS_CHAR* pLoginName, const IVS_CHAR* pPWD);

    /******************************************************************
    function		: IVS_SDK_GetAutoCreatePWD
    description	: 根据角色获取符合账号规则的密码
    input			: iSessionID    登录成功后的会话ID
					  pDomainCode   域编码
					  uiRoleID    角色ID
					  pLoginName    用户登录名
					  uiPWDLen     密码长度
    output		: pNewPWD     密码字符串
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAutoCreatePWD(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID, const IVS_CHAR* pLoginName, IVS_CHAR* pNewPWD, IVS_UINT32 uiPWDLen);

    /******************************************************************
    function		: IVS_SDK_GetPWDStrength
    description	: 获取密码强度
    input			: pPWD  密码
    output		: uiPWDStrength  密码强度
    return		: 返回密码强度（值参考 IVS_KEY_RELIABILITY）
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPWDStrength(const IVS_CHAR* pPWD, IVS_UINT32* uiPWDStrength);

	/******************************************************************
    function		: IVS_SDK_GetPWDExpireInfo
    description	: 获取密码过期信息
	input			: iSessionID    登录成功后的会话ID
    output		: pPWDExpireInfo    密码过期信息
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPWDExpireInfo(IVS_INT32 iSessionID, IVS_PWD_EXPIRE_INFO* pPWDExpireInfo);

	/**************************************************************************
	* name			: IVS_SDK_GetUserID
	* description	: 获取登陆用户的用户ID
	* input			: iSessionID  登录成功后的会话ID
	* output		: puiUserID  用户ID
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	**************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserID(IVS_INT32 iSessionID, IVS_UINT32* puiUserID);

	/*************************************************************************
	* name			: IVS_SDK_AddRecordPlan
	* description	: 新增平台/前端录像计划
	* input			: iSessionID         登录成功后的会话ID
						  uiDeviceNum        摄像机个数
						  pDeviceList        摄像机编码列表
						  pRecordPlan        录像计划
						  uiResultBufferSize 缓存大小
	* output		: pResultList        录像计划新增结果
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRecordPlan(IVS_INT32 iSessionID,IVS_UINT32 uiDeviceNum,
		const IVS_DEVICE_CODE* pDeviceList,const IVS_RECORD_PLAN* pRecordPlan,IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_ModifyRecordPlan
	* description	: 修改平台/前端录像计划
	* input			: iSessionID         登录成功后的会话ID
						  uiDeviceNum        摄像机个数
						  pDeviceList        摄像机编码列表
						  pRecordPlan        录像计划
						  uiResultBufferSize 缓存大小
	* output		: pResultList        录像计划修改结果
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyRecordPlan(IVS_INT32 iSessionID,IVS_UINT32 uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList,
		const IVS_RECORD_PLAN* pRecordPlan,IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_DeleteRecordPlan
	* description	: 删除平台/前端录像计划
	* input			: iSessionID         登录成功后的会话ID
						  uiDeviceNum        摄像机个数
						  pDeviceList        摄像机编码列表
						  uiRecordMethod     录像方式：0-平台录像 1-前端录像
						  uiResultBufferSize 缓存大小
	* output		: pResultList        录像计划删除结果
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRecordPlan(IVS_INT32 iSessionID,IVS_UINT32 uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList,
		IVS_UINT32 uiRecordMethod,IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_GetRecordPlan
	* description	: 查询平台/前端录像计划
	* input			: iSessionID         登录成功后的会话ID
						  pCameraCode        摄像机编码
						  uiRecordMethod     录像方式：0-平台录像 1-前端录像
	* output		: pRecordPlan        录像计划
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordPlan(IVS_INT32 iSessionID,const char* pCameraCode,IVS_UINT32 uiRecordMethod,IVS_RECORD_PLAN *pRecordPlan);

	/*************************************************************************
	* name			: IVS_SDK_AddBookmark
	* description	: 新增录像书签
	* input			: iSessionID    登录成功后的会话ID
						  pCameraCode   摄像机编码
						  pNVRCode      NVR编码
						  pBookmarkName 书签名称
						  pBookmarkTime 书签时间
	* output		: uiBookmarkID  书签ID
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddBookmark(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_CHAR* pNVRCode,
		const IVS_CHAR* pBookmarkName,const IVS_CHAR* pBookmarkTime,IVS_UINT32* uiBookmarkID);

	/*************************************************************************
	* name			: IVS_SDK_ModifyBookmark
	* description	: 修改录像书签
	* input			: iSessionID    登录成功后的会话ID
						  pNVRCode      NVR编码
						  pCameraCode   摄像机编码
						  uiBookmarkID  书签ID
						  pBookmarkName 新书签名称
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyBookmark(IVS_INT32 iSessionID,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID,const IVS_CHAR* pNewBookmarkName);
	/*************************************************************************
	* name			: IVS_SDK_DeleteBookmark
	* description	: 删除录像书签
	* input			: iSessionID    登录成功后的会话ID
						  pNVRCode      NVR编码
						  pCameraCode   摄像机编码
						  uiBookmarkID  书签ID
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteBookmark(IVS_INT32 iSessionID,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID);

	/*************************************************************************
	* name			: IVS_SDK_GetBookmarkList
	* description	: 查询录像书签列表
	* input			: iSessionID    登录成功后的会话ID
						  pUnifiedQuery 查询条件
						  uiBufSize     缓存大小
	* output		: pBookmarkList 书签查询结果列表
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetBookmarkList(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, 
		IVS_RECORD_BOOKMARK_INFO_LIST* pBookmarkList,IVS_UINT32 uiBufSize);

	/*************************************************************************
	* name			: IVS_SDK_QueryRecordList
	* description	: 查询录像列表
	* input			: iSessionID      登录成功后的会话ID
						  pUnifiedQuery   查询条件
						  uiBufferSize    缓存大小
	* output		: pRecordInfoList 录像查询结果列表
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_QueryRecordList(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, 
		IVS_RECORD_INFO_LIST* pRecordInfoList,IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_GetRecordPolicyByTime
	* description	: 查询录像时间策略
	* input			: iSessionID    登录成功后的会话ID
						  pCameraCode   摄像机编码
	* output		: pRecordPolicy 录像策略
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordPolicyByTime(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_RECORD_POLICY_TIME* pRecordPolicy);

	/*************************************************************************
	* name         : IVS_SDK_SetRecordPolicyByTime
	* description : 设置录像时间策略
	* input			: iSessionID    登录成功后的会话ID
						  pCameraCode   摄像机编码
						  pRecordPolicy 录像策略
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetRecordPolicyByTime(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_RECORD_POLICY_TIME* pRecordPolicy);

	/*************************************************************************
	* name			: IVS_SDK_GetRecordList
	* description	: 查询录像列表
	* input			: iSessionID    登录成功后的会话ID
						  pCameraCode   摄像机ID
						  iRecordMethod 录像方式：0-平台录像 1-前端录像 2-备份服务器
						  pTimeSpan     查询时间段
						  pIndexRange   分页信息
						  iBufSize      缓存大小
	* output		: pRecordList   返回录像列表
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordList(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_INT32 iRecordMethod,const IVS_TIME_SPAN* pTimeSpan,
		const IVS_INDEX_RANGE* pIndexRange,IVS_RECORD_INFO_LIST* pRecordList,IVS_UINT32 uiBufSize);


	/*************************************************************************
	* name			: IVS_SDK_SubscribeAlarmQuery
	* description	: 告警订阅查询
	* input			: iSessionID 登录成功后的会话ID
						  pReqXml    告警订阅查询请求xml
	* output		: pRspXml    告警订阅查询响应xml
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SubscribeAlarmQuery(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/******************************************************************
	* name			: IVS_SDK_ReleaseBuffer
	* description	: 释放SDK内部申请的内存
	* input			: pBuffer 指向SDK内部申请内存的指针
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	  *******************************************************************/  
	  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ReleaseBuffer(IVS_CHAR *pBuffer);

	/*************************************************************************
	* name			: IVS_SDK_SubscribeAlarm
	* description	: 新增告警订阅
	* input			: iSessionID 登录成功后的会话ID
						  pReqXml    告警订阅请求xml
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SubscribeAlarm(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name			: IVS_SDK_AddAlarmLinkage
	* description	: 新增告警联动策略
	* input			: iSessionID 登录成功后的会话ID
						  pReqXml    请求消息
	* output		: pRspXml    响应消息，包含有联动策略ID
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddAlarmLinkage(IVS_INT32 iSessionID,	const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name			: IVS_SDK_ModifyAlarmLinkage
	* description	: 修改告警联动策略
	* input			: iSessionID 登录成功后的会话ID
						  pReqXml    请求消息
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyAlarmLinkage(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name			: IVS_SDK_DeleteAlarmLinkage
	* description	: 删除告警联动策略
	* input			: iSessionID 登录成功后的会话ID
						  pReqXml    请求消息
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteAlarmLinkage(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmlinkageList
	* description	: 查看告警联动策略列表
	* input			: iSessionID 登录成功后的会话ID
						  pReqXml    请求消息
	* output		: pRspXml    响应消息
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmlinkageList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmLinkage
	* description	: 查询告警联动策略
	* input			: iSessionID 登录成功后的会话ID
						  pReqXml    请求消息
	* output		: pRspXml    响应消息
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLinkage (IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name			: IVS_SDK_StartAlarmOut
	* description	: 启动开关量输出告警
	* input			: iSessionID    登录成功后的会话ID
						  pAlarmOutCode 开关量输出设备编码
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartAlarmOut(IVS_INT32 iSessionID, const IVS_CHAR* pAlarmOutCode);

	/*************************************************************************
	* name			: IVS_SDK_StopAlarmOut
	* description	: 停止开关量输出告警
	* input			: iSessionID    登录成功后的会话ID
						  pAlarmOutCode 开关量输出设备编码
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopAlarmOut(IVS_INT32 iSessionID, const IVS_CHAR* pAlarmOutCode);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmEventInfo
	* description	: 查询告警信息
	* input			: iSessionID      登录成功后的会话ID
						  ullAlarmEventID 告警事件ID
						  pAlarmInCode    告警源编码
	* output		: pAlarmEvent     事件详情
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmEventInfo(IVS_INT32 iSessionID, IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, IVS_ALARM_EVENT* pAlarmEvent);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmEventList
	* description	: 查询历史告警信息列表
	* input			: iSessionID      登录成功后的会话ID
						  pUnifiedQuery   查询条件
						  uiBufferSize    缓存大小
	* output		: pAlarmEventList 告警信息列表
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmEventList(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ALARM_EVENT_LIST* pAlarmEventList, IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmTypeList
	* description	: 查询告警类型列表
	* input			: iSessionID 登录成功后的会话ID
						  pUnifiedQuery 查询条件
						  uiBufferSize 缓存大小
	* output			: pAlarmTypeList 告警类型列表
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmTypeList (IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ALARM_TYPE_LIST* pAlarmTypeList, IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_SetAlarmTypeLevel
	* description	: 设置告警类型级别
	* input			: iSessionID 登录成功后的会话ID
						  pAlarmTypeList 告警类型列表
	* output			: NA
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetAlarmTypeLevel(IVS_INT32 iSessionID, const IVS_ALARM_TYPE_LIST* pAlarmTypeList);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmLevelList
	* description	: 查询告警级别列表
	* input			: iSessionID 登录成功后的会话ID
						  uiMaxAlarmLevelNum 系统最大告警级别个数IVS_MAX_ALARM_LEVEL_NUM
						  uiBufferSize  告警类型类表的内存空间大小
	* output			: puiAlarmLevelNum 查询返回的系统告警级别个数
						  pAlarmLevel 告警类型列表
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLevelList(IVS_INT32 iSessionID,
		IVS_UINT32 uiMaxAlarmLevelNum,IVS_UINT32* puiAlarmLevelNum,IVS_ALARM_LEVEL* pAlarmLevel,IVS_UINT32 uiBufferSize);

	/**************************************************************************
    * name			: IVS_SDK_GetAlarmLevel
    * description	: 查询告警级别详情
    * input			: iSessionID     登录成功后的会话ID
						  uiAlarmLevelID    告警级别ID
    * output			: pAlarmLevel    告警级别详情 
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark			: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLevel(IVS_INT32 iSessionID,IVS_UINT32 uiAlarmLevelID,IVS_ALARM_LEVEL* pAlarmLevel);

	/*************************************************************************
	* name			: IVS_SDK_AddAlarmArea
	* description	: 新增防区
	* input			: iSessionID 登录成功后的会话ID
						  pReq 请求XML
	* output			: pRsp 响应XML
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddAlarmArea(IVS_INT32 iSessionID, const IVS_CHAR* pReq, IVS_CHAR** pRsp);

	/*************************************************************************
	* name			: IVS_SDK_ModifyAlarmArea
	* description	: 修改防区
	* input			: iSessionID 登录成功后的会话ID
						  pReqXml 请求XML
	* output			: NA
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyAlarmArea(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name			: IVS_SDK_DeleteAlarmArea
	* description	: 删除防区
	* input			: iSessionID 登录成功后的会话ID
						  pDomainCode 目标域编码
						  uiAlarmAreaId 防区ID
	* output			: NA
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteAlarmArea(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiAlarmAreaId);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmAreaList
	* description	: 查看防区列表
	* input			: iSessionID 登录成功后的会话ID
						  pReqXml 请求XML
	* output			: pRspXml 响应XML
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmAreaList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name			: IVS_SDK_GetAlarmAreaInfo
	* description	: 查看防区详情
	* input			: iSessionID 登录成功后的会话ID
						  pDomainCode 域编码
						  uiAlarmAreaId 防区ID
	* output			: pAlarmArea 防区详情
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmAreaInfo(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId,IVS_CHAR** pAlarmArea);

	/*************************************************************************
	* name			: IVS_SDK_StartAlarmAreaGuard
	* description	: 手动布防
	* input			: iSessionID 登录成功后的会话ID
						  pDomainCode 域编码
						  uiAlarmAreaId  防区ID
	* output			: NA
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartAlarmAreaGuard(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId);

	/*************************************************************************
	* name			: IVS_SDK_SubscribeAlarmQuery
	* description	: 手动撤防
	* input			: iSessionID 登录成功后的会话ID
						  pDomainCode 域编码
						  uiAlarmAreaId  防区ID
	* output			: NA
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopAlarmAreaGuard(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId);

	/*************************************************************************
	* name			: IVS_SDK_AllocAreaGuardPlan
	* description	: 新增布防计划
	* input			: iSessionID 登录成功后的会话ID
						  pReqXml 请求XML
	* output			: NA
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AllocAreaGuardPlan(IVS_INT32 iSessionID,const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name			: IVS_SDK_GetAreaGuardPlan
	* description	: 查看布防计划
	* input			: iSessionID 登录成功后的会话ID
						  pDomainCode 域编码
						  uiAlarmAreaId 防区ID
	* output			: pRspXml 响应XML
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAreaGuardPlan(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId, IVS_CHAR** pRspXml);

	/*********************************************************************************************
	function		: IVS_SDK_SetDeviceGuard
	description	: 设备布撤防
	input			: iSessionID 登录成功后的会话ID
					  pReqXml  请求XML消息
	output		: NA
	return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	**********************************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDeviceGuard(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name			: IVS_SDK_QueryDeviceList
	* description	: 查询设备列表
	* input			: iSessionID 登录成功后的会话ID
						  pDomainCode 域编码
						  uiDeviceType 设备类型，取值参考IVS_DEVICE_TYPE，目前支持根据条件查询的设备类型是：
                                1-主设备
						  pQueryFormat 通用查询结构体
						  uiBufferSize 缓存大小
	* output		: pBuffer  分配的内存，和uiDeviceType对应
                                1-主设备：IVS_DEVICE_BRIEF_INFO_LIST
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_QueryDeviceList(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiDeviceType, 
		const IVS_QUERY_UNIFIED_FORMAT *pQueryFormat, 
		IVS_VOID* pBuffer, 
		IVS_UINT32 uiBufferSize);

	/******************************************************************
	* name			: IVS_SDK_GetDeviceList
	* description	: 获取设备列表
	* input			: iSessionID   登录成功后的会话ID
						  uiDeviceType 设备类型，取值参考IVS_DEVICE_TYPE，目前支持获取以下3种设备列表：
                                1-主设备
								2-摄像机设备
                                3-语音设备
                                4-告警设备
						  pIndexRange  分页信息
						  uiBufferSize 缓存大小
	* output		: pDeviceList  设备列表指针，具体说明如下：
                                主设备：IVS_DEVICE_BRIEF_INFO_LIST
								摄像机设备：IVS_CAMERA_BRIEF_INFO_LIST
                                语音设备：IVS_AUDIO_BRIEF_INFO_LIST
                                告警设备：IVS_ALARM_BRIEF_INFO_LIST
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceList(
		IVS_INT32 iSessionID,
		IVS_UINT32 uiDeviceType,
		const IVS_INDEX_RANGE* pIndexRange,
		IVS_VOID* pDeviceList,
		IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_AddDeviceGroup
	* description	: 新增设备组
	* input			: iSessionID 登录成功后的会话ID
						  pDevGroup 设备组信息
	* output			: puiDevGroupID 设备组ID
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddDeviceGroup (IVS_INT32 iSessionID, const IVS_DEV_GROUP* pDevGroup,IVS_UINT32* puiDevGroupID);

	/*************************************************************************
    * name			: IVS_SDK_DeleteDeviceGroup
    * description	: 删除设备组
    * input			: iSessionID 登录成功后的会话ID
						  pDomainCode 删除设备组所属的域编码
						  uiDevGroupID 删除的设备组ID
    * output			: NA
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark			: NA
    *************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteDeviceGroup( IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID);

	/**************************************************************************
    * name			: IVS_SDK_ModifyDeviceGroupName
    * description	: 修改设备组名称
    * input			: iSessionID  登录成功后的会话ID
						  pDomainCode  设备组所属域编码
						  uiDevGroupID  设备组ID
						  pNewDevGroupName  设备组新名称
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark			: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyDeviceGroupName(IVS_INT32 iSessionID,  const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID, const IVS_CHAR* pNewDevGroupName);

	/*************************************************************************
	* name			: IVS_SDK_MoveDeviceGroup
	* description	: 移动设备组
	* input			: iSessionID 登录成功后的会话ID
						  pDomainCode  设备组所属域编码
						  uiNewParentDevGroupID  设备组的新父组ID
						  uiDevGroupID  设备组ID
	* output			: NA
	* return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_MoveDeviceGroup(IVS_INT32 iSessionID,  const IVS_CHAR* pDomainCode, IVS_UINT32 uiNewParentDevGroupID, IVS_UINT32 uiDevGroupID);

	/*************************************************************************
	* name			: IVS_SDK_GetDeviceGroup
	* description	: 查询设备组列表
	* input			: iSessionID    登录成功后的会话ID
						  pDomainCode   设备组所属域编码
						  pDevGroupCode 设备组ID
						  uiBufferSize  缓存大小
	* output		: pDeviceGroupList 设备组列表
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/		
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceGroup(IVS_INT32 iSessionID,
                                                            const IVS_CHAR* pDomainCode,
                                                            const IVS_CHAR* pDevGroupCode,
                                                            IVS_DEVICE_GROUP_LIST* pDeviceGroupList,
                                                            IVS_UINT32 uiBufferSize);

	/******************************************************************
    function		: IVS_SDK_AddDeviceToGroup
    description	: 新增设备组下设备
    input			: iSessionID  登录成功后的会话ID
					  pTargetDomainCode  目标设备组域编码
					  uiTargetDevGroupID  目标设备组ID
					  uiDeviceNum  新增设备数量
					  pGroupCameraList  新增设备信息数组指针
    output		: NA
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddDeviceToGroup( IVS_INT32 iSessionID, 
                                        const IVS_CHAR* pTargetDomainCode,
                                        IVS_UINT32 uiTargetDevGroupID,
                                        IVS_UINT32 uiDeviceNum,
                                        const IVS_DEV_GROUP_CAMERA* pGroupCameraList);

	/******************************************************************
    function		: IVS_SDK_DeleteDeviceFromGroup
    description	: 删除设备组下设备
    input			: iSessionID  登录成功后的会话ID
					  pTargetDomainCode  目标设备组域编码
					  uiDeviceNum  删除设备数量
					  pGroupCameraList  删除设备信息数组指针
    output		: NA
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteDeviceFromGroup( IVS_INT32 iSessionID, 
                                                                    const IVS_CHAR* pTargetDomainCode,
                                                                    IVS_UINT32 uiDeviceNum,
                                                                    const IVS_DEV_GROUP_CAMERA* pGroupCameraList);

    /**************************************************************************
    * name			: IVS_SDK_StartRealPlay
    * description	: 开始实时浏览
    * input			: iSessionID    登录成功后的会话ID
						  pRealplayPara 实况参数，详见实况参数结构体
						  pCamerCode    摄像头编码
						  hWnd          窗口句柄
    * output		: pHandle       播放句柄
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlay(IVS_INT32 iSessionID, IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, HWND hWnd, IVS_ULONG* pHandle);

	/**************************************************************************
    * name			: IVS_SDK_StartRealPlayCBRaw
    * description	: 开始实时浏览，以回调方式输出拼帧后的裸码流
    * input			: iSessionID           登录成功后的会话ID
						  pRealplayPara        实况参数，详见实况参数结构体
						  pCamerCode           摄像头编码
						  fRealPlayCallBackRaw 实时浏览回调函数
						  pUserData            用户数据，自定义，相应回调函数被调用时返回该参数
    * output		: pHandle              播放句柄
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayCBRaw(IVS_INT32 iSessionID, IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, 
                                                                RealPlayCallBackRaw fRealPlayCallBackRaw, void* pUserData, IVS_ULONG* pHandle );

	/**************************************************************************
    * name			: IVS_SDK_StartRealPlayCBFrame
    * description	: 开始实时浏览，以回调方式输出YUV流
    * input			: iSessionID               登录成功后的会话ID
						  pRealplayPara            实况参数，详见实况参数结构体
						  pCamerCode               摄像头编码
						  fRealPlayCallBackFrame   实时浏览回调函数
						  pUserData                用户数据，自定义，相应回调函数被调用时返回该参数
    * output		: pHandle                  播放句柄
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayCBFrame(IVS_INT32 iSessionID, IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, 
                                                                  RealPlayCallBackFrame fRealPlayCallBackFrame, void* pUserData, IVS_ULONG* pHandle);

	/**************************************************************************
    * name			: IVS_SDK_StopRealPlay
    * description	: 停止实时浏览
    * input			: iSessionID 登录成功后的会话ID
						  ulHandle   播放句柄
    * output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlay( IVS_INT32 iSessionID, IVS_ULONG ulHandle);

	/**************************************************************************
    * name			: IVS_SDK_AddPlayWindowPartial
    * description	: 添加局部放大窗口
    * input			: iSessionID   登录成功后的会话ID
						  ulPlayHandle 播放句柄
						  hWnd         窗口句柄
						  pPartial     局部放大描述矩形，表示原图位置百分比，具体说明如下：
								需要放大矩形坐标top、bottom、left、right
								图像尺寸width、height
								pPartial->top = top*100/height
								pPartial->bottom = bottom*100/height
								pPartial->left = left*100/width
								pPartial->right = right*100/width

    * output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddPlayWindowPartial(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd, void *pPartial);

	/**************************************************************************
    * name			: IVS_SDK_UpdateWindowPartial
    * description	: 刷新局部放大窗口，如放大区域位置移动等
    * input			: iSessionID   登录成功后的会话ID
						  ulPlayHandle 播放句柄
						  hWnd         窗口句柄
						  pPartial     局部放大描述矩形，表示原图位置百分比，具体说明如下：
								需要放大矩形坐标top、bottom、left、right
								图像尺寸width、height
								pPartial->top = top*100/height
								pPartial->bottom = bottom*100/height
								pPartial->left = left*100/width
								pPartial->right = right*100/width
    * output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UpdateWindowPartial(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd, void *pPartial);

	/**************************************************************************
    * name			: IVS_SDK_StartPlatformPlayBack
    * description	: 开始平台录像回放
    * input			: iSessionID      登录成功后的会话ID
						  pCameraCode     摄像机编码
						  pPlaybackParam  回放参数
						  hWnd            窗口句柄
    * output		: pHandle         播放句柄
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBack(IVS_INT32 iSessionID,
                                                                   const IVS_CHAR* pCameraCode,
                                                                   const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                                                   HWND hWnd,
                                                                   IVS_ULONG* pHandle);

   /**************************************************************************
    * name			: IVS_SDK_StopPlatformPlayBack
    * description	: 停止平台录像回放
    * input			: iSessionID      会话ID
						  ulPlayHandle    播放句柄
    * output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformPlayBack(IVS_INT32 iSessionID,IVS_ULONG ulPlayHandle);

	/**************************************************************************
    * name			: IVS_SDK_StartPlatformPlayBackByIP
    * description	: 开始客户端平台录像回放
    * input			: iSessionID      会话ID
						  pCameraCode     摄像机编码
						  pPlaybackParam  回放参数
						  pMediaAddrDst  媒体流目标地址
	* output			: pMediaAddrSrc 媒体流源地址
						  pHandle 播放句柄
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark			: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBackByIP(IVS_INT32 iSessionID,
																	const IVS_CHAR* pCameraCode,
																	const IVS_PLAYBACK_PARAM* pPlaybackParam,
																	const IVS_MEDIA_ADDR* pMediaAddrDst,
																	IVS_MEDIA_ADDR* pMediaAddrSrc,
																	IVS_ULONG* pHandle);

   /**************************************************************************
    * name			: IVS_SDK_StopPlatformPlayBackByIP
    * description	: 停止客户端平台录像回放
    * input			: iSessionID      会话ID
						  ulPlayHandle    播放句柄
    * output			: NA
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark			: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformPlayBackByIP(IVS_INT32 iSessionID,IVS_ULONG ulPlayHandle);
	
	/**************************************************************************
    * name			: IVS_SDK_PlatformPlayBackPauseByIP
    * description: 暂停平台录像回放
    * input			: iSessionID	会话ID
							: ulPlayHandle	播放句柄
    * output			: NA
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlatformPlayBackPauseByIP(IVS_INT32 iSessionID,
																	IVS_ULONG ulPlayHandle);

	/**************************************************************************
    * name			: IVS_SDK_PlatformPlayBackResumeByIP
    * description: 恢复平台录像回放
    * input			: iSessionID	会话ID
							: ulPlayHandle	播放句柄
    * output			: NA
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlatformPlayBackResumeByIP(IVS_INT32 iSessionID,
																	IVS_ULONG ulPlayHandle);

	 /**************************************************************************
    * name			: IVS_SDK_SetPlayBackSpeedByIP
    * description: 设置平台录像码流回放速度
    * input			: iSessionID	会话ID
							: ulPlayHandle	播放句柄
							: fSpeed	播放速度
    * output			: NA
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackSpeedByIP(IVS_INT32 iSessionID, 
		IVS_ULONG ulPlayHandle, 
		IVS_FLOAT fSpeed);

	 /**************************************************************************
    * name			: IVS_SDK_SetPlayBackTimeByIP
    * description: 设置平台录像码流回放时间
    * input			: iSessionID	会话ID
							: ulPlayHandle	播放句柄
							: uiTime	拖动尺度，单位秒，正整数，绝对时间
    * output			: NA
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackTimeByIP(IVS_INT32 iSessionID, 
		IVS_ULONG ulPlayHandle, 
		IVS_UINT32 uiTime);

	/**************************************************************************
    * name			: IVS_SDK_StartLocalPlayBack
    * description	: 开始本地录像回放
    * input			: iSessionID      登录成功后的会话ID
						  pFileName       本地录像文件
						  pPlaybackParam  回放参数
						  hWnd            窗口句柄
    * output		: pHandle         播放句柄
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 IVS_SDK_StartLocalPlayBack (
		                       IVS_INT32 iSessionID,
		                       const IVS_CHAR* pFileName,
		                       const IVS_LOCAL_PLAYBACK_PARAM* pPlaybackParam,
		                       HWND hWnd, 
							   IVS_ULONG* pHandle);
	
	/**************************************************************************
    * name			: IVS_SDK_StartPlatformPlayBack
    * description	: 停止本地录像回放
    * input			: iSessionID      登录成功后的会话ID
						  ulPlayHandle    回放句柄
    * output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 IVS_SDK_StopLocalPlayBack(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);

    /**************************************************************************
    * name			: IVS_SDK_StartPlatformPlayBackCBRaw
    * description	: 开始平台录像回放(裸码流回调)
    * input			: iSessionID           登录成功后的会话ID
						  pCameraCode          摄像机编码
						  pPlaybackParam       回放参数
						  fPlayBackCallBackRaw 录像回放裸码流回调函数
						  pUserData            用户数据，自定义，相应回调函数被调用时返回该参数
    * output		: pHandle              播放句柄
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBackCBRaw(IVS_INT32 iSessionID,
																		const IVS_CHAR* pCameraCode,
																		const IVS_PLAYBACK_PARAM* pPlaybackParam,
																		PlayBackCallBackRaw fPlayBackCallBackRaw, 
																		void* pUserData,
																		IVS_ULONG* pHandle);


	/**************************************************************************
    * name			: IVS_SDK_StartPlatformPlayBackCBFrame
    * description	: 开始平台录像回放(帧码流回调)
    * input			: iSessionID             登录成功后的会话ID
						  pCameraCode            摄像机编码
						  pPlaybackParam         回放参数
						  fPlayBackCallBackFrame 录像回放解码后帧码流回调函数
						  pUserData              用户数据，自定义，相应回调函数被调用时返回该参数
    * output		: pHandle                播放句柄
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBackCBFrame(IVS_INT32 iSessionID,
		   																  const IVS_CHAR* pCameraCode,
																		  const IVS_PLAYBACK_PARAM* pPlaybackParam,
																		  PlayBackCallBackFrame fPlayBackCallBackFrame, 
																		  void* pUserData,
																		  IVS_ULONG* pHandle);


    /******************************************************************
     function		: IVS_SDK_PlayBackPause
     description	: 暂停回放
     input			: iSessionID   登录成功后的会话ID
						  ulPlayHandle 播放句柄
     output			: NA
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlayBackPause(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);

    /******************************************************************
     function		: IVS_SDK_PlayBackResume
     description	: 恢复回放
     input			: iSessionID   登录成功后的会话ID
						  ulPlayHandle 播放句柄
     output			: NA
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlayBackResume(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);

	/******************************************************************
	 function		: IVS_SDK_GetPlayBackStatus
	 description	: 获取回放状态
     input			: iSessionID   登录成功后的会话ID
						  ulPlayHandle 播放句柄
     output			: pbPause      播放状态：0-暂停，1-播放
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	*******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPlayBackStatus(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_BOOL* pbPause);

    /******************************************************************
     function		: IVS_SDK_PlayBackFrameStepForward
     description	: 单帧步进
     input			: iSessionID   登录成功后的会话ID
						  ulPlayHandle 播放句柄
     output			: NA
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlayBackFrameStepForward(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);

    /******************************************************************
     function		: IVS_SDK_PlayBackFrameStepBackward
     description	: 单帧回退
     input			: iSessionID   登录成功后的会话ID
						  ulPlayHandle 播放句柄
     output			: NA
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlayBackFrameStepBackward(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);

    /******************************************************************
     function		: IVS_SDK_SetPlayBackSpeed
     description	: 设置回放速度
     input			: iSessionID   登录成功后的会话ID
						  ulPlayHandle 播放句柄
						  fSpeed       播放速度
     output			: NA
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackSpeed(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_FLOAT fSpeed);

	/******************************************************************
	 function		: IVS_SDK_GetPlayBackSpeed
	 description	: 获取回放速度
     input			: iSessionID   登录成功后的会话ID
						  ulPlayHandle 播放句柄
     output			: fSpeed       播放速度
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	*******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPlayBackSpeed(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_FLOAT* pfSpeed);

    /******************************************************************
     function		: IVS_SDK_SetPlayBackTime
     description	: 回放拖动
     input			: iSessionID   登录成功后的会话ID
						  ulPlayHandle 播放句柄
						  uiTime       拖动尺度，单位秒，正整数，绝对时间
     output			: NA
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackTime(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_UINT32 uiTime);

    /******************************************************************
     function		: IVS_SDK_GetPlayBackTime
     description	: 获取当前回放时间
     input			: iSessionID    登录成功后的会话ID
						  ulPlayHandle  播放句柄
     output			: pPlayBackTime 回放时间
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPlayBackTime(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_PLAYBACK_TIME* pPlayBackTime);

	/******************************************************************
     function		: IVS_SDK_StartPlatformDownload
     description	: 开始平台录像下载
     input			: iSessionID      登录成功后的会话ID
						  pCameraCode     摄像机编码
						  pDownloadParam  录像下载参数
     output			: pulHandle       录像下载句柄
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformDownload(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode,
		const IVS_DOWNLOAD_PARAM* pDownloadParam, IVS_ULONG* pulHandle);

	/******************************************************************
     function		: IVS_SDK_StopPlatformDownload
     description	: 停止平台录像下载
     input			: iSessionID 登录成功后的会话ID
						  ulHandle   录像下载句柄
	 output			: NA
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformDownload(IVS_INT32 iSessionID, IVS_ULONG ulHandle);

	/******************************************************************
     function		: IVS_SDK_StartPUDownload
     description	: 开始前端录像下载
     input			: iSessionID      登录成功后的会话ID
						  pCameraCode     摄像机编码
						  pDownloadParam  录像下载参数
     output			: pulHandle       录像下载句柄
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPUDownload(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode,
					const IVS_DOWNLOAD_PARAM* pDownloadParam, IVS_ULONG* pulHandle);

	/******************************************************************
     function		: IVS_SDK_StopPUDownload
     description	: 停止前端录像下载
     input			: iSessionID 登录成功后的会话ID
						  ulHandle   录像下载句柄
	 output			: NA
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPUDownload(IVS_INT32 iSessionID, IVS_ULONG ulHandle);

	/******************************************************************
     function		: IVS_SDK_DownloadPause
     description	: 暂停录像下载
     input			: iSessionID 登录成功后的会话ID
						  ulHandle   录像下载句柄
	 output			: NA
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DownloadPause(IVS_INT32 iSessionID, IVS_ULONG ulHandle);

	/******************************************************************
     function		: IVS_SDK_DownloadResume
     description	: 恢复录像下载
     input			: iSessionID 登录成功后的会话ID
						  ulHandle   录像下载句柄
	 output			: NA
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DownloadResume(IVS_INT32 iSessionID,IVS_ULONG ulHandle);

	/******************************************************************
     function		: IVS_SDK_GetDownloadInfo
     description	: 获取下载信息
     input			: iSessionID    登录成功后的会话ID
						  ulHandle      录像下载句柄
	 output			: pDownloadInfo 录像下载信息
     return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDownloadInfo(IVS_INT32 iSessionID, 
		              IVS_ULONG ulHandle, IVS_DOWNLOAD_INFO* pDownloadInfo);

	/**************************************************************************
    * name			: IVS_SDK_StartPUPlayBack
    * description	: 开始前端录像回放
    * input			: iSessionID      登录成功后的会话ID
						  pCameraCode     摄像机编码
						  pPlaybackParam  回放参数
						  hWnd            窗口句柄
    * output		: pHandle         回放句柄
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark		: NA
    **************************************************************************/
#ifdef WIN32
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPUPlayBack(
                                    IVS_INT32 iSessionID,
                                    const IVS_CHAR* pCameraCode,
                                    const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                    HWND hWnd,
                                    IVS_ULONG* pHandle);
#endif
   /**************************************************************************
    * name			: IVS_SDK_StopPUPlayBack
    * description	: 停止前端录像回放
    * input			: iSessionID      登录成功后的会话ID
						  ulPlayHandle    播放句柄
    * output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPUPlayBack(IVS_INT32 iSessionID,
                                                       IVS_ULONG ulPlayHandle);

	/**************************************************************************
    * name			: IVS_SDK_GetMediaInfo
    * description	: 获取媒体信息
    * input			: iSessionID     登录成功后的会话ID
						  ulHandle       业务句柄，包括实况、回放
    * output		: pMediaInfo     媒体信息
    * return		: 成功返回0（IVS_SUCCEED）；失败返回错误码
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetMediaInfo(IVS_INT32 iSessionID, IVS_ULONG ulHandle, IVS_MEDIA_INFO* pMediaInfo);

	/**************************************************************************
	* name			: IVS_SDK_SetDisplayScale
	* description	: 设置播放显示比例
	* input			: iSessionID        登录成功后的会话ID
						  ulHandle          业务句柄，包括实况、回放
						  iDisplayScale     显示比例值，1-原始比例显示 2-铺满窗口 
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED）；失败返回错误码
	* remark		: NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDisplayScale(IVS_INT32 iSessionID, IVS_ULONG ulHandle,IVS_INT32 iDisplayScale);

	/**************************************************************************
    * name			: IVS_SDK_GetNVRList
    * description	: 查询NVR设备列表
    * input			: iSessionID     登录成功后的会话ID
						  pDomainCode    域编码
						  uiServerType   NVR类型，具体类型如下：
                                  0 全部
                                  1 NVR
                                  2 TAS
                                  3 MBS
						  pIndexRange    分页索引
						  uiBufferSize   缓存大小
    * output		: pNvrList       Nvr列表      
    * return		: 成功返回0（IVS_SUCCEED）；失败返回错误码
    * remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetNVRList(IVS_INT32 iSessionID, 
		                                                const IVS_CHAR* pDomainCode,
		                                                IVS_UINT32 uiServerType,
		                                                const IVS_INDEX_RANGE* pIndexRange,
		                                                IVS_DEVICE_BRIEF_INFO_LIST* pNvrList,
		                                                IVS_UINT32 uiBufferSize);

	/**************************************************************************
    * name			: IVS_SDK_SetDeviceConfig
    * description	: 设置设备参数
    * input			: iSessionID        登录成功后的会话ID
						  pDevCode          设备编码
						  uiConfigType      参数类型，值参考IVS_CONFIG_TYPE
						  pBuffer           数据缓存，使用的数据结构体参考IVS_CONFIG_TYPE里说明
						  uiBufferSize      缓存大小
    * output		: NA        
    * return		: 成功返回0（IVS_SUCCEED）；失败返回错误码
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDeviceConfig(IVS_INT32 iSessionID,
                                                             const IVS_CHAR* pDevCode, 
                                                             IVS_UINT32 uiConfigType,
                                                             IVS_VOID* pBuffer,
                                                             IVS_UINT32 uiBufSize);

	/**************************************************************************
    * name			: IVS_SDK_GetDeviceConfig
    * description	: 获取设备参数
    * input			: iSessionID        登录成功后的会话ID
						  pDevCode          设备编码
						  uiConfigType      参数类型，值参考IVS_CONFIG_TYPE
						  uiBufferSize      缓存大小
    * output		: pBuffer           数据缓存，使用的数据结构体参考IVS_CONFIG_TYPE里说明       
    * return		: 成功返回0（IVS_SUCCEED）；失败返回错误码
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceConfig(IVS_INT32 iSessionID,
                                                             const IVS_CHAR* pDevCode,
                                                             IVS_UINT32 uiConfigType,
                                                             IVS_VOID* pBuffer,
                                                             IVS_UINT32 uiBufSize);

  /**************************************************************************
    * name			: IVS_SDK_StartPlatformRecord
    * description	: 开始平台录像
    * input			: iSessionID      登录成功后的会话ID
						  pCameraCode     开始平台录像的摄像机编码
						  iRecordTime     录像时长，单位秒，整数，300 ~ 43200（12小时）
    * return		: 成功返回0（IVS_SUCCEED）；失败返回错误码
    * remark		: NA
	**************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformRecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_INT32 iRecordTime);

  /**************************************************************************
    * name			: IVS_SDK_StopPlatformRecord
    * description	: 停止平台录像
    * input			: iSessionID      登录成功后的会话ID
						  pCameraCode     停止平台录像的摄像机编码
    * return		: 成功返回0（IVS_SUCCEED）；失败返回错误码
    * remark		: NA
  **************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformRecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode);

  /**************************************************************************
  * name			: IVS_SDK_StartPURecord(
  * description	: 开始前端录像
  * input			: iSessionID      登录成功后的会话ID
						  pCameraCode     开始前端录像的摄像机编码
						  iRecordTime     录像时长
  * return			: 成功返回0（IVS_SUCCEED）；失败返回错误码
  * remark			: NA
  **************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPURecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_INT32 iRecordTime);

  /**************************************************************************
    * name			: IVS_SDK_StopPURecord
    * description	: 停止前端录像
    * input			: iSessionID      登录成功后的会话ID
						  pCameraCode     停止平台录像的摄像机编码
    * return		: 成功返回0（IVS_SUCCEED）；失败返回错误码
    * remark		: NA
  **************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPURecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode);

  /**************************************************************************
   * name			: IVS_SDK_StartLocalRecord
   * description	: 开始本地录像
   * input			: iSessionID     登录成功后的会话ID
						  pRecordParam   本地录像参数
						  ulPlayHandle   播放句柄
						  pSaveFileName  保存文件名
   * return			: 成功返回0（IVS_SUCCEED）；失败返回错误码
   * remark		: NA
   **************************************************************************/
   IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartLocalRecord(IVS_INT32 iSessionID,
	                                                         const IVS_LOCAL_RECORD_PARAM* pRecordParam, 
                                                             IVS_ULONG ulPlayHandle, 
                                                             const IVS_CHAR* pSaveFileName);

  /**************************************************************************
   * name			: IVS_SDK_StopLocalRecord
   * description	: 停止本地录像
   * input			: iSessionID      登录成功后的会话ID
						  ulPlayHandle    播放句柄
   * return			: 成功返回0（IVS_SUCCEED）； 失败返回错误码
   * remark		: NA
   **************************************************************************/
   IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopLocalRecord(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);

  /******************************************************************
  function		: IVS_SDK_GetDomainRoute
  description	: 查询域路由信息
  input			: iSessionID       登录成功后的会话ID
					  uiBufferSize     缓存大小
  output			: pDomainRouteList 路由信息列表
  return			: 成功返回0（IVS_SUCCEED）；失败返回错误码
  *******************************************************************/  
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDomainRoute( IVS_INT32 iSessionID, 
                          IVS_DOMAIN_ROUTE_LIST* pDomainRouteList,
                          IVS_UINT32 uiBufferSize);
						  
  /******************************************************************
  function   : IVS_SDK_GetPTZAbsPosition
  description: 获取云台当前位置
  input      : iSessionID 登录成功后返回的会话ID
			   pDomainCode 域编码
               pCameraCode 摄像机编码
  output     : pPTZPosInfo 云台位置信息
  return     : 成功返回0；失败返回错误码
  *******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPTZAbsPosition(IVS_INT32 iSessionID,
	  const IVS_CHAR* pDomainCode, const IVS_CHAR* pCameraCode, IVS_PTZ_ABSPOSITION *pPtzAbsPosition);

  /******************************************************************
  function		: IVS_SDK_PtzControl
  description	: 云镜控制
  input			: iSessionID    登录成功后的会话ID
					  pCameraCode   摄像机编码
					  iControlCode  云台控制码，值参考IVS_PTZ_CODE
					  pControlPara1 参数1
					  pControlPara2 参数2
  output			: pLockStatus   云台锁定状态：0-解锁，1-锁定
  return			: 成功返回0（IVS_SUCCEED）；失败返回错误码
  *******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PtzControl(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode, IVS_INT32 iControlCode,const IVS_CHAR* pControlPara1,const IVS_CHAR* pControlPara2, IVS_UINT32* pLockStatus);

  /******************************************************************
	function		: IVS_SDK_PtzControlWithLockerInfo
	description: 云镜控制
	input			: iSessionID 登录成功后返回的会话ID
					  pCameraCode 摄像机编码
					  iControlCode 云台控制码
					  pControlPara1 参数1
					  pControlPara2 参数2
	output		: pPtzControlInfo 云台控制信息
	return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	*******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PtzControlWithLockerInfo(IVS_INT32 iSessionID,
															  const IVS_CHAR* pCameraCode, 
															  IVS_INT32 iControlCode,
															  const IVS_CHAR* pControlPara1,
															  const IVS_CHAR* pControlPara2, 
															  IVS_PTZ_CONTROL_INFO* pPtzControlInfo);

    /*************************************************************************
	* name			: IVS_SDK_PlaySound
	* description	: 播放随路语音
	* input			: iSessionID   登录成功后的会话ID, 
						  uiPlayHandle 播放句柄
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlaySound(IVS_INT32 iSessionID, IVS_UINT32 uiPlayHandle);

    /*************************************************************************
	* name			: IVS_SDK_StopSound
	* description	: 停止播放随路语音
	* input			: iSessionID   登录成功后的会话ID, 
						  uiPlayHandle 播放句柄
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopSound(IVS_INT32 iSessionID, IVS_UINT32 uiPlayHandle);


    /*************************************************************************
	* name			: IVS_SDK_SetVolume
	* description	: 设置音量
	* input			: iSessionID    登录成功后的会话ID, 
						  uiPlayHandle  播放句柄
						  uiVolumeValue 音量，取值范围0~100
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetVolume(IVS_INT32 iSessionID,
                                                       IVS_UINT32 uiPlayHandle, 
                                                       IVS_UINT32 uiVolumeValue);


    /*************************************************************************
	* name			: IVS_SDK_GetVolume
	* description	: 获取音量
	* input			: iSessionID    登录成功后的会话ID, 
						  uiPlayHandle  播放句柄
	* output		: puiVolumeValue 音量，取值范围0~100
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetVolume(IVS_INT32 iSessionID,
                                                       IVS_UINT32 uiPlayHandle, 
                                                       IVS_UINT32* puiVolumeValue);

	/**************************************************************************
    * name			: IVS_SDK_LocalSnapshot
    * description	: 本地抓拍单张图片
    * input			: iSessionID      登录成功后的会话ID
						  ulPlayHandle    播放句柄
						  uiPictureFormat 图片格式：1-JPG，2-BMP
						  pFileName	   抓拍图片绝对路径，最大长度为256字节（包括后缀和结束符），文件后缀名和图片格式保持一致（.jpg、.bmp）
	* output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LocalSnapshot(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_UINT32 uiPictureFormat, const IVS_CHAR* pFileName);
	
	/**************************************************************************
    * name			: IVS_SDK_AddPTZPreset
    * description	: 增加预置位
    * input			: iSessionID    登录成功后的会话ID
						  pCameraCode   摄像机编码
						  pPTZPreset    预置位名称
	* output		: uiPresetIndex 预置位索引号
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddPTZPreset (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CHAR* pPresetName, IVS_UINT32* uiPresetIndex);

	/**************************************************************************
    * name			: IVS_SDK_DelPTZPreset
    * description	: 删除预置位
    * input			: iSessionID    登录成功后的会话ID
						  pCameraCode   摄像机编码
						  uiPresetIndex 预置位索引
	* output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelPTZPreset (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiPresetIndex);

	/**************************************************************************
    * name			: IVS_SDK_ModPTZPreset
    * description	: 修改预置位
    * input			: iSessionID  登录成功后的会话ID
						  pCameraCode 摄像机编码
						  pPTZPreset  预置位信息
	* output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModPTZPreset (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_PTZ_PRESET* pPTZPreset);

	/**************************************************************************
    * name			: IVS_SDK_GetPTZPresetList
    * description	: 查询预置位列表
    * input			: iSessionID     登录成功后的会话ID
						  pCameraCode    摄像机编码
						  pPTZPresetList 预置位信息列表（最多128个，请预先分配128个的内存空间）
						  uiBufferSize   缓存大小
	* output		: uiPTZPresetNum 实际预置位个数
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPTZPresetList (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_PTZ_PRESET* pPTZPresetList, IVS_UINT32 uiBufferSize, IVS_UINT32* uiPTZPresetNum);

	/**************************************************************************
    * name			: IVS_SDK_SetGuardPos
    * description	: 设置看守位
    * input			: iSessionID    登录成功后的会话ID
						  pCameraCode   摄像机编码
						  pGuardPosInfo 看守位信息
	* output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetGuardPos (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_GUARD_POS_INFO* pGuardPosInfo);

	/**************************************************************************
    * name			: IVS_SDK_DelGuardPos
    * description	: 删除看守位
    * input			: iSessionID    登录成功后的会话ID
						  pCameraCode   摄像机编码
						  uiPresetIndex 预置位索引
	* output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelGuardPos (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiPresetIndex);

	/**************************************************************************
    * name			: IVS_SDK_GetGuardPos
    * description	: 获取看守位
    * input			: iSessionID    登录成功后的会话ID
						  pCameraCode   摄像机编码
	* output		: pGuardPosInfo 看守位信息
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetGuardPos (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_GUARD_POS_INFO* pGuardPosInfo);

	/**************************************************************************
    * name			: IVS_SDK_AddCruiseTrack
    * description	: 增加巡航轨迹
    * input			: iSessionID       登录成功后的会话ID
						  pCameraCode      摄像机编码
						  pCuriseTrackInfo 巡航轨迹信息
	* output		: uiTrackIndex 轨迹索引
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddCruiseTrack (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CRUISE_TRACK* pCuriseTrackInfo, IVS_UINT32* uiTrackIndex);

	/**************************************************************************
    * name			: IVS_SDK_DelCruiseTrack
    * description	: 删除巡航轨迹
    * input			: iSessionID   登录成功后的会话ID
						  pCameraCode  摄像机编码
						  uiTrackIndex 轨迹索引
						  uiCruiseType 轨迹类型：1-轨迹巡航 2-模式巡航
	* output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelCruiseTrack (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiTrackIndex, IVS_UINT32 uiCruiseType);

	/**************************************************************************
    * name			: IVS_SDK_ModCruiseTrack 
    * description	: 修改巡航轨迹
    * input			: iSessionID       登录成功后的会话ID
						  pCameraCode      摄像机编码
						  pCuriseTrackInfo 巡航轨迹信息
	* output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModCruiseTrack (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CRUISE_TRACK* pCuriseTrackInfo);

	/**************************************************************************
    * name			: IVS_SDK_GetCruiseTrackList
    * description	: 查询巡航轨迹列表
    * input			: iSessionID           登录成功后的会话ID
						  pCameraCode          摄像机编码
						  pCruiseTrackInfoList 巡航轨迹列表（注：此时不返回轨迹点信息）。最多16个，由外部调用分配足够空间
						  uiBufferSize         缓存大小
	* output		: uiCruiseTrackNum     实际查询到的巡航轨迹个数
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCruiseTrackList (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_CRUISE_TRACK* pCruiseTrackList, IVS_UINT32 uiBufferSize, IVS_UINT32* uiCruiseTrackNum);

	/**************************************************************************
    * name			: IVS_SDK_GetCruiseTrack
    * description	: 查询巡航轨迹信息
    * input			: iSessionID   登录成功后的会话ID
						  pCameraCode  摄像机编码
						  uiTrackIndex 轨迹索引
	* output		: pCruiseTrackInfo 巡航轨迹信息
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCruiseTrack (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_INT32 uiTrackIndex, IVS_CRUISE_TRACK* pCruiseTrackInfo);

	/**************************************************************************
    * name			: IVS_SDK_SetCruisePlan
    * description	: 设置巡航计划
    * input			: iSessionID   登录成功后的会话ID
						  pCameraCode  摄像机编码
						  pCruisePlan  巡航计划
	* output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetCruisePlan (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CRUISE_PLAN* pCruisePlan);

	/**************************************************************************
    * name			: IVS_SDK_GetCruisePlan
    * description	: 查询巡航计划
    * input			: iSessionID   登录成功后的会话ID
						  pCameraCode  摄像机编码
	* output		: pCruisePlan  巡航计划
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCruisePlan (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_CRUISE_PLAN* pCruisePlan);
	
	/**************************************************************************
    * name			: IVS_SDK_PlatformSnapshot
    * description	: 平台抓拍 
    * input			: iSessionID  登录成功后的会话ID
						  pCameraCode 摄像机编码
	* output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlatformSnapshot(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode);

	/*****************************************************************
    * name			: IVS_SDK_GetSnapshotList 
    * description	: 从服务器获取图片列表 
    * input			: iSessionID    登录成功后的会话ID
						  pCameraCode   摄像机编码
						  pQueryParam   抓拍图片查询参数
						  uiBufSize     缓存大小
	* output		: pSnapshotList 抓拍图片列表信息
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetSnapshotList(IVS_INT32 iSessionID,
															 const IVS_CHAR* pCameraCode,
															 const IVS_QUERY_SNAPSHOT_PARAM* pQueryParam,
															 IVS_SNAPSHOT_INFO_LIST* pSnapshotList,
															 IVS_UINT32 uiBufSize);

    /*************************************************************************
	* name			: IVS_SDK_StartVoiceTalkback
	* description	: 开始语音对讲
	* input			: iSessionID     登录成功后的会话ID
						  pTalkbackParam 语音对讲参数（注：目前只支持UDP方式）
						  pCameraCode    摄像机编码
	* output		: pHandle        对讲句柄
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartVoiceTalkback(
                                IVS_INT32 iSessionID,
                                const IVS_VOICE_TALKBACK_PARAM* pTalkbackParam,
                                const IVS_CHAR* pCameraCode,
                                IVS_ULONG* pHandle);

    /*************************************************************************
	* name			: IVS_SDK_StopVoiceTalkback
	* description	: 停止语音对讲
	* input			: iSessionID   登录成功后的会话ID
						  ulHandle     对讲句柄
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopVoiceTalkback(
                                                          IVS_INT32 iSessionID,
                                                          IVS_ULONG ulHandle);

    /**************************************************************************
    * name			: IVS_SDK_SetMicVolume
    * description	: 设置麦克风音量
    * input			: uiVolumeValue 音量值，范围0~100
    * output		: NA
    * return		: 成功返回0（IVS_SUCCEED）；失败返回错误码
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetMicVolume(IVS_UINT32 uiVolumeValue);

    /**************************************************************************
    * name			: IVS_SDK_GetMicVolume
    * description	: 获取麦克风音量
    * input			: NA
    * output		: NA
    * return		: 音量值，范围0~100
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_UINT32 __SDK_CALL IVS_SDK_GetMicVolume();

    /*************************************************************************
	* name			: IVS_SDK_StartRealBroadcast
	* description	: 开始实时语音广播
	* input			: iSessionID   登录成功后的会话ID
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealBroadcast(IVS_INT32 iSessionID);

    /*************************************************************************
	* name			: IVS_SDK_StopRealBroadcast
	* description	: 停止实时语音广播
	* input			: iSessionID   登录成功后的会话ID
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealBroadcast(IVS_INT32 iSessionID);

    /*************************************************************************
	* name			: IVS_SDK_StartFileBroadcast
	* description	: 开始文件语音广播
	* input			: iSessionID 登录成功后的会话ID, 
						  pFileName  音频文件名,
						  bCycle     是否循环播放：0-不循环，1-循环
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartFileBroadcast(
                                                    IVS_INT32 iSessionID,
                                                    const IVS_CHAR* pFileName,
                                                    IVS_BOOL bCycle);

    /*************************************************************************
	* name			: IVS_SDK_StopFileBroadcast
	* description	: 停止文件语音广播
	* input			: iSessionID   登录成功后的会话ID, 
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopFileBroadcast(IVS_INT32 iSessionID);

    /*************************************************************************
	* name			: IVS_SDK_AddBroadcastDevice
	* description	: 添加广播设备
	* input			: iSessionID  登录成功后的会话ID, 
						  pCameraCode 摄像机编码
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddBroadcastDevice(
                                                   IVS_INT32 iSessionID,
                                                   const IVS_CHAR* pCameraCode);

    /*************************************************************************
	* name			: IVS_SDK_DeleteBroadcastDevice
	* description	: 删除广播设备
	* input			: iSessionID  登录成功后的会话ID, 
						  pCameraCode 摄像机编码
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteBroadcastDevice (
                                                   IVS_INT32 iSessionID,
                                                   const IVS_CHAR* pCameraCode);


	/*****************************************************************
    * name			: IVS_SDK_GetAlarmLinkageAction
    * description	: 查询告警联动动作详情
	* input			: iSessionID 登录成功后的会话ID
						  pReqXml    请求消息
	* output		: pRspXml    响应消息
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*****************************************************************/	
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLinkageAction(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRsqXml);

	/*****************************************************************
	* name			: IVS_SDK_DeleteSnapshot
	* description	: 删除平台抓拍图片
	* input			: iSessionID  登录成功后的会话ID
						  pCameraCode 摄像机编码
						  uiPictureID 图片ID
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteSnapshot(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiPictureID);

	/******************************************************************
    function		: IVS_SDK_SetLogPath
    description: 设置日志根目录
    input			: pLogPath 日志根目录
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetLogPath(const IVS_CHAR* pLogPath);

	/*****************************************************************
	function		: IVS_SDK_StartSearchDevice
	description	: 启动搜索前端设备
	input			: iSessionID   登录成功后的会话ID
					  pDomainCode  目标域编码
					  pSearchInfo 设备搜索信息XML
	output		: NA
	return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartSearchDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pSearchInfo);

	/*****************************************************************
	function		: IVS_SDK_StopSearchDevice
	description	: 停止搜索前端设备
	input			: iSessionID   登录成功后的会话ID
					  pDomainCode  目标域编码
					  pSearchInfo 设备搜索信息XML
	output		: NA
	return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopSearchDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pSearchInfo);

	/*****************************************************************
	function		: IVS_SDK_VerifyDevice
	description	: 验证前端设备
	input			: iSessionID   登录成功后的会话ID
					  pDomainCode  目标域编码
					  pVerifyInfo 设备验证信息XML
	output		: pRspXml   验证结果XML 
	return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_VerifyDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pVerifyInfo, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name			: IVS_SDK_AddDevice
	* description	: 增加设备
	* input			: iSessionID 登录成功后的会话ID
						  pDomainCode 域编码
						  uiDeviceNum 设备数量
						  pDevList 设备列表
						  uiResultBufferSize 结果列表缓存大小
	* output		: pResultList 结果列表
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode,  IVS_UINT32 uiDeviceNum, 
		const IVS_DEVICE_OPER_INFO* pDevList, IVS_DEVICE_OPER_RESULT_LIST* pResultList, IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name			: IVS_SDK_DeleteDevice
	* description	: 删除设备
	* input			: iSessionID 登录成功后的会话ID
						  pDomainCode 域编码
						  uiDeviceNum 设备数量
						  pDevList 设备列表
						  uiResultBufferSize 结果列表缓存大小
	* output		: pResultList 结果列表
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteDevice(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,IVS_UINT32 uiDeviceNum,
		const IVS_DEVICE_CODE* pDevCodeList, IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize);

	/**************************************************************************
    * name			: IVS_SDK_RebootDevice
    * description	: 重启前端设备
    * input			: iSessionID 登录成功后的会话ID
						  pDeviceCode  主设备编码
						  uiRebootType 重启类型：0-重启 1-恢复默认配置并重启
	* output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_RebootDevice (IVS_INT32 iSessionID, const IVS_CHAR* pDeviceCode, IVS_UINT32 uiRebootType);

	/*************************************************************************
	* name			: IVS_SDK_SetMainDeviceName
	* description	: 修改主设备名称
	* input			: iSessionID 登录成功后的会话ID
						  pDevCode 主设备编码
						  pNewDevName 主设备新的名称
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetMainDeviceName(	IVS_INT32 iSessionID, const IVS_CHAR* pDevCode, const IVS_CHAR* pNewDevName);

	/*************************************************************************
	* name			: IVS_SDK_SetCameraName
	* description	: 修改摄像机名称
	* input			: iSessionID 登录成功后的会话ID, 
						  pCameraCode 摄像机编码
						  pNewCameraName 摄像机新的名称
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetCameraName(	IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CHAR* pNewCameraName);

	/******************************************************************
	function		: IVS_SDK_SetDevPWD
	description: 设置前端设备密码
	input			: iSessionID  登录成功后返回的会话ID
					  pDevCode  设备编码；
					  pDevUserName    设备登录用户名
					  pDevPWD  设备登录密码
					  pDevRegPWD  设备向平台注册密码
	output		: NA
	return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	*******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDevPWD(IVS_INT32 iSessionID, const IVS_CHAR* pDevCode, const IVS_CHAR* pDevUserName, const IVS_CHAR* pDevPWD, const IVS_CHAR* pDevRegPWD);

	/******************************************************************
    function		: IVS_SDK_GetChannelList
    description: 根据主设备查询子设备列表
    input			: iSessionID  登录成功后返回的会话ID
					  pDevCode   主设备编码
					  uiBufferSize 缓存大小
    output		: pChannelList  主设备的子设备列表
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetChannelList(IVS_INT32 iSessionID, const IVS_CHAR* pDevCode, IVS_DEV_CHANNEL_BRIEF_INFO_LIST* pChannelList, IVS_UINT32 uiBufferSize);

	/**************************************************************************
    * name			: IVS_SDK_SetPlayQualityMode
    * description	: 设置实况播放模式：清晰、流畅
    * input			: iSessionID   登录成功后的会话ID
						  ulHandle   业务句柄，包括实况、回放
						  iPlayQualityMode  播放模式 参考：IVS_PLAY_QUALITY_MODE
    * output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    * remark		: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetPlayQualityMode(IVS_INT32 iSessionID, IVS_ULONG ulHandle,IVS_INT32 iPlayQualityMode);

	/**************************************************************************
    * name			: IVS_SDK_StartRealPlayByIP
    * description	: 开始实时浏览
    * input			: iSessionID 登录成功后的会话ID
						  pCameraCode  摄像机编码
						  pMediaAddrDst  媒体流目标地址
	* output		: pMediaAddrSrc 媒体流源地址
						  pHandle 播放句柄（唯一指定此播放通道）
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayByIP(IVS_INT32 iSessionID, 
		const IVS_CHAR* pCameraCode, const IVS_MEDIA_ADDR* pMediaAddrDst, IVS_MEDIA_ADDR* pMediaAddrSrc, IVS_ULONG* pHandle);

	/**************************************************************************
    * name			: IVS_SDK_StopRealPlayByIP
    * description	: 停止实时浏览
    * input			: iSessionID 登录成功后的会话ID
						  ulHandle 播放句柄（唯一指定此播放通道）
	* output		: NA
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlayByIP(IVS_INT32 iSessionID, IVS_ULONG ulHandle);

	/**************************************************************************
    * name			: IVS_SDK_StartRealPlayByIPEx
    * description	: 开始实时浏览
    * input			: iSessionID 登录成功后的会话ID
						  pRealplayParam  播放参数
						  pCameraCode  摄像机编码
						  pMediaAddrDst  媒体流目标地址
	* output			: pMediaAddrSrc 媒体流源地址
						  pHandle 播放句柄（唯一指定此播放通道）
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayByIPEx(IVS_INT32 iSessionID, 
															const IVS_REALPLAY_PARAM  *pRealplayParam,
															const IVS_CHAR* pCameraCode,
															const IVS_MEDIA_ADDR* pMediaAddrDst,
															IVS_MEDIA_ADDR* pMediaAddrSrc,
															IVS_ULONG* pHandle);

	/**************************************************************************
    * name			: IVS_SDK_StopRealPlayByIPEx
    * description	: 停止实时浏览
    * input			: iSessionID 登录成功后的会话ID
						  ulHandle 播放句柄（唯一指定此播放通道）
	* output			: NA
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlayByIPEx(IVS_INT32 iSessionID, IVS_ULONG ulHandle);

	/******************************************************************
	function		: IVS_SDK_GetRecordStatus
	description: 查询录像状态
	input			: iSessionID 登录成功后返回的会话ID
					  pCameraCode 摄像机编码
					  uiRecordMethod 录像方式
	output		: pRecordState 录像状态
	return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	*******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordStatus(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_UINT32 uiRecordMethod,IVS_UINT32* pRecordState);

	/*****************************************************************
	function		: IVS_SDK_GetRecordTask
	description: 查询当前用户的手动录像任务
	input			: iSessionID   登录成功后的会话ID
	output		: pRspXml   查询当前用户的手动录像任务返回信息XML
	return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordTask(IVS_INT32 iSessionID, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name			: IVS_SDK_AddRealPlayBookmark
	* description	: 新增实况书签
	* input			: iSessionID 登录成功后的会话ID
						  pCameraCode 摄像机编码
						  pBookmarkName 书签名
						  pBookmarkTime 书签时间
	* output		: uiBookmarkID 书签ID
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRealPlayBookmark(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,
		const IVS_CHAR* pBookmarkName,const IVS_CHAR* pBookmarkTime,IVS_UINT32* uiBookmarkID);

	/*************************************************************************
	* name			: IVS_SDK_ModifyRealPlayBookmark
	* description	: 修改实况书签
	* input			: iSessionID 登录成功后的会话ID
						  pCameraCode 摄像机编码
						  uiBookmarkID 书签ID
						  pNewBookmarkName 新书签名
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyRealPlayBookmark(IVS_INT32 iSessionID, 
		const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID,const IVS_CHAR* pNewBookmarkName);

	/*************************************************************************
	* name			: IVS_SDK_DeleteRealPlayBookmark
	* description	: 删除实况书签
	* input			: iSessionID 登录成功后的会话ID
						  pCameraCode 摄像机编码
						  uiBookmarkID 书签ID
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRealPlayBookmark(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID);

	/*************************************************************************
	* name			: IVS_SDK_GetCurrentFrameTick
	* description	: 获取回放时间（精确到毫秒）
	* input			: iSessionID 登录成功后的会话ID
						  ulPlayHandle 播放句柄
	* output		: pTick 回放时间，精确到毫秒
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCurrentFrameTick(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_UINT64* pTick);

	/*************************************************************************
	* name			: IVS_SDK_AlarmConfirm
	* description	: 告警确认
	* input			: iSessionID 登录成功后的会话ID
						  ullAlarmEventID 告警事件ID
						  pAlarmInCode 告警源编码
						  pAlarmOperateInfo 告警处理信息，包含用户ID、处理信息、处理时间
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmConfirm(IVS_INT32 iSessionID, IVS_UINT64 ullAlarmEventID, 
		const IVS_CHAR* pAlarmInCode, const IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo);

	/*************************************************************************
	* name			: IVS_SDK_AlarmCancel
	* description	: 告警撤销
	* input			: iSessionID 登录成功后的会话ID
						  ullAlarmEventID 告警事件ID
						  pAlarmInCode 告警源编码
						  pAlarmOperateInfo 告警处理信息，包含用户ID、处理信息、处理时间
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmCancel(IVS_INT32 iSessionID, IVS_UINT64 ullAlarmEventID, 
		const IVS_CHAR* pAlarmInCode, const IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo);

	/*************************************************************************
	* name			: IVS_SDK_AlarmCommission
	* description	: 告警授权
	* input			: iSessionID 登录成功后的会话ID
						  pAlarmInCode 告警源编码
						  pAlarmCommission 告警授权信息
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmCommission (IVS_INT32 iSessionID, 
		const IVS_CHAR* pAlarmInCode, const IVS_ALARM_COMMISSION* pAlarmCommission);

	/*************************************************************************
	* name			: IVS_SDK_UserAlarmAuthorization
	* description	: 授权告警用户认证
	* input			: iSessionID 登录成功后的会话ID
						  pPWD 用户密码
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UserAuthentication(IVS_INT32 iSessionID, const IVS_CHAR *pPWD);

	/*************************************************************************
	* name			: IVS_SDK_AlarmReport
	* description	: 告警信息上报（告警推送）
	* input			: iSessionID  登录成功后的会话ID
						  pAlarmReport  信息上报结构体
	* output		: NA
	* return		: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmReport(IVS_INT32 iSessionID, const IVS_ALARM_REPORT* pAlarmReport);

	/******************************************************************
    function		: IVS_SDK_AddUser
    description: 添加用户
    input			: iSessionID  登录成功后的会话ID
					  pUserInfo   用户信息结构体
    output		: puiUserID     添加成功后返回用户ID
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddUser(IVS_INT32 iSessionID, const IVS_USER_INFO* pUserInfo, IVS_UINT32* puiUserID);

    /******************************************************************
    function		: IVS_SDK_ModifyUser
    description: 修改用户
    input			: iSessionID   登录成功后的会话ID
					  pUserInfo    用户信息结构体
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyUser(IVS_INT32 iSessionID, const IVS_USER_INFO* pUserInfo);

    /******************************************************************
    function		: IVS_SDK_DeleteUser
    description: 删除用户
    input			: iSessionID   登录成功后的会话ID
					  pDomainCode    域编码
					  uiUserID     用户ID
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID);

    /******************************************************************
    function		: IVS_SDK_GetUserList
    description: 查询用户列表
    input			: iSessionID  登录成功后的会话ID
					  pDomainCode 域编码
					  pUnifiedQuery  通用查询请求
					  uiBufferSize   缓存大小
	output		: pUserList   用户信息列表
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserList(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_USER_INFO_LIST* pUserList, IVS_UINT32 uiBufferSize);

    /******************************************************************
    function		: IVS_SDK_GetOnlineUser
    description: 查询在线用户列表
	input			: iSessionID   登录成功后的会话ID
					  pDomainCode 域编码
					  pUnifiedQuery  通用查询请求
					  uiBufferSize   缓存大小
    output		: pOnlineUserList   在线用户信息列表
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetOnlineUser(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ONLINE_USER_LIST* pOnlineUserList, IVS_UINT32 uiBufferSize);

    /******************************************************************
    function		: IVS_SDK_GetUserInfo
    description: 查询用户信息
    input			: iSessionID   登录成功后的会话ID
					  pDomainCode    域编码
					  uiUserID   用户ID
    output		: pUserInfo  用户信息
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserInfo(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_USER_INFO* pUserInfo);

    /******************************************************************
    function		: IVS_SDK_LockUser
    description: 锁定/解锁用户
    input			: iSessionID   登录成功后的会话ID
					  pDomainCode    域编码
					  uiUserID   用户ID
					  bLock   是否锁定用户：0-解锁，1-锁定
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LockUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_BOOL bLock);

	/******************************************************************
    function		: IVS_SDK_SignOutUser
    description: 根据用户ID签退用户的所有会话
    input			: iSessionID  登录成功后的会话ID
					  pDomainCode    域编码
					  uiUserID   用户ID
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SignOutUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID);

    /******************************************************************
    function		: IVS_SDK_SignOutUserByRole
    description: 根据角色ID签退用户的所有会话
    input			: iSessionID  登录成功后的会话ID
					  pDomainCode    域编码
					  uiRoleID  角色ID
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SignOutUserByRole(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID);

    /******************************************************************
    function		: IVS_SDK_SignOutOnlineUser
    description: 签退在线用户
    input			: iSessionID  登录成功后的会话ID
					  pDomainCode    域编码
					  uiUserOnlineID      在线用户的会话ID
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SignOutOnlineUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserOnlineID);

	/******************************************************************
    function		: IVS_SDK_SetUserDeviceList
    description: 设置用户设备列表
    input			: iSessionID   登录成功后的会话ID
					  pDomainCode     域编码
					  uiUserID    用户ID
				      pUserDevList  用户设备列表
					  uiBufferSize   缓存大小
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetUserDeviceList(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, const IVS_USER_DEVICE_INFO_LIST* pUserDevList, IVS_UINT32 uiBufferSize);

    /******************************************************************
    function		: IVS_SDK_GetUserDeviceList
    description: 获取用户设备列表
    input			: iSessionID    登录成功后的会话ID
					  pDomainCode    域编码
					  uiUserID  用户ID
					  pIndexRange   分页信息
					  uiBufferSize     缓存大小
    output		: pUserDevList       用户设备列表
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserDeviceList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, 
		IVS_UINT32 uiUserID, const IVS_INDEX_RANGE* pIndexRange, IVS_USER_DEVICE_INFO_LIST* pUserDevList, IVS_UINT32 uiBufferSize);

	/**************************************************************************
    * name			: IVS_SDK_GetDeviceUserList
    * description	: 获取拥有指定设备权限的用户列表
    * input			: iSessionID 登录成功后的会话ID
						  pDomainCode 域编码
						  pDevCode   设备编码
						  uiIsUserOnline 用户是否在线：0-离线 1-在线 2-所有
						  pIndexRange 分页信息
						  uiBufferSize 缓存大小
	* output		: pUserDevList 用户设备列表
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL  IVS_SDK_GetDeviceUserList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, 
		IVS_UINT32 uiIsUserOnline, const IVS_INDEX_RANGE* pIndexRange, IVS_DEVICE_USER_INFO_LIST* pDevUserList, IVS_UINT32 uiBufferSize);

	/******************************************************************
    function		: IVS_SDK_AddUserGroup
    description	: 增加用户组
    input			: iSessionID    登录成功后的会话ID
					  pUserGroup   用户组信息
    output		: puiUserGroupID    用户组ID
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddUserGroup(IVS_INT32 iSessionID, const IVS_USER_GROUP* pUserGroup, IVS_UINT32* puiUserGroupID);

    /******************************************************************
    function		: IVS_SDK_ModifyUserGroup
    description	: 修改用户组
    input			: iSessionID   登录成功后的会话ID
					  pUserGroup   用户组信息
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyUserGroup(IVS_INT32 iSessionID, const IVS_USER_GROUP* pUserGroup);

    /******************************************************************
    function		: IVS_SDK_DeleteUserGroup
    description	: 删除用户组
    input			: iSessionID    登录成功后的会话ID
					  pDomainCode    域编码
					  uiUserGroupID    用户组ID
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteUserGroup(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserGroupID);

    /******************************************************************
    function		: IVS_SDK_MoveUserGroup
    description	: 移动用户组
    input			: iSessionID   登录成功后的会话ID
					  pDomainCode   域编码
					  uiTargetGroupID   目标用户组ID
					  pUserGroupList  用户组列表
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_MoveUserGroup(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiTargetGroupID, const IVS_USER_GROUP_LIST* pUserGroupList);

	/******************************************************************
    function		: IVS_SDK_GetUserGroupList
    description: 查询用户组列表
    input			: iSessionID   登录成功后的会话ID
					  pDomainCode    域编码
					  pIndexRange     分页信息
					  uiBufferSize    缓存大小
    output		: pUserGroupList   用户组列表
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserGroupList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, 
		const IVS_INDEX_RANGE* pIndexRange, IVS_USER_GROUP_LIST* pUserGroupList, IVS_UINT32 uiBufferSize);

	/******************************************************************
    function		: IVS_SDK_AddRole
    description	: 增加角色
    input			: iSessionID    登录成功后的会话ID
					  pRoleInfo     角色信息
    output		: puiRoleID     角色ID
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRole(IVS_INT32 iSessionID, const IVS_ROLE_INFO* pRoleInfo, IVS_UINT32* puiRoleID);

    /******************************************************************
    function		: IVS_SDK_ModifyRole
    description	: 修改角色
    input			: iSessionID   登录成功后的会话ID
					  pRoleInfo    角色信息结构体
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyRole(IVS_INT32 iSessionID, const IVS_ROLE_INFO* pRoleInfo);

    /******************************************************************
    function		: IVS_SDK_DeleteRole
    description	: 删除角色
    input			: iSessionID    登录成功后的会话ID
					  pDomainCode    域编码
					  uiRoleID     角色ID
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRole(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID);

	/******************************************************************
    function		: IVS_SDK_GetRoleList
    description: 查询角色列表
    input			: iSessionID   登录成功后的会话ID
					  pDomainCode     域编码
					  pIndexRange     分页信息
					  uiBufferSize    缓存大小
    output		: pRoleInfoList     角色列表
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRoleList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, 
		const IVS_INDEX_RANGE* pIndexRange, IVS_ROLE_INFO_LIST* pRoleInfoList, IVS_UINT32 uiBufferSize);

	/******************************************************************
    function		: IVS_SDK_GetRoleInfoByRoleID
    description: 根据角色ID获取角色详细信息
    input			: iSessionID   登录成功后的会话ID
					  pDomainCode    域编码
					  uiRoleID    角色ID
    output		: pRoleInfo      角色信息
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRoleInfoByRoleID(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID, IVS_ROLE_INFO* pRoleInfo);

    /******************************************************************
    function		: IVS_SDK_GetRoleInfoByUserID
    description: 根据用户ID获取角色详细信息
    input			: iSessionID    登录成功后的会话ID
					  pDomainCode    域编码
					  uiUserID    角色ID
    output		: pRoleInfo    角色信息
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRoleInfoByUserID(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_ROLE_INFO* pRoleInfo);

	/******************************************************************
    function		: IVS_SDK_GetDefaultRoleAccountRule
    description	: 获取默认账户规则
    input			: iSessionID    登录成功后的会话ID
					  pDomainCode     域编码
    output		: pRoleAccountRule   账户规则
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDefaultRoleAccountRule(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_ROLE_ACCOUNT_RULE* pRoleAccountRule);

	/******************************************************************
    function		: IVS_SDK_GetOperationRightGroup
    description: 获取用户权限信息
    input			: iSessionID   登录成功后的会话ID
					  pDomainCode    域编码
					  uiBufferSize    缓存大小
    output		: pRightGroupList   权限组列表
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetOperationRightGroup(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_OPERATION_RIGHT_GROUP_LIST* pRightGroupList, IVS_UINT32 uiBufferSize);

	/******************************************************************
    function		: IVS_SDK_OMUQueryDiskUsageRateReport
    description: 磁盘存储占用率报表查询
    input			: iSessionID   登录成功后的会话ID
					  pReqXml    查询条件
    output		: pRspXml    查询结果
    return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryDiskUsageRateReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/**************************************************************************
    * name			: IVS_SDK_OMUQueryRaidGroup
    * description	: 磁盘组信息查询
    * input			: iSessionID 登录成功后的会话ID
	* output		: pRspXml  响应XML
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRaidGroup(IVS_INT32 iSessionID, IVS_CHAR** pRspXml);

	/**************************************************************************
    * name			: IVS_SDK_OMUQueryRaidGroupDisk
    * description	: 磁盘信息查询
    * input			: iSessionID 登录成功后的会话ID
						  pReqXml 请求XML
	* output		: pRspXml  响应XML
    * return		: 成功返回0（IVS_SUCCEED），失败返回错误码
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRaidGroupDisk(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/**************************************************************************
    * name			: IVS_SDK_GetOperationLog
    * description	: 查询操作日志
    * input			: iSessionID 登录成功后的会话ID
						  pUnifiedQuery 通用查询结构体
						  uiBufSize 缓存大小
    * output			: pResOperLogList 操作日志
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark			: NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetOperationLog(IVS_INT32 iSessionID,
		const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,IVS_OPERATION_LOG_LIST* pResOperLogList, IVS_UINT32 uiBufSize);


	/**************************************************************************
    * name			: IVS_SDK_GetOperationLogEx
    * description: 扩展查询操作日志
    * input			: iSessionID 登录成功后的会话ID
	* input			: pUnifiedQuery 统一通用查询条件
	* input			: pOptLogQueryEx 扩展查询操作日志条件
	* input			: uiBufSize 事件内容大小
    * output			: pResOperLogList 操作日志查询查询结果
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
    **************************************************************************/

	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetOperationLogEx(IVS_INT32 iSessionID,
		const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,
		const IVS_QUERY_OPTLOG_EX *pOptLogQueryEx,
		IVS_OPERATION_LOG_LIST* pResOperLogList,
		const IVS_UINT32 uiBufSize);
		
	/*****************************************************************
	     function   : IVS_SDK_GetCPUMemory
	     description: CPU/内存性能查询
	     output     : IVS_INT32 iSessionID         登录成功后的会话ID
					  const IVS_CHAR * pQueryXml   查询XML
		 output     : IVS_CHAR** pRspXml           响应XML 
	     return     : 成功返回0（IVS_SUCCEED），失败返回错误码
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCPUMemory(IVS_INT32 iSessionID, const IVS_CHAR* pNodeCode, IVS_CHAR** pPerformance);
	
	/******************************************************************
    function : IVS_SDK_OMUQueryDiskUsageRateReport
    description: 磁盘存储占用率报表查询
    input : IVS_INT32 iSessionID        登录成功后的会话ID
    input : const IVS_CHAR * pReqXml    查询条件
    input : IVS_CHAR * * pRspXml        查询结果
    output : NA
    return : 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryDiskUsageRateReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);
	
  	/******************************************************************
  	function   : IVS_SDK_GetRecordStatus
 	description: 查询录像状态
  	input      : iSessionID 登录成功后返回的会话ID
               pCameraCode 摄像机编码
			   uiRecordMethod 录像方式
  	output     : NA
  	return     : 成功返回0（IVS_SUCCEED），失败返回错误码
  	*******************************************************************/
  	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordStatus(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_UINT32 uiRecordMethod,IVS_UINT32* pRecordState);
  
  	/******************************************************************
    function : IVS_SDK_OMUQueryRecordIntegralityReport
    description: 录像完整性报表查询
    input : IVS_INT32 iSessionID        登录成功后的会话ID
    input : const IVS_CHAR * pReqXml    查询条件
    input : IVS_CHAR * * pRspXml        查询结果
    output : NA
    return : 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRecordIntegralityReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);
		
	/******************************************************************
    function			: IVS_SDK_OMUQueryDiskProperty
    description	: 查询磁盘信息接口
    input				: iSessionID 登录成功后的会话ID
    output			: pRspXml 结果XML
	return				: 成功返回0，失败返回错误码
    remark			: pRspXml是SDK内部申请内存，使用完应释放。
    return				: 成功返回0（IVS_SUCCEED），失败返回错误码
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryDiskProperty(IVS_INT32 iSessionID, 
		IVS_CHAR** pRspXml);
		
	/**************************************************************************
    * name			: IVS_SDK_GetVideoDiagnose
    * description: 查询单台VCN的视频质量诊断计划
    * input			: iSessionID 登录成功后的会话ID
	* input			: pDomainCode 域编码
	* input			: pCameraCode 摄像机编码
	* output			: pRspXml 单台VCN的视频质量诊断计划xml
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: pRspXml是SDK内部申请内存，使用完应调用IVS_SDK_ReleaseBuffer释放。
	* history			: 2014-6-9
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetVideoDiagnose(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		const IVS_CHAR* pCameraCode,
		IVS_CHAR** pRspXml);

	/**************************************************************************
    * name			: IVS_SDK_SetVideoDiagnose
    * description: 设置单台VCN的视频质量诊断计划
    * input			: iSessionID 登录成功后的会话ID
	* input			: pReqXml 单台VCN的视频质量诊断计划xml
	* output			: NA
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	* history			: 2014-6-9
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetVideoDiagnose(IVS_INT32 iSessionID, 
		const IVS_CHAR* pReqXml);

	/**************************************************************************
    * name			: IVS_SDK_GetStreamListByCam
    * description: 根据摄像机Code，查询该摄像机正在给哪些用户提供视频业务
    * input			: iSessionID 登录成功后的会话ID
	* input			: pDomainCode 被查询的摄像机所在域编码
	* input			: pCameraCode 摄像机编码
	* input			: uiBufSize 申请内存大小
	* output			: pStreamListByCam 摄像机开启的媒体业务列表
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: C/C++中uiBufSize固定为sizeof(IVS_STREAM_LIST_BY_CAM) + 
								(IVS_MAX_STREAM_USER_INFO_NUM -1) * sizeof(IVS_STREAM_USER_INFO)
	* history			: 2014-6-10
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetStreamListByCam(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode,
		const IVS_CHAR* pCameraCode,
		IVS_UINT32 uiBufSize,
		IVS_STREAM_LIST_BY_CAM* pStreamListByCam);

	/**************************************************************************
    * name			: IVS_SDK_GetStreamListByUser
    * description: 根据用户编码，查询该用户正在给调用哪些摄像机的视频业务
    * input			: iSessionID 登录成功后的会话ID
	* input			: pDomainCode 被查询的用户所在域编码
	* input			: uiUserID 被查询的用户ID
	* input			: uiBufSize 申请内存大小
	* output			: pStreamListByUser 用户正在调用的媒体业务列表
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: C/C++中uiBufSize固定为sizeof(IVS_STREAM_LIST_BY_USER) + 
								(IVS_MAX_STREAM_CAM_INFO_NUM -1) * sizeof(IVS_STREAM_CAM_INFO)
	* history			: 2014-6-24
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetStreamListByUser(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiUserID,
		IVS_UINT32 uiBufSize,
		IVS_STREAM_LIST_BY_USER* pStreamListByUser);

	/**************************************************************************
    * name			: IVS_SDK_ShutdownStreamByUser
    * description: 根据摄像机Code和用户ID，关闭该用户正在使用的视频业务
    * input			: iSessionID 登录成功后的会话ID
	* input			: pDomainCode 用户所在域编码
	* input			: uiUserID 用户ID
	* input			: pCameraCode 摄像机编码
	* input			: iStreamType 视频业务类型 1-实况，2-回放
	* input			: iDuration 关闭时长，在此时间之内该用户不能再次点播，单位为秒，0为不控制关闭时长
	* output			: NA
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	* history			: 2014-6-10
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ShutdownStreamByUser(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiUserID,
		const IVS_CHAR* pCameraCode,
		IVS_INT32 iStreamType,
		IVS_INT32 iDuration);

	/**************************************************************************
    * name			: IVS_SDK_PasswdValidation
    * description: 用户名、密码验证
    * input			: iSessionID 登录成功后的会话ID
	* input			: pLoginName 登录用户名
	* input			: pPasswd 密码
	* output			: NA
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	* history			: 2014-6-27
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PasswdValidation(IVS_INT32 iSessionID,
		const IVS_CHAR* pLoginName,
		const IVS_CHAR *pPasswd);

	/**************************************************************************
    * name			: IVS_SDK_SetVideoEffect
    * description: 录像视频质量调节
	* input			: iSessionID 登录成功后的会话ID
    * input			: ulHandle 播放句柄
	* input			: lBrightValue 亮度
	* input			: lContrastValue 对比度
	* input			: lSaturationValue 饱和度
	* input			: lHueValue 色调
	* output			: NA
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	* history			: 2014-9-2
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetVideoEffect(IVS_INT32 iSessionID,
		IVS_ULONG ulPlayHandle, IVS_LONG lBrightValue,
		IVS_LONG lContrastValue, IVS_LONG lSaturationValue,
		IVS_LONG lHueValue);

	/**************************************************************************
    * name			: IVS_SDK_GetDomainDeviceList
    * description: 获取指定域所包含的设备列表
	* input			: iSessionID 登录成功后的会话ID
    * input			: pDomainCode 域编码，为需要查询设备的域的编码
	* input			: uiDeviceType 设备类型，值参考IVS_DEVICE_TYPE，支持除主设备，外域告警设备外的其他设备
	* input			: pIndexRange 分页信息
	* output			: pDeviceList 设备列表指针
	* input			: uiBufferSize 缓存大小
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: NA
	* history			: 2014-9-9
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDomainDeviceList(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiDeviceType,
		const IVS_INDEX_RANGE* pIndexRange,
		IVS_VOID* pDeviceList,
		IVS_UINT32 uiBufferSize);
		
	/**************************************************************************
    * name       : IVS_SDK_DelPlayWnd
    * description: 删除播放窗口
    * input      : iSessionID 登录成功后的会话ID
	               ulPlayHandle   播放句柄
                   hWnd       窗口句柄
    * output     : NA
    * return     : 成功返回IVS_SUCCESSED，失败返回错误码
    * author     ：
	* remark     : NA
	* history    : 2012-12-15
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelPlayWnd(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd);
	
	/**************************************************************************
    * name       : IVS_SDK_SetDrawCallBack
    * description: 设置画图显示的回调接口
    * input      : iSessionID 登录成功后的会话ID
	               ulPlayHandle   播放句柄
                   hWnd       窗口句柄
				   callback		画图回调函数指针；
				   pUserData		用户数据指针
    * output     : NA
    * return     : 成功返回IVS_SUCCESSED，失败返回错误码
    * author     ：
	* remark     : NA
	* history    : 2012-12-15
    **************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDrawCallBackEx(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd, DRAW_PROC callback, void *pUserData);
  
  /**************************************************************************
    * name			: IVS_SDK_EnablePURealplayIAOverLay
    * description: 启动摄像机实况的智能分析轨迹叠加
	* input			: iSessionID 登录成功后的会话ID
	* input			: ulRealPlayHandle 实况播放句柄
    * input			: pDomainCode 摄像头ID
	* input			: pNVRCode NVR编码
	* input			: hWnd 媒体窗口句柄
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: 需要摄像头具备智能分析功
	* history			: 2015-1-13
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_EnablePURealplayIAOverLay(IVS_INT32 iSessionID,
		IVS_ULONG ulRealPlayHandle,
		const IVS_CHAR* pCameraCode,
		const IVS_CHAR* pNVRCode,
		HWND hWnd);

	/**************************************************************************
    * name			: IVS_SDK_DisablePURealplayIAOverLay
    * description: 停止摄像机实况的智能分析轨迹叠加
	* input			: iSessionID 登录成功后的会话ID
	* input			: ulRealPlayHandle 实况播放句柄
    * return			: 成功返回0（IVS_SUCCEED），失败返回错误码
	* remark		: 需要摄像头具备智能分析功
	* history			: 2015-1-13
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DisablePURealplayIAOverLay(IVS_INT32 iSessionID,
		IVS_ULONG ulRealPlayHandle);
		
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_IVS_SDK_H_

