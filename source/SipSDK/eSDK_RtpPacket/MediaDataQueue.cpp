#include "stdafx.h"
#include "MediaDataQueue.h"
#include "Common_Def.h"

/**
* Description: 构造函数
* @param 无
* @return 无
*/
CMediaDataQueue::CMediaDataQueue()
{
#ifndef WIN32
    atomic_set(m_ActiveFlag, 1); // 运行标志为0时，所有操作都为返回失败
    m_unArraySize    = 0;
    m_unMaxQueueSize = 0;

    atomic_set(m_QueueSize, 0);
    m_pDataArray  = NULL;

    atomic_set(m_WriteIndex, 0);
    atomic_set(m_WriteTag, 0);
    atomic_set(m_ReadIndex, 0);
    atomic_set(m_ReadTag, 0);
#endif
}

CMediaDataQueue::~CMediaDataQueue()
{
#ifndef WIN32
    if (NULL != m_pDataArray)
    {
        delete[] m_pDataArray;
        m_pDataArray = NULL;
    }
#endif
}

/**
* Description: 初始化队列
* @param [in] unQueueSize: 队列最大缓冲消息块数
* @return NRU_SUCCESS - 成功，Others - 失败
*/
int CMediaDataQueue::init(unsigned int unQueueSize)
{
#ifndef WIN32
    atomic_set(m_ActiveFlag, 1); // 运行标志为0时，所有操作都为返回失败
    m_unMaxQueueSize = unQueueSize;
    m_unArraySize    = unQueueSize + 1;

    atomic_set(m_QueueSize, 0);
    m_pDataArray  = NULL;

    atomic_set(m_WriteIndex, 0);
    atomic_set(m_WriteTag, 0);
    atomic_set(m_ReadIndex, 0);
    atomic_set(m_ReadTag, 0);

    m_unMaxQueueSize = unQueueSize;
    m_unArraySize    = unQueueSize + 1;

    try
    {
        // 必须留一个空位
        m_pDataArray = new ACE_Message_Block*[m_unArraySize];
    }
    catch (...)
    {
        return NRU_FAIL;
    }

    memset(m_pDataArray, 0x0, sizeof(ACE_Message_Block*) * m_unArraySize);
    return NRU_SUCCESS;
#else
    return m_DataQueue.open(unQueueSize * MAX_DATA_BLOCK_SIZE);
#endif
}

/**
* Description: 关闭队列，会导致出入队列的操作立即返回失败
* @param 无
* @return 无
*/
void CMediaDataQueue::close()
{
#ifndef WIN32
    atomic_set(m_ActiveFlag, 0); // 运行标志为0时，所有操作都为返回失败
#else
    (void)m_DataQueue.close();
#endif
}

/**
* Description: 获取当前队列中数据块数
* @param 无
* @return 返回队列数据块个数
*/
#ifndef WIN32
unsigned int CMediaDataQueue::message_count() const
{
    return (unsigned int) atomic_read(m_QueueSize);
}

#else
unsigned int CMediaDataQueue::message_count()
{
    return m_DataQueue.message_count();
}

#endif
/**
* Description: 检查队列是否为空
* @param 无
* @return true - 队列空  false - 队列满
*/
#ifndef WIN32

bool CMediaDataQueue::empty() const
{
    return (atomic_read(m_QueueSize) == 0);
}
#else

bool CMediaDataQueue::empty()
{
    return m_DataQueue.is_empty();
}
#endif

/**
* Description: 检查队列是否已满
* @param 无
* @return true - 队列满  false - 队列不满
*/
#ifndef WIN32

bool CMediaDataQueue::full() const
{
    return ((unsigned int) atomic_read(m_QueueSize) >= m_unMaxQueueSize);
}
#else

bool CMediaDataQueue::full()
{
    return m_DataQueue.is_full();
}

#endif
/**
* Description: 向队列尾部添加数据块
* @param [in] mb : 要添加的数据块
* @param [in] timeout : 超时时间，要求必须为相对时间，如果为NULL则操作可能发生阻塞
* @return NRU_SUCCESS - 成功，Others - 失败
*/
int CMediaDataQueue::enqueue_tail(ACE_Message_Block* mb, const ACE_Time_Value *timeout)
{
    if (NULL == mb)
    {
        return NRU_FAIL;
    }
#ifndef WIN32
    if ((NULL == m_pDataArray) || (0 == m_unArraySize))
    {
        return NRU_FAIL;
    }
    bool bFlag               = false;
    bool bTimeOut            = false;
    unsigned int unHeadIndex = 0;

    ACE_Time_Value tvTimeOut;
    ACE_Time_Value startTime;
    if (NULL != timeout)
    {
        startTime = ACE_OS::gettimeofday();
        bTimeOut  = true;
        tvTimeOut = *timeout;
    }

    bool bHaveTry = false;      // 是否进行过尝试放消息

    // 先尝试把要加的数据位占用住
    while (0 != atomic_read(m_ActiveFlag))
    {
        if (bTimeOut && bHaveTry && (ACE_OS::gettimeofday() - startTime > tvTimeOut))
        {
            return NRU_FAIL;
        }

        bHaveTry = true;

        unHeadIndex = (unsigned int)atomic_read(m_WriteIndex);
        unsigned int unTag       = (unsigned int)atomic_read(m_WriteTag);
        if ((unHeadIndex + 1) % m_unArraySize == (unsigned int)atomic_read(m_ReadIndex))
        {
            // 队列满，等待1ms
            //ACE_Time_Value timeout(0, 0);
            //ACE_OS::sleep(timeout);
            (void)usleep(0);

            continue;
        }

        if (NULL != m_pDataArray[unHeadIndex])
        {
            // 出错重试
            continue;
        }

        if (compare_and_swap2(&m_WriteIndex, unHeadIndex, unTag, (unHeadIndex + 1) % m_unArraySize, unTag + 1))
        {
            bFlag = true;
            break;
        }

    }

    if (bFlag)
    {
        // 直接赋值
        m_pDataArray[unHeadIndex] = mb;
        atomic_inc(&m_QueueSize);

        return NRU_SUCCESS;
    }

    return NRU_FAIL;
#else
    ACE_Time_Value enTime;

    if (timeout)
    {
        enTime = ACE_OS::gettimeofday();
        enTime += *timeout;
        ACE_OS::last_error(0);
        if (-1 == m_DataQueue.enqueue_tail(mb, &enTime))
        {
            int errno;
            ACE_OS::last_error(errno);
            //IVS_NEW_DBG_LOG ("enqueue fail, now count=%d, errno=%d.", m_DataQueue.message_count(),errno);
            return NRU_FAIL;
        }
    }
    else if (-1 == m_DataQueue.enqueue_tail(mb))
    {
        //IVS_NEW_DBG_LOG ("enqueue fail, now count=%d", m_DataQueue.message_count());
        return NRU_FAIL;
    }

    return NRU_SUCCESS;
#endif
}

/**
* Description: 从队列头部取出数据块
* @param [out] mb : 数据块指针
* @param [in] timeout : 超时时间，要求必须为相对时间，如果为NULL则操作可能发生阻塞
* @return NRU_SUCCESS - 成功，Others - 失败
*/
int CMediaDataQueue::dequeue_head(ACE_Message_Block* &mb, const ACE_Time_Value *timeout)
{
#ifndef WIN32
    if ((NULL == m_pDataArray) || (0 == m_unArraySize))
    {
        return NRU_FAIL;
    }

    bool bFlag               = false;
    bool bTimeOut            = false;
    unsigned int unTailIndex = 0;
    ACE_Time_Value tvTimeOut;
    ACE_Time_Value startTime;
    
    if (NULL != timeout)
    {
        startTime = ACE_OS::gettimeofday();
        bTimeOut  = true;
        tvTimeOut = *timeout;
    }

    bool bHaveTry = false;      // 是否进行过尝试取消息

    while (0 != atomic_read(m_ActiveFlag))
    {
        if (bTimeOut && bHaveTry && (ACE_OS::gettimeofday() - startTime > tvTimeOut))
        {
            return NRU_FAIL;
        }

        bHaveTry = true;

        unTailIndex = (unsigned int)atomic_read(m_ReadIndex);
        unsigned int unTag = (unsigned int)atomic_read(m_ReadTag);

        if (unTailIndex == (unsigned int)atomic_read(m_WriteIndex))
        {
            // 队列为空，等待1ms
            ACE_Time_Value timeout(0, 1000);
            ACE_OS::sleep(timeout);
            //(void)usleep(0);

            continue;
        }


        if (NULL == m_pDataArray[unTailIndex])
        {
            // 队列不为空但要取的值为NULL，出现错误，需要重试
            continue;
        }

        if (compare_and_swap2(&m_ReadIndex, unTailIndex, unTag, (unTailIndex + 1) % m_unArraySize, unTag + 1))
        {
            bFlag = true;
            break;
        }

    }

    if (bFlag)
    {
        mb = m_pDataArray[unTailIndex];
        m_pDataArray[unTailIndex] = 0;
        atomic_dec(&m_QueueSize);

        return NRU_SUCCESS;
    }

    return NRU_FAIL;

#else
    ACE_Time_Value enTime;

    if (timeout)
    {
        enTime = ACE_OS::gettimeofday();
        enTime += *timeout;
        if (-1 == m_DataQueue.dequeue_head(mb, &enTime))
        {
            return NRU_FAIL;
        }
    } 
    else if(-1 == m_DataQueue.dequeue_head(mb))
    {
        return NRU_FAIL;
    }

    return NRU_SUCCESS;
#endif
}

