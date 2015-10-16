//===================================================================
//	Copyright(C)2013-2020, Huawei Tech. Co., Ltd.
//
//	文件名	：SoftTerminalAdapter.h
//	作  者	：方明亮
//	版  本	：V100R002C03
//	完成日期：2013-03-22
//	说  明	：实现软终端接口C++ 和C#之间转换
//	其  它	：
//	历史记录：w00206574 2014-08-22改为动态调用SotCall.dll
//===================================================================
#include "stdafx.h"
#include <string>
#include "SoftTerminalAdapter.h"

//获取dll路径
std::string SoftTerminalAdapter::GetCurPath(void)
{
	char path[_MAX_DIR+1] = {0};
	GetModuleFileName(NULL, path, sizeof(path));
	std::string DllPath = path;
	unsigned int pos = DllPath.find_last_of("\\", DllPath.size());
	if(pos<DllPath.size())
	{
		DllPath.resize(pos);
	}
	return DllPath;
}
//===================================================================
//	函数名称：SoftTerminalAdapter
//	功能描述：构造函数
//	调用清单：NULL
//	被调清单：MonitorChannelManager.cs
//	输入参数：NULL
//	输出参数：NULL
//	  返回值：NULL
//	其他说明：
//===================================================================
SoftTerminalAdapter::SoftTerminalAdapter()
{
	m_hDllInst = LoadLibraryEx((GetCurPath().append("\\SotCall.dll")).c_str(),NULL,LOAD_WITH_ALTERED_SEARCH_PATH);
	if(m_hDllInst != NULL)
	{
		pSoftTerminalInit = (PSoftTerminalInit)GetProcAddress(m_hDllInst,"SoftTerminalInit");
		pSoftTerminalAddCallChannel = (PSoftTerminalAddCallChannel)GetProcAddress(m_hDllInst,"SoftTerminalAddCallChannel");
		pSoftTerminalDeleteCallChannel = (PSoftTerminalDeleteCallChannel)GetProcAddress(m_hDllInst,"SoftTerminalDeleteCallChannel");
		pSoftTerminalSendVideoData=(PSoftTerminalSendVideoData)GetProcAddress(m_hDllInst,"SoftTerminalSendVideoData");
		pSoftTerminalSendG711AudioData=(PSoftTerminalSendG711AudioData)GetProcAddress(m_hDllInst,"SoftTerminalSendG711AudioData");
	}
}

//===================================================================
//	函数名称：~SoftTerminalAdapter
//	功能描述：析构函数
//	调用清单：NULL
//	被调清单：MonitorChannelManager.cs
//	输入参数：NULL
//	输出参数：NULL
//	  返回值：NULL
//	其他说明：
//===================================================================
SoftTerminalAdapter::~SoftTerminalAdapter()
{
	try
	{
		if(m_hDllInst)
		{
			FreeLibrary(m_hDllInst);
			m_hDllInst = NULL;

			pSoftTerminalInit = NULL;
			pSoftTerminalAddCallChannel =NULL;
			pSoftTerminalDeleteCallChannel =NULL;
			pSoftTerminalSendVideoData =NULL;
			pSoftTerminalSendG711AudioData =NULL;
		}
	}
	catch (...)
	{}

}

//===================================================================
//	函数名称：Init
//	功能描述：初始化
//	调用清单：SoftTerminal.h
//	被调清单：MonitorChannelManager.cs
//	输入参数：ip 融合网关ip
//	输出参数：NULL
//	  返回值：错误码
//	其他说明：调用软终端接口实现加载dll，设置参数，初始化等功能
//===================================================================
SmcErr^ SoftTerminalAdapter::Init(InitSotParam^ sotParam)
{
	SmcErr^ err = gcnew CgwError();
	marshal_context^ context = gcnew marshal_context();	
	SOT_APP_REG_FN_S pstCallBackSet ;
	SOT_SYSCFG_S param;

	if (String::IsNullOrEmpty(sotParam->cgwIp))
	{
		err->SetErrorNo(CgwError::ERR_CGW_CHANNEL_SOT_INIT_FAIL);
		return err;
	}

	memset(&param,0,sizeof(SOT_SYSCFG_S));
	memset(&pstCallBackSet,0,sizeof(SOT_APP_REG_FN_S));

	const char* pszAddr = context->marshal_as<const char*>(sotParam->cgwIp);	
	unsigned long dwIP = 0;
	if(0 == strcmp(pszAddr,"127.0.0.1"))
	{
		dwIP = inet_addr("0.0.0.0");
	}
	else
	{
		dwIP = inet_addr(pszAddr);
	}

	param.stLocalAddr.ipv4 = dwIP;
	param.stLocalAddr.eIpVesion = SOT_IP_VER_E::SOT_IP_V4;
	param.usLocalPort = sotParam->channelCallPort;
	param.eLogLevel = SOT_LOG_TRACE;
	param.usLocalRtpBasePort = sotParam->rtpStartPort; /*rtp基础端口，在6K-60K之间*/
	param.usMediaMode = SOT_AUDIO_SENDONLY|SOT_VIDEO_SENDONLY; /*暂时只发视频码流*/
	param.usLocalRtpPortNum = sotParam->rtpPortCount; //每路通话需要8个端口，分配所需端口的2倍
	param.eEntityType = SOT_ENTITY_CGW;

	if (sotParam->audioType == StreamType::AUDIO_G711A)
	{
		param.eCapAudio = SOT_CAP_AUDIO_E::SOT_CAP_AUDIO_G711_A;
	}
	else if (sotParam->audioType == StreamType::AUDIO_G711U)
	{
		param.eCapAudio = SOT_CAP_AUDIO_E::SOT_CAP_AUDIO_G711_U;
	}
	else
	{
		err->SetErrorNo(CgwError::ERR_CGW_CHANNEL_SOT_INIT_FAIL);
		return err;
	}

	// 注册回调
	pstCallBackSet.pfSotCallLogFunc = (pFnSotLog)&WriteLog;
	pstCallBackSet.pfSotIFrameReq = (pFnSotIFrameReq)&MakeIFrame;

	pstCallBackSet.pfSotReceiveCallNotify=(pFnSotReceiveCallNotify)&ReceiveCallCallback;
	pstCallBackSet.pfSotCreateSessionNotify=(pFnSotCreateSessionNotify)&CreateSessionCallback;
	pstCallBackSet.pfSotCallHangup=(pFnSotCallHangup)&CallHandupCallback;
	pstCallBackSet.pfSotErrorReport = (pFnSotErrorReport)&ErrorReportCallback;
	
	// 软终端初始化
	if (!pSoftTerminalInit||!pSoftTerminalInit(&param, &pstCallBackSet))
	{
		err->SetErrorNo(CgwError::ERR_CGW_CHANNEL_SOT_INIT_FAIL);
		return err;
	}

	return err;
}


SOT_VOID ReceiveCallCallback(SOT_INT8 * pcSotNum,SOT_RECV_CALL_STAT_E eRecStat)
{


}

SOT_VOID CreateSessionCallback(SOT_INT8 * pcSotNum,SOT_CREATE_SESSION_STAT_E eCreSessionStat,SOT_UINT8 ucMediaType,SOT_UINT8 ucPayloadType,SOT_UINT16 usReceivePort)
{

}

SOT_VOID   CallHandupCallback(SOT_INT8 * pcSotNum)
{


}

SOT_VOID   ErrorReportCallback(SOT_INT8 * pcSotNum, SOT_ERROR_CODE_E eErrorCode)
{
	// 输入条件判断
	if (nullptr == SoftTerminalAdapter::errReportHandler || NULL == pcSotNum)
	{
		return;
	}

	String^ code = gcnew String(pcSotNum);
	SotErrorType errType = SotErrorType::InitStackErr;

	if (eErrorCode == SOT_ERROR_CODE_E::SOT_INIT_STACK_ERROR)
	{
		errType = SotErrorType::InitStackErr;
	}
	else if (eErrorCode == SOT_ERROR_CODE_E::SOT_INIT_STACK_ERROR)
	{
		errType = SotErrorType::RtpSessionCreateErr;
	}
	else
	{
		return;
	}

	SoftTerminalAdapter::errReportHandler(code, errType);
}


//===================================================================
//	函数名称：RegistSotCallback
//	功能描述：注册回调函数
//	调用清单：SoftTerminal.h
//	被调清单：MonitorChannelManager.cs
//	输入参数：callback 回调函数
//	输出参数：NULL
//	  返回值：错误码
//	其他说明：注册写日志，i帧重传的回调函数
//===================================================================
bool SoftTerminalAdapter::RegistSotCallback(SoftTerminalCallback^ callback)
{
	SmcErr^ err = gcnew CgwError();
	
	// 判断输入是否为空
	if (nullptr == callback)
	{
		return false;
	}

	// 判断回调委托是否为空
	if (nullptr == callback->NlogHandler || nullptr == callback->IFrameHandler || nullptr == callback->errReportHandler)
	{
		return false;
	}

	NLogHandler = callback->NlogHandler;
	IFrameHandler = callback->IFrameHandler;
	errReportHandler = callback->errReportHandler;

	return true;
}

//===================================================================
//	函数名称：AddChannel
//	功能描述：添加呼叫通道
//	调用清单：SoftTerminal.h
//	被调清单：MonitorChannelManager.cs
//	输入参数：channelLabel 通道标识，confAccessCode 会议号
//	输出参数：NULL
//	  返回值：操作结果
//	其他说明：
//===================================================================
bool SoftTerminalAdapter::AddChannel(String^ channelLabel, String^ confAccessCode)
{
	marshal_context^ context = gcnew marshal_context();
	SOT_CHANNEL_INFO_S channelInfo;

	// 输入参数判断
	if (String::IsNullOrEmpty(channelLabel) || String::IsNullOrEmpty(confAccessCode))
	{
		return false;
	}

	const char* pszChannelLabel = context->marshal_as<const char*>(channelLabel);
	const char* pszConfAccessCode = context->marshal_as<const char*>(confAccessCode);

	// 打包
	channelInfo.pcCallNumber = _strdup(pszChannelLabel);
	channelInfo.ulCallLen = strlen(pszChannelLabel);
	channelInfo.pcConfNumber = _strdup(pszConfAccessCode);
	channelInfo.ulConfLen = strlen(pszConfAccessCode);

	// 调用dll接口

	bool isSuccess = false;
	if (pSoftTerminalAddCallChannel)
	{
		isSuccess = pSoftTerminalAddCallChannel(&channelInfo);
	}	

	// 释放通道标识指针
	if (NULL != channelInfo.pcCallNumber)
	{
		free(channelInfo.pcCallNumber);
		channelInfo.pcCallNumber = NULL;
	}

	// 释放会议号指针
	if (NULL != channelInfo.pcConfNumber)
	{
		free(channelInfo.pcConfNumber);
		channelInfo.pcConfNumber = NULL;
	}
	
	delete context;

	return isSuccess;
}

//===================================================================
//	函数名称：RemoveChannel
//	功能描述：删除呼叫通道
//	调用清单：SoftTerminal.h
//	被调清单：MonitorChannelManager.cs
//	输入参数：channelLabel 通道标识
//	输出参数：NULL
//	  返回值：操作结果
//	其他说明：
//===================================================================
bool SoftTerminalAdapter::RemoveChannel(String^ channelLabel)
{
	marshal_context^ context = gcnew marshal_context();

	if (String::IsNullOrEmpty(channelLabel))
	{
		return false;
	}

	const char* pszChannelLabel = context->marshal_as<const char*>(channelLabel);
	SOT_INT8 *pszSotChannelLabel = _strdup(pszChannelLabel);
	SOT_UINT32 channelLabelLen = strlen(pszChannelLabel);

	bool isSuccess =  false;
	if (pSoftTerminalDeleteCallChannel)
	{
		isSuccess = pSoftTerminalDeleteCallChannel(pszSotChannelLabel, channelLabelLen);
	}

	// 释放通道标识指针
	if (NULL != pszSotChannelLabel)
	{
		free(pszSotChannelLabel);
		pszSotChannelLabel = NULL;
	}

	delete context;

	return isSuccess;
}

//===================================================================
//	函数名称：SendRtpData
//	功能描述：发送码流
//	调用清单：SoftTerminal.h
//	被调清单：MonitorChannelManager.cs
//	输入参数：channelLabel 通道标识 data 码流数据
//	输出参数：NULL
//	  返回值：操作结果
//	其他说明：
//===================================================================
bool SoftTerminalAdapter::SendMediaData(String^ channelLabel, MediaData^ data)
{
	marshal_context^ context = gcnew marshal_context();
	bool isSuccess = false;

	// 输入条件判断
	if (String::IsNullOrEmpty(channelLabel) || nullptr == data)
	{
		return false;
	}

	const char* pszChannelLabel = context->marshal_as<const char*>(channelLabel);
	SOT_INT8 *pszSotChannelLabel = _strdup(pszChannelLabel);

	// 转换码流类型
	SOT_H264_DATA_TYPE_E dataType = SOT_H264_DATA_TYPE_E::RTP_H264_DATA;
	switch (data->DataType)
	{
	case MediaDataType::FRAME_DATA:
		dataType = SOT_H264_DATA_TYPE_E::RAW_H264_DATA;
		break;
	case MediaDataType::RTP_DATA:
		dataType = SOT_H264_DATA_TYPE_E::RTP_H264_DATA;
		break;
	default:
		break;
	}

	// 转换码流数据
	array<Byte>^ dataArray = data->Data;
	unsigned char * pszData = AllocateNativeMemory(dataArray);
	int len = data->Size;

	// 调用接口
	switch (data->StreamType)
	{
	case StreamType::VIDEO_H264:
		if (pSoftTerminalSendVideoData)
		{
			isSuccess =  pSoftTerminalSendVideoData(pszSotChannelLabel, dataType, pszData, len);
		}		
		break;
	case StreamType::AUDIO_G711A:
		if (pSoftTerminalSendG711AudioData)
		{		
			isSuccess =  pSoftTerminalSendG711AudioData(pszSotChannelLabel, SOT_AUDIO_DATA_TYPE_E::G711_A_AUDIO_DATA, pszData, len);
		}
		break;
	case StreamType::AUDIO_G711U:
		if (pSoftTerminalSendG711AudioData)
		{
			isSuccess =  pSoftTerminalSendG711AudioData(pszSotChannelLabel, SOT_AUDIO_DATA_TYPE_E::G711_U_AUDIO_DATA, pszData, len);
		}
		break;
	default:
		break;
	}
	

	// 释放码流数据指针
	if (NULL != pszData)
	{
		free(pszData);
		pszData = NULL;
	}

	// 释放通道标识指针
	if (NULL != pszSotChannelLabel)
	{
		free(pszSotChannelLabel);
		pszSotChannelLabel = NULL;
	}

	delete context;

	return isSuccess;
}

//===================================================================
//	函数名称：AllocateNativeMemory
//	功能描述：将Byte[]转换成unsigned char指针
//	调用清单：NULL
//	被调清单：MonitorChannelManager.cs
//	输入参数：ManagedArray 二进制数组
//	输出参数：NULL
//	  返回值：字符串指针
//	其他说明：
//===================================================================
unsigned char* SoftTerminalAdapter::AllocateNativeMemory(IN array<Byte> ^ManagedArray)
{
	if (ManagedArray == nullptr)
	{
		return NULL;
	}

	unsigned char * DestNativePtr	= (unsigned char*) malloc(ManagedArray->Length );
	if (NULL == DestNativePtr)
	{
		return NULL;
	}
	memset(DestNativePtr,0,ManagedArray->Length );

	IntPtr NativePtr(DestNativePtr);
	Marshal::Copy(ManagedArray,0,NativePtr,ManagedArray->Length);

	return DestNativePtr;
}

//===================================================================
//	函数名称：WriteLog
//	功能描述：写日志回调
//	调用清单：c#委托
//	被调清单：软终端
//	输入参数：eLevel 日志级别， pcLog 日志字符串， lLen 日志长度
//	输出参数：NULL
//	  返回值：0
//	其他说明：
//===================================================================
SOT_UINT32 WriteLog(SOT_LOG_LEVEL_E eLevel,SOT_INT8 * pcLog,SOT_UINT32 lLen)
{
	NLogLevel level = NLogLevel::Info;

	if (nullptr == SoftTerminalAdapter::NLogHandler)
	{
		return 0;
	}

	switch (eLevel)
	{
	case SOT_LOG_LEVEL_E::SOT_LOG_ERROR:
		level = NLogLevel::Error;
		break;
	case SOT_LOG_LEVEL_E::SOT_LOG_WARNING:
		level = NLogLevel::Warn;
		break;
	case SOT_LOG_LEVEL_E::SOT_LOG_INFO:
		level = NLogLevel::Info;
		break;
	case SOT_LOG_LEVEL_E::SOT_LOG_TRACE:
		level = NLogLevel::Trace;
		break;
	default:
		break;
	}

	String^ log = gcnew String(pcLog);
	return SoftTerminalAdapter::NLogHandler(level, log, lLen);

}

//===================================================================
//	函数名称：MakeIFrame
//	功能描述：i帧重发回调
//	调用清单：c#委托
//	被调清单：软终端
//	输入参数：pcSotNum 通道号码
//	输出参数：NULL
//	  返回值：0
//	其他说明：
//===================================================================
SOT_UINT32 MakeIFrame(IN SOT_INT8 * pcSotNum)
{
	if (nullptr == SoftTerminalAdapter::IFrameHandler)
	{
		return 0;
	}

	String^ channelLabel = gcnew String(pcSotNum);
	return SoftTerminalAdapter::IFrameHandler(channelLabel);
}



