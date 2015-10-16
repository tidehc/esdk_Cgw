/********************************************************************
 filename    :    RtpFrameOrganizer.cpp
 author      :    shilei 00194833
 created     :    2013-02-21
 description :    实现一个PS流转换成ES流的处理器
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013-02-21 初始版本
*********************************************************************/
#include "stdafx.h"
// 错误码头文件

#include "ivs_error.h"
#include "RtpFrameOrganizer.h"
#include "MediaBlockBuffer.h"
#include "RtpPacket.h"
#include "Common_Def.h"
#include "Log.h"
#include "eSDKTool.h"
#include "eSDKMem.h"

#define PRINT_MIN_VALUE 1500
#define INVALID_RTP_SEQ     (0x80000000)     // 定义无效的RTP序列号

void CPacketChangeProcessor::setAudioEncodeMode(unsigned char ucAudioEncodeMode)
{
    switch(ucAudioEncodeMode)
    {
    case NSS_AUDIO_G711A:
        {
            m_ucAudioEncodeMode = IVS_PT_G711A;
            break;
        }

    case NSS_AUDIO_G711U:
        {
            m_ucAudioEncodeMode = IVS_PT_G711U;
            break;
        }

    case NSS_AUDIO_G726:
        {
            m_ucAudioEncodeMode = IVS_PT_G726;
            break;
        }

    case NSS_AUDIO_AAC:
        {
            m_ucAudioEncodeMode = IVS_PT_AAC;
            break;
        }

    default:
        m_ucAudioEncodeMode = IVS_PT_G711U;
    }
}

CRtpFrameOrganizer::CRtpFrameOrganizer()
{
    m_unMaxCacheFrameNum    = 0;
    m_pRtpFrameHandler      = NULL;

    m_unCurFrameNum         = 0;
    m_unLastDequeueSeq      = INVALID_RTP_SEQ;
    m_bSeqReversal          = false;
    m_unSeqReversalPos      = 0;
    m_strServiceID          = "";
    m_iFrameNumber          = 0;
    m_bIsLostPacket         = false;

    m_bRunFlag              = false;
    m_bThreadExit           = true;

    m_pSendThreadSemaphore  = NULL;
    m_iRtpQueueLogThreshold = NRU_ZERO;
}

CRtpFrameOrganizer::~CRtpFrameOrganizer()
{
    m_bRunFlag = false;
	m_pRtpFrameHandler = NULL;
	m_pSendThreadSemaphore = NULL;
    try
    {
        release();
    }
    catch(...){}
}

void CRtpFrameOrganizer::release()
{
    while (!m_RtpQueue.empty())
    {
        if(NULL != m_RtpQueue[0].pRtpMsgBlock)
        {
            m_RtpQueue[0].pRtpMsgBlock->release();
        }
        //CMediaBlockBuffer::instance().freeMediaBlock(m_RtpQueue[0].pRtpMsgBlock);
        m_RtpQueue.pop_front();
    }

    while (!m_RtpAudioQueue.empty())
    {
        if(NULL != m_RtpAudioQueue[0].pRtpMsgBlock)
        {
            m_RtpAudioQueue[0].pRtpMsgBlock->release();
        }
        //CMediaBlockBuffer::instance().freeMediaBlock(m_RtpAudioQueue[0].pRtpMsgBlock);
        m_RtpAudioQueue.pop_front();
    }

    ACE_Time_Value tmDelay(0,100*1000);//lint !e747
    while(!m_bThreadExit)
    {
 
        ACE_OS::sleep(tmDelay);
 
    }

    HW_DELETE(m_pSendThreadSemaphore);

    //BP_RUN_LOG_INF("success to handle finish frame and release", "sessionID=%s, framenum=%d",        m_strServiceID.c_str(), m_iFrameNumber);
    m_iFrameNumber = 0;
    m_iRtpQueueLogThreshold = NRU_ZERO;
    return;
}

int CRtpFrameOrganizer::init(IRtpFrameHandler* pHandler,
                             const std::string& strServiceID,
                             unsigned int unMaxFrameCache)
{
    //CHECK_POINTER(pHandler, IVS_PARA_INVALID);
    if (0 == unMaxFrameCache)
    {        
        ERROR_LOG("Frame cache size invalid ,size:%d.",unMaxFrameCache);
        return IVS_SUCCEED;
    }

    m_pRtpFrameHandler  = pHandler;
    m_unMaxCacheFrameNum = unMaxFrameCache;
    m_strServiceID = strServiceID;

    int nRet = startThread();
    if(IVS_SUCCEED != nRet)
    {
        ERROR_LOG("Fail to start thread.");
        return IVS_FAIL;
    }
    m_iRtpQueueLogThreshold = NRU_ZERO;
    //BP_RUN_LOG_INF("Init rtp frame organizer success", "organizer=%p, sessionID=%s.", this, strServiceID.c_str());
    return IVS_SUCCEED;
}

/*****************************************************************************
 Function:    insertRtpPacket
 Description: 插入一包数据
 Input:       pRtpBlock 一包媒体数据
 Output:      
 Return:      IVS_SUCCEED 成功
              其他 失败
*****************************************************************************/
int CRtpFrameOrganizer::insertRtpPacket(const ACE_Message_Block* pRtpBlock)
{
    //CHECK_POINTER(pRtpBlock, IVS_PARA_INVALID);  //lint !e831

    CRtpPacket rtpPacket;
    if (IVS_SUCCEED != rtpPacket.ParsePacket(pRtpBlock->rd_ptr(), pRtpBlock->length()))
    {
        ERROR_LOG("Insert rtp packet fail , parse rtp packet fail");
        return IVS_FAIL;
    }

    // 浅拷贝一份媒体数据
    // 内部指针可以自由移动
    ACE_Message_Block* pRtpMb = pRtpBlock->clone();
    //CHECK_POINTER(pRtpMb, IVS_ALLOC_MEMORY_ERROR);  //lint !e831 !e774

    RTP_PACK_INFO_S  rtpInfo;
    rtpInfo.bMarker = rtpPacket.GetMarker();
    rtpInfo.usSeq   = rtpPacket.GetSeqNum();
    rtpInfo.unTimestamp = rtpPacket.GetTimeStamp();
    rtpInfo.pRtpMsgBlock = pRtpMb;

    // 对于音频数据直接放入音频对列
    if ((IVS_PT_G711U == rtpPacket.GetPayloadType())
        ||(IVS_PT_G711A == rtpPacket.GetPayloadType())
		||(ELTE_PT_AMR==rtpPacket.GetPayloadType()))
    {
        m_RtpAudioQueue.push_back(rtpInfo);
        return IVS_SUCCEED;
    }

    if (IVS_SUCCEED != insert(rtpInfo))
    {
        ERROR_LOG("Insert rtp packet fail , seq: %d,timestamp:%d,marker:%d",rtpInfo.usSeq,rtpInfo.unTimestamp,rtpInfo.bMarker);
        if(NULL != pRtpMb)
        {
             pRtpMb->release();
        }
        return IVS_FAIL;
    }

    // 根据RTP Marker计算帧数
    if (rtpInfo.bMarker)
    {
        m_unCurFrameNum++;
    }

    // 检查是否有接收完整的帧
    if (1 < m_unCurFrameNum)
    {
        checkFrame();
    }

    return IVS_SUCCEED;
}

/*****************************************************************************
 Function:    insert
 Description: 插入一包数据
 Input:       info 一包媒体数据相关信息
 Output:      
 Return:      IVS_SUCCEED 成功
              其他 失败
*****************************************************************************/
int CRtpFrameOrganizer::insert(const RTP_PACK_INFO_S &info)
{
    if (INVALID_RTP_SEQ != m_unLastDequeueSeq)
    {
        // 检查新来的RTP包是否有效，比最后出去的序列号小，且序列号未翻转，则认为是过期RTP包
        if ((info.usSeq < m_unLastDequeueSeq)
            && (m_unLastDequeueSeq - (unsigned int)info.usSeq < MAX_RTP_SEQ / 2))
        {
            //BP_RUN_LOG_INF("Insert overdue rtp packet fail", "seq=%u, last seq=%u, queue size=%u, sessionID=%s.", info.usSeq, m_unLastDequeueSeq, m_RtpQueue.size(), m_strServiceID.c_str());
            return IVS_FAIL;
        }
    }

    // 队列为空，直接插入
    if (m_RtpQueue.empty())
    {
        m_RtpQueue.push_back(info);
        return IVS_SUCCEED;
    }

    //若媒体流异常，则抛弃队列首数据 C02代码
    //if (MAX_RTP_QUEUE_NUM < m_RtpQueue.size())
    //{
    //    if (NRU_ZERO == (m_iRtpQueueLogThreshold % LOG_COUNT_THRESHOLD_1000))
    //    {
    //        //BP_RUN_LOG_ERR(IVS_FAIL, "Current Rtp Queue is too big","will discard part data, serviceID:%s RtpQueue.size:%d Queue[0].unTimestamp:%d Queue[0].bMarker:%d Queue[0].usSeq:%d.", m_strServiceID.c_str(), m_RtpQueue.size(), m_RtpQueue[0].unTimestamp, m_RtpQueue[0].bMarker, m_RtpQueue[0].usSeq);
    //        ERROR_LOG("Current Rtp Queue is too big,will discard part data, serviceID:" << m_strServiceID << ",RtpQueue.size:"<< m_RtpQueue.size() << ",unTimestamp:" << m_RtpQueue[0].unTimestamp << ".";
    //        m_iRtpQueueLogThreshold = NRU_ZERO;
    //    }
    //    m_iRtpQueueLogThreshold ++;

    //    m_RtpQueue[0].pRtpMsgBlock->release();
    //    if (m_RtpQueue[0].bMarker && 0 < m_unCurFrameNum)
    //    {
    //        m_unCurFrameNum--;
    //    }
    //    m_RtpQueue.pop_front();
    //}

	// 媒体流异常：
	// 1、所有的RTP报文时间戳一样且无mark标志位；
	// 2、PS流中RTP报文的序列号有重复；
	// 3、PS流中RTP报文的时间戳无规律跳变；
	// 4、PS流中RTP报文的序列号非法翻转会导致队列缓存的RTP报文越来越多，内存消耗越来越大，当RTP报文数超过MAX_FRAME_CACHE_NUM*1024，便释放内存
	if (MAX_RTP_QUEUE_NUM <= m_RtpQueue.size())
	{
		ERROR_LOG("Current Rtp Queue is too big,will discard part data, serviceID:%s,RtpQueue.size:%d,unTimestamp:%d",m_strServiceID.c_str(),m_RtpQueue.size(),m_RtpQueue[0].unTimestamp);
		// 重置RTP包序号
		m_unLastDequeueSeq = INVALID_RTP_SEQ;

		// 释放已完成的帧
		releaseRtpPacket(m_RtpQueue.size() - 1);
		m_bIsLostPacket = false;
	}

	// 队列为空，直接插入
	if (m_RtpQueue.empty())
	{
		m_RtpQueue.push_back(info);
		return IVS_SUCCEED;
	}

	if (info.unTimestamp != m_RtpQueue[m_RtpQueue.size() - 1].unTimestamp)
	{
		// 与上一帧的时间戳不一致时，认为收到另一帧，主要处理丢到marker标志位的包
		if (!m_RtpQueue[m_RtpQueue.size() - 1].bMarker)
		{
			m_RtpQueue[m_RtpQueue.size() - 1].bMarker = true;
			m_unCurFrameNum++;
		}
		checkFrame();
	}

	if (m_RtpQueue.empty())
	{
		m_RtpQueue.push_back(info);
		return IVS_SUCCEED;
	}

    unsigned short usFirstSeq = m_RtpQueue[0].usSeq;
    unsigned short usLastSeq = m_RtpQueue[m_RtpQueue.size() - 1].usSeq;

    if (!m_bSeqReversal)
    {
        // 队列内未翻转，那么最后一个元素最大
        if (info.usSeq >= usLastSeq)
        {
            if (info.usSeq - usFirstSeq > MAX_RTP_SEQ / 2)
            {
                // 在缓冲队列的第一个包前发生翻转
                m_RtpQueue.push_front(info);

                m_bSeqReversal = true;
                m_unSeqReversalPos = 1;         // 本来在0处发生翻转，但又在前面插入了一个翻转前的数
                //BP_RUN_LOG_INF("Seq reversal", "Seqreverse=%d, pos=%d, size=%d.",                                  info.usSeq, m_unSeqReversalPos, m_RtpQueue.size());
            }
            else
            {
                m_RtpQueue.push_back(info);
            }
        }
        else
        {
            if (usLastSeq - info.usSeq > MAX_RTP_SEQ / 2)
            {
                // 这是有翻转了
                m_RtpQueue.push_back(info);

                m_bSeqReversal = true;
                m_unSeqReversalPos = m_RtpQueue.size() - 1;
                //BP_RUN_LOG_INF("Seq reversal", "Seqreverse=%d, pos=%d, size=%d.",                     info.usSeq, m_unSeqReversalPos, m_RtpQueue.size());
            }
            else
            {
                // 未翻转，是乱序
                return insertRange(info, 0, m_RtpQueue.size() - 1);
            }
        }

        return IVS_SUCCEED;
    }

    // 队列有翻转
    if ((0 == m_unSeqReversalPos)
        || (m_unSeqReversalPos >= m_RtpQueue.size()))
    {
        //BP_RUN_LOG_INF("Insert rtp packet fail", "rtp packet=%u : %u, reversal pos=%u invalid, sessionID=%s , size=%d.",                         info.usSeq, info.unTimestamp, m_unSeqReversalPos, m_strServiceID.c_str(), m_RtpQueue.size());
        return IVS_FAIL;
    }

    // 判断新的Seq是翻转前还是翻转后
    unsigned short usOverflowSeq = m_RtpQueue[m_unSeqReversalPos - 1].usSeq;
    if ((usFirstSeq <= info.usSeq && usOverflowSeq >= info.usSeq)
        || (info.usSeq >= usOverflowSeq))
    {
        if (IVS_SUCCEED != insertRange(info, 0, m_unSeqReversalPos - 1))
        {
            return IVS_FAIL;
        }

        m_unSeqReversalPos++; // 翻转前加了一个数，所以翻转位置要向后移
        return IVS_SUCCEED;
    }

    return insertRange(info, m_unSeqReversalPos, m_RtpQueue.size() - 1);
}

/*****************************************************************************
 Function:    insert
 Description: 插入一包数据
 Input:       info 一包媒体数据相关信息
              unStartPos  翻转起始位置
              unStopPos   翻转结束位置
 Output:      
 Return:      IVS_SUCCEED 成功
              其他 失败
*****************************************************************************/
int CRtpFrameOrganizer::insertRange(const RTP_PACK_INFO_S &info,
                                    unsigned int unStartPos,
                                    unsigned int unStopPos)
{
    if (unStartPos >= m_RtpQueue.size()
            || unStopPos >= m_RtpQueue.size()
            || unStopPos < unStartPos)
    {
        //BP_RUN_LOG_INF("Insert rtp packet fail", "rtp packet=%u : %u, insert range=%u - %u invalid, sessionID=%s.",                        info.usSeq, info.unTimestamp, unStartPos, unStopPos, m_strServiceID.c_str());
        return IVS_FAIL;
    }

    if (info.usSeq >= m_RtpQueue[unStopPos].usSeq)
    {
        m_RtpQueue.insert(m_RtpQueue.begin() + (int)unStopPos + 1, info);
        return IVS_SUCCEED;
    }

    unsigned int i = unStartPos;
    while (i <= unStopPos)
    {
        if (info.usSeq <= m_RtpQueue[i].usSeq)
        {
            m_RtpQueue.insert(m_RtpQueue.begin() + (int)i, info);
            return IVS_SUCCEED;
        }

        i++;
    }

    //BP_RUN_LOG_INF("Insert rtp packet fail", "rtp packet=%u : %u, can't find insert pos in range=%u - %u, sessionID=%s.", info.usSeq, info.unTimestamp, unStartPos, unStopPos, m_strServiceID.c_str());
    return IVS_FAIL;
}

/*****************************************************************************
 Function:    checkFrame
 Description: 检测一帧数据是否收完
 Input:      
 Output:      
 Return:      N/A
*****************************************************************************/
void CRtpFrameOrganizer::checkFrame()
{
    if (m_RtpQueue.empty())
    {
        return;
    }

    // 从前向后，找到第一个接收完成的帧
    unsigned short usSeq = (INVALID_RTP_SEQ == m_unLastDequeueSeq) ? m_RtpQueue[0].usSeq : ((unsigned short)(m_unLastDequeueSeq + 1));
    unsigned int uiCurrTimeStamp = m_RtpQueue[0].unTimestamp;
	/*lint -save -e850*/
    for (unsigned int unEndPos = 0; unEndPos < m_RtpQueue.size(); unEndPos++)
    {
        if (NRU_ZERO != unEndPos)
        {
            // 丢包检查
            if (MAX_RTP_SEQ == usSeq)
            {
                // RTP序列号翻转，需判断是从0开始，还是1开始
                // 存在判断误差，如实际序号从0开始，但未收到序号为0的RTP，而收到的序号为1的RTP，亦不会认为丢包
                usSeq = (NRU_ZERO == m_RtpQueue[unEndPos].usSeq) ? NRU_ZERO : NRU_ONE;
            }
            else
            {
                usSeq++;
            }
        }

        if (usSeq != m_RtpQueue[unEndPos].usSeq)
        {
            // 有丢包，系统缓存3帧
            if (m_unCurFrameNum > MAX_RTP_FRAME_CACHE_NUM)
            {
                //BP_RUN_LOG_ERR(IVS_FAIL, "rtp frame organizer check rtp packet fail  2  ", "lost=%u - %u, unEndPos=%d m_unCurFrameNum=%d sessionID=%s.",usSeq, m_RtpQueue[unEndPos].usSeq, unEndPos, m_unCurFrameNum, m_strServiceID.c_str());
                ERROR_LOG("rtp frame organizer check rtp packet fail lost :%d  - %d,unEndPos :%d,m_unCurFrameNum:%d",usSeq,m_RtpQueue[unEndPos].usSeq,unEndPos,m_unCurFrameNum);
                usSeq = m_RtpQueue[unEndPos].usSeq;
                //if (uiCurrTimeStamp != m_RtpQueue[unEndPos].unTimestamp && unEndPos > 0)
                //{
                //    unEndPos -= 1; // unEndPos=0时不会走到这里
                //    m_RtpQueue[unEndPos].bMarker = true;
                //}
                m_bIsLostPacket = true;
            }
            else
            {
                return;
            }
        }

        // 找到结束帧
        if (m_RtpQueue[unEndPos].bMarker)
        {
            handleFinishedFrame(unEndPos);

            // 更新上一个结束帧ID
            m_unLastDequeueSeq = (unsigned int)m_RtpQueue[unEndPos].usSeq;

            if (PRINT_MIN_VALUE == m_iFrameNumber)
            {
                //BP_DBG_LOG("success to handle finish frame, sessionID=%s, framenum=%d",                     m_strServiceID.c_str(), m_iFrameNumber);
                m_iFrameNumber = 0;
            }
            else
            {
                ++m_iFrameNumber;
            }
            // 释放已完成的帧
            releaseRtpPacket(unEndPos);
            m_bIsLostPacket = false;
            return;
        }
    }
	/*lint -restore*/

    return;
}

/*****************************************************************************
 Function:    handleFinishedFrame
 Description: 处理完整的一帧数据
 Input:       unEndPos结束的位置
 Output:      
 Return:      N/A
*****************************************************************************/
void CRtpFrameOrganizer::handleFinishedFrame(unsigned int unEndPos)
{
    if (m_RtpQueue.empty())
    {
        return;
    }

    if (NULL == m_pRtpFrameHandler)
    {
        return;
    }

    RTP_FRAME_LIST_T   frameList;
    for (unsigned int i = 0; i <= unEndPos; i++)
    {
        if (NULL != m_RtpQueue[i].pRtpMsgBlock)
        {
            frameList.push_back(m_RtpQueue[i].pRtpMsgBlock);
        }
    }

    m_pRtpFrameHandler->handleMediaFrame(frameList);

    //音频帧永远跟在一帧视频帧后面
    RTP_FRAME_LIST_T   audioframeList;
    unsigned int unAudioSize = m_RtpAudioQueue.size();
    if (0 != unAudioSize)
    {
        for (unsigned int i = 0; i < unAudioSize; i++)
        {
            if (NULL != m_RtpAudioQueue[i].pRtpMsgBlock)
            {
                audioframeList.push_back(m_RtpAudioQueue[i].pRtpMsgBlock);
            }
        }

        m_pRtpFrameHandler->handleMediaFrame(audioframeList);
    }

    return;
}

void CRtpFrameOrganizer::releaseRtpPacket(unsigned int unEndPos)
{
    if (m_RtpQueue.empty())
    {
        return;
    }

    if (unEndPos >= m_RtpQueue.size())
    {
        return;
    }

    //  释放内存时要释放存的帧个数
    for (unsigned int i = 0; i <=  unEndPos; i++)
    {
        //ediaBlockBuffer::instance().freeMediaBlock(m_RtpQueue[0].pRtpMsgBlock);
        //m_RtpQueue[0].pRtpMsgBlock->release();
		if(NULL != m_RtpQueue[0].pRtpMsgBlock)
		{
			m_RtpQueue[0].pRtpMsgBlock->release();
			m_RtpQueue[0].pRtpMsgBlock = NULL;
		}
		
		if (m_RtpQueue[0].bMarker && 0 < m_unCurFrameNum)
        {
            m_unCurFrameNum--;
        }

        m_RtpQueue.pop_front();
    }

    unsigned int unAudioQueueSize = m_RtpAudioQueue.size();
    for (unsigned int i = 0; i < unAudioQueueSize; i++)
    {
        //ediaBlockBuffer::instance().freeMediaBlock(m_RtpAudioQueue[0].pRtpMsgBlock);
        m_RtpAudioQueue[0].pRtpMsgBlock->release();
        m_RtpAudioQueue.pop_front();
    }

    // 更新序列号翻转的位置
    if (m_bSeqReversal)
    {
        if (m_unSeqReversalPos > unEndPos)
        {
            m_unSeqReversalPos -= (unEndPos + 1);
            if (0 == m_unSeqReversalPos)
            {
                m_bSeqReversal = false;
            }
        }
        else
        {
            m_bSeqReversal = false;
            m_unSeqReversalPos = 0;
        }
    }

    return;
}

bool CRtpFrameOrganizer::checkLostPacket()const
{
    return m_bIsLostPacket;
}

void CRtpFrameOrganizer::SetSendMediaFunFlag(const bool bRunFlag)
{
    m_bRunFlag = bRunFlag;
    return;
}

void CRtpFrameOrganizer::acquireSendThreadSemaphore()
{
    if (NULL != this->m_pSendThreadSemaphore)
    {
        this->m_pSendThreadSemaphore->acquire();
    }
}

int CRtpFrameOrganizer::startThread()
{
    m_bRunFlag = true;
    //启动发送数据的线程	
	/*lint -save -e774*/
    HW_NEW(m_pSendThreadSemaphore, ACE_Thread_Semaphore(0));
	/*lint -restore*/
    if(NULL == m_pSendThreadSemaphore)
    {
        //BP_RUN_LOG_ERR(IVS_FAIL, "Failure to create ps-es send thread semaphore", " errno: ");
        return IVS_FAIL;
    }
    return IVS_SUCCEED;
}


