/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sdpenums.h

    Author          : zhaolei (ronnie)

    Version         : 1.0

    Date            : 2005-11-24
    Description     : This header file defined enumerate for SDP module.

    Function List   : None

    History:
        1. Date: <DD-MM-HH>
        Tag: None
        Author:
        Modification:
******************************************************************************/
#ifndef _SDP_ENUMS_H_
#define _SDP_ENUMS_H_

#include "sdptypes.h"

#ifdef __cplusplus
extern "C"
{
#endif

/* Define the configuration item of the SDP */
typedef enum
{
    /* Policy data begin id */
    SDP_CFGID_POLICY_BEGIN = 0,

    /* Save decode failure option */
    SDP_CFGID_SAVEDECFAIL = SDP_CFGID_POLICY_BEGIN,

    /* Encoding decode failure option */
    SDP_CFGID_ENCDECFAIL,

    /* Save unknown field option */
    SDP_CFGID_SAVEUNKNOWN,

    /* Encoding unknown field option */
    SDP_CFGID_ENCUNKNOWN,

    /* Decode process failure option */
    SDP_CFGID_DECFAIL,

    /* Decode process unknown option */
    SDP_CFGID_UNKNOWN,

    /* Decode check strick sequence */
    SDP_CFGID_DECSTRICTSEQ,

    /* Skip encode error */
    SDP_CFGID_SKIPENCERR,

    /* Policy data end id */
    SDP_CFGID_POLICY_END = SDP_CFGID_SKIPENCERR,

    /* Max encoding continuous buffer size, encoding process
       will pre-allocated memory, if the memory is not enough,
       and will return failure */
    SDP_CFGID_MAXENCSIZE,

    /* The token handle of SDP */
    SDP_CFGID_TKNHDL,

    /* Extern configuration item id */
    SDP_CFGID_EXTN,
    SDP_CFGID_END = SDP_ENUM_END
}EN_SDP_CFGID;

/* Define SDP module start error code */
#define SDP_RET_START  0x1000

/* Define the ret code of SDP */
typedef enum
{
    /* success */
    SDP_RET_SUCCESS = VPP_SUCCESS,

    /* normal failure */
    SDP_RET_FAILURE = VPP_FAILURE,

    /* un initialize to the SDP module */
    SDP_RET_UNINIT  = SDP_RET_START,

    /* re initialize to the SDP module */
    SDP_RET_REINIT,

    /* No memory */
    SDP_RET_NOMEM,

    /* Invalid argument */
    SDP_RET_INVALIDARG,

    /* Can't set value in the configration item */
    SDP_RET_CANNOTCHG,

    /* Invalid configration item */
    SDP_RET_INVALIDCFGITEM,

    /* Invalid SDP announce structure */
    SDP_RET_INVALIDSDPANN,

    /* Assert error only valid in the debug version */
    SDP_RET_ASSERTERR,

    /* Invalid sequence when decoding */
    SDP_RET_DECSEQERR,

    /* Message is finished */
    SDP_RET_MSGEND,

    /* Decode failure */
    SDP_RET_DEC_FAILURE,

    SDP_RET_ENC_PASS,

    /* Encode failure */
    SDP_RET_ENC_FAIL,

    SDP_RET_EXTN,
    SDP_RET_END = SDP_ENUM_END
}EN_SDP_RESULT;

/* SDP parameter ID */
typedef enum
{
    SDP_INVALID_PARAM = 0,
    SDP_V,
    SDP_O, /* 2*/
    SDP_S,
    SDP_I,
    SDP_U,
    SDP_E,
    SDP_P,
    SDP_C,
    SDP_B,
    SDP_T, /* 10 */
    SDP_R,
    SDP_Z,
    SDP_K,
    SDP_A,
    SDP_M  /* 15 */
}EN_SDP_PARAM;

typedef enum
{
    SDP_M_I = SDP_M + 1,
    SDP_M_C,
    SDP_M_B,
    SDP_M_K,
    SDP_M_A /* 20 */
}EN_SDP_MEDIA;

/* This enumerate will used for control the encoding and
   decoding process */
typedef enum
{
    SDP_CONTEXT_STATE_UNKNOW = 0,

    /* Context is in the init state */
    SDP_CONTEXT_STATE_INIT = 1,

    /* Context is in the parameter state */
    SDP_CONTEXT_STATE_PARAM,

    /* Context is in the time field state */
    SDP_CONTEXT_STATE_TIME,

    /* Context is in the media field state */
    SDP_CONTEXT_STATE_MEDIA,

    /* Context is met next sdp message, so current message is finished */
    SDP_CONTEXT_STATE_FIN,

    SDP_CONTEXT_STATE_EXTN,
    SDP_CONTEXT_STATE_END = SDP_ENUM_END
}EN_SDP_CONTEXT_STATE_TYPE;

/* This domain will save in the decoding failure field to
   tell user, where decode failure */
typedef enum
{
    SDP_CONTEXT_DOMAIN_UNKNOW = 0,

    /* Context is in the parameter decode domain */
    SDP_CONTEXT_DOMAIN_PARAM,

    /* Context is in the attribute decode domain */
    SDP_CONTEXT_DOMAIN_ATTR,

    SDP_CONTEXT_DOMAIN_EXTERN,
    SDP_CONTEXT_DOMAIN_END = SDP_ENUM_END
}EN_SDP_CONTEXT_DOMAIN_TYPE;


/* Define the extension function type, now SDP support two type
   function extension */
typedef enum
{
    /* Attribute type extension */
    SDP_FUNC_CLASS_ATTR,

    /* Fmtp type extension */
    SDP_FUNC_CLASS_FMTP,

    SDP_FUNC_CLASS_EXTN,
    SDP_FUNC_CLASS_END = SDP_ENUM_END
}EN_SDP_FUNC_CLASS;

/* Define the extension fmtp payload type, now SDP support two type
   payload type */
typedef enum
{
    /* Support register fixed value of the payload,
       this condition should register the payload value. And
       now SDP only support value type paylaod and can't support
       strig type payload */
    SDP_PAYLOAD_FIXED,

    /* Support register dynamic value of the payload, this condition
       should register the encoding name not the payload value,
       because of the payload value is not fixed. */
    SDP_PAYLOAD_DYNAMIC,

    SDP_PAYLOAD_EXTN,
    SDP_PAYLOAD_END = SDP_ENUM_END
}EN_SDP_PAYLOAD_TYPE;

/* Define the token type of SDP common */
typedef enum
{
    /* Network token type */
    SDP_TKN_TYPE_NETWORK,

    /* Address token type */
    SDP_TKN_TYPE_ADDR,

    /* Bandwidth token type */
    SDP_TKN_TYPE_BW_MODF,

    /* Key method token type */
    SDP_TKN_TYPE_KEY_METHOD,

    /* Attribute token type */
    SDP_TKN_TYPE_ATTR,

    /* Encoding name token type */
    SDP_TKN_TYPE_ATTR_ENC_NAME,

    /* Orient token type */
    SDP_TKN_TYPE_ATTR_ORIENT,

    /* Conference token type */
    SDP_TKN_TYPE_ATTR_CONF_TYPE,

    /* Media token type */
    SDP_TKN_TYPE_MEDIA,

    /* Media transport token type */
    SDP_TKN_TYPE_MEDIA_TRANSPORT,

    /* T38 data rate management token type */
    SDP_TKN_TYPE_ATTR_T38DRMM,

    /* T38 error correction token type */
    SDP_TKN_TYPE_ATTR_T38EC,

    /* H261 token type */
    SDP_TKN_TYPE_ATTR_H261,

    /* H263 token type */
    SDP_TKN_TYPE_ATTR_H263,

    /* H263 cap size token type */
    SDP_TKN_TYPE_ATTR_H263_SIZE,

    /* H263 cap param token type */
    SDP_TKN_TYPE_ATTR_H263_PARAM,

    /* H263 cap options token type */
    SDP_TKN_TYPE_ATTR_H263_OPT,

    /* H263 request token type */
    SDP_TKN_TYPE_ATTR_H263_REQ,

    /* H263 PLUS (H263 1998 and H263 2000) token type */
    SDP_TKN_TYPE_ATTR_H263_PLUS,

    /* H264 token type */
    SDP_TKN_TYPE_ATTR_H264,

    /* MPEG1 token type */
    SDP_TKN_TYPE_ATTR_MPEG12,

    /* MPEG1 token type */
    SDP_TKN_TYPE_ATTR_MPEG4,

    /* rtsp range toekn type */
    SDP_TKN_TYPE_ATTR_RANGE,

    /* sip toekn type */
    SDP_TKN_TYPE_ATTR_SIP,

    /* pgmpu toekn type */
    SDP_TKN_TYPE_ATTR_PGMPU,

    /* MPEG4 IOD toekn type */
    SDP_TKN_TYPE_ATTR_MPEG4IOD,

    SDP_TKN_TYPE_EXTN,
    SDP_TKN_TYPE_END = SDP_ENUM_END
}EN_SDP_TKN_TYPE;

/* Define the network token type token value */
typedef enum
{
    SDP_NET_TYPE_UNKOWN = 0,

    /* Define the "IN" network type */
    SDP_NET_TYPE_IN,

    /* Define the "H221" network type */
    SDP_NET_TYPE_H221,

    /* Define the "H223" network type */
    SDP_NET_TYPE_H223,

    SDP_NET_TYPE_EXTN,
    SDP_NET_TYPE_END = SDP_ENUM_END
}EN_SDP_NET_TYPE;

/* Define the address type value */
typedef enum
{
    SDP_ADDR_TYPE_UNKNOWN = 0,

    /* Address type "IP4" */
    SDP_ADDR_TYPE_IP4,

    /* Address type "IP6" */
    SDP_ADDR_TYPE_IP6,

    SDP_ADDR_TYPE_EXTN,
    SDP_ADDR_TYPE_END = SDP_ENUM_END
}EN_SDP_ADDR_TYPE;

/* Define the bandwidth mod token value */
typedef enum
{
    SDP_BW_MOD_UNKNOWN = 0,

    /* Bandwidth type "CT" */
    SDP_BW_MOD_CT,

    /* Bandwidth type "AS" */
    SDP_BW_MOD_AS,

    /* Bandwidth type "RS" */
    SDP_BW_MOD_RS,

    /* Bandwidth type "RR" */
    SDP_BW_MOD_RR,

    /* Bandwidth type "X-AV" */
    SDP_BW_MOD_X_AV,

    SDP_BW_MOD_EXTN,
    SDP_BW_MOD_END = SDP_ENUM_END
}EN_SDP_MOD_TYPE;

/* Define the key token value */
typedef enum
{
    SDP_KEY_UNKNOW = 0,

    /* Key value "clear" */
    SDP_KEY_CLEAR,

    /* Key value "base64" */
    SDP_KEY_BASE64,

    /* Key value "uri" */
    SDP_KEY_URI,

    /* Key value "prompt" */
    SDP_KEY_PROMPT,

    SDP_KEY_EXTN,
    SDP_KEY_END = SDP_ENUM_END
}EN_SDP_KEY_TYPE;

/* Define the attribute value */
typedef enum
{
    SDP_ATTR_UNKOWN = 0,

    /* RFC 2327 default att-field */
    /* attribute "cat" */
    SDP_ATTR_CAT,

    /* attribute "rtpmap" */
    SDP_ATTR_RTPMAP,

    /* attribute "keywords" */
    SDP_ATTR_KEYWDS,

    /* attribute "tool" */
    SDP_ATTR_TOOL,

    /* attribute "ptime" */
    SDP_ATTR_PTIME,

    /* attribute "recvonly" */
    SDP_ATTR_RECVONLY,

    /* attribute "sendrecv" */
    SDP_ATTR_SENDRECV,

    /* attribute "sendonly" */
    SDP_ATTR_SENDONLY,

    /* attribute "orient" */
    SDP_ATTR_ORIENT,

    /* attribute "type" */
    SDP_ATTR_TYPE,

    /* attribute "charset" */
    SDP_ATTR_CHARSET,

    /* attribute "sdplang" */
    SDP_ATTR_SDPLANG,

    /* attribute "lang" */
    SDP_ATTR_LANG,

    /* attribute "frmrate" */
    SDP_ATTR_FRMRATE,

    /* attribute "quality" */
    SDP_ATTR_QUALITY,

    /* attribute "fmtp" */
    SDP_ATTR_FMTP,

    /* attribute "T38FaxVersion" */
    SDP_ATTR_T38FAXVER,

    /* attribute "T38MaxBitRate" */
    SDP_ATTR_T38MAXBITRATE,

    /* attribute "T38FaxFillBitRemoval" */
    SDP_ATTR_T38FAXFILLBITRMV,

    /* attribute "T38FaxTranscodingMMR" */
    SDP_ATTR_T38TRANSCODINGMMR,

    /* attribute "T38FaxTranscodingJBIG" */
    SDP_ATTR_T38TRANSCODINGJBIG,

    /* attribute "T38FaxRateManagement" */
    SDP_ATTR_T38FAXRATEMNGT,

    /* attribute "T38FaxMaxBuffer" */
    SDP_ATTR_T38FAXMAXBUF,

    /* attribute "T38FaxMaxDatagram" */
    SDP_ATTR_T38FAXMAXDATAGRAM,

    /* attribute "T38FaxUdpEC" */
    SDP_ATTR_T38FAXUDPEC,

    /* attribute "t38errctl" */
    SDP_ATTR_T38ERRCTL,

    /* attribute "control" */
    SDP_ATTR_CONTROL,

    /* attribute "range" */
    SDP_ATTR_RANGE,

    /* attribute "etag" */
    SDP_ATTR_ETAG,

    /* attribute "curr" */
    SDP_ATTR_CURR,

    /* attribute "des" */
    SDP_ATTR_DES,

    /* attribute "conf" */
    SDP_ATTR_CONF,

    /* attribute "maxptime" */
    SDP_ATTR_MAXPTIME,

    /* attribute "cliprect" */
    SDP_ATTR_CLIPRECT,

    /* attribute "mpeg4-iod" */
    SDP_ATTR_MPEG4_IOD,

    /* attribute "isma-compliance" */
    SDP_ATTR_ISMA_COMPLIANCE,

    /* attribute "mpeg4-esid" */
    SDP_ATTR_MPEG4_ESID,

    /* attribute "stream" */
    SDP_ATTR_STREAM,

    /* attribute "pgmpu" */
    SDP_ATTR_PGMPU,

    /* attribute "maxps" */
    SDP_ATTR_MAXPS,

    /* attribute "framesize" */
    SDP_ATTR_FRMSIZE,

    /* attribute "x-envivio-verid" */
    SDP_ATTR_X_ENVIVIO_VERID,

    SDP_ATTR_EXTN,
    SDP_ATTR_END = SDP_ENUM_END
}EN_SDP_ATTR_TYPE;

/* Define the encoding name token value, this token id can register
   as the dynamic payload type */
typedef enum
{
    SDP_ATTR_ENC_NAME_UNKNOWN,

    /* encoding name "H261" */
    SDP_ATTR_ENC_NAME_H261,

    /* encoding name "H263-1998" */
    SDP_ATTR_ENC_NAME_H263_1998,

    /* encoding name "H263-2000" */
    SDP_ATTR_ENC_NAME_H263_2000,

    /* encoding name "MP4A-LATM" */
    SDP_ATTR_ENC_NAME_MP4A,

    /* encoding name "MP4V-ES" */
    SDP_ATTR_ENC_NAME_MP4V,

    /* encoding name "telephone-event" */
    SDP_ATTR_ENC_NAME_TELE_EVENT,

    /* encoding name "tone" */
    SDP_ATTR_ENC_NAME_TONE,

    /* encoding name "mpa-robust" */
    SDP_ATTR_ENC_NAME_MPAROBUST,

    /* encoding name "mp3" */
    SDP_ATTR_ENC_NAME_MP3,

    /* encoding name "DIV4" */
    SDP_ATTR_ENC_NAME_DIV4,

    /* encoding name "G722" */
    SDP_ATTR_ENC_NAME_G722,

    /* encoding name "G723" */
    SDP_ATTR_ENC_NAME_G723,

    /* encoding name "G726-16" */
    SDP_ATTR_ENC_NAME_G726_16,

    /* encoding name "G726-24" */
    SDP_ATTR_ENC_NAME_G726_24,

    /* encoding name "G726-32" */
    SDP_ATTR_ENC_NAME_G726_32,

    /* encoding name "G726-40" */
    SDP_ATTR_ENC_NAME_G726_40,

    /* encoding name "G728" */
    SDP_ATTR_ENC_NAME_G728,

    /* encoding name "G729" */
    SDP_ATTR_ENC_NAME_G729,

    /* encoding name "G729D" */
    SDP_ATTR_ENC_NAME_G729D,

    /* encoding name "G729E" */
    SDP_ATTR_ENC_NAME_G729E,

    /* encoding name "GSM" */
    SDP_ATTR_ENC_NAME_GSM,

    /* encoding name "GSM-EFR" */
    SDP_ATTR_ENC_NAME_GSM_EFR,

    /* encoding name "PCMA" */
    SDP_ATTR_ENC_NAME_PCMA,

    /* encoding name "PCMU" */
    SDP_ATTR_ENC_NAME_PCMU,

    /* encoding name "LPC" */
    SDP_ATTR_ENC_NAME_LPC,

    /* encoding name "L16" */
    SDP_ATTR_ENC_NAME_L16,

    /* encoding name "QCELP" */
    SDP_ATTR_ENC_NAME_QCELP,

    /* encoding name "CN" */
    SDP_ATTR_ENC_NAME_CN,

    /* encoding name "MPA" */
    SDP_ATTR_ENC_NAME_MPA,

    /* encoding name "L8" */
    SDP_ATTR_ENC_NAME_L8,

    /* encoding name "RED" */
    SDP_ATTR_ENC_NAME_RED,

    /* encoding name "VDVI" */
    SDP_ATTR_ENC_NAME_VDVI,

    /* encoding name "CelB" */
    SDP_ATTR_ENC_NAME_CEIB,

    /* encoding name "JPEG" */
    SDP_ATTR_ENC_NAME_JPEG,

    /* encoding name "nv" */
    SDP_ATTR_ENC_NAME_NV,

    /* encoding name "MPV" */
    SDP_ATTR_ENC_NAME_MPV,

    /* encoding name "h264" */
    SDP_ATTR_ENC_NAME_H264,

    /* encoding name "amr" */
    SDP_ATTR_ENC_NAME_AMR,

    /* encoding name "mpeg4-generic" */
    SDP_ATTR_ENC_NAME_MPEG4_GENERIC,

    /* encoding name "x-asf-pf" */
    SDP_ATTR_ENC_NAME_X_ASF_PF,

    /* encoding name "x-wms-rtx" */
    SDP_ATTR_ENC_NAME_X_WMS_RTX,

    SDP_ATTR_ENC_EXTN,
    SDP_ATTR_ENC_END = SDP_ENUM_END
}EN_SDP_ATTR_ENC_TYPE;

/* sdp attribute orient */
typedef enum
{
    SDP_ATTR_ORIENT_UNKNOWN = 0,

    /* orient token type "portrait" */
    SDP_ATTR_ORIENT_PORTRAIT,

    /* orient token type "landscape" */
    SDP_ATTR_ORIENT_LANDSCAPE,

    /* orient token type "seascape" */
    SDP_ATTR_ORIENT_SEASCAPE,

    SDP_ATTR_ORIENT_EXTN,
    SDP_ATTR_ORIENT_END = SDP_ENUM_END
}EN_SDP_ATTR_ORIENT_TYPE;

/* sdp attribute conference type */
typedef enum
{
    SDP_ATTR_TYPE_UNKNOWN = 0,

    /* attribute conference type "broadcast" */
    SDP_ATTR_TYPE_BROADCAST,

    /* attribute conference type "meeting" */
    SDP_ATTR_TYPE_MEETING,

    /* attribute conference type "moderated" */
    SDP_ATTR_TYPE_MODERATED,

    /* attribute conference type "test" */
    SDP_ATTR_TYPE_TEST,

    /* attribute conference type "H332" */
    SDP_ATTR_TYPE_H332,

    SDP_ATTR_TYPE_EXTN,
    SDP_ATTR_TYPE_END = SDP_ENUM_END
}EN_SDP_ATTR_CONFTYPE;

/* sdp media type */
typedef enum
{
    SDP_MEDIA_UNKNOWN = 0,

    /* media type "audio" */
    SDP_MEDIA_AUDIO,

    /* media type "video" */
    SDP_MEDIA_VIDEO,

    /* media type "application" */
    SDP_MEDIA_APP,

    /* media type "data" */
    SDP_MEDIA_DATA,

    /* media type "control" */
    SDP_MEDIA_CONTROL,

    /* media type "image" */
    SDP_MEDIA_IMAGE,

    SDP_MEDIA_TYPE_EXTN,
    SDP_MEDIA_TYPE_END = SDP_ENUM_END
}EN_SDP_MEDIA_TYPE;

/* sdp media protocol type */
typedef enum
{
    SDP_MEDIA_PROTO_UNKNOWN = 0,

    /* media proto type "rtp/avp" */
    SDP_MEDIA_PROTO_RTP_AVP,

    /* media proto type "udp" */
    SDP_MEDIA_PROTO_UDP,

    /* media proto type "h221" */
    SDP_MEDIA_PROTO_H221,

    /* media proto type "h223" */
    SDP_MEIDA_PROTO_H223,

    /* media proto type "tcp" */
    SDP_MEDIA_PROTO_TCP,

    /* media proto type "udptl" */
    SDP_MEDIA_PROTO_UDPTL,

    /* media proto type "local" */
    SDP_MEDIA_PROTO_LOCAL,

    SDP_MEDIA_PROTO_EXTN,
    SDP_MEDIA_PROTO_END = SDP_ENUM_END
}EN_SDP_MEDIA_PROTO_TYPE;

/* H.261 */
typedef enum
{
    SDP_ATTR_H261_UNKNOWN = 0,

    /* h261 type "qcif" */
    SDP_ATTR_H261_QCIF,

    /* h261 type "cif" */
    SDP_ATTR_H261_CIF,

    /* h261 type "D" */
    SDP_ATTR_H261_ANNEX_D,

    SDP_ATTR_H261_EXTN,
    SDP_ATTR_H261_END = SDP_ENUM_END
}EN_SDP_ATTR_H261_TYPE;

/* H.263 */
typedef enum
{
    SDP_ATTR_H263_SIZE_UNKNOWN = 0,

    /* h263 cap size type "sqcif" */
    SDP_ATTR_H263_SIZE_SQCIF,

    /* h263 cap size type "qcif" */
    SDP_ATTR_H263_SIZE_QCIF,

    /* h263 cap size type "cif" */
    SDP_ATTR_H263_SIZE_CIF,

    /* h263 cap size type "cif4" */
    SDP_ATTR_H263_SIZE_CIF4,

    /* h263 cap size type "cif16" */
    SDP_ATTR_H263_SIZE_CIF16,

    /* h263 cap size type "xmax" */
    SDP_ATTR_H263_SIZE_XMAX,

    /* h263 cap size type "ymax" */
    SDP_ATTR_H263_SIZE_YMAX,

    /* h263 cap size type "mpi" */
    SDP_ATTR_H263_SIZE_MPI,

    SDP_ATTR_H263_SIZE_EXTN,
    SDP_ATTR_H263_SIZE_END = SDP_ENUM_END
}EN_SDP_ATTR_H263_SIZE_TYPE;

typedef enum
{
    SDP_ATTR_H263_PARAMS_UNKNOW = 0,

    /* h263 cap param type "par" */
    SDP_ATTR_H263_PARAMS_PAR,

    /* h263 cap param type "cpcf" */
    SDP_ATTR_H263_PARAMS_CPCF,

    /* h263 cap param type "maxbr" */
    SDP_ATTR_H263_PARAMS_MAXBR,

    /* h263 cap param type "bpp" */
    SDP_ATTR_H263_PARAMS_BPP,

    /* h263 cap param type "hrd" */
    SDP_ATTR_H263_PARAMS_HRD,

    SDP_ATTR_H263_PARAMS_EXTN,
    SDP_ATTR_H263_PARAMS_END = SDP_ENUM_END
}EN_SDP_ATTR_H263_PARAMS_TYPE;

typedef enum
{
    SDP_ATTR_H263_OPT_UNKNOW = 0,

    /* h263 cap options type "d" */
    SDP_ATTR_H263_OPT_D,

    /* h263 cap options type "e" */
    SDP_ATTR_H263_OPT_E,

    /* h263 cap options type "f" */
    SDP_ATTR_H263_OPT_F,

    /* h263 cap options type "g" */
    SDP_ATTR_H263_OPT_G,

    /* h263 cap options type "i" */
    SDP_ATTR_H263_OPT_I,

    /* h263 cap options type "j" */
    SDP_ATTR_H263_OPT_J,

    /* h263 cap options type "k" */
    SDP_ATTR_H263_OPT_K,

    /* h263 cap options type "l" */
    SDP_ATTR_H263_OPT_L,

    /* h263 cap options type "m" */
    SDP_ATTR_H263_OPT_M,

    /* h263 cap options type "n" */
    SDP_ATTR_H263_OPT_N,

    /* h263 cap options type "o" */
    SDP_ATTR_H263_OPT_O,

    /* h263 cap options type "p" */
    SDP_ATTR_H263_OPT_P,

    /* h263 cap options type "q" */
    SDP_ATTR_H263_OPT_Q,

    /* h263 cap options type "r" */
    SDP_ATTR_H263_OPT_R,

    /* h263 cap options type "s" */
    SDP_ATTR_H263_OPT_S,

    /* h263 cap options type "t" */
    SDP_ATTR_H263_OPT_T,

    SDP_ATTR_H263_OPT_EXTN,
    SDP_ATTR_H263_OPT_END = SDP_ENUM_END
}EN_SDP_ATTR_H263_OPT_TYPE;

typedef enum
{
    SDP_ATTR_H263_REQ_UNKNOWN = 0,

    /* h263 request type "I-UPDATE" */
    SDP_ATTR_H263_REQ_I_UPDATE,

    /* h263 request type "GOB-UPDATE" */
    SDP_ATTR_H263_REQ_GOB_UPDATE,

    SDP_ATTR_H263_REQ_EXTN,
    SDP_ATTR_H263_REQ_END = SDP_ENUM_END
}EN_SDP_ATTR_H263_REQ_TYPE;

/* H.263+ 1998 */
typedef enum
{
    SDP_ATTR_H263PLUS_UNKNOWN = 0,

    /* h263 plus type "sqcif" */
    SDP_ATTR_H263PLUS_SQCIF,

    /* h263 plus type "qcif" */
    SDP_ATTR_H263PLUS_QCIF,

    /* h263 plus type "cif" */
    SDP_ATTR_H263PLUS_CIF,

    /* h263 plus type "cif4" */
    SDP_ATTR_H263PLUS_CIF4,

    /* h263 plus type "cif16" */
    SDP_ATTR_H263PLUS_CIF16,

    /* h263 plus type "custom" */
    SDP_ATTR_H263PLUS_CUSTOM,

    /* h263 plus type "f" */
    SDP_ATTR_H263PLUS_F,

    /* h263 plus type "i" */
    SDP_ATTR_H263PLUS_I,

    /* h263 plus type "j" */
    SDP_ATTR_H263PLUS_J,

    /* h263 plus type "k" */
    SDP_ATTR_H263PLUS_K,

    /* h263 plus type "n" */
    SDP_ATTR_H263PLUS_N,

    /* h263 plus type "p" */
    SDP_ATTR_H263PLUS_P,

    /* h263 plus type "t" */
    SDP_ATTR_H263PLUS_T,

    /* h263 plus type "par" */
    SDP_ATTR_H263PLUS_PAR,

    /* h263 plus type "cpcf" */
    SDP_ATTR_H263PLUS_CPCF,

    /* h263 plus type "bpp" */
    SDP_ATTR_H263PLUS_BPP,

    /* h263 plus type "hrd" */
    SDP_ATTR_H263PLUS_HRD,

    /* H.263+ 2000 externtion */

    /* h263 plus type "profile" */
    SDP_ATTR_H263PLUS_PROFILE,

    /* h263 plus type "level" */
    SDP_ATTR_H263PLUS_LEVEL,

    /* h263 plus type "interlace" */
    SDP_ATTR_H263PLUS_INTERLACE,

    SDP_ATTR_H263PLUS_EXTN,
    SDP_ATTR_H263PLUS_END = SDP_ENUM_END
}EN_SDP_ATTR_H263PLUS_TYPE;

/* H.264 */
typedef enum
{
    SDP_ATTR_H264_UNKNOWN = 0,

    /* h264 type "profile-level-id" */
    SDP_ATTR_H264_PFLV_ID,

    /* h264 type "max-mbps" */
    SDP_ATTR_H264_MAX_MBPS,

    /* h264 type "max-fs" */
    SDP_ATTR_H264_MAX_FS,

    /* h264 type "max-cpb" */
    SDP_ATTR_H264_MAX_CPB,

    /* h264 type "max-dpb" */
    SDP_ATTR_H264_MAX_DPB,

    /* h264 type "max-br" */
    SDP_ATTR_H264_MAX_BR,

    /* h264 type "redundant-pic-cap" */
    SDP_ATTR_H264_RP_CAP,

    /* h264 type "sprop-parameter-sets" */
    SDP_ATTR_H264_SP_SETS,

    /* h264 type "parameter-add" */
    SDP_ATTR_H264_PARM_ADD,

    /* h264 type "packetization-mode" */
    SDP_ATTR_H264_PKG_MODE,

    /* h264 type "sprop-interleaving-depth" */
    SDP_ATTR_H264_SI_DEPTH,

    /* h264 type "sprop-deint-buf-req" */
    SDP_ATTR_H264_SDB_REQ,

    /* h264 type "deint-buf-cap" */
    SDP_ATTR_H264_DB_CAP,

    /* h264 type "sprop-init-buf-time" */
    SDP_ATTR_H264_SIB_TIME,

    /* h264 type "sprop-max-don-diff" */
    SDP_ATTR_H264_SPMD_DIFF,

    /* h264 type "max-rcmd-nalu-size" */
    SDP_ATTR_H264_MRN_SIZE,

    SDP_ATTR_H264_EXTN,
    SDP_ATTR_H264_END = SDP_ENUM_END
}EN_SDP_ATTR_H264_TYPE;

/* MPV(MPEG1, MPEG2) */
typedef enum
{
    SDP_ATTR_MPV_UNKNOWN = 0,

    /* MPV type "mpeg1" */
    SDP_ATTR_MPEG1,

    /* MPV type "mpeg2-halfd1" */
    SDP_ATTR_MPEG2_HALFD1,

    /* MPV type "mpeg2-fulld1" */
    SDP_ATTR_MPEG2_FULLD1,

    SDP_ATTR_MPV_EXTN,
    SDP_ATTR_MPV_END = SDP_ENUM_END
}EN_SDP_ATTR_MPV_TYPE;

/* MPEG4 */
typedef enum
{
    SDP_ATTR_MPEG4_UNKNOW = 0,

    /* MPEG4 type "profile-level-id" */
    SDP_ATTR_MPEG4_PROLEVELID,

    /* MPEG4 type "bitrate" */
    SDP_ATTR_MPEG4_BITRATE,

    /* MPEG4 type "object" */
    SDP_ATTR_MPEG4_OBJECT,

    /* MPEG4 type "config" */
    SDP_ATTR_MPEG4_CONFIG,

    /* MPEG4 type "cpresent" */
    SDP_ATTR_MPEG4_CPRESENT,

    /* MPEG4 type "ptime" */
    SDP_ATTR_MPEG4_PTIME,

    SDP_ATTR_MPEG4_EXTN,
    SDP_ATTR_MPEG4_END = SDP_ENUM_END
}EN_SDP_ATTR_MPEG4_TYPE;


/* attribute range */
typedef enum
{
    SDP_ATTR_RANGE_UNKNOW = 0,

    /* Range type "smpte" */
    SDP_ATTR_RANGE_SMPTE,

    /* Range type "smpte-30-drop" */
    SDP_ATTR_RANGE_SMPTE_30_DROP,

    /* Range type "smpte-25" */
    SDP_ATTR_RANGE_SMPTE_25,

    /* Range type "npt" */
    SDP_ATTR_RANGE_NPT,

    /* Range type "utc" */
    SDP_ATTR_RANGE_UTC,

    SDP_ATTR_RANGE_EXTN,
    SDP_ATTR_RANGE_END = SDP_ENUM_END
}EN_SDP_ATTR_RANGE_TYPE;


/* sdp attribute t.38 Data Rate Management Method */
typedef enum
{
    SDP_ATTR_T38_DRMM_UNKNOW = 0,

    /* T38 DRMM type "localTCF" */
    SDP_ATTR_T38_DRMM_LCLTCF,

    /* T38 DRMM type "transferredTCF" */
    SDP_ATTR_T38_DRMM_TRFTCF,

    SDP_ATTR_T38_DRMM_EXTN,
    SDP_ATTR_T38_DRMM_END = SDP_ENUM_END
}EN_SDP_ATTR_T38_DRMM_TYPE;


/* sdp attribute t.38 Error Correction */
typedef enum
{
    SDP_ATTR_T38_UDP_UNKNOW = 0,

    /* T38 UDPEC type "t38UDPFEC" */
    SDP_ATTR_T38_UDPFEC,

    /* T38 UDPEC type "t38UDPRedundancy" */
    SDP_ATTR_T38_UDPREDUNDANCY,

    SDP_ATTR_T38_UDP_EXTN,
    SDP_ATTR_T38_UDP_END = SDP_ENUM_END
}EN_SDP_ATTR_T38_UDP_TYPE;

/* Attribute PGMPU token id */
typedef enum
{
    SDP_ATTR_PGMPU_UNKNOW = 0,

    /* Pgmpu type "base64" */
    SDP_ATTR_PGMPU_BASE64,

    SDP_ATTR_PGMPU_EXTN,
    SDP_ATTR_PGMPU_END = SDP_ENUM_END
}EN_SDP_ATTR_PGMPU_TYPE;

/* Attribute Mpeg4 IOD token id */
typedef enum
{
    SDP_ATTR_MPEGP4IOD_UNKNOW = 0,

    /* MPEG4IOD type "base64" */
    SDP_ATTR_MPEGP4IOD_BASE64,

    SDP_ATTR_MPEGP4IOD_EXTN,
    SDP_ATTR_MPEGP4IOD_END = SDP_ENUM_END
}EN_SDP_ATTR_MPEGP4IOD_TYPE;

/* Attribute Sip token id */
typedef enum
{
    SDP_ATTR_SIP_UNKNOW = 0,
    SDP_ATTR_SIP_PRECOND_QOS, /* precondition "qos" */
    SDP_ATTR_SIP_STRTAG_MAND, /* strength-tag: mandatory */
    SDP_ATTR_SIP_STRTAG_OPT,  /* strength-tag: optional */
    SDP_ATTR_SIP_STRTAG_NONE, /* strength-tag: none */
    SDP_ATTR_SIP_STRTAG_FAIL, /* strength-tag: failure */
    SDP_ATTR_SIP_STRTAG_UNK,  /* strength-tag: unknown */
    SDP_ATTR_SIP_SAT_E2E,     /* status-type:  e2e*/
    SDP_ATTR_SIP_SAT_LOCAL,   /* status-type:  local */
    SDP_ATTR_SIP_SAT_REMOTED, /* status-type:  remote */
    SDP_ATTR_SIP_DIRTAG_NONE, /* direction-tag: none */
    SDP_ATTR_SIP_DIRTAG_SEND, /* direction-tag: send */
    SDP_ATTR_SIP_DIRTAG_RECV, /* direction-tag: recv */
    SDP_ATTR_SIP_DIRTAG_SDRV, /* direction-tag: sendrecv */
    SDP_ATTR_SIP_DIRTAG_EXTN,
    SDP_ATTR_SIP_DIRTAG_END = SDP_ENUM_END
}EN_SDP_ATTR_SIP_TYPE;

/* sdp attribute fmtp type */
typedef enum
{
    SDP_ATTR_FMTP_UNKNOWN = 0,

    /* Fmtp type "h261" */
    SDP_ATTR_FMTP_H261,

    /* Fmtp type "h263" */
    SDP_ATTR_FMTP_H263,

    /* Fmtp type h263-plus "h263-1998" */
    SDP_ATTR_FMTP_H263_1998,

    /* Fmtp type h263-plus "h263-2000" */
    SDP_ATTR_FMTP_H263_2000,

    /* Fmtp type "h264" */
    SDP_ATTR_FMTP_H264,

    /* Fmtp type "mpeg1" */
    SDP_ATTR_FMTP_MPEG1,

    /* Fmtp type "mpg3" */
    SDP_ATTR_FMTP_MP3,

    /* Fmtp type "mpeg4a" */
    SDP_ATTR_FMTP_MPEG4A,

    /* Fmtp type "mpeg4v" */
    SDP_ATTR_FMTP_MPEG4V,

    /* Fmtp type "dtmf" */
    SDP_ATTR_FMTP_DTMF,

    SDP_ATTR_FMTP_EXTN,
    SDP_ATTR_FMTP_END = SDP_ENUM_END
}EN_SDP_ATTR_FMTP_TYPE;

#ifdef __cplusplus
}
#endif

#endif /* _SDP_ENUMS_H_ */

