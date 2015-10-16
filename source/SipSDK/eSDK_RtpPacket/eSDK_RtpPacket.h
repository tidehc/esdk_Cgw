#ifndef ESDK_RTP_PACKET_H_
#define ESDK_RTP_PACKET_H_
#ifdef ESDK_RTPPACKET_EXPORTS
#define ESDK_RTPPACKET  extern "C"  __declspec(dllexport)
#else
#define ESDK_RTPPACKET  extern "C"  __declspec(dllimport)
#endif

#ifdef WIN32
#define __SDK_CALL __stdcall
#else
#define __SDK_CALL
#endif

//自定义数据类型
typedef unsigned int	UINT32;
typedef int				INT32;

//接口返回值
typedef enum _EM_RET_RESULT
{
	_RET_SUCCESS				=	0,	//成功
	_RET_FAILURE				=	1	//失败
}EM_RET_RESULT;

//码流类型
typedef enum _EM_StreamType
{
	_PAY_LOAD_NO_TYPE = -1,
	_PAY_LOAD_TYPE_PCMU = 0,		// G711的u率
	_PAY_LOAD_TYPE_PCM = 1,
	_PAY_LOAD_TYPE_G723 = 4,		// G723
	_PAY_LOAD_TYPE_PCMA = 8,		// G711的a率
	_PAY_LOAD_TYPE_G722 = 9,		// G722

	_PAY_LOAD_TYPE_G726 = 16,	// G726
	_PAY_LOAD_TYPE_AAC = 17,		// AAC

	_PAY_LOAD_TYPE_MJPEG = 26,	// MJPEG
	_PAY_LOAD_TYPE_H264_1 = 96,	// H264(非标准定义)
	_PAY_LOAD_TYPE_AMR_MB = 97,	// AMR_NB
	_PAY_LOAD_TYPE_MPEG4 = 98,	// MPEG4
	_PAY_LOAD_TYPE_H264 = 99,	// H264
	_PAY_LOAD_TYPE_AVS = 100,	// AVS
	_PAY_LOAD_TYPE_MP3 = 101,	// MP3
	_PAY_LOAD_TYPE_3GPP = 102,	// 3GPP
	_PAY_LOAD_TYPE_TRACK = 107  // 轨迹
}EM_StreamType;

// H264帧类型
typedef enum _EM_H264NaluType
{
	_H264_NALU_TYPE_UNDEFINED = 0,
	_H264_NALU_TYPE_SLICE = 1,
	_H264_NALU_TYPE_DPA = 2,
	_H264_NALU_TYPE_DPB = 3,
	_H264_NALU_TYPE_DPC = 4,
	_H264_IDR_NALU_TYPE = 5,
	_H264_SEI_NALU_TYPE = 6,
	_H264_SPS_NALU_TYPE = 7,
	_H264_PPS_NALU_TYPE = 8,
	_H264_NALU_TYPE_AUD = 9,
	_H264_NALU_TYPE_EOSEQ = 10,
	_H264_NALU_TYPE_EOSTREAM = 11,
	_H264_NALU_TYPE_FILL = 12,

	_H264_NALU_TYPE_STAP_A = 24,
	_H264_NALU_TYPE_STAP_B = 25,
	_H264_NALU_TYPE_MTAP16 = 26,
	_H264_NALU_TYPE_MTAP24 = 27,
	_H264_NALU_TYPE_FU_A = 28,
	_H264_NALU_TYPE_FU_B = 29,
	_H264_NALU_TYPE_END = 30
}EM_H264NaluType;

//回调的帧数据类型
typedef struct
{
	INT32 iStreamType;		//流类型,参考EM_StreamType
	INT32 iFrameDataType;	//帧类型,参考EM_H264NaluType
}ST_FRAME_DATA;

//帧数据回调函数定义
typedef void (__SDK_CALL * FrameDataCallBack)( void* pBuf, UINT32 uiBufSize,ST_FRAME_DATA* pFrameData,UINT32 uiChannel);

#ifdef __cplusplus
extern "C"
{
#endif
	/**
	 *初始化
	 * 
	 *该函数用于初始化
	 *
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	最先开始调用的函数
	 *@par			无
	**/
	ESDK_RTPPACKET INT32  __SDK_CALL ESDK_RTP_Init(void);

	/**
	 *打开通道处理ps码流
	 * 
	 *该函数用于打开通道处理码流
	 *
	 *@param[in]	frameDataCallBack	回调接口
	 *@param[in]	uiChannel			通道号
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	无
	 *@par			无
	**/
	ESDK_RTPPACKET INT32  __SDK_CALL ESDK_RTP_OpenChannel(FrameDataCallBack frameDataCallBack,UINT32 uiChannel);

	/**
	 *打开通道处理es码流
	 * 
	 *该函数用于打开通道处理码流
	 *
	 *@param[in]	frameDataCallBack	回调接口
	 *@param[in]	uiChannel			通道号
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	无
	 *@par			无
	**/
	ESDK_RTPPACKET INT32  __SDK_CALL ESDK_RTP_OpenESChannel(FrameDataCallBack frameDataCallBack,UINT32 uiChannel);

	/**
	 *打开通道处理码流
	 * 
	 *该函数用于打开通道处理码流
	 *
	 *@param[in]	pBuf				输入码流的地址
	 *@param[in]	uiBufSize			码流数据的大小
	 *@param[in]	uiChannel			通道号
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	收到码流回调后调用此参数
	 *@par			无
	**/
	ESDK_RTPPACKET INT32  __SDK_CALL ESDK_RTP_ProcessPacket(char* pBuf, UINT32 uiBufSize,UINT32 uiChannel);  

	/**
	 *设置是否需要音频帧回调
	 * 
	 *该函数用于设置是否需要音频帧回调
	 *
	 *@param[in]	bIsNeedAudioFrame	通道号，0表示不需要，非0表示需要
	 *@param[in]	uiChannel			通道号
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	无
	 *@par			无
	**/
	ESDK_RTPPACKET INT32  __SDK_CALL ESDK_RTP_SetIsNeedAudioFrame(int bIsNeedAudioFrame,UINT32 uiChannel);

	/**
	 *关闭通道处理码流
	 * 
	 *该函数用于关闭通道处理码流
	 *
	 *@param[in]	uiChannel			通道号
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	无
	 *@par			无
	**/
	ESDK_RTPPACKET INT32  __SDK_CALL ESDK_RTP_CloseChannel(UINT32 uiChannel);

	/**
	 *去初始化
	 * 
	 *该函数用于去初始化
	 *
	 *@return		0	成功
	 *@return		非0	失败（参考错误返回码）
	 *@attention	无
	 *@par			无
	**/
	ESDK_RTPPACKET INT32  __SDK_CALL ESDK_RTP_UnInit(void);

#ifdef __cplusplus
}
#endif 

#endif
