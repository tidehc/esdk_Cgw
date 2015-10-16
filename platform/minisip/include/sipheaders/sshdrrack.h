/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrrack.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-15

    Description     : This file provides the initialization function for RAck

    Function List   :
        1. SipHdrRAckInit

    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : Harsha GXXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDR_RACK_H__
#define __SS_HDR_RACK_H__

#include "ssheaders.h"
#include "ssfrmreg.x"
#include "ssenums.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssdatatype.h"
#include "ssheaders.h"

typedef struct _SipRAck
{
    SS_UINT32   ulResponseNum;
    SipCSeq     stCSeqPart;
}SipRAck;

/*******************************************************************************
 Function   : SipHdrRAckInit

 Description: This function is used to initialize the RAck Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrRAckInit
(
    SipHdrInfoHdl hdlHdrInfo
);

#ifdef __cplusplus
}
#endif

#endif /*__SS_HDR_RACK_H__*/

