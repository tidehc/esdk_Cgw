/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrcntnttypeclass.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-23

    Description     : This file provides the initialization interfaces for
                      Content-Type class headers.
                      (Accept and ContentType headers)

    Function List   :
        1. SipHdrAcceptInit
        2. SipHdrContentTypeInit


    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDRCNTNTTYPECLASS_H__
#define __SS_HDRCNTNTTYPECLASS_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssheaders.h"
#include "sscbbtypes.x"

typedef struct _SipContentType
{
    SipString              strMediaType;
    SipString              strSubMediaType;
    SipGenericParamListHdl hdlMediaParams;
}SipContentType;

typedef SipContentType SipAccept;
typedef SipHeaderListHdl  SipAcceptListHdl;



/*******************************************************************************
 Function   : SipHdrAcceptInit

 Description: This function is used to initialize the Accept Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrAcceptInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrContentTypeInit

 Description: This function is used to initialize the ContentType Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrContentTypeInit
(
    SipHdrInfoHdl hdrHdrInfo
);

#ifdef __cplusplus
}
#endif

#endif /* __SS_HDRCNTNTTYPECLASS_H__ */

