/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : vpphashutil.x

    Author          : zhaolei (ronnie), Li Minghua

    Version         : 1.0

    Date            : 2005-11-12

    Description     : The file defines the interface functions and
                      structures of HASH KEY.

    Function List   :
        1. VppStrHashKey
        2. VppIdHashKey

    History:
        1. Date        : <YYYY-MM-DD>
           Tag         : None
           Author      :
           Modification:
******************************************************************************/
#ifndef _VPP_HASHUTIL_X_
#define _VPP_HASHUTIL_X_

#include "vpptypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************

                            Start Of File

******************************************************************************/

/******************************************************************************
 Function   : VppStrHashKey

 Description: Calculate the key in Hash by String

 Input      : usType  - Any int type value
              pcStr   - the pointer of string
              usLen   - Any int type value

 Output     : None

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStrHashKey
(
    IN  VPP_UINT16     usType,
    IN  VPP_CHAR      *pcStr,
    IN  VPP_UINT16     usLen
);

/******************************************************************************
 Function   : VppIdHashKey

 Description: Calculate the key in Hash by Id

 Input      : usType  - Any int type value
              usId    - Any int type value

 Output     : None

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppIdHashKey
(
    IN  VPP_UINT16    usType,
    IN  VPP_UINT16    usId
);

/******************************************************************************

                              End of file

******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _VPP_HASHUTIL_X_ */

