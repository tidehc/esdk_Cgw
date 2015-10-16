/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : vppstring.x

    Author          : zhaolei (ronnie), Li Minghua

    Version         : 1.0

    Date            : 2005-11-12

    Description     : This file supports the APIs of String.

    Function List   :
        1. VppStringCreate
        2. VppStringDataCreate
        3. VppStringDataDestroy
        4. VppStringDestroy
        5. VppStringCmp
        6. VppStringICmp
        7. VppStringNCmp
        8. VppStringNICmp
        9. VppStringCpy
        10.VppStringNCpy
        11.VppStringStr
        12.VppStringIStr
        13.VppStringIsDigit
        14.VppStringGetDigit
        15.VppStringFillData
        16.VppStringUlToA
        17.VppStringIToA
        18.VppStringAToUS
        19.VppStringAToUl

    History:
        1. Date        : <YYYY-MM-DD>
           Tag         : None
           Author      :
           Modification:
******************************************************************************/
#ifndef _VPP_STRING_X_
#define _VPP_STRING_X_

#include "vppmemcp.x"

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************

                            Start Of File

******************************************************************************/
#define VPPSTR_SET_STR(_pstStr, _pcDat, _ulLen)\
{\
    (_pstStr)->ulLen = _ulLen;\
    (_pstStr)->pcData = _pcDat;\
}

#define VPPSTR_GET_LEN(_pstStr) ((_pstStr)->ulLen)
#define VPPSTR_GET_DATA(_pstStr) ((_pstStr)->pcData)

#define VPP_TOLOWER(_cChar)  ((((_cChar) >= 'A') && ((_cChar) <= 'Z'))\
                             ? ((_cChar) + 'a' - 'A'):(_cChar))

#define VPP_STR_CHAR_TO_DIGIT(_cChar) (((((_cChar) >= 'A') && ((_cChar) <= 'F'))\
                                 || (((_cChar) >= 'a') && ((_cChar) <= 'f')))\
                                 ? ((((_cChar) >= 'A') && ((_cChar) <= 'F'))\
                                 ? (((_cChar) + 10) - 'A')\
                                 : (((_cChar) + 10) - 'a'))\
                                 : ((_cChar) - '0'))

#define VPPSTR_GET_CHAR(_pstStr, _usOffset)\
    (((_usOffset) < (_pstStr)->ulLen) ? (_pstStr)->pcData[_usOffset] : 0)

typedef struct _VppStringS
{
    VPP_CHAR      *pcData; /* The string data */
    VPP_UINT32     ulLen;  /* The string length */
}VppStringS;

/******************************************************************************
 Function   : VppStringCreate

 Description: This function allocates the memory for the data part of the
              string, which will be filled later.

 Input      : hdlMemCp  - MemCp handle pointer
              pcData    - string data pointer
              ulLen     - string length

 Output     : ppstStr   - the string where the data is copied

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStringCreate
(
    IN  VppMemCpHdl   hdlMemCp,
    IN  VPP_CHAR     *pcData,
    IN  VPP_UINT32    ulLen,
    OUT VppStringS  **ppstStr
);

/******************************************************************************
 Function   : VppStringDataCreate

 Description: This function allocates the memory for the string data space.

 Input      : hdlMemCp - MemCp handle pointer
              pstStr   - VppStringS pointer
              pcData   - string data pointer
              ulLen    - length of allocation

 Output     : pstStr   - VppStringS pointer

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStringDataCreate
(
    IN  VppMemCpHdl   hdlMemCp,
    IN  VPP_CHAR     *pcData,
    IN  VPP_UINT32    ulLen,
    IO  VppStringS   *pstStr
);

/******************************************************************************
 Function   : VppStringDataDestroy

 Description: This function destroy a String data.

 Input      : hdlMemCp - MemCp handle pointer
              pStr     - VppStringS pointer

 Output     : pStr     - VppStringS pointer

 Return     : None
******************************************************************************/
VPP_VOID VppStringDataDestroy
(
    IN  VppMemCpHdl   hdlMemCp,
    IO  VppStringS   *pStr
);

/******************************************************************************
 Function   : VppStringDestroy

 Description: This function destroy the memory for string.

 Input      : hdlMemCp - MemCp handle pointer
              ppStr    - String pointer of need to freed

 Output     : ppStr    - String pointer

 Return     : None
******************************************************************************/
VPP_VOID VppStringDestroy
(
    IN  VppMemCpHdl   hdlMemCp,
    IO  VppStringS  **ppStr
);

/******************************************************************************
 Function   : VppStringGetCStr

 Description: Convert VppString to c string('\0' append).

 Input      : hdlMemCp - MemCp handle
              pstStr   - VppString pointer

 Output     : None

 Return     : C string pointer
******************************************************************************/
VPP_CHAR* VppStringGetCStr
(
    IN VppMemCpHdl      hdlMemCp,
    IN VppStringS      *pstStr
);

/******************************************************************************
 Function   : VppStringCmp

 Description: This function compare two string.

 Input      : pstFirstStr  - VppStringS need to compare
              pstSecondStr - VppStringS need to compare

 Output     : None

 Return     : the value of enum EN_VPP_CMP_RESULT
******************************************************************************/
EN_VPP_CMP_RESULT VppStringCmp
(
    IN  VppStringS  *pstFirstStr,
    IN  VppStringS  *pstSecondStr
);

/******************************************************************************
 Function   : VppStringICmp

 Description: Compare two string case insensitive

 Input      : pstFirstStr  - VppStringS need to compare
              pstSecondStr - VppStringS need to compare

 Output     : None

 Return     : The value of enum EN_VPP_CMP_RESULT
******************************************************************************/
EN_VPP_CMP_RESULT VppStringICmp
(
    IN  VppStringS  *pstFirstStr,
    IN  VppStringS  *pstSecondStr
);

/******************************************************************************
 Function   : VppStringNCmp

 Description: Compare two string use max compare length

 Input      : pstFirstStr - VppStringS need to compare
              pcSecondStr - String need to compare
              ulLen       - Length for the compare operation

 Output     : None

 Return     : the value of enum EN_VPP_CMP_RESULT
******************************************************************************/
EN_VPP_CMP_RESULT VppStringNCmp
(
    IN  VppStringS   *pstFirstStr,
    IN  VPP_CHAR     *pcSecondStr,
    IN  VPP_UINT32    ulLen
);

/******************************************************************************
 Function   : VppStringNICmp

 Description: Compare two string case insensitive and use max compare length.

 Input      : pstFirstStr - VppStringS need to compare
              pcSecondStr - String need to compare
              ulLen       - Length for the compare operation

 Output     : None

 Return     : the value of enum EN_VPP_CMP_RESULT
******************************************************************************/
EN_VPP_CMP_RESULT VppStringNICmp
(
    IN  VppStringS   *pstFirstStr,
    IN  VPP_CHAR     *pcSecondStr,
    IN  VPP_UINT32    ulLen
);

/******************************************************************************
 Function   : VppStringCpy

 Description: This function copies the string to another string.

 Input      : pstSrc   - VppStringS pointer Source

 Output     : pstDest  - VppStringS pointer Destination

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStringCpy
(
    OUT VppStringS   *pstDest,
    IN  VppStringS   *pstSrc
);

/******************************************************************************
 Function   : VppStringNCpy

 Description: This function copies the string to another string using the
              max length.

 Input      : pcSrc   - source string pointer
              ulLen   - string length

 Output     : pstDest - VppStringS pointer Source

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStringNCpy
(
    OUT VppStringS   *pstDest,
    IN  VPP_CHAR     *pcSrc,
    IN  VPP_UINT32    ulLen
);


/******************************************************************************
 Function   : VppStringIsDigit

 Description: Judge a char is a digit or not

 Input      : cChar  - destination char
              usBase - digit base

 Output     : None

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_BOOL VppStringIsDigit
(
    IN  VPP_CHAR      cChar,
    IN  VPP_UINT16    usBase
);

/******************************************************************************
 Function   : VppStringGetDigit

 Description: Get digit from specified string.

 Input      : pstStr    - VppStringS pointer
              usBase    - Digit base

 Output     : pulDigit  - digit have found
              pusOffset - position of digit end

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStringGetDigit
(
    IN  VppStringS   *pstStr,
    IN  VPP_UINT16    usBase,
    OUT VPP_UINT32   *pulDigit,
    OUT VPP_UINT16   *pusOffset
);

/******************************************************************************
 Function   : VppStringFillData

 Description: This function copies the given data to a position specified in
              the string.

 Input      : pstInStr - VppStringS pointer
              pcData   - Data string pointer
              ulIndex  - Position of fill in the VppStringS
              ulLen    - the length of the data space to be allocated.

 Output     : pstInStr - VppStringS pointer

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStringFillData
(
    IO  VppStringS   *pstInStr,
    IN  VPP_UINT32    ulIndex,
    IN  VPP_CHAR     *pcData,
    IN  VPP_UINT32    ulLen
);

/******************************************************************************
 Function   : VppStringAppendData

 Description: This function appending the given data to a to the string.

 Input      : hdlMemCp  - MemCp handle
              pstString - VppStringS pointer
              pcData    - Data string pointer
              ulIndex   - Position of fill in the VppStringS
              ulLen     - the length of the data space to be reallocated.

 Output     : pstString - VppStringS pointer

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStringAppendData
(
    IN  VppMemCpHdl   hdlMemCp,
    IO  VppStringS   *pstString,
    IN  VPP_CHAR     *pcData,
    IN  VPP_UINT32    ulLen
);

/******************************************************************************
 Function   : VppStringUlToA

 Description: Convert a unsigned integer number to char array. To avoid
              the reverse or recurse algorithm, a pointer to the input
              array is returned which point to the start of the first
              digit after converting.
              Caller must make sure the space of char array can
              accomandate the chars. Typically, the size of the array can
              be 10.

 Input      : ulInt      - the integer number will be converted
              pcAry      - the input char array which the converted char
                           will be put.
              ulAryLen   - the length of the input char array.

 Output     : pstCvtdStr - the pointer of output string.

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStringUlToA
(
    IN  VPP_UINT32    ulInt,
    IN  VPP_CHAR     *pcAry,
    IN  VPP_UINT32    ulAryLen,
    OUT VppStringS   *pstCvtdStr
);

/******************************************************************************
 Function   : VppStringIToA

 Description: Convert a signed integer number to char array. To avoid
              the reverse or recurse algorithm, a pointer to the input
              array is returned which point to the start of the first
              digit after converting.
              Caller must make sure the space of char array can
              accomandate the chars. Typically, the size of the array can
              be 10.

 Input      : siInt       - the integer number will be converted
              pcAry      - the input char array which the converted char
                           will be put.
              ulAryLen   - the length of the input char array.

 Output     : pstCvtdStr - the pointer of output string.

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStringIToA
(
    IN  VPP_SINT32    siInt,
    IN  VPP_CHAR     *pcAry,
    IN  VPP_UINT32    ulAryLen,
    OUT VppStringS   *pstCvtdStr
);

/******************************************************************************
 Function   : VppStringAToUS

 Description: Convert char array to a short integer. The radix is fixed to
              be 10.
              Notes: it cannot convert a string to a negative number.
              Also the conversion risk in case of long string is left to
              the caller

 Input      : pcChar     - the char array will be converted.
              usLen      - the length of the char array.

 Output     : pusConvNum - the converted integer.

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStringAToUS
(
    IN  VPP_CHAR     *pcChar,
    IN  VPP_UINT16    usLen,
    OUT VPP_UINT16   *pusConvNum
);

/******************************************************************************
 Function   : VppStringAToUl

 Description: Convert char array to a integer. The radix is fixed to be
              10.
              Notes: it cannot convert a string to a negative number.

 Input      : pcChar     - the char array will be converted.
              ulLen      - the length of the char array.

 Output     : pulConvNum - the converted integer.

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStringAToUl
(
    IN  VPP_CHAR     *pcChar,
    IN  VPP_UINT32    ulLen,
    OUT VPP_UINT32   *pulConvNum
);

/******************************************************************************

                              End of file

******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _VPP_STRING_X_ */

