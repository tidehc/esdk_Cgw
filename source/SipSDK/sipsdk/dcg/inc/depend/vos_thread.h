#ifndef IVSCBB_SS_VOS_THREAD_H
#define IVSCBB_SS_VOS_THREAD_H

namespace SipStack{

typedef struct tagVOSThread
{
#if VOS_APP_OS == VOS_OS_WIN32
    ULONG ptheadID;
    HANDLE pthead; 
#else
    pthread_attr_t attr;
    pthread_t pthead;
#endif

}VOS_Thread;

#if VOS_APP_OS == VOS_OS_WIN32

typedef  ULONG(__stdcall * VOS_THREAD_FUNC)(VOID *)  ;

#else

typedef  VOID* ( * VOS_THREAD_FUNC)(VOID *)  ;

#endif

ULONG VOS_CreateThread( VOS_THREAD_FUNC pfnThread, VOID *args, 
          VOS_Thread **pstVosThread,ULONG ulStackSize);

VOID  VOS_pthread_exit(void *retval);

LONG  VOS_ThreadJoin(const VOS_Thread *pstVosThread );

VOID  VOS_DeleteThread( VOS_Thread *pstVosThread );

#if VOS_APP_OS == VOS_OS_WIN32
HANDLE  VOS_pthread_self();
#else
pthread_t  VOS_pthread_self();
#endif

}//end namespace

#endif /* __VOS_THREAD_H__ */
