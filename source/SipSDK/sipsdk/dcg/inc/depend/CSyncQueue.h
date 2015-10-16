#ifndef IVSCBB_SS_SYNC_QUEUE_H
#define IVSCBB_SS_SYNC_QUEUE_H

#include <queue>
#include "CSynchronized.h"
using namespace std;
extern void CONN_WRITE_LOG(long lLevel, const char *format, ...);

template <class Type>
class CSyncQueue:public CSynchronized
{
public:
    CSyncQueue( );
    ~CSyncQueue();
    long init(long maxQueueLen);
    bool empty(void);
    long size(void);
    long pushBackEv(Type *ev,long timeOut = 0,long mode = QUEUE_MODE_NOWAIT );
    long popFrontEv(Type *&ev,long timeout = 0,long mode = QUEUE_MODE_WAIT );
private:
    queue<Type *> p_Queue;
    long m_maxQueueLen;
};

template <class Type> 
CSyncQueue<Type>::CSyncQueue()
{
	m_maxQueueLen = 0;
}

template <class Type>
CSyncQueue<Type>::~CSyncQueue()
{
}

template <class Type>
long CSyncQueue<Type>::init(long maxQueueLen)
{
    long result = VOS_OK;

    m_maxQueueLen = maxQueueLen;
    result = start();

    return result ;
}

template <class Type>
bool CSyncQueue<Type>::empty(void)
{
    bool em;

    lock();
    em = p_Queue.empty();
    unlock();

    return em ;
}

template <class Type>
long CSyncQueue<Type>::size(void)
{
    long sz;

    lock();
    sz = p_Queue.size();
    unlock();

    return sz ;
}

template <class Type>
long CSyncQueue<Type>::popFrontEv(Type *&ev,long timeout , long mode )
{
    long result = VOS_OK ;

    lock();
    while( VOS_TRUE )
    { 
        result = p_Queue.empty();
        if( VOS_FALSE == result )
        {
            break ;
        }

        if( QUEUE_MODE_NOWAIT == mode )
        {
            unlock();
            return VOS_ERR_QUE_EMPTY ;
        }

        result = popWait(timeout);
        if( VOS_OK != result )
        {
            unlock();
            return result ;
        }
    }

    ev = p_Queue.front();
    p_Queue.pop();
    unlock();

    (void)notifyWrite();

    return result ;
}

template <class Type>
long  CSyncQueue<Type>::pushBackEv(Type *ev, long timeout,long mode)
{
    long result = VOS_OK;
    long queulen = 0 ;

    lock();
    queulen = p_Queue.size();
    if( queulen >= m_maxQueueLen )
    {
        if( QUEUE_MODE_NOWAIT == mode )
        {
            unlock();
            return VOS_ERR_QUE_LEN;
        }

        result = pushWait(timeout);
        if( VOS_OK != result )
        {
            unlock();
            return VOS_ERR_SYS;
        }
    }

    p_Queue.push(ev);
    unlock();

   (void)notifyRead();

    return result ;
}

#endif

