/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : vppset.x

    Author          : zhaolei (ronnie), Li Minghua

    Version         : 1.0

    Date            : 2005-11-12

    Description     : The file defines the interface functions and
                      structures of Set.

    Function List   :
        1. VppSetCreate
        2. VppSetDestroy
        3. VppSetGetData
        4. VppSetGetCount
        5. VppSetRemove
        6. VppSetInsert
        7. VppSetFindEmpty
        8. VppSetIterate

    History:
        1. Date        : <2006-05-17>
           Tag         : <Tag.YDHD02161>
           Author      : Manjunath Warad
           Modification: New API to get max count of the set is added.
******************************************************************************/
#ifndef _VPP_SET_X_
#define _VPP_SET_X_

#include "vppmemcp.x"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************

                            Start Of File

******************************************************************************/
typedef VPP_VOID * VppSetHdl;

typedef VPP_VOID (*pFnVppSetIterate)
(
    VPP_VOID       *pvData,    /* Any data or structure pointer */
    VPP_UINT16      usIndex    /* The position in Set */
);

/******************************************************************************
 Function   : VppSetCreate

 Description: This interface initialize a VPP Simple set control block buffer.

 Input      : hdlMemCp       - MemCP memory handle
              usInitElmNum   - The number of elements in Set.
              usStepElmNum   - The number of elements added each time when you
                               want to extend the Set.
                               Note: This value can be zero.
              phdlSet        - The Set handle

 Output     : phdlSet        - The Set handle

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppSetCreate
(
    IN  VppMemCpHdl   hdlMemCp,
    IN  VPP_UINT16    usInitElmNum,
    IN  VPP_UINT16    usStepElmNum,
    IO  VppSetHdl    *phdlSet
);

/******************************************************************************
 Function   : VppSetDestroy

 Description: This interface will destroy a VPP simple set structure.

 Input      : phdlSet      - The Set handle

 Output     : phdlSet      - The Set handle

 Return     : None
******************************************************************************/
VPP_VOID VppSetDestroy
(
    IO  VppSetHdl    *phdlSet
);

/******************************************************************************
 Function   : VppSetGetData

 Description: Get the data from the Vpp Set using index

 Input      : hdlSet       - The Set structure handle
              usIndex      - Position in the set array begin with ZERO

 Output     : ppvData      - Value saved in the set array

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppSetGetData
(
    IN  VppSetHdl     hdlSet,
    IN  VPP_UINT16    usIndex,
    OUT VPP_VOID    **ppvData
);

/******************************************************************************
 Function   : VppSetGetCount

 Description: Get the used count of data in the Vpp Set

 Input      : hdlSet       - The Set structure handle

 Output     : None

 Return     : Return the count of Set or ZERO which means failure.
******************************************************************************/
VPP_UINT16 VppSetGetCount
(
    IN  VppSetHdl     hdlSet
);

/* Begin <Tag.YDHD02161>*/
/******************************************************************************
 Function   : VppSetGetMaxCount

 Description: This API will give the maximum number of elements that
              can be use in Set. During Set Create, this will be equal to
              initialised value; however, this value might change during
              run-time by VppSetInsert API based on the requirement.

 Input      : hdlSet - The Set structure handle

 Output     : None

 Return     : Return the maxcount of Set or ZERO which means failure.
******************************************************************************/
VPP_UINT16 VppSetGetMaxCount
(
    IN  VppSetHdl     hdlSet
);
/* End <Tag.YDHD02161>*/

/******************************************************************************
 Function   : VppSetInsert

 Description: Insert a data to the Vpp Set

 Input      : hdlSet     - The Set structure handle
              usIndex    - The position of you want to insert which begin with 0.
              pvData     - Value will be saved in the set array

 Output     : hdlSet     - The Set structure handle

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppSetInsert
(
    IO  VppSetHdl     hdlSet,
    IN  VPP_UINT16    usIndex,
    IN  VPP_VOID     *pvData
);

/******************************************************************************
 Function   : VppSetRemove

 Description: Remove a data from the Vpp Set

 Input      : hdlSet     - The Set structure handle
              usIndex    - Position begin with ZERO in the set array of which
                           you want to remove.

 Output     : hdlSet     - The Set handle
              ppvData    - The value saved in the set array

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppSetRemove
(
    IO  VppSetHdl     hdlSet,
    IN  VPP_UINT16    usIndex,
    OUT VPP_VOID    **ppvData
);

/******************************************************************************
 Function   : VppSetFindEmpty

 Description: Find a empty index from the Set

 Input      : hdlSet    - The Set structure handle
              pusIndex  - Position in the set array

 Output     : hdlSet    - The Set structure handle
              pusIndex  - The real Position in the set array

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppSetFindEmpty
(
    IO  VppSetHdl     hdlSet,
    IO  VPP_UINT16   *pusIndex
);

/******************************************************************************
 Function   : VppSetIterate

 Description: Iterate all the set items, it can used for print,
              initialize or reset Set.

 Input      : hdlSet       - The Set structure handle
              pfnIterate   - The iterate function pointer

 Output     : None

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppSetIterate
(
    IN  VppSetHdl        hdlSet,
    IN  pFnVppSetIterate pfnIterate
);

/******************************************************************************

                              End of file

******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _VPP_SET_X_ */

