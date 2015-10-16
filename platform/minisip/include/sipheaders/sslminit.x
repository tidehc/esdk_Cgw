/*******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName   : sslminit.x

    Author     : Srinivas

    Version    : 1.0

    Date       : 2005-09-09

    Description: This header file contains the API functions provided by the
                 stack for the stack users to maintain the stack.

    Function List:
        1. SipLmCoreLibInit
        2. SipLmCoreLibDeInit
        3. SipLmCoreLibGetMandConfig
        4. SipLmCoreLibGetOptConfig
        5. SipLmCoreLibResetOptConfig
        6. SipAllocGlobalVariable
        7. SipFreeGlobalVariable
        8. SipLmGetStackVersion

    History:
        1. Date         : <YYYY-MM-DD>
           Tag          :
           Author       :
           Modification :
*******************************************************************************/

#ifndef _SS_LM_INIT_X_
#define _SS_LM_INIT_X_

#include "ssdatatype.h"
#include "sssipbase.h"
#include "ssenums.h"
#include "ssfrmreg.x"

#ifdef __cplusplus
extern "C" {
#endif



/*
Description: usMaxObjs of SipCoreLibMandConfigS :
Value Range: usMaxObjs: 1 to 65535
Access: LayerManager & Transaction module
Remarks: None
*/

/*
Description: usMaxNumOfExtHdrs of SipCoreLibMandConfigS :
Value Range: usMaxNumOfExtHdrs: 0 to 128
Access: LayerManager & Codec module
Remarks: None
*/
#if 0
#define SIP_STACK_VERSION  "Vpp MiniSip V2.1.0.6"
#endif

/* The last version number is SIP Vpp MiniSip V2.1.0.6 before the version RAI V3.3.1.0 (MiniSIP) */
#define SIP_STACK_VERSION  "VPP V300R003C18T033 (MiniSIP) "

typedef struct _SipCoreLibMandConfigS
{
    SS_UINT16 usMaxObjs;     /* No of Transaction CBs max value is 65535 only*/
    SS_UINT16 usMaxNumOfExtHdrs;/*  The Maximum Number of Extended Headers the
                                  User wants to support max value is 255 only */
}SipCoreLibMandConfigS;

typedef struct _SipTxnOptConfigS
{
    SS_UINT32 ulSipTxnT1Val;        /* T1,RTT Estimate, default   500ms */
    SS_UINT32 ulSipTxnT2Val;        /* T2, The maximum retransmit interval for non-INVITE requests and INVITE responses, default  4000ms */
    SS_UINT32 ulSipTxnT4Val;        /* T4, Maximum duration a message will remain in the network, default  5000ms */
    SS_UINT32 ulSipTxnTDVal;        /* Timer D, Wait time for response retransmits, default 32000ms for UDP, 0 for TCP */
    SS_UINT32 ulSipTxnTURespVal;    /* Wait Time for Response from TU, default 64000ms */
    SS_UINT32 ulSipTxnPeerRespVal;  /* Wait Time for Response from Peer, default 64000ms */
    SS_UINT32 ulMtu;                /* max Data trasmitting Unit */
}SipTxnOptConfigS;

typedef struct _SipCoreLibOptConfigS
{
    SipTxnOptConfigS  *pstTxnOptCfg;  /* Txn Optional Configuration */
    SipCodecOptCfgS   *pstCodecCfg;  /* Codec Optional Configuration */
}SipCoreLibOptConfigS;

/*
Description: pstMandCfgPara mandatory for intialization in SipInitParaS :
Value Range: usMaxObjs: 1 to 65535 & usMaxNumOfExtHdrs: 0 to 128
Access: LayerManager , Codec module & Transaction module
Remarks: None
*/

/*
Description: pstOptCfgPara optional member for intialization in SipInitParaS :
Value Range: NA
Access: LayerManager , Codec module & Transaction module
Remarks: None
*/
typedef struct _SipInitParaS
{
    SipCoreLibMandConfigS  *pstMandCfgPara;/* CoreLib Mandatory Config Struct */
    SipCoreLibOptConfigS   *pstOptCfgPara; /* CoreLib Optional Config Struct */
} SipInitParaS;

/******************************************************************************
 Function    : SipLmCoreLibInit

 Description : This function will Initialize  the Core Library.

 Input       : None.

 Output      : pstCoreLibCfg - Pointer to Structure of the  config
                               paramters of the Core Library.

 Return      : SS_RET_SUCCESS or SS_RET_FAILURE
*******************************************************************************/
EN_SIP_RESULT SipLmCoreLibInit
(
    IN SipInitParaS *pstCoreLibCfg
);

/*******************************************************************************
 Function    : SipLmCoreLibDeInit

 Description : This function will De-Intialize the Core Library.

 Input       : None.

 Output      : None.

 Return      : SS_VOID
*******************************************************************************/
SS_VOID SipLmCoreLibDeInit
(
    SS_VOID
);

/*******************************************************************************
 Function    : SipLmCoreLibGetMandConfig

 Description : This function is to get the mandatory configuration
               of the Core Library.

 Input       : None.

 Output      : pstCoreLibMandCfg - Pointer to Structure of the mandatory config
                                   paramters of the Core Library.

 Return      : SS_RET_SUCCESS or SS_RET_FAILURE
*******************************************************************************/
EN_SIP_RESULT SipLmCoreLibGetMandConfig
(
    OUT SipCoreLibMandConfigS *pstCoreLibMandCfg
);


/*******************************************************************************
 Function    : SipLmCoreLibGetOptConfig

 Description : This function is to get the optional configuration of the
               Core Library.

 Input       : None.

 Output      : pstCoreLibOptCfg - Pointer to Structure of the optional config
                                   paramters of the Core Library.

 Return      : SUCCESS or FAILURE
*******************************************************************************/
EN_SIP_RESULT SipLmCoreLibGetOptConfig
(
    OUT SipCoreLibOptConfigS *pstCoreLibOptCfg
);
/*******************************************************************************
 Function    : SipLmCoreLibRecfgOptConfig

 Description : This function is to Reset the optional configuration of the
               Core Library.

 Input       : None.

 Output      : pstCoreLibOptCfg - Pointer to Structure of the optional config
                                  paramters of the Core Library.

 Return      : SUCCESS or FAILURE
*******************************************************************************/
EN_SIP_RESULT SipLmCoreLibRecfgOptConfig
(
    IN SipCoreLibOptConfigS *pstCoreLibOptCfg
);

#ifdef SS_NON_GLOBAL_VARIABLE
/*******************************************************************************
 Function    : SipAllocGlobalVariable

 Description : This function is to Allocate the Memory for a global variabel
               which has defined a pointer in the stack user global structre
               for symbian etc OSs to support No global variable.So the Uer who
               defined stack global varibale in stack user code has to invoke
               this API before Intializing the Stack.

 Input       : None.

 Output      : None.

 Return      : SS_VOID*
*******************************************************************************/

SS_VOID* SipAllocGlobalVariable
(
    SS_VOID
);

/*******************************************************************************
 Function    : SipFreeGlobalVariable

 Description : This function gets the Sip stack global data pointer by
               SipGlobalVariableGet and free it. Before call this function,
               The user shall invoke SipLmCoreLibDeInit at first.

 Input       : None.

 Output      : None.

 Return      : SS_VOID
*******************************************************************************/
SS_VOID SipFreeGlobalVariable
(
    SS_VOID
);

/*******************************************************************************
 Function    : SipGlobalVariableGet

 Description : This function is implemented by the stack user for symbian etc
               environments where the user will define a pointer for Stack
               global variable.The Stack will call this to get its global
               variable.

 Input       : None.

 Output      : None.

 Return      : SS_VOID*
*******************************************************************************/


SS_VOID *SipGlobalVariableGet
(
    SS_VOID
);

#endif
/******************************************************************************
 Function    : SipLmGetStackVersion

 Description : This API function will be invoked by Stack User
               to get the current stack version.

 Input(s)    : None.

 Output(s)   : None.

 Returns     : Null Terminated stack version string
******************************************************************************/
SS_CCHAR *SipLmGetStackVersion
(
    SS_VOID
);

/******************************************************************************
 Function    : SipLmTestGetStackVersion

 Description : This API function will be invoked by Test team for verifying the
               testing Stack release version.

 Input(s)    : None.

 Output(s)   : None.

 Returns     : Null Terminated stack version string
******************************************************************************/
SS_CCHAR *SipLmTestGetStackVersion
(
    SS_VOID
);

#ifdef __cplusplus
}
#endif

#endif

