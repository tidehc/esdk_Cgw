/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sdpattr.h

    Author          : zhaolei (ronnie)
    Version         : 1.0

    Date            : 2005-11-16
    Description     : This file contains the API declare of SDP module
                      extension part attribute sub module.
                      User can register this part function to use it.

    Function List   :
        1. SdpLibUtilDecodeDigit
        2. SdpLibUtilEncodeDigit
        3. SdpLibAttrDecodeFmtp
        4. SdpLibAttrEncodeFmtp
        5. SdpLibAttrDecodeRtpmap
        6. SdpLibAttrEncodeRtpmap
        7. SdpLibAttrDecodeConftype
        8. SdpLibAttrEncodeConftype
        9. SdpLibAttrDecodeOrient
        10.SdpLibAttrEncodeOrient
        11.SdpLibAttrDecodeT38RateMng
        12.SdpLibAttrEncodeT38RateMng
        13.SdpLibAttrDecodeT38ErrCorrection
        14.SdpLibAttrEncodeT38ErrCorrection
        15.SdpLibAttrDecodeRange
        16.SdpLibAttrEncodeRange
        17.SdpLibAttrDecodeCurr
        18.SdpLibAttrEncodeCurr
        19.SdpLibAttrDecodeDes
        20.SdpLibAttrEncodeDes

    History:
        1. Date: <2006-05-23>
        Tag: <Tag.YDHD02189>
        Author: Manjunath Warad
        Modification: Decode & Encode of Pgmpu is moved to sample code.

        2. Date: <2006-05-23>
        Tag: <Tag.YDHD02190>
        Author: Manjunath Warad
        Modification: Decode & Encode of cliprect is moved to sample code.

        3. Date: <2006-05-23>
        Tag: <Tag.YDHD02191>
        Author: Manjunath Warad
        Modification: Decode & Encode of mpeg4-iod is moved to sample code.
******************************************************************************/
#ifndef __SDP_ATTR_H__
#define __SDP_ATTR_H__

#include "sdpstructure.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Default encoding name parameter value */
#define SDP_DEFAULT_ENCODING_NAME_PARAM 1

/* Property type attribute size */
#define SDP_ATTR_PROPERTY_SIZE          0

/******************************************************************************
 Function      : SdpLibUtilDecodeDigit

 Description   : Decode digit field of the SDP message.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pulDigit      - Digit result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibUtilDecodeDigit
(
    IO VppMsgCbHdl             hdlMsgCb,
    OUT SDP_UINT32            *pulDigit
);

/******************************************************************************
 Function      : SdpLibUtilEncodeDigit

 Description   : Encoding the digit field of the SDP.

 Input         : hdlMsgCb      - Handle of MsgCb
                 pulDigit      - Digit value pointer

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibUtilEncodeDigit
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SDP_UINT32             *pulDigit
);

/******************************************************************************
 Function      : SdpLibAttrDecodeFmtp

 Description   : Decode fmtp field of the SDP message. This function can
                 decode the user extension attribute.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pstFmtp       - Fmtp result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrDecodeFmtp
(
    IN VppMsgCbHdl             hdlMsgCb,
    OUT SdpAttrFmtpS          *pstFmtp
);

/******************************************************************************
 Function      : SdpLibAttrEncodeFmtp

 Description   : Encoding the fmtp field of the SDP. This function can
                 encode the user extension attribute.

 Input         : hdlMsgCb      - Handle of MsgCb
                 pstFmtp       - Fmtp pointer

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrEncodeFmtp
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SdpAttrFmtpS           *pstFmtp
);

/******************************************************************************
 Function      : SdpLibAttrDecodeRtpmap

 Description   : Decode rtpmap field of the SDP message. This function are
                 relate to the FMTP dynamic payload extension.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pstRtpmap     - Rtpmap result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrDecodeRtpmap
(
    IO VppMsgCbHdl             hdlMsgCb,
    OUT SdpAttrRtpmapS        *pstRtpmap
);

/******************************************************************************
 Function      : SdpLibAttrEncodeRtpmap

 Description   : Encoding the rtpamp field of the SDP.

 Input         : hdlMsgCb      - Handle of MsgCb
                 pstRtpmap     - Rtpmap pointer

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrEncodeRtpmap
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SdpAttrRtpmapS         *pstRtpmap
);

/******************************************************************************
 Function      : SdpLibAttrDecodeConftype

 Description   : Decode attribute type field of the SDP message.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pulConfType   - Conference type result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrDecodeConftype
(
    IO VppMsgCbHdl             hdlMsgCb,
    OUT SDP_UINT32            *pulConfType
);

/******************************************************************************
 Function      : SdpLibAttrEncodeConftype

 Description   : Encoding the attribute type field of the SDP.

 Input         : hdlMsgCb      - Handle of MsgCb
                 pulConfType   - Attribute type value pointer

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrEncodeConftype
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SDP_UINT32             *pulConfType
);

/******************************************************************************
 Function      : SdpLibAttrDecodeOrient

 Description   : Decode orient field of the SDP message.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pulOrient     - Orient type result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrDecodeOrient
(
    IN VppMsgCbHdl             hdlMsgCb,
    OUT SDP_UINT32            *pulOrient
);

/******************************************************************************
 Function      : SdpLibAttrEncodeOrient

 Description   : Encoding the orient field of the SDP.

 Input         : hdlMsgCb      - Handle of MsgCb
                 pulOrient     - Orient value pointer

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrEncodeOrient
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SDP_UINT32             *pulOrient
);

/******************************************************************************
 Function      : SdpLibAttrDecodeT38RateMng

 Description   : Decode T38 rate management field of the SDP message.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pulRateMng    - T38 rate management result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrDecodeT38RateMng
(
    IO VppMsgCbHdl             hdlMsgCb,
    OUT SDP_UINT32            *pulRateMng
);

/******************************************************************************
 Function      : SdpLibAttrEncodeCliprect

 Description   : Encoding the T38 rate management field of the SDP.

 Input         : hdlMsgCb      - Handle of MsgCb
                 pulDataRateMng- T38 rate management value pointer

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrEncodeT38RateMng
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SDP_UINT32             *pulRateMng
);

/******************************************************************************
 Function      : SdpLibAttrDecodeT38ErrCorrection

 Description   : Decode T38 error correction field of the SDP message.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pulEc         - T38 error correction result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrDecodeT38ErrCorrection
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SDP_UINT32             *pulEc
);

/******************************************************************************
 Function      : SdpLibAttrEncodeT38ErrCorrection

 Description   : Encoding the T38 error correction field of the SDP.

 Input         : hdlMsgCb      - Handle of MsgCb
                 pulEc         - Error correction value pointer

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrEncodeT38ErrCorrection
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SDP_UINT32             *pulEc
);

/******************************************************************************
 Function      : SdpLibAttrDecodeRange

 Description   : Decode rtsp range field of the SDP message.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pstAttrRange  - Range result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrDecodeRange
(
    IO VppMsgCbHdl             hdlMsgCb,
    OUT SdpAttrRangeS         *pstAttrRange
);

/******************************************************************************
 Function      : SdpLibAttrEncodeRange

 Description   : Encoding the rtsp range field of the SDP.

 Input         : hdlMsgCb      - Handle of MsgCb
                 pstAttrRange  - Range pointer

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrEncodeRange
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SdpAttrRangeS          *pstAttrRange
);

/******************************************************************************
 Function      : SdpLibAttrDecodeCurr

 Description   : Decode current status field of the SDP message.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pstAttrCurr   - Current status result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrDecodeCurr
(
    IO VppMsgCbHdl             hdlMsgCb,
    OUT SdpAttrCurrS          *pstAttrCurr
);

/******************************************************************************
 Function      : SdpLibAttrEncodeCurr

 Description   : Encoding the current status field of the SDP.

 Input         : hdlMsgCb      - Handle of MsgCb
                 pstAttrCurr   - Current status pointer

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrEncodeCurr
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SdpAttrCurrS           *pstAttrCurr
);

/******************************************************************************
 Function      : SdpLibAttrDecodeDes

 Description   : Decode status description field of the SDP message.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pstAttrDes    - Status description result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrDecodeDes
(
    IO VppMsgCbHdl             hdlMsgCb,
    OUT SdpAttrDesS           *pstAttrDes
);

/******************************************************************************
 Function      : SdpLibAttrEncodeDes

 Description   : Encoding the status description field of the SDP.

 Input         : hdlMsgCb      - Handle of MsgCb
                 pstAttrDes    - Status description pointer

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibAttrEncodeDes
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SdpAttrDesS            *pstAttrDes
);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __SDP_ATTR_H__ */

