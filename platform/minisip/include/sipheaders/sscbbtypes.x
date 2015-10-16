/******************************************************************************

    FileName: sscbbtypes.x

    Author:  Srnivas R Thota
    Version:

    Date: 2005-11-12
    Description: This file has datatypes dependent on VppCBB.

    Function List: N.A

    History:

*******************************************************************************/

#ifndef __SS_CBBTYPES_X__
#define __SS_CBBTYPES_X__

#include "vppcommon.x"
#include "vppmemcp.x"
#include "vppstring.x"
#include "vpptoken.x"
#include "ssdatatype.h"
#include "vpplist.x"
#include "vppstrbuf.x"

#ifdef __cplusplus
extern "C" {
#endif

#define SipMemCpCreate  VppMemCpCreate
#define SipMemCpDestroy VppMemCpDestroy
#define SipMemCpMalloc  VppMemCpMallocX
#define SipMemCpFree    VppMemCpFreeX

typedef VppListHdl  SipHeaderListHdl;

typedef VppListHdl  SipListHdl;

typedef VppStrBufHdl SipStrBufHdl;

typedef VppStringS  SipString;

typedef VppTokenS   SipToken;

typedef VppMemCpHdl SipMemCpHdl;

#ifdef __cplusplus
}
#endif


#endif /* __SS_CBBTYPES_X__ */

