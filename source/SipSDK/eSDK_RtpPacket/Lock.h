#ifndef __IVS_PLAYER_LOCK_H__
#define __IVS_PLAYER_LOCK_H__


#ifdef _MSC_VER   // windows use 
# include <Windows.h>
typedef CRITICAL_SECTION     LOCK_T;
#  define LOCK_INIT(lock)      InitializeCriticalSection(&(lock))
#  define LOCK(lock)           EnterCriticalSection(&(lock))
#  define UNLOCK(lock);        LeaveCriticalSection(&(lock))
#  define LOCK_DESTROY(lock)   DeleteCriticalSection(&(lock))

#else  //linux

#include <pthread.h>
typedef pthread_mutex_t      LOCK_T;
#  define LOCK_INIT(lock)      pthread_mutex_init(&(lock), NULL)
#  define LOCK(lock)           pthread_mutex_lock(&(lock))
#  define UNLOCK(lock);        pthread_mutex_unlock(&(lock))
#  define LOCK_DESTROY(lock)   pthread_mutex_destroy(&(lock))
#endif // _MSC_VER


class CIVSMutex
{
public:
	CIVSMutex()  { LOCK_INIT(m_lock); }
	~CIVSMutex() { LOCK_DESTROY(m_lock);}
    void Lock()   { LOCK(m_lock); }
	void UnLock() { UNLOCK(m_lock); }
private:
	LOCK_T  m_lock;
};
//lint -e1712
class CAutoLock
{
public:
    CAutoLock(CIVSMutex& t) { m_pMutex = &t; m_pMutex->Lock(); }
	~CAutoLock() 
	{ 
		try{
			m_pMutex->UnLock(); 
		}
		catch(...)
		{}
		m_pMutex = NULL;
	}
private:
    CIVSMutex *m_pMutex;
};
//lint +e1712




#endif // __IVS_PLAYER_LOCK_H__
