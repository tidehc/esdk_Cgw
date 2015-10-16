/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrevntclass.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-23

    Description     : This file provides the initialization interfaces for
                      Event class headers.
                      (Event and Allow-Events headers)

    Function List   :
        1. SipHdrAllowEventsInit
        2. SipHdrEventInit


    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDREVNTCLASS_H__
#define __SS_HDREVNTCLASS_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssheaders.h"
#include "sscbbtypes.x"

typedef struct _SipEventType
{
    SipString        *pstrEvntPkgType;
    SipStringListHdl  hdlStrEvntTemplateListHdl;
}SipEventType;

typedef struct _SipEvent
{
    SipEventType           stEvntType;
    SipGenericParamListHdl hdlEventParams;
}SipEvent;

typedef SipEventType       SipAllowEvent;
typedef SipHeaderListHdl   SipAllowEventListHdl; /* list of Event*/


/*******************************************************************************
 Function   : SipHdrEventInit

 Description: This function is used to initialize the Event Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrEventInit
(
    SipHdrInfoHdl hdrHdrInfo
);

/*******************************************************************************
 Function   : SipHdrAllowEventsInit

 Description: This function is used to initialize the AllowEvents Header

 Input      : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdrHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrAllowEventsInit
(
    SipHdrInfoHdl hdrHdrInfo
);

#ifdef __cplusplus
}
#endif

#endif /* __SS_HDREVNTCLASS_H__ */

