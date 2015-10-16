#include "stdafx.h"
#include "MediaBlockBuffer.h"
#include "Common_Def.h"

//lint -e438
int CBlockCheckTimer::handle_timeout(const ACE_Time_Value &tv, const void *arg)
{
    UNUSED_PARA(tv);
	ACE_Message_Block* pMb = NULL;
    int nRet = 0;

    if (NULL == arg)
    {
        return 0;
    }
	/*lint -save -e1773*/
    CMediaBlockBuffer* pMediaBlockBuffer = (CMediaBlockBuffer*)(arg);
	/*lint -restore*/
    unsigned long ulRellocNum = pMediaBlockBuffer->getMediaBlockFailNum();
    unsigned long ulFreeFailNum = ulRellocNum;
    unsigned int unSize = pMediaBlockBuffer->getBufferSize();

    CMediaDataQueue* pMediaQueue = pMediaBlockBuffer->getMediaQueue();
    if (NULL == pMediaQueue)
    {
        return 0;
    }


    if (0 != ulRellocNum && 0 != unSize)
    {
        unsigned int iAddNumber = 0;
        for (unsigned long i = 0; i < ulRellocNum; i++)
        {
            pMb = pMediaBlockBuffer->createMediaBlock(unSize);
            if (NULL == pMb)
            {
                break;
            }

            ACE_Time_Value timeValue(0, MEDIA_MAX_QUEUE_DELAY);//lint !e747
            nRet = pMediaQueue->enqueue_tail(pMb, &timeValue);
            if (NRU_SUCCESS != nRet)
            {
                delete pMb;
                pMb = NULL;

                break;
            }

            iAddNumber++;
        }

        if (ulFreeFailNum >= iAddNumber)
        {
            ulFreeFailNum = ulFreeFailNum - iAddNumber;
            pMediaBlockBuffer->setMediaBlockFailNum(ulFreeFailNum);
        }
        else
        {
            pMediaBlockBuffer->setMediaBlockFailNum(0);
        }
        
    }    
    return 0;
}

CMediaBlockBuffer* CMediaBlockBuffer::g_MediaBuffer = NULL;
CMediaBlockBuffer::CMediaBlockBuffer()
{
    m_unSize    = 0;
    m_unCount   = 0;
    m_iLastFreeNum = 0;
    m_FailureTimes = 0;
    m_unAllocBuffFailTimes = 0;
    m_pMediaQueue  = NULL;
	m_lQueueTimerId = -1;
	m_ulQueueFreeFailNum = 0;
    m_pBlockCheckTimer = NULL;
}

CMediaBlockBuffer::~CMediaBlockBuffer()
{
    try
    {
        close();
    }
    catch(...)
    {

    }
    m_pMediaQueue = NULL;
	m_lQueueTimerId = -1;
	m_ulQueueFreeFailNum = 0;
    m_pBlockCheckTimer = NULL;
}

int CMediaBlockBuffer::init(unsigned int unBlockSize, unsigned int unBlockCount)
{
    ACE_Message_Block* pMb = NULL;

    try
    {
        m_pMediaQueue = new CMediaDataQueue;
    }
    catch(...)
    {
        m_pMediaQueue = NULL; 
    }
    if (NULL == m_pMediaQueue)
    {
        
        return NRU_FAIL;
    }
    try
    {
        m_pBlockCheckTimer = new CBlockCheckTimer;
    }
    catch(...)
    {
        m_pBlockCheckTimer = NULL;
    }
    if (NULL == m_pBlockCheckTimer)
    {
        if (NULL != m_pMediaQueue)
        {
            delete m_pMediaQueue;
			m_pMediaQueue = NULL;
        }    
        
        return NRU_FAIL;
    }

    int nRet = m_pMediaQueue->init(unBlockCount);
    if (NRU_SUCCESS != nRet)
    {
       
        return NRU_FAIL;
    }

    for (unsigned int i = 0; i < unBlockCount; i++)
    {
        pMb = createMediaBlock(unBlockSize);
        if (NULL == pMb)
        {
            
            return NRU_FAIL;
        }

        // add by g00195859:添加去队列等待时间
        ACE_Time_Value tv(0, MEDIA_MAX_QUEUE_DELAY);//lint !e747
        nRet = m_pMediaQueue->enqueue_tail(pMb, &tv);
        if (NRU_SUCCESS != nRet)
        {
             
            return NRU_FAIL;
        }
    }
    
    m_unSize = unBlockSize;
    m_unCount = unBlockCount;
    m_iLastFreeNum = unBlockCount;//lint !e713
    return NRU_SUCCESS;
}

void CMediaBlockBuffer::close()
{ 
    if (NULL != m_pMediaQueue)
    {
        ACE_Message_Block* pMb = NULL;

        // add by g00195859
        // 出队列的超时时间定为100ms，如果时间太短，在大压力下不够线程切换的时间
        ACE_Time_Value tv(0, MEDIA_MAX_QUEUE_DELAY); //lint !e747
        while(!m_pMediaQueue->empty())
        {
            (void)m_pMediaQueue->dequeue_head(pMb, &tv);
            if (NULL != pMb)
            {           
                delete pMb;
                pMb = NULL;
            }
        }

        m_pMediaQueue->close();

        delete m_pMediaQueue;
        m_pMediaQueue = NULL;
    }

    
    return;
}

ACE_Message_Block *CMediaBlockBuffer::allocMediaBlock()
{  
    if (NULL == m_pMediaQueue)
    {
		 
        return NULL;
    }

    if (m_pMediaQueue->empty())
    {
        if ((0 == m_unAllocBuffFailTimes)
            ||(0 == (m_unAllocBuffFailTimes % 10000)))
        {
             
            m_unAllocBuffFailTimes = 0;
        }
        m_FailureTimes++;
        ++m_unAllocBuffFailTimes;
        return NULL;
    }

    ACE_Message_Block *pMb = NULL;
    // 出队列的超时时间定为100ms，如果时间太短，在大压力下不够线程切换的时间
    ACE_Time_Value tv(0, 100 * 1000);  //lint !e747   
    int lRet = m_pMediaQueue->dequeue_head(pMb, &tv);
    if (NRU_SUCCESS != lRet)
    {
        m_FailureTimes++;
         
        return NULL;
    }

    // 当空闲数目变化超过OUTPUT_FREE_NUM_DLT时，打印下日志
    unsigned int unMsgCount = this->m_pMediaQueue->message_count();
    int iTmp = this->m_iLastFreeNum - unMsgCount;//lint !e713 !e737
    if (iTmp >= MEDIA_BLOCK_FREE_NUM_DLT || iTmp <= -MEDIA_BLOCK_FREE_NUM_DLT)
    {
        
        this->m_iLastFreeNum = unMsgCount;//lint !e713
    }

    return pMb;
}

void CMediaBlockBuffer::freeMediaBlock(ACE_Message_Block *&pMb)
{
    if (NULL == pMb)
    {
         
        return;
    }

    if (pMb->reference_count() > 1)
    {
        pMb->release();
        return;
    }

    // 没有人在使用该消息块了
    pMb->reset();

    if (NULL != m_pMediaQueue)
    {
        // add by g00195859:添加入队列等待时间
        ACE_Time_Value tv(0, MEDIA_MAX_QUEUE_DELAY);   //lint !e747
        int lRet = m_pMediaQueue->enqueue_tail(pMb, &tv);
        if (NRU_SUCCESS != lRet)
        {
            pMb->release();
            pMb = NULL;    
            m_ulQueueFreeFailNum++;
            return;
        }
    }
    else
    {
        // 队列为空，或者入队列失败，需要释放内存 
        pMb->release();
        pMb = NULL; 
        m_ulQueueFreeFailNum++;
        return;
    }

    // 当空闲数目变化超过OUTPUT_FREE_NUM_DLT时，打印下日志
    unsigned int unMsgCount = this->m_pMediaQueue->message_count();//lint !e613
    int iTmp = this->m_iLastFreeNum - unMsgCount;//lint !e713 !e737
    if (iTmp >= MEDIA_BLOCK_FREE_NUM_DLT || iTmp <= -MEDIA_BLOCK_FREE_NUM_DLT)
    {
         
        this->m_iLastFreeNum = unMsgCount;//lint !e713
    }

    return;
}

// 获取系统总的缓冲块个数
unsigned int CMediaBlockBuffer::getMaxBlockNum() const
{
    return m_unCount;
}

// 获取剩余缓冲块个数
unsigned int CMediaBlockBuffer::getRemainBlockNum() const
{
    if (NULL == m_pMediaQueue)
    {
        return 0;
    }
    else
    {
        return m_pMediaQueue->message_count();
    }
}

// 获取内存分配失败的次数
unsigned int CMediaBlockBuffer::getFailureTimes()const
{
    return m_FailureTimes;
}

unsigned int CMediaBlockBuffer::getBlockSize()const
{
    return m_unSize;
}

/*lint -e429*/
ACE_Message_Block* CMediaBlockBuffer::createMediaBlock(unsigned int unBlockSize)const
{
    ACE_Message_Block* pMb = NULL;
    try
    {
        pMb = new ACE_Message_Block(unBlockSize);
    }
    catch(...)
    {
        pMb = NULL;        
    }
    if(NULL == pMb)//lint !e831
    {        
       
        return NULL;
    }

	if(NULL == pMb->data_block())
	{
		delete pMb;
		 
		return NULL;
	}

    if (NULL == pMb->base())
    {
         
        delete pMb;
        return NULL;
    }

    return pMb;
}
/*lint +e429*/
//lint +e438

