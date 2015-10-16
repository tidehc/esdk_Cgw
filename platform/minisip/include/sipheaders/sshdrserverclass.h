/******************************************************************************

    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : sshdrserverclass.h

    Author          : Harsha G

    Version         : 1.0

    Date            : 2005-12-23

    Description     : This file provides the initialization interfaces for
                      server class headers.
                      (Server and UserAgent)

    Function List   :
        1. SipHdrServerInit
        2. SipHdrUserAgentInit


    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : Harsha GXXX
           Modification: XXX

*******************************************************************************/

#ifndef __SS_HDRSERVERCLASS_H__
#define __SS_HDRSERVERCLASS_H__



#ifdef __cplusplus
extern "C" {
#endif

#include "ssfrmreg.x"
#include "ssenums.h"
#include "ssheaders.h"
#include "sscbbtypes.x"

typedef struct _SipServer
{
    SipString *pstrProduct;
    SipString *pstrProductVersion;
    SipString *pstrComments;
}SipServer;

typedef SipHeaderListHdl SipServerListHdl;

typedef SipServer     SipUserAgent;

typedef SipServerListHdl SipUserAgentListHdl;



/*******************************************************************************
 Function   : SipHdrServerInit

 Description: This function is used to initialize the Server Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrServerInit
(
    SipHdrInfoHdl hdlHdrInfo
);


/*******************************************************************************
 Function   : SipHdrUserAgentInit

 Description: This function is used to initialize the UserAgent Header

 Input      : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Output     : hdlHdrInfo - It is a container to store header name, compact name,
              header flags and function pointers.

 Return     : Success or Error Code
*******************************************************************************/
EN_SIP_RESULT SipHdrUserAgentInit
(
    SipHdrInfoHdl hdlHdrInfo
);


#ifdef __cplusplus
}
#endif

#endif /* __SS_HDRSERVERCLASS_H__ */

