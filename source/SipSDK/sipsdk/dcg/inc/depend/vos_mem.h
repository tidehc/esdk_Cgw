#ifndef IVSCBB_SS_VOS_MEM_H
#define IVSCBB_SS_VOS_MEM_H

#include <stdlib.h>

namespace SipStack
{
	VOID *VOS_malloc( LONG size );
	VOID VOS_free( VOID *buff );
}//end namespace
#endif /* __VOS_MEM_H__ */
