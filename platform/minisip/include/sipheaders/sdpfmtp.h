/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sdpfmtp.h

    Author          : zhaolei (ronnie)

    Version         : 1.0

    Date            : 2005-11-16

    Description     : This file contains the declares of SDP module
                      extension part Fmtp sub module.
                      User can register this part function to use it.

    Function List   :
        1. SdpLibDecodeFmtpH261
        2. SdpLibEncodeFmtpH261
        3. SdpLibDecodeFmtpH264
        4. SdpLibEncodeFmtpH264
        5. SdpLibDecodeFmtpMpeg1_2
        6. SdpLibEncodeFmtpMpeg1_2
        7. SdpLibDecodeFmtpMpeg4
        8. SdpLibEncodeFmtpMpeg4

    History:
        1.  Date         : <2006-05-30>
            Tag          : <Tag.YDHD02328>
            Author       : Manjunath Warad
            Modification : Decode & Encode of H263 is moved to sample code.
******************************************************************************/
#ifndef __SDP_FMTP_H__
#define __SDP_FMTP_H__

#include "sdpstructure.h"

#ifdef __cplusplus
extern "C"
{
#endif

/******************************************************************************
 Function      : SdpLibDecodeFmtpH261

 Description   : Decode fmtp h261 field of the SDP message.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pstFmtpH261   - Fmtp H261 result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibDecodeFmtpH261
(
    IO VppMsgCbHdl             hdlMsgCb,
    OUT SdpListParamS         *pstFmtpH261
);

/******************************************************************************
 Function      : SdpLibEncodeFmtpH261

 Description   : Encoding the h261 field of the SDP.

 Input         : hdlMsgCb      - Handle of MsgCb
                 pstFmtpH261   - Fmtp H261 pointer

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibEncodeFmtpH261
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SdpListParamS          *pstFmtpH261
);

/******************************************************************************
 Function      : SdpLibDecodeFmtpH264

 Description   : Decode h264 field of the SDP message.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pstFmtpH264   - H264 result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibDecodeFmtpH264
(
    IO VppMsgCbHdl             hdlMsgCb,
    OUT SdpListParamS         *pstFmtpH264
);

/******************************************************************************
 Function      : SdpLibEncodeFmtpH264

 Description   : Encoding the h264 field of the SDP.

 Input         : hdlMsgCb      - Handle of MsgCb
                 pstFmtpH264   - Fmtp h264 pointer

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibEncodeFmtpH264
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SdpListParamS          *pstFmtpH264
);

/******************************************************************************
 Function      : SdpLibDecodeFmtpMpeg1_2

 Description   : Decode mpeg1_2 field of the SDP message.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pulParam      - MPV parameter result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibDecodeFmtpMpeg1_2
(
    IO VppMsgCbHdl             hdlMsgCb,
    OUT SDP_UINT32            *pulParam
);

/******************************************************************************
 Function      : SdpLibEncodeFmtpMpeg1_2

 Description   : Encoding the Mpeg1_2 field of the SDP.

 Input         : hdlMsgCb      - Handle of MsgCb
                 ulParam       - MPV parameter value

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibEncodeFmtpMpeg1_2
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SDP_UINT32             *pulParam
);

/******************************************************************************
 Function      : SdpLibDecodeFmtpMpeg4

 Description   : Decode mpeg4 field of the SDP message.

 Input         : hdlMsgCb      - Handle of MsgCb

 Output        : hdlMsgCb      - Update by the decode some field.
                 pstFmtpMp4    - Mpeg4 result of decode.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibDecodeFmtpMpeg4
(
    IO VppMsgCbHdl             hdlMsgCb,
    OUT SdpListParamS         *pstFmtpMp4
);

/******************************************************************************
 Function      : SdpLibEncodeFmtpMpeg4

 Description   : Encoding the fmtp mpeg4 field of the SDP.

 Input         : hdlMsgCb      - Handle of MsgCb
                 pstFmtpMp4    - Fmtp mpeg4 parameter value

 Output        : hdlMsgCb      - Update by the encode process.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
SDP_UINT32       SdpLibEncodeFmtpMpeg4
(
    IO VppMsgCbHdl             hdlMsgCb,
    IN SdpListParamS          *pstFmtpMp4
);


#ifdef __cplusplus
} /* extern "c" */
#endif

#endif /* __SDP_FMTP_H__ */

