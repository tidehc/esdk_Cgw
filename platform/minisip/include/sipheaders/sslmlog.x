/******************************************************************************
    Copyright (C), 1988-2006, Huawei Tech. Co., Ltd.

    FileName   : sslmlog.x

    Author     : Srinivas

    Version    : 1.0

    Date       : 2005-09-09

    Description: This header file contains the API functions to
                register the log functionality.

    Function List:
        1. SipLmRegLogHndlr

    History:
        1. Date         : <YYYY-MM-DD>
           Tag          :
           Author       :
           Modification :
******************************************************************************/

#ifndef _SS_LM_LOG_X_
#define _SS_LM_LOG_X_

#include "sslmcommon.h"
#include "sssipbase.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
// User has to Register Log function
    if (SS_RET_SUCCESS != SipLmRegLogHndlr (SipLmLogVosFileHandler) )
    {
        printf ("WARNING:: Failed To register Log Handler");
    }
Then the Implementation may be as follows:
SS_VOID SipLmLogVosFileHandler
(
    SS_UINT16          usComponentId,
    EN_SIP_LOG_LEVEL   enLogLevel,
    SS_UINT16          usFileId,
    SS_UINT16          usFunctionId,
    SS_UINT16          usLineNo,
    SS_UINT16          usLogStringId
)
{
    SS_CHAR *p = SS_NULL_PTR;
    SS_CHAR *sval = SS_NULL_PTR;
    SS_INT ival = SS_NULL_LONG;
    FILE *fp = SS_NULL_PTR;

    if ( (SS_FALSE == gstSipLogFilterInfo.abComponentEnabled[enComponent]) ||
        (SS_FALSE == gstSipLogFilterInfo.abLevelEnabled[enLogLevel]) )
    {
        return;
    }

    // open in append mode
    fp = fopen("SipLog.txt", "a+");
    if ( fp == SS_NULL_PTR )
    {
        return;
    }

 // Both Module and Log are enabled in filter, so now we can dump log on console
    switch (enLogLevel)
    {

        case    SIP_LOG_LEVEL_INFO:
            fprintf(fp, "\n\n SIP STACK INFORMATIONAL LOG ::");
            break;

        case    SIP_LOG_LEVEL_WARNING:
            fprintf(fp, "\n\n SIP STACK WARNING LOG ::");
            break;

        case    SIP_LOG_LEVEL_ERROR:
            fprintf(fp, "\n\n SIP STACK ERROR LOG ::");
            break;

        default:
            fclose(fp);
            return;
            break;
    }

    // Dump the basic Log information
    fprintf (fp, "Component = %u ", usComponentId) );
    fprintf (fp, "\n File = %u Function = %u Line = %u ", usFileId,
        usFunctionId, usLineNo);
    fprintf (fp, "\nLog Additional Info: %u  ", usLogStringId);

}
*/

/******************************************************************************
 Function Name : SipLmRegLogHndlr

 Description   : Registers the SMP Adaptor's Log handling function

 Input(s)      : pfnLogHandler - The log handler function of the SMP
                 Adaptor

 Output(s)     : None.

 Returns       : SS_RET_SUCCESS
******************************************************************************/
EN_SIP_RESULT SipLmRegLogHndlr
(
   IN  pFnSipLmLogHndlr pfnLogHandler
);

#ifdef __cplusplus
}
#endif

#endif

