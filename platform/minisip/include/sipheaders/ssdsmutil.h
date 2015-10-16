/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName: ssdsmutils.h

    Author:
    Version:

    Date: 2005-11-17
    Description: This file provides functions declarion for those utility functions

    Function List:

    History:
        1. Date:
        Defect:
        Tag: <MOD.001>
        Author:
        Modification:

*******************************************************************************/
#ifndef __SSDSMUTIL_H__
#define __SSDSMUTIL_H__

#include "ssdatatype.h"
#include "ssenums.h"
#include "sssipbase.h"
#include "ssmessage.h"
#include "ssheaders.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct  _SipUriCmpBitSetS
{
    SS_UINT32   bit1SkipUserInfoCmp:1;   /* Do not Compare User Info (including password) in Uri */
    SS_UINT32   bit1SkipMethodParamCmp:1;/* Do not compare Method paramter */
    SS_UINT32   bit1SkipTransportParamCmp:1;/*Donot compare transport Param*/
    SS_UINT32   bit1SkipUserParamCmp:1;/* Do not compare User Parameter */
    SS_UINT32   bit1SkipMaddrParamCmp:1;/* Do not compare Maddr Parameter */
    SS_UINT32   bit1SkipPortCmp:1;     /* Do Not Compare Port */
    SS_UINT32   bit1SkipUriHeaderCmp:1;/* Do not compare Uri Headers */
    SS_UINT32   bit1SkipUriParamsCmp:1;/*Donot compare extension URI paramters*/
    SS_UINT32   bit1SkipUriTtlCmp:1;   /*Do not compare Ttl paramters */
    SS_UINT32   bit1SkipUriCompTypeCmp:1;/*Do not compare CompType paramters */
    SS_UINT32   bit21SkipResrved:22;     /* Reserved for future Use */

}  SipUriCmpBitSetS;

#define SSDSM_UTIL_QUOTES_LEN    2

#if 0
/******************************************************************************
    Function Name : SipCopyMethod

    Description  :This function Copies the Method from the i/p param to the o/p
                  param

    Inputs      :SipMemCpHdl * : Memory pointer.
                 Method * : Input Method pointer

    Output      : Method * : IN/OUT Method pointer

    Returns     : EN_SIP_RESULT
                    SS_RET_SUCCESS: If Method is successfully copied
                    SS_RET_MALLOC_FAILED: If memory allocation fails
                    SS_RET_INVALID_PARAM: If input param is SS_NULL_PTR
******************************************************************************/
EN_SIP_RESULT SipDsmCopyMethod
(
    SipMemCpHdl hdlMemCp,
    Method *pstInMethod,
    Method *pstOutMethod
);
#endif
/******************************************************************************
    Function Name : SipDsmCmpTptAddr

    Description   : This function is to compare two Host, when host type is
                    SIP_ADDR_TYPE_DOMAIN
                    or SIP_ADDR_TYPE_IPV6 the host name is case-insensitive:

    Input(s)      : pstFirstAddr  - the pointer to the first Address
                    pstSecondAddr  - the pointer to the second Address

    Output(s)     : None
    Returns       : SS_BOOL to indicate whether the two Address are equal
******************************************************************************/
SS_BOOL SipDsmCmpTptAddr
(
    IN SipTptAddressS *pstFirstAddr,
    IN SipTptAddressS *pstSecondAddr
);

/******************************************************************************
    Function Name: SipDsmCopyTptAddrToHostPort

    Description  : This function is to copy SipTptAddressS data to HostPort

    Input(s)     : pstTptAddr - SipTptAddressS structure from where to copy
                                data

    Output(s)    : pstHostPort - HostPort structure to which data will copy

    Returns      : void

******************************************************************************/
EN_SIP_RESULT   SipDsmCopyTptAddrToHostPort
(
    IN  SipTptAddressS *pstTptAddr,
    OUT SipHostPort *pstHostPort
);

/******************************************************************************
    Function Name: SipDsmConvTptAddrToHostString

    Description  : This function is to convert the TPT address to encoded string

    Input(s)     : pstTptAddr - SipTptAddressS structure from where to copy
                                data
                   pstMemCp   - The control pointer of memory

    Output(s)    : pstStr     - The encoded IP address

    Returns      : EN_SIP_RESULT or error code

******************************************************************************/
EN_SIP_RESULT  SipDsmConvTptAddrToHostString
(
    IN  SipMemCpHdl        hdlMemCp,
    IN  SipTptAddressS    *pstTptAddr,
    OUT SipString         *pstStr
);

/******************************************************************************
    Function Name: SipDsmFillTptAddrByNwInfo

    Description  : This function fill SipTptAddressS with SipTptNwInfoS.

    Input(s)     : pstTptInfo  - the network information structure.
                   bUseSrcAddr - Use source address to fill pstTptAddr.
                   pstTptAddr  - SipTptAddressS structure will be filled

    Output(s)    : pstTptAddr  - The IP and port that is filled.

    Returns      : EN_SIP_RESULT or error code

******************************************************************************/
EN_SIP_RESULT  SipDsmFillTptAddrByNwInfo
(
    IN  SipTptNwInfoS     *pstNwInfo,
    IN  SS_BOOL            bUseSrcAddr,
    IO  SipTptAddressS    *pstTptAddr
);

/******************************************************************************
    Function Name: SipDsmCmpIpPort

    Description  : This function compare 2 SipTptIpPortS structure.

    Input(s)     : enAddrType - IP addres type.
                   pstIpP1  - IP/Port structure 1.
                   pstIpP2  - IP/Port structure 2.

    Output(s)    : None

    Returns      : SS_TRUE    - the pstIpP1 and pstIpP2 have same value
                   SS_FALSEor - the pstIpP1 and pstIpP2 have different value

******************************************************************************/
SS_BOOL SipDsmCmpIpPort
(
    EN_SIP_ADDRESS_TYPE  enAddrType,
    SipTptIpPortS       *pstIpP1,
    SipTptIpPortS       *pstIpP2
);

/******************************************************************************
    Function Name : SipUtilCompareGenericParamList

    Description   : This function is compare two GenericParamLists, the rule
is:
                  if there is any param existing in both GenericParamLists
and the value of this
                  param in two GenericParamLists are different, then it
returns SS_FALSE. otherwise
                  it return SS_TRUE
                  Compare of name of param is case-insensitive
                  Compare of value of param is case-sensitive
    Input(s)      : pFirstList  - the pointer to the first SipGenericParamListHdl
                   pSecondList  - the pointer to the second SipGenericParamListHdl

    Output(s)     : None
    Returns       : SS_BOOL to indicate whether the two GenericParamLists are
equal
******************************************************************************/
SS_BOOL SipDsmCompareGenericParamList
(
    IN SipGenericParamListHdl hdlFirstList,
    IN SipGenericParamListHdl hdlSecondList
);

/******************************************************************************
    Function Name : SipDsmCompareHost

    Description   : This function is compare two Host, when host type is
SIP_ADDR_TYPE_DOMAIN
                  or SIP_ADDR_TYPE_IPV6 the host name is case-insensitive:

    Input(s)      : pFirstHost  - the pointer to the first Host
                   pSecondHost  - the pointer to the second Host

    Output(s)     : None
    Returns       : SS_BOOL to indicate whether the two Hosts are equal
******************************************************************************/
SS_BOOL SipDsmCompareHost
(
    IN SipHost *pstFirstHost,
    IN SipHost *pstSecondHost
);

/******************************************************************************
    Function Name : SipDsmCompareURI

    Description   : This function is compare two non-NULL URI
    Input(s)      :
                    pstCmpBitSet - bitset
                    pFirstUri  - the pointer to the first URI
                    pSecondUri  - the pointer to the second URI

    Output(s)     : None
    Returns       : SS_BOOL to indicate whether the two URIs are equal
******************************************************************************/
SS_BOOL SipDsmCompareURI
(
    IN SipUriCmpBitSetS *pstCmpBitSet,
    IN URI *pstFirstURI,
    IN URI *pstSecondURI
);

/******************************************************************************
    Function Name : SipDsmCompareSipURI

    Description   : This function is compare two non-NULL Sip URI
    Input(s)      :  pstCmpBitSet - bitset
                        pFirstSipURI  - the pointer to the first Sip URI
                        pSecondSipURI  - the pointer to the second Sip URI

    Output(s)     : None
    Returns       :  SS_BOOL to indicate whether the two Sip URIs are equal
******************************************************************************/
SS_BOOL SipDsmCompareSipURI
(
    IN SipUriCmpBitSetS *pstCmpBitSet,
    IN SipURI           *pstFirstSipURI,
    IN SipURI           *pstSecondSipURI
);

/*****************************************************************************
 Function   : SipDsmCopyReversedNwInfo
 Description:
             This function will copy Nw information
             If already allocated we have to call SipTxnFreeNwInfo before
             calling this function
 Input      :
            pSrcNwInfo: Nw info From Where Information is copied

 Output     :
            pDstNwInfo: Nw info to which Information is copied
 Return     :
             SUCCESS or FAILURE
*****************************************************************************/
SS_VOID SipDsmCopyReversedNwInfo
(
    IN  SipTptNwInfoS *pstSrcNwInfo,
    OUT SipTptNwInfoS *pstDstNwInfo
);

/******************************************************************************
    Function Name : SipDsmAppendGenericParam

    Description   : This function adds a generic param to the List (ebven if it
                    is empty )
    Input(s)      : pMemCp  - Memory Control Pointer
                    pstList - The List to which the paramter is to be added
                    pstName - The Name
                    pstValue - The value (quoted or unquoted and may be NULL)

    Output(s)     : None
    Returns       : SS_RET_SUCCESS or error code
******************************************************************************/
EN_SIP_RESULT   SipDsmAppendGenericParam
(
    IN SipMemCpHdl          hdlMemCp,
    IO SipGenericParamListHdl hdlList,
    IN SipString           *pstName,
    IN SipString           *pstValue,
    IN SS_BOOL              bQuoted
);

/******************************************************************************
    Function Name : SipDsmGetGenericParamByName

    Description   : This function gets a generic param from the List by its Name
    Input(s)      : pstList - The List to which the paramter is to be added
                    pstName - The Name

    Output(s)     : None
    Returns       : Pointer to a NameValue structure
******************************************************************************/
SipGenParNameValue   *SipDsmGetGenericParamByName
(
    IN SipGenericParamListHdl    hdlList,
    IN SipString              *pstName
);

/******************************************************************************
    Function Name : SipDsmGetGenericParamByName

    Description   : This function gets a generic param from the List by its Name
    Input(s)      : pstMemCp - Memory Control Pointer
                    pstList  - The List to which the paramter is to be added
                    pstName  - The Name

    Output(s)     : None
    Returns       : Pointer to a NameValue structure
******************************************************************************/
EN_SIP_RESULT   SipDsmRmvGenericParamByName
(
    SipMemCpHdl           hdlMemCp,
    SipGenericParamListHdl  hdlList,
    SipString            *pstName
);

/*******************************************************************************
    Function Name : SipDsmGetHdrNameValueFromListByName

    Description   : This API gets the header-name value from the .

    Input(s)      : hdlHdrList       - list containing SipHdrNameValue as items
                    pstName          - header-name string Eg:- "Via"
                    chCompactName    - compact name for the header Eg:- 'v' ,
                                       This can optional value (SS_ZERO).
    Output(s)     : None

    Returns       : reference to SipHdrNameValue if present else NULL is
                    returned.
*******************************************************************************/
SipHdrNameValue * SipDsmGetHdrNameValueFromListByName
(
    IN SipHeaderListHdl hdlHdrList,
    IN SipString       *pstName,
    IN SS_CHAR          chCompactName

);

/*******************************************************************************
    Function Name : SipDsmCopyDecFailedHdr

    Description   : This copies a failed header if present in source sip message
                    to destination sip message.

    Input(s)      : usHdrId      - failed header-id which has to be copied
                    pstSrcSipMsg - source sip message from where the source
                                   failed header has to be copied from
                    pstDesSipMsg - destination sip message to which the source
                                   failed header has to be copied into
    Output(s)     : None

    Return        : SIP_RET_SUCCESS or Error Code
*******************************************************************************/
EN_SIP_RESULT SipDsmCopyDecFailedHdr
(
    IN SS_UINT16 usHdrId,
    IN SipMsgS  *pstSrcSipMsg,
    IO SipMsgS  *pstDesSipMsg
);


#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif /* __SSDSMUTILS_H__*/
