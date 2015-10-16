
/******************************************************************************
   版权所有 (C), 2008-2011, 华为技术有限公司

 ******************************************************************************
  文件名          : CLockGuard.cpp
  版本号          : 1.0
  作者            : 
  生成日期        : 2008-8-17
  最近修改        : 
  功能描述        : 实现智能锁功能
  函数列表        : 
  修改历史        : 
  1 日期          : 
    作者          : 
    修改内容      : 
*******************************************************************************/


#include "vos.h"
#include "CLockGuard.h"
namespace SipStack{  
CLockGuard::CLockGuard(VOS_Mutex *pMutex)
{
    m_pMutex = NULL;

    if(NULL == pMutex)
    {
        return;
    }

    m_pMutex = pMutex;

    (void)VOS_MutexLock(m_pMutex);
}

CLockGuard::~CLockGuard()
{
    if(NULL == m_pMutex)
    {
        return;
    }
    (void)VOS_MutexUnlock(m_pMutex);
    
    m_pMutex = NULL;
}

void CLockGuard::lock(VOS_Mutex *pMutex)
{
    if(NULL == pMutex)
    {
        return;
    }
    (void)VOS_MutexLock(pMutex);
}

void CLockGuard::unlock(VOS_Mutex *pMutex)
{
    if(NULL == pMutex)
    {
        return;
    }
    (void)VOS_MutexUnlock(pMutex);
}
}//end namespace

