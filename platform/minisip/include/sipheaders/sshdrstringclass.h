/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrstringclass.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-23

    Description     : This file provides the initialization interfaces for
                      string class headers.

    Function List   :
        1. SipHdrSubjectInit
        2. SipHdrContentLanguageInit
        3. SipHdrInReplyToInit
        4. SipHdrOrganizationInit
        5. SipHdrPChargVectorInit
        6. SipHdrPMediaAuthorizationInit
        7. SipHdrSipETagInit
        8. SipHdrSIPIfMatchInit



    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : Harsha GXXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDRSTRINGCLASS_H__
#define __SS_HDRSTRINGCLASS_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssdatatype.h"
#include "ssheaders.h"
#include "sscbbtypes.x"

typedef SipStringListHdl SipContentLanguageListHdl;

typedef SipStringListHdl SipInReplyToListHdl;

typedef SipString SipOrganization;

typedef SipStringListHdl SipPMediaAuthorizationListHdl;

typedef SipString SIPEtag;

typedef SipString SIPIfMatch;

typedef SipString SipSubject;


/*******************************************************************************
 Function   : SipHdrContentLanguageInit

 Description: This function is used to initialize the ContentLanguage Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrContentLanguageInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrInReplyToInit

 Description: This function is used to initialize the InReplyTo Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrInReplyToInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrOrganizationInit

 Description: This function is used to initialize the Organization Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrOrganizationInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrRSeqInit

 Description: This function is used to initialize the RSeq Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrPMediaAuthorizationInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrSipETagInit

 Description: This function is used to initialize the SipETag Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrSipETagInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrSipIfMatchInit

 Description: This function is used to initialize the SipIfMatch Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrSIPIfMatchInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrSubjectInit

 Description: This function is used to initialize the Subject Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrSubjectInit
(
    SipHdrInfoHdl hdlHdrInfo
);


#ifdef __cplusplus
}
#endif

#endif /* __SS_HDRSTRINGCLASS_H__ */

