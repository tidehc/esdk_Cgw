/******************************************************************************
    FileName: sigcomp.x

    Author:  Li Minghua(ID: 43584)

    Version: 2.1

    Date: 2005-12-05

    Description: This file provides the interface functions between the SIGCOMP
                 component and the SIP Stack User.

    Function List:
        1. SzipInit
        2. SzipDeInit
        3. SzipCfgMandParam
        4. SzipCfgOptParam
        5. SzipCfgAdvParam
        6. SzipGetCfgMandParam
        7. SzipGetCfgOptParam
        8. SzipGetCfgAdvParam
        9. SzipCompressMsg
        10.SzipDecompressMsg
        11.SzipDelimitCompMsg
        12.SzipAppendDelimiter
        13.SzipRemoveDelimiter
        14.SzipCreateCompartmentReq
        15.SzipDestroyCompartmentReq
        16.SzipFlushCompartmentReq
        17.SzipUpdateCompartment
        18.SzipLoadDictionaryReq
        19.SzipUnloadDictionaryReq
        20.SzipAlgCallback
        21.SzipRegCompressionAlg
        22.pFnSzipInterCheckCbck
        23.SzipRegInterCheckHndlr
        24.SzipInterCheckResponse

    History:
        1. Date        : <YYYY-MM-DD>
           Tag         : None
           Author      :
           Modification:
******************************************************************************/
#ifndef __SIGCOMP_X__
#define __SIGCOMP_X__

#include "sztypes.h"
#include "sslmcommon.h"

#ifdef __cplusplus
extern "C" {
#endif
/*****************************************************************************

                            Start Of File

******************************************************************************/
typedef enum _EN_SZIP_RESULT
{
    SZIP_SUCCESS = 0,
    SZIP_FAILURE,
    SZIP_INVALID_COMPARTMENTID,
    SZIP_INVALID_USDID,
    SZIP_UNSUPPORTED_ALGORITHM,
    SZIP_COMP_NORESOURCES,/*5*/
    SZIP_NO_FREE_COMPARTMENT_AVAILABLE,
    SZIP_NO_FREE_UDVM_AVAILABLE,
    SZIP_NO_STATE_ITEM_FOUND,
    SZIP_STATE_ITEM_ALREADY_EXISTS,
    SZIP_MULTIPLE_STATE_ITEMS_FOUND,/*10*/
    SZIP_INSUFFICIENT_STATE_MEMORY,
    SZIP_INTERNAL_ERROR,
    SZIP_INVALID_INPUT_PARAM,
    SZIP_MEM_ALLOC_FAILED,
    SZIP_INVALID_SIGCOMP_MESSAGE,/*15*/
    SZIP_DECOMPRESSION_FAILURE,
    SZIP_USD_OCCUPIED_BY_COMPARTMENT,
    SZIP_MAX_MSG_LENGTH_EXCEEDED,
    SZIP_USD_LEN_EXCEEDING_512_BYTES,
    SZIP_NOT_ENABLED,
    SZIP_MSG_NOT_COMPLETE,/* Identify the delimit function
                          haven't received a completed compressed message */
    SZIP_MSG_NOTMATCH_VERSION,/* For delimiter function to judge not matched
                          version message which includes 0XFF80 not quoted. */
    SZIP_UDVM_NO_MORE_DATA,/* The instruction requests data that lies beyond
                          the end of the SigComp message, no data is returned.*/

    SZIP_COMP_BUTT,
    SZIP_COMP_ENUM_END = SZIP_ENUM_END
}EN_SZIP_RESULT;

typedef enum _EN_SZIP_CPSR_ALGO
{
    SZIP_CPRS_ALGO_DEFAULT = 0,
    SZIP_CPRS_ALGO_LZSS,
    SZIP_CPRS_ALGO_DEFLATE,
    SZIP_CPRS_ALGO_UNUSED1,
    SZIP_CPRS_ALGO_UNUSED2,
    SZIP_CPRS_ALGO_UNUSED3,
    SZIP_CPRS_ALGO_UNUSED4,
    SZIP_CPRS_ALGO_CUSTOM1,
    SZIP_CPRS_ALGO_CUSTOM2,
    SZIP_CPRS_ALGO_CUSTOM3,
    SZIP_CPRS_ALGO_CUSTOM4,
    SZIP_CPRS_ALGO_CUSTOM5,
    SZIP_CPRS_ALGO_CUSTOM6,
    SZIP_CPRS_ALGO_CUSTOM7,
    SZIP_CPRS_ALGO_CUSTOM8,

    SZIP_CPRS_ALGO_BUTT,
    SZIP_CPRS_ALGO_ENUM_END = SZIP_ENUM_END
}EN_SZIP_CPSR_ALGO;

typedef enum _EN_SZIP_STATE_TYPE
{
    STATE_TYPE_UNACKED = 0,
    STATE_TYPE_ACKED,
    STATE_TYPE_SAVED,
    STATE_TYPE_SHARED,
    STATE_TYPE_LOCAL,
    STATE_TYPE_LOCAL_USD,
    STATE_TYPE_REMOTE_USD,

    STATE_TYPE_BUTT,
    STATE_TYPE_ENUM_END = SZIP_ENUM_END
}EN_SZIP_STATE_TYPE;

/***************** Error information enum for Log *********************/
typedef enum _EN_SZIP_LOG_ID
{
    /* Start from end of sip log information defined sslmcommon.h. */
    SZIP_LOG_STRING_INVALID_PARAMETERS = SIP_LOG_STRING_BUTT,
    SZIP_LOG_STRING_MEMORY_ALLOC_FAILURE,
    SZIP_LOG_STRING_STRING_BUFFER_ERROR,
    SZIP_LOG_LIST_INITIALISE_FAILURE,
    SZIP_LOG_NO_FREE_COMPARTMENTS,
    SZIP_LOG_NO_FREE_ALGORITHM_CB,
    SZIP_LOG_NO_FREE_USD_CB,
    SZIP_LOG_TIMER_INIT_FAILURE,
    SZIP_LOG_COMPRESSION_FAILURE,
    SZIP_LOG_SHA1_FAILURE,
    SZIP_LOG_HASH_OVERFLOW,
    SZIP_LOG_HASH_ENTRY_ALREADY_FREED,
    SZIP_LOG_GET_CB_FAILURE,
    SZIP_LOG_FLUSH_COMPARTMENT_FAILURE,
    SZIP_LOG_DESTROY_COMPARTMENT_FAILURE,
    SZIP_LOG_LOAD_USD_INTO_STATE_HANDLER_FAILED,
    SZIP_LOG_UNLOAD_USD_FROM_STATE_HANDLER_FAILED,
    SZIP_LOG_FREE_FROM_LIST_FAILED,
    SZIP_LOG_INTERCHECK_FAILED,
    SZIP_LOG_ADD_TO_HASH_FAILURE,
    SZIP_LOG_FREE_FROM_HASH_FAILURE,
    SZIP_LOG_FIND_IN_HASH_FAILURE,
    SZIP_LOG_MULTIPLE_STATES_FOUND,
    SZIP_LOG_LOCAL_STATE_DELETE_ATTEMPTED,
    SZIP_LOG_ENCODE_FAILURE,
    SZIP_LOG_UNKNOWN_OPCODE,
    SZIP_LOG_UNKNOWN_LITERAL_OPERAND,
    SZIP_LOG_UNKNOWN_REFERENCE_OPERAND,
    SZIP_LOG_UNKNOWN_MULTI_TYPE_OPERAND,
    SZIP_LOG_HANDLE_FEEDBACK_FAILURE,
    SZIP_LOG_SAVE_SHARED_STATE_FAILURE,
    SZIP_LOG_STATE_ACCESS_FAILURE,
    SZIP_LOG_OUT_OF_UDVM_RANGE,
    SZIP_LOG_EXCEEDED_MAX_FREE_REQUESTS,
    SZIP_LOG_INSUFFICIENT_STATE_MEMORY,
    SZIP_LOG_DELETE_STATE_FAILURE,
    SZIP_LOG_PARSE_SIGCOMP_MSG_FAILURE,
    SZIP_LOG_NO_FREE_UDVM,
    SZIP_LOG_UDVM_INIT_FAILURE,
    SZIP_LOG_MSG_DECOMPRESSION_FAILURE,
    SZIP_LOG_CREATE_STRING_FAILURE,
    SZIP_LOG_UPDATE_COMPARTMENT_FAILURE,
    SZIP_LOG_INVALID_DESTINATION_ADDRESS,
    SZIP_LOG_INVALID_SIGCOMP_HDR,
    SZIP_LOG_EXCEEDS_MESSAGE_LENGTH,
    SZIP_LOG_ADD_STATE_FAILURE,
    SZIP_LOG_FAILED_TO_FREE_STATE_MEMORY,
    SZIP_LOG_STATE_CONFLICT,
    SZIP_LOG_INTERNAL_ERROR,
    SZIP_LOG_TOO_SMALL_STATEID,
    SZIP_LOG_STATE_STILL_IN_USE,
    SZIP_LOG_INSUFFICIENT_COMPRESSOR_MEMORY,
    SZIP_LOG_INSUFFICIENT_UDVM_MEMORY_SIZE,
    SZIP_LOG_STATE_NOT_FOUND,
    SZIP_LOG_INVALID_COMPRESSOR_STATE,
    SZIP_LOG_COMPRESSION_ALGORITHM_NOT_REGISTERED,
    SZIP_LOG_FAILED_TO_UPDATE_BYTECODE_ID,
    SZIP_LOG_FAILED_TO_ADD_HEADERS,
    SZIP_LOG_FAILED_TO_GET_STATES_FOR_COMPRESSION,
    SZIP_LOG_FAILED_TO_ADD_BYTECODE,
    SZIP_LOG_DYNAMIC_STATE_SAVE_FAILED,
    SZIP_LOG_INVALID_EVENT,
    SZIP_LOG_FAILED_TO_HANDLE_EVENT,
    SZIP_LOG_FAILED_TO_CONFIRM_STATE,
    SZIP_LOG_SWITCHED_TO_DIFFERENT_ALGORITHM,
    SZIP_LOG_UDVM_CYCLE_COUNT_EXCEEDED,
    SZIP_LOG_INVALID_HASH_KEY,
    SZIP_LOG_FEATURE_NOT_ENABLED,
    SZIP_LOG_INPUT_BITS_LEN_INVALID,
    SZIP_LOG_INPUT_BIT_ORDER_INVALID,
    SZIP_LOG_INPUT_HUFFMAN_INVALID,

    SZIP_LOG_STRING_BUTT,
    SZIP_LOG_STRING_ENUM_END = SZIP_ENUM_END
}EN_SZIP_LOG_ID;

/***************** File Name enum for Log *********************/
typedef enum _EN_SZIP_FILE_NAME
{
    /* For Sigcomp file name enums, which following Sip file name defined
    in sslmcommon.h */
    SZ_CD = EN_SIP_FILE_BUTT,
    SZ_CPSR,
    SZ_DD,
    SZ_DELIMITE,
    SZ_ENC,
    SZ_LIB,
    SZ_LM,
    SZ_RSRC,
    SZ_SRCH,
    SZ_STMG,
    SZ_UDVM,

    EN_SZIP_FILE_BUTT,
    EN_SZIP_FILE_END = SZIP_ENUM_END
}EN_SZIP_FILE_NAME;

typedef struct _SzipStateInfoS
{
    SZIP_UINT8        *pucState;       /* Pointer to State value */
    SZIP_UINT32        ulLength;      /* Length of State Item */
    EN_SZIP_STATE_TYPE enType;        /* Type of State */
}SzipStateInfoS;

typedef struct _SzipStateInfoSetS
{
    SZIP_UINT32      ulCount;         /* Number of States Available */
    SzipStateInfoS **ppstStateInfo;/* Array of  pointers to State Information */
}SzipStateInfoSetS;

typedef struct _SzipCompartmentS
{
    SZIP_UINT32 ulCompartmentId;
}SzipCompartmentS;

typedef struct _SzipCfgParamS
{
    SZIP_UINT32 ulMaxNumofComp;
    SZIP_UINT32 ulMaxNumofUdvms;
    SZIP_UINT32 ulDfltCprsAlgo;
    SZIP_UINT32 ulStateMemSize;
    SZIP_UINT32 ulDecompressorMemSize;
    SZIP_UINT32 ulCyclesPerBit;
    SZIP_UINT32 ulMinStateLength;
    SZIP_UINT32 ulUdvmTmrVal;
    SZIP_UINT32 ulShrdCompEnable;
    SZIP_UINT32 ulInterCheckEnable;
    SZIP_UINT32 ulInterCheckTmrVal;
    SZIP_UINT32 ulCompressorMemSize;
}SzipCfgParamS;

typedef struct _SzipCfgMandParamS
{
    SZIP_UINT32 ulMaxNumofComp;
    SZIP_UINT32 ulMaxNumofUdvms;
}SzipCfgMandParamS;

typedef struct _SzipCfgOptParamS
{
    SZIP_UINT32 ulDfltCprsAlgo;
    SZIP_UINT32 ulShrdCompEnable;
}SzipCfgOptParamS;

typedef struct _SzipCfgAdvParamS
{
    SZIP_UINT32 ulStateMemSize;
    SZIP_UINT32 ulDecompressorMemSize;
    SZIP_UINT32 ulCyclesPerBit;
    SZIP_UINT32 ulMinStateLength;
    SZIP_UINT32 ulUdvmTmrVal;
    SZIP_UINT32 ulInterCheckEnable;
    SZIP_UINT32 ulInterCheckTmrVal;
    SZIP_UINT32 ulCompressorMemSize;
}SzipCfgAdvParamS;

typedef struct _SzipInitParaS
{
    SzipCfgMandParamS *pstMandCfg;
    SzipCfgOptParamS  *pstOptCfg;
    SzipCfgAdvParamS  *pstAdvCfg;
}SzipInitParaS;

typedef struct _SzipAuxDataS
{
    SZIP_BOOL  bAddTrailingEOF;
}SzipAuxDataS;

/******************************************************************************
 Function   : SzipInit

 Description: Initialization for whole Sigcomp stack.

 Input      : pstPara - Mandatory/optional/Advanced Config Paramters to be set.

 Output     : None

 Return     : SZIP_SUCCESS or any other return Code
******************************************************************************/
EN_SZIP_RESULT SzipInit
(
    IN SzipInitParaS  *pstPara
);

/******************************************************************************
 Function   : SzipDeInit

 Description: De- Initialization for Sigcomp

 Input      : None

 Output     : None

 Return     : None
******************************************************************************/
SZIP_VOID SzipDeInit
(
    SZIP_VOID
);

#ifdef SZIP_NON_GLOBAL_VARIABLE
/*******************************************************************************
 Function    : SzipAllocGlobalVariable

 Description : This function is to Allocate the Memory for a global variable
               which has defined a pointer in the stack user global structure
               for Symbian or Brew etc OSs to support no global variable.
               So the User who defined stack global variable in stack user code
               has to invoke this API before initializing the Stack.

 Input       : None.

 Output      : pstMem - Pointer to pointer type of void.

 Return      : SZIP_NULL_PTR if failure, the allocated memory pointer if success.
*******************************************************************************/
SZIP_VOID *SzipAllocGlobalVariable
(
    SZIP_VOID
);

/*******************************************************************************
 Function    : SzipFreeGlobalVariable

 Description : This function will free the memory allocated by
               SzipAllocGlobalVariable. The component user shall assure invoke
               SzipDeInit before invoking this function.

 Input       : None.

 Output      : SZIP_VOID

 Return      : SZIP_VOID
*******************************************************************************/
SZIP_VOID SzipFreeGlobalVariable
(
    SZIP_VOID
);

/*******************************************************************************
 Function    : SzipGlobalVariableGet

 Description : This function will implement by user. Sigcomp component will get
               the global variable through this function.

 Input       : None.

 Output      : SZIP_VOID

 Return      : SZIP_VOID
*******************************************************************************/
SZIP_VOID *SzipGlobalVariableGet
(
    SZIP_VOID
);

#endif  /*SZIP_NON_GLOBAL_VARIABLE */

/******************************************************************************
 Function   : SzipCfgOptParam

 Description: Configure the optional parameters of SigComp.

 Input      : pstCfgOptParam : Optional Configuration parameters

 Output     : None

 Return     : SZIP_SUCCESS or any other return code

 Others     : None
******************************************************************************/
EN_SZIP_RESULT SzipCfgOptParam
(
    IN SzipCfgOptParamS *pstCfgOptParam
);


/******************************************************************************
 Function   : SzipCfgAdvParam

 Description: Configure the advanced parameters of SigComp

 Input      : pstCfgAdvParam : Advanced Configuration parameters

 Output     : None

 Return     : SZIP_SUCCESS or any other return code

 Others     : None
******************************************************************************/
EN_SZIP_RESULT  SzipCfgAdvParam
(
    IN SzipCfgAdvParamS *pstCfgAdvParam
);

/******************************************************************************
 Function   : SzipGetCfgMandParam

 Description: To query mandatory configuration of compression component.

 Input      : None

 Output     : pstCfgMandParam : SigComp mandatory Configuration Parameters

 Return     : SZIP_SUCCESS or any other return Code
******************************************************************************/
EN_SZIP_RESULT  SzipGetCfgMandParam
(
    OUT SzipCfgMandParamS *pstCfgMandParam
);

/******************************************************************************
 Function   : SzipGetCfgOptParam

 Description: To query optional configuration of compression component.

 Input      : None

 Output     : pstCfgOptParam : SigComp optional Configuration Parameters

 Return     : SZIP_SUCCESS or any other return Code
******************************************************************************/
EN_SZIP_RESULT  SzipGetCfgOptParam
(
    OUT SzipCfgOptParamS *pstCfgOptParam
);

/******************************************************************************
 Function   : SzipGetCfgAdvParam

 Description: To query Advanced configuration of compression component.

 Input      : None

 Output     : pstCfgAdvParam : SigComp advanced Configuration Parameters

 Return     : SZIP_SUCCESS or any other return Code
******************************************************************************/
EN_SZIP_RESULT  SzipGetCfgAdvParam
(
    OUT SzipCfgAdvParamS *pstCfgAdvParam
);

/******************************************************************************
 Function   : SzipCompressMsg

 Description: This API is invoked by SIP stack to compress the SIP message using
              a particular compartment.The API accepts the application message
              as input, compresses the message using the states available for
              the compartment and returns the SigComp message back to application.
              Note: Memory for the compressed message is allocated by SigComp
              using the function provided in Memory control pointer. This memory
              should be freed by the SIP stack.
              The maximum allowed length of the input message is 4096 Bytes.

 Input      : hdlMemCp - [IN] Memory control handle.
              pstMsg   - [IN] Pointer to the message to be compressed.
              ulCompartmentId - [IN] ID of the compartment to be used.

 Output     : phdlCompMsg     - [OUT]Pointer Compressed message.

 Returns    : SZIP_SUCCESS or any other return Code
******************************************************************************/
EN_SZIP_RESULT SzipCompressMsg
(
    IN  SzipMemCpHdl   hdlMemCp,
    IN  SzipString    *pstMsg,
    IN  SZIP_UINT32    ulCompartmentId,
    OUT SzipStrBufHdl *phdlCompMsg
);

/******************************************************************************
 Function   : SzipDecompressMsg

 Description: Handle requests from application to decompress a message.
              On receiving a SigComp message, it processes the SigComp header
              to check whether the Bytecode is loaded as part of SigComp message
              or the only the state identifier is available.
              If only the partial identifier is available, it queries the state
              handler and uploads the state item to UDVM.

 Input      : hdlMemCp   - [IN] Memcp from whcih the buffer for decompressed
                                message should be created.
              pstCompMsg - [IN] Pointer to the message to be compressed.
              pstAuxdata - [IN] To extend the API defintion. Currently it
                                contains only one flag, bAddTrailingEOF to tell
                                if two \0 has to be added at the end of
                                decompressed message.(Required for current SIP
                                parser implementation.)

 Output     : phdlDeCompMsg   - [OUT]Pointer Compressed message.
              pulRefId        - [OUT]Reference Identifier

 Return     : SZIP_SUCCESS or any other error code

 Others     : None
******************************************************************************/
EN_SZIP_RESULT  SzipDecompressMsg
(
    IN  SzipMemCpHdl   hdlMemCp,
    IN  SzipString    *pstCompMsg,
    IN  SzipAuxDataS  *pstAuxdata,
    OUT SzipStrBufHdl *phdlDeCompMsg,
    OUT SZIP_UINT32   *pulRefId
);

/******************************************************************************
 Function   : SzipDelimitCompMsg

 Description: This is the interface function to the TptD module. It is used for
              delimit whether a compressed message be completely received or not.

 Input      : hdlCompMsg: Compressed message received from the network.

 Output     : pulLength: The length of message has been received.

 Return     : SZIP_SUCCESS or any other error code
******************************************************************************/
EN_SZIP_RESULT  SzipDelimitCompMsg
(
    IN  SzipStrBufHdl hdlCompMsg,
    OUT SZIP_UINT32  *pulLength
);

/******************************************************************************
 Function   : SzipAppendDelimiter

 Description: This is the interface function to the TptD module. Append the
              "Record Marking" to compressed message before send it to network.

 Input      : hdlCompMsg: Compressed message.

 Output     : hdlCompMsg: Compressed message after append delimiter.


 Return     : SZIP_SUCCESS or any other error code
******************************************************************************/
EN_SZIP_RESULT  SzipAppendDelimiter
(
    IO  SzipStrBufHdl hdlCompMsg
);

/******************************************************************************
 Function   : SzipRemoveDelimiter

 Description: This is the interface function to the TptD module. Remove the
              "Record Marking" which used for delimit the SigComp message before
              send it to decompress.

 Input      : hdlCompMsg: Compressed message received from the network.

 Output     : hdlCompMsg: Compressed message that after remove the delimiter.

 Return     : SZIP_SUCCESS or any other error code
******************************************************************************/
EN_SZIP_RESULT  SzipRemoveDelimiter
(
    IO  SzipStrBufHdl hdlCompMsg
);

/******************************************************************************
 Function   : SzipCreateCompartmentReq

 Description: Handle Requests from application to create a compartment
              If the number of existing compartments exceed the configuration
              value, the request is rejected. A free compartment is made
              available and resources allocated for compartment. The compartment
              is initialized and a compartment id is returned to application.

 Input      : enCmprsnAlgo : Compression Algorithm to be used
              ulStateMemSize : State Memory Size
              ulUsdId : Unique ID for the User Specific Dictionary

 Output     : pulCompartmentId : Compartment Identifier

 Return     : SZIP_SUCCESS or any other error code

 Others     : None
******************************************************************************/
EN_SZIP_RESULT SzipCreateCompartmentReq
(
    IN EN_SZIP_CPSR_ALGO enCmprsnAlgo,
    IN SZIP_UINT32     ulStateMemSize,
    IN SZIP_UINT32     ulUsdId,
    OUT SZIP_UINT32   *pulCompartmentId
);

/******************************************************************************
 Function   : SzipDestroyCompartmentReq

 Description: Handle requests from application to destroy a compartment.
              Any request to delete a compartment whose compartment id is zero
              is rejected. The resources allocated for the compartment are freed.
              The compartment id is made available for future use.

 Input      : ulCompartmentId : Compartment Identifier

 Output     : None

 Return     : SZIP_SUCCESS or other return code

 Others     : None
******************************************************************************/
EN_SZIP_RESULT SzipDestroyCompartmentReq
(
    IN SZIP_UINT32 ulCompartmentId
);

/******************************************************************************
 Function   : SzipFlushCompartmentReq

 Description: Handle Requests from application to flush a compartment.
              The states stored in the compressor memory and the shared state
              stored in the decompressor memory are flushed out. However the
              resources allocated to the compartment are not freed.It can be
              invoked by the application as an alternative to a sequence of
              destroyCompartment and createCompartment.

 Input      : ulCompartmentId : Compartment Identifier

 Output     : None

 Return     : SZIP_SUCCESS or other return code

 Others     : None
******************************************************************************/
EN_SZIP_RESULT SzipFlushCompartmentReq
(
    IN SZIP_UINT32 ulCompartmentId
);

/******************************************************************************
 Function   : SzipUpdateCompartment

 Description: Receive confirmation from application after decompressing the
              message. All STATE CREATE and STATE free operations will be
              executed.

 Input      : ulRefId : Reference id
              ulCompartmentId : Compartment Id of the decompressed message

 Output     : None

 Return     : SZIP_SUCCESS or any other return code
******************************************************************************/
EN_SZIP_RESULT SzipUpdateCompartment
(
    IN SZIP_UINT32 ulRefId,
    IN SZIP_UINT32 ulCompartmentId
);

/******************************************************************************
 Function   : SzipLoadDictionaryReq

 Description: Handle requests from application to load a dictionary.
              The dictionary is stored in the SigComp as a state. It is
              associated with a identifier which is returned to the application.
              This will be done by the application..

 Input      : pstDict : User Specific Dictionary to be loaded

 Output     : pulUsdId : Unique Identifier for the USD.

 Return     : SZIP_SUCCESS or other return code

 Others     : None
******************************************************************************/
EN_SZIP_RESULT SzipLoadDictionaryReq
(
    IN  SzipString  *pstDict,
    OUT SZIP_UINT32 *pulUsdId
);

/******************************************************************************
 Function   : SzipUnloadDictionaryReq

 Description: Handle Request from application to unload a given USD.
              The dictionary is deleted from the Sigcomp provided no compartment
              is using it.

 Input      : ulUsdId : USD Identifier

 Output     : None

 Return     : SZIP_SUCCESS or other return code

 Others     : None
******************************************************************************/
EN_SZIP_RESULT SzipUnloadDictionaryReq
(
    IN SZIP_UINT32 ulUsdId
);

/******************************************************************************
 Function   : SzipInterCheckResponse

 Description: Intercheck response sent by the user.

 Input      : ulCount : Number of compartments for which intercheck has failed.
              pstCompIdListAck :  List of compartments for which intercheck
                                  has failed.

 Output     : None

 Return     : SZIP_SUCCESS or other return code.

 Others     : None
******************************************************************************/
EN_SZIP_RESULT SzipInterCheckResponse
(
    IN SZIP_UINT32       ulCount,
    IN SzipCompartmentS *pstCompIdListAck
);

/******************************************************************************
 Function   : pFnSzipAlgCallback

 Description: Callback function definition for user defined compression algorithm

 Input      : pstMsg : Uncompressed message
              pstStateInfo : The set of state information

 Output     : phdlCompMsg : Compressed message

 Return     : SZIP_SUCCESS or other return code
******************************************************************************/
typedef EN_SZIP_RESULT (*pFnSzipAlgCallback)
(
    IN  SzipString        *pstMsg,
    IN  SzipStateInfoSetS *pstStateInfo,
    OUT SzipStrBufHdl     *phdlComMsg
);

/******************************************************************************
 Function   : pFnSzipInterCheckCbck

 Description: Call back function defintion for Intercheck

 Input      : ulCount : Number of compartments to be interchecked.

 Output     : pstInterCheckReqList :  List of compartment Ids to be interchecked.

 Return     : SZIP_SUCCESS or other return code.
******************************************************************************/
typedef SZIP_VOID (*pFnSzipInterCheckCbck)
(
    IN  SZIP_UINT32       ulCount,
    OUT SzipCompartmentS *pstInterCheckReqList
);

/******************************************************************************
 Function   : SzipRegCompressionAlg

 Description: Handle requests from application to register a compression
              algorithm.

 Input      : pfnSzipAlgCallBack : Call back function to compress a message.

 Output     : pulCompAlgId : Id of the registered compression algorithm.

 Return     : SZIP_SUCCESS or other return code.
******************************************************************************/
EN_SZIP_RESULT SzipRegCompressionAlg
(
    IN  pFnSzipAlgCallback pfnSzipAlgCallBack,
    OUT SZIP_UINT32       *pulCompAlgId
);

/******************************************************************************
 Function   : SzipRegInterCheckHndlr

 Description: Register call back function for intercheck requests.

 Input      : pfnInterCheckHndlr : Pointer to call back function for sending
                                  Intercheck Requests

 Output     : None

 Return     : SZIP_SUCCESS or other return code.
******************************************************************************/
EN_SZIP_RESULT SzipRegInterCheckHndlr
(
    IN pFnSzipInterCheckCbck pfnInterCheckRqst
);

/******************************************************************************
 Function   : SipSigCompTimeoutHandler

 Description: This is a interface of the timer that will be invoked by the user
              on time out.

 Input(s)   : ulTimerID  - Timer ID
              ulParams  - Gives of the Sigcomp block & Timer Name & Module ID.

 Output     : None.

 Return     : SZIP_VOID
 ******************************************************************************/
SZIP_VOID SipSigCompTimeoutHandler
(
    IN SZIP_UINT32 ulParams,
    IN SZIP_UINT32 ulTimerId
);

/******************************************************************************

                              End Of File

******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif/* not defined _SIGCOMP_X_ */

