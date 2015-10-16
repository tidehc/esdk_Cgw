/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sdpapi.h

    Author          : zhaolei (ronnie)

    Version         : 1.0

    Date            : 2005-11-16

    Description     : This file contains the declare of SDP module
                      interface.

    Function List   :
        1. SdpLibInit
        2. SdpLibDeinit
        3. SdpLibRegFunction
        4. SdpLibRegFmtpType
        5. SdpLibCfgSet
        6. SdpLibCfgGet
        7. SdpLibCreatePdu
        8. SdpLibReleasePdu
        9. SdpLibDecodeMsg
        10.SdpLibDecodeStrBuf
        11.SdpLibEncodeMsg
        12.SdpLibEncodeStrBuf

    History:
        1. Date:
        Tag: None
        Author:
        Modification:
*******************************************************************************/

#ifndef __SDP_API_H__
#define __SDP_API_H__

#include "sdptypes.h"
#include "sdpenums.h"
#include "sdpstructure.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define SDP_VERSION "Vpp MiniSdp V2.1.0.5"

/******************************************************************************
 Function      : SdpLibInit

 Description   : Init the SDP module, function user must call this function
                 first then call other api functions. This function can't
                 call twice.

 Input         : None

 Output        : None

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
EN_SDP_RESULT    SdpLibInit
(
    SDP_VOID
);

/******************************************************************************
 Function      : SdpLibDeinit

 Description   : Deinit the SDP module, if need. After deinit this module
                 can't call any other API again.

 Input         : None

 Output        : None

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
EN_SDP_RESULT    SdpLibDeinit
(
    SDP_VOID
);

/******************************************************************************
 Function      : SdpLibRegFunction

 Description   : This function will register a process function to process the
                 extension field from structure to string or from string to
                 structure. The parameter "usEnumType" will be same to the
                 registered token value.

                 Features:
                 1. usElmntSize is zero, can use to register perperty type
                    attribute.
                 2. usElmntSize is SDP_NULL_UINT32, to unregister this
                    extension.
                 3. pfnDecode and pfnEncode two pointer both can be null.
                 4. usClass function type class is fixed, just support
                    attribute extension and fmtp extension.
                 5. usEnumType is the registered VppToken Id, User should
                    register the token first then register the extension
                    function.

 Input         : usClass       - Function class defined by SDP module,
                                 user can not extend.
                 usEnumType    - If it's a extension token, this value should
                                 be same to VppTokenReg 's usTokenValue
                                 parameter.
                 ulElmntSize   - Element size of the register element. And this
                                 element will used by the decode function and
                                 encode functions.
                 pfnDecode     - Element decode function pointer
                 pfnEncode     - Element encode function pointer

 Output        : None

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
EN_SDP_RESULT    SdpLibRegFunction
(
    IN EN_SDP_FUNC_CLASS       usClass,
    IN SDP_UINT16              usEnumType,
    IN SDP_UINT32              ulElmntSize,
    IN pFnSdpDecode            pfnDecode,
    IN pFnSdpEncode            pfnEncode
);

/******************************************************************************
 Function      : SdpLibRegFmtpType

 Description   : Register fmtp enumerate value bind with the fixed payload or
                 dynamic payload type. Dynamic payload type register using the
                 registered encoding name's token value.
                 Fixed example:
                 SdpLibRegFmtpType(SDP_PAYLOAD_FIXED, SDP_ATTR_FMTP_H263, 34);
                 Dynamic example:
                 SdpLibRegFmtpType(SDP_PAYLOAD_DYNAMIC, SDP_ATTR_FMTP_H261,
                       SDP_ATTR_ENC_NAME_H261);

 Input         : enPayloadType - Payload value type, maybe fixed or dynamic.
                 usFmtpType    - Fmtp enumerate value, please refer to
                                 EN_SDP_ATTR_FMTP_TYPE.
                 usParam       - This value maybe the payload value or
                                 encoding name' token value.

 Output        : None

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
EN_SDP_RESULT    SdpLibRegFmtpType
(
    IN EN_SDP_PAYLOAD_TYPE     enPayloadType,
    IN SDP_UINT16              usFmtpType,
    IN SDP_UINT16              usParam
);

/******************************************************************************
 Function      : SdpLibCfgSet

 Description   : Configurate a item to the SDP module. SDP user can change the
                 behavior or some internel data of the SDP component by using
                 this operation.

 Input         : enCfgId       - Configration item id. For the specific value
                                 meaning, please refer to EN_SDP_CFGID.
                 pvValue       - Configration item value. For the specific value
                                 meaning, please refer to EN_SDP_CFGID.

 Output        : None

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
EN_SDP_RESULT    SdpLibCfgSet
(
    IN EN_SDP_CFGID             enCfgId,
    IN SDP_VOID                *pvValue
);

/******************************************************************************
 Function      : SdpLibCfgGet

 Description   : Inquire a configuration item from the SDP moduel. User can
                 know the inside control parameters or information by this
                 operation.

 Input         : enCfgId       - Configration item id. For the specific value
                                 meaning, please refer to EN_SDP_CFGID.
                 pvValue       - Configration item value. For the specific value
                                 meaning, please refer to EN_SDP_CFGID.

 Output        : None

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
EN_SDP_RESULT    SdpLibCfgGet
(
    IN  EN_SDP_CFGID           enCfgId,
    OUT SDP_VOID              *pvValue
);

/******************************************************************************
 Function      : SdpLibCreatePdu

 Description   : Create SDP announce to simply the user operation.

 Input         : None

 Output        : ppstSdpAnn    - Create the SDP announce PDU for user.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
EN_SDP_RESULT    SdpLibCreatePdu
(
   OUT SdpAnnounceS          **ppstSdpAnn
);

/******************************************************************************
 Function      : SdpLibReleasePdu

 Description   : Destroy SDP announce to simply the user operation. And set
                 the *ppSdpObj to NULL.

 Input         : ppstSdpAnn    - The destroy target. Value is SdpAnnounce
                                 pointer.

 Output        : ppstSdpAnn    - After destroy this, will set this pointer to
                                 NULL.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
EN_SDP_RESULT    SdpLibReleasePdu
(
    IO   SdpAnnounceS        **ppSdpObj
);

/******************************************************************************
 Function      : SdpLibDecodeMsg

 Description   : Decode a continuous message to SDP Announce PDU. This function
                 decodes a SIP message Body filled into a SDP Announce
                 structure. If pulAbsorbed is not NULL, will output the
                 absorbed length of decoding process.

 Input         : pstSdpBody    - Sdp message body.

 Output        : ppstSdpAnn    - After decode finished, will create the whole
                                 SDP announce PDU.
                 pulAbsorbed   - The absorbed length of SDP decode function.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
EN_SDP_RESULT    SdpLibDecodeMsg
(
    IN  VppStringS            *pstSdpBody,
    OUT SdpAnnounceS         **ppstSdpAnn,
    OUT SDP_UINT32            *pulAbsorbed
);

/******************************************************************************
 Function      : SdpLibDecodeStrBuf

 Description   : Decode a strbuf message to SDP Announce PDU. This function
                 decodes a SIP message Body filled into a SDP Announce
                 structure. If pulAbsorbed is not NULL, will output the
                 absorbed length of decoding process.

 Input         : hdlSdpBody    - SDP strbuf message body.
                 ulCurPos      - Current position of the strbuf, is from the
                                 start to decode, please use zero.

 Output        : ppstSdpAnn    - After decode finished, will create the whole
                                 SDP announce PDU.
                 pulAbsorbed   - The absorbed length of SDP decode function.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
EN_SDP_RESULT    SdpLibDecodeStrBuf
(
    IN  VppStrBufHdl           hdlSdpBody,
    IN  SDP_UINT32             ulCurPos,
    OUT SdpAnnounceS         **ppstSdpAnn,
    OUT SDP_UINT32            *pulAbsorbed
);

/******************************************************************************
 Function      : SdpLibEncodeMsg

 Description   : Encode a SDP Announce PDU to SDP continuous string. This
                 function encoding a SDP announce structure into a SDP string
                 message.

 Input         : pstSdpAnn     - SDP announce PDU pointer.

 Output        : pstSdpBody    - The output continuous string.
                 pulEncLen     - The encoding length of SDP decode function.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
EN_SDP_RESULT    SdpLibEncodeMsg
(
    IN  SdpAnnounceS          *pstSdpAnn,
    OUT VppStringS            *pstSdpBody,
    OUT SDP_UINT32            *pulEncLen
);

/******************************************************************************
 Function      : SdpLibEncodeStrBuf

 Description   : Encode a SDP Announce PDU to the tail of a strbuf.

 Input         : pstSdpAnn     - SDP announce PDU pointer.
                 hdlSdpBody    - The target of the SDP output strbuf.

 Output        : hdlSdpBody    - SDP message will append into this strbuf.
                 pulEncLen     - The encoding length of SDP decode function.

 Return        : SDP_RET_SUCCESS if success, others if failure.
******************************************************************************/
EN_SDP_RESULT    SdpLibEncodeStrBuf
(
    IN  SdpAnnounceS          *pstSdpAnn,
    IO  VppStrBufHdl           hdlSdpBody,
    OUT SDP_UINT32            *pulEncLen
);

/******************************************************************************
 Function      : SdpLibGetVersion

 Description   : To Get the Version of SDP Release.

 Input         : None.

 Output        : None.

 Return        : Returns the verion of SDP.
******************************************************************************/
SDP_CHAR* SdpLibGetVersion
(
    SDP_VOID
);

/******************************************************************************
 Function      : SdpLibGetTestVersion

 Description   : To Get the Version of SDP Release for testing team.

 Input         : None.

 Output        : None.

 Return        : Returns the verion of SDP.
******************************************************************************/
SDP_CHAR* SdpLibGetTestVersion
(
    SDP_VOID
);

#ifdef __cplusplus
}
#endif

#endif /* __SDP_API_H__ */

