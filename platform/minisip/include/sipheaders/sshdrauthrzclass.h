/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrauthrzclass.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-23

    Description     : This file provides the initialization interfaces for
                      Authorization class headers.
                      (Authorization and Proxy-Authorization)

    Function List   :
        1. SipHdrAuthorizationInit
        2. SipHdrProxyAuthorizationInit


    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDRAUTHRZCLASS_H__
#define __SS_HDRAUTHRZCLASS_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssdatatype.h"
#include "sscbbtypes.x"
#include "ssenums.h"
#include "ssheaders.h"

typedef struct _SipAuthorization
{
    SS_BOOL                bIsAuthDigestType;
    SipString             *pstrAuthScheme;
    SipString             *pstrUsername;
    SipString             *pstrRealm;
    SipString             *pstrNonce;
    SipString             *pstrDigestUri;
    SipString             *pstrDResponse;
    SipString             *pstrAlgorithm;
    SipString             *pstrCNonce;
    SipString             *pstrOpaque;
    SipString             *pstrMessageQOP;
    SipString             *pstrNonceCount;
    SipGenericParamListHdl hdlAuthrzParams;
}SipAuthorization;

typedef SipHeaderListHdl SipAuthorizationListHdl;/*ListHdl of SipAuthorization*/

typedef SipAuthorization     SipProxyAuthorization;

typedef SipAuthorizationListHdl SipProxyAuthorizationListHdl;


/*******************************************************************************
 Function   : SipHdrAuthorizationInit

 Description: This function is used to initialize the Authorization Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrAuthorizationInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrXXXXInit

 Description: This function is used to initialize the XXXX Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrProxyAuthorizationInit
(
    SipHdrInfoHdl hdrHdrInfo
);

#ifdef __cplusplus
}
#endif

#endif /* __SS_HDRAUTHRZCLASS_H__ */

