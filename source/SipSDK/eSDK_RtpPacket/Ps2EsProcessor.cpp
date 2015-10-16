/********************************************************************
 filename    :    Ps2EsProcessor.cpp
 author      :     
 created     :    2014-03-10
 description :    实现一个PS流转换成ES流的处理器
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013-02-21 初始版本
*********************************************************************/
#include "stdafx.h"
#include "ace_header.h"
// 错误码头文件
#include "ivs_error.h"
#include "stdint.h"
#include "nss_df_c02.h"
#include "NssMedia.h"
#include "RtpPacket.h"
#include "Ps2EsProcessor.h"
#include "Common_Def.h"
#include "Log.h"
#include "eSDKTool.h"
#include "eSDKMem.h"

#define SEGMENT_MAX_SIZE_T 1400            //帧分片最大字节数
//lint -e826
//lint -e818
CPs2EsProcessor::CPs2EsProcessor()
{
    m_strServiceID   = "";
    m_pRtpFrameCache = NULL;
    m_pWritePos      = NULL;
    m_pHstPESParse   = NULL;
    m_pExtendHeader  = NULL;
    
    m_usVideoRtpSeq      = 0;
    m_usAudioRtpSeq      = 0;
    m_ulVideoTimeTick    = 0;
    m_ulAudioTimeTick    = 0;
    m_ucVideoPayloadType = IVS_PT_H264;
    m_bIsFirstTime       = false;

	m_bIsNeedAudioFrame = true;

    m_curPacketList.clear();

    m_usecLastSendTime    = 0;
    m_tmLastSendTime      = 0;
    m_uiRealRecordSecond  = 0;
    m_uiRealRecordMSecond = 0;
    m_uiReserved          = 0;
}

CPs2EsProcessor::~CPs2EsProcessor()
{
	try 
	{
		ReleasePktList();
	}
	catch(...)
	{
	}

    if (NULL != m_pRtpFrameCache)
    {
        HW_DELETE_A(m_pRtpFrameCache);
        m_pWritePos = NULL;
    }
    
    if (NULL != m_pHstPESParse)
    {        
		try 
		{
			(void)m_pHstPESParse->Delete();
		}
		catch(...)
		{
		}
        
        try
        {
            HW_DELETE(m_pHstPESParse);
        }
        catch(...)
        {
            m_pHstPESParse = NULL;
        }
    }
	try 
	{
		m_pExtendHeader->release();
		m_pExtendHeader = NULL;
	}
	catch(...)
	{
	}
	m_pWritePos      = NULL;
}
//lint -e423
int CPs2EsProcessor::init( std::string strServiceID )
{
    try
    {
        m_pExtendHeader   = new ACE_Message_Block(MDU_MSG_BLOCK_SIZE);
    }
    catch(...)
    {
        m_pExtendHeader   = NULL;
    }

    if(NULL == m_pExtendHeader)
	{
		return IVS_PARA_INVALID;
	}
    m_pExtendHeader->reset();
    if (IVS_SUCCEED != m_RtpFrameOrganizer.init((IRtpFrameHandler*)this, strServiceID.c_str()))
    {
        m_pExtendHeader->release();
        m_pExtendHeader = NULL;
        //BP_RUN_LOG_ERR(IVS_FAIL, "Init frame organizer fail", "serviceID=%s", strServiceID.c_str());
        return IVS_FAIL;
    }

    if (NULL == m_pRtpFrameCache)
    {
        HW_NEW_A(m_pRtpFrameCache, char, RTP_FRAME_CACHE_SIZE);  //lint !e774 !e831
        //CHECK_POINTER(m_pRtpFrameCache, IVS_ALLOC_MEMORY_ERROR);
        m_pWritePos = m_pRtpFrameCache;
    }

    m_strServiceID        = strServiceID;
    m_bIsFirstTime        = true;
    m_uiRealRecordSecond  = 0;
    m_uiRealRecordMSecond = 0;
    m_uiReserved          = 0;
    //BP_RUN_LOG_INF("Init ps to es processor success", "serviceID=%s.", m_strServiceID.c_str());
    return IVS_SUCCEED;
}
//lint +e423

/*****************************************************************************
 Function:    receviePacket
 Description: 接收一包PS媒体数据
 Input:       pMb 媒体流数据块
 Output:      
 Return:      N/A
*****************************************************************************/
//lint -e1788
void CPs2EsProcessor::receviePacket(const ACE_Message_Block* pMb)
{
    //CHECK_POINTER_VOID(pMb);
	CAutoLock iLock(m_Mutex);	
	 
    //  第一次时需要获得RTP包中的数据来初始化RTP信息
    if (m_bIsFirstTime)
    {
        CRtpPacket rtpPacket;
        int iRet = rtpPacket.ParsePacket(pMb->rd_ptr(), pMb->length());
        if (IVS_SUCCEED != iRet)
        {
            //BP_RUN_LOG_ERR(IVS_FAIL, "Parse rtp packet fail",                "retcode=%d, serviceID=%s.", iRet, m_strServiceID.c_str());
            return;
        }

        m_usVideoRtpSeq = rtpPacket.GetSeqNum();
        m_usAudioRtpSeq = (m_usVideoRtpSeq/2);
        m_bIsFirstTime = false;
    }

    if (IVS_SUCCEED != m_RtpFrameOrganizer.insertRtpPacket(pMb))
    {
        //BP_RUN_LOG_ERR(IVS_FAIL, "Insert rtp packet to frame organizer fail","serviceID=%s.", m_strServiceID.c_str());
        return;
    }

    return;
}
//lint +e1788

/*****************************************************************************
 Function:    sendEsRtpPacket
 Description: 
 Input:       pRtpData  解析成ES流后的一包数据
              unDataSize ES流的一包数据的长度
 Output:      
 Return:      N/A
*****************************************************************************/
void CPs2EsProcessor::sendEsRtpPacket(const char* pRtpData, unsigned int unDataSize)
{
    //CHECK_POINTER_VOID(pRtpData);

    if (0 == unDataSize)
    {        
        //BP_RUN_LOG_ERR(IVS_FAIL, "Send Es rtp packet fail, data size invalid",            "size=%u, serviceID=%s.", unDataSize, m_strServiceID.c_str());
        return;
    }

    ACE_Message_Block *pRtpBlock = CMediaBlockBuffer::instance().allocMediaBlock();
    if (NULL == pRtpBlock)
    {
        //BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Alloc media block fail.",            "serviceID=%s.", m_strServiceID.c_str());
        return;
    }
	NALU_HEADER naluheader = *(NALU_HEADER*)pRtpBlock->rd_ptr();
    pRtpBlock->reset();
    pRtpBlock->copy(pRtpData, unDataSize);

	ST_FRAME_DATA frameData;
	frameData.iFrameDataType = naluheader.TYPE;
	frameData.iStreamType = _PAY_LOAD_TYPE_H264;
	//m_Mutex.Lock();
	if(NULL != m_fDataCallBack)
	{
		m_fDataCallBack(m_pUser, pRtpBlock,&frameData);
	}
	//m_Mutex.UnLock();
    CMediaBlockBuffer::instance().freeMediaBlock(pRtpBlock);
    return;
}

/*****************************************************************************
 Function:    handleMediaFrame
 Description: 拼完一帧PS数据后处理媒体数据
 Input:       rtpFrameList PS数据包列表
 Output:      
 Return:      N/A
*****************************************************************************/
void CPs2EsProcessor::handleMediaFrame(RTP_FRAME_LIST_T &rtpFrameList)
{
	if(NULL == m_pExtendHeader )
	{
		ERROR_LOG("m_pExtendHeader is NULL");
		return;
	}
	if(NULL == m_pRtpFrameCache)
	{
		ERROR_LOG("m_pRtpFrameCache is NULL");
		return;
	}
    if (rtpFrameList.empty())
    {
        ERROR_LOG("Handle PS media frame abnormal , the frame list is empty");
        return;
    }

    if (MAX_RTP_PACKET_COUNT < rtpFrameList.size())
    {
        ERROR_LOG("Handle PS media frame abnormal , the frame list exceeds the Threshold[1024], the rtp packet count: %d",rtpFrameList.size());
        return;
    }

    // 新的一帧到达，缓存应该是空的
    if (m_pWritePos != m_pRtpFrameCache)
    {
        m_pWritePos = m_pRtpFrameCache;
        //BP_RUN_LOG_INF("Handle PS media frame abnormal",            "Write postion not in cache head, serviceID=%s.", m_strServiceID.c_str());
    }

    // 将收到的一帧PS数据拷贝至缓冲区
    ACE_Message_Block* pRtpBlock = NULL;
    bool bFirst = true;
    CRtpPacket rtpPacket;
    unsigned int unCacheSize = RTP_FRAME_CACHE_SIZE;
    int iRet = IVS_SUCCEED;
    for (RTP_FRAME_LIST_T_ITER iter = rtpFrameList.begin(); iter != rtpFrameList.end(); ++iter)
    {
        pRtpBlock = *iter;
        iRet = rtpPacket.ParsePacket(pRtpBlock->rd_ptr(), pRtpBlock->length());
        if (IVS_SUCCEED != iRet)
        {
            m_pWritePos = m_pRtpFrameCache;
            m_pExtendHeader->reset();
            ERROR_LOG("Parse rtp packet fail ,retcode:%d",iRet);
            return;
        }

        if (bFirst)
        {
            if (1 == rtpPacket.GetExtension())
            {
                if(NULL != rtpPacket.GetMuExtData())
                {
                    m_pExtendHeader->copy((char *)rtpPacket.GetMuExtData(), sizeof(RTP_EXTENSION_DATA_MU_S));
                }
                else if(NULL != rtpPacket.GetExtData())
                {
                    m_pExtendHeader->copy((char *)rtpPacket.GetExtData(), sizeof(RTP_EXTENSION_DATA_S));
                }
                else
                {
                   ERROR_LOG("Error extension label");
                }
            }

            if (m_bAppendExtInfo)
            {
                REAL_RECORD_TIME* realRecordTime = (REAL_RECORD_TIME*)(pRtpBlock->base());
                if (NULL != realRecordTime)
                {
                    m_uiRealRecordSecond = realRecordTime->uiSecond;
                    m_uiRealRecordMSecond = realRecordTime->uiMSecond;
                    uint32_t* pStreamRate = (uint32_t*)(pRtpBlock->base() + sizeof(REAL_RECORD_TIME));
                    m_uiReserved = *pStreamRate;
                }
                else
                {
                    ERROR_LOG("Error real record time info");
                }
            }

            bFirst = false;
        }

        // 移除RTP消息头
        pRtpBlock->rd_ptr(rtpPacket.GetHeadLen());

        if (unCacheSize >= pRtpBlock->length())
        {
            memcpy(m_pWritePos, pRtpBlock->rd_ptr(), pRtpBlock->length());
            m_pWritePos += pRtpBlock->length();
            unCacheSize -= pRtpBlock->length();
        }
        else
        {
            // 缓冲区长度不够
            ERROR_LOG("Current frame is too big exceed cache size 1.5M , will discard part data,  rtp package list size=%d,curr rtp package length &d",rtpFrameList.size(),rtpPacket.GetPacketLen());
            m_pWritePos = m_pRtpFrameCache;
            m_pExtendHeader->reset();
            return;
        }
    }

    // 将PS数据转换成ES数据
    int iVideoLen = 0;
    int iAudioLen = 0;
    int iTotalLen = m_pWritePos - m_pRtpFrameCache;
    unsigned char *pFrame = NULL;
    if (NRU_ZERO > iTotalLen || RTP_FRAME_CACHE_SIZE < iTotalLen)
    {
        m_pWritePos = m_pRtpFrameCache;
        m_pExtendHeader->reset();
         ERROR_LOG("Parse PS packet to ES fail and discard curr frame , ulVideoTimeTick:%d,iTotalLen:%d",m_ulVideoTimeTick,iTotalLen);
        return;
    }

    iRet = HSPspkt2ESFrm((unsigned char*)m_pRtpFrameCache, iTotalLen,
                                         pFrame, iVideoLen, iAudioLen);
    if (IVS_SUCCEED != iRet)
    {
        m_pWritePos = m_pRtpFrameCache;
        m_pExtendHeader->reset();
        ERROR_LOG("Parse PS packet to ES fail and discard curr frame,  ulVideoTimeTick:%d",m_ulVideoTimeTick);
        return;
    }

    if ((NRU_ZERO > iVideoLen || RTP_FRAME_CACHE_SIZE < iVideoLen)
        || (NRU_ZERO > iAudioLen || MAX_AUDIO_LENGTH < iAudioLen))
    {
        m_pWritePos = m_pRtpFrameCache;
        m_pExtendHeader->reset();
        ERROR_LOG("Parse PS packet to ES fail and discard curr frame, ulVideoTimeTick: %d,iVideoLen:%d,iAudioLen:%d",m_ulVideoTimeTick,iVideoLen,iAudioLen);
        return;
    }

    if (0 != iVideoLen)
    {
        (void)sendVideoFrame((char *)pFrame, iVideoLen);
    }

    if (0 != iAudioLen)
    {
        (void)sendAudioFrame((char *)(pFrame + iVideoLen), iAudioLen);
    }

    // 重置缓冲区写位置
    m_pWritePos = m_pRtpFrameCache;

    m_pExtendHeader->reset();
    return;
}


/*****************************************************************************
 Function:    HSPspkt2ESFrm
 Description: 拼完一帧PS数据后处理媒体数据
 Input:       pPacket PS帧数据缓存
              iPktLen PS帧数据长度
 Output:      pFrame 转换后数据存储缓存
              iVideoLen 视频长度
              iAudioLen 音频长度
 Return:      IVS_SUCCEED 成功
              其他 失败
*****************************************************************************/
int CPs2EsProcessor::HSPspkt2ESFrm(unsigned char * pPacket, int iPktLen, unsigned char *&pFrame,
                                   int &iVideoLen, int &iAudioLen)
{
    if (m_pHstPESParse == NULL)
    {
        HW_NEW(m_pHstPESParse, CHstPESParse);      //lint !e774 !e831
        if(NULL == m_pHstPESParse)
		{
			ERROR_LOG("m_pHstPESParse is NULL");
			return IVS_ALLOC_MEMORY_ERROR;
		}

        if (m_pHstPESParse->Create())
        {
            HW_DELETE(m_pHstPESParse);
            ERROR_LOG("Create PS parse fail");
            return IVS_FAIL;
        }
    }

    int nStreamId = 0;
    HstPSHeader stPSHeader;
    HstPSFrame stPSFrame;
    stPSFrame.buf = m_pHstPESParse->mRawFrameBuffer.pPayLoad;
    stPSFrame.length = 0;
    stPSFrame.audio_length = 0;
    stPSFrame.max = m_pHstPESParse->mRawFrameBuffer.nMaxLength;
    int nConsumedBytes  = 0;
    int nPSPacketLength = 0;
    int nVideoPayloadLength = 0;
    int nAudioPayloadLength = 0;
    unsigned char* pPSFrame = pPacket;
    unsigned char* pVideoPayload = m_pHstPESParse->mRawFrameBuffer.pCurPSPacketPayLoad;
    int uiTimes = 0;
    while (1) //lint !e716
    {
        nPSPacketLength = m_pHstPESParse->ParsePSPacket(
            &stPSHeader,
            pPSFrame + nConsumedBytes,
            iPktLen - nConsumedBytes,
            &stPSFrame,
            &nStreamId);
        if (nPSPacketLength <= 0)
        {
            ERROR_LOG("Parse ps packet fail, it's a bad ps packet");
            // 保证前面解析出来的数据不被抛弃
            if (0 != uiTimes)
            {
                break;
            }
            return IVS_FAIL;
        }

        nConsumedBytes += nPSPacketLength;
        if (nConsumedBytes > iPktLen)
        {
            ERROR_LOG("Parse ps packet fail, ps packet length is wrong.");
            return IVS_FAIL;
        }
        memcpy(pVideoPayload + nVideoPayloadLength, stPSFrame.buf, (unsigned int)(stPSFrame.length + stPSFrame.audio_length));
        nVideoPayloadLength += stPSFrame.length;
        nAudioPayloadLength += stPSFrame.audio_length;
        if (nConsumedBytes == iPktLen)
        {
            break;
        }

        ++uiTimes;

        // 此处为异常处理，出现失败直接恢复到原来的状态
        if (m_RtpFrameOrganizer.checkLostPacket()) //lint !e1013 !e1055 !e746
        {
            int iBuffLen = stPSFrame.length + stPSFrame.audio_length;
            unsigned char *pFindPESBuff = NULL;

            HW_NEW_A(pFindPESBuff, unsigned char, (unsigned int)iBuffLen);
            if (NULL == pFindPESBuff) //lint !e831
            {
            
                ERROR_LOG("Alloc finde PES header buff fail");
                continue;
            }

            memcpy(pFindPESBuff, stPSFrame.buf, (unsigned int)(stPSFrame.length + stPSFrame.audio_length));

            for (int i = iBuffLen - (SEGMENT_MAX_SIZE_T / 2); i >= SEGMENT_MAX_SIZE_T; --i)
            {
                unsigned char ucType = 0;
                int iRet = m_pHstPESParse->CheckPSHeader(pFindPESBuff + i, 4, ucType);
                if (IVS_SUCCEED != iRet)
                {
                    continue;
                }
                else
                {
                    if (0xc0 == ucType)
                    {
                        nAudioPayloadLength -= (iBuffLen - i);
                    }
                    else
                    {
                        nVideoPayloadLength -= (iBuffLen - i);
                    }
                    nConsumedBytes -= (iBuffLen - i);
                    //BP_DBG_LOG("Find pes header success, index=%d", i);
                    break;
                }
            }

            HW_DELETE_A(pFindPESBuff);
        }   //lint !e438
    }

    pFrame = pVideoPayload;

    iVideoLen = nVideoPayloadLength;
    iAudioLen = nAudioPayloadLength;
    m_ulVideoTimeTick = (unsigned long)stPSFrame.pts;
    m_ulAudioTimeTick = (unsigned long)stPSFrame.pts;

    return IVS_SUCCEED;
}





/*****************************************************************************
 Function:    setIsNeedAudioFrame
 Description: 设置是否返回音频帧数据
							设置m_bIsNeedAudioFrame值
 Input:       isNeedAudioFrame 是否需要返回音频帧数据 true表示需要返回
 Output:      
 Return:      IVS_SUCCEED 成功
*****************************************************************************/
int CPs2EsProcessor::setIsNeedAudioFrame(bool isNeedAudioFrame)
{
	m_bIsNeedAudioFrame = isNeedAudioFrame;
	return IVS_SUCCEED;
}
/*****************************************************************************
 Function:    sendAudioFrame
 Description: 发送ES音频帧
							根据用户设置判断是否返回音频帧，使用m_bIsNeedAudioFrame控制
 Input:       pVideobuff ES帧数据缓存
              iVideobuffLen ES帧数据长度
 Output:      
 Return:      IVS_SUCCEED 成功
              其他 失败
*****************************************************************************/
//lint -e593
int CPs2EsProcessor::sendAudioFrame(char *pAudiobuff, int iAudiobuffLen)
{
    //CHECK_POINTER(pAudiobuff, IVS_PARA_INVALID);
    if (0 == iAudiobuffLen)
    {        
        ERROR_LOG("Send Es audio rtp packet fail, data size invalid , size: %d",iAudiobuffLen);
        return IVS_FAIL;
    }

	//直接放回帧
	if (m_bIsNeedAudioFrame)
	{ 
		ACE_Message_Block *pSendBlock = new ACE_Message_Block(pAudiobuff, (unsigned int)iAudiobuffLen, 0);
		pSendBlock->length((unsigned int)iAudiobuffLen);
		ST_FRAME_DATA framedata;
		framedata.iFrameDataType = 0;					
		switch (m_ucAudioEncodeMode)
		{
		case IVS_PT_G711A:
			{
				framedata.iStreamType = _PAY_LOAD_TYPE_PCMA;
				break;
			}
		case IVS_PT_G711U:
			{
				framedata.iStreamType = _PAY_LOAD_TYPE_PCMU;
				break;
			}
		case IVS_PT_G726:
			{
				framedata.iStreamType = _PAY_LOAD_TYPE_G726;
				break;
			}
		case IVS_PT_AAC:
			{
				framedata.iStreamType = _PAY_LOAD_TYPE_AAC;
				break;
			}
		default:
			{
				framedata.iStreamType = _PAY_LOAD_TYPE_PCMA;			
			}

		}
		//m_Mutex.Lock();
		if(NULL != m_fDataCallBack)
		{
			m_fDataCallBack(m_pUser, pSendBlock,&framedata);
		}		
		//m_Mutex.UnLock();
	}
	return IVS_SUCCEED;   
}
//lint +e593

/*****************************************************************************
 Function:    sendVideoFrame
 Description: 发送ES视频帧
 Input:       pVideobuff ES帧数据缓存
              iVideobuffLen ES帧数据长度
 Output:      
 Return:      IVS_SUCCEED 成功
              其他 失败
*****************************************************************************/
int CPs2EsProcessor::sendVideoFrame(char *pVideobuff, int iVideobuffLen)
{
    //CHECK_POINTER(pVideobuff, IVS_PARA_INVALID);
    if (0 == iVideobuffLen)
    {        
        ERROR_LOG("Send Es video rtp packet fail, data size invalid ,size:%d",iVideobuffLen);
        return IVS_FAIL;
    }
 
    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, pObj, m_listMutex, IVS_FAIL);
    int iRet = parseH264frame(pVideobuff, iVideobuffLen);
    if (IVS_SUCCEED != iRet)
    {
        ERROR_LOG("Parse h264 frame fail");
        return IVS_FAIL;
    }



    if (m_curFrameList.empty())
    {
        ERROR_LOG("Send curr frame fail, frame list is empty");
        return IVS_FAIL;
    }

    std::list <ACE_Message_Block*>::iterator mbIter;
    for (mbIter = m_curFrameList.begin(); mbIter != m_curFrameList.end(); ++mbIter)
    {
        ACE_Message_Block *pMb = *mbIter;
        NALU_HEADER naluheader = *(NALU_HEADER*)pMb->rd_ptr();

		ST_FRAME_DATA framedata;
		framedata.iFrameDataType = naluheader.TYPE;
		framedata.iStreamType = _PAY_LOAD_TYPE_H264;
		
		//m_Mutex.Lock(); 
		if(NULL != m_fDataCallBack)
		{
			m_fDataCallBack(m_pUser, pMb,&framedata);
		}
        //m_Mutex.UnLock();
    }

    ReleaseFrameList();

    if (m_curPacketList.empty())
    {
        //BP_RUN_LOG_ERR(IVS_FAIL, "Send video packet fail", "packet list is empty");
        return IVS_FAIL;
    }

    //for (mbIter = m_curPacketList.begin(); mbIter != m_curPacketList.end(); ++mbIter)
    //{
    //    ACE_Message_Block *pSendBlock = *mbIter;
    //    handleEsRtpPacket((char *)pSendBlock->rd_ptr(), pSendBlock->length());
    //}

    //ReleasePktList();

    return IVS_SUCCEED;
}

/*****************************************************************************
 Function:    parseH264frame
 Description: 解析H264帧
 Input:       pVideobuff ES帧数据缓存
              iBuffLen ES帧数据长度
 Output:      
 Return:      IVS_SUCCEED 成功
              其他 失败
*****************************************************************************/
int CPs2EsProcessor::parseH264frame(char *pVideobuff, int iBuffLen)
{
    //CHECK_POINTER(pVideobuff, IVS_PARA_INVALID);
    if (0 == iBuffLen)
    {
        ERROR_LOG("Send Es video rtp packet fail, data size invalid,size:%d",iBuffLen);
        return IVS_FAIL;
    }

    // 清除视频帧数据存储的内存
    ReleaseFrameList();

    char *pReadPoint = pVideobuff;
    char *pCopyPoint = pVideobuff;
    int iLeftLength = iBuffLen;
    unsigned char ucType = H264_NALU_TYPE_UNDEFINED;

    int iRet = parseSingleNalu(pReadPoint, pCopyPoint, iLeftLength, ucType);
    if (IVS_SUCCEED != iRet)
    {
        return iRet;
    }

    // 是SPS、PPS、SEI时只有一个包，长度需重新计算
    if ((H264_NALU_TYPE_SPS == ucType)
        || (H264_NALU_TYPE_PPS == ucType)
        || (H264_NALU_TYPE_SEI == ucType))
    {
        iLeftLength = iBuffLen - (pCopyPoint - pVideobuff);
        ACE_Message_Block * pMb = NULL;
        try
        {
            pMb = new ACE_Message_Block((unsigned int)iLeftLength);
        }
        catch(...)
        {
            pMb = NULL;
        }

        if (NULL == pMb)  //lint !e774
        {
            ERROR_LOG("Receive alloc media block fail");
            ReleaseFrameList();
            return IVS_FAIL;
        }

        pMb->reset();
        pMb->copy(pCopyPoint, (unsigned int)iLeftLength);
        m_curFrameList.push_back(pMb);
        return IVS_SUCCEED;  //lint !e429
    }

    // 下面只能是I帧或P帧
    if (H264_START_CODE_LENGTH <= iLeftLength)
    {
        ACE_Message_Block * pMb = NULL;
        try
        {
            pMb = new ACE_Message_Block((unsigned int)iLeftLength);
        }
        catch(...)
        {
            pMb = NULL;
        }
        if (NULL == pMb)  //lint !e774
        {
            ERROR_LOG("Receive alloc media block fail");
            ReleaseFrameList();
            return IVS_FAIL;
        }

        pMb->reset();
        pMb->copy(pCopyPoint, (unsigned int)iLeftLength);
        m_curFrameList.push_back(pMb);
    }//lint !e429
    else
    {
        ERROR_LOG("Curr frame invalid");
        ReleaseFrameList();
        return IVS_FAIL;
    }

    return IVS_SUCCEED;
}//lint !e429

/*****************************************************************************
 Function:    parseSingleNalu
 Description: 解析单个帧
 Input:       pReadPoint ES帧数据缓存读指针
              pCopyPoint ES帧数据长度
 Output:      iLeftLength 剩余长度
              ucType 帧类型
 Return:      IVS_SUCCEED 成功
              其他 失败
*****************************************************************************/
int CPs2EsProcessor::parseSingleNalu(char *&pReadPoint, char *&pCopyPoint, int &iLeftLength, unsigned char &ucType)
{
    //CHECK_POINTER(pReadPoint, IVS_PARA_INVALID);
    //CHECK_POINTER(pCopyPoint, IVS_PARA_INVALID);
    unsigned int uiStartCodeLength = 0;

    // 此处是拆分一帧数据
    // I帧SPS|PPS|SEI|IDR含有0x00000001  拆分成SPS、PPS、SEI、IDR帧不含0x00000001，pCopyPoint返回最后一个0x00000001的位置
    // P类似
    while (H264_START_CODE_LENGTH <= iLeftLength)
    {
        if (checkStartCode(pReadPoint, uiStartCodeLength))  //lint !e826
        {
            NALU_HEADER naluheader = *(NALU_HEADER*)(pReadPoint + uiStartCodeLength);
            switch (naluheader.TYPE)
            {
            case H264_NALU_TYPE_SILCE:
            case H264_NALU_TYPE_IDR:
            case H264_NALU_TYPE_SEI:
            case H264_NALU_TYPE_PPS:
                {
                    // 非关键帧时
                    if (pReadPoint == pCopyPoint)
                    {
                        pCopyPoint = pReadPoint + uiStartCodeLength;
                        break;
                    }

                    ACE_Message_Block * pMb = NULL;
                    try
                    {
                        pMb = new ACE_Message_Block((unsigned int)(pReadPoint - pCopyPoint));
                    }
                    catch(...)
                    {
                        pMb = NULL;
                    }

                    if (NULL == pMb)  //lint !e774
                    {
                        ERROR_LOG("Receive alloc media block fail");
                        ReleaseFrameList();
                        return IVS_FAIL;
                    }
                    pMb->reset();
                    pMb->copy(pCopyPoint, (unsigned int)(pReadPoint - pCopyPoint));
                    m_curFrameList.push_back(pMb);
                    pCopyPoint = pReadPoint + uiStartCodeLength;
                    break;
                } //lint !e429
            case H264_NALU_TYPE_SPS:
                {
                    pCopyPoint = pReadPoint + uiStartCodeLength;
                    break;
                }
            default:
                break;
            }

            ucType = naluheader.TYPE;
            pReadPoint  += uiStartCodeLength;
            iLeftLength -= (int)uiStartCodeLength;
            if ((H264_NALU_TYPE_SILCE == naluheader.TYPE)   //lint !e747
                || (H264_NALU_TYPE_IDR == naluheader.TYPE)) //lint !e747
            {
                break;
            }

            continue;
        }

        ++pReadPoint;
        --iLeftLength;
    }

    return IVS_SUCCEED;
} //lint !e818

/*****************************************************************************
    Function:    checkStartCode
    Description: 校验数据包起始码
    Input:       
    Output:      
    Return:      true 成功
                其他 失败
*****************************************************************************/
//lint -e1762
bool CPs2EsProcessor::checkStartCode(char *pReadPoint, unsigned int &uiStartCodeLength)
{
    uiStartCodeLength = 0;

    if (NULL == pReadPoint)
    {
        return false;
    }

    unsigned long ulFrameValue = ntohl((unsigned long)*(int *)pReadPoint);
    if (H264_START_CODE == ulFrameValue)
    {
        uiStartCodeLength = H264_START_CODE_LENGTH;
        return true;
    }
    else if (H264_START_CODE_SHORT == (ulFrameValue & H264_START_CODE_VALUE))
    {
        uiStartCodeLength = H264_START_CODE_SHORT_LENGTH;
        return true;
    }

    return false;
}
//lint +e1762
/*****************************************************************************
 Function:    cutSliceToRtp
 Description: 将一个slice拆分成Rtp包
 Input:       pVideobuff ES帧数据缓存
              iBuffLen ES帧数据长度
 Output:      
 Return:      IVS_SUCCEED 成功
              其他 失败
*****************************************************************************/
int CPs2EsProcessor::cutSliceToRtp(char *pVideobuff, int iBuffLen)
{
    ////CHECK_POINTER(pVideobuff, IVS_PARA_INVALID);
    if (0 == iBuffLen)
    {
        //BP_RUN_LOG_ERR(IVS_FAIL, "Send Es video rtp packet fail, data size invalid",            "size=%u, serviceID=%s.", iBuffLen, m_strServiceID.c_str());
        return IVS_FAIL;
    }

    ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, pObj, m_listMutex, IVS_FAIL);

    NALU_HEADER naluheader = *(NALU_HEADER*)pVideobuff;

    //按1414字节把NSS帧拆分
    if (SEGMENT_MAX_SIZE_T >= iBuffLen)
    {
        ACE_Message_Block *pMsg = CMediaBlockBuffer::instance().allocMediaBlock();
        if (NULL == pMsg)
        {
            // 申请数据块失败
            //BP_RUN_LOG_ERR(IVS_FAIL, "Receive alloc media block fail", "");
            return IVS_FAIL;
        }

        pMsg->reset();

        appendExtInfo(pMsg);

        RTP_FIXED_HEADER *pRtpHeader = (RTP_FIXED_HEADER*)pMsg->wr_ptr(); //lint !e826
        setRtpInfo((char*)pRtpHeader, FRAME_TYPE_VIDEO, true);
        pMsg->wr_ptr(sizeof(RTP_FIXED_HEADER));
        if (0 != m_pExtendHeader->length())
        {
            pMsg->wr_ptr(sizeof(RTP_EXTEND_HEADER));         //有扩展头要偏移扩展头的长度
            pMsg->wr_ptr(m_pExtendHeader->length());         //有扩展头要偏移扩展头的长度
        }

        // 写Nalu头
        NALU_HEADER *pNaluHeader = (NALU_HEADER*)pMsg->wr_ptr();
        pNaluHeader->F = naluheader.F;
        pNaluHeader->NRI = naluheader.NRI;
        pNaluHeader->TYPE = H264_NALU_TYPE_FU_A;
        pMsg->wr_ptr(sizeof(NALU_HEADER));

        // 写Fu头
        FU_HEADER *pFuHeader = (FU_HEADER*)pMsg->wr_ptr();
        pFuHeader->S = 1;
        pFuHeader->E = 1;

        pFuHeader->R = 0;
        pFuHeader->TYPE = naluheader.TYPE;
        pMsg->wr_ptr(sizeof(FU_HEADER));

        // 加减1是为了偏移NALU_HEADER
        pMsg->copy((const char*)(pVideobuff + 1), (unsigned int)(iBuffLen - 1));
        m_curPacketList.push_back(pMsg);
    }
    else
    {
        int iCopyLen = 0;
        // 加减1是为了偏移NALU_HEADER
        char * pCopyData = const_cast <char*>(pVideobuff + 1);
        iBuffLen -= 1;
        bool bIsMarker = false;
        bool bIsFirst = true;
        while (0 < iBuffLen)
        {
            ACE_Message_Block *pMsg = CMediaBlockBuffer::instance().allocMediaBlock();
            if (NULL == pMsg)
            {
                // 申请数据块失败
                //BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Receive alloc media block fail", "");

                //如果发生错误，把本次拷贝的帧块清理
                //ReleasePktList();
                return IVS_FAIL;
            }
            pMsg->reset();

            if ((SEGMENT_MAX_SIZE_T - 2) < iBuffLen)
            {
                iCopyLen = (SEGMENT_MAX_SIZE_T - 2);
            }
            else
            {
                iCopyLen = iBuffLen;
                bIsMarker = true;
            }

            pMsg->reset();

            appendExtInfo(pMsg);

            // 写RTP头
            setRtpInfo(pMsg->rd_ptr(), FRAME_TYPE_VIDEO, bIsMarker);
            pMsg->wr_ptr(sizeof(RTP_FIXED_HEADER));
            if (0 != m_pExtendHeader->length())
            {
                pMsg->wr_ptr(sizeof(RTP_EXTEND_HEADER));         //有扩展头要偏移扩展头的长度
                pMsg->wr_ptr(m_pExtendHeader->length());         //有扩展头要偏移扩展头的长度
            }
            // 写Nalu头
            NALU_HEADER *pNaluHeader = (NALU_HEADER*)pMsg->wr_ptr();
            pNaluHeader->F = naluheader.F;
            pNaluHeader->NRI = naluheader.NRI;
            pNaluHeader->TYPE = H264_NALU_TYPE_FU_A;
            pMsg->wr_ptr(sizeof(NALU_HEADER));

            // 写Fu头
            FU_HEADER *pFuHeader = (FU_HEADER*)pMsg->wr_ptr();
            if (bIsFirst)
            {
                pFuHeader->S = 1;
                pFuHeader->E = 0;
                bIsFirst = false;
            }
            else if(bIsMarker)
            {
                pFuHeader->S = 0;
                pFuHeader->E = 1;
                bIsMarker = false;
            }
            else
            {
                pFuHeader->S = 0;
                pFuHeader->E = 0;
            }
            pFuHeader->R = 0;
            pFuHeader->TYPE = naluheader.TYPE;
            pMsg->wr_ptr(sizeof(FU_HEADER));

            pMsg->copy(pCopyData, (unsigned int)iCopyLen);
            m_curPacketList.push_back(pMsg);
            pCopyData += iCopyLen;
            iBuffLen -= iCopyLen;
        }
    }

    return IVS_SUCCEED;
}

void CPs2EsProcessor::ReleasePktList()
{
    ACE_Message_Block* pMb = NULL;

    ACE_GUARD(ACE_Recursive_Thread_Mutex, pObj, m_listMutex);

    // 释放当前帧内的内存
    while (!m_curPacketList.empty())
    {
        pMb = m_curPacketList.front();
        CMediaBlockBuffer::instance().freeMediaBlock(pMb);
        m_curPacketList.pop_front();
    }

    return;
}

void CPs2EsProcessor::ReleaseFrameList()
{
    ACE_Message_Block* pMb = NULL;

    // 释放当前帧内的内存
    while (!m_curFrameList.empty())
    {
        pMb = m_curFrameList.front();
        if (NULL != pMb)
        {
            pMb->release();
        }
        m_curFrameList.pop_front();
    }

    return;
}

/*****************************************************************************
 Function:    setRtpInfo
 Description: 将一个slice拆分成Rtp包
 Input:       pRtpHead RTP头
              ucFrameType 帧类型
              bIsMarker mark标志是否有效
 Output:      
 Return:      
*****************************************************************************/
void CPs2EsProcessor::setRtpInfo(char *pRtpHead, unsigned char ucFrameType, bool bIsMarker)
{
	if(NULL == m_pExtendHeader)
	{
		ERROR_LOG("m_pExtendHeader is NULL ");
		return;
	}
	CRtpPacket rtpPacket;
	(void)rtpPacket.GeneratePacket(pRtpHead, sizeof(RTP_FIXED_HEADER));
	if (FRAME_TYPE_VIDEO == ucFrameType)
	{
		m_usVideoRtpSeq += 1;
		(void)rtpPacket.SetSeqNum(m_usVideoRtpSeq);
		(void)rtpPacket.SetTimeStamp(m_ulVideoTimeTick);
		(void)rtpPacket.SetPayloadType(m_ucVideoPayloadType);
	}
	else
	{
		m_usAudioRtpSeq += 1;
		(void)rtpPacket.SetSeqNum(m_usAudioRtpSeq);
		(void)rtpPacket.SetTimeStamp(m_ulAudioTimeTick);
		(void)rtpPacket.SetPayloadType(m_ucAudioEncodeMode);
	}

    // 增加扩展头
    if (0 != m_pExtendHeader->length())
    {
        (void)rtpPacket.SetExtension(true);
        (void)rtpPacket.GenerateExtensionPacket((pRtpHead + sizeof(RTP_FIXED_HEADER)), (m_pExtendHeader->length() + sizeof(RTP_EXTEND_HEADER)));
        // 将扩展数据拷到发送缓存
        memcpy((pRtpHead + sizeof(RTP_FIXED_HEADER) + sizeof(RTP_EXTEND_HEADER)), m_pExtendHeader->rd_ptr(), m_pExtendHeader->length());
    }

    (void)rtpPacket.SetMarker(bIsMarker);
    // todo rtp包带时间
}

/*****************************************************************************
 Function:    appendExtInfo
 Description: 添加扩展信息
 Input:       
 Output:      
 Return:      
*****************************************************************************/
//lint -e1762
void CPs2EsProcessor::appendExtInfo(ACE_Message_Block *pMsg)
{
    //PS转换ES流，添加附加时间戳信息
    if (m_bAppendExtInfo && NULL != pMsg)
    {
        REAL_RECORD_TIME* real_record_time = (REAL_RECORD_TIME*)(pMsg->base());
        real_record_time->uiSecond = m_uiRealRecordSecond;//lint !e712
        real_record_time->uiMSecond = m_uiRealRecordMSecond;//lint !e712
        uint32_t* pStreamRate = (uint32_t*)(pMsg->base() + sizeof(REAL_RECORD_TIME));
		*pStreamRate = m_uiReserved;
        pMsg->wr_ptr(sizeof(REAL_RECORD_TIME) + sizeof(uint32_t));
        pMsg->rd_ptr(sizeof(REAL_RECORD_TIME) + sizeof(uint32_t));
    }
}
//lint +e1762
//lint +e818
//lint +e826

