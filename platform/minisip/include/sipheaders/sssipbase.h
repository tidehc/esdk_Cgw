/*******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName   : sssipbase.h

    Author     :

    Version    : 1.0

    Date       :

    Description: Contains  structures and Macros used by all modules

    Function List:

    History:
        1. Date        :  <YYYY-MM-DD>
           Tag         :
           Author      :
           Defect      :
           Modification:
*******************************************************************************/

#ifndef __SSSIPBASE_H__
#define __SSSIPBASE_H__

#include "ssdatatype.h"
#include "ssenums.h"

#ifdef __cplusplus
extern "C" {
#endif

#if  (defined(SS_LOG) || defined(SS_DBG))

#define  SIP_DEF_FILE_NAME(pcFile)   CONST  static  SS_CHAR* __THIS_FILE__ = (SS_CHAR *)pcFile;

#else

#define  SIP_DEF_FILE_NAME(pcFile)

#endif/*SS_LOG || SS_DBG*/

#define SIP_LINT_DISABLE  /*lint -e767*/
#define SIP_LINT_ENABLE   /*lint +e767*/

/* [DTS2010120900001][r72401][2010-12-12][VPP Defect]
    [MiniSip]: megre SIP sigcomp defect to minisip
    DTS2010101401179 Begin */
#define SIP_PCLINT_DISABLE_WARN(code)  /*lint -save code*/

#define SIP_PCLINT_ENABLE_WARN(code)    /*lint -restore*/
/* [DTS2010120900001][r72401][2010-12-12] End */


#define SIP_INVALID_VALUE -1

/* SIP Version and Warning Codes that are used by the stack */
#define SIP_VERSION_2    "2.0"
#define SIP_VERSION_LEN      3

#define SIP_VERSION_MAJOR    2
#define SIP_VERSION_MINOR    0
#define SIP_MISC_WARNING   399

/*Used to test rport in via header */
#define SIP_VIA_RPORT_ABSENT          -1
#define SIP_VIA_RPORT_PRESENT_NO_VAL  -2
#define SIP_VIA_RPORT_REPEATED        -3
#define SIP_VIA_PROTOCOL_SIP           1
#define SIP_VIA_PROTOCOL_NON_SIP       0

/* The sequence number in CSeq MUST be less than 2**31, refer RFC3261 8.1.1.5 */
#define SIP_INVALID_CSEQ_SEQ_NUMBER 0x80000000

#define SIP_TKN_ID_EXT 0

/* Used for memsetting dynamically allocated memory */
#define SS_MEM_CHAR_FILL '\0'

/*It's used to hook function to do interchecking*/
#define SIP_CB_IS_USER_OPAQUE_ID_VALID(iId)     ((iId) != 0 && (iId) != SS_NULL_LONG)


#define SIP_DEF_TCP_PORT  5060
#define SIP_DEF_UDP_PORT  5060
#define SIP_DEF_SCTP_PORT 5060
#define SIP_DEF_TLS_PORT  5061

#define SS_IPV4_ADDR_LEN   4
#define SS_IPV6_ADDR_LEN  16

#define SS_UNUSED_VAR(x) ((x)=(x))

#define SS_MAX_UINT32_LEN 10

/*for Return*/
#define SIP_RETVOID() return;
#define SIP_RETURN(x) return (x);

/* Note 255.255.255.255 */
#define SIP_MAX_IPV4_STRING_LEN  15

/* ffff:ffff:ffff:ffff:ffff:ffff:255.255.255.255 */
#define SIP_MAX_IPV6_STRING_LEN  45


/* TptD Padding of '\0\0' for Parser */
#define TPTD_PAD_CHAR_LENGTH     2


/* Stores Different types of Address */
typedef struct SipTptAddressStru
{
    EN_SIP_ADDRESS_TYPE enAddressType;   /*Host Name, IPV4, IPV6 */
    union addrType
    {
        SS_UCHAR    aucIPv4Addr[SS_IPV4_ADDR_LEN]; /* IPv4 Address definition */
        SS_UCHAR    aucIPv6Addr[SS_IPV6_ADDR_LEN]; /* IPv6 Address definition */
    }u;
    SS_INT           iPort;      /* Port number */
    EN_SIP_TRANSPORT enProtocol; /* Transport protocol type */
} SipTptAddressS;

typedef struct SipTptIpPortStru
{
    union IpType
    {
        SS_UCHAR    aucIPv4Addr[SS_IPV4_ADDR_LEN]; /* IPv4 Address definition */
        SS_UCHAR    aucIPv6Addr[SS_IPV6_ADDR_LEN]; /* IPv6 Address definition */
    }u;
    SS_INT iPort;  /* Port number */
} SipTptIpPortS;

/* Network information */
typedef struct SipTptNwInfoStru
{
    SS_UINT8      ucAddressType;/* Host Name, IPV4, IPV6, EN_SIP_ADDRESS_TYPE */
    SS_UINT8      ucProtocol;   /* Transport protocol type, EN_SIP_TRANSPORT */
    SS_UINT16     usPad;

    SipTptIpPortS stSrcAddr;
    SipTptIpPortS stDstAddr;
} SipTptNwInfoS;

/* This macro can be moved to Module */
#define SIP_INVALID_TPT_PORT            -1

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IO
#define IO
#endif

#if defined(SS_BREW_SYS) || defined(SS_SYMBIAN_SYS)
#define SS_NON_GLOBAL_VARIABLE
#endif

#ifdef __cplusplus
}
#endif


#endif

