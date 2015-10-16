/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : vpptoken.x

    Author          : zhaolei (ronnie), Li Minghua

    Version         : 1.0

    Date            : 2005-11-12

    Description     : The file defines the interface functions and
                      structures of Token.

    Function List   :
        1.VppTokenReg
        2.VppTokenUnreg

    History:
        1. Date        : <YYYY-MM-DD>
           Tag         : None
           Author      :
           Modification:
******************************************************************************/
#ifndef _VPP_TOKEN_X_
#define _VPP_TOKEN_X_

#include "vppstring.x"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************

                            Start Of File

******************************************************************************/
#define VPP_TOKEN_UNKNOW_ID  0

typedef VPP_VOID*  VppTokenCbHdl;

typedef struct _VppTokenS
{
    VPP_UINT16         usTokenId;    /* token id */
    VPP_UINT16         usSpare;      /* 32 bit align */
    VppStringS         stString;     /* token string */
}VppTokenS;


/******************************************************************************
 Function   : VppTokenCreate

 Description: Init a token module, create hash table

 Input      : hdlMemCp      - the handle of MemCp
              ulTokenNum    - your estimate token number value

 Output     : phdlTokenCb   - return the TokenCB handle

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppTokenCreate
(
    IN  VppMemCpHdl    hdlMemCp,
    IN  VPP_UINT32     ulTokenNum,
    OUT VppTokenCbHdl *phdlTokenCb
);


/******************************************************************************
 Function   : VppTokenDestroy

 Description: Destroy all the token hash table

 Input      : phdlTokenCb   - The TokenCbS pointer

 Output     : phdlTokenCb   - The TokenCbS pointer

 Return     : None
******************************************************************************/
VPP_VOID VppTokenDestroy
(
    IO  VppTokenCbHdl *phdlTokenCb
);


/******************************************************************************
 Function   : VppTokenReg

 Description: Register token to the token hash table

 Input      : hdlTokenCb     - the handle of Token Cb
              usTokenID      - user specified the token ID
              pcToken        - token string
              usTokenLen     - token string length
              usTokenType    - user specified the token type
              bCaseSensitive - identify whether token is case sensitive or not

 Output     : None

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppTokenReg
(
    IN  VppTokenCbHdl  hdlTokenCb,
    IN  VPP_UINT16     usTokenID,
    IN  VPP_CHAR      *pcToken,
    IN  VPP_UINT16     usTokenLen,
    IN  VPP_UINT16     usTokenType,
    IN  VPP_BOOL       bCaseSensitive
);

/******************************************************************************
 Function   : VppTokenUnreg

 Description: Unregister token from the token hash table

 Input      : hdlTokenCb   - the pointer of TokenCbS handle
              usTokenID    - user specified the token ID
              usTokenType  - token type user specified the token type

 Output     : None

 Return     : None
******************************************************************************/
VPP_VOID VppTokenUnreg
(
    IN  VppTokenCbHdl  hdlTokenCb,
    IN  VPP_UINT16     usTokenID,
    IN  VPP_UINT16     usTokenType
);

/******************************************************************************

                              End of file

******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _VPP_TOKEN_X_ */

