/*******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName   : randutil.h

    Author     : SriniCH, Bajeed

    Version    : 1.0

    Date       : 2005-12-08

    Description: This file details the random utilities

    Function List:
        1. SipUtilRandInit

    History:
        1. Date         : <YYYY-MM-DD>
           Tag          :
           Author       :
           Modification :
*******************************************************************************/

#ifndef IVSCBB_SS_RAND_UTIL_H
#define IVSCBB_SS_RAND_UTIL_H

#include "rand.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


typedef struct _SipUtilGlobalVarS
{
    randctx   stRandomContext;
    SS_UINT32 ulRandomIP;
    SS_UINT32 ulTickCount;
}SipUtilGlobalVarS;

#ifdef SS_NON_GLOBAL_VARIABLE
/* external function defined by the rand user */
extern SipUtilGlobalVarS *SipUtilGloabalVariableGet
(
    SS_VOID
);

/*******************************************************************************
 Function   : SipUtilAllocGlobalVariable

 Description: This API is used to allocate the memory for util global data

 Input      : None

 Output     : ppstUtilData - UtilData allocated

 Return     : VOID
*******************************************************************************/
SipUtilGlobalVarS * SipUtilAllocGlobalVariable
(
    SS_VOID
);

/*******************************************************************************
 Function   : SipUtilFreeGlobalVariable

 Description: This API is used to free the memory of util global data

 Input      : None

 Output     : None

 Return     : VOID
*******************************************************************************/
SS_VOID SipUtilFreeGlobalVariable
(
    SS_VOID
);

#endif

/*******************************************************************************
 Function   : SipUtilRandInit

 Description: This function will Initialize the random generator

 Input      : None

 Output     : None

 Return     : success or failure
*******************************************************************************/
EN_SIP_RESULT SipUtilRandInit
(
    SS_VOID
);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif //randutil.h

