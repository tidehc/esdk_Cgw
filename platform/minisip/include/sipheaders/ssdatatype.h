/*******************************************************************************
    Copyright (C), 1988-1999, Huawei Tech. Co., Ltd.

    FileName   : ssdatatype.h

    Author     : SriniCH

    Version    : 1.0

    Date       : 2005-11-12

    Description: This file contains some definition of SIP stack.


    Function List:
        None

    History:
        1. Date        : yyyy-mm-dd
           Tag         : <MOD.00X>
           Author      : XXX
           Modification: XXX
*******************************************************************************/


#ifndef __SSDATATYPE_H__
#define __SSDATATYPE_H__


#ifdef __cplusplus
extern "C" {
#endif


#ifndef CONST
#define CONST              const
#endif

#ifdef  _lint
#define STATIC             static
#else
#define STATIC
#endif

/* Define the Mini SIP types */
#define SS_VOID             void

typedef const void         SS_CVOID;

typedef unsigned char      SS_UINT8;

typedef signed char        SS_INT8;
typedef char               SS_CHAR;
typedef const char         SS_CCHAR;
typedef unsigned char      SS_UCHAR;

typedef unsigned short     SS_UINT16;
typedef signed   short     SS_INT16;

typedef signed int         SS_INT32;

typedef unsigned int       SS_UINT32;

typedef signed int         SS_INT;
typedef unsigned int       SS_UINT;

typedef SS_UINT32          SS_BOOL;

typedef long               SS_LONG;
typedef unsigned long      SS_ULONG;

typedef unsigned long      SS_SIZET;

/* Define some welknown values for readable */

#define SS_NULL            0
#define SS_NULL_PTR        0L
#define SS_NULL_LONG       0xFFFFFFFFL
#define SS_NULL_SHORT      0xFFFF
#define SS_NULL_BYTE       0xFF

#define SS_NULL_INT        -1
#define SS_NULL_INT8       -1
#define SS_NULL_INT16      -1
#define SS_NULL_INT32      -1

#define SS_NULL_UCHAR      0xFF

#define SS_NULL_UINT       0xFFFFFFFF
#define SS_NULL_UINT8      0xFF
#define SS_NULL_UINT16     0xFFFF
#define SS_NULL_UINT32     0xFFFFFFFF

#define SS_ZERO            0
#define SS_ONE             1

#define SS_SUCCESS         0
#define SS_FAILURE         1

#define SS_TRUE            1
#define SS_FALSE           0

#ifdef __cplusplus
}
#endif /* __cpluscplus */

#endif /* __SSDATATYPE_H__ */

