/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrtokenclass.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-23

    Description     : This file provides the initialization interfaces for token
                      class headers.

    Function List   :
        1. SipHdrContentEncodingInit
        2. SipHdrPriorityInit
        3. SipHdrProxyRequireInit
        4. SipHdrRequestDispositionInit
        5. SipHdrRequireInit
        6. SipHdrSupportedInit
        7. SipHdrUnsupportedInit


    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : Harsha GXXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDRTOKENCLASS_H__
#define __SS_HDRTOKENCLASS_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssdatatype.h"
#include "ssheaders.h"
#include "sscbbtypes.x"

typedef SipTokenListHdl SipContentEncodingListHdl;

typedef SipToken        SipPriority;

typedef SipTokenListHdl SipProxyRequireListHdl;

typedef SipTokenListHdl SipRequestDispositionListHdl;

typedef SipTokenListHdl SipRequireListHdl;

typedef SipTokenListHdl SipSupportedListHdl;

typedef SipTokenListHdl SipUnsupportedListHdl;



/*******************************************************************************
 Function   : SipHdrContentEncInit

 Description: This function is used to initialize the ContentEnc Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrContentEncodingInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrPriorityInit

 Description: This function is used to initialize the Priority Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrPriorityInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrProxyRequireInit

 Description: This function is used to initialize the ProxyRequire Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrProxyRequireInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrRequestDispositionInit

 Description: This function is used to initialize the RequestDisposition Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrRequestDispositionInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrRequireInit

 Description: This function is used to initialize the Require Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrRequireInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrRSeqInit

 Description: This function is used to initialize the RSeq Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrSupportedInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrUnSupportedInit

 Description: This function is used to initialize the UnSupported Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrUnsupportedInit
(
    SipHdrInfoHdl hdrHdrInfo
);


#ifdef __cplusplus
}
#endif

#endif /* __SS_HDRTOKENCLASS_H__ */

