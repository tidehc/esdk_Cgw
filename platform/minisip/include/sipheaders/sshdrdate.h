/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrdate.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-15

    Description     : This file provides the initialization function for Date

    Function List   :
        1. SipHdrDateInit

    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : Harsha GXXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDR_DATE_H__
#define __SS_HDR_DATE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ssheaders.h"
#include "ssfrmreg.x"
#include "ssdatatype.h"
#include "sscbbtypes.x"
#include "ssenums.h"


typedef struct _SipDate
{
    SS_UINT32   ulYear;        /* 4 bytes  */
    SipToken    stMonth;       /* 12 bytes */
    SipToken    stWeekDay;     /* 12 bytes */
    SS_UCHAR    ucDay;         /* 1 byte   */
    SS_UCHAR    ucHour;        /* 1 byte   */
    SS_UCHAR    ucMinute;      /* 1 byte   */
    SS_UCHAR    ucSecond;      /* 1 bytes  */
}SipDate;


/*******************************************************************************
 Function   : SipHdrDateInit

 Description: This function is used to initialize the Date Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrDateInit
(
    SipHdrInfoHdl hdlHdrInfo
);

#ifdef __cplusplus
}
#endif

#endif /*__SS_HDR_DATE_H__*/

