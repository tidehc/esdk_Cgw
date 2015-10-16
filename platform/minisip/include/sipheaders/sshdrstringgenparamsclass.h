/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrstringgenparamsclass.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-23

    Description     : This file provides the initialization interfaces for
                      string generic-params headers.

    Function List   :
        1. SipHdrAcceptLanguageInit
        2. SipHdrAlertInfoInit
        3. SipHdrCallInfoInit
        4. SipHdrErrorInfoInit
        5. SipHdrJoinInit
        6. SipHdrPVisitedNetworkIDInit
        7. SipHdrReplacesInit

    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : Harsha GXXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDRSTRINGGENPARAMSCLASS_H__
#define __SS_HDRSTRINGGENPARAMSCLASS_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssheaders.h"
#include "sscbbtypes.x"

typedef SipStringGenParamsListHdl SipAcceptLanguageListHdl;

typedef SipStringGenParamsListHdl SipAlertInfoListHdl;

typedef SipStringGenParamsListHdl SipCallInfoListHdl;

typedef SipStringGenParamsListHdl SipErrorInfoListHdl;

typedef SipStringGenParamsListHdl SipPVisitedNetworkIDListHdl;

typedef SipStringGenParams SipJoin;

typedef SipStringGenParams SipReplaces;


/*******************************************************************************
 Function   : SipHdrAcceptLanguageInit

 Description: This function is used to initialize the AcceptLanguage Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrAcceptLanguageInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrAlertInfoInit

 Description: This function is used to initialize the AlertInfo Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrAlertInfoInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrCallInfoInit

 Description: This function is used to initialize the CallInfo Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrCallInfoInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrErrorInfoInit

 Description: This function is used to initialize the ErrorInfo Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrErrorInfoInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrJoinInit

 Description: This function is used to initialize the Join Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrJoinInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrPVisitedNetworkIDInit

 Description: This function is used to initialize the PVisitedNetworkID Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrPVisitedNetworkIDInit
(
    SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrReplacesInit

 Description: This function is used to initialize the Replaces Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrReplacesInit
(
    SipHdrInfoHdl hdlHdrInfo
);

#ifdef __cplusplus
}
#endif

#endif /* __SS_HDRSTRINGGENPARAMSCLASS_H__ */

