

#include "vos.h"
#include "CSynchronized.h"


#ifdef WIN32
long CSynchronized::start()
{
    semEvent = CreateEvent(0, FALSE, FALSE, 0);
    if( VOS_NULL == semEvent )
    {
        return VOS_ERR_SYS;
    }
    semMutex = CreateMutex(0, FALSE, 0);
    if( VOS_NULL == semMutex )
    {
        (void)CloseHandle(semEvent);
        return VOS_ERR_SYS;
    }    

    semPushEvent = CreateEvent(0, FALSE, FALSE, 0);
    if( VOS_NULL == semPushEvent )
    {
        (void)CloseHandle(semEvent);
        (void)CloseHandle(semMutex);
        return VOS_ERR_SYS;
    }    
    
    semPushMutex = CreateMutex(0, FALSE, 0);
    if( VOS_NULL == semPushMutex )
    {
        (void)CloseHandle(semEvent);
        (void)CloseHandle(semMutex);
        (void)CloseHandle(semPushEvent);
        return VOS_ERR_SYS;
    }

    return VOS_OK ;
}
#else
long CSynchronized::start()
{
    long result = VOS_OK ;
    
    memset(&monitor, 0, sizeof(monitor));
    result = pthread_mutex_init(&monitor, 0);
    if( VOS_OK != result )
    {
        return result;
    }

    memset(&push_monitor, 0, sizeof(push_monitor));
    result = pthread_mutex_init(&push_monitor, 0);
    if( VOS_OK != result )
    {
        pthread_mutex_destroy(&monitor);
        return result;
    }

    memset(&pop_cond, 0, sizeof(pop_cond));
    result = pthread_cond_init(&pop_cond, 0);
    if( VOS_OK != result )
    {
        pthread_mutex_destroy(&monitor);
        pthread_mutex_destroy(&push_monitor);
        return result;
    }
    
    memset(&push_cond, 0, sizeof(push_cond));
    result = pthread_cond_init(&push_cond, 0);
    if( VOS_OK != result )
    {
        pthread_cond_destroy(&pop_cond);
        pthread_mutex_destroy(&monitor);
        pthread_mutex_destroy(&push_monitor);
        return result;
    }
    
    return result ;
}
#endif

CSynchronized::CSynchronized()
{
#ifdef WIN32
    numNotifies = 0;
    semEvent = NULL;
    semMutex = NULL;
    semPushEvent = NULL;
    semPushMutex = NULL;
#else
#endif
}

CSynchronized::~CSynchronized()
{
#ifdef WIN32

    (void)CloseHandle(semEvent);
    semEvent = NULL;
    (void)CloseHandle(semMutex);
    semMutex = NULL;

    (void)CloseHandle(semPushEvent);
    semPushEvent = NULL;
    (void)CloseHandle(semPushMutex);
    semPushMutex = NULL;

#else
    pthread_cond_destroy(&push_cond);
    pthread_cond_destroy(&pop_cond);
    pthread_mutex_destroy(&monitor);
    pthread_mutex_destroy(&push_monitor);
#endif
}

long CSynchronized::popWait( long timeout )
{
    long result;
#ifdef WIN32
    result = wait(semEvent,semMutex,timeout);
#else
    result = wait(&pop_cond,&monitor,timeout);
#endif
    return result;
}

long CSynchronized::pushWait( long timeout )
{
    long result;
#ifdef WIN32
    result = wait(semPushEvent,semMutex,timeout);
#else
    result = wait(&push_cond,&monitor,timeout);
#endif
    return result;
}

#ifndef WIN32
long CSynchronized::cond_timed_wait( pthread_cond_t *cond,pthread_mutex_t *monitor,struct timespec *ts) 
{
    long result;

    if(ts) 
    {
        result = pthread_cond_timedwait(cond, monitor, ts);
    }
    else 
    {
        result = pthread_cond_wait(cond, monitor);
    }

    return result;
}
#endif


#ifdef WIN32
long  CSynchronized::wait(HANDLE hSemEvent,HANDLE hSemMutex,long timeout)const
{
    unsigned long err;
    long result = VOS_OK ;

    if(!ReleaseMutex(hSemMutex))
    {
        return VOS_ERR_SYS;
    }

    if( timeout == 0 )
    {
        timeout = (long)INFINITE ;
    }

    err = WaitForSingleObject(hSemEvent, (unsigned long)timeout);
    switch(err)
    {
        case WAIT_TIMEOUT:
            result = VOS_ERR_QUE_TIMEOUT;
            break;
        case WAIT_ABANDONED://lint !e835
            result = VOS_ERR_SYS;
            break;
        case WAIT_OBJECT_0://lint !e835
            result = VOS_OK;
            break;
        default:
            result = VOS_ERR_SYS;
            break;
    }

    if(WaitForSingleObject (hSemMutex, INFINITE) != WAIT_OBJECT_0)//lint !e835
    {
        return VOS_ERR_SYS;
    }

    return result ;
}
#else
long  CSynchronized::wait(pthread_cond_t *cond,pthread_mutex_t *monitor,long timeout)
{
    struct timespec ts;
    struct timeval  tv;

    long result= VOS_OK ;

    gettimeofday(&tv, 0);
    ts.tv_sec  = tv.tv_sec  + (long)timeout/1000;
    ts.tv_nsec = (tv.tv_usec + (timeout %1000)*1000) * 1000;

    long err;
    if( timeout )
    {
        err = cond_timed_wait(cond,monitor,&ts);
    }
    else
    {
        err = cond_timed_wait(cond,monitor,VOS_NULL);
    }
    if( err  > VOS_OK )
    {
        switch(err)
        {
            case ETIMEDOUT:
                 result = VOS_ERR_QUE_TIMEOUT;
            break;

            default:
                 result = VOS_ERR_SYS;
            break;
        }
    }

    return result;
}
#endif

#ifdef WIN32
long  CSynchronized::notifyRead()
{
    numNotifies = 1;
    if(!SetEvent(semEvent))
    {
        return VOS_ERR_SYS;
    }

    return VOS_OK ;
}
#else
long  CSynchronized::notifyRead()
{
    long result;

    result = pthread_cond_signal(&pop_cond);
    if(result)
    {
        return result; 
    }

    return VOS_OK ;
}
#endif

#ifdef WIN32
long  CSynchronized::notifyWrite()
{
    numNotifies = 1;
    if(!SetEvent(semPushEvent))
    {
        return VOS_ERR_SYS;
    }

    return VOS_OK ;
}
#else
long  CSynchronized::notifyWrite()
{
    long result;

    result = pthread_cond_signal(&push_cond);
    if(result)
    {
        return result; 
    }

    return VOS_OK ;
}
#endif


#ifdef WIN32
long CSynchronized::notify_all()
{
    numNotifies = (char)0x7F;
    while (numNotifies--)
    {
        if(!SetEvent(semEvent))
        {
            return VOS_ERR_SYS;
        }
    }

    return VOS_OK;
}
#else
long CSynchronized::notify_all()
{
    long result;

    result = pthread_cond_broadcast(&pop_cond);
    if(result)
    {
        return result ;
    }

    return VOS_OK;
}
#endif

#ifdef WIN32
long CSynchronized::lock()
{
    if(WaitForSingleObject(semMutex, INFINITE) != WAIT_OBJECT_0)//lint !e835
    {
        return VOS_ERR;
    }

    return VOS_OK;
}    
#else
long CSynchronized::lock()
{
    if(pthread_mutex_lock(&monitor))
    {
        return VOS_ERR;
    }

    return VOS_OK ;
}
#endif



#ifdef WIN32
long CSynchronized::unlock()/*lint !e1714*/ //供外部调用
{
    if(!ReleaseMutex(semMutex))
    {
        return VOS_ERR ;
    }

    return VOS_OK ;
}
#else
long CSynchronized::unlock()
{
    if(pthread_mutex_unlock(&monitor))
    {
        return VOS_ERR ;
    }

    return VOS_OK ;
}
#endif

#ifndef WIN32
bool CSynchronized::trylock()
{
    long result = VOS_OK;

    result = pthread_mutex_trylock(&monitor);
    if( VOS_OK == result )
    {
        return VOS_TRUE ;
    }
    else
    {
        return VOS_FALSE ;
    }
}
#endif




