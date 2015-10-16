/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrretryafterclass.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-23

    Description     : This file provides the initialization interfaces for
                      Retry-After class headers.
                      (Retry-After, Min-SE, Session-Expires)

    Function List   :
        1. SipHdrMinSEInit
        2. SipHdrRetryAfterInit
        3. SipHdrSessionExpiresInit


    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDRRETRYAFTERCLASS_H__
#define __SS_HDRRETRYAFTERCLASS_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssdatatype.h"
#include "ssheaders.h"
#include "sscbbtypes.x"

typedef struct _SipRetryAfter
{
    SS_UINT32               ulDeltaSeconds;
    SipString              *pstrComment;
    SipGenericParamListHdl  hdlRetryParams;
}SipRetryAfter;

typedef SipRetryAfter SipSessionExpires;

typedef SipRetryAfter SipMinSE;


/*******************************************************************************
 Function   : SipHdrRetryAfterInit

 Description: This function is used to initialize the RetryAfter Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrRetryAfterInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrMinSEInit

 Description: This function is used to initialize the MinSE Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrMinSEInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrSessionExpiresInit

 Description: This function is used to initialize the SessionExpires Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrSessionExpiresInit
(
    SipHdrInfoHdl hdrHdrInfo
);

#ifdef __cplusplus
}
#endif

#endif /* __SS_HDRRETRYAFTERCLASS_H__ */

