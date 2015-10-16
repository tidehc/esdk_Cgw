#include "vos.h"

//lint -e438
namespace SipStack
{  
	VOID *VOS_malloc( LONG size )
	{
		CHAR *buf = VOS_NULL ;

		buf = ( CHAR *)malloc( (ULONG)size );//lint !e838
		if( NULL == buf )
		{
			return VOS_NULL ;
		}
		memset( buf , 0x00 , (ULONG)size );

		return buf ;
	}

	VOID VOS_free( VOID *buff )
	{
		if (NULL != buff)
		{
			free( buff );
			buff = VOS_NULL ;
		}

		return ;
	}
}//end namespace

//lint +e438

