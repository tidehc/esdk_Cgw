/*******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName   : ssutil.c

    Author     : SriniCH, Bajeed

    Version    : 1.0

    Date       : 2005-11-23

    Description: This file provides the random utilities

    Function List:
        1. SipGetTick
        2. SipUtilRandInit
        3. SipUtilGenRandomData

    History:
        1. Date         : <YYYY-MM-DD>
           Tag          :
           Author       :
           Modification :
*******************************************************************************/

#include <stdlib.h>
#include "randutil.h"


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */


#define SIP_TICK_COUNT_INIT  (19760601)  /* random number */

#define SIP_UTL_RAND_MASK  0x0000000F
#define SIP_UTIL_BIT_SHIFT 4

#define SIP_RAND_INIT_NUM  1887043216


#ifndef SS_NON_GLOBAL_VARIABLE

STATIC SipUtilGlobalVarS gstUtilData;

#define gstSipRandomContext (gstUtilData.stRandomContext)
#define gulSipRandomIP      (gstUtilData.ulRandomIP)
#define gulTickCount        (gstUtilData.ulTickCount)

#else /* For SS_NON_GLOBAL_VARIABLE */

#define gstSipRandomContext (((SipUtilGlobalVarS *)SipUtilGloabalVariableGet())->stRandomContext)
#define gulSipRandomIP      (((SipUtilGlobalVarS *)SipUtilGloabalVariableGet())->ulRandomIP)
#define gulTickCount        (((SipUtilGlobalVarS *)SipUtilGloabalVariableGet())->ulTickCount)


/*******************************************************************************
 Function   : SipUtilAllocGlobalVariable

 Description: This API is used to allocate the memory for util global data

 Input      : None

 Output     : None

 Return     : ppstUtilData - UtilData allocated
*******************************************************************************/
SipUtilGlobalVarS *SipUtilAllocGlobalVariable
(
    SS_VOID
)
{
    return ((SipUtilGlobalVarS *)VppSysMalloc(sizeof(SipUtilGlobalVarS)));
}

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
)
{
    SipUtilGlobalVarS *pstUtilData = SS_NULL_PTR;

    pstUtilData = SipUtilGloabalVariableGet();
    if ( SS_NULL_PTR != pstUtilData )
    {
        VppSysFree(pstUtilData);
    }
    return;
}

#endif


/*******************************************************************************
 Function   : SipGetTick

 Description: This function will be used to generate a tick value

 Input      : None

 Output     : None

 Return     : the tick number
*******************************************************************************/
STATIC SS_UINT32 SipGetTick
(
    SS_VOID
)
{
    
    //gulTickCount++;

    /* wrap the tick to start with initial value */
    /*
    if ( SS_NULL_UINT32 == gulTickCount )
    {
        gulTickCount = SIP_TICK_COUNT_INIT;
    }
    */
    
    //直接取系统时间戳
    gulTickCount = VppSysGetTick();

    SIP_RETURN(gulTickCount)
}

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
)
{
    SS_UINT32 ulIP = SIP_RAND_INIT_NUM;
    SS_INT    i    = 0;

#ifndef SS_NON_GLOBAL_VARIABLE
    (SS_VOID)VppSysMemSet(&gstUtilData, 0, sizeof(SipUtilGlobalVarS));
#endif

    gstSipRandomContext.randa=(SS_UINT)0;
    gstSipRandomContext.randb=(SS_UINT)0;
    gstSipRandomContext.randc=(SS_UINT)0;
    for (i = 0; i < RANDSIZ; ++i)//lint !e838
    {
        gstSipRandomContext.randrsl[i]=(SS_UINT)0;
    }

    siprandinit(&gstSipRandomContext, SS_TRUE);

    gulSipRandomIP = ulIP;
    gstSipRandomContext.posn = 0;
    gulTickCount = SIP_TICK_COUNT_INIT;

    SIP_RETURN(SIP_RET_SUCCESS)
}

/*******************************************************************************
 Function   : SipUtilGenRandomData

 Description: This API will generate the random string of given length,
              generally useful to generate Tags, BranchIds etc.

 Input      : ulLen - Length of the random data to be generated

 Output     : pucData - Generated random data is copied here

 Return     : VOID
*******************************************************************************/
SS_VOID SipUtilGenRandomData
(
    IN SS_UCHAR *pucData,
    IN SS_UINT32 ulLen
)
{
    SS_UINT32 i        = 0;
    SS_UINT32 j        = 0;
    SS_UINT32 ctr      = 0;
    SS_UINT   locCache = 0;
    SS_CHAR   hexMap[] = "0123456789abcdef";
    SS_UINT32 ulTime   = 0;
    SS_UINT32 mask     = 0;

    ulTime = SipGetTick();//lint !e838
    mask = ulTime ^ gulSipRandomIP;//lint !e838

    if (   ( RANDSIZ <= gstSipRandomContext.posn )
        || ( ulLen >= (RANDSIZ - gstSipRandomContext.posn) * 8) )
    {
        sipisaac(&gstSipRandomContext);
        i = 0;
    }
    else
    {
        i = gstSipRandomContext.posn;
    }
    ctr=0;//lint !e838

    for (; i < RANDSIZ; i++)
    {
        locCache = gstSipRandomContext.randrsl[i];
        locCache ^= mask;
        for (j = 0; j < 8; j++)
        {
            pucData[ctr++] = (SS_UCHAR)hexMap
                [(locCache >> (j * SIP_UTIL_BIT_SHIFT)) & SIP_UTL_RAND_MASK];
            if ( ctr == ulLen )
            {
                /*lint -e801 */
                goto __sugrdLoopExit;
                /*lint -e801 */
            }
        }
    }

__sugrdLoopExit:
   gstSipRandomContext.posn = i + 1;
   SIP_RETVOID()
}



#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */
