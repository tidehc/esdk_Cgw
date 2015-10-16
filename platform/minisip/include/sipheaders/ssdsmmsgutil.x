/*********************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName: ssdsmmsgutil.x

    Author:Lei
    Version:1.0

    Date: 2005-11-17
    Description: This file provides message utility APIs to

    Function List:
            1.
    History:
        1. Date:
        Tag:
        Defect:
        Author:
        Modification:
**********************************************************/
#ifndef __SSDSM_MSG_UTIL_X
#define __SSDSM_MSG_UTIL_X

#include "sssipbase.h"
#include "ssmessage.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SS_DSM_MSG_BUFFER_SIZE   512
#define SS_DSM_MSG_BUFFER_GROWTH 128

/******************************************************************************
    Function Name : SipDsmCreateCSeqInMsg

    Description   : This API creates one CSeq header based on the inputted method
                    and copy it into message

    Input(s)      : pstMethod - The pointer to the method
                    pstSipMsg - The SIP Message

    Output(s)     : pstSipMsg - The updated SIP Message

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
EN_SIP_RESULT SipDsmCreateCSeqInMsg
(
    IN SipToken  *pstMethod,
    IO SipMsgS   *pSipMsg
);


/******************************************************************************
    Function Name : SipDsmCreateRequestLine

    Description   : This API creates request line in the sip message

    Input(s)      : pstSipMsg - The SIP Message

    Output(s)     : pstSipMsg - The updated SIP Message

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
EN_SIP_RESULT   SipDsmCreateRequestLine
(
    IO SipMsgS *pSipMsg
);

/******************************************************************************
    Function Name : SipDsmCreateStatusLine

    Description   : This API creates status line in the sip message

    Input(s)      : pstSipMsg - The SIP Message

    Output(s)     : pstSipMsg - The updated SIP Message

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
EN_SIP_RESULT   SipDsmCreateStatusLine
(
    IO SipMsgS *pSipMsg
);

/******************************************************************************
    Function Name : SipDsmCopyMsgRef

    Description   : This API copy message reference count to avoid frequent memroy
                    copy

    Input(s)      : pSipOrigMsg - The SIP Message

    Output(s)     : ppSipCopiedMsg - The updated SIP Message

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
EN_SIP_RESULT SipDsmCopyMsgRef
(
    IN  SipMsgS *pSipOrigMsg,
    OUT SipMsgS **ppSipCopiedMsg
);

/******************************************************************************
    Function Name : SipDsmReleaseMsgRef

    Description   : This API minus the reference count of sip message. If the
                    count number is 0, the message should be freed actually.

    Input(s)      : ppSipMsg - The pointer to the memory storing SIP Message

    Output(s)     : ppSipMsg - The pointer to the memory storing SIP Message

    Returns       : SS_VOID
******************************************************************************/
SS_VOID SipDsmReleaseMsgRef
(
    IO SipMsgS **ppSipMsg
);

/******************************************************************
    Function Name : SipDsmCreateRspMsgFromReqMsg

    Description   : This function will create SIP response Msgs.

    Input(s)      : RespCode   : Response Code for which mesasge is to be
                                 generated.
                    pInpSipMsg : Input Sip Request for which Response is
                                 to be created.

    Output(s)     : ppOutSipRespMsg : Created Response Message.

    Returns       : EN_SIP_RESULT - Return code enum
                    success or failure or some other return code
******************************************************************/
EN_SIP_RESULT SipDsmCreateRspMsgFromReqMsg
(
    IN  SS_UINT32   ulRespCode,
    IN  SipMsgS     *pInpSipMsg,
    OUT SipMsgS     **ppOutSipRespMsg
);

/******************************************************************
    Function Name : SipDsmCreateMsg

    Description   : This function will create SIP Msgs.

    Input(s)      : ppSipMsg - The pointer to the created sip message

    Output(s)     : ppstSipMsg - The pointer to the created sip message

    Returns       : EN_SIP_RESULT - Return code enum
                    success or failure or some other return code
******************************************************************/
EN_SIP_RESULT SipDsmCreateMsg
(
    IO SipMsgS **ppSipMsg
);

/*******************************************************************************
    Function Name : SipDsmCreateReqMsg

    Description   : This function will create request Msgs.

    Input(s)      : pstMethod - The create message uses it to start line
                                and CSeq header

    Output(s)     : ppstSipRequest - The pointer to the created sip message

    Returns       : EN_SIP_RESULT - Return code enum
                    success or failure or some other return code
*******************************************************************************/
EN_SIP_RESULT SipDsmCreateReqMsg
(
    IN  SipToken *pstMethod,
    OUT SipMsgS **ppstSipRequest
);

/******************************************************************
    Function Name : SipDsmCreateRspMsg

    Description   : This function will create Resonse Msg.

    Input(s)      : iRespCode - The response code to the message
                    ppSipMsg  - The pointer to the created sip message
                    pstMethod - The create message uses it to start line
                                and CSeq header

    Output(s)     : ppSipMsg

    Returns       : EN_SIP_RESULT - Return code enum
                    success or failure or some other return code
******************************************************************/
EN_SIP_RESULT SipDsmCreateRspMsg
(
    IN  SS_INT     iRespCode,
    IN  SipToken  *pstMethod,
    OUT SipMsgS  **ppSipResp
);

/*Later consider if we can remove this fucntion*/
/******************************************************************
    Function Name : SipDsmCreteSDU

    Description   : This function will create request Msgs.

    Input(s)      : pSipSDU - The constructed SDU

    Output(s)     : pSipSDU
                    pstHdlMemCpInSDU - The handle of memcp in the SDU

    Returns       : EN_SIP_RESULT - Return code enum
                    success or failure or some other return code
******************************************************************/
EN_SIP_RESULT SipDsmConstructSDU
(
    IO  SipDataUnitS  *pSipSDU,
    OUT SipMemCpHdl   *pstHdlMemCpInSDU
);

/*******************************************************************************
    Function Name : SipDsmInitSdu

    Description   : This function initializes the input SDU.

    Input(s)      : pstSdu : Input Sip Request for which Response is
                                 to be created.

    Output(s)     : pstSdu : Created Response Message.

    Returns       : EN_SIP_RESULT - Return code enum
                    success or failure or some other return code
*******************************************************************************/
SS_VOID SipDsmInitSdu
(
    IO SipDataUnitS *pstSdu
);

/******************************************************************************
    Function Name : SipDsmGetHdrFromMsg

    Description   : This API retrieves a handle to the header from the message

    Input(s)      : usHdrId   - Header ID
                    pstSipMsg - The SIP Message

    Output(s)     : The handle to the SIP Header within the message

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
SS_BOOL SipDsmGetHdrPresenceInMsg
(
    IN SS_UINT16 usHdrId,
    IN SipMsgS  *pstSipMsg
);

/******************************************************************
    Function Name : SipDsmSetBody

    Description   : This function will set SIPBody to sip message.

    Input(s)      : hdlBody - The handle of SIPBody
                    bIsClone - If it's true, the function will clone the body,
                    then set into the message. Or it will set into message
                    using the inputted body handle without deep copying.
    Output(s)     : pSipMsg - Expected to accept the set BODY

    Returns       : EN_SIP_RESULT - Return code enum
                    success or failure or some other return code
******************************************************************/
EN_SIP_RESULT SipDsmSetBody
(
    IO SipMsgS *pSipMsg,
    IN SipBody  hdlBody,
    IN SS_BOOL  bIsClone
);

/******************************************************************
    Function Name : SipDsmGetBody

    Description   : This function will get SIPBody from sip message.

    Input(s)      : pSipMsg - The sip message containing the SIPBody

    Output(s)     : None

    Returns       : EN_SIP_RESULT - Return code enum
                    success or failure or some other return code
******************************************************************/
SipBody SipDsmGetBody
(
    IN SipMsgS *pSipMsg
);

/******************************************************************
    Function Name : SipDsmCloneBody

    Description   : This function will clone one SIPBody to the destination.

    Input(s)      : hdlBody - The source body

    Output(s)     : pstHdlClonedBody - The cloned body

    Returns       : EN_SIP_RESULT - Return code enum
                    success or failure or some other return code
******************************************************************/
EN_SIP_RESULT SipDsmCloneBody
(
    IN  SipBody   hdlBody,
    OUT SipBody  *pstHdlClonedBody
);

/******************************************************************
    Function Name : SipDsmReleaseBody

    Description   : This function will release SIPBody.

    Input(s)      : pstHdlBody - The pointer of sip body

    Output(s)     : None

    Returns       : SS_VOID
******************************************************************/
SS_VOID SipDsmReleaseBody
(
    IO SipBody *pstHdlBody
);

#ifdef __cplusplus
}
#endif

#endif
