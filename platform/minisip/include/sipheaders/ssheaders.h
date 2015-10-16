/*******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName   : ssheaders.h

    Author     : Srinivas R Thota

    Version    : 1.0

    Date       : 2005-11-12

    Description: This file contains the Common structure definitions ,Basic
                 header definitions .


    Function List:
        None


    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX
*******************************************************************************/


#ifndef _SS_HEADERS_H_
#define _SS_HEADERS_H_

#include "sscbbtypes.x"
#include "ssdatatype.h"
#include "sssipbase.h"

#ifdef __cplusplus
extern "C" {
#endif


/* Generic parameter Name Value */
typedef struct _SipGenParNameValue
{
    SipString  strGenParName;   /* string will be stored for param name */
    SipString *pstrGenParValue; /* value can be optional */
}SipGenParNameValue;


/* List of GenParNameValue */
typedef SipHeaderListHdl SipGenericParamListHdl;



/* Token Generic parameters */
/* eg:- Content-Disposition: session;handling=optional */
typedef struct _SipTokenGenParams
{
    SipToken               stToken;             /* this can contain well-known string recognized as token-id or string with token-id value as zero */
    SipGenericParamListHdl hdlGenericParamList; /* generic-params list */
}SipTokenGenParams;


/* List of SipToken(SipToken) eg:- Allow : INVITE,ACK,EXTENSION */
typedef SipHeaderListHdl SipTokenListHdl;

/* List of SipTokenGenParams eg:-  SipAccept-Encoding: gzip;g1=v1 , tar,g2 */
typedef SipHeaderListHdl SipTokenGenParamsListHdl;

/* List of strings */
typedef SipHeaderListHdl SipStringListHdl;


/* eg.  Join: 12345600@atlanta.example.com;from-tag=1234567;to-tag=23431 */
typedef struct _SipStringGenParams
{
    SipString              strString;           /* header value string */
    SipGenericParamListHdl hdlGenericParamList; /* generic-params list */
}SipStringGenParams;

/* eg:- Alert-Info: <http://www.example.com/sounds/moo.wav>;
   param-name=param-value */
typedef SipHeaderListHdl SipStringGenParamsListHdl;


/* URI related */
typedef struct _SipHost
{
    EN_SIP_ADDRESS_TYPE enHostType;
    union
    {
        SipString *pstrHostName;              /* will contain a value if enHostType is SIP_ADDR_TYPE_DOMAIN */
        SS_UCHAR   ipv4[SS_IPV4_ADDR_LEN];    /* will contain a value if enHostType is SIP_ADDR_TYPE_IPV4   */
        SS_UCHAR   ipv6[SS_IPV6_ADDR_LEN];    /* will contain a value if enHostType is SIP_ADDR_TYPE_IPV6   */
    }uHostContent;
}SipHost;

typedef struct _SipHostPort
{
    SipHost stHost;
    SS_INT  iPort;  /* -1 if the port information is absent */
}SipHostPort;

typedef struct _SipURI
{
    SipString              *pstrUserName;   /* "user" data                               */
    SipString              *pstrPass;       /* "password" data                           */
    SipHostPort             stHostPort;     /* "hostport" data                           */
    SS_BOOL                 bLr;            /* is "lr" param present in uri              */
    SS_INT                  iTtl;           /* "ttl" param value                         */
    SipToken               *pstMethod;      /* "method" param value (EN_SIP_METHOD)      */
    SipToken               *pstUserParam;   /* "user" param value (EN_SIP_USER)          */
    SipHost                *pstMaddr;       /* "mddr" param value                        */
    SipToken               *pstTransport;   /* "transport" param value (EN_SIP_TRANSPORT)*/
    SipToken               *pstCompression; /* "comp" param value (EN_SIP_COMPRESSION)   */
    SipGenericParamListHdl  hdlUriParams;   /* uri-parameters  data                      */
    SipGenericParamListHdl  hdlUriHeaders;  /* headers  data                             */
}SipURI;


/* This MUST be defined like this, our implementations can refer sip and sips
URI's using same union member name */
typedef SipURI SipsURI;

typedef union _URI_Union
{
    SipString *pstrOpaque;  /* if enURISchemeType in URI is SIP_URI_SCH_EXT  */
    SipURI    *pstSipUri;   /* if enURISchemeType in URI is SIP_URI_SCH_SIP  */
    SipsURI   *pstSipsUri;  /* if enURISchemeType in URI is SIP_URI_SCH_SIPS */

}URI_Union;

typedef struct _URI
{
    EN_SIP_URI_SCHEME  enURISchemeType;
    URI_Union          uri;
    SipString         *pstrURIScheme; /* if enURISchemeType in URI is SIP_URI_SCH_EXT  */
}URI;

/* eg:- Route: "Alice" <alice123@huawei.com> */
typedef struct _SipUriHeader
{
    SipString             *pstrDisplayName;   /* "display-name" data  */
    URI                    stUri;             /* "addr-spec" data     */
    SipGenericParamListHdl hdlGenericParams;  /* "generic-param" data */
}SipUriHeader;

/* List of UriHeader 's */
typedef SipHeaderListHdl SipUriHeaderListHdl;

/* eg:-  P-Charging-Function-Addresses: ccf=192.1.1.1; ccf=192.1.1.2 */
typedef SipGenericParamListHdl SipGenericParamsHdr;


/* List of SipGenericParamsHdr 's */
/* eg:- Accept-Contact: *;methods="BYE";class="business";q=1.0 ,*;video;explicit  */
typedef SipHeaderListHdl SipGenericParamsHdrListHdl;



/* Basic Header Definitions */

typedef struct _SipFrom
{
    SipString              *pstrDisplayName;
    URI                     stUri;
    SipString               strTag;
    SipGenericParamListHdl  hdlGenericParams;
}SipFrom;

typedef SipFrom SipTo;

typedef struct _SipCSeq
{
    SS_UINT32   ulSequence;
    SipToken    stMethod;
}SipCSeq;

typedef SipString SipCallID;

typedef struct _SipVia
{
    SS_UINT32              ulProtocolType;
    SipString             *pstrProtocolType;
    SipString              strProtocolVersion;
    SipToken               stTransport;
    SipHostPort            stSentBy;
    SipHost               *pstReceived;
    SipString              strBranch;
    SS_INT                 iRPort;
    SipToken              *pstCompression;
    SipGenericParamListHdl hdlViaParams;
}SipVia;

/* List of SipVia's */
typedef SipHeaderListHdl    SipViaListHdl;



typedef SipUriHeader        SipRoute;

/* List of SipRoute's */
typedef SipUriHeaderListHdl SipRouteListHdl;



typedef SipUriHeader        SipRecordRoute;

/* List of SipRecordRoute's */
typedef SipUriHeaderListHdl SipRecordRouteListHdl;



typedef SS_UINT32           SipContentLength;



typedef struct _SipContact
{
    SipUriHeader  *pstContact;
    SS_BOOL       bIsStarSet;
}SipContact;

/* List of Contact's */
typedef SipHeaderListHdl SipContactListHdl;



typedef struct _SipWarning
{
    SS_INT    iWarnCode;
    SipString strWarningAgent;
    SipString strWarnText;
}SipWarning;

/* List of Warning's */
typedef SipHeaderListHdl SipWarningListHdl;




#ifdef MINISIP_SUPPORTED_64BIT
typedef struct _SipTimeStamp
{
    SS_UINT32 lTimeSecs;
    SS_INT32  lTimeMilliSecs;
    SS_INT32  lDelaySecs;
    SS_INT32  lDelayMilliSecs;
}SipTimeStamp;
#else
typedef struct _SipTimeStamp
{
    SS_ULONG lTimeSecs;
    SS_LONG  lTimeMilliSecs;
    SS_LONG  lDelaySecs;
    SS_LONG  lDelayMilliSecs;
}SipTimeStamp;
#endif

#ifdef __cplusplus
}
#endif

#endif /* _SS_HEADERS_H_ */

