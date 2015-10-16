/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName   : ssmultipartbodycodec.x

    Author     : Sajeesh

    Version    : 1.0

    Date       : 2005-12-05

    Description: This file provides the interface functions between the MPB
                  component and the SIP Stack User.
    Function List:
        1.SipMpbParseMultipartBody
        2.SipMpbEncodeMultipartBody
        3.SipMpbFreeSinglePartBody
        4.SipMpbFreeMultipartBody
        5.SipMpbLocateBoundary
        6.SipMpbClearSIPContentHeaders
        7.SipMpbFreeSIPContentHeaders
        8.SipMpbCopySIPContentHeaders
        9.SipMpbCopySIPContentHeadersFromMsg
       10.SipMpbCloneSIPContentHeadersFromMsg
       11.SipMpbDecodeContentHeaders


    History:
        1. Date        : <YYYY-MM-DD>
           Tag         :
           Author      :
           Modification:
*******************************************************************************/

#ifndef __MULTIPARTBODYCODEC_X__
#define __MULTIPARTBODYCODEC_X__

#include "ssmessage.h"
#include "sshdrtokengenparamsclass.h"
#include "ssheaders.h"
#include "sshdrstringclass.h"
#include "sshdrtokenclass.h"
#include "sshdrcntnttypeclass.h"

#ifdef  __cplusplus
extern "C" {
#endif


#define SS_MIME_BDRY_MAX_LEN         (70)  /* This length defined in RFC2046 */
#define SS_MPB_SB_SIZE 512

typedef struct _SIPContentHeaders
{
    SipContentDisposition    *pstContentDisposition;
    SipContentEncodingListHdl hdlContentEncoding;
    SipContentLanguageListHdl hdlContentLanguage;
    SipContentLength         *pstContentLength;
    SipContentType           *pstContentType;
} SIPContentHeaders;

typedef struct _SinglepartBodyS
{
    /* The content headers associated with  this body part */

    SIPContentHeaders *pstContentHdrs;

    /* The data of the body represented in String format */

    SipString          strBodyData;
}SinglepartBodyS;

typedef struct _MultipartBodyS
{

    /* Number of single part bodies present in this body part */

    SS_UINT32                ulNoOfSinglePartBody;

    /* The single part bodies present in this body part */

    SinglepartBodyS        **ppstSpBodies;

}MultipartBodyS;



/******************************************************************************
 Function    : SipMpbParseMultipartBody

 Description : This API splits a multipart body into its components

 Input(s)    : hdlMemCp    - Memory control pointer used to allocate memory for
                             this
               pstrBody    - Pointer to Message Body string
               pstrBdry    - Pointer to the boundary of  body

 Output(s)   : ppstMPBody - Pointer to the list of parsed multipart body
                            components

 Returns     : SS_RET_SUCCESS on successful parsing or else error Code
******************************************************************************/

EN_SIP_RESULT SipMpbParseMultipartBody
(
    IN SipMemCpHdl       hdlMemCp,
    IN SipString        *pstrBody,
    IN SipString        *pstrBdry,
    OUT MultipartBodyS **ppstMPBody
);

/******************************************************************************
 Function    : SipMpbEncodeMultipartBody

 Description : This API Encodes a multipart body structure into SIP body

 Input(s)    : hdlMemCp      - Memory control pointer used to allocate memory
                               for this
               pstMpb        - Pointer to multipart body structure

 Output(s)   : phdlMPBody - pointer to the encoded SIP message body
               ppstrBdry     - Pointer to the boundary of body

 Returns       : SS_RET_SUCCESS on successful encoding or else SS_RET_FAILURE
******************************************************************************/

EN_SIP_RESULT SipMpbEncodeMultipartBody
(
    IN  SipMemCpHdl      hdMemCp,
    IN  MultipartBodyS  *pstMpb,
    OUT SipString      **ppstrBdry,
    OUT SipBody         *phdlMPBody
);

/******************************************************************************
 Function    : SipMpbFreeSinglePartBody

 Description : This API releases the single part body. It does not release the
               outer pointer

 Input(s)    : hdlMemCp - Memory control pointer used to allocate memory for
                          this
               ppstBody - The pointer to the single part body whose memory has
                          to be released

 Output(s)   :  None

 Returns     :  None
******************************************************************************/

SS_VOID SipMpbFreeSinglePartBody
(
    IN SipMemCpHdl       hdlMemCp,
    IN SinglepartBodyS **ppstBody
);

/******************************************************************************
 Function    : SipMpbFreeMultipartBody

 Description : This API releases the multi part body
               It releases all the internal single part bodies and multipart
               bodies present within this structure.It does not release the
               outer pointer

 Input(s)    : hdlMemCp - Memory control pointer used to allocate memory for
                          this
               ppstBody - The pointer to the multipart body whose memory has to
                          be released

 Output(s)   : None

 Returns     : None
******************************************************************************/

SS_VOID SipMpbFreeMultipartBody
(
    IN SipMemCpHdl       phdlMemCp,
    IN MultipartBodyS  **ppstBody
);

/******************************************************************************
 Function    : SipMpbLocateBoundary

 Description : This API locates the boundary of multi part body

 Input(s)    : pucStr    -  Pointer to the multipart body string
               ulStrLen  -  Length of the multipart body string
               pucBdry   -  Pointer to the string of boundary
               ulBdryLen -  Length of the boundary string
               bHyphPres -  Flag to indicate whether to check '--'

 Output(s)   : pulBdryPos-  Pointer to the boundary position

 Returns     : The result of locating boundary
               If SS_RET_SUCCESS, it has succeed to locate the boundary
               Otherwise, it hasn't located the boundary
******************************************************************************/

EN_SIP_RESULT SipMpbLocateBoundary
(
    IN SS_UCHAR         *pucStr,
    IN SS_UINT32         ulStrLen,
    IN SS_UCHAR         *pucBdry,
    IN SS_UINT32         ulBdryLen,
    IN SS_BOOL           bHyphPres,
    OUT SS_UINT32       *pulBdryPos
);

/******************************************************************************
    Function Name : SipMpbClearSIPContentHeaders

    Description   : This function clears the SIP Content headers structure

    Input(s)      : hdlMemCp   - Memory control handle
                    pstCntHdrs - Pointer to the SIPContentHeaders structure

    Output(s)     : pstCntHdrs - Pointer to the SIPContentHeaders structure
                                 after clearing

    Returns       : None
******************************************************************************/

SS_VOID SipMpbClearSIPContentHeaders
(
    IN  SipMemCpHdl          hdlMemCp,
    IO  SIPContentHeaders   *pstCntHdrs
);

/******************************************************************************
    Function Name : SipMpbFreeSIPContentHeaders

    Description   : This function frees the SIP Content headers structure and
                    it contents

    Input(s)      : hdlMemCp  - Memory Control handle
                    ppstCntHdrs  - Pointer to the SIPContentHeaders structure
                                   pointer which is to be freed
    Output(s)     : None

    Returns       : None
******************************************************************************/

SS_VOID SipMpbFreeSIPContentHeaders
(
    IN  SipMemCpHdl         hdlMemCp,
    IN SIPContentHeaders  **ppstCntHdrs
);

/******************************************************************************
    Function Name : SipMpbCopySIPContentHeaders

    Description   : This function copies the contents of SIPContent Headers
                    structure wrapper

    Input(s)      : hdlMemCp      - Memory Control handle

                    pstSrcCntHdrs - the source structure

    Output(s)     : pstDstCntHdrs - the destination structure

    Returns       : SIP_RET_SUCCESS or any error code defined in EN_SIP_RESULT
******************************************************************************/

EN_SIP_RESULT SipMpbCopySIPContentHeaders
(
    IN  SipMemCpHdl          hdlMemCp,
    IN  SIPContentHeaders   *pstSrcCntHdrs,
    OUT SIPContentHeaders   *pstDstCntHdrs
);

/******************************************************************************
    Function Name : SipMpbCopySIPContentHeadersFromMsg

    Description   : This function copies the contents of SIPContent Headers
                    structure from SipMessage

    Input(s)      : hdlMemCp  - Memory Control handle
                    pstSipMsg - SipMessage

    Output(s)     : pstDstCntHdrs - handle to the created destination structure

    Returns       : Returns SIP_RET_SUCCESS or any error code defined in
                    EN_SIP_RESULT
******************************************************************************/

EN_SIP_RESULT SipMpbCopySIPContentHeadersFromMsg
(
    IN  SipMemCpHdl          hdlMemCp,
    IN  SipMsgS             *pstSipMsg,
    OUT SIPContentHeaders   *pstDstCntHdrs
);

/******************************************************************************
    Function Name : SipMpbCloneSIPContentHeadersFromMsg

    Description   : This function Clone the contents of SIPContent Headers
                    structure from SipMessage

    Input(s)      : hdlMemCp  - Memory Control handle
                    pstSipMsg -SipMessage

    Output(s)     : ppstDstCntHdrs - handle to the created destination structure

    Returns       : SIP_RET_SUCCESS or any error code defined in EN_SIP_RESULT
******************************************************************************/

EN_SIP_RESULT SipMpbCloneSIPContentHeadersFromMsg
(
    IN  SipMemCpHdl          hdlMemCp,
    IN  SipMsgS             *pstSipMsg,
    OUT SIPContentHeaders  **ppstDstCntHdrs
);

/*******************************************************************************
    Function Name : SipMpbDecodeContentHeaders

    Description   : This funtion decodes the content headers.

    Input(s)      : hdlMemCp - Handle to MemCp
                    pucData  - The string contiaining the input content headers
                               in string format
                    ulLen    - The length of the input string.

    Output(s)     : pulBytesAbsorbed - The number of bytes absorbed in the input
                                       string during decoding
                    pstContentHdrs   - The output reference to the content
                                       header structure

    Returns       : SIP_RET_SUCCESS or Error Code
*******************************************************************************/

EN_SIP_RESULT SipMpbDecodeContentHeaders
(
    IN  SipMemCpHdl         hdlMemCp,
    IN  SS_UCHAR           *pucData,
    IN  SS_UINT32           ulLen,
    OUT SS_UINT32          *pulBytesAbsorbed,
    OUT SIPContentHeaders  *pstContentHdrs
);


#ifdef  __cplusplus
}
#endif

#endif
