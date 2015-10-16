/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrurihdrclass.h

    Author          : Manjunath S Warad

    Version         : 1.0

    Date            : 2005-12-23

    Description     : This file provides the initialization interfaces for
                      Uri-header class headers.

    Function List   :

        1. SipHdrReferToInit
        2. SipHdrReplyToInit
        3. SipHdrReferredByInit
        4. SipHdrServiceRouteInit
        5. SipHdrHistoryInfoInit
        6. SipHdrPAssertedIdentityInit
        7. SipHdrPAssociatedURIInit
        8. SipHdrPathInit
        9. SipHdrPCalledPartyIDInit
        10. SipHdrPPreferredIdentityInit

    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDRURIHDRCLASS_H__
#define __SS_HDRURIHDRCLASS_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssheaders.h"
#include "ssenums.h"

typedef SipUriHeaderListHdl SipHistoryInfoListHdl;

typedef SipUriHeaderListHdl SipPAssociatedURIListHdl;

typedef SipUriHeaderListHdl SipPAssertedIdListHdl;

typedef SipUriHeaderListHdl SipPathListHdl;

typedef SipUriHeader SipPCalledPartyId;

typedef SipUriHeaderListHdl SipPPreferredIdListHdl;

typedef SipUriHeader SipReferredBy;

typedef SipUriHeader SipReferTo;

typedef SipUriHeader SipReplyTo;

typedef SipUriHeaderListHdl SipServiceRouteListHdl;


/*******************************************************************************
 Function   : SipHdrHistoryInfoInit

 Description: This function is used to initialize the HistoryInfo Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrHistoryInfoInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrPAssociatedURIInit

 Description: This function is used to initialize the PAssociatedURI Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrPAssociatedURIInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrPAssertedIdentityInit

 Description: This function is used to initialize the PAssertedIdentity Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrPAssertedIdentityInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrPathInit

 Description: This function is used to initialize the Path Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrPathInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrPCalledPartyIDInit

 Description: This function is used to initialize the PCalledPartyID Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrPCalledPartyIDInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrPPreferredIdentityInit

 Description: This function is used to initialize the PPreferredIdentity Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrPPreferredIdentityInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrReferredByInit

 Description: This function is used to initialize the ReferredBy Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrReferredByInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrReferToInit

 Description: This function is used to initialize the ReferTo Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrReferToInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrReplyToInit

 Description: This function is used to initialize the ReplyTo Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrReplyToInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrServiceRouteInit

 Description: This function is used to initialize the ServiceRoute Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrServiceRouteInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
);


#ifdef __cplusplus
}
#endif

#endif /* __SS_HDRURIHDRCLASS_H__ */

