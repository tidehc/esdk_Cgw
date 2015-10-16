/*******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName   : sstxnutil.x

    Author     : Bajeed

    Version    : 1.0

    Date       : 2005-09-08

    Description: This header file contains the utilities provided by the
                 transaction component for the transaction users

    Function List:
        1. SipTxnUtilGenTag
        2. SipTxnUtilGenBranchId
        3. SipTxnTimeoutHandler
        4. SipTxnUtilCopyCallPrefHeaders

    History:
           1. Date      : <2006-06-08>
           Tag          : <Tag.YDHD02398>
           Author       : SriniCH
           Modification : The order of params are wrong in SipTxnTimeoutHandler

*******************************************************************************/

#ifndef _SS_TXN_UTIL_X_
#define _SS_TXN_UTIL_X_


#include "ssdatatype.h"
#include "ssmessage.h"
#include "sssipbase.h"
#include "sscbbtypes.x"


#ifdef __cplusplus
extern "C" {
#endif

/*Start <Tag.YDHD02398> */
/*******************************************************************************
 Function   : SipTxnTimeoutHandler

 Description: This is a interface of the timer that will be invoked by the user
              on time out if Module ID is transaction.

 Input(s)   : ulParams - Gives the Transaction block(16bits) & Timer
                         Name(12bits) & Module ID(4bits). First 4 bits module
                         and next 12 bits timer name and last 16 bits TXN Id

              ulTimerId - Id of timer, which is timed out. This id is the output
                          parameter of SipStartTimer.
 Output     : None

 Return     : VOID
*******************************************************************************/
SS_VOID SipTxnTimeoutHandler
(
   IN SS_UINT32 ulParams,
   IN SS_UINT32 ulTimerId
);
/*End <Tag.YDHD02398> */
/*******************************************************************************
 Function   : SipTxnUtilGenTag

 Description: This function will generate the tag, this function can be used
              to generate To tag and From tag

 Input      : hdlMemCp - pointer to the MemCp, from where memory for Tag
                         will be allocated
              pstrTag - the caller should not allocate the memory for pcData in
                        pstrTag, pcData in pstrTag can be NULL, and ulLen
                        can be 0

 Output     : pstrTag - Generated Tag, this API will allocate memory for pcData
                        in pstrTag, and update the ulLen with allocated memory
                        length

 Return     : Success/Failure code
*******************************************************************************/
EN_SIP_RESULT SipTxnUtilGenTag
(
    IN SipMemCpHdl  hdlMemCp,
    IO SipString   *pstrTag
);


/*******************************************************************************
 Function   : SipTxnUtilGenBranchId

 Description: This function will generate the 3261 BranchId, which will contain
              the 3261 magic cookie "z9hG4bK"

 Input      : hdlMemCp - pointer to the MemCp, from where memory for BranchID
                         will be allocated
              pstrBranchId - the caller should not allocate the memory for
                             pcData in BranchID, pcData in pstrBranchId can be
                             NULL, and ulLen can be 0

 Output     : pstrBranchId - Generated BranchID, this API will allocate memory
                             for pcData in BranchID, and update the ulLen with
                             allocated memory length

 Return     : Success/Failure code
*******************************************************************************/
EN_SIP_RESULT SipTxnUtilGenBranchId
(
    IN SipMemCpHdl  hdlMemCp,
    IO SipString   *pstrBranchId
);

/*******************************************************************************
 Function   : SipTxnUtilCopyCallPrefHeaders

 Description: This function will copy the call preference headers
              Accept-Contact, Reject-Contact, and Request-Disposition headers
              from Src SipMsg to Dest SipMsg.

 Input      : pstSrcMsg - message from which call pref headers to be copied

 Output     : pstDestMsg - message to which call pref headers to be copied

 Return     : Success/Failure code
*******************************************************************************/
EN_SIP_RESULT SipTxnUtilCopyCallPrefHeaders
(
    IN SipMsgS *pstSrcMsg,
    IO SipMsgS *pstDestMsg
);


#ifdef __cplusplus
}
#endif

#endif /* _SS_TXN_UTIL_X_ */

