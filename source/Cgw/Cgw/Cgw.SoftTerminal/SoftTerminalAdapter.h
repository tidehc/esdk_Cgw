//===================================================================
//	Copyright(C)2013-2020, Huawei Tech. Co., Ltd.
//
//	文件名	：SoftTerminalAdapter.h
//	作  者	：方明亮
//	版  本	：V100R002C03
//	完成日期：2013-03-22
//	说  明	：实现软终端接口C++ 和C#之间转换
//	其  它	：
//	历史记录：
//===================================================================
#if !defined(AFX_SOFTTERMINALADAPTER_20130321)
#define AFX_SOFTTERMINALADAPTER_20130321

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "include/soft_terminal_interface.h"
#include <msclr/marshal.h>        // .NET string to C-style string
#include <msclr/marshal_cppstd.h> // .NET string to STL string

using namespace System;
using namespace msclr::interop;
using namespace System::Runtime::InteropServices;
using namespace Cgw::Common;
using namespace Cgw::SmcError;

SOT_UINT32 WriteLog(SOT_LOG_LEVEL_E eLevel,SOT_INT8 * pcLog,SOT_UINT32 lLen);   // 软终端写日志
SOT_UINT32 MakeIFrame(IN SOT_INT8 * pcSotNum);        // I帧重传


/** 建立接收呼叫是否成功的通知回调函数（注：现只有在接收呼叫成功才调用该回调函数） */
SOT_VOID ReceiveCallCallback(SOT_INT8 * pcSotNum,SOT_RECV_CALL_STAT_E eRecStat);

/** 创建session是否成功的通知回调函数（注：现只有在创建session成功才调用该回调函数） */
//ucMediaType:0:audio ,1:video
SOT_VOID CreateSessionCallback(SOT_INT8 * pcSotNum,SOT_CREATE_SESSION_STAT_E eCreSessionStat,SOT_UINT8 ucMediaType, SOT_UINT8 ucPayloadType,SOT_UINT16 usReceivePort);

//挂断回调
SOT_VOID   CallHandupCallback(SOT_INT8 * pcSotNum);

//错误上报
SOT_VOID   ErrorReportCallback(SOT_INT8 * pcSotNum, SOT_ERROR_CODE_E eErrorCode);

typedef  SOT_BOOL (__stdcall *PSoftTerminalInit)(const SOT_SYSCFG_S * pstParamSet,const SOT_APP_REG_FN_S *pstCallBackSet);
typedef  SOT_BOOL (__stdcall *PSoftTerminalAddCallChannel)(SOT_CHANNEL_INFO_S* channelInfo);
typedef  SOT_BOOL (__stdcall *PSoftTerminalDeleteCallChannel)(SOT_INT8 *pszSotChannelLabel,SOT_UINT32 channelLabelLen);
typedef  SOT_BOOL (__stdcall *PSoftTerminalSendVideoData)(SOT_INT8 *pszSotChannelLabel,SOT_H264_DATA_TYPE_E dataType,unsigned char * pszData,int len);
typedef  SOT_BOOL (__stdcall *PSoftTerminalSendG711AudioData)(SOT_INT8 *pszSotChannelLabel,SOT_AUDIO_DATA_TYPE_E dataType,unsigned char * pszData,int len);

public ref class SoftTerminalAdapter
{
public:
	// 构造函数
	SoftTerminalAdapter();
	//析构函数
	~SoftTerminalAdapter();

public:
	// 初始化软终端
	SmcErr^ Init(InitSotParam^ param);

	// 注册回调函数
	bool RegistSotCallback(SoftTerminalCallback^ callback);

	// 添加软终端呼叫通道
	bool AddChannel(String^ channelLabel, String^ confLabel);

	// 删除软终端呼叫通道
	bool RemoveChannel(String^ channelLabel);

	// 发送码流
	bool SendMediaData(String^ channelLabel, MediaData^ data);

	std::string SoftTerminalAdapter::GetCurPath(void);

private:
	// 将托管byte数组转换成unsigned char*
	unsigned char* AllocateNativeMemory(IN array<Byte> ^ManagedArray);

	//模块句柄
	HMODULE  m_hDllInst;

	//初始处理函数
	PSoftTerminalInit pSoftTerminalInit;

	//添加呼叫通道
	PSoftTerminalAddCallChannel pSoftTerminalAddCallChannel;

	//删除呼叫通道
	PSoftTerminalDeleteCallChannel pSoftTerminalDeleteCallChannel;

	//发送数据，有2类数据：1）rtp包，2）一帧的裸的数据
	PSoftTerminalSendVideoData pSoftTerminalSendVideoData;

	//发送音频数据(G.711a或者G.711u) 
	PSoftTerminalSendG711AudioData pSoftTerminalSendG711AudioData;

public:
	// NLog日志句柄
	static NLogCallback^ NLogHandler;

	// I帧重传句柄
	static IFrameCallback^ IFrameHandler;

	// 错误回调句柄
	static SotErrorReport^ errReportHandler;
};

#endif // !defined(AFX_SOFTTERMINALADAPTER_20130321)