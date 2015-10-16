/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrnumclass.h

    Author          : Manjunath S Warad

    Version         : 1.0

    Date            : 2005-12-23

    Description     : This file provides the initialization interfaces for
                      number class headers.

    Function List   :
        1. SipHdrRSeqInit
        2. SipHdrExpiresInit
        3. SipHdrMaxForwardsInit
        4. SipHdrMinExpiresInit



    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDRNUMCLASS_H__
#define __SS_HDRNUMCLASS_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssheaders.h"
#include "ssdatatype.h"

typedef SS_UINT32 SipExpires;

typedef SS_UINT32 SipMinExpires;

typedef SS_UINT32 SipMaxForwards;

typedef SS_UINT32 SipRSeq;



/*******************************************************************************
 Function   : SipHdrCSeqInit

 Description: This function is used to initialize the CSeq Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrExpiresInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrMinExpiresInit

 Description: This function is used to initialize the MinExpires Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrMinExpiresInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
);

/*******************************************************************************
 Function   : SipHdrMaxForwardsInit

 Description: This function is used to initialize the MaxForwards Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrMaxForwardsInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
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
EN_SIP_RESULT SipHdrRSeqInit
(
    IO  SipHdrInfoHdl hdlHdrInfo
);



#ifdef __cplusplus
}
#endif

#endif /* __SS_HDRNUMCLASS_H__ */

