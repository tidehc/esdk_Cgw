/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrpxyauthenclass.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-23

    Description     : This file provides the initialization interfaces for
                      Proxy-Authenticate class headers.
                      (Proxy-Authenticate and WWW-Authenticate).


    Function List   :
        1. SipHdrProxyAuthenticateInit
        2. SipHdrWWWAuthenicateInit


    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDRPXYAUTHENCLASS_H__
#define __SS_HDRPXYAUTHENCLASS_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssdatatype.h"
#include "ssheaders.h"
#include "sscbbtypes.x"

typedef struct _SipProxyAuthenticate
{
    SS_BOOL                bIsAuthDigestType;
    SipString             *pstrAuthScheme;
    SipString             *pstrRealm;
    SipString             *pstrDomainURI;
    SipString             *pstrNonce;
    SipString             *pstrOpaque;
    SipString             *pstrStale;
    SipString             *pstrAlgorithm;
    SipString             *pstrQOPOptions;
    SipGenericParamListHdl hdlAuthenParams;
}SipProxyAuthenticate;

typedef SipHeaderListHdl SipProxyAuthenticateListHdl; /* list of SipProxyAuthenticate's */

typedef SipProxyAuthenticate     SipWWWAuthenticate;
typedef SipProxyAuthenticateListHdl SipWWWAuthenticateListHdl;



/*******************************************************************************
 Function   : SipHdrProxyAuthenticateInit

 Description: This function is used to initialize the ProxyAuthenticate Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrProxyAuthenticateInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrWWWAuthenicateInit

 Description: This function is used to initialize the WWWAuthenicate Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrWWWAuthenicateInit
(
    SipHdrInfoHdl hdrHdrInfo
);

#ifdef __cplusplus
}
#endif

#endif /* __SS_HDRPXYAUTHENCLASS_H__ */

