/*******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : ssosfuncwrapper.h

    Author          : SriniCH

    Version         : 1.0

    Date            : 2005-12-01

    Description     : This file contains OS Wrapper interfaces to be used
                      internally by all modules of the SIP Stack which
                      are dependent on System Services

    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX
*******************************************************************************/

#ifndef __SS_OSFUNCWRAPPER_H__
#define __SS_OSFUNCWRAPPER_H__

#include "sssipbase.h"
#include "vppcommon.x"

#ifdef __cplusplus
extern "C" {
#endif

/* Define some macro for SIP module use it conveniency */

#define SIP_MALLOC(ulSize)   VppSysMalloc((VPP_UINT32)(ulSize))

#define SIP_FREE(pvAddr)     VppSysFree((VPP_VOID*)(pvAddr))

#define SIP_MEMSET(pvAddr, cCharSet, ulSize) \
    (SS_VOID)VppSysMemSet((VPP_VOID*)(pvAddr), (VPP_CHAR)(cCharSet), (VPP_UINT32)(ulSize))

#define SIP_MEMCPY(pvDest, pvSrc, ulSize) \
    (SS_VOID)VppSysMemCpy((VPP_VOID *)(pvDest), (VPP_VOID *)(pvSrc), (VPP_UINT32)(ulSize))

#define SIP_MEMCMP(pvDest, pvSrc, ulSize) \
    VppSysMemCmp((VPP_VOID *)(pvDest), (VPP_VOID *)(pvSrc), (VPP_UINT32)(ulSize))


/* Assertions are for the debug version and they will be invoked only if the
   assert is enabled with the SS_DBG macro */
#ifdef SS_DBG

#ifdef WIN32

SS_VOID exit(SS_INT32 istatus);

/* If some unexpected error occured, call the system function directly */
#define SS_ASSERT(cond)  \
{\
    if (!(cond))\
    {\
        exit((SS_INT32)SIP_RET_FAILURE);\
    }\
}
#else

/* Define assert as empty if not debug */
#define SS_ASSERT(cond)

#endif   /* #ifdef WIN32 */

#else

/* Define assert as empty if not debug */
#define SS_ASSERT(cond)

#endif



#ifdef __cplusplus
}
#endif

#endif  /*__SS_OSFUNCWRAPPER_H__ */

