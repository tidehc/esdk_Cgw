/*******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName   : ssrandutil.h

    Author     : SriniCH, Bajeed

    Version    : 1.0

    Date       : 2005-12-08

    Description: This file details the random utilities that should be provided
                 by MiniSip user

    Function List:
        1. SipUtilGenRandomData

    History:
        1. Date         : <YYYY-MM-DD>
           Tag          :
           Author       :
           Modification :
*******************************************************************************/

#ifndef _SS_RAND_UTIL_H_
#define _SS_RAND_UTIL_H_


#ifdef __cplusplus
extern "C" {
#endif


/*******************************************************************************
 Function   : SipUtilGenRandomData

 Description: This API will generate the random data, generally useful to
              generate Tags, BranchIds etc.

 Input      : ulLen - Length of the random data to be generated

 Output     : pucData - Generated random data is copied here

 Return     : VOID
*******************************************************************************/
SS_VOID SipUtilGenRandomData
(
    IN SS_UCHAR *pucData,
    IN SS_UINT32 ulLen
);


#ifdef __cplusplus
}
#endif

#endif

