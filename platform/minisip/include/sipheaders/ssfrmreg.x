/*******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName   : ssfrmreg.x

    Author     : Srinivas R Thota

    Version    : 1.0

    Date       : 2005-11-12

    Description: This file provides the Registeration framework interfaces.


    Function List:
        1. SipCodecRegHeader
        2. SipCodecRegToken
        3. SipCodecUnRegHeader


    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX
*******************************************************************************/
#ifndef __SS_FRMREG_X__
#define __SS_FRMREG_X__

#include "sssipbase.h"
#include "ssenums.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _SipCodecOptCfgS
{
    SS_BOOL bDecUnknownHdrs;   /* Keep Unknown headers in SipHdrS during decoding */
    SS_BOOL bDecKeepFailedHdrs;/* Keep failed headers in SipHdrS during decoding */
    SS_BOOL bEncUseCompactForm;/* Encode using compact names such as "v" for Via */
    SS_BOOL bEncInMultipleLines;/* Only Effective Non-Singleton Headers such as Via */
}SipCodecOptCfgS;

typedef SS_VOID* SipHdrInfoHdl;

typedef EN_SIP_RESULT (*pFnCodecHdrInit)(SipHdrInfoHdl hdlHdrInfo);

/*******************************************************************************
    Function Name : SipCodecRegHeader

    Description   : This API registers a Sip header into the Codec & DSM Framework.

    Input(s)      : usHdrId       - The header Id
                    pfnHeaderInit - Pointer to header Initialization funtion

    Output(s)     : None

    Returns       : SIP_RET_SUCCESS or Error Code
*******************************************************************************/
EN_SIP_RESULT SipCodecRegHeader
(
    IN SS_UINT16        usHdrId,
    IN pFnCodecHdrInit  pfnHeaderInit
);


/***************************************************************************8****
    Function Name : SipCodecUnRegHeader

    Description   : This API unregisters a Sip header from the Framework

    Input(s)      : usHdrId - The header Id

    Output(s)     : None

    Returns       : SIP_RET_SUCCESS or Error Code
*******************************************************************************/
EN_SIP_RESULT SipCodecUnRegHeader
(
    IN SS_UINT16    usHdrId
);


/*******************************************************************************
    Function Name : SipCodecRegToken

    Description   : This API registers a Token (well-kown string to be recognized
                    during decoding and encoding)

    Input(s)      : usTokenId  - The token Id
                    pcToken    - The token string
                    usTokenLen - The token string length
                    usHdrId    - The header-id of the header for which the Token
                                 has to be registered

    Output(s)     : None

    Returns       : SIP_RET_SUCCESS or Error Code
*******************************************************************************/
EN_SIP_RESULT SipCodecRegToken
(
    IN SS_UINT16    usTokenId,
    IN SS_CHAR     *pcToken,
    IN SS_UINT16    usTokenLen,
    IN SS_UINT16    usHdrId
);

/******************************************************************************
    Function Name : SipCodecUnRegToken

    Description   : This API un-registers a Token (well-kown string to be recognized
                    during decoding and encoding) which was earlier registered
                    using SipCodecRegToken

    Input(s)      : usTokenId  - The token Id
                    usHdrId    - The header-id of the header for which the Token
                                 has to be registered

    Output(s)     : None

    Returns       : SIP_RET_SUCCESS or Error Code
******************************************************************************/
EN_SIP_RESULT SipCodecUnRegToken
(
    IN SS_UINT16    usTokenId,
    IN SS_UINT16    usHdrId
);

#ifdef __cplusplus
}
#endif

#endif /* __SS_FRMREG_X__ */

