/******************************************************************************
   版权所有 (C), 2008-2011, 华为技术有限公司

 ******************************************************************************
  文件名          : CLockGuard.h
  版本号          : 1.0
  作者            : 
  生成日期        : 2008-8-17
  最近修改        : 
  功能描述        : 实现智能锁功能
  函数列表        : 
  修改历史        : 
    日期          : 
    作者          : 
    修改内容      : 
*******************************************************************************/

#ifndef IVSCBB_SS_LOCKGUARD
#define IVSCBB_SS_LOCKGUARD

#include "vos.h"

namespace SipStack{
class CLockGuard
{
  public:
    CLockGuard(VOS_Mutex *pMutex);
    ~CLockGuard();
    
  public:
    static void lock(VOS_Mutex *pMutex);
    static void unlock(VOS_Mutex *pMutex);

 private:
    VOS_Mutex *m_pMutex;
};

}//end namespace
#endif /* CLockGuard_H_INCLUDE  */


