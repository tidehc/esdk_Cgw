/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrauthinfo.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-19

    Description     : This file provides the initialization function for
                      AuthenticationInfo header

    Function List   :
        1. SipHdrAuthenticationInfoInit

    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : Harsha GXXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDR_AUTHINFO_H__
#define __SS_HDR_AUTHINFO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "sscbbtypes.x"
#include "ssenums.h"

typedef struct _SipAuthenticationInfoS
{
    SipString *pstrNextNonce;
    SipString *pstrMessageQOP;
    SipString *pstrRespAuth;
    SipString *pstrCNonce;
    SipString *pstrNonceCount;
}SipAuthenticationInfoS;

typedef SipHeaderListHdl SipAuthenticationInfoListHdl; /* list of AuthenticationInfo*/

/*******************************************************************************
 Function   : SipHdrAuthenticationInfoInit

 Description: This function is used to initialize the AuthenticationInfo Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrAuthenticationInfoInit
(
    SipHdrInfoHdl hdlHdrInfo
);

#ifdef __cplusplus
}
#endif

#endif /* __SS_HDR_AUTHINFO_H__ */

