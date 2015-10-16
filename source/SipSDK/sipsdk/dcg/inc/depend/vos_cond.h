#ifndef IVSCBB_SS_VOS_COND_H
#define IVSCBB_SS_VOS_COND_H

namespace SipStack{
typedef struct
{
#if VOS_APP_OS == VOS_OS_WIN32
    HANDLE  event;
#else
    pthread_cond_t cond;
    pthread_mutex_t  mutex;
#endif
}VOS_COND;


    LONG VOS_CondInit(VOS_COND* pCond);
    LONG VOS_CondWait(VOS_COND* pCond);
    LONG VOS_CondTimeWait(VOS_COND* pCond,LONG lSeconds);
    LONG VOS_CondSignal(VOS_COND* pCond);
    LONG VOS_CondDestroy(VOS_COND* pCond);

}//end namespace

#endif /* __VOS_COND_H__ */
