/*******************************************************************************
    Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.

    FileName   : cbbadaptor.c

    Author     : SriniCH

    Version    : 1.0

    Date       : 2006-01-04

    Description: This file contains the cbb adaptor sample functions.

    Function List:
        1.

    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX
*******************************************************************************/

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include "MiniSip.h"
#include "vos.h"
#ifdef __cplusplus
extern "C" {
#endif


#ifdef MULTI_THREAD

/*******************************************************************************
 Function Name : VppSysGlobalAlloc

 Description   :

 Input(s)      : ulSize - No Of Bytes to allocate.

 Output(s)     : None.

 Returns       : VPP_VOID *.
*******************************************************************************/
/*VPP_VOID*        VppSysGlobalAlloc(VPP_UINT32 ulBytes)
{
    return GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, ulBytes);
}
*/
/*******************************************************************************
 Function Name : VppSysGlobalFree

 Description   :

 Input(s)      : ulSize - No Of Bytes to allocate.

 Output(s)     : None.

 Returns       : VPP_VOID *.
*******************************************************************************/
/*VPP_VOID* VppSysGlobalFree(VPP_VOID* pvGlobalHdl)
{
    return GlobalFree(pvGlobalHdl);
}
*/
/*******************************************************************************
 Function Name : VppSysGlobalLock

 Description   :

 Input(s)      : ulSize - No Of Bytes to allocate.

 Output(s)     : None.

 Returns       : VPP_VOID *.
*******************************************************************************/
VPP_VOID* VppSysGlobalLock(VPP_VOID* pvGlobalHdl)
{
    return GlobalLock(pvGlobalHdl);
}

/*******************************************************************************
 Function Name : VppSysGlobalUnlock

 Description   :

 Input(s)      : ulSize - No Of Bytes to allocate.

 Output(s)     : None.

 Returns       : VPP_VOID *.
*******************************************************************************/
VPP_BOOL  VppSysGlobalUnlock(VPP_VOID* pvMem)
{
    return GlobalUnlock(pvMem);
}

#endif

/*******************************************************************************
 Function Name : VppSysMalloc

 Description   : This function is called by stack to allocate memory.

 Input(s)      : ulSize - No Of Bytes to allocate.

 Output(s)     : None.

 Returns       : VPP_VOID *.
*******************************************************************************/
VPP_VOID * VppSysMalloc
(
    IN VPP_UINT32 ulSize
)
{
    VPP_VOID   *pAddr;

    pAddr = malloc(ulSize);
	if(NULL != pAddr)
	{
		memset(pAddr, 0, ulSize);
	}

    return pAddr;
}

/*******************************************************************************
 Function Name : VppSysRealloc

 Description   : This function is called by stack to reallocate memory.

 Input(s)      : ulNewSize - No Of Bytes to reallocate.
                 pvMem     - The pointer of former memory

 Output(s)     : pvMem     - The pointer of former memory.

 Returns       : VPP_VOID *.
*******************************************************************************/
VPP_VOID *VppSysRealloc
(
    IO VPP_VOID  *pvMem,
    IN VPP_UINT32 ulNewSize
)
{
    return realloc(pvMem, ulNewSize);
}

/*******************************************************************************
 Function Name : VppSysFree

 Description   : This function is called by stack to free memory.

 Input(s)      : pvData     - Previously allocated memory block to be freed

 Output(s)     : None.

 Returns       : VPP_VOID *.
*******************************************************************************/
VPP_VOID VppSysFree
(
    IO VPP_VOID *pvData
)
{
    free(pvData);
}

/*******************************************************************************
 Function Name : VppSysMemCpy

 Description   : This function is called by stack to copy the memory.

 Input(s)      : pvSrc     - The Buffer from where we supposed to copy
                 ulLen     - The Length of the buffer we supposed to copy

 Output(s)     : pvDest    - The output buffer to where we copied the data.

 Returns       : VPP_VOID *.
*******************************************************************************/
VPP_VOID *VppSysMemCpy
(
    OUT VPP_VOID  *pvDest,
    IN  VPP_VOID  *pvSrc,
    IN  VPP_UINT32 ulLen
)
{
    return memcpy(pvDest, pvSrc, ulLen);
}//lint !e818

/*******************************************************************************
 Function Name : VppSysMemCmp

 Description   : This function is called by stack to compare the memory.

 Input(s)      : pcStr1     - The Source Buffer which we have to compare
                 pcStr2     - The Source Buffer which we have to compare
                 ulLen      - The Length of the buffer we supposed to compare

 Output(s)     : None.

 Returns       : VPP_SINT32
*******************************************************************************/
VPP_SINT32 VppSysMemCmp
(
    IN VPP_VOID  *pcStr1,
    IN VPP_VOID  *pcStr2,
    IN VPP_UINT32 ulLen
)
{
    return (VPP_SINT32)memcmp(pcStr1, pcStr2, ulLen);
}//lint !e818

/*******************************************************************************
 Function Name : VppSysMemSet

 Description   : This function is called by stack to set the memory.

 Input(s)      : pvAddr     - The Source Buffer which we have to set
                 cByte      - The character which we have to set
                 ulLen      - The Length of the buffer we supposed to
                               intialize with char

 Output(s)     : None.

 Returns       : VPP_VOID *
*******************************************************************************/
VPP_VOID *VppSysMemSet
(
    IO VPP_VOID  *pvAddr,
    IN VPP_CHAR   cByte,
    IN VPP_UINT32 ulSize
)
{
    return memset(pvAddr, cByte, ulSize);
}

/*******************************************************************************
 Function Name : VppSysMemMove

 Description   : This function is called by stack to move the memory.

 Input(s)      : pcDest     - The Dest Buffer to which we have to mve
                 pcSrc      - The Sorce buffer from which we have to move
                 ulLen      - The Length of the buffer we supposed move

 Output(s)     : None.

 Returns       : VPP_VOID *
*******************************************************************************/
VPP_VOID *VppSysMemMove
(
    IO VPP_VOID  *pcDest,
    IN VPP_VOID  *pcSrc,
    IN VPP_UINT32 ulLen
)
{
    return memmove(pcDest, pcSrc, ulLen);
}//lint !e818

/*******************************************************************************
 Function Name : VppSysStrLen

 Description   : This function is called by stack to get string length.

 Input(s)      : pcStr     - The Null terminated string.

 Output(s)     : None.

 Returns       : VPP_UINT32
*******************************************************************************/
VPP_UINT32  VppSysStrLen
(
    IN VPP_CHAR *pcStr
)
{
    return strlen(pcStr);
}//lint !e818

/*******************************************************************************
 Function Name : VppSysStrNICmp

 Description   : This function is called by stack to compare strings by given
                length case insensitively.

 Input(s)      : pcStr1     - The source one Null terminated string.
                 pcStr2     - The source Two Null terminated string.
                 ulLen      - The Length of the string to compare case
                                insenstively.
 Output(s)     : None.

 Returns       : VPP_SINT32
*******************************************************************************/
VPP_SINT32 VppSysStrNICmp
(
    IN VPP_CHAR  *pcStr1,
    IN VPP_CHAR  *pcStr2,
    IN VPP_UINT32 ulLen
)
{
#ifdef WIN32	
    return strnicmp(pcStr1, pcStr2, ulLen);
#else
	return strncasecmp(pcStr1, pcStr2, ulLen);
#endif
}//lint !e818

/*******************************************************************************
 Function Name : VppSysStrNCmp

 Description   : This function is called by stack to compare strings by given
                length case sensitively.

 Input(s)      : pcStr1     - The source one Null terminated string.
                 pcStr2     - The source Two Null terminated string.
                 ulLen      - The Length of the string to compare case
                               senstively.
 Output(s)     : None.

 Returns       : VPP_SINT32
*******************************************************************************/
VPP_SINT32 VppSysStrNCmp
(
    IN VPP_CHAR  *pcStr1,
    IN VPP_CHAR  *pcStr2,
    IN VPP_UINT32 ulLen
)
{
    return strncmp(pcStr1, pcStr2, ulLen);
}//lint !e818

/*******************************************************************************
 Function Name : VppSysStrChr

 Description   : This function is called by stack to search a char in given string
 Input(s)      : pcStr     - The source Null terminated string.
                 iChar     - The char which we need to search.

 Output(s)     : None.

 Returns       : VPP_CHAR *
*******************************************************************************/
VPP_CHAR *VppSysStrChr
(
    IN VPP_CHAR    *pcStr,
    IN VPP_SINT32   iChar
)
{
    return strchr(pcStr,iChar);
}


/* Get os current tick function */
VPP_UINT32 VppSysGetTick()
{
    return SipStack::VOS_GetTicks();
}


#ifdef __cplusplus
}
#endif

