/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrgenparamslistclass.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-23

    Description     : This file provides the initialization interfaces for
                      Generic-params list class headers.

    Function List   :
        1. SipHdrAcceptContactInit
        2. SipHdrPChargFuncAddrInit
        3. SipHdrRejectContactInit
        4. SipHdrPChargVectInit

    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : Harsha GXXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDRGENPARAMSLISTCLASS_H__
#define __SS_HDRGENPARAMSLISTCLASS_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssheaders.h"
#include "sscbbtypes.x"

typedef SipGenericParamsHdr        SipAcceptContact;
typedef SipGenericParamsHdrListHdl SipAcceptContactListHdl;

typedef SipGenericParamsHdr        SipPChargFuncAddr;

typedef SipGenericParamsHdr        SipPChargVect;

typedef SipGenericParamsHdr        SipRejectContact;
typedef SipGenericParamsHdrListHdl SipRejectContactListHdl;


/*******************************************************************************
 Function   : SipHdrAcceptContactInit

 Description: This function is used to initialize the AcceptContact Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrAcceptContactInit
(
    SipHdrInfoHdl hdrHdrInfo
);


/*******************************************************************************
 Function   : SipHdrRejectContactInit

 Description: This function is used to initialize the RejectContact Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrRejectContactInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrPChargFuncAddrInit

 Description: This function is used to initialize the PChargFuncAddr Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrPChargFuncAddrInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrPChargVectInit

 Description: This function is used to initialize the PChargVctr Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrPChargVectInit
(
    SipHdrInfoHdl hdrHdrInfo
);


#ifdef __cplusplus
}
#endif

#endif /* __SS_HDRGENPARAMSLISTCLASS_H__ */

