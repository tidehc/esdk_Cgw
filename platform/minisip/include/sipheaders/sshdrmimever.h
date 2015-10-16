/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrmimever.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-15

    Description     : This file provides the initialization function for
                      MIMEVersion header

    Function List   :
        1. SipHdrMIMEVersionInit

    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : Harsha GXXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDR_MIMEVER_H__
#define __SS_HDR_MIMEVER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssheaders.h"
#include "sscbbtypes.x"

typedef struct _SipMIMEVersion
{
    SS_UINT16 usMajorVersion;
    SS_UINT16 usMinorVersion;
}SipMIMEVersion;

/*******************************************************************************
 Function   : SipHdrMIMEVersionInit

 Description: This function is used to initialize the MIMEVersion Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrMIMEVersionInit
(
    SipHdrInfoHdl hdlHdrInfo
);

#ifdef __cplusplus
}
#endif

#endif /* __SS_HDR_MIMEVER_H__*/

