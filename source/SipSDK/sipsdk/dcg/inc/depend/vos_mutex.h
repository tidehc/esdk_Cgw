#ifndef IVSCBB_SS_VOS_MUTEX_H
#define IVSCBB_SS_VOS_MUTEX_H

namespace SipStack
{
	typedef struct tagVOSMutex
	{
	#if VOS_APP_OS == VOS_OS_WIN32
	  HANDLE  mutex;
	#else
	  pthread_mutex_t  mutex;
	#endif
	}VOS_Mutex;


	VOS_Mutex *VOS_CreateMutex( );
	ULONG VOS_DestroyMutex( VOS_Mutex *pstMutex );
	ULONG VOS_MutexLock( VOS_Mutex *pstMutex );
	ULONG VOS_MutexUnlock( VOS_Mutex *pstMutex );
}//end namespace

#endif /* __VOS_MUTEX_H__ */
