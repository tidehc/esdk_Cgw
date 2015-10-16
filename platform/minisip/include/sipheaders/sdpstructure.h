/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sdpstructure.h

    Author          : zhaolei (ronnie)

    Version         : 1.0

    Date            : 2005-11-16

    Description     : This file contains the declare of SDP module
                      structure.

    Function List   :
        1.

    History:
        1.  Date         : <2006-05-30>
            Tag          : <Tag.YDHD02328>
            Author       : Manjunath Warad
            Modification : Structures of H263 is moved to sample code.
*******************************************************************************/
#ifndef __SDP_STRUCTURE_H__
#define __SDP_STRUCTURE_H__

#include "sdpenums.h"
#include "sdptypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* If this field is a token, usType should be the token ID.
   If this is unknown token ID, the usType is zero, and stString
   will save the unknown token string.
   If this field is a format digit, usType should be the digit value.
   If this field is not a digit, this field should be zero, and
   stString will save the exception string. */
typedef struct _SdpTypeS
{
    SDP_UINT16          usType;    /* Enumerate type or extension type */
    SDP_UINT16          usSpare;   /* reserved for 32 bit align */
    VppStringS          stString;  /* unknown type string */
}SdpTypeS;

typedef struct _SdpOriginS
{
    /* Origin user name */
    VppStringS          stUserName;
    /* session id */
    VppStringS          stSessionID;
    /* sessio version */
    VppStringS          stSessionVer;
    /* network type */
    SdpTypeS            stNetType;
    /* address type */
    SdpTypeS            stAddrType;
    /* address, no need parse this stuff,
       has provide other CBB interface to
       parse it, please refer to VppIpv4Dec and VppIpv6Dec.
       User can base on the addrtype then decide
       which function need to call */
    VppStringS          stAddr;
}SdpOriginS;

#define SDP_CONN_TTL_PRES  0x1
#define SDP_CONN_NUM_PRES  0x2

typedef struct _SdpConnectionS
{
    /* To express ttl and ulNum is present or not. Must have "ttl" first,
       then can have "num" field */
    SDP_UINT32          ulMask;
    /* network type */
    SdpTypeS            stNetType;
    /* address type */
    SdpTypeS            stAddrType;
    /* address, no need parse this stuff,
       has provide other CBB interface to
       parse it, please refer to VppIpv4Dec and VppIpv6Dec.
       User can base on the addrtype then decide
       which function need to call */
    VppStringS          stAddr;
    /* ttl value */
    SDP_UINT8           ucTtl;
    SDP_UINT8           aucSpare[3]; /* reserved bytes */
    /* number of the address */
    SDP_UINT32          ulNum;
}SdpConnectionS;

typedef struct _SdpBandwidthS
{
    /* bandwith type: CT, AS, X-AAA, but sdp won't check the
       extension is using the "X-" as the prefix or not. If SDP
       user really need, user can't check it outside.
       Otherwise omit it for tolerable. */
    SdpTypeS            stModifier;
    /* Bandwith value */
    SDP_UINT32          ulBandwidth;
}SdpBandwidthS;

typedef struct _SdpTimeS
{
    /* start time of the time parameter */
    VppStringS          stStartTime;

    /* stop time of the time parameter */
    VppStringS          stStopTime;
}SdpTimeS;

typedef struct _SdpTimeDescS
{
    /* Time parameter information */
    SdpTimeS           *pstTime;

    /* Repeat parameter list information */
    VppListHdl          hdlRepList;

    /* Zone parameter information */
    VppStringS         *pstZoneAdjust;
}SdpTimeDescS;

/* Key method token value, the value should be registered by
   VppTokenReg function.
   Please register this sdp key type values, please refer
   to EN_SDP_KEY_TYPE.
   If user dosen't register it, this field will decode as a failure
   field.
 */
typedef struct _SdpKeyS
{
    SDP_UINT16          usMethod;
    /* reserved bytes */
    SDP_UINT16          usSpare;
    /* All the key string will decode as string */
    VppStringS         *pstKeyData;
}SdpKeyS;

/* Attribute structure:
 * All the attribute will be decode into this structure.
 * If the attribute string token is been registered.
 * stAttrType will have a attribute ID. If the attribute
 * has already registered a decode function, the type pvAttVal
 * will depend on the user registeration.
 * If user haven't register the decode function, pvAttVal
 * is a VppString.
 * If user haven't register the attribute token, pvAttVal
 * is a VppString.
 */
typedef struct _SdpAttrS
{
    /* Attribute type */
    SdpTypeS            stAttrType;
    /* Attribute value */
    SDP_VOID           *pvAttVal;
}SdpAttrS;

/**
 * Attribute rtpmap field detail information.
 * Payload may be digit or string
 */
typedef struct _SdpAttrRtpmapS
{
    /* Media payload */
    SdpTypeS            stPayload;

    /* Encoding name */
    SdpTypeS            stEncName;

    /* Clock rate */
    SDP_UINT32          ulClockRate;

    /* Encoding parameter */
    SDP_UINT32          ulEncParm;
}SdpAttrRtpmapS;

/**
 * Attribute fmtp field detail information.
 * Payload may be digit or string
 */
typedef struct _SdpAttrFmtpS
{
    SdpTypeS            stPayload;
    /* Fmt specification type is depend on the user registeration */
    SDP_VOID           *pvFmtSpec;
}SdpAttrFmtpS;

typedef struct _SdpAttrRangeS
{
    SDP_UINT16          usType;
    SDP_UINT16          usSpare;
    VppStringS          stRange;
}SdpAttrRangeS;

/*
current-status     =  "a=curr:" precondition-type
    SP status-type SP direction-tag
 */
typedef struct _SdpAttrCurrS
{
    SdpTypeS            stPreConditionType;
    SdpTypeS            stStatusType;
    SdpTypeS            stDirectionType;
}SdpAttrCurrS;

/* "a=des:" precondition-type
   SP strength-tag SP status-type
   SP direction-tag
 */
typedef struct _SdpAttrDesS
{
    SdpTypeS            stPreConditionType;
    SdpTypeS            stStrengthTag;
    SdpTypeS            stStatusType;
    SdpTypeS            stDirectionType;
}SdpAttrDesS;

typedef SdpAttrCurrS  SDPAttrConf;

typedef struct _SdpMediaS
{
    SdpTypeS            stMedia;
    /* Channel ID, this field is a digit normally. But there are some
       special application maybe a string, or special char */
    VppStringS          stChannelID;
    /* channel number */
    SDP_UINT32          ulPortNum;
    /* proto token */
    SdpTypeS            stProto;
    /* payload list */
    VppListHdl          hdlPayloadList; /* List element is SdpTypeS */
}SdpMediaS;

typedef struct _SdpMediaDescS
{
    /* Media parameter information */
    SdpMediaS          *pstMedia;
    /* Information parameter */
    VppStringS         *pstInfo;
    /* Connection parameter list */
    VppListHdl          hdlConnList;  /* SdpConnectionS */
    /* Bandwidth parameter list */
    VppListHdl          hdlBandwidthList; /* SdpBandwidthS */
    /* Key parameter list */
    SdpKeyS            *pstKeyField;
    /* Attribute parameter list */
    VppListHdl          hdlAttrList;  /* SdpAttrS */
}SdpMediaDescS;

/* SDP Announce : PDU of SDP */
typedef struct _SdpAnnounceS
{
    /* Strucutre size of this element */
    SDP_UINT32          ulStruSize;
    /* MemCp handle, will save the whole structure */
    VppMemCpHdl         hdlMemCp;
    /* SDP version */
    SDP_UINT32          ulVer;
    /* Origin parameter field  */
    SdpOriginS         *pstOrigin;
    /* Session field  */
    VppStringS         *pstSessionName;
    /* Inforamtion field  */
    VppStringS         *pstInfo;
    /* Uri field  */
    VppStringS         *pstUri;
    /* Email field  */
    VppListHdl          hdlEmailList;  /* VppStringS */
    /* Phone field  */
    VppListHdl          hdlPhoneList;  /* VppStringS */
    /* Connection field  */
    SdpConnectionS     *pstConnection;
    /* Bandwidth field  */
    VppListHdl          hdlBandwidthList;  /* SdpBandwidthS */
    /* Time description field  */
    VppListHdl          hdlTimeDescList;    /* SdpTimeDescS  */
    /* Key field  */
    SdpKeyS            *pstKeyField;
    /* Attribute field  */
    VppListHdl          hdlAttrList;        /* SdpAttrS */
    /* Media description field  */
    VppListHdl          hdlMediaDescList;   /* SdpMediaDescS */
    /* Unknown field list */
    VppListHdl          hdlUnknownFieldList; /* SdpUnkFieldS */
    /* Decode failure field list */
    VppListHdl          hdlDecFailFieldList; /* SdpDecFailFieldS */
}SdpAnnounceS;

/* Unknown field information */
typedef struct _SdpUnkFieldS
{
    /* which state decode unknown, please refer to EN_SDP_CONTEXT_STATE_TYPE */
    SDP_UINT32          ulState;
    /* The previous parameter ID of the unknown field */
    SDP_UINT32          ulPrevId;
    /* When in the multi-parameter, the the previous parameter ID index */
    SDP_UINT32          ulIndex;
    /* Unknown parameter name */
    VppStringS          stNameStr;
    /* Unknown parameter value */
    VppStringS         *pstValueStr;
}SdpUnkFieldS;

/* Decode failure information */
typedef struct _SdpDecFailFieldS
{
    /* Which state decode failure, please refer to EN_SDP_CONTEXT_STATE_TYPE */
    SDP_UINT32          ulState;
    /* Which domain decode failure,please refer to EN_SDP_CONTEXT_DOMAIN_TYPE*/
    SDP_UINT32          ulDomain;
    /* When decode failure in the time description field, or in the
       media description field. */
    SDP_UINT32          ulIndex;
    /* Current Id, when domain is in the parameter, ulId is a parameter ID
       when domain is in the attribute, ulId is a attribute Id.
       when domain is in the fmtp, ulId is a fmtp Id.*/
    SDP_UINT32          ulId;
    /* Current position in the same id at the same domain */
    SDP_UINT32          ulCount;
    /* decode failure string */
    VppStringS         *pstFailString;
}SdpDecFailFieldS;

/* Fmtp H261 syntax informaiton */
typedef struct _SdpH261SyntaxS
{
    SDP_UINT16          usType;
    SDP_UINT16          usMpi;
}SdpH261SyntaxS;

/* SDP name value structure */
typedef struct _SdpNameValueS
{
    SdpTypeS            stParamType;
    VppStringS         *pstParamString;
}SdpNameValueS;

/* SDP list parameter for extension */
/* When user want to extend a list type structure, can't the VppListHdl
   directly, must capsulate it witch a structure.
   Then in the user extension function, user can use %l to map the hdlParamList,
   element of this structure */
/* Now this structure just for some fmtp extension field use */
typedef struct _SdpListParamS
{
    VppListHdl          hdlParamList;
}SdpListParamS;

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __SDP_STRUCTURE_H__ */

