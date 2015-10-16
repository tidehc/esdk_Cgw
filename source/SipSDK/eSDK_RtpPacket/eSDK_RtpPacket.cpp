// eSDK_RtpPacket.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RtpPacket.h"
#include "Ps2EsProcessor.h"
#include "ace_header.h"
#include "eSDK_RtpPacket.h"
#include "PacketProcessMgr.h"
#include "eSDKTool.h"
#include "eSDKMem.h"
#include "Log.h"

#ifdef WIN32
#pragma comment(linker, "/EXPORT:ESDK_RTP_Init=_ESDK_RTP_Init@0")
#pragma comment(linker, "/EXPORT:ESDK_RTP_OpenChannel=_ESDK_RTP_OpenChannel@8")
#pragma comment(linker, "/EXPORT:ESDK_RTP_OpenESChannel=_ESDK_RTP_OpenESChannel@8")
#pragma comment(linker, "/EXPORT:ESDK_RTP_CloseChannel=_ESDK_RTP_CloseChannel@4")
#pragma comment(linker, "/EXPORT:ESDK_RTP_SetIsNeedAudioFrame=_ESDK_RTP_SetIsNeedAudioFrame@8")
#pragma comment(linker, "/EXPORT:ESDK_RTP_ProcessPacket=_ESDK_RTP_ProcessPacket@12")
#pragma comment(linker, "/EXPORT:ESDK_RTP_UnInit=_ESDK_RTP_UnInit@0")
#endif
//lint -e1784
//lint -e438
INT32 __SDK_CALL ESDK_RTP_Init(void)
{
	std::string path;
	eSDKTool::getCurrentPath(path);
	std::string strLogCfgFile = path+"\\eSDKClientLogCfg.ini";
	std::string strLogStorePath = path+"\\rtplog";
	unsigned int logLevel[LOG_CATEGORY] = {INVALID_LOG_LEVEL,INVALID_LOG_LEVEL,INVALID_LOG_LEVEL};
	LogInit(PRODUCT_NAME,strLogCfgFile.c_str(),logLevel,strLogStorePath.c_str());
	INFO_TRACE("");
	(void)PCAKETPROCESSORMGR();
	return _RET_SUCCESS;
}
//lint -e818
void g_afterPacketProtockChangeDataCallBack(void* pUser,ACE_Message_Block* pRtpBlock,ST_FRAME_DATA* pFreameData)
{	
	if(NULL == pUser)
	{
		ERROR_LOG("pUser is NULL.");
		return;
	}
	if(NULL == pRtpBlock)
	{
		ERROR_LOG("RtpBlock is NULL.");
		return;
	}
	if (0 == pRtpBlock->length())
	{
		ERROR_LOG("RtpBlock length is 0;");
		return;
	}

	DEBUG_LOG("iStreamType=%d,iFrameDataType=%d",pFreameData->iStreamType,pFreameData->iFrameDataType);

	CPacketChangeProcessor* processor = (CPacketChangeProcessor*)pUser;
	const FrameDataCallBack pCBFunc = processor->GetFrameDataCallBack();
	if(NULL != pCBFunc)
	{
		pCBFunc(pRtpBlock->rd_ptr(), pRtpBlock->length(),pFreameData,processor->GetChannel());
	}	
} 
//lint +e818
INT32  __SDK_CALL ESDK_RTP_OpenChannel(FrameDataCallBack frameDataCallBack,UINT32 uiChannel)
{
	if(NULL == frameDataCallBack)
	{
		ERROR_LOG("frameDataCallBack is NULL.");
		return _RET_FAILURE;
	}

	INFO_LOG("frameDataCallBack=0X%x,uiChannel=%d",frameDataCallBack,uiChannel);

	CPacketChangeProcessor* processor = PCAKETPROCESSORMGR().OpenChannel(uiChannel,PS2ES);
	if(NULL == processor)
	{
		ERROR_LOG("processor is NULL.");
		return _RET_FAILURE;
	}
	processor->setDataCallBack(g_afterPacketProtockChangeDataCallBack, processor);
	processor->SetFrameDataCallBack(frameDataCallBack);

	return _RET_SUCCESS;
}

INT32  __SDK_CALL ESDK_RTP_OpenESChannel(FrameDataCallBack frameDataCallBack,UINT32 uiChannel)
{
	if(NULL == frameDataCallBack)
	{
		ERROR_LOG("frameDataCallBack is NULL.");
		return _RET_FAILURE;
	}

	INFO_LOG("frameDataCallBack=0X%x,uiChannel=%d",frameDataCallBack,uiChannel);

	CPacketChangeProcessor* processor = PCAKETPROCESSORMGR().OpenChannel(uiChannel,ES2PS);
	if(NULL == processor)
	{
		ERROR_LOG("processor is NULL.");
		return _RET_FAILURE;
	}
	processor->setDataCallBack(g_afterPacketProtockChangeDataCallBack, processor);
	processor->SetFrameDataCallBack(frameDataCallBack);
	return _RET_SUCCESS;

}

INT32  __SDK_CALL ESDK_RTP_CloseChannel(UINT32 uiChannel)
{
	INFO_LOG("uiChannel=%d",uiChannel);
	return PCAKETPROCESSORMGR().CloseChannel(uiChannel);
}


INT32 __SDK_CALL ESDK_RTP_SetIsNeedAudioFrame(int bIsNeedAudioFrame,UINT32 uiChannel)
{
	CPacketChangeProcessor* processor  = PCAKETPROCESSORMGR().GetProcessor(uiChannel);
	if(NULL == processor)
	{
		ERROR_LOG("processor is NULL.");
		return _RET_FAILURE;
	}
	bool iNeed = false;
	if(0 != bIsNeedAudioFrame)
	{
		iNeed = true;
	}
	return processor->setIsNeedAudioFrame(iNeed);
}
//lint -e818
INT32 __SDK_CALL ESDK_RTP_ProcessPacket(char* pBuf, UINT32 uiBufSize,UINT32 uiChannel)
{	
	INFO_LOG("uiBufSize=%d,uiChannel=%d",uiBufSize,uiChannel);
	CPacketChangeProcessor* processor  = PCAKETPROCESSORMGR().GetProcessor(uiChannel);
	if(NULL == processor)
	{
		INFO_LOG("uiBufSize=%d,uiChannel=%d",uiBufSize,uiChannel);
		ERROR_LOG("processor is NULL.");
		return _RET_FAILURE;
	}

	ACE_Message_Block* pMb = new ACE_Message_Block(pBuf, uiBufSize, 0);
	pMb->length(uiBufSize);
	processor->receviePacket(pMb);
	processor->SetChannel(uiChannel);

	delete pMb;
	pMb = NULL;
	
	return _RET_SUCCESS;
}
//lint +e818
INT32  __SDK_CALL ESDK_RTP_UnInit(void)
{
	LogFini(PRODUCT_NAME);
	return _RET_SUCCESS;
}

//lint +e438
//lint +e1784

