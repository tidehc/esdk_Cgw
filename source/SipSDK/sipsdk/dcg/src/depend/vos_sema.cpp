#include "vos.h"

//lint -e438
namespace SipStack{  
#if VOS_APP_OS != VOS_OS_VXWORKS

ULONG  VOS_CreateSemaphore( ULONG ulInitValue ,VOS_Sem **pstVosSem)
{
    VOS_Sem *pstVosSemTemp = VOS_NULL ;
    LONG lResult = VOS_OK ; 

    pstVosSemTemp = (VOS_Sem *) VOS_malloc(sizeof(VOS_Sem));//lint !e838
    if ( VOS_NULL == pstVosSemTemp )
    {
        return VOS_ERR_MEM ;
    }

#if VOS_APP_OS == VOS_OS_LINUX
    lResult = sem_init(&pstVosSemTemp->sem, 0, ulInitValue );
    if ( VOS_OK != lResult ) 
    {
        VOS_free(pstVosSemTemp);
        return VOS_ERR_SYS ;
    }
#elif VOS_APP_OS == VOS_OS_WIN32
    pstVosSemTemp->sem = CreateSemaphore(NULL,(LONG)ulInitValue,(LONG)ulInitValue,NULL);
    if (NULL == pstVosSemTemp->sem)
    {
        VOS_free(pstVosSemTemp);
        return VOS_ERR_SYS ;
    }
#endif
    *pstVosSem = pstVosSemTemp;

    return VOS_OK ;
}//lint !e529

ULONG VOS_DestroySemaphore( VOS_Sem *pstVosSem )
{
    if ( VOS_NULL == pstVosSem ) 
    {
        return VOS_ERR_PARAM ;
    }

#if VOS_APP_OS == VOS_OS_LINUX
    sem_destroy( &pstVosSem->sem );
#elif VOS_APP_OS == VOS_OS_WIN32
    CloseHandle(pstVosSem->sem);
#endif

    VOS_free( pstVosSem );    

    return VOS_OK ;
}

ULONG VOS_SemPost(VOS_Sem *pstVosSem)
{
    ULONG ulResult = VOS_OK ;

    if( VOS_NULL == pstVosSem )
    {
        return VOS_ERR_PARAM ;
    }

#if VOS_APP_OS == VOS_OS_LINUX
    ulResult = ( ULONG )sem_post(&pstVosSem->sem );
    if( VOS_OK != ulResult )
    {
        return VOS_ERR_SYS ;
    }
#elif VOS_APP_OS == VOS_OS_WIN32
    if (TRUE != ReleaseSemaphore(pstVosSem->sem,1,NULL))
    {
        return VOS_ERR_SYS;
    }
#endif

    return ulResult ;
}//lint !e818


ULONG VOS_SemWait( VOS_Sem *pstVosSem )
{
    ULONG ulResult = VOS_OK ;

    if ( VOS_NULL == pstVosSem ) 
    {
        return VOS_ERR_PARAM ;
    }

#if VOS_APP_OS == VOS_OS_LINUX
    ulResult = (ULONG)sem_wait( &pstVosSem->sem );
    if( VOS_OK != ulResult )
    {
        return VOS_ERR_SYS ;
    }
#elif VOS_APP_OS == VOS_OS_WIN32
    ulResult = WaitForSingleObject(pstVosSem->sem,INFINITE);//lint !e838
    if (WAIT_OBJECT_0 != ulResult)//lint !e835
    {
        return VOS_ERR_SYS ;
    }
#endif

    return ulResult ;
}//lint !e818

ULONG VOS_SemTryWait( VOS_Sem *pstVosSem )
{
    ULONG ulResult = VOS_OK ;

    if ( VOS_NULL == pstVosSem ) 
    {
        return VOS_ERR_PARAM ;
    }

#if VOS_APP_OS == VOS_OS_LINUX
    ulResult = ( ULONG )sem_trywait( &pstVosSem->sem );
    if( VOS_OK != ulResult )
    {
        return VOS_ERR_SEMA_TIMEOUT ;
    }
#elif VOS_APP_OS == VOS_OS_WIN32
    ulResult = WaitForSingleObject(pstVosSem->sem,0);//lint !e838
    if (WAIT_OBJECT_0 != ulResult)//lint !e835
    {
        return VOS_ERR_SEMA_TIMEOUT ;
    }
#endif

    return ulResult ;
}//lint !e818


ULONG VOS_SemWaitTimeout(VOS_Sem *pstVosSem, ULONG ulTimeout)
{
    ULONG ulResult = VOS_OK ;

    if ( VOS_NULL == pstVosSem ) 
    {
        return VOS_ERR_PARAM ;
    }
#if VOS_APP_OS == VOS_OS_LINUX
    if ( VOS_MUTEX_MAXWAIT == ulTimeout ) 
    {
        return VOS_SemWait( pstVosSem );
    }

    ulTimeout += SipStack::VOS_GetTicks();
    do 
    {
        ulResult = VOS_SemTryWait( pstVosSem );
        if ( VOS_OK == ulResult ) 
        {
            break;
        }
        
        ulResult = VOS_ERR_SEMA_TIMEOUT ;

        SipStack::VOS_Delay(1);
    } while ( SipStack::VOS_GetTicks() < ulTimeout );
#elif VOS_APP_OS == VOS_OS_WIN32
    ulResult = WaitForSingleObject(pstVosSem->sem,ulTimeout);//lint !e838
    switch(ulResult)
    {
    case WAIT_OBJECT_0://lint !e835
        return VOS_OK;
    case WAIT_TIMEOUT:
        return VOS_ERR_SEMA_TIMEOUT;
    default:
        break;
    }
#endif

    return ulResult ;
}//lint !e818

#if VOS_APP_OS == VOS_OS_LINUX
ULONG VOS_GetSemValue( VOS_Sem *pstVosSem )
{
    int lValue = 0 ;

    if( VOS_NULL == pstVosSem )
    {
        return VOS_ERR_PARAM ;
    }

    sem_getvalue(&pstVosSem->sem, &lValue);
    if ( lValue < 0 ) 
    {
        lValue = 0;
    }

    return (ULONG)lValue;
}//lint !e818
#elif VOS_APP_OS == VOS_OS_WIN32
ULONG VOS_GetSemValue( VOS_Sem *pstVosSem )
{
    return 0;
}//lint !e818
#endif

#endif /* #if (VOS_APP_OS != VOS_OS_VXWORKS)  */

}//end namespace

//lint +e438


