/*******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName   : ssmessage.h

    Author     : Srinivas R Thota

    Version    : 1.0

    Date       : 2005-11-12

    Description: This file contains SipMsgS and SipHdrS and other related
                 strucutre definitions .


    Function List:
        None


    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX
*******************************************************************************/


#ifndef __SSMESSAGE_H__
#define __SSMESSAGE_H__

#include "sscbbtypes.x"
#include "ssheaders.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _SipRequestLine
{
    SipToken   stMethod;          /* Request method (EN_SIP_METHOD) */
    URI        stRequestURI;
    SS_UINT16  usSipVersionMajor; /* SIP-Version "2" in "2.0" */
    SS_UINT16  usSipVersionMinor; /* SIP-Version "0" in "2.0" */
}SipRequestLine;

typedef struct _SipStatusLine
{
    SS_INT     iSipStatusCode;
    SipString  strReasonPhrase;
    SS_UINT16  usSipVersionMajor; /* SIP-Version "2" in "2.0" */
    SS_UINT16  usSipVersionMinor; /* SIP-Version "0" in "2.0" */
}SipStatusLine;


typedef union _SipFirstLineUnion
{
    SipRequestLine stRequestLine; /* if enMsgType in SipMsgS is SIP_REQUEST  */
    SipStatusLine  stStatusLine;  /* if enMsgType in SipMsgS is SIP_RESPONSE */
}SipFirstLineUnion;

typedef struct _SipHdrNameValue
{
    SipString  strHdrName;    /* contains header-name string such as "Accept"           */
    SipString  strHdrValue;   /* contains header-value string such as "application/sdp" */
}SipHdrNameValue;

typedef struct _SipHdrIdValue
{
    SS_UINT16   usHdrId;  /* contains header-id */
    SS_UINT16   usSpare;  /* not used */
    SS_VOID    *pvHdrVal; /* pointer to header value structure */
}SipHdrIdValue;

typedef struct _SipHdrS
{
    SipCSeq               *pstCseq;
    SipCallID             *pstCallID;
    SipContactListHdl      hdlContactList;
    SipContentLength      *pstContentLength;
    SipFrom               *pstFrom;
    SipRecordRouteListHdl  hdlRecordRouteList;
    SipRouteListHdl        hdlRouteList;
    SipTimeStamp          *pstTimeStamp;
    SipTo                 *pstTo;
    SipViaListHdl          hdlViaList;
    SipWarningListHdl      hdlWarningList;
    SipHeaderListHdl       hdlExtHeaders;       /* list of SipHdrIdValue */
    SipHeaderListHdl       hdlUnknownHdrs;      /* list of SipHdrNameValue */
    SipHeaderListHdl       hdlDecodeFailedHdrs; /* list of SipHdrNameValue */
}SipHdrS;

typedef SipStrBufHdl SipBody;

typedef struct _SipMsgS
{
    SipMemCpHdl         hdlMemCp;
    EN_SIP_MESSAGE_TYPE enMsgType;
    SipFirstLineUnion   uFirstLine;
    SipHdrS             stHeaders;
    SipBody             hdlBody;
    SipToken           *pstSipMethod;
    /*SS_INT              iStatusCode;*/
    SS_UINT32           ulReferenceCnt;
    SS_BOOL             bFromNetwork;
}SipMsgS;

typedef struct _SipDataUnitS
{
    SipMsgS        *pstSipMsg;
    SS_UINT32       ulCompId;    /* The compartment id when using compression */
    SS_UINT32       ulDCompRefId;/* Reference ID of the SIGCOMP */
}SipDataUnitS;

#ifdef __cplusplus
}
#endif

#endif /* __SSMESSAGE_H__ */

