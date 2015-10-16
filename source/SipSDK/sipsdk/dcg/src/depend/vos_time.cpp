#include "vos.h"

namespace SipStack{  
    
ULONG VOS_GetTicks()
{
    ULONG ticks = 0 ;
#if VOS_APP_OS == VOS_OS_LINUX
    struct timeval now;
    gettimeofday(&now, VOS_NULL);
    ticks = (ULONG)(now.tv_sec * 1000 + now.tv_usec / 1000);
#elif VOS_APP_OS == VOS_OS_VXWORKS
    ticks = (ULONG)(tickGet() * 1000 / sysClkRateGet());
#else
    ticks = GetTickCount();  //lint !e838
#endif
    return( ticks );
}

VOID VOS_Delay(ULONG ulDelayTimeMs)
{
    LONG was_error;

    struct timeval tv;
    ULONG then, now, elapsed;
    then = VOS_GetTicks();

    do 
    {
        errno = 0;
        /* Calculate the time interval left (in case of interrupt) */
        now = VOS_GetTicks();
        elapsed = (now-then);
        then = now;
        if ( elapsed >= ulDelayTimeMs )
        {
            break;
        }

        ulDelayTimeMs -= elapsed;
        tv.tv_sec = (long)(ulDelayTimeMs/1000);
        tv.tv_usec = (long)((ulDelayTimeMs%1000)*1000);

        was_error = select(0, VOS_NULL, VOS_NULL, VOS_NULL, &tv);

    } while ( was_error && (errno == EINTR) );
}

VOID VOS_Sleep( ULONG ulMs )
{
#if VOS_APP_OS == VOS_OS_WIN32
    Sleep(ulMs);
#else
    usleep(ulMs * 1000);
#endif

    return ;
}
}//end namespace

