/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName    : sscodec.x

    Author      : Manjunath S Warad

    Version     : 1.0

    Date        : 2005-12-12

    Description : This file provides the Codec interfaces.

    Function List:
        1. SipDecHeader
        2. SipDecHostPort
        3. SipDecUri
        4. SipDecMsgStr
        5. SipDecMsgStrBuff
        6. SipDecFragMsgStr
        7. SipCodecPeekMsg
        8. SipEncHeader
        9. SipEncMsgStr
        10.SipEncMsgStrBuff

    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX
*******************************************************************************/

#ifndef __SS_CODEC_X__
#define __SS_CODEC_X__

#include "ssenums.h"
#include "sssipbase.h"
#include "sscbbtypes.x"
#include "ssheaders.h"
#include "ssmessage.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
    Function    : SipDecHeader

    Description : This API can be used to Decode individual Headers.

    Input(s)    : usHdrId - Header Id eg:- HEADER_ID_VIA

                  hdlMemCp - The Memoy control pointer that is to be used for
                           allocating and deallocating memory (Buffer Memory)


                  pstrHdr - Input character string (doesnot include
                            HeaderName: eg:- "Via:")
                           (eg:- "SIP/2.0/UDP 10.78.233.247:5060;branch=z9hG4bK,
                            SIP/2.0/UDP 10.78.233.247:5060;branch=z9hG4bK\r\n")
                  pvHdrVal - Reference to the destination structure.

    Output      : pvHdrVal - Reference to the destination structure.
                             (eg:- ViaList )

                  pulBytesAborbed - The no of bytes absorbed by the decoder

    Return      : SIP_RET_SUCCESS or Error Code
*******************************************************************************/
EN_SIP_RESULT SipDecHeader
(
    IN  SS_UINT16       usHdrId,
    IN  SipMemCpHdl     hdlMemCp,
    IN  SipString      *pstHdr,
    IO  SS_VOID        *pvHdrVal,
    OUT SS_UINT32      *pulBytesAborbed
);

/*******************************************************************************
    Function    : SipDecHostPort

    Description : This API parses the HostPort information present in the
                  pstrBuffer and stores into pstHostPort.The ulBytesAborbed
                  parameter is optional ,it can be a NULL pointer.

    Input(s)    : hdlMsgCb - The Memory control pointer that is to be used for
                             allocating and deallocating memory.

                  pstrBuffer - Input character string.

                  pstHostPort - Reference to the destination structure .

                  pulBytesAbsorbed - This contains the no.of bytes absorbed

    Output(s)   : pstHostPort - Reference to the destination structure .

                  pulBytesAbsorbed - This contains the no.of bytes absorbed

    Returns     : SIP_RET_SUCCESS or Error Code
*******************************************************************************/
EN_SIP_RESULT SipDecHostPort
(
    IN  SipMemCpHdl    hdlMemCp,
    IN  SipString     *pstrBuffer,
    IO  SipHostPort   *pstHostPort,
    IO  SS_UINT32     *pulBytesAbsorbed
);

/*******************************************************************************
    Function    : SipDecUri

    Description : This API parses the URI information present in the pstrBuffer
                  and stores into pstURI.The ulBytesAborbed parameter is
                  optional ,it can be a NULL pointer.

    Input(s)    : hdlMsgCb - The Memory control pointer that is to be used for
                             allocating and deallocating memory

                  pstrBuffer - Input character string.

                  pstURI - Reference to the destination structure .

    Output(s)   : pstURI - Reference to the destination structure .

                  pulBytesAbsorbed - This contains the no.of bytes absorbed.

    Returns     : SIP_RET_SUCCESS or Error Code

    Remarks     : Decodes URI of the form sip:user:pass@host.com:port;lr i.e.,
                  if the URI with angle quotes cannot be decoded using this API.
*******************************************************************************/
EN_SIP_RESULT SipDecUri
(
    IN  SipMemCpHdl  hdlMemCp,
    IN  SipString   *pstrBuffer,
    IO  URI         *pstURI,
    IO  SS_UINT32   *pulBytesAbsorbed
);

/*******************************************************************************
     Function   : SipDecMsgStr

     Description: This function will be used to Parse the SIP Message present in
                  the pstrMsgBuffer as continuous string.The ulBytesAborbed
                  parameter is optional ,it can be a NULL pointer.

     Input(s)   : pstrMsgStr - Input character string buffer containing
                               text SIP message.

                  pstSipMsg - Reference to the destination structure.

     Output(s)  : pstSipMsg - Reference to the destination structure.

                  pulBytesAbsorbed - This contains the no.of bytes absorbed

     Return     : SIP_RET_SUCCESS or Error Code

     Remarks    : Uses pstMemCp which present in pstSipMsg
*******************************************************************************/
EN_SIP_RESULT SipDecMsgStr
(
    IN  SipString   *pstrMsgStr,
    IO  SipMsgS     *pstSipMsg,
    IO  SS_UINT32   *pulBytesAbsorbed
);

/*******************************************************************************
     Function   : SipDecMsgStrBuff

     Description: This function will be used to Parse the SIP Message present in
                  the pstrMsgBuffer as string-buffer.The ulBytesAborbed parameter
                  is optional ,it can be a NULL pointer.

     Input(s)   : hdlMsgStrBuffer - Input character string buffer containing
                                     text SIP message.

                  pstSipMsg - Reference to the destination structure.

                  pulBytesAbsorbed - This contains the no.of bytes absorbed.

     Output(s)  : pstSipMsg - Reference to the destination structure.

                  pulBytesAbsorbed - This contains the no.of bytes absorbed.

     Return     : SIP_RET_SUCCESS or Error Code

     Remarks    : Uses pstMemCp which present in pSIPMessage
*******************************************************************************/
EN_SIP_RESULT SipDecMsgStrBuff
(
    IN  SipStrBufHdl     hdlMsgStrBuffer,
    IO  SipMsgS         *pstSipMsg,
    IO  SS_UINT32       *pulBytesAbsorbed
);

/*******************************************************************************
     Function   : SipDecFragMsgStr

     Description: This function will be used to Parse the SIP Frag Message
                  present in the "message"

     Input(s)   : pstrMsgStr - Input character string buffer containing
                               text SIP message.

     Output     : pstSipMsg - Reference to the destination structure

                  pulBytesAbsorbed - This contains the no.of bytes absorbed

     Return     : SIP_RET_SUCCESS or Error Code

*******************************************************************************/
EN_SIP_RESULT SipDecFragMsgStr
(
    IN  SipString  *pstrMsgStr,
    IO  SipMsgS    *pstSipMsg,
    IO  SS_UINT32  *pulBytesAbsorbed
);

/*******************************************************************************
    Function Name : SipCodecPeekMsg

    Description   : This API finds the whole length (SipMessageLenght+BodyLength)
                    of raw Sip-Message in text format.

    Input(s)      : hdlMsgStrBuffer - Input character string buffer containing
                                      text SIP message.

    Output(s)     : EN_SIP_MSG_PEEK_INFO - Will contain the following

                    SIP_MSG_PEEK_INFO_COMPLETE - The pstMsgCb contains  entire
                                                 SIP-message and entire Body but
                                                 may or may not have some
                                                 excesses part.

                    SIP_MSG_PEEK_INFO_INCOMPLETE - The pstMsgCb contains
                                                   incomplete message i.e it
                                                   still does not have the
                                                   whole message .

                    SIP_MSG_PEEK_INFO_UNKNOWN - Cannot determine the whole
                                                length as Content-Length header
                                                is not found and end of
                                                SIP-message in not found.

                    ulMsgLen - Whole length of Sip Message.

    Returns       : Failure - If End-of-message found without ContentLength
                              header else success.
******************************************************************************/
EN_SIP_RESULT SipCodecPeekMsg
(
    IN   SipStrBufHdl            hdlMsgStrBuffer,
    IO   EN_SIP_MSG_PEEK_INFO   *penMsgPeekInfo,
    IO   SS_UINT32              *pulMsgLen
);

/*******************************************************************************
    Function   : SipEncHeader

    Description: This API can be used to encode individual Headers.

    Input(s)   : usHdrId - Header Id eg:- HEADER_ID_VIA

                 pvHdrVal - Input Header structure

    Output     : phdlHdrBuffer - Output encoded character string

    Return     : SIP_RET_SUCCESS or Error Code
*******************************************************************************/
EN_SIP_RESULT SipEncHeader
(
    IN  SS_UINT16       usHdrId,
    IN  SS_VOID        *pvHdrVal,
    IO  SipStrBufHdl    hdlHdrBuffer
);

/*******************************************************************************
    Function Name : SipEncMsgStr

    Description : This is function is used to encode SIP message structure to a
                  byte buffer. This api can also be used to encode Sip Frag
                  message.

    Input(s)    : pSipMsg- the pointer to SipMsgS structure to be encoded

                  pEncMsgStr - pointer to the String which
                               contains the encoded string.

                  pulBytesEncoded - How many bytes already be encoded.

    Output(s)   : pEncMsgStr - pointer to the String which
                               contains the encoded string.

                  pulBytesEncoded - How many bytes already be encoded.

    Returns     : SS_RET_SUCCESS or Error code
*******************************************************************************/
EN_SIP_RESULT SipEncMsgStr
(
    IN SipMsgS     *pstSipMsg,
    IO SipString   *pEncMsgStr, /* pre-allocate memory */
    IO SS_UINT32   *pulBytesEncoded
);

/*******************************************************************************
    Function Name : SipEncMsgStrBuff

    Description : This is the wrapper function that encodes a complete SIP
                  message structure to a byte buffer.

    Input(s)    : pstSipMsg- the pointer to SipMsgS structure to be encoded

    Output(s)   : hdlEncMsg - pointer to the String-Buffer which
                                   contains the encoded string

    Returns     :SS_RET_SUCCESS or Error code
*******************************************************************************/
EN_SIP_RESULT SipEncMsgStrBuff
(
    IN SipMsgS      *pstSipMsg,
    IO SipStrBufHdl  hdlEncMsg
);

#ifdef __cplusplus
}
#endif

#endif/*__SS_CODEC_X__*/

