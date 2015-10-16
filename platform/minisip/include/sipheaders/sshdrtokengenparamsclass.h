/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrtokengenparamsclass.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-23

    Description     : This file provides the initialization interfaces for
                      Token generic-params class headers.


    Function List   :
        1. SipHdrContentDispositionInit
        2. SipHdrPAccessNetworkInfoInit
        3. SipHdrReasonInit
        4. SipHdrSecurityClientInit
        5. SipHdrSecurityServerInit
        6. SipHdrSecurityVerifyInit
        7. SipHdrSubscriptionStateInit
        8. SipHdrAcceptEncodingInit

    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : Harsha GXXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDRTOKENGENPARAMSCLASS_H__
#define __SS_HDRTOKENGENPARAMSCLASS_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssheaders.h"

typedef SipTokenGenParams        SipPAccessNetworkInfo;

typedef SipTokenGenParams        SipContentDisposition;

typedef SipTokenGenParamsListHdl SipAcceptEncodingListHdl;

typedef SipTokenGenParamsListHdl SipReasonListHdl;

typedef SipTokenGenParamsListHdl SipSecurityClientListHdl;

typedef SipTokenGenParamsListHdl SipSecurityServerListHdl;

typedef SipTokenGenParamsListHdl SipSecurityVerifyListHdl;

typedef SipTokenGenParams        SipSubscriptionState;


/*******************************************************************************
 Function   : SipHdrContentDispositionInit

 Description: This function is used to initialize the ContentDisposition Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrContentDispositionInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrPAccessNetworkInfoInit

 Description: This function is used to initialize the PAccessNetworkInfo Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrPAccessNetworkInfoInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrAcceptEncodingInit

 Description: This function is used to initialize the AcceptEncoding Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrAcceptEncodingInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrReasonInit

 Description: This function is used to initialize the Reason Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrReasonInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrSecurityClientInit

 Description: This function is used to initialize the SecurityClient Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrSecurityClientInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrSecurityServerInit

 Description: This function is used to initialize the SecurityServer Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrSecurityServerInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrSecurityVerifyInit

 Description: This function is used to initialize the SecurityVerify Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrSecurityVerifyInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrSubscriptionStateInit

 Description: This function is used to initialize the SubscriptionState Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrSubscriptionStateInit
(
    SipHdrInfoHdl hdlHdrInfo
);

#ifdef __cplusplus
}
#endif

#endif /* __SS_HDRTOKENGENPARAMSCLASS_H__ */

