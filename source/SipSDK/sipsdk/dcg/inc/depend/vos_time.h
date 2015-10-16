#ifndef IVSCBB_SS_VOS_TIME_H
#define IVSCBB_SS_VOS_TIME_H

namespace SipStack{  

extern  VOID  VOS_Sleep( ULONG ulMs );

extern  ULONG  VOS_GetTicks();

extern  VOID  VOS_Delay (ULONG ulDelayTimeMs);

}//end namespace

#endif /* __VOS_TIME_H__ */
