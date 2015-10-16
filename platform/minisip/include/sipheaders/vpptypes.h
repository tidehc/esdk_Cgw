/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : vpptypes.h

    Author          : zhaolei (ronnie), Li Minghua

    Version         : 1.0

    Date            : 2005-11-12

    Description     : The file defines the basic structures, macros and
                      interface functions of CBB.

    Function List   :

    History:
        1. Date        : <YYYY-MM-DD>
           Tag         : None
           Author      :
           Modification:
******************************************************************************/
#ifndef _VPP_TYPES_H_
#define _VPP_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************

                            Start Of File

******************************************************************************/

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IO
#define IO
#endif

#define VPP_NULL            0
#define VPP_NULL_PTR        0L
#define VPP_NULL_LONG       0xFFFFFFFFL
#define VPP_NULL_SHORT      0xFFFF
#define VPP_NULL_BYTE       0xFF

#define VPP_NULL_INT        (-1)
#define VPP_NULL_INT8       (-1)
#define VPP_NULL_INT16      (-1)
#define VPP_NULL_INT32      (-1)

#define VPP_NULL_UCHAR      0xFF

#define VPP_NULL_UINT       0xFFFFFFFF
#define VPP_NULL_UINT8      0xFF
#define VPP_NULL_UINT16     0xFFFF
#define VPP_NULL_UINT32     0xFFFFFFFF

#define VPP_ZERO            0

#define VPP_SUCCESS         0x0
#define VPP_FAILURE         0x1
#define VPP_INVALID_PARAM   0x2
#define VPP_MALLOC_FAIL     0x3
#define VPP_ASSERT_ERR      0x4

#ifndef CONST
#define CONST               const
#endif

#define VPP_TRUE            1
#define VPP_FALSE           0

#define VPP_ENUM_END        0xFFFFFFF
#define VPP_STR_MAXCHAR     1024

/* [[SWED11986] [2008-02-26] [l48202]]  【VPP发现问题】整理和规范代码中的宏定义问题 BEGIN */
#if defined(SS_DBG)
#define VPP_DBG
#endif
/* [[SWED11986] [2008-02-26] [l48202]] END */

#ifdef VPP_DBG
#define VPP_ASSERT(_condition) \
{ \
    if (!(_condition)) \
    { \
        VPP_RETURN(VPP_ASSERT_ERR); \
    } \
}
#define VPP_ASSERT_VOID(_condition) \
{ \
    if (!(_condition)) \
    { \
        return; \
    } \
}
#else
#define VPP_ASSERT(_condition)
#define VPP_ASSERT_VOID(_condition)
#endif

#define VPP_BINARY          2

#define VPP_OCTAL           8

#define VPP_DECIMAL         10

#define VPP_HEXADECIMAL     16

#define VPP_MEMSIZE_ALIGN(_size) ((_size + 3) & (~3))

#define VPP_UNUSED_VAR(x)   ((x) = (x))

#define VPP_RETVOID()       return

#define VPP_RETURN(_ret)    return(_ret)

/* define the basic type */

#define VPP_VOID        void

typedef signed int         VPP_SINT32;

typedef unsigned int       VPP_UINT32;

typedef unsigned short  VPP_UINT16;
typedef signed   short  VPP_SINT16;

typedef unsigned char   VPP_UINT8;
typedef signed   char   VPP_SINT8;

typedef unsigned char   VPP_UCHAR;
typedef signed   char   VPP_SCHAR;
typedef          char   VPP_CHAR;


typedef unsigned int    VPP_BOOL;



/* The result of compare which string and strbuf will use it. */
typedef enum _EN_VPP_CMP_RESULT
{
    VPP_RET_LESS = -1,     /* The first data less than second data */
    VPP_RET_IDENTICAL,     /* The two data is equal */
    VPP_RET_GREATER,       /* The first data larger than second data */
    VPP_RET_NOT_IDENTICAL, /* Invalid input parameter */
    VPP_RET_BUTT,
    VPP_RET_ENMU_END = VPP_ENUM_END
}EN_VPP_CMP_RESULT;

/******************************************************************************

                              End of file

******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _VPP_TYPES_H_  */


