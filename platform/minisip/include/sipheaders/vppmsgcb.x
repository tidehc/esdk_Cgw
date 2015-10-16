/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : vppmsgcb.x

    Author          : zhaolei (ronnie), Li Minghua

    Version         : 1.0

    Date            : 2005-11-12

    Description     : The file defines the interface functions and
                      structures of MsgCb.

    Function List   :
        1. VppMsgCbCreate
        2. VppMsgCbCreateFromStringBuf
        3. VppMsgCbDestroy
        4. VppMsgGetLastErr
        5. VppMsgGetMemCpHdl
        6. VppMsgGetParam
        7. VppMsgGetPrintLen
        8.VppMsgGetScanLen
        9.VppMsgGetTotalPrintLen
        10.VppMsgGetTotalScanLen
        11.VppMsgPrintf
        12.VppMsgScanf
        13.VppMsgSetParam
        14.VppMsgCbRollback

    History:
        1. Date        : <YYYY-MM-DD>
           Tag         : None
           Author      :
           Modification:
******************************************************************************/
#ifndef __VPP_MSGCB_X__
#define __VPP_MSGCB_X__

#include "vpptypes.h"
#include "vppmemcp.x"
#include "vpptoken.x"
#include "vppstrbuf.x"
#include "vppstring.x"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */

/*****************************************************************************

                            Start Of File

******************************************************************************/

/* VppMsgCb Handle */
typedef VPP_VOID*   VppMsgCbHdl;

/* Common decode function for decode a field */
typedef VPP_UINT32 (*pFnVppDecodeFunc)
(
    IO VppMsgCbHdl       hdlMsgCb,
    OUT VPP_VOID        *pvData
);

/* Common encode function for encode a field */
typedef VPP_UINT32 (*pFnVppEncodeFunc)
(
    IO VppMsgCbHdl       hdlMsgCb,
    IN VPP_VOID         *pvData
);

/* VppMsgCb errcode definition */
#define VPP_MSGCB_ERRCODE_START 0x11

enum
{
    /* Invalid input format string */
    VPP_MSGCB_INVALID_FORMAT = VPP_MSGCB_ERRCODE_START,

    /* Find invalid char when validate every char */
    VPP_MSGCB_INVALID_CHARSET,

    /* MsgCb module inner error */
    VPP_MSGCB_INTERNAL_ERROR,

    /* Can't skip this type */
    VPP_MSGCB_SKIP_ERROR,

    /* Message is finished */
    VPP_MSGCB_MSG_FINISHED,

    /* Syntax is not end */
    VPP_MSGCB_SYNTAX_NOT_END,

    /* Skip string is not match */
    VPP_MSGCB_SKIP_STRING_NOMATCH,

    VPP_MSGCB_ERRCODE_END = VPP_ENUM_END
};

/******************************************************************************
 Function   : VppMsgCbCreate

 Description: Create a MsgCb handle, and initialize the parameter using
                continuous string mode. It will prepare a MsgCb for encoding
                and decoding process. If function call is successed, will
                return user a MsgCb handle, and user will use this handle
                to call other utility functions.

              Some features about these parameters:
              1. This supports optional token handle which decode token for
                 user, if this handle is null, won't decode token into an ID,
                 just save as string.
              2. It also supports optional charset which validate the charset
                 for user, if this handle is null, won't check the charset any
                 way, even if using the "v" flag in the string and token field.
              3. If will use MemCp handle memory for user structure. If this
                 pointer is null, will use dynamic memory for user structure to
                 allocate memroy.
              4. "pcMsg and ulMsgLen" is to describe an continuous string
                 buffer. This buffer can be empty for encoding, and can be
                 filled by text for decoding.

 Input      : hdlMemCp    - MemCp handle, can be null or not
              hdlTokenCb  - TokenCb handle, can be null or not
              pusCharSet  - Charset to validate every char in string or
                            token field
              pcMsg       - Message start pointer, can't be null
              ulMsgLen    - Message length, can't be zero

 Output     : phdlMsgCb   - Generate the MsgCb handle base on the input
                            parameters.

 Return     : VPP_SUCCESS if success, VPP_MALLOC_FAIL if malloc memory failure,
              VPP_INVALID_PARAM if input parameters is invalid.
******************************************************************************/
VPP_UINT32    VppMsgCbCreate
(
    OUT VppMsgCbHdl       *phdlMsgCb,
    IN VppMemCpHdl         hdlMemCp,
    IN VppTokenCbHdl       hdlTokenCb,
    IN VPP_UINT16         *pusCharSet,
    IN VPP_CHAR           *pcMsg,
    IN VPP_UINT32          ulMsgLen
);

/******************************************************************************
 Function   : VppMsgCbCreateFromStringBuf

 Description: Create a MsgCb handle, and initialize the parameter using
                paged string(VppStrBuf) mode. It will prepare a MsgCb for
                encoding and decoding process. If function call is successed,
                will return user a MsgCb handle, and user will use this handle
                to call other utility functions.

              Some features about these parameters:
              1. This supports optional token handle which decode token for
                 user, if this handle is null, won't decode token into an ID,
                 just save as string.
              2. It also supports optional charset which validate the charset
                 for user, if this handle is null, won't check the charset any
                 way, even if using the "v" flag in the string and token field.
              3. If will use MemCp handle memory for user structure. If this
                 pointer is null, will use dynamic memory for user structure to
                 allocate memroy.
              4. "hdlStringBuf" is the paged string buffer handle, this
                 parameter can't be null.
              5. Encoding or decoding progress maybe is not start at the first
                 position, so ulCurPos means the start position will use.

 Input      : hdlMemCp    - MemCp handle, can be null or not
              hdlTokenCb  - TokenCb handle, can be null or not
              pusCharSet   - Charset to validate every char in string or
                            token field
              hdlStringBuf- VppStrBuf handle, is for the manage the paged
                            string buffer.
              ulCurPos - The current position.

 Output     : phdlMsgCb   - Generate the MsgCb handle base on the input
                            parameters.

 Return     : VPP_SUCCESS if success, VPP_MALLOC_FAIL if malloc memory failure,
              VPP_INVALID_PARAM if input parameters is invalid.
******************************************************************************/
VPP_UINT32    VppMsgCbCreateFromStringBuf
(
    OUT VppMsgCbHdl      *phdlMsgCb,
    IN VppMemCpHdl        hdlMemCp,
    IN VppTokenCbHdl      hdlTokenCb,
    IN VPP_UINT16        *pusCharSet,
    IN VppStrBufHdl       hdlStringBuf,
    IN VPP_UINT32         ulCurPos
);

/******************************************************************************
 Function   : VppMsgCbDestroy

 Description: Destroy a MsgCb handle, release the relate memory of the MsgCb.
              There are two function can create the MsgCb handle, this function
              can release the resource automatically.

 Input      : phdlMsgCb   - The handle is a MsgCb handle created by
                            "VppMsgCbCreate" or "VppMsgCbCreateFromStringBuf".

 Output     : phdlMsgCb   - After destroyed, will set the value to null.

 Return     : None
******************************************************************************/
VPP_VOID      VppMsgCbDestroy
(
    IO VppMsgCbHdl         *phdlMsgCb
);

/******************************************************************************
 Function   : VppMsgSetParam

 Description: Set parameter in the MsgCb, and the parameter is commonly used
              in some encoding and decoding context parameter.

 Input      : hdlMsgCb    - The handle is a MsgCb handle created by
                            "VppMsgCbCreate" or "VppMsgCbCreateFromStringBuf".
              pvParam     - Maybe encoding or decoding context parameters.

 Output     : None

 Return     : None
******************************************************************************/
VPP_VOID      VppMsgSetParam
(
    IN VppMsgCbHdl        hdlMsgCb,
    IN VPP_VOID          *pvParam
);

/******************************************************************************
 Function   : VppMsgGetParam

 Description: Get parameter from the MsgCb, and the parameter is commonly used
              in some encoding and decoding context parameter.

 Input      : hdlMsgCb    - The handle is a MsgCb handle created by
                            "VppMsgCbCreate" or "VppMsgCbCreateFromStringBuf".

 Output     : None.

 Return     : Content value if success, VPP_NULL_PTR if failure.
******************************************************************************/
VPP_VOID*    VppMsgGetParam
(
    IN VppMsgCbHdl        hdlMsgCb
);

/******************************************************************************
 Function   : VppMsgGetTotalPrintLen

 Description: Get total print length from the MsgCb. This means from the MsgCb
              create point to the current point(Function call point), what's the
              total print function produced the length.

 Input      : hdlMsgCb    - The handle is a MsgCb handle created by
                            "VppMsgCbCreate" or "VppMsgCbCreateFromStringBuf".

 Output     : None

 Return     : Total length of the print operation, 0 if failure.
******************************************************************************/
VPP_UINT32    VppMsgGetTotalPrintLen
(
    IN VppMsgCbHdl        hdlMsgCb
);

/******************************************************************************
 Function   : VppMsgGetTotalScanLen

 Description: Get total scan length from the MsgCb. This means from the MsgCb
              create point to the current point(Function call point), what's the
              total scan function absorbed length.

 Input      : hdlMsgCb    - The handle is a MsgCb handle created by
                            "VppMsgCbCreate" or "VppMsgCbCreateFromStringBuf".

 Output     : None

 Return     : Total length of the scan operation, 0 if failure.
******************************************************************************/
VPP_UINT32    VppMsgGetTotalScanLen
(
    IN VppMsgCbHdl        hdlMsgCb
);

/******************************************************************************
 Function   : VppMsgGetPrintLen

 Description: Get one time cal VppMsgPrintf print length from the MsgCb. This
              function will get the last time call VppMsgPrintf length.

 Input      : hdlMsgCb    - The handle is a MsgCb handle created by
                            "VppMsgCbCreate" or "VppMsgCbCreateFromStringBuf".

 Output     : None.

 Return     : Total length of the print operation, 0 if failure.
******************************************************************************/
VPP_UINT32    VppMsgGetPrintLen
(
    IN VppMsgCbHdl        hdlMsgCb
);

/******************************************************************************
 Function   : VppMsgGetScanLen

 Description: Get one time cal VppMsgScanf scan length from the MsgCb. This
              function will get the last time call VppMsgScanf length.

 Input      : hdlMsgCb    - The handle is a MsgCb handle created by
                            "VppMsgCbCreate" or "VppMsgCbCreateFromStringBuf".

 Output     : None

 Return     : Length of the scan operation, 0 if failure.
******************************************************************************/
VPP_UINT32    VppMsgGetScanLen
(
    IN VppMsgCbHdl        hdlMsgCb
);

/******************************************************************************
 Function   : VppMsgGetMemCpHdl

 Description: Get MemCp handle which specified when call "VppMsgCbCreate" or
              "VppMsgCbCreateFromStringBuf".

 Input      : hdlMsgCb    - The handle is a MsgCb handle created by
                            "VppMsgCbCreate" or "VppMsgCbCreateFromStringBuf".

 Output     : None.

 Return     : MemCp handle or VPP_NULL_PTR
******************************************************************************/
VppMemCpHdl   VppMsgGetMemCpHdl
(
    IN VppMsgCbHdl        hdlMsgCb
);

/******************************************************************************
 Function   : VppMsgGetLastErr

 Description: Get last error for the latest VppMsgScanf or VppMsgPrintf
              operation. This function will calls only if cares error detail.

 Input      : hdlMsgCb    - The handle is a MsgCb handle created by
                            "VppMsgCbCreate" or "VppMsgCbCreateFromStringBuf".

 Output     : None.

 Return     : VPP_SUCCESS if success, VPP_INVALID_PARAM if parameter is invalid.
******************************************************************************/
VPP_UINT32    VppMsgGetLastErr
(
    IN VppMsgCbHdl        hdlMsgCb
);

/******************************************************************************
 Function   : VppMsgScanf

 Description: Scan the msg base on the format string, this function support
              variant paramets to call.
              Like:
                VppMsgScanf(hdlMsgCb, "xxxxx", param1, param2, param3);
                VppMsgScanf(hdlMsgCb, "yyyy", param1, param2);
                ...

 Input      : hdlMsgCb    - The handle is a MsgCb handle created by
                            "VppMsgCbCreate" or "VppMsgCbCreateFromStringBuf".
              pcFormat    - Rule string of scan operation, About the format
                            syntax please refer to other document.
              others      - depend on the format string

 Output     : hdlMsgCb    - Update result after operate the message
              others      - depend on the format string

 Return     : VPP_SUCCESS if success, others if failure. MsgCb will save the
              last error code. Can use the VppMsgGetLastErr to get the error
              code.
******************************************************************************/
VPP_UINT32    VppMsgScanf
(
    IO VppMsgCbHdl        hdlMsgCb,
    IN CONST VPP_CHAR    *pcFormat,
    ...
);

/******************************************************************************
 Function   : VppMsgPrintf

 Description: Print the msg base on the format string, this function support
              variant paramets to call.
              Like:
                VppMsgPrintf(hdlMsgCb, "xxxxx", param1, param2, param3);
                VppMsgPrintf(hdlMsgCb, "yyyy", param1, param2);
                ...

 Input      : hdlMsgCb    - The handle is a MsgCb handle created by
                            "VppMsgCbCreate" or "VppMsgCbCreateFromStringBuf".
              pcFormat    - Rule string of print operation, About the format
                            syntax please refer to other document.
              others      - depend on the format string

 Output     : hdlMsgCb    - Update result after operate the message
              others      - depend on the format string

 Return     : VPP_SUCCESS if success, others if failure. MsgCb will save the
              last error code. Can use the VppMsgGetLastErr to get the error
              code.
******************************************************************************/
VPP_UINT32    VppMsgPrintf
(
    IO VppMsgCbHdl        hdlMsgCb,
    IN CONST VPP_CHAR    *pcFormat,
    ...
);


/******************************************************************************

                              End of file

******************************************************************************/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __VPP_MSGCB_X__ */

