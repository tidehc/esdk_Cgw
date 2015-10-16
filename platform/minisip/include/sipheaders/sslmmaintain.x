/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName   : sslmmaintain.x

    Author     : Srinivas

    Version    : 1.0

    Date       : 2005-09-09

    Description: This header file contains the API functions provided by the
                 stack for the stack users to maintain the stack.

    Function List:
        1. SipLmTxnGetNumOfUsedTxnCbs
        2. SipLmTxnGetNumOfFreeTxnCbs
        3. SipLmTxnDumpTxnblk
        4. SipLmTxnGetTxnFSMStatus
        5. SipLmResetCoreLib


    History:
        1. Date         : <YYYY-MM-DD>
           Tag          :
           Author       :
           Modification :
******************************************************************************/

#ifndef _SS_LM_MAINTAIN_X_
#define _SS_LM_MAINTAIN_X_

#ifdef __cplusplus
extern "C" {
#endif


#include "ssdatatype.h"
#include "sssipbase.h"

#ifdef SS_MAINTAIN

/*****************************************************************************
 Function    : SipLmTxnGetNumOfUsedTxnCbs

 Description : This function will return the number of used Txn Cbs.

 Input       : None.

 Output      : None.

 Return      : Number of used elements in Txn Table.
*******************************************************************************/
SS_UINT32 SipLmTxnGetNumOfUsedTxnCbs
(
    SS_VOID
);

/*****************************************************************************
 Function    : SipLmTxnGetNumOfFreeTxnCbs

 Description : This function will return the number of free Txn Cbs.

 Input       : None.

 Output      : None.

 Return      : Number of free elements in Txn Table.
  *****************************************************************************/
SS_UINT32 SipLmTxnGetNumOfFreeTxnCbs
(
    SS_VOID
);

/*****************************************************************************
 Function    : SipLmTxnDumpTxnblk

 Description : This function is called to dump the transaction block.

 Input       : ulTxnObjId - The object id of the transaction.

 Output      : pulTxnBlkSize - Pointer to Txn Blk size.

 Return      : Pointer to the Txn Block is type casted to SS_UCHAR *is retunred.
 *******************************************************************************/
SS_UCHAR * SipLmTxnDumpTxnblk
(
    IN SS_UINT32 ulTxnObjId,
    OUT SS_UINT32 *pulTxnBlkSize
);

/*****************************************************************************
 Function    : SipLmTxnGetTxnFSMStatus

 Description : This function will return the transaction FSM Status.

 Input       : ulTxnObjId - The object Id of the transaction.

               ulTuObjId  - The object of the TU which corresponds to this
                            transaction.

 Output      : None.

 Return      : Return the Status of the Transaction FSM.
 *******************************************************************************/

SS_UINT32  SipLmTxnGetTxnFSMStatus
(
    IN SS_UINT32 ulTxnObjId,
    IN SS_UINT32 ulTuObjId
);
/*****************************************************************************
 Function    : SipLmResetCoreLib

 Description : This function will Free All the used transaction blocks
               and related resources forcefully.

 Input       : None.

 Output      : None.

 Return      : SS_VOID.
 *******************************************************************************/

SS_VOID  SipLmResetCoreLib
(
    SS_VOID
);

#endif  /*#ifdef SS_MAINTAIN */


#ifdef __cplusplus
}
#endif

#endif

