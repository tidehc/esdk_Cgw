/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName    : sztypes.h

    Author      : Li Minghua(ID: 43584)

    Version     : 2.1

    Date        : 2005-12-05

    Description : This file provides the interface functions between the SIGCOMP
                 component and the SIP Stack User.

    Function List:

    History:
        1. Date        : <YYYY-MM-DD>
           Tag         :
           Author      :
           Modification:

*******************************************************************************/
#ifndef __SZ_TYPES_H__
#define __SZ_TYPES_H__

#include "vppcommon.x"
#include "vppmemcp.x"
#include "vppstring.x"
#include "vpplist.x"
#include "vppstrbuf.x"

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************

                            Start Of File

******************************************************************************/
#define SZIP_SIZEOF_INT      4

#if defined(SS_BREW_SYS) || defined(SS_SYMBIAN_SYS)
#define SZIP_NON_GLOBAL_VARIABLE
#endif

#if defined(SS_DBG)
#define SZIP_DBG
#endif

#if defined(SS_LOG)
#define SZIP_LOG_ENABLE
#endif

#if defined(SS_LITTLE_ENDIAN_SYS)
#define SZIP_LITTLE_ENDIAN
#endif

#define SZIP_LINT_DISABLE  /*lint -e767*/
#define SZIP_LINT_ENABLE   /*lint +e767*/

#define SZIP_LOG_LEVEL_INFO    SIP_LOG_LEVEL_INFO     /* Informational Log.*/
#define SZIP_LOG_LEVEL_WARNING SIP_LOG_LEVEL_WARNING  /* An abnormal condition.
                                                Usually occurs before an error*/
#define SZIP_LOG_LEVEL_ERROR   SIP_LOG_LEVEL_ERROR    /* An error has happened.
                                                Need to monitor stack */


#define SZIP_NULL            0
#define SZIP_NULL_PTR        0L
#define SZIP_NULL_LONG       0xFFFFFFFFL
#define SZIP_NULL_SHORT      0xFFFF
#define SZIP_NULL_BYTE       0xFF

#define SZIP_NULL_INT        -1
#define SZIP_NULL_INT8       -1
#define SZIP_NULL_INT16      -1
#define SZIP_NULL_INT32      -1

#define SZIP_NULL_UCHAR      0xFF

#define SZIP_NULL_UINT       0xFFFFFFFF
#define SZIP_NULL_UINT8      0xFF
#define SZIP_NULL_UINT16     0xFFFF
#define SZIP_NULL_UINT32     0xFFFFFFFF

#define SZIP_ZERO            0

#define SZIP_TRUE            1
#define SZIP_FALSE           0

#define SZIP_ENUM_END        0xFFFFFFF

#define SZIP_VOID            void

typedef          char        SZIP_CHAR;
typedef const    char        SZIP_CCHAR;
typedef unsigned char        SZIP_UCHAR;

typedef signed   char        SZIP_INT8;
typedef unsigned char        SZIP_UINT8;

typedef signed   short       SZIP_INT16;
typedef unsigned short       SZIP_UINT16;

#if SZIP_SIZEOF_INT == 4
  typedef signed int         SZIP_INT32;
#else
# error "no 32-bits type found !!"
#endif

#if SZIP_SIZEOF_INT == 4
  typedef unsigned int       SZIP_UINT32;
#else
# error "no 32-bits type found !!"
#endif

typedef unsigned int         SZIP_BOOL;

typedef          long        SZIP_LONG;
typedef unsigned long        SZIP_ULONG;

typedef unsigned long        SZIP_SIZET;

/* The datatypes dependent on VppCBB */

typedef VppStringS   SzipString;

typedef VppMemCpHdl  SzipMemCpHdl;

typedef VppStrBufHdl SzipStrBufHdl;


/******************************************************************************

                              End of file

******************************************************************************/

#ifdef __cplusplus
}
#endif


#endif /* __SZ_TYPES_H__ */

