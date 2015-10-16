/*****************************************************************************
*               Copyright (C) 2013, Huawei Tech. Co., Ltd.               
*                           ALL RIGHTS RESERVED                               
* FileName: soft_terminal_api.h                                                          
* Version: 1.0                                                                
* Description:  呼叫模块的呼叫处理头文件    
*                                                                             
* History:                                                                    
* 1. 2013-03-14,y00186060 Create this file.                                 
******************************************************************************/

#ifndef __CALL_MGNT_H__
#define __CALL_MGNT_H__

#include "soft_terminal_type.h"


#ifdef __cplusplus
extern "C"{
#endif


//初始化参数集合
/* see in soft_terminal_type.h*/

//外部注册的回调函数集合
/* see in soft_terminal_type.h*/

//初始处理
SOT_BOOL SoftTerminalInit(const SOT_SYSCFG_S * pstParamSet,const SOT_APP_REG_FN_S *pstCallBackSet);

//添加呼叫通道
SOT_BOOL SoftTerminalAddCallChannel(const SOT_CHANNEL_INFO_S * pstChannelInfo);

//删除呼叫通道
SOT_BOOL SoftTerminalDeleteCallChannel(SOT_INT8 * pcCallNumber,SOT_UINT32 ulCallNumberLen);

//发送数据，有2类数据：1）rtp包，2）一帧的裸的数据
SOT_BOOL SoftTerminalSendVideoData(SOT_INT8 * pcCallNumber, SOT_H264_DATA_TYPE_E edataType,SOT_UINT8 * pucData,SOT_UINT32 ulDataLen);

//发送音频数据，旧的接口，待删除  
SOT_BOOL SoftTerminalSendAudioData(SOT_INT8 * pcCallNumber,SOT_UINT8 * pucData,SOT_UINT32 ulDataLen);

//发送音频数据(G.711a或者G.711u) 
SOT_BOOL SoftTerminalSendG711AudioData(SOT_INT8 * pcCallNumber,SOT_AUDIO_DATA_TYPE_E edataType,SOT_UINT8 * pucData,SOT_UINT32 ulDataLen);

//请求I帧重传
SOT_BOOL SoftTerminalRequestIFrame(SOT_INT8 * pcCallNumber);

//查询通道状态
SOT_CHANNEL_STATUS_E SoftTerminalGetChannelStatus(SOT_INT8 * pcCallNumber);


#ifdef __cplusplus
}
#endif

#endif
