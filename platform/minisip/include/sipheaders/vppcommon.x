/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : vppcommon.x

    Author          : zhaolei (ronnie), Li Minghua

    Version         : 1.0

    Date            : 2005-11-12

    Description     : The file defines the basic system dependent functions for CBB.

    Function List   :
        1.VppSysMalloc
        2.VppSysRealloc
        3.VppSysFree
        4.VppSysMemCpy
        5.VppSysMemCmp
        6.VppSysMemSet
        7.VppSysMemMove
        8.VppSysStrLen
        9.VppSysStrChar


    History:
        1. Date        : <YYYY-MM-DD>
           Tag         : None
           Author      :
           Modification:
******************************************************************************/
#ifndef _VPP_COMMON_X_
#define _VPP_COMMON_X_

#include "vpptypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************

                            Start Of File

******************************************************************************/

/* Allocates memory blocks. */
VPP_VOID *VppSysMalloc
(
    VPP_UINT32 ulSize  /* Bytes to allocate  */
);

/* Reallocate memory blocks. */
VPP_VOID *VppSysRealloc
(
    VPP_VOID  *pvMem,   /* The pointer of former memory */
    VPP_UINT32 ulNewSize/* Bytes to new allocate  */
);

/* Deallocates or frees a memory block. */
VPP_VOID VppSysFree
(
    VPP_VOID *pvData    /* Previously allocated memory block to be freed */
);

/* Copies characters between buffers. */
VPP_VOID *VppSysMemCpy
(
    VPP_VOID  *pvDest,  /* New buffer */
    VPP_VOID  *pvSrc,   /* Buffer to copy from  */
    VPP_UINT32 ulLen    /* Number of characters to copy */
);

/* Compare characters in two buffers. */
VPP_SINT32 VppSysMemCmp
(
    VPP_VOID  *pcStr1,  /* First buffer  */
    VPP_VOID  *pcStr2,  /* Second buffer  */
    VPP_UINT32 ulLen    /* Number of characters */
);

/* Sets buffers to a specified character. */
VPP_VOID *VppSysMemSet
(
    VPP_VOID  *pvAddr,  /* Pointer to destination */
    VPP_CHAR   cByte,   /* Character to set  */
    VPP_UINT32 ulSize   /* Number of characters */
);

/* Moves one buffer to another.*/
VPP_VOID *VppSysMemMove
(
    VPP_VOID  *pcDest,  /* Destination object  */
    VPP_VOID  *pcSrc,   /* Source object  */
    VPP_UINT32 ulLen    /* Number of bytes of characters to copy */
);

/* Get the length of a string function. */
VPP_UINT32  VppSysStrLen
(
    VPP_CHAR *pcStr     /* Null-terminated string */
);

/* Find a character in a string. */
VPP_CHAR *VppSysStrChr
(
    VPP_CHAR    *pcStr, /* Null-terminated source string */
    VPP_SINT32   iChar  /* Character to be located */
);

/* Copy a string. */
VPP_CHAR *VppSysStrCpy
(
    VPP_CHAR  *pcDst,   /* Destination string  */
    VPP_CHAR  *pcSrc    /* Null-terminated source string */
);

/* Compare characters of two strings without regard to case. */
VPP_SINT32 VppSysStrNICmp
(
    VPP_CHAR  *pcStr1,  /* Null-terminated strings to compare */
    VPP_CHAR  *pcStr2,  /* Null-terminated strings to compare */
    VPP_UINT32 ulLen    /* Number of characters to compare  */
);

/* Compare characters of two strings with regard to case. */
VPP_SINT32 VppSysStrNCmp
(
    VPP_CHAR  *pcStr1,  /* Null-terminated strings to compare */
    VPP_CHAR  *pcStr2,  /* Null-terminated strings to compare */
    VPP_UINT32 ulLen    /* Number of characters to compare  */
);

/* Get os current tick function */
VPP_UINT32 VppSysGetTick(VPP_VOID);

/* Get a random value */
VPP_UINT32 VppSysGetRandom(VPP_VOID);

/******************************************************************************

                              End of file

******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _VPP_COMMON_X_ */

