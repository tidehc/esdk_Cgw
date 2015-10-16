/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName        : vppstrbuf.x

    Author          : zhaolei (ronnie), Li Minghua

    Version         : 1.0

    Date            : 2005-11-12

    Description     : The file defines the interface functions and
                      structures of StrBuff.

    Function List   :
        1. VppStrBufCreate
        2. VppStrBufDestroy
        3. VppStrBufReset
        4. VppStrBufClone
        5. VppStrBufGetLength
        6. VppStrBufInsert
        7. VppStrBufRemove
        8. VppStrBufCutTail
        9. VppStrBufAppendData
       10. VppStrBufCopyDataOut
       11. VppStrBufQuery
       12.VppStrBufCompare
       13.VppStrBufInsertFromStrBuf
       14.VppStrBufAppendFromStrBuf
       15.VppStrBufFillData
       16.VppStrBufFillDataFromStrBuf

    History:
        1. Date        : <YYYY-MM-DD>
           Tag         : None
           Author      :
           Modification:
******************************************************************************/
#ifndef _VPP_STRBUF_X_
#define _VPP_STRBUF_X_

#include "vppstring.x"

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************

                            Start Of File

******************************************************************************/
typedef VPP_VOID * VppStrBufHdl;

/******************************************************************************
 Function   : VppStrBufCreate

 Description: Init a StrBuf control block.

 Input      : hdlMemCp     - MemCP memory handle
              usPageSize   - The size of page used to manage StrBuf memory
              pvData       - Start pointer of the data to be initialized.
              ulLen        - The length of data to be initialized

 Output     : phdlStrBuf   - The strBuff control block handle

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStrBufCreate
(
    IN  VppMemCpHdl    hdlMemCp,
    IN  VPP_UINT16     usPageSize,
    IN  VPP_VOID      *pvData,
    IN  VPP_UINT32     ulLen,
    OUT VppStrBufHdl  *phdlStrBuf
);

/******************************************************************************
 Function   : VppStrBufDestroy

 Description: Destroy a StrBuf control block.

 Input      : phdlStrBuf   - The handle of the StrBuf control block

 Output     : phdlStrBuf   - The result of strBuff control block handle

 Return     : None.
******************************************************************************/
VPP_VOID VppStrBufDestroy
(
    IO  VppStrBufHdl  *phdlStrBuf
);

/******************************************************************************
 Function   : VppStrBufReset

 Description: Reset a StrBuf control block.

 Input      : phdlStrBuf   - The handle of the StrBuf control block

 Output     : phdlStrBuf   - The result of strBuff control block handle

 Return     : None.
******************************************************************************/
VPP_VOID VppStrBufReset
(
    IO  VppStrBufHdl    hdlStrBuf
);

/******************************************************************************
    Function Name : VppStrBufClone

    Description   : This function clone the data from source strbuf to
                    dest strbuf.  Allocates memory for dest strbuf.

    Inputs        : hdlMemCp         - MemCp handle
                    hdlSrcStrBuf     - Source StrBuf handle.

    Output        : phdlDestStrBuf   - Destination StrBuf handle

    Returns       : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStrBufClone
(
    IN  VppMemCpHdl   hdlMemCp,
    OUT VppStrBufHdl *psthdlDestStrBuf,
    IN  VppStrBufHdl  hdlSrcStrBuf
);

/******************************************************************************
 Function   : VppStrBufGetLength

 Description: Get the used length in StrBuf.

 Input      : hdlStrBuf   - The handle of the StrBuf control block

 Output     : None

 Return     : Return the length of StrBuf or ZERO means failure.
******************************************************************************/
VPP_UINT32 VppStrBufGetLength
(
    IN  VppStrBufHdl   hdlStrBuf
);

/******************************************************************************
 Function   : VppStrBufInsert

 Description: Insert a section data to the StrBuf.

 Input      : hdlStrBuf    - The handle of the StrBuf control block
              ulIndex      - The start index begin with 0 to be inserted,
                             if index is outbound append it.
              pvData       - The start pointer of data to be inserted
              ulLen        - The length of data to be inserted.

 Output     : hdlStrBuf    - The strBuff control block handle

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStrBufInsert
(
    IO  VppStrBufHdl  hdlStrBuf,
    IN  VPP_UINT32    ulIndex,
    IN  VPP_VOID     *pvData,
    IN  VPP_UINT32    ulLen
);

/******************************************************************************
 Function   : VppStrBufRemove

 Description: Remove a section data from the StrBuf.

 Input      : hdlStrBuf   - The handle of the StrBuf control block
              ulIndex     - The start index of data to be removed in the StrBuf
              ulLen       - The length of data section to be removed

 Output     : hdlStrBuf   - The strBuff control block handle

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStrBufRemove
(
    IO  VppStrBufHdl  hdlStrBuf,
    IN  VPP_UINT32    ulIndex,
    IN  VPP_UINT32    ulLen
);

/******************************************************************************
 Function   : VppStrBufCutTail

 Description: Remove data from the tail of StrBuf
              (Can be remove the whole StrBuf)

 Input      : hdlStrBuf  - The handle of the StrBuf control block
              ulLen      - The length of which want to cut from tail
                           (ulLen <= pstStrBuf->ulLength)

 Output     : hdlStrBuf  - The handle of the StrBuf control block

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStrBufCutTail
(
    IO  VppStrBufHdl  hdlStrBuf,
    IN  VPP_UINT32    ulLen
);

/******************************************************************************
 Function   : VppStrBufAppendData

 Description: Append data to the tail of StrBuf directly.

 Input      : hdlStrBuf   - The handle of the StrBuf control block
              pvData      - The start pointer of data
              ulLen       - The length of data which want to append, can not be 0

 Output     : None

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStrBufAppendData
(
    IO  VppStrBufHdl  hdlStrBuf,
    IN  VPP_VOID     *pvData,
    IN  VPP_UINT32    ulLen
);

/******************************************************************************
 Function   : VppStrBufCopyDataOut

 Description: Copy a section data out from the StrBuf

 Input      : hdlStrBuf    - The handle of the StrBuf control block
              ulIndex      - The start position of data to be copied out
                             begin with ZERO.
              pvData       - The user buffer data pointer
              pulLen       - The data length want to copy out.
                             Can not be ZERO.

 Output     : pvData       - The user buffer data pointer
              pulLen       - The pointer of length real copied out

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStrBufCopyDataOut
(
    IN  VppStrBufHdl  hdlStrBuf,
    IN  VPP_UINT32    ulIndex,
    IO  VPP_VOID     *pvData,
    IO  VPP_UINT32   *pulLen
);

/******************************************************************************
 Function   : VppStrBufQuery

 Description: Query a memory block from StrBuf if continuous, if the memory
              block is not continuous will return VPP_NULL_PTR.

 Input      : hdlStrBuf    - The handle of the StrBuf control block
              ulIndex      - The start position of data to be query begin
                             with ZERO.
              ulLen        - The data length want to query.

 Output     : None

 Return     : Data pointer if success, VPP_NULL_PTR if failure.
******************************************************************************/
VPP_VOID* VppStrBufQuery
(
    IN  VppStrBufHdl  hdlStrBuf,
    IN  VPP_UINT32    ulIndex,
    IN  VPP_UINT32    ulLen
);


/******************************************************************************
 Function   : VppStrBufCompare

 Description: Compare two segment in one StrBuf or in different two StrBuf.

 Input      : hdlFirStrBuf     - The handle of the first StrBuf control block
              hdlSecStrBuf     - The handle of the second StrBuf control block
              ulFirIndex       - The start position to be compared in first
                                 StrBuf that cannot be outbound.
              ulSecIndex       - The start position to be compared in second
                                 StrBuf that cannot be outbound.
              ulLen            - The data length to be compared

 Output     : None

 Return     : The value of enum EN_VPP_CMP_RESULT.
******************************************************************************/
EN_VPP_CMP_RESULT VppStrBufCompare
(
    IN  VppStrBufHdl  hdlFirStrBuf,
    IN  VppStrBufHdl  hdlSecStrBuf,
    IN  VPP_UINT32    ulFirIndex,
    IN  VPP_UINT32    ulSecIndex,
    IN  VPP_UINT32    ulLen
);

/******************************************************************************
 Function   : VppStrBufInsertFromStrBuf

 Description: Insert a section data from Src String buffer to Destination StrBuf.

 Input      : hdlSrcStrBuf - The handle of the Source StrBuf Buffer
              ulSrcIndex   - The start index of the Src String buffer.
              ulDstIndex   - The start index of the Dest String buffer from here
                             the has to be inserted.
              ulLen        - The length of data to be inserted, can not be 0

 Output     : hdlDstStrBuf    - The Destination strBuff handle

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStrBufInsertFromStrBuf
(
    IO  VppStrBufHdl  hdlDstStrBuf,
    IN  VppStrBufHdl  hdlSrcStrBuf,
    IN  VPP_UINT32    ulDstIndex,
    IN  VPP_UINT32    ulSrcIndex,
    IN  VPP_UINT32    ulLen
);

/******************************************************************************
 Function   : VppStrBufAppendFromStrBuf

 Description: Append data to the tail of StrBuf directly.

 Input      : hdlSrcStrBuf - The handle of the src StrBuf control block.
              ulSrcIndex   - The index of src StrBuf.
              ulLen     - The length of data which want to append, can not be 0

 Output     : hdlDstStrBuf - The handle of the dest StrBuf control block.

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32 VppStrBufAppendFromStrBuf
(
    IO  VppStrBufHdl  hdlDstStrBuf,
    IN  VppStrBufHdl  hdlSrcStrBuf,
    IN  VPP_UINT32    ulSrcIndex,
    IN  VPP_UINT32    ulLen
);

/******************************************************************************
 Function   : VppStrBufFillData

 Description: Fill data to index of Destination StrBuf from the continuous
              buffer.

 Input      : hdlDstStrBuf - The handle of the destination StrBuf
              ulDstIndex   - The start pointer of destination.
              pvData       - The data will insert into the StrBuf.
              ulLen        - The length of data which want to filled.

 Output     : hdlDstStrBuf - The handle of the destination StrBuf

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32    VppStrBufFillData
(
    IO  VppStrBufHdl  hdlDstStrBuf,
    IN  VPP_UINT32    ulDstIndex,
    IN  VPP_VOID     *pvData,
    IN  VPP_UINT32    ulLen
);

/******************************************************************************
 Function   : VppStrBufFillDataFromStrBuf

 Description: Fill data to index of Destination StrBuf from the source StrBuff.
              Current version can't support source StrBuf and DstBuf is the
              same buffer.

 Input      : hdlDstStrBuf - The handle of the destination StrBuf
              hdlSrcStrBuf - The handle of the source StrBuf
              ulDstIndex   - The start pointer of destination.
              ulSrcIndex   - The index of the source StrBuf
              ulLen        - The length of data which want to filled.

 Output     : hdlDstStrBuf - The handle of the destination StrBuf

 Return     : VPP_SUCCESS if success, others if failure.
******************************************************************************/
VPP_UINT32    VppStrBufFillDataFromStrBuf
(
    IO  VppStrBufHdl  hdlDstStrBuf,
    IN  VppStrBufHdl  hdlSrcStrBuf,
    IN  VPP_UINT32    ulDstIndex,
    IN  VPP_UINT32    ulSrcIndex,
    IN  VPP_UINT32    ulLen
);

/******************************************************************************

                              End of file

******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _VPP_STRBUF_X_ */

