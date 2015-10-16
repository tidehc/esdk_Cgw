/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName: sipdsmapi.x

    Author:
    Version:

    Date: 2005-11-17
    Description: This file provides the DSM interfaces for Version 2 of the DSM
    with complete support for incremental on-demand parsing.

    Function List:

    History:
        1. Date:
        Tag: <MOD.001>
        Author:
        Modification:

*******************************************************************************/
#ifndef __SIPDSMAPI_X__
#define __SIPDSMAPI_X__

#include "ssenums.h"
#include "ssmessage.h"
#include "ssheaders.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*

1.Message, Header and Header item

    Message means SIPMessage.

    Header : has two categories, one is sigle value header, another is
             multiple-value header. Each value of that header is
             called header item.

2.Concept of API:

    APIs are organized as pairs:

    A. Header level API and header item.

        Copy    <-> Clear
        Clone   <-> Free
        Create

        The diffence between Copy and Clone is that for Copy API,
        caller of DSM must allocate the memory of the outer structure,
        such as have to allocate a memory of sizeof(From), pass
        this pointer or pass a local From variable to Copy API.

    B. Mesasge level

        Set     <-> Remove
        Copy    <-> Remove

    Please take care for Message level Remove API, the removed header and
    header item is never freed by these API, the user of DSM must take
    care of free this.

3.Concept of Top and Bottom:

    This is a header in a SIPMessage:

    INVITE sip:122@dummy.com SIP/2.0
    Via: SIP/2.0/UDP 1@foo.com;branch=233  // This is Top and index as 0
    Via: SIP/2.0/UDP 2@bar.com;branch=345  // This is index as 1
    Via: SIP/2.0/UDP 3@don.com;branch=567  // This is bottom
    ...other headers

    for Append, API will append the item at the Bottom.
    for Prepend, API will prepend the item at the Top.
*/


/************* H E A D E R    M A N I P U L A T I O N    A P I ***************/


/******************************************************************************
    Function Name : SipDsmCreateHdrItem

    Description   : This API creates a Header item

    Input(s)      : usHdrId - The SIP Header type
                    hdlMemCp - The Memory control pointer that is to be used for
                                allocating and deallocating memory

    Output(s)     : ppvHdrItem - The output Header container

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
EN_SIP_RESULT   SipDsmCreateHdrItem
(
    IN  SS_UINT16    usHdrId,
    IN  SipMemCpHdl  hdlMemCp,
    OUT SS_VOID    **ppvHdrItem
);

/******************************************************************************
    Function Name : SipDsmCopyHdrItem

    Description  :  To Copy the header Item to input memory

    Inputs      : hdlMemCp   : Memory pointer.
                  pvSrcHdrItem *: Pointer to Header from which copied
                        has to be performed
                  usHdrId :header Type which is required to copied

    Output      : pvDstHdrItem *: Pointer to Header to which copied
                        has to be performed.

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If DigestInfo is copied to SipMsg
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR

******************************************************************************/
EN_SIP_RESULT SipDsmCopyHdrItem
(
    IN  SS_UINT16    usHdrId,
    IN  SipMemCpHdl  hdlMemCp,
    IN  SS_VOID     *pvSrcHdrItem,
    OUT SS_VOID     *pvDstHdrItem
);

/******************************************************************************
    Function Name : SipDsmClearHdrItem

    Description  :  To Clear the header whose meta-data(in file ssAbnfSipMsgElmn.c)
                    defined as Seq.

    Inputs      : usHdrId :header Type which is required to copied
                  hdlMemCp: Memory pointer.
                  pvHdr   : Pointer to Header to which Clean Opearation
                    has to be performed

    Output      : pvHdr   : Pointer to Header to which Clean Opearation
                    has to be performed

    Returns     : SS_VOID
******************************************************************************/
SS_VOID SipDsmClearHdrItem
(
    IN SS_UINT16 usHdrId,
    IN SipMemCpHdl hdlMemCp,
    IN SS_VOID  *pvHdr
);

/******************************************************************************
    Function Name : SipDsmCloneHdrItem

    Description   : This API deep-copies an header Item
                    (i.e a Header list member)
                    and all it's contents from a source data structure

    Input(s)      : usHdrId - The SIP Header type
                    hdlMemCp - The Memory control pointer that is to be used for
                                allocating and deallocating memory
                    pvSrcHdrItem - The header item which is to be copied

    Output(s)     : ppvClonedHdrItem - The header item where the copy
                                is to be made

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
EN_SIP_RESULT SipDsmCloneHdrItem
(
    IN  SS_UINT16       usHdrId,
    IN  SipMemCpHdl     hdlMemCp,
    IN  SS_VOID        *pvSrcHdrItem,
    OUT SS_VOID       **ppvClonedHdrItem
);

/******************************************************************************
    Function Name : SipDsmFreeHdrItem

    Description   : This API frees a previously cloned header item

    Input(s)      : usHdrId - The SIP Header type
                    hdlMemCp - The Memory control pointer that is to be used for
                                allocating and deallocating memory
                    ppvClonedHdrItem - The cloned header item that needs to be
                                freed

    Output(s)     : ppvClonedHdrItem - The cloned header item that needs to be
                                freed

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
SS_VOID SipDsmFreeHdrItem
(
    IN  SS_UINT16    usHdrId,
    IN  SipMemCpHdl  hdlMemCp,
    IO  SS_VOID    **ppvClonedHdrItem
);

/******************************************************************************
    Function Name : SipDsmCreateHdr

    Description   : This API creates space for the container so that
                    header content can be added, like allocate a memory of
                    Call-ID and return that pointer.

    Input(s)      : usHdrId - The SIP Header type
                    hdlMemCp - The Memory control pointer that is to be used for
                                allocating and deallocating memory

    Output(s)     : ppvHdr - The output Header container

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
EN_SIP_RESULT   SipDsmCreateHdr
(
    IN  SS_UINT16    usHdrId,
    IN  SipMemCpHdl  hdlMemCp,
    OUT SS_VOID    **ppvHdr
);

/******************************************************************************
    Function Name : SipDsmCopyHdr

    Description  :  To Copy the header.

    Inputs      : hdlMemCp   : Memory pointer.
                  pvSrcHdr *: Pointer to Header from which copied
                            has to be performed
                  usHdrId :Header type For which Copy opearation
                            has to performed

    Output      : pvDstHdr *: Pointer to Header to which copied
                            has to be performed.

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If DigestInfo is copied to SipMsg
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCopyHdr
(
    IN  SS_UINT16    usHdrId,
    IN  SipMemCpHdl  hdlMemCp,
    IN  SS_VOID     *pvSrcHdr,
    IO  SS_VOID     *pvDstHdr
);

/******************************************************************************
    Function Name : SipDsmCloneHdr

    Description  :  To Clone the header.

    Inputs      : hdlMemCp   : Memory pointer.
                  pvSrcHdr : Pointer to Header from which Cloning
                            has to be performed
                  usHdrId :Header Type which needs to be Cloned

    Output      : ppvClonedHdr: Double Pointer to Header to which
                            Cloned has to be performed.

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If DigestInfo is copied to SipMsg
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCloneHdr
(
    IN  SS_UINT16    usHdrId,
    IN  SipMemCpHdl  hdlMemCp,
    IN  SS_VOID     *pvSrcHdr,
    OUT SS_VOID    **ppvClonedHdr
);

/******************************************************************************
    Function Name : SipDsmClearHdr

    Description  :  To Clear the Header which is passed as rgument of this Function.

    Inputs      : hdlMemCp   : Memory pointer.
                  pvHdr *: Pointer to Header to which Clear Operation
                            has to be performed.
                  usHdrId :Header Type which needs to be Cleared

    Output      : None

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If DigestInfo is copied to SipMsg
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
SS_VOID SipDsmClearHdr
(
    IN SS_UINT16    usHdrId,
    IN SipMemCpHdl  hdlMemCp,
    IN SS_VOID     *pvHdr
);

/******************************************************************************
    Function Name : SipDsmFreeHdr

    Description  :  To Free the header

    Inputs      : hdlMemCp   : Memory pointer.

                  usHdrId :Header Type which needs to be freed

    Output      : ppvClonedHdr: Double Pointer to Cloned Header which
                                   needs to be freed

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If DigestInfo is copied to SipMsg
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
SS_VOID SipDsmFreeHdr
(
    IN  SS_UINT16     usHdrId,
    IN  SipMemCpHdl   hdlMemCp,
    OUT SS_VOID     **ppvClonedHdr
);

/******************************************************************************
    Function Name : SipDsmInsertHdrItemToHdr

    Description   : This API append or prepend an header item to the container
                    header structure. If the container is NULL, the api shall
                    alocate space for the container and insert this header item

    Input(s)      : usHdrId - The SIP Header type
                    hdlMemCp - The Memory control pointer that is to be used for
                                allocating and deallocating memory
                    bAppend  - If true, it's to append header item to the header.
                               Or prepend the header item to the header.
                    pvHdrItem - The header item to be inserted

    Output(s)     : pvHdr - The output Header container containing the header
                    handle to the header if created, otherwise remain same as
                    the input parameter

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
EN_SIP_RESULT   SipDsmAppendPrependHdrItemToHdr
(
    IN  SS_UINT16   usHdrId,
    IN  SipMemCpHdl hdlMemCp,
    IN  SS_VOID    *pvHdrItem,
    IN  SS_BOOL     bAppend,
    IO  SS_VOID    *pvHdr
);

/******************************************************************************
    Function Name : SipDsmGetHdrItemCount

    Description  :  To Get the Header Item Count  from Header.

    Inputs      : usHdrId: Header Type Defined as SS_UINT32 enum
                : pvHdr: Header from which get action has to be performed


    Returns     : SS_ZERO-Incase of failure or if there is no HeaderItem in Header
                  No of Header Item in Header
******************************************************************************/
SS_UINT32   SipDsmGetHdrItemCount
(
    IN  SS_UINT16  usHdrId,
    IN SS_VOID   *pvHdr
);

/******************************************************************************
    Function Name : SipDsmGetHdrItemFromHdr

    Description  :  To Copy the header whose meta-data(in file ssAbnfSipMsgElmn.c)
                    defined as SeqOf.

    Inputs      : usHdrId: Header Type Defined as SS_UINT32 enum
                  ulIndex :The Index at which the header item is required.
                  pvHdr: Header in which action has to be performed

    Output      : None

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If DigestInfo is copied to SipMsg
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
SS_VOID *SipDsmGetHdrItemFromHdr
(
    IN  SS_UINT16    usHdrId,
    IN  SS_UINT16    usIndex,
    IN  SS_VOID     *pvHdr
);

/******************************************************************************
    Function Name : SipDsmGetTopBottomHdrItemFromHdr

    Description  :  To Copy the header whose meta-data(in file ssAbnfSipMsgElmn.c)
                    defined as SeqOf.

    Inputs      : usHdrId:Header Type Defined as SS_UINT32 enum
                  bTop:   If true, get top hdr item, or get bottom hdr item
                  pvHdr: Header in which Remove action has to be performed

    Output      : None

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If DigestInfo is copied to SipMsg
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
SS_VOID * SipDsmGetTopBtmHdrItemFromHdr
(
    IN SS_UINT16 usHdrId,
    IN SS_BOOL   bTop,
    IN SS_VOID  *pvHdr
);

/******************************************************************************
    Function Name : SipDsmRmvTopBtmHdrItemFromHdr

    Description  :  To Remove the Top Header Item from header.

    Inputs      : usHdrId:Header Type Defined as SS_UINT32 enum
                  hdlMemCp   : Memory pointer.
                  bTop:   If true, remove top hdr item, or remove bottom hdr item
                  pvHdr: Header in which Remove action has to be performed

    Output      : pvHdr: Header in which Remove action has to be performed

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If DigestInfo is copied to SipMsg
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
SS_VOID*   SipDsmRmvTopBtmHdrItemFromHdr
(
    IN SS_UINT16   usHdrId,
    IN SipMemCpHdl hdlMemCp,
    IN SS_BOOL     bTop,
    IO SS_VOID    *pvHdr
);

/*For URI, Host, SipURI*/
/******************************************************************************
    Function Name : SipDsmCopyUsualParams

    Description  :This function Copies the general params from the i/p param to
                  the o/p param including URI, SipURI, Host. Later we can
                  extend it.

    Inputs      : usParamId:  The registered parameter ID in the stack which
                              record the parameter ID
                  hdlMemCp   :Memory pointer.
                  pvInParam : Input paramter pointer

    Output      : pvOutParam : IN/OUT parameter pointer.
                  For URI,    it's URI type.
                  For SipURI, it's SipURI type.
                  For Host,   it's Host type.

    Returns     : EN_SIP_RESULT
                  SS_RET_SUCCESS: If the parameter is successfully copied
                  SS_RET_MALLOC_FAILED: If memory allocation fails
                  SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCopyUsualParams
(
    IN  SS_UINT16     usParamId,
    IN  SipMemCpHdl   hdlMemCp,
    IN  SS_VOID      *pvInParam,
    OUT SS_VOID      *pvOutParam
);

/******************************************************************************
    Function Name : SipDsmClearUsualParams

    Description  :This function Clears the given parameter

    Inputs      :usParamId:  The registered parameter ID in the stack which
                              record the parameter ID
                 hdlMemCp   : Memory pointer.
                 pvParam : Input parameter pointer

    Output      : None

    Returns     : SS_VOID
******************************************************************************/
SS_VOID SipDsmClearUsualParams
(
    IN SS_UINT16    usParamId,
    IN SipMemCpHdl  hdlMemCp,
    IN SS_VOID     *pvParam
);

/******************************************************************************
    Function Name : SipDsmCloneUsualParams

    Description  :This function Clones a parameter

    Inputs      :usParamId:  The registered parameter ID in the stack which
                              record the parameter ID
                 hdlMemCp   : Memory pointer.
                 pvInParam: Input paramter pointer

    Output      : ppvOutParam: IN/OUT parameter pointer

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If URI is successfully copied
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCloneUsualParams
(
    IN  SS_UINT16    usParamId,
    IN  SipMemCpHdl  hdlMemCp,
    IN  SS_VOID     *pvInParam,
    OUT SS_VOID    **ppvOutParam
);

/******************************************************************************
    Function Name : SipDsmCreateUsualParams

    Description  :This function Creates the Structure related to parameter ID

    Inputs      :usParamId:  The registered parameter ID in the stack which
                              record the parameter ID
                 hdlMemCp   : Memory pointer.

    Output      : ppstOutParam: IN/OUT parameter pointer

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If URI is successfully copied
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT  SipDsmCreateUsualParams
(
    IN  SS_UINT16   usParamId,
    IN  SipMemCpHdl   hdlMemCp,
    OUT SS_VOID   **ppvOutParam
);

/******************************************************************************
    Function Name : SipDsmFreeUsualParams

    Description  :This function Clears the usual parameters including URI,
                  SipURI, Host. Later we can extend it.

    Inputs      :usParamId  : The registered parameter ID in the stack which
                              record the parameter ID
                 hdlMemCp   : Memory pointer.
                 ppvParam : Input paramter pointer

    Output      :ppvParam : Input paramter pointer

    Returns     : SS_VOID
******************************************************************************/
SS_VOID SipDsmFreeUsualParams
(
    IN SS_UINT16    usParamId,
    IN SipMemCpHdl  hdlMemCp,
    IO SS_VOID    **ppvParam
);

/******************************************************************************
    Function Name : SipDsmCreateTopBtmHdrItemInHdr

    Description   : This API create an header item and preppend or append
                    this to the header structure. If the header is NULL,
                    it will fail.

    Input(s)      : usHdrId  - The SIP Header type
                    hdlMemCp - The Memory control pointer that is to be used
                               for  allocating and deallocating memory
                    bTop     - If true, remove top hdr item, or remove bottom
                               hdr item
                    pvHdr   - The header which the header item have to be
                              created

    Output(s)     : pvHdr   - The header which the header item have to be
                              created

    Returns       : The created and append Top header item.
******************************************************************************/
SS_VOID* SipDsmCreateTopBtmHdrItemInHdr
(
    IN SS_UINT16    usHdrId,
    IN SipMemCpHdl  hdlMemCp,
    IN SS_BOOL      bTop,
    IO SS_VOID     *pvHdr
);


/************* MESSAGE    M A N I P U L A T I O N    A P I ***************/
/******************************************************************************
    Function Name : SipDsmGetHdrFromMsg

    Description   : This API retrieves a handle to the header from the message

    Input(s)      : usHdrId   - Header ID
                    pstSipMsg - The SIP Message

    Output(s)     : None

    Returns       : The handle to the SIP Header within the message
******************************************************************************/
SS_VOID* SipDsmGetHdrFromMsg
(
    IN SS_UINT16   usHdrId,
    IN SipMsgS    *pstSipMsg
);

/******************************************************************************
    Function Name : SipDsmAppendPrependHdrItemToMsg

    Description   : This API create an header item and preppend this to the
                    message in header structure. If the header is NULL,
                    it will generate it.

    Input(s)      : usHdrId    - The SIP Header type
                    pstSipMsg  - The sip message which will be inserted with one hdr item
                    bAppend    - If true, append the hdr item. Or prepend it to the hdrlist of message.
                    pvHdrItem - The header item expected to insert into the message

    Output(s)     : NULL

    Returns       : The created and append Top header item.
******************************************************************************/
EN_SIP_RESULT SipDsmAppendPrependHdrItemToMsg
(
    IN SS_UINT16  usHdrId,
    IO SipMsgS    *pstSipMsg,
    IN SS_VOID    *pvHdrItem,
    IN SS_BOOL     bAppend
);

/******************************************************************************
    Function Name : SipDsmRmvTopBtmHdrItemFromMsg

    Description   : This API removes the topmost header item for a non-singular
                    header

    Input(s)      : usHdrId   - The header ID
                    pstSipMsg - The SIP Message
                    bTop      - If true, remove top hdr item. Or remove bottom item.
    Output(s)     :

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
SS_VOID * SipDsmRmvTopBtmHdrItemFromMsg
(
    IN SS_UINT16 usHdrId,
    IN SipMsgS  *pstSipMsg,
    IN SS_BOOL   bTop
);

/******************************************************************************
    Function Name : SipDsmCreateTopBtmHdrItemInMsg

    Description   : This API create an header item and preppend this to the
                    SIP Message. If the header is not present, it will be
                    created

    Input(s)      : usHdrId   - The SIP Header ID
                    pstSipMsg - The SIP Message
                    bTop      - If true, create top hdr item. Or create bottom
                                header item.

    Output(s)     : pstSipMsg - The SIP Message

    Returns       : The created and append Top header item.
******************************************************************************/
SS_VOID* SipDsmCreateTopBtmHdrItemInMsg
(
    IN  SS_UINT16   usHdrId,
    IO  SipMsgS    *pstSipMsg,
    IN  SS_BOOL     bTop
);

/******************************************************************************
    Function Name : SipDsmGetTopBtmHdrItemFromMsg

    Description   : This API is to get top or bottom header item from the
                    message

    Input(s)      : usHdrId   - The SIP Header ID
                    pstSipMsg - The SIP Message
                    bTop      - If true, get top hdr item. Or get bottom header
                                item.

    Output(s)     : None

    Returns       : The created and append Top header item.
******************************************************************************/
SS_VOID* SipDsmGetTopBtmHdrItemFromMsg
(
    IN SS_UINT16   usHdrId,
    IN SipMsgS    *pstSipMsg,
    IN SS_BOOL     bTop
);

/******************************************************************************
    Function Name : SipDsmRmvHdrFromMsg

    Description   : This API removes a header from the SIP Message

    Input(s)      : usHdrId   - The SIP Header ID
                    pstSipMsg - The SIP Message

    Output(s)     : None

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
SS_VOID SipDsmRmvHdrFromMsg
(
    IN SS_UINT16  usHdrId,
    IO SipMsgS   *pstSipMsg
);

/******************************************************************************
    Function Name : SipDsmSetHdrToMsg

    Description   : This API sets a header in the SIP Message any existing
                    header (for the specified header type) will be replaced
                    without deallocating memory for the existing data (this
                    should be done by the caller). No deepcopy is done and the
                    caller and message share the same contents

    Input(s)      : usHdrId - The SIP Header ID
                    pvHdr - the header from which the item need to be retrieved
                    pstSipMsg - The sip message which accepts the header

    Output(s)     : None

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
EN_SIP_RESULT SipDsmSetHdrToMsg
(
    IN SS_UINT16  usHdrId,
    IO SipMsgS   *pstSipMsg,
    IN SS_VOID   *pvHdr
);

/******************************************************************************
    Function Name : SipDsmCopyHdrToMsg

    Description   : This API deepcopies a header into the SIP Message. It will
                    not free the existing contents. The caller should first
                    call the rmv API to free the existing contents

    Input(s)      : usHdrId   - The SIP Header ID
                    pstSipMsg - The SIP Message
                    pvHdr    - The header from which the item need to be retrieved

    Output(s)     :

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
EN_SIP_RESULT SipDsmCopyHdrToMsg
(
    IN SS_UINT16  usHdrId,
    IO SipMsgS   *pstSipMsg,
    IN SS_VOID   *pvHdr
);

/******************************************************************************
    Function Name : SipDsmCreateHdrInMsg

    Description   : This API sets a header in the SIP Message any existing
                    header (for the specified header type) will be replaced
                    without deallocating memory for the existing data (this
                    should be done by the caller). No deepcopy is done and the
                    caller and message share the same contents

    Input(s)      : usHdrId   - The SIP Header ID
                    pstSipMsg - The message which is expected to accept the
                                created header

    Output(s)     :

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
SS_VOID* SipDsmCreateHdrInMsg
(
    IN SS_UINT16  usHdrId,
    IO SipMsgS   *pstSipMsg
);

/******************************************************************************
    Function Name : SipDsmIsHeaderSingleton

    Description   : This API checks whether this header is singleton header
                    or multi-value header.

    Input(s)      : usHdrId -  Header ID
    Output(s)     :

    Returns       : If true, it's a singleton header. Or it's a multi-value header.
******************************************************************************/
SS_BOOL SipDsmIsHeaderSingleton
(
    IN SS_UINT16 usHdrId
);

#ifdef  __cplusplus
}
#endif /*__cplusplus__*/

#endif /*__SIPDSMAPI_X__*/

