/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName   : sstptintf.x

    Author     : Sajeesh

    Version    : 1.0

    Date       : 2005-11-23

    Description: This Header file contains the APIs exposed by the TPT module


    Function List:
        1. SipTptLmRegOptFeature
        2. SipTptCheckSipVersion
        3. SipTptValidateReqMethod
        4. SipTptCheckProtocolMismatch
        5. SipTptLiStrRecvInd
        6. SipTptLiSndResultInd
        7. SipTptLiStrSend

    History:
        1. Date         : <YYYY-MM-DD>
           Tag          :
           Author       :
           Modification :
******************************************************************************/

#ifndef __SSTPTINTF_X__
#define __SSTPTINTF_X__

#include "sssipbase.h"
#include "ssdatatype.h"
#include "ssmessage.h"

#ifdef __cplusplus
extern "C" {
#endif


/* ulMsgFlag bit:  7 6 5 4 3 2 1 0
                             | | |- 1 , stateful; 0, stateless
                             | |--- 1 , request message; 0, resp message.
                             |----- 1 , need compress; 0, no need compress.
*/

#define TPT_MSGFLAG_STATEFUL      0x00000001
#define TPT_MSGFLAG_REQUEST       0x00000002
#define TPT_MSGFLAG_SIGCOMP       0x00000004

#define SIP_TPT_REQUEST           TPT_MSGFLAG_REQUEST
#define SIP_TPT_RESPONSE          0x00


#define SIP_TPT_SET_STATEFUL(ulMsgFlags)  ((ulMsgFlags) |= TPT_MSGFLAG_STATEFUL)

#define SIP_TPT_SET_COMPRESSION_NEEDED(ulMsgFlags)  \
                                          ((ulMsgFlags) |= TPT_MSGFLAG_SIGCOMP)

#define SIP_TPT_IS_STATEFUL(ulFlags) \
         (TPT_MSGFLAG_STATEFUL == ((ulFlags) & TPT_MSGFLAG_STATEFUL))

#define SIP_TPT_IS_REQUEST_MSG(ulFlags)  \
         (TPT_MSGFLAG_REQUEST == ((ulFlags) & TPT_MSGFLAG_REQUEST))

#define SIP_TPT_REQUIRE_COMPRESSION(ulFlags)  \
         (TPT_MSGFLAG_SIGCOMP == ((ulFlags) & TPT_MSGFLAG_SIGCOMP))


typedef struct _SipTptLiAuxDataS
{
    SS_UINT32 ulCompId;
    SS_UINT32 ulDeCompRefId;
}SipTptLiAuxDataS;


typedef EN_SIP_RESULT (*pFnSipTptCheckProtocolMismatch)
(
    SipMsgS  *pstSipMsg,
    SipTptNwInfoS *pstTptNwInfo
);

typedef EN_SIP_RESULT (*pFnSipTptCheckSipVersion)
(
    SipMsgS *pstSipMsg
);

typedef EN_SIP_RESULT (*pFnSipTptCheckReqMethod)
(
    SipMsgS *pstSipMsg
);

typedef struct _SipTptOptionalFeatures
{
    pFnSipTptCheckProtocolMismatch  pfnSipTptCheckProtocolMismatch;
    pFnSipTptCheckSipVersion        pfnSipTptCheckSipVersion;
    pFnSipTptCheckReqMethod         pfnSipTptCheckReqMethod;
}SipTptOptionalFeatures;


/******************************************************************************
 Function   : SipTptLmRegOptFeature

 Description: This API will be invoked by the TU component to register the
              tailorable features of Transport module.

 Input      : pstOptFeatures - Pointer to the structure containing the callback
                               functions pointers for
                               SipTptCheckProtocolMismatch(),
                               SipTptCheckSipVersion() and
                               SipTptCheckReqMethod() if they have to tailor
                               each feature.
 Output     : None

 Return     : SS_RET_SUCCESS or Error Code indicating FAILURE
******************************************************************************/

EN_SIP_RESULT SipTptLmRegOptFeature
(
    IN SipTptOptionalFeatures *pstOptFeatures

);

/******************************************************************************
 Function   : SipTptCheckSipVersion

 Description: This API will check whether the SIP version in the first line is
              valid or not.

 Input      : pstSipMsg       - Pointer to Parsed SIP Message

 Output     : None

 Return     : SS_RET_SUCCESS or Error Code indicating FAILURE
******************************************************************************/

EN_SIP_RESULT SipTptCheckSipVersion
(
    IN  SipMsgS     *pstSipMsg
);

/******************************************************************************
 Function   : SipTptValidateReqMethod

 Description: This API will check whether the SIP method in the first line of
              the request line and that in the CSeq header are same

 Input      : pstSipMsg       - Pointer to Parsed SIP Message

 Output     : None

 Return     : SS_RET_SUCCESS or Error Code indicating FAILURE
******************************************************************************/

EN_SIP_RESULT SipTptValidateReqMethod
(
    IN  SipMsgS     *pstSipMsg
);

/******************************************************************************
 Function   : SipTptCheckProtocolMismatch

 Description: This API will check whether the transport protocol in the received
              messsage is same as that in the trasport info.

 Input      : pstSipMsg       - Pointer to Parsed SIP Message
              pstTptNwInfo    - Pointer to the TPT info
 Output     : None

 Return     : SS_RET_SUCCESS or Error Code indicating FAILURE
******************************************************************************/

EN_SIP_RESULT SipTptCheckProtocolMismatch
(
    IN  SipMsgS       *pstSipMsg,
    IN  SipTptNwInfoS *pstTptNwInfo
);

/******************************************************************************
 Function   : SipTptLiStrRecvInd

 Description: This API will be invoked by the TptD component to send a message
              received from network to Transport.

 Input      : pstNwStr     - Pointer to the message received from network.
              pstTptNwInfo - Pointer to the network information containing
                             the source and network transport information
              pstAuxData   - If not NULL, contains the compartment id for
                             Sigcomp

 Output     : None

 Return     : SS_RET_SUCCESS or Error Code indicating FAILURE
******************************************************************************/

EN_SIP_RESULT SipTptLiStrRecvInd
(
    IN SipString        *pstNwStr,
    IN SipTptNwInfoS    *pstTptNwInfo,
    IN SipTptLiAuxDataS *pstAuxData
);

/******************************************************************************
 Function   : SipTptLiSndResultInd

 Description: This API will be invoked by the TptD component to notify the
              the result after sending a message to network.

 Input      : ulMsgUsrId        - Message user id.
              ulMsgUsrTimestamp - Message user timestamp
              enResultType      - Sending Result defined in
                                  EN_SIP_TPTD_SEND_RESULT
              pstTptInfo        - Pointer to the Tptinfo of the sent message
              pstAuxData        - If not NULL, this parameter contains the
                                  Sigcomp compartment id.
 Output     : None

 Return     : None
******************************************************************************/

SS_VOID SipTptLiSndResultInd
(
    IN SS_UINT32                ulMsgUsrId,
    IN SS_UINT32                ulMsgUsrTimestamp,
    IN EN_SIP_SEND_RESULT       enResultType,
    IN SipTptNwInfoS           *pstTptInfo,
    IN SS_UINT32                ulMsgFlags,
    IN SipTptLiAuxDataS        *pstAuxData
);

/******************************************************************************
 Function   : SipTptLiStrSend

 Description: This API will be invoked by the TpT module to send message to TpTd

 Input      : ulMsgUsrId        - Message user id.
              ulMsgUsrTimestamp - Message user timestamp
              pstStrToTptD      - It points to the message that will be sent to
                                  the network by TptD.
              ulMsgFlag         - It specified whether the message is SIP
                                  request message, whether the message user is
                                  stateful.

 ulMsgFlag bit:  7 6 5 4 3 2 1 0
                           | | |- 1 , stateful; 0, stateless
                           | |--- 1 , request message; 0, resp message.
                           |----- 1 , need compress; 0, no need compress.



              pstTptNwInfo       - It points to the Network information,
                                   specifying the source and destination
                                   addresses for the received network message.
              iTmpConPort        - When send response, the stack will use the
                                   temporal connection port to find the old
                                   connection at first
              pstViaHdr          - Pointer to TopVia

              pstAuxData         - Pointer to auxialiary data

 Output     : None

 Return     : None
******************************************************************************/

EN_SIP_RESULT SipTptLiStrSend
(
    IN SS_UINT32         ulMsgUsrId,
    IN SS_UINT32         ulMsgUsrTimeStamp,
    IN SipString        *pstrStrToTptD,
    IN SS_UINT32         ulMsgFlag,
    IN SipTptNwInfoS    *pstTptNwInfo,
    IN SS_INT            iTmpConPort,
    IN SipVia           *pstViaHdr,
    IN SipTptLiAuxDataS *pstAuxData
);



#ifdef __cplusplus
}
#endif

#endif

