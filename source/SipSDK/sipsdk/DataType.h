#ifndef _DATATYPE_H_
#define _DATATYPE_H_

//最大长度
const int MAX_LENGTH = 1024;

//函数返回结果
typedef enum _EM_SIP_RESULT
{
	SIPSDK_RET_SUCCESS				=	0,	//成功
	SIPSDK_RET_FAILURE				=	1,	//失败
	SIPSDK_RET_NOT_INIT			=	2,	//未初始化
	SIPSDK_RET_HAS_INIT			=	3,	//已经初始化
	SIPSDK_RET_NOT_REGISTER		=	4,	//未注册
	SIPSDK_RET_INVALID_PARAM		=	5	//参数错误
}EM_SIP_RESULT;

//收到的消息类型
typedef enum _EM_SIP_MSG_TYPE
{
	SIP_REQUEST_MSG		=	0,	//收到的请求消息
	SIP_RESPONSE_MSG	=	1	//收到的响应消息
}EM_SIP_MSG_TYPE;

//请求消息的类型
typedef enum _EM_SIP_REQUEST_EVENT
{
	SIP_REQ_REGISTER		=	0,	//注册请求事件
	SIP_REQ_INVITE			=	1,	//邀请请求事件
	SIP_REQ_MESSAGE			=	2	//消息请求事件
}EM_SIP_REQUEST_EVENT;

//响应消息的类型
typedef enum _EM_SIP_RESPONSE_EVENT
{
	SIP_RSP_SUCCESSS		=	200,	//响应成功事件
	SIP_RSP_NOT_REG			=	403		//注册失败响应事件
}EM_SIP_RESPONSE_EVENT;

//消息通知回调函数结果
typedef int (_stdcall *SIP_CallBack)(int msgType,int eventType,const char* pPara);
#endif 