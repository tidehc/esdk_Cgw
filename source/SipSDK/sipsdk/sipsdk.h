//lint -e1784
#ifndef _ESDK_SIPSDK_H
#define _ESDK_SIPSDK_H

// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 ESDK_UCSERVICE_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// ESDK_UCSERVICE_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#ifdef SIPSDK_EXPORTS
#define ESDK_SIPSDK_API __declspec(dllexport)
#else
#define ESDK_SIPSDK_API __declspec(dllimport)
#endif

#ifdef WIN32
#define __SIP_SDK_CALL __stdcall
#else
#define __SIP_SDK_CALL
#endif

#include "DataType.h"

#ifdef __cplusplus
extern "C"
{
#endif

	/**
	 *初始化
	 * 
	 *该函数用于初始化SIP协议栈
	 *	 
	 *@param[in]	pLocalSipAccount	本端sip鉴权用户名	 
	 *@param[in]	pLocalSipPasswd		本端sip鉴权密码
	 *@param[in]	pLocalID		本地ID	
	 *@param[in]	pLocalIP		本地IP
	 *@param[in]	iLocalPort		本地端口
	 *@param[in]	pServerSipAccount	对端sip鉴权用户名	 
	 *@param[in]	pServerSipPasswd	对端sip鉴权密码
	 *@param[in]	pServerID		对方ID
	 *@param[in]	pServerIP		对方IP
	 *@param[in]	iServerPort		对方端口
	 *@param[in]	pCallBackFunc	回调函数
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	最先开始调用的函数,pSipAccount和pSipPasswd可以为空字符串，其他不允许为空
	 *@par			无
	**/
	ESDK_SIPSDK_API int __SIP_SDK_CALL SIP_SDK_Init(const char* pLocalSipAccount
		,const char* pLocalSipPasswd
		,const char* pLocalID
		,const char* pLocalIP
		,int iLocalPort
		,const char* pServerSipAccount
		,const char* pServerSipPasswd
		,const char* pServerID
		,const char* pServerIP
		,int iServerPort
		,SIP_CallBack pCallBackFunc);

	/**
	 *去初始化
	 * 
	 *该函数用于去初始化SIP协议栈
	 *
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	
	 *@par			无
	**/
	ESDK_SIPSDK_API int __SIP_SDK_CALL SIP_SDK_UnInit(void);

	/**
	 *SIP注册
	 * 
	 *该函数用于发送SIP Register消息
	 *
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	
	 *@par			无
	**/
	ESDK_SIPSDK_API int __SIP_SDK_CALL SIP_SDK_REGISTER(void);

	/**
	 *SIP注注销
	 * 
	 *该函数用于发送SIP Register消息，有效期为0
	 *
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	
	 *@par			无
	**/
	ESDK_SIPSDK_API int __SIP_SDK_CALL SIP_SDK_UNREGISTER(void);

	/**
	 *SIP Invite
	 * 
	 *该函数用于发送SIP Invite消息
	 *
	 *@param[in]	pDevCode		设备编码，比如摄像机编码04598710001311861862
	 *@param[in]	pLocalSdpBody	本地SDP描述字符串
	 *@param[out]	iResponseID		响应ID
	 *@param[out]	strRemoteBody	对方SDP描述
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	
	 *@par			无
	**/
	ESDK_SIPSDK_API int __SIP_SDK_CALL SIP_SDK_INVITE(const char* pDevCode
		,const char* pLocalSdpBody
		,int& iResponseID
		,char strRemoteBody[MAX_LENGTH]);

	/**
	 *SIP Message
	 * 
	 *@param[in]	pDevCode		设备编码，比如摄像机编码04598710001311861862
	 *@param[in]	pBody			发送的xml字段
	 *该函数用于发送SIP Message消息
	 *
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	
	 *@par			无
	**/
	ESDK_SIPSDK_API int __SIP_SDK_CALL SIP_SDK_MESSAGE(const char* pDevCode,const char* pBody);

	/**
	 *SIP BYE
	 * 
	 *该函数用于发送SIP BYE消息
	 *
	 *@param[in]	iResponseID		Invite成功后的响应ID
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	调用者需要传入SIP_SDK_INVITE返回的值
	 *@par			无
	**/
	ESDK_SIPSDK_API int __SIP_SDK_CALL SIP_SDK_BYE(int iResponseID);

	/**
	 *SIP ACK
	 * 
	 *该函数用于发送SIP ACK消息
	 *
	 *@param[in]	iResponseID		Invite成功后的响应ID
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	调用者需要传入SIP_SDK_INVITE返回的值
	 *@par			无
	**/
	ESDK_SIPSDK_API int __SIP_SDK_CALL SIP_SDK_ACK(int iResponseID);

	/**
	 *订阅
	 * 
	 *该函数用于发送subscribe消息
	 *
	 *@param[in]	pRemoteURI		订阅对象URI
	 *@param[in]	pBody			body体
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	最先开始调用的函数
	 *@par			无
	**/
	ESDK_SIPSDK_API int __SIP_SDK_CALL SIP_SDK_Subscribe(const char* pRemoteURI,const char* pBody);



#ifdef __cplusplus
}
#endif 


#endif

