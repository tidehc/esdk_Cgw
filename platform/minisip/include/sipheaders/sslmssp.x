/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName      : sslmssp.x

    Author        : Srinivas

    Version       : 1.0

    Date          : 2005-09-14

    Description   : This file contains the external dependency interfaces towards
                 the system service plane which needs to be supplied by the
                 stack user.

    Function List :
       1.SipStartTimer
       2.SipStopTimer

    History       :
        1. Date:<2005-09-14>
        Tag: None
        Defect: NA
        Author: Srinivas
        Modification: Creation

******************************************************************************/

#ifndef __SS_LM_SSP_X__
#define __SS_LM_SSP_X__

#include "sssipbase.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 Function Name : SipStartTimer

 Description   : This function is called by stack to start a timer but
                 implemented by the stack user.

 Input(s)      : ulTimerLength - expire time(millseconds).
                 ulTimerParam  - Any additional timer param required in
                                 callback API and first 4 bits should be the
                                 user of the timer.

 Output(s)     : *pulTimerIdx    - Timer ID.

 Returns       : Currently SIP Stack components do not need to check return
                 value
*******************************************************************************/
EN_SIP_RESULT SipStartTimer
(
    IN  SS_UINT32      ulTimerLength,
    IN  SS_UINT32      ulTimerParam,
    OUT SS_UINT32     *pulTimerId
);

/******************************************************************************
    Function Name : SipStopTimer

    Description   : This function is called by stack to stop a timer
                    but implemented by the stack user.

    Input(s)      : ulTimerId   -   Timer ID

    Output(s)     : None

    Returns       : There is no need to check the return value
******************************************************************************/
SS_VOID SipStopTimer
(
    IN SS_UINT32      ulTimerId
);



#ifdef __cplusplus
}
#endif

#endif /*__SS_LM_SSP_X__ */
