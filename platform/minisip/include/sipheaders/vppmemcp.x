/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : vppmemcp.x

    Author          : zhaolei (ronnie), Li Minghua

    Version         : 1.0

    Date            : 2005-11-12

    Description     : This file defines the interface functions of MemCp.

    Function List   :
        1. VppMemCpCreate
        2. VppMemCpDestroy
        3. VppMemCpMalloc
        4. VppMemCpRealloc
        5. VppMemCpMallocX
        6. VppMemCpReallocX
        7. VppMemCpFreeX

    History:
        1. Date        : <YYYY-MM-DD>
           Tag         : None
           Author      :
           Modification:
******************************************************************************/
#ifndef _VPP_MEMCP_X_
#define _VPP_MEMCP_X_

#include "vpptypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************

                            Start Of File

******************************************************************************/
typedef VPP_VOID   *VppMemCpHdl;
typedef VPP_VOID   *VppMemCpStateHdl;

/******************************************************************************
 Function   : VppMemCpCreate

 Description: Create MemCp for memory manage

 Input      : ulInitSize   - The first page size of MemCP
              ulMinSize    - The minimum page size of MemCP

 Output     : phdlMemCp    - The output MemCP handle pointer

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppMemCpCreate
(
    IN  VPP_UINT32    ulInitSize,
    IN  VPP_UINT32    ulMinSize,
    OUT VppMemCpHdl  *phdlMemCp
);

/******************************************************************************
 Function   : VppMemCpDestroy

 Description: Destroy MemCP from system memory

 Input      : phdlMemCp    - The pointer of the MemCP handle

 Output     : phdlMemCp    - The pointer of the MemCP handle

 Return     : None
******************************************************************************/
VPP_VOID VppMemCpDestroy
(
    IO  VppMemCpHdl   *phdlMemCp
);

/******************************************************************************
 Function   : VppMemCpMalloc

 Description: Allocated memory from the MemCP.

 Input      : phdlMemCp  - The handle of the MemCP
              ulSize     - The size of memory user want to allocate.

 Output     : None

 Return     : VPP_NULL_PTR if failure, the allocated memory pointer if success.
******************************************************************************/
VPP_VOID *VppMemCpMalloc
(
    IN  VppMemCpHdl   hdlMemCp,
    IN  VPP_UINT32    ulSize
);

/******************************************************************************
 Function   : VppMemCpRealloc

 Description: Reallocated memory from the MemCP if the old memory is not enough.

 Input      : hdlMemCp      - The handle of the MemCP
              pvOldMem      - The pointer of old memory block
              ulOldSize     - The old memory size
              ulNewSize     - The new memory size

 Output     : pvOldMem      - The pointer of old memory block

 Return     : VPP_NULL_PTR if failure, the allocated memory pointer if success.
******************************************************************************/
VPP_VOID *VppMemCpRealloc
(
    IN  VppMemCpHdl    hdlMemCp,
    IO  VPP_VOID      *pvOldMem,
    IN  VPP_UINT32     ulOldSize,
    IN  VPP_UINT32     ulNewSize
);

/******************************************************************************
 Function   : VppMemCpMallocX

 Description: This API encapsulates the memory allocation that can be allocated
              from MemCp or SysMem.

 Input      : hdlMemCp     - The handle of the MemCp
              ulSize       - The size of user need to allocated.

 Output     : None

 Return     : VPP_NULL_PTR if failure, the allocated memory pointer if success.
******************************************************************************/
VPP_VOID *VppMemCpMallocX
(
    IN  VppMemCpHdl   hdlMemCp,
    IN  VPP_UINT32    ulSize
);

/******************************************************************************
 Function   : VppMemCpReallocX

 Description: Reallocates memory from the MemCP or system memory if the old
              memory is not enough.
              Note:This API encapsulated the MemCp or SysMem alloction which
              hdlMemCp can be null or can be not null.
                   The invoked VppSysRealloc's protoypte is void *realloc(
              void *memblock, size_t size ) which the original block can be
              freed automatically.


 Input      : hdlMemCp      - The handle of the MemCP
              pvOldMem      - The pointer of old memory block
              ulOldSize     - The old memory size
              ulNewSize     - The new memory size

 Output     : None

 Return     : VPP_NULL_PTR if failure, the allocated memory pointer if success.
******************************************************************************/
VPP_VOID *VppMemCpReallocX
(
    IN  VppMemCpHdl    hdlMemCp,
    IO  VPP_VOID      *pvOldMem,
    IN  VPP_UINT32     ulOldSize,
    IN  VPP_UINT32     ulNewSize
);

/******************************************************************************
 Function   : VppMemCpFreeX

 Description: Free memory using the MemCp. Only the system memory is freed, and
              the buffer memory won't be freed.

 Input      : hdlMemCp      - The handle of the MemCP
              ppvFree       - The memory expected to free

 Output     : None

 Return     : None
******************************************************************************/
VPP_VOID VppMemCpFreeX
(
    IN VppMemCpHdl   hdlMemCp,
    IN VPP_VOID    **ppvFree
);

/******************************************************************************
 Function   : VppMemCpSaveState

 Description: Save the current memory state in MemCp.
              Prepare for restore when some operation failures.

 Input      : hdlMemCp       - The MemCP handle pointer

 Output     : phdlMemCpState - The VppMemCpStateS handle pointer

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppMemCpSaveState
(
    IN  VppMemCpHdl        hdlMemCp,
    OUT VppMemCpStateHdl  *phdlMemCpState
);

/******************************************************************************
 Function   : VppMemCpRestore

 Description: Restore the MemCp to the previous state before the latest
              operation when the operation is failure.

 Input      : hdlMemCp      - The MemCP handle pointer

 Output     : hdlMemCpState - The VppMemCpStateS handle pointer

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppMemCpRestore
(
    IO  VppMemCpHdl        hdlMemCp,
    IN  VppMemCpStateHdl   hdlMemCpState
);

/******************************************************************************

                              End of file

******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _VPP_MEMCP_X_ */

