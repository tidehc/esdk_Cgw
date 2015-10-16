/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName: ssdsmmsgcloneutil.x

    Author:Lei
    Version:1.0

    Date: 2005-11-17
    Description: This file provides Clone functionality APIs on Ext,Codec Failed
                 & Unknown headers

    Function List:

    History:
        1. Date:
        Defect:
        Tag: <MOD.001>
        Author:
        Modification:

*******************************************************************************/
#ifndef __SSDSMMSGCLONEUTIL_X__
#define __SSDSMMSGCLONEUTIL_X__

#include "sssipbase.h"
#include "ssmessage.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
    Function Name : SipCopyExtHdrToMsg

    Description  :This function clones extension header and insert into the
                  Extesnsion hdr List of the Msg.

    Inputs      :  pstExtHdrs - The Extension header

    Output      :  pstSipMsg - The SIP Message where the headers are copied

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If Ext hdr list is successfully copied
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCloneExtHdrToMsg
(
    IN   SipHdrIdValue  *pstExtHdr,
    OUT  SipMsgS        *pstSipMsg
);

/******************************************************************************
    Function Name : SipDsmCopyExtHdrListToMsg

    Description  :This function Copies the given Extension hdr List
                   into the output Sip Msg.

    Inputs      :   hdlExtHdrList - Extension hdr List Handle.

    Output      : pstSipMsg - The SIP Message where the headers are copied

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If Ext hdr list is successfully copied
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCopyExtHdrListToMsg
(
    IN  SipHeaderListHdl hdlExtHdrList,
    OUT SipMsgS         *pstSipMsg
);

/******************************************************************************
    Function Name : SipCopyExtHdrs

    Description  :This function Copies the Ext Header List from
                  Input Sip Msg  to  Output Sip Msg.

    Inputs      :  pstInSipMsg - The source sip message

    Output      :  pstOutSipMsg - The destination sip message

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If Ext hdr is successfully copied
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCloneExtHdrListToMsg
(
    IN  SipMsgS *pstInSipMsg,
    OUT SipMsgS *pstOutSipMsg
);

/******************************************************************************
    Function Name : SipDsmCloneUnknownHdrToMsg

    Description  : This function Clones the Unknown Header and insert into the
                   un known hdr list in the msg.

    Inputs      :  pstUnknownHdr - The Unknown header list

    Output      :  pstSipMsg - The SIP Message where the headers are copied

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If Ext hdr list is successfully copied
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCloneUnknownHdrToMsg
(
    IN   SipHdrNameValue *pstUnknownHdr,
    OUT  SipMsgS         *pstSipMsg
);

/******************************************************************************
    Function Name : SipDsmCopyUnknownHdrListToMsg

    Description  :This function Copies the Unknown Hdr List into the
                  given output sip msg.

    Inputs      :   hdlUnknownHdrList - handle to unknwon hdr List

    Output      : pstSipMsg - The SIP Message where the headers are copied

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If Ext hdr list is successfully copied
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCopyUnknownHdrListToMsg
(
    IN  SipHeaderListHdl hdlUnknownHdrList,
    OUT SipMsgS         *pstSipMsg
);

/******************************************************************************
    Function Name : SipDsmCloneUnknownHdrListToMsg

    Description  :This function clones  the Unknown Hdr List from the input
                  Sip Message  into the output SipMessage.

    Inputs      :  pstInSipMsg - The source sip message

    Output      :  pstOutSipMsg - The destination sip message

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If Ext hdr is successfully copied
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCloneUnknownHdrListToMsg
(
    IN  SipMsgS *pstInSipMsg,
    OUT SipMsgS *pstOutSipMsg
);

/******************************************************************************
    Function Name : SipCopyDecFailedHdrToMsg

    Description  :This function Clones the Decode Failed Hdr and inserts
                  into the Decode Failed hdr List of the Message.

    Inputs      :  pstDecFailedHdr - The DecFailed header list

    Output      :  pstSipMsg - The SIP Message where the headers are copied

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If Ext hdr list is successfully copied
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCloneDecFailedHdrToMsg
(
    IN   SipHdrNameValue *pstDecFailedHdr,
    OUT  SipMsgS         *pstSipMsg
);

/******************************************************************************
    Function Name : SipDsmCopyDecFailedHdrListToMsg

    Description  :This function Copies the Decode Failed Hdr List
                  into the output Sip Message.

    Inputs      :   hdlDecFailedHdrList - Decode Failed Hdr List

    Output      : pstSipMsg - The SIP Message where the headers are copied

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If Ext hdr list is successfully copied
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCopyDecFailedHdrListToMsg
(
    IN  SipHeaderListHdl hdlDecFailedHdrList,
    OUT SipMsgS         *pstSipMsg
);

/******************************************************************************
    Function Name : SipDsmCloneDecFailedHdrListToMsg

    Description  :This function Clones the DecFailed Hdr List from the input
                  Sip Message to the output Sip Message.

    Inputs      :  pstInSipMsg - The source sip message

    Output      :  pstOutSipMsg - The destination sip message

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If Ext hdr is successfully copied
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCloneDecFailedHdrListToMsg
(
    IN  SipMsgS *pstInSipMsg,
    OUT SipMsgS *pstOutSipMsg
);

/******************************************************************************
    Function Name : SipDsmCloneMsg

    Description  :This function Clone one message

    Inputs      :  pstSipOrigMsg - The source sip message

    Output      :  ppstSipCopiedMsg - The destination sip message

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If Ext hdr is successfully copied
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCloneMsg
(
    IN  SipMsgS  *pstSipOrigMsg,
    OUT SipMsgS **ppstSipCopiedMsg
);

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif /* __SSDSMMSGCLONEUTIL_H__*/

