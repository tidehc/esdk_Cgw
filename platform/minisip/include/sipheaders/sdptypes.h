/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sdptypes.h

    Author          : zhaolei (ronnie)

    Version         : 1.0

    Date            : 2005-11-16

    Description     : This file contains the declare of SDP module
                      common and self types.

    Function List   :
        1.

    History:
        1. Date:
        Tag: None
        Author:
        Modification:
*******************************************************************************/
#ifndef __SDP_TYPES_H__
#define __SDP_TYPES_H__

#include "vpp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* sdp types define */
typedef unsigned char      SDP_UINT8;

typedef signed char        SDP_INT8;
typedef char               SDP_CHAR;
typedef const char         SDP_CCHAR;
typedef unsigned char      SDP_UCHAR;

typedef unsigned short     SDP_UINT16;
typedef signed   short     SDP_INT16;

typedef signed int         SDP_INT;
typedef unsigned int       SDP_UINT;

typedef signed int         SDP_INT32;
typedef unsigned int       SDP_UINT32;

typedef SDP_UINT32         SDP_BOOL;

typedef long               SDP_LONG;
typedef unsigned long      SDP_ULONG;

typedef unsigned long      SDP_SIZET;
typedef unsigned long      SDP_ID;


/* common macro */
#undef SDP_VOID
#define SDP_VOID           void

#undef IN
#define IN

#undef OUT
#define OUT

#undef IO
#define IO

#ifndef CONST
#define CONST              const
#endif

/* Special value */

#define SDP_NULL_UINT32    0xFFFFFFFF

#define SDP_ENUM_END       0x0FFFFFFF
#define SDP_NULL_PTR       (0)

#define SDP_FALSE          0
#define SDP_TRUE           1

/* Stack error infor output function */
typedef SDP_VOID (*pFnSdpPrint)
(
    SDP_CHAR * pcFormat,
    ...
);

typedef SDP_UINT32 (*pFnSdpDecode)
(
    IO VppMsgCbHdl       hdlMsgCb,
    OUT SDP_VOID        *pvResult
);

typedef SDP_UINT32 (*pFnSdpEncode)
(
    IO VppMsgCbHdl       hdlMsgCb,
    IN SDP_VOID         *pvValue
);

#ifdef MULTI_THREAD
/*extern VPP_VOID* VppSysGlobalAlloc(VPP_UINT32 ulBytes);
extern VPP_VOID* VppSysGlobalFree(VPP_VOID* pvGlobalHdl);*/
extern VPP_VOID* VppSysGlobalLock(VPP_VOID* pvGlobalHdl);
extern VPP_BOOL  VppSysGlobalUnlock(VPP_VOID* pvMem);
#endif

extern SDP_VOID* SdpGetGlobal(SDP_VOID);
extern SDP_VOID  SdpSetGlobal(SDP_VOID *pvGlobal);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __SDP_TYPES_H__ */

