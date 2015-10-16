/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : vpplist.x

    Author          : zhaolei (ronnie), Li Minghua

    Version         : 1.0

    Date            : 2005-11-12

    Description     : The file defines the interface functions and
                      structures of List.

    Function List   :
        1. VppListCreate
        2. VppListDestroy
        3. VppListGetData
        4. VppListGetCount
        5. VppListInsert
        6. VppListRemove
        7. VppListIterate

    History:
        1. Date        : <YYYY-MM-DD>
           Tag         : None
           Author      :
           Modification:
******************************************************************************/
#ifndef _VPP_LIST_X_
#define _VPP_LIST_X_

#include "vppmemcp.x"

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************

                            Start Of File

******************************************************************************/
typedef VPP_VOID * VppListHdl;

typedef VPP_VOID (*pFnVppListIterate)
(
    VPP_VOID       *pvData,   /* Any data or structure pointer */
    VPP_UINT16      usIndex   /* The position in List */
);

/******************************************************************************
 Function   : VppListCreate

 Description: Create a Vpp List control block

 Input      : hdlMemCp     - MemCP memory handle
              usInitElmNum - The number of elements in List.
              usStepElmNum - The number of elements added each time when you
                             want to extend the List.
                             Note: This value can be ZERO.
              phdlList   - The VppListS handle

 Output     : phdlList   - The VppListS handle

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppListCreate
(
    IN  VppMemCpHdl   hdlMemCp,
    IN  VPP_UINT16    usInitElmNum,
    IN  VPP_UINT16    usStepElmNum,
    IO  VppListHdl   *phdlList
);

/******************************************************************************
 Function    : VppListDestroy

 Description : Destroy a Vpp List control block

 Input       : phdlList      - The VppListS handle

 Output      : phdlList      - The VppListS handle

 Return      : None
******************************************************************************/
VPP_VOID VppListDestroy
(
    IO  VppListHdl     *phdlList
);

/******************************************************************************
 Function    : VppListGetData

 Description : Get the data from a Vpp List control block

 Input       : hdlList     - The handle of List
               usIndex     - The position begin with zero of which
                             you want to get the data.

 Output      : ppvData     - The data pointer of you want to get.

 Return      : VPP_SUCCESS or any other error code
******************************************************************************/
VPP_UINT32 VppListGetData
(
    IN  VppListHdl    hdlList,
    IN  VPP_UINT16    usIndex,
    OUT VPP_VOID    **ppvData
);

/******************************************************************************
 Function    : VppListGetCount

 Description : Get the used count of a Vpp List control block

 Input       : hdlList     - The handle of List

 Output      : None

 Return      : Return the count of Set or ZERO which means failure.
******************************************************************************/
VPP_UINT16 VppListGetCount
(
    IN  VppListHdl     hdlList
);

/******************************************************************************
 Function    : VppListInsert

 Description : Insert a data to the Vpp List control block

 Input       : hdlList     - The handle of List
               usIndex     - The position of you want to insert which begin with 0.
               pvData      - Value will be saved in the set array

 Output      : hdlList     - The handle of List

 Return      : VPP_SUCCESS or any other error code
******************************************************************************/
VPP_UINT32 VppListInsert
(
    IO  VppListHdl    hdlList,
    IN  VPP_UINT16    usIndex,
    IN  VPP_VOID     *pvData
);

/******************************************************************************
 Function    : VppListRemove

 Description : Remove a data from the Vpp List control block

 Input       : hdlList     - The handle of List
               usIndex     - The data position begin with ZERO of which you
                             want to remove.

 Output      : ppvData     - the pointer of removed data

 Return      : VPP_SUCCESS or any other error code
******************************************************************************/
VPP_UINT32 VppListRemove
(
    IO  VppListHdl    hdlList,
    IN  VPP_UINT16    usIndex,
    OUT VPP_VOID    **ppvData
);

/******************************************************************************
 Function    : VppListIterate

 Description : Iterate a Vpp List control block, it can used for print,
               initialize or reset List.

 Input       : hdlList     - The handle of List
               pfnIterate  - The pointer of iterate function

 Output      : None

 Return      : VPP_SUCCESS or any other error code
******************************************************************************/
VPP_UINT32 VppListIterate
(
    IN  VppListHdl         hdlList,
    IN  pFnVppListIterate  pfnIterate
);

/******************************************************************************
 Function    : VppListAppendList

 Description : Appends the contents of source list to destination list

 Input       : hdlDstList - Destination list where the items has to be appended
               hdlSrcList - Source list

 Output      : hdlDstList - Destination list after appending

 Return      : VPP_SUCCESS or any other error code.
******************************************************************************/

VPP_UINT32 VppListAppendList
(
    IO VppListHdl hdlDstList,
    IN VppListHdl hdlSrcList
);
/******************************************************************************

                              End of file

******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _VPP_LIST_X_ */

