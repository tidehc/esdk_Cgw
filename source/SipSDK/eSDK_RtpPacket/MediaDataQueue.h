#ifndef __CMEDIADATAQUEUE_H__
#define __CMEDIADATAQUEUE_H__
#include "stdafx.h"
#include "ace_header.h"

#ifndef WIN32
#include "Atomic.h"
#else
#define MAX_DATA_BLOCK_SIZE    (2 * 1024)
#endif

/**
* @class   CMediaDataQueue
* @brief   媒体数据队列
* Description: 采用无锁设计的媒体数据队列，满足多线程操作
*/
class CMediaDataQueue
{
public:
    CMediaDataQueue();

    virtual ~CMediaDataQueue();

    /// 初始化队列，传入队列能够保存的消息块最大个数
    int init(unsigned int unQueueSize);

    /// 关闭队列，所有阻塞操作会立即返回失败
    void close();

#ifndef WIN32
    /// 获取队列中消息块个数
    unsigned int message_count() const;

    /// 检查队列是否为空
    bool empty() const;

    /// 检查队列是否已满
    bool full() const;
#else
    /// 获取队列中消息块个数
    unsigned int message_count();

    /// 检查队列是否为空
    bool empty();

    /// 检查队列是否已满
    bool full();
#endif

    /// 向队列尾部添加数据块
    int enqueue_tail(ACE_Message_Block* mb, const ACE_Time_Value *timeout = NULL);

    /// 从队列头取出一个数据块
    int dequeue_head(ACE_Message_Block*& mb, const ACE_Time_Value *timeout = NULL);
private:
#ifdef WIN32
    ACE_Message_Queue<ACE_SYNCH>    m_DataQueue;
#else
    // 下面四个定义顺序不能被修改或插入其它定义，CAS2明确需要如下定义
    volatile unsigned int  m_WriteIndex;           /// 始终指向下一个要放入位置的索引
    volatile unsigned int  m_WriteTag;
    volatile unsigned int  m_ReadIndex;            /// 指向要读取的数据的位置
    volatile unsigned int  m_ReadTag;

    volatile int           m_ActiveFlag;           /// 队列激活标志，Close后队列去激活
    unsigned int           m_unMaxQueueSize;       /// 队列最大缓冲数据块数
    unsigned int           m_unArraySize;          /// 缓冲区实际长度: m_unMaxQueueSize + 1

    volatile unsigned int  m_QueueSize;            /// 当前数据块数
    ACE_Message_Block**    m_pDataArray;           /// 数据队列
#endif
};

#endif // __CMEDIADATAQUEUE_H__
