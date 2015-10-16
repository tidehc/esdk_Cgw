/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName: ssdsmunknownhdrs.x

    Author: Lei
    Version:1.0

    Date: 2005-11-17
    Description: This file is to provide some APIs to get some unrecognized
                heades including unknown and Dec-Failed header list
    Function List:

    History:
        1. Date:
        Defect:
        Tag: <MOD.001>
        Author:
        Modification:

*******************************************************************************/
#ifndef __SSDSMUNKKNOWNHDRS_X__
#define __SSDSMUNKKNOWNHDRS_X__

#include "ssmessage.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
    Function Name : SipDsmGetUnknownHdrFromMsg

    Description  :This function is to get the unknown header from the message
                  according to the inputted index

    Inputs      :  usIndex    - The index on the unkown header list
                   pstSipMsg  - The SIP message containing the unknown header list

    Output      :  None

    Returns     : SipHdrNameValue * - The found header name value
******************************************************************************/
SipHdrNameValue * SipDsmGetUnknownHdrFromMsg
(
    IN   SS_UINT16      usIndex,
    IN   SipMsgS       *pstSipMsg
);

/******************************************************************************
    Function Name : SipDsmGetDecFailedHdrFromMsg

    Description  :This function is to get the DecFailed header from the message
                  according to the inputted index

    Inputs      :  usIndex    - The index on the Dec-Failed header list
                   pstSipMsg  - The SIP message containing the DecFailed header list

    Output      :  None

    Returns     : SipHdrNameValue * - The found header name value
******************************************************************************/
SipHdrNameValue * SipDsmGetDecFailedHdrFromMsg
(
    IN   SS_UINT16      usIndex,
    IN   SipMsgS       *pstSipMsg
);

/*******************************************************************************
    Function Name : SipDsmGetUnknownHdrCountOfMsg

    Description  :This function is to get the count on the list

    Inputs      :  pstMsg - The corresponding sip message.

    Output      :  pusCount - The count of unknown headers in the message

    Returns     : EN_SIP_RESULT
*******************************************************************************/
EN_SIP_RESULT SipDsmGetUnknownHdrCountOfMsg
(
    IN   SipMsgS     *pstMsg,
    OUT  SS_UINT16   *pusCount
);

/*******************************************************************************
    Function Name : SipDsmGetDecFailedHdrCountOfMsg

    Description  :This function is to get the count on the list

    Inputs      : pstMsg - The corresponding sip message

    Output      : pusCount - The count of DecFailed headers in the message

    Returns     : EN_SIP_RESULT
*******************************************************************************/
EN_SIP_RESULT SipDsmGetDecFailedHdrCountOfMsg
(
    IN   SipMsgS     *pstMsg,
    OUT  SS_UINT16   *pusCount
);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif /* __SSDSMUNKKNOWNHDRS_X__*/

