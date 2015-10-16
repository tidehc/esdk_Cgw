/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : vpphash.x

    Author          : zhaolei (ronnie),Li Minghua

    Version         : 1.0

    Date            : 2005-11-12

    Description     : This file defines the interface functions and
                      structrues of Hash for user.
    Function List   :
        1. VppHashCreate
        2. VppHashDestroy
        3. VppHashSearch
        4. VppHashInsert
        5. VppHashRemove

    History:
        1. Date        : <YYYY-MM-DD>
           Tag         : None
           Author      :
           Modification:
******************************************************************************/
#ifndef _VPP_HASH_X_
#define _VPP_HASH_X_

#include "vppmemcp.x"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************

                            Start Of File

******************************************************************************/
typedef VPP_VOID* VppHashHdl;

typedef VPP_UINT32 (*pFnVppHashCmp)
(
    IN  VPP_VOID     *pvData, /* Any data or structure pointer */
    IN  VPP_VOID     *pulKey1,/* Three key value */
    IN  VPP_VOID     *pulKey2,
    IN  VPP_VOID     *pulKey3
);

typedef VPP_UINT32 (*pFnVppHashKey)
(
    IN  VPP_VOID     *pulKey1,/* Three key value */
    IN  VPP_VOID     *pulKey2,
    IN  VPP_VOID     *pulKey3
);

/******************************************************************************
 Function   : VppHashCreate

 Description: Create a Vpp Common Hash control block

 Input      : hdlMemCp     - MemCP memory handle pointer
              ulMapSize    - The map size of hash table
              pfHashKey    - Hash key caclulate function pointer
              pfHashCmp    - Hash compare function pointer

 Output     : phdlHash     - The hash control block handle pointer

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppHashCreate
(
    IN  VppMemCpHdl     hdlMemCp,
    IN  VPP_UINT32      ulMapSize,
    IN  pFnVppHashKey   pfnHashKey,
    IN  pFnVppHashCmp   pfnHashCmp,
    OUT VppHashHdl     *phdlHash
);

/******************************************************************************
 Function   : VppHashDestroy

 Description: Destroy a common Hash table

 Input      : phdlHash    - hash table control handle pointer

 Output     : phdlHash    - result of hash table control handle pointer

 Return     : None
******************************************************************************/
VPP_VOID VppHashDestroy
(
    IO  VppHashHdl    *phdlHash
);

/******************************************************************************
 Function   : VppHashSearch

 Description: Search data in the hash table by three key value

 Input      : hdlHash     - hash table control handle
              pulKey1~3   - 3 key value to search in the hash table

 Output     : ppvData     - Search result in the hash table

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppHashSearch
(
    IN  VppHashHdl    hdlHash,
    IN  VPP_VOID     *pulKey1,
    IN  VPP_VOID     *pulKey2,
    IN  VPP_VOID     *pulKey3,
    OUT VPP_VOID    **ppvData
);

/******************************************************************************
 Function   : VppHashInsert

 Description: Insert data to the hash table

 Input      : hdlHash     - hash table control handle
              pulKey1~3   - 3 key value to search in the hash table
              pvData      - data will saved in the hash table

 Output     : hdlHash     - hash table control pointer

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppHashInsert
(
    IO  VppHashHdl    hdlHash,
    IN  VPP_VOID     *pulKey1,
    IN  VPP_VOID     *pulKey2,
    IN  VPP_VOID     *pulKey3,
    IN  VPP_VOID     *pvData
);

/******************************************************************************
 Function   : VppHashRemove

 Description: Remove data from the hash table

 Input      : hdlHash     - hash table control handle
              pulKey1~3   - 3 key value to search in the hash table

 Output     : hdlHash     - hash table control handle
              ppvData     - data will remove from the hash table

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppHashRemove
(
    IO  VppHashHdl    hdlHash,
    IN  VPP_VOID     *pulKey1,
    IN  VPP_VOID     *pulKey2,
    IN  VPP_VOID     *pulKey3,
    OUT VPP_VOID    **ppvData
);

/******************************************************************************

                              End of file

******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _VPP_HASH_X_ */

