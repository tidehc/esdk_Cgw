/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrprivacy.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-15

    Description     : This file provides the initialization function for Privacy

    Function List   :
        1. SipHdrPrivacyInit

    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : Harsha GXXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDR_PRIVACY_H__
#define __SS_HDR_PRIVACY_H__

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssheaders.h"
#include "sscbbtypes.x"

#ifdef __cplusplus
extern "C" {
#endif

typedef SipTokenListHdl SipPrivacyListHdl;

/*******************************************************************************
 Function   : SipHdrPrivacyInit

 Description: This function is used to initialize the Privacy Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrPrivacyInit
(
    SipHdrInfoHdl hdlHdrInfo
);

#ifdef __cplusplus
}
#endif

#endif /* __SS_HDR_PRIVACY_H__*/

