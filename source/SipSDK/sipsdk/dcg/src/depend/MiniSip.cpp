/**
* @file  MiniSip.cpp
* @brief MiniSip相关函数的实现
*
* Copyright (c) 2010
* Huawei Tech.Co.,Ltd
*
* @author   Li GengQiang/l00124479
* @date     2010/12/06
* @version  NVS V100R002C01
*
*/

#include "MiniSip.h"
#include "randutil.h"
#include "digcalc.h"

#include "CSipStack.hpp"
#include <sstream>
#include <cstdio>
#include <iostream>
#include <ace/OS_NS_time.h>
#include <ace_header.h>
#include "sip_namespace.h"
#include "Log.h"

//lint -e438
//lint -e818

#ifndef FALSE
#define FALSE false
#endif
//namespace IvsCbb{ namespace SipStack{  
//SIP的UDP的Socket
unsigned long g_ulSipUdpSocket = 0;


EN_SIP_RESULT SipStartTimer
(
    IN  SS_UINT32      ulTimerLength,
    IN  SS_UINT32      ulTimerParam,
    OUT SS_UINT32     *pulTimerId
)
{
    if (VOS_NULL == SipStack::g_pSipStack)
    {
        return SIP_RET_FAILURE;
    }

    //生成事件信息，处理事件线程中释放该内存
    SipStack::SIP::EVENT_INFO_TIMER_REGISTER* pstTimerInfo = VOS_NEW(pstTimerInfo);
    if (VOS_NULL == pstTimerInfo)
    {
        return SIP_RET_FAILURE;
    }

    pstTimerInfo->ulTimerId         = SipStack::g_pSipStack->GetTimerID();    //定时器ID
    pstTimerInfo->ulTimerLength     = ulTimerLength;                //定时器时长
    pstTimerInfo->ulTimerPara       = ulTimerParam;                 //定时器参数
    pstTimerInfo->ulStartTimeTick   = SipStack::VOS_GetTicks();               //开始时间戳
    //BEGIN R002C01SPC100 ADD 2011-08-15 ligengqiang 00124479 for 超时后SipTxnTimeoutHandler里崩溃问题，防止重复停止定时器
    pstTimerInfo->bStop             = false;                        //是否已停止
    //END   R002C01SPC100 ADD 2011-08-15 ligengqiang 00124479 for 超时后SipTxnTimeoutHandler里崩溃问题，防止重复停止定时器

    //设置定时器ID
    *pulTimerId = pstTimerInfo->ulTimerId;

    SipStack::SIP::EVENT_INFO stEventInfo(SipStack::SIP::EVENT_TYPE_STACK_TIMER_REGISTER, pstTimerInfo, sizeof(*pstTimerInfo));
    //添加定时器注册事件
    long lResult = SipStack::g_pSipStack->AddEvent(stEventInfo);
    if (SipStack::SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("AddEvent failed,timer_id=%d,timer_param=%d,timer_length=%d",pstTimerInfo->ulTimerId,ulTimerParam,ulTimerLength);
        VOS_DELETE(pstTimerInfo);
        pstTimerInfo = NULL;
        return SIP_RET_FAILURE;
    }

    return SIP_RET_SUCCESS;
}

SS_VOID SipStopTimer
(
    IN SS_UINT32  ulTimerId
)
{
    if (VOS_NULL == SipStack::g_pSipStack)
    {
        ERROR_LOG("stop timer - invalid param.");
        return ;
    }

    //生成事件信息，处理事件线程中释放该内存
    SipStack::SIP::EVENT_INFO_TIMER_CANCEL* pstEventInfo = VOS_NEW(pstEventInfo);
    if (VOS_NULL == pstEventInfo)
    {
        ERROR_LOG("stop timer - alloc event.");
        return ;
    }

    pstEventInfo->ulTimerId = ulTimerId;  //定时器ID

    SipStack::SIP::EVENT_INFO stEventInfo(SipStack::SIP::EVENT_TYPE_STACK_TIMER_CANCEL, pstEventInfo, sizeof(*pstEventInfo));
    //添加定时器注册事件
    long lResult = SipStack::g_pSipStack->AddEvent(stEventInfo);
    if (SipStack::SIP::RET_CODE_OK != lResult)
    {
        VOS_DELETE(pstEventInfo);
        ERROR_LOG("AddEvent failed,timer_id=%d", ulTimerId);
        return ;
    }

    //INFO_LOG("stop timer - success - timer_id=%d.", ulTimerId);
    return ;
}

SS_VOID SipTxnHiSfReqTimeout
(
 IN SS_UINT32         ulTuObjId,
 IN SS_UINT32         ulTxnObjId,
 IN SipDataUnitS     *pstSipSdu,
 IN SipTxnHiAuxDataS *pstAuxData
)
{
    DEBUG_LOG("SipTxnHiSfReqTimeout:mini sip call me.");
    ulTuObjId   = ulTuObjId;
    ulTxnObjId  = ulTxnObjId;
    pstSipSdu   = pstSipSdu;
    pstAuxData  = pstAuxData;
}

EN_SIP_RESULT SipTxnHiSfReqInd
(
 IN  SS_UINT32         ulTxnObjId,
 IN  SipDataUnitS     *pstSipSdu,
 IN  SipTptNwInfoS    *pstTptNwInfo,
 IN  SipTxnHiAuxDataS *pstAuxData,
 OUT SS_UINT32        *pulTuObjId
)
{
    DEBUG_LOG("SipTxnHiSfReqInd:mini sip call me.");

    //下述参数不需处理
    pstAuxData   = pstAuxData;
    pstTptNwInfo = pstTptNwInfo;
    //下述参数不会修改
    pstSipSdu    = pstSipSdu;

    //参数检查
    if ( VOS_NULL == pstSipSdu
      || VOS_NULL == pstSipSdu->pstSipMsg
      || VOS_NULL == pstTptNwInfo
      || VOS_NULL == SipStack::g_pSipStack
       )
    {
        DEBUG_LOG("SipTxnHiSfReqInd:invalid args");
        return SIP_RET_FAILURE;
    }

    //设置TU对象ID
    SS_UINT32 ulTuObjId = SipStack::g_pSipStack->GetTuObjId();
    *pulTuObjId = ulTuObjId;

    //处理有状态请求消息
    SipStack::g_pSipStack->HandleSipTxnHiSfReqInd(ulTuObjId, ulTxnObjId, *pstTptNwInfo, *pstSipSdu->pstSipMsg);

    return SIP_RET_SUCCESS;
}

SS_VOID SipTxnHiSfRspInd
(
    IN SS_UINT32         ulTuObjId,
    IN SS_UINT32         ulTxnObjId,
    IN SipDataUnitS     *pstSipSdu,
    IN SipTptNwInfoS    *pstTptNwInfo,
    IN SipTxnHiAuxDataS *pstAuxData
)
{
    DEBUG_LOG("SipTxnHiSfRspInd:minisip call me.");

    pstSipSdu       = pstSipSdu;
    pstTptNwInfo    = pstTptNwInfo;
    pstAuxData      = pstAuxData;

    if ( VOS_NULL == pstSipSdu
      || VOS_NULL == pstSipSdu->pstSipMsg
      || VOS_NULL == pstTptNwInfo
      || VOS_NULL == SipStack::g_pSipStack
       )
    {
        ERROR_LOG("SipTxnHiSfReqInd:invalid args");
        return ;
    }

    SipStack::g_pSipStack->HandleSipTxnHiSfRspInd(ulTuObjId, ulTxnObjId, *pstTptNwInfo, *pstSipSdu->pstSipMsg);

    INFO_LOG("SipTxnHiSfRspInd Success."); 
}

SS_VOID SipTxnHiSfMatchedCancelInd
(
 IN SS_UINT32         ulCanceledTuObjId,
 IN SS_UINT32         ulCanceledTxnObjId,
 IN SipDataUnitS     *pstSipSdu,
 IN SipTptNwInfoS    *pstTptNwInfo,
 IN SipTxnHiAuxDataS *pstAuxData
 )
{
    DEBUG_LOG("SipTxnHiSfMatchedCancelInd:minisip call me.");
    ulCanceledTuObjId = ulCanceledTuObjId;
    ulCanceledTxnObjId  = ulCanceledTxnObjId;
    pstSipSdu           = pstSipSdu;
    pstTptNwInfo        = pstTptNwInfo;
    pstAuxData          = pstAuxData;
}

EN_SIP_RESULT SipTptLiStrSend
(
 IN SS_UINT32         ulMsgUsrId,
 IN SS_UINT32         ulMsgUsrTimeStamp,
 IN SipString        *pstStrToTptD,
 IN SS_UINT32         ulMsgFlag,
 IN SipTptNwInfoS    *pstTptNwInfo,
 IN SS_INT            iTmpConPort,
 IN SipVia           *pstViaHdr,
 IN SipTptLiAuxDataS *pstAuxData
 )
{
    DEBUG_LOG("SipTptLiStrSend:minisip call me.");

    //下列参数不需处理，或者不会修改
    ulMsgUsrId          = ulMsgUsrId;
    ulMsgUsrTimeStamp   = ulMsgUsrTimeStamp;
    pstStrToTptD        = pstStrToTptD;
    ulMsgFlag           = ulMsgFlag;
    pstTptNwInfo        = pstTptNwInfo;
    iTmpConPort         = iTmpConPort;
    pstViaHdr           = pstViaHdr;
    pstAuxData          = pstAuxData;

    //打印日志前后，替换最后一个字符为结束符
    char& cEnd = pstStrToTptD->pcData[pstStrToTptD->ulLen - 1];
    const char cEndValue = cEnd;
    cEnd = '\0';
    DEBUG_LOG("ready to send sip message......dst_addr=%d.%d.%d.%d:%d,length=%d,data=\n%s",
		(unsigned int)pstTptNwInfo->stDstAddr.u.aucIPv4Addr[0], 
		(unsigned int)pstTptNwInfo->stDstAddr.u.aucIPv4Addr[1],
		(unsigned int)pstTptNwInfo->stDstAddr.u.aucIPv4Addr[2],
		(unsigned int)pstTptNwInfo->stDstAddr.u.aucIPv4Addr[3],
		(unsigned int)pstTptNwInfo->stDstAddr.iPort,
		pstStrToTptD->ulLen,
		pstStrToTptD->pcData);
    cEnd = cEndValue;//lint !e838

    if (0 == g_ulSipUdpSocket)
    {
        DEBUG_LOG("failure to send sip message......invalid socket.");
        return SIP_RET_FAILURE;
    }

    sockaddr_in stAddr = {0};
    stAddr.sin_family = AF_INET;
    SS_UINT8* pIP = (SS_UINT8*)&stAddr.sin_addr.s_addr;
    pIP[0] = pstTptNwInfo->stDstAddr.u.aucIPv4Addr[0];
    pIP[1] = pstTptNwInfo->stDstAddr.u.aucIPv4Addr[1];
    pIP[2] = pstTptNwInfo->stDstAddr.u.aucIPv4Addr[2];
    pIP[3] = pstTptNwInfo->stDstAddr.u.aucIPv4Addr[3];
    
   // stAddr.sin_addr.s_addr = inet_addr(pIP);
    stAddr.sin_port = htons((unsigned short)pstTptNwInfo->stDstAddr.iPort);

    // cout<<"send to addr: "<<pIP[0]<<"."<<pIP[1]<<"."<<pIP[2]<<"."<<pIP[3]<<":"<<(unsigned short)pstTptNwInfo->stDstAddr.iPort<<endl;

    int iResult = sendto(g_ulSipUdpSocket, pstStrToTptD->pcData, (int)pstStrToTptD->ulLen, 0, (sockaddr*)&stAddr, sizeof(sockaddr));
    if (SOCKET_ERROR == iResult)
    {
        DEBUG_LOG("failure to send sip message......send_result=%d",CONN_ERRNO); 
        return SIP_RET_FAILURE;
    }

    DEBUG_LOG("success to send sip message......send_size=%d",iResult); 
    return SIP_RET_SUCCESS;
}

EN_SIP_RESULT SipTxnHiSlRspInd
(
 IN SipDataUnitS     *pstSipSdu,
 IN SipTptNwInfoS    *pstTptNwInfo,
 IN SipTxnHiAuxDataS *pstAuxData
 )
{
    DEBUG_LOG("SipTxnHiSlRspInd:minisip call me.");
    pstSipSdu       = pstSipSdu;
    pstTptNwInfo    = pstTptNwInfo;
    pstAuxData      = pstAuxData;
    return EN_SIP_RESULT(0);
}

EN_SIP_RESULT SipTxnHiSlReqInd
(
 IN SipDataUnitS     *pstSipSdu,
 IN SipTptNwInfoS    *pstTptNwInfo,
 IN SipTxnHiAuxDataS *pstAuxData
 )
{
    DEBUG_LOG("SipTxnHiSlReqInd:minisip call me.");
    pstSipSdu       = pstSipSdu;
    pstTptNwInfo    = pstTptNwInfo;
    pstAuxData      = pstAuxData;

    if ( VOS_NULL == pstSipSdu
      || VOS_NULL == pstSipSdu->pstSipMsg
      || VOS_NULL == pstTptNwInfo
      || VOS_NULL == SipStack::g_pSipStack
       )
    {
        ERROR_LOG("SipTxnHiSfReqInd:invalid args");
        return SIP_RET_FAILURE;
    }

    //处理无状态请求消息
    SipStack::g_pSipStack->HandleSipTxnHiSlReqInd(*pstTptNwInfo, *pstSipSdu->pstSipMsg);

    INFO_LOG("SipTxnHiSlReqInd Success.");
    return SIP_RET_SUCCESS;
}

SS_VOID SipLmLogCumwHandler
(
    SS_UINT16          usComponentId,
    EN_SIP_LOG_LEVEL   enLogLevel,
    SS_UINT16          usFileId,
    SS_UINT16          usFunctionId,
    SS_UINT16          usLineNo,
    SS_UINT16          usLogStringId
)
{
    //INFO_LOG("SipLmLogCumwHandler In. ComponentID = %d. LogLevel = %d. FileID = %d. FunctionID = %d. LineNo = %d."
        //"LogStringID = %d.", usComponentId, enLogLevel, usFileId, usFunctionId, usLineNo, usLogStringId);
    //DEBUG_LOG("SipLmLogCumwHandler:ID=file" << usFileId << ".func" << usFunctionId << ".line" << usLineNo << ".str" << usLogStringId << ",thread_id="  << (unsigned long long)ACE_OS::thr_self();
}

/**
  EN_SIP_RESULT (*pFnSipTxnHiSlMsgSendResultInd)
  (
  IN SS_UINT32 ulTuObjId,
  IN SS_UINT32 ulTuObjTimeStamp,
  IN EN_SIP_SEND_RESULT enResultType,
  IN SipTptNwInfoS *pstTptNwInfo,
  IN SipTxnHiAuxDataS *pstAuxData
  )
  */
EN_SIP_RESULT SipTxnHiSlMsgSendResultInd
(
  IN SS_UINT32 ulTuObjId,
  IN SS_UINT32 ulTuObjTimeStamp,
  IN EN_SIP_SEND_RESULT enResultType,
  IN SipTptNwInfoS *pstTptNwInfo,
  IN SipTxnHiAuxDataS *pstAuxData
)
{
    DEBUG_LOG("SipTxnHiSlMsgSendResultInd:minisip call me.");
    return SIP_RET_SUCCESS;
}

/**
  EN_SIP_RESULT (*pFnSipTxnHiSfUnmatchedCancelInd)
  (
  IN SS_UINT32 ulTxnObjId,
  IN SipDataUnitS *pstSipSdu,
  IN SipTptNwInfoS *pstTptNwInfo,
  IN SipTxnHiAuxDataS *pstAuxData,
  IO SS_UINT32 *pulTuObjId
  )
  */
  EN_SIP_RESULT SipTxnHiSfUnmatchedCancelInd
  (
  IN SS_UINT32 ulTxnObjId,
  IN SipDataUnitS *pstSipSdu,
  IN SipTptNwInfoS *pstTptNwInfo,
  IN SipTxnHiAuxDataS *pstAuxData,
  IO SS_UINT32 *pulTuObjId
  )
{
    DEBUG_LOG("SipTxnHiSfUnmatchedCancelInd:minisip call me.");
    return SIP_RET_SUCCESS;
}

/**
* Description:  InitMiniSip().  初始化MiniSip
* @return       long.       返回码
*/
long SipStack::SIP::InitMiniSip()
{
    INFO_LOG("Init MiniSip Begin.");

    //初始化MiniSIP
    SipInitParaS stInitpara = {0};

    //必选参数
    SipCoreLibMandConfigS stMandCfg = {0};
    stMandCfg.usMaxObjs         = MAX_NUM_TRANSACTIONS;    //最大事务数
    stMandCfg.usMaxNumOfExtHdrs = EX_HDR_ID_MAX;           //外部SIP头域数量

    //可选参数
    SipCoreLibOptConfigS stOptCfg = {0};

    SipCodecOptCfgS stCodecOptCfg = {0};
    stCodecOptCfg.bEncInMultipleLines = SS_TRUE;    //多Via时，分拆为多个头域

    stOptCfg.pstCodecCfg = &stCodecOptCfg;

    stInitpara.pstMandCfgPara   = &stMandCfg;
    stInitpara.pstOptCfgPara    = &stOptCfg;
    
    //MiniSip函数返回值
    EN_SIP_RESULT enResult = SIP_RET_SUCCESS;

    //初始化
    enResult = SipLmCoreLibInit(&stInitpara);//lint !e838
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipLmCoreLibInit Failed On Init MiniSip. Error = %s.", STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

    //注册日志回调
    enResult = SipLmRegLogHndlr(SipLmLogCumwHandler);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipLmRegLogHndlr Failed On Init MiniSip. Error = %s.", STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

    // register tu callback functions
    SipTxnTuOptRegFnS sipTxnTuOptRegFns;
    sipTxnTuOptRegFns.pfnSfUnmatchedCancelInd = SipTxnHiSfUnmatchedCancelInd;
    sipTxnTuOptRegFns.pfnSlMsgSendResultInd = SipTxnHiSlMsgSendResultInd;
    if (SIP_RET_SUCCESS != SipTxnRegTxnTuOptIntf(&sipTxnTuOptRegFns))
    {
        //ERROR_LOG("SipTxnRegTxnTuOptIntf  Init Failed . Error = %s.", STR_ARR_EN_SIP_RESULT[enResult]);
	    return RET_CODE_FAIL;
    }

    //注册扩展头域
    long lResult = RegExHeaders();
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Register Extend Headers Failed On Init MiniSip.");
        return RET_CODE_FAIL;
    }

    //初始化随机字串发生器
    (void)SipUtilRandInit();

    INFO_LOG("Init MiniSip Success.");
    return RET_CODE_OK;
}

/**
* Description:  RegExHeader().  注册扩展头域
* @return       long.       返回码
*/
long SipStack::SIP::RegExHeaders()
{
    //MiniSip函数返回值
    EN_SIP_RESULT enResult = SIP_RET_SUCCESS;

    //注册扩展头域之Max-Forwards
    enResult = SipCodecRegHeader(EX_HDR_ID_MAX_FORWARDS, SipHdrMaxForwardsInit);//lint !e838
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipCodecRegHeader Max-Forwards Failed On Register Extend Headers. Error = %s.", 
            //STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

    //注册扩展头域之User-Agent
    enResult = SipCodecRegHeader(EX_HDR_ID_USER_AGENT, SipHdrUserAgentInit);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipCodecRegHeader User-Agent Failed On Register Extend Headers. Error = %s.", 
            //STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

    //注册扩展头域之Authorization
    enResult = SipCodecRegHeader(EX_HDR_ID_AUTHORIZATION, SipHdrAuthorizationInit);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipCodecRegHeader Authorization Failed On Register Extend Headers. Error = %s.", 
            //STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

    //注册扩展头域之Expires
    enResult = SipCodecRegHeader(EX_HDR_ID_EXPIRES, SipHdrExpiresInit);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipCodecRegHeader Expires Failed On Register Extend Headers. Error = %s.", 
            //STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

    //注册扩展头域之Content-Type
    enResult = SipCodecRegHeader(EX_HDR_ID_CONTENT_TYPE, SipHdrContentTypeInit);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipCodecRegHeader Content-Type Failed On Register Extend Headers. Error = %s.", 
            //STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

    //注册扩展头域之WWW-Authenticate
    enResult = SipCodecRegHeader(EX_HDR_ID_WWW_AUTHENTICATE, SipHdrWWWAuthenicateInit);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipCodecRegHeader WWW-Authenticate Failed On Register Extend Headers. Error = %s.", 
            //STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

    //注册扩展头域之Authentication-Info
    enResult = SipCodecRegHeader(EX_HDR_ID_AUTHENTICATION_INFO, SipHdrAuthenticationInfoInit);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipCodecRegHeader Authentication-Info Failed On Register Extend Headers. Error = %s.", 
            //STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

    //注册扩展头域之Subject
    enResult = SipCodecRegHeader(EX_HDR_ID_SUBJECT, SipHdrSubjectInit);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipCodecRegHeader Subject Failed On Register Extend Headers. Error = %s.", 
            //STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

    //注册扩展头域之reason
    enResult = SipCodecRegHeader(EX_HDR_ID_REASON, SipHdrReasonInit);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipCodecRegHeader Reason Failed On Register Extend Headers. Error = %s.", 
            //STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

	//注册扩展头域之Date
	//enResult = SipCodecRegHeader(EX_HDR_ID_DATE, SipHdrDateInit);
	//if (SIP_RET_SUCCESS != enResult)
	//{
	//	//ERROR_LOG("SipCodecRegHeader Date Failed On Register Extend Headers. Error = %s.", 
	//		STR_ARR_EN_SIP_RESULT[enResult]);
	//	return RET_CODE_FAIL;
	//}

    return RET_CODE_OK;
}

/**
* Description:  CreateReqMsg(). 创建SIP请求消息，并且设置CSeq头域
* @param  [in]  enSipMethod SIP请求类型 
* @param  [out] pstSipMsg   SIP消息指针
* @return       long.       返回码
*/
long SipStack::SIP::CreateReqMsg
(
    EN_SIP_METHOD   enSipMethod, 
    SipMsgS*&       pstSipMsg
)
{
    //请求类型
    SipToken stTokenMethod = {0};
    stTokenMethod.usTokenId = (unsigned short)enSipMethod;

    //创建请求消息。会创建CSeq头域，但是需重新设置Cseq的值
    EN_SIP_RESULT enResult = SipDsmCreateReqMsg(&stTokenMethod, &pstSipMsg);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipDsmCreateReqMsg Failed On Create SIP Request Message. Error = %s.", 
            //STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

	/*每次发送直接重新生成
    //设置CSeq
    pstSipMsg->stHeaders.pstCseq->ulSequence = ulSequence;
    */

    return RET_CODE_OK;
}

/**
* Description:  SetSipTptIpPort().   设置SipTptIpPort
* @param  [out] stTptIpPort 待设置的SipTptIpPortS引用
* @param  [in]  objAddr     网路地址
* @return       void.
*/
void SipStack::SIP::SetSipTptIpPort
(
    SipTptIpPortS&      stTptIpPort, 
    const CNetworkAddr& objAddr
)
{
    stTptIpPort.iPort = ntohs(objAddr.m_usPort);
    unsigned char* pIP = (unsigned char*)&objAddr.m_lIpAddr;//lint !e1773
    stTptIpPort.u.aucIPv4Addr[0] = pIP[0];
    stTptIpPort.u.aucIPv4Addr[1] = pIP[1];
    stTptIpPort.u.aucIPv4Addr[2] = pIP[2];
    stTptIpPort.u.aucIPv4Addr[3] = pIP[3];
}

/**
* Description:  SetSipTptNwInfo().  设置SipTptNwInfo
* @param  [out] stTptNwInfo 待设置的SipTptNwInfoS引用
* @param  [in]  objDstAddr  目的网路地址
* @param  [in]  objSrcAddr  源网路地址
* @return       long.       返回码
*/
void SipStack::SIP::SetSipTptNwInfo
(
    SipTptNwInfoS&      stTptNwInfo,
    const CNetworkAddr& objDstAddr, 
    const CNetworkAddr& objSrcAddr
)
{
    //IPV4
    stTptNwInfo.ucAddressType   = SIP_ADDR_TYPE_IPV4;
    //UDP
    stTptNwInfo.ucProtocol      = SIP_TRANSPORT_UDP;
    //设置目标地址
    SetSipTptIpPort(stTptNwInfo.stDstAddr, objDstAddr);
    //设置源地址
    SetSipTptIpPort(stTptNwInfo.stSrcAddr, objSrcAddr);
}

/**
* Description:  CreateSipString().  创建SipString
* @param  [in]  hdlMemCp        内存控制块指针
* @param  [out] pstSipString    待创建的SipString
* @param  [in]  szContent       内容
* @return       long.           返回码
*/
long SipStack::SIP::CreateSipString
(
    SipMemCpHdl     hdlMemCp, 
    VppStringS*&    pstSipString, 
    const char*     szContent
)
{
    //内容可以为空
    if (VOS_NULL == szContent)
    {
        return RET_CODE_OK;
    }

    VPP_UINT32 nResult = VppStringCreate(hdlMemCp, (char*)szContent, strlen(szContent), &pstSipString);//lint !e1773
    if (VPP_SUCCESS != nResult)
    {
        ERROR_LOG("VppStringCreate Failed On Create SIP String. ErrorCode = %d. Content = %s",nResult,szContent);
        return RET_CODE_FAIL;
    }

    return RET_CODE_OK;
}

/**
* Description:  CreateQuoteString().  创建带引号的SipString
* @param  [in]  hdlMemCp        内存控制块指针
* @param  [out] pstSipString    待创建的SipString
* @param  [in]  szContent       内容
* @return       long.           返回码
*/
long SipStack::SIP::CreateQuoteString
(
    SipMemCpHdl     hdlMemCp, 
    VppStringS*&    pstSipString,
    const char*     szContent
)
{
    //内容可以为空
    if (VOS_NULL == szContent)
    {
        return RET_CODE_OK;
    }

    //加上双引号的长度
    const unsigned long ulQuoteStrLen = strlen(szContent) + 3;

    VPP_UINT32 nResult = VppStringCreate(hdlMemCp, VOS_NULL, ulQuoteStrLen, &pstSipString);
    if (VPP_SUCCESS != nResult)
    {
        ERROR_LOG("VppStringCreate Failed On Create SIP String. ErrorCode = %d. Content = %s",nResult,szContent);
        return RET_CODE_FAIL;
    }

    //加上双引号
    (void)snprintf(pstSipString->pcData, ulQuoteStrLen, "\"%s\"", szContent);
    pstSipString->ulLen = ulQuoteStrLen-1;

    return RET_CODE_OK;
}

/**
* Description:  CreateSipStringData().  创建SipString的数据
* @param  [in]  hdlMemCp        内存控制块指针
* @param  [out] stSipString     待创建的SipString的数据
* @param  [in]  szContent       内容
* @return       long.           返回码
*/
long SipStack::SIP::CreateSipStringData
(
    SipMemCpHdl     hdlMemCp, 
    VppStringS&     stSipString, 
    const char*     szContent
)
{
    //内容可以为空
    if (VOS_NULL == szContent)
    {
        return RET_CODE_OK;
    }
    
    VPP_UINT32 nResult = VppStringDataCreate(hdlMemCp, (char*)szContent, strlen(szContent), &stSipString);//lint !e1773
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("VppStringDataCreate Failed On Create SIP String. ErrorCode = %d. Content = %s.", nResult, szContent);
        return RET_CODE_FAIL;
    }

    return RET_CODE_OK;
}

/**
* Description:  SetSipString(). 设置SipString
* @param  [in]  stSipString     待创建的SipString
* @param  [in]  szContent       内容
* @return       void.
*/
void SipStack::SIP::SetSipString
(
    SipString&  stSipString, 
    const char* szContent
)
{
    stSipString.pcData = (char*)szContent;//lint !e1773
    stSipString.ulLen  = strlen(szContent);
}

/**
* Description:  SetSipHostPort().   设置SipHostPort
* @param  [out] stHostPort  待设置的HostPort引用
* @param  [in]  objAddr     网路地址
* @return       void.
*/
void SipStack::SIP::SetSipHostPort
(
    SipHostPort&        stHostPort, 
    const CNetworkAddr& objAddr
)
{
    stHostPort.iPort = ntohs(objAddr.m_usPort);
    stHostPort.stHost.enHostType = SIP_ADDR_TYPE_IPV4;
    SS_UINT8* pIP = (SS_UINT8*)&objAddr.m_lIpAddr;//lint !e1773
    stHostPort.stHost.uHostContent.ipv4[0] = pIP[0];
    stHostPort.stHost.uHostContent.ipv4[1] = pIP[1];
    stHostPort.stHost.uHostContent.ipv4[2] = pIP[2];
    stHostPort.stHost.uHostContent.ipv4[3] = pIP[3];
}

void SipStack::SIP::SetSipHostPort_New
(
    SipHostPort&         sip_host_port_st_r, 
    const string&   ip_str_r,
    const unsigned short port_us
)
{
    //stHostPort.iPort = ntohs(objAddr.m_usPort);
    //stHostPort.stHost.enHostType = SIP_ADDR_TYPE_IPV4;
    //SS_UINT8* pIP = (SS_UINT8*)&objAddr.m_lIpAddr;
    //stHostPort.stHost.uHostContent.ipv4[0] = pIP[0];
    //stHostPort.stHost.uHostContent.ipv4[1] = pIP[1];
    //stHostPort.stHost.uHostContent.ipv4[2] = pIP[2];
    //stHostPort.stHost.uHostContent.ipv4[3] = pIP[3];
    //(long)inet_addr

    //设置协议类型
    sip_host_port_st_r.stHost.enHostType = SIP_ADDR_TYPE_IPV4;

    //设置端口
    sip_host_port_st_r.iPort = port_us;

    //设置网址
    unsigned long addr = (unsigned long)ACE_OS::inet_addr(ip_str_r.c_str());
    SS_INT8* ip_p = (SS_INT8*)&addr;
    sip_host_port_st_r.stHost.uHostContent.ipv4[0] = (SS_UCHAR)ip_p[0];
    sip_host_port_st_r.stHost.uHostContent.ipv4[1] = (SS_UCHAR)ip_p[1];
    sip_host_port_st_r.stHost.uHostContent.ipv4[2] = (SS_UCHAR)ip_p[2];
    sip_host_port_st_r.stHost.uHostContent.ipv4[3] = (SS_UCHAR)ip_p[3];
}

/**
* Description:  SetSipHostName().    设置HostName
* @param  [in]  hdlMemCp        内存控制块指针
* @param  [out] stHostPort      待设置的HostPort引用
* @param  [in]  pszHostName     主机名
* @return       long.   返回值
*/
long SipStack::SIP::SetSipHostName
(
    SipMemCpHdl     hdlMemCp, 
    SipHostPort&    stHostPort, 
    const char*     pszHostName
)
{
    stHostPort.iPort = SIP_INVALID_VALUE;
    stHostPort.stHost.enHostType = SIP_ADDR_TYPE_HOST_NAME;

    //设置HostName
    long lResult = CreateSipString(hdlMemCp, stHostPort.stHost.uHostContent.pstrHostName, pszHostName);
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Create Host Name Failed On Set Host Name.");
        return RET_CODE_FAIL;
    }

    return RET_CODE_OK;
}

/**
* Description:  SetURI().   设置URI
* @param  [in]  hdlMemCp    内存控制块指针
* @param  [out] stURI       待设置的URI引用
* @param  [in]  pszUriName  URI名
* @return       long.       返回码
*/
long SipStack::SIP::SetURI
(
    SipMemCpHdl hdlMemCp, 
    URI&        stURI, 
    const char* pszUriName
)
{
    stURI.enURISchemeType = SIP_URI_SCH_SIP;

    //在内存控制块上分配内存
    stURI.uri.pstSipUri = (SipURI*)SipMemCpMalloc(hdlMemCp, sizeof(SipURI));
    if (SS_NULL_PTR == stURI.uri.pstSipUri)
    {
        //ERROR_LOG("SipMemCpMalloc Failed On Set URI.");
        return RET_CODE_FAIL;
    }

    //TTL，暂不使用
    stURI.uri.pstSipUri->iTtl = SIP_INVALID_VALUE;

    //创建URI UserName
    long lResult = CreateSipString(hdlMemCp, stURI.uri.pstSipUri->pstrUserName, pszUriName);
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Create URI User Name Failed On Set URI.");
        return RET_CODE_FAIL;
    }

    return RET_CODE_OK;
}

/**
* Description:  SetUriByDomain().   设置域名URI
* @param  [in]  hdlMemCp        内存控制块指针
* @param  [out] stURI           待设置的URI引用
* @param  [in]  pszUriName      URI名
* @param  [in]  pszUriDomain    URI域
* @return       long.       返回码
*/
long SipStack::SIP::SetUriByDomain
(
    SipMemCpHdl     hdlMemCp, 
    URI&            stURI, 
    const char*     pszUriName, 
    const char*     pszUriDomain
)
{
    long lResult = RET_CODE_OK;

    lResult = SetURI(hdlMemCp, stURI, pszUriName);//lint !e838
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Set URI Failed By Domain.");
        return RET_CODE_FAIL;
    }

    lResult = SetSipHostName(hdlMemCp, stURI.uri.pstSipUri->stHostPort, pszUriDomain);
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Set Host Name Failed On Set URI By Domain.");
        return RET_CODE_FAIL;
    }

    return RET_CODE_OK;
}

/**
* Description:  SetUriByAddr(). 设置地址型URI
* @param  [in]  hdlMemCp        内存控制块指针
* @param  [out] stURI           待设置的URI引用
* @param  [in]  pszUriName      URI名
* @param  [in]  objAddr         网路地址
* @return       long.       返回码
*/
long SipStack::SIP::SetUriByAddr
(
    SipMemCpHdl         hdlMemCp, 
    URI&                stURI, 
    const char*         pszUriName, 
    const CNetworkAddr& objAddr
)
{
    long lResult = RET_CODE_OK;

    lResult = SetURI(hdlMemCp, stURI, pszUriName);//lint !e838
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Set URI Failed By Domain.");
        return RET_CODE_FAIL;
    }

    SetSipHostPort(stURI.uri.pstSipUri->stHostPort, objAddr);

    return RET_CODE_OK;
}

/**
* Description:  SetRequestUri().    设置Request-URI
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  pszUriName      URI名
* @param  [in]  pszUriDomain    URI域
* @return       long.       返回码
*/
long SipStack::SIP::SetRequestUri
(
    SipMsgS&        stSipMsg,
    const char*     pszUriName, 
    const char*     pszUriDomain
)
{
    //Request URI
    long lResult = SetUriByDomain(stSipMsg.hdlMemCp, 
                                  stSipMsg.uFirstLine.stRequestLine.stRequestURI, 
                                  pszUriName,
                                  pszUriDomain);
    return lResult;
}

/**
* Description:  SetCallId().  设置Call-ID头域
* @param  [in]          stSipMsg        Sip消息结构体引用
* @param  [in/out]  strCallId       CallID字符串的值，为空时将随机生成
* @return       long.       返回码
*/
long SipStack::SIP::SetCallId
(
    SipMsgS&    stSipMsg,
    string&     strCallId
)
{    
    //CallID
	SipCallID* pCallID = (SipCallID*)SipDsmCreateHdrInMsg(SIP_BASIC_HDR_ID_CALLID, &stSipMsg);
	if(NULL == pCallID)
	{
		ERROR_LOG("SipDsmCreateHdrInMsg return NULL.");
		return RET_CODE_FAIL;
	}
    SipCallID& stCallID = (*pCallID);

    //直接使用设置值
    if (!strCallId.empty())
    {
        long lResult = CreateSipStringData(stSipMsg.hdlMemCp, stCallID, strCallId.c_str());

        if (RET_CODE_OK != lResult)
        {
            //ERROR_LOG("Set Call-ID Value Failed On Set Call-ID Header.");
            return RET_CODE_FAIL;
        }
        
        return RET_CODE_OK;
    }
    
    //创建随机的Call-ID
    EN_SIP_RESULT enResult = SipTxnUtilGenTag(stSipMsg.hdlMemCp, &stCallID);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("Create Call-ID Failed On Set Call-ID Header. Error = %s.", STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

    //计算buffer的最大长度，加上结束符的长度
	unsigned long ulBufLen = 0;
	if(ULONG_MAX - stCallID.ulLen > 1)
	{
		ulBufLen = stCallID.ulLen + 1;
	}
	else
	{
		ERROR_LOG("stCallID.ulLen is too large");
		return RET_CODE_FAIL;
	}

    char* pBuffer = VOS_NEW(pBuffer, ulBufLen);
    if (VOS_NULL == pBuffer)
    {
        //ERROR_LOG("Create Buffer Failed On Set Call-ID Header.");
        return RET_CODE_FAIL;
    }
    memset(pBuffer, 0, ulBufLen);
    strncpy(pBuffer, stCallID.pcData, ulBufLen - 1);

    //保存Call-ID
    strCallId = pBuffer;

    VOS_DELETE(pBuffer, MULTI);
    return RET_CODE_OK;
}

/**
* Description:  SetFrom().  设置From头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  pszUriName      URI名
* @param  [in]  pszUriDomain    URI域
* @return       long.       返回码
*/
long SipStack::SIP::SetFrom
(
    SipMsgS&        stSipMsg,
    const char*     pszUriName, 
    const char*     pszUriDomain
)
{
    //自定义函数返回值
    long lResult = RET_CODE_OK;

    //From
	SipFrom* pSipFrom = (SipFrom*)SipDsmCreateHdrInMsg(SIP_BASIC_HDR_ID_FROM, &stSipMsg);
	if(NULL == pSipFrom)
	{
		WARN_LOG("SipDsmCreateHdrInMsg return null.");
		return RET_CODE_FAIL;
	}
    SipFrom& stFrom = *pSipFrom;

    /*每次发送直接重新生成
    //From Tag
    EN_SIP_RESULT enResult = SipTxnUtilGenTag(stSipMsg.hdlMemCp, &stFrom.strTag);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("Create From Tag Failed On Set From Header. Error = %s.", STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }
    */

    //From DisplayName
	if (SIP_METHOD_INVITE != stSipMsg.pstSipMethod->usTokenId)
	{
		lResult = CreateSipString(stSipMsg.hdlMemCp, stFrom.pstrDisplayName, pszUriName);
		//可以不设置，故失败时仅打印告警日志
		if (RET_CODE_OK != lResult)
		{
			//WARN_LOG("Create From Display Name Failed On Set From Header.");
		}
	}

    //From URI
    lResult = SipStack::SIP::SetUriByDomain(stSipMsg.hdlMemCp, stFrom.stUri, pszUriName, pszUriDomain);
    if (SipStack::SIP::RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Set From URI Failed On Set From Header.");
        return lResult;
    }

    return RET_CODE_OK;
}

/**
* Description:  SetTo().    设置To头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  pszUriName      URI名
* @param  [in]  pszUriDomain    URI域
* @return       long.       返回码
*/
long SipStack::SIP::SetTo
(
    SipMsgS&        stSipMsg,
    const char*     pszUriName, 
    const char*     pszUriDomain
)
{
    //自定义函数返回值
    long lResult = RET_CODE_OK;

    //To
	SipTo* pSipTo = (SipTo*)SipDsmCreateHdrInMsg(SIP_BASIC_HDR_ID_TO, &stSipMsg);
	if(NULL == pSipTo)
	{
		WARN_LOG("SipDsmCreateHdrInMsg return null.");
		return RET_CODE_FAIL;
	}
    SipTo& stTo = *pSipTo;

    //To DisplayName
	if (SIP_METHOD_INVITE != stSipMsg.pstSipMethod->usTokenId)
	{
		lResult = CreateSipString(stSipMsg.hdlMemCp, stTo.pstrDisplayName, pszUriName);
		//可以不设置，故失败时仅打印告警日志
		if (RET_CODE_OK != lResult)
		{
			//WARN_LOG("Create To Display Name Failed On Set To Header.");
		}
	}

    //To URI
    lResult = SipStack::SIP::SetUriByDomain(stSipMsg.hdlMemCp, stTo.stUri, pszUriName, pszUriDomain);
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Set To URI Failed On Set To Header.");
        return lResult;
    }

    return RET_CODE_OK;
}

/**
* Description:  SetToTag().    设置To的Tag
* @param  [in]  stSipMsg        Sip消息结构体引用
* @return       long.       返回码
*/
long SipStack::SIP::SetToTag
(
    SipMsgS& stSipMsg
)
{
    if (VOS_NULL == stSipMsg.stHeaders.pstTo)
    {
        //ERROR_LOG("Set To Tag Failed. To Header Not Exist.");
        return RET_CODE_FAIL;
    }

    //To的Tag不存在时才设置
    if (VOS_NULL == stSipMsg.stHeaders.pstTo->strTag.pcData)
    {
        //随机生成To的Tag
        EN_SIP_RESULT enResult = SipTxnUtilGenTag(stSipMsg.hdlMemCp, &stSipMsg.stHeaders.pstTo->strTag);
        if (SIP_RET_SUCCESS != enResult)
        {
            //ERROR_LOG("Create To Tag Failed On Set To Tag. Error = %s.",
                //STR_ARR_EN_SIP_RESULT[enResult]);
            return RET_CODE_FAIL;
        }
    } 

    return RET_CODE_OK;
}//lint !e1764

/**
* Description:  SetVia().          设置Via头域
* @param  [in]  sip_msg_st_r       Sip消息结构体引用
* @param  [in]  ip_str_r           网路地址
                port_us            网络端口
* @return       long.              返回码
*/
long SipStack::SIP::SetVia
(
    SipMsgS&             sip_msg_st_r,
    const string&   ip_str_r,
    const unsigned short port_us
)
{
    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;

    //创建Via列表
    SipViaListHdl pViaList = (SipViaListHdl)SipDsmCreateHdrInMsg(SIP_BASIC_HDR_ID_VIA_LIST, &sip_msg_st_r);

    //创建Via
    SipVia* pstVia = (SipVia*)SipMemCpMalloc(sip_msg_st_r.hdlMemCp, sizeof(SipVia));
    if (SS_NULL_PTR == pstVia)
    {
        //ERROR_LOG("Create Via Header Failed On Set Via Header.");
        return RET_CODE_FAIL;
    }

    SipVia& stVia = *pstVia;

    //添加到Via列表
    nResult = VppListInsert(pViaList, 0, &stVia);//lint !e838
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("Insert Via to List Failed On Set Via Header.");
        return RET_CODE_FAIL;
    }

    //Via SIP
    stVia.ulProtocolType = SIP_VIA_PROTOCOL_SIP;
    //Via SIP Version
    SetSipString(stVia.strProtocolVersion, SIP_VERSION_2);
    //Via UDP
    stVia.stTransport.usTokenId = SIP_TRANSPORT_UDP;
    //Via IP Port
    //SetSipHostPort(stVia.stSentBy, objAddr);
    SetSipHostPort_New(stVia.stSentBy, ip_str_r, port_us);

    /*
    //Via Branch
    EN_SIP_RESULT enResult = SipTxnUtilGenBranchId(stSipMsg.hdlMemCp, &stVia.strBranch);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("Create Branch Failed On Set Via Header. Error = %s.", STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }
    */

    //Via Rport
    //stVia.iRPort = SIP_INVALID_VALUE;
    stVia.iRPort = SIP_VIA_RPORT_PRESENT_NO_VAL;

    return RET_CODE_OK;
}

/**
* Description:  AddExHeader().  添加扩展头域
* @param  [in]  stSipMsg    Sip消息结构体引用
* @param  [in]  usHdrId     扩展头域ID
* @param  [in]  pvHdrVal    扩展头域值指针
* @return       long.       返回码
*/
long SipStack::SIP::AddExHeader
(
    SipMsgS&        stSipMsg,
    unsigned short  usHdrId,
    void*           pvHdrVal
)
{
    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;

    //先将已有头域删除
    SipDsmRmvHdrFromMsg(usHdrId, &stSipMsg);

    //获得扩展头域列表指针
    SipHeaderListHdl pHeaderList = stSipMsg.stHeaders.hdlExtHeaders;

    if (SS_NULL_PTR == pHeaderList)
    {
        //创建扩展头域列表
        nResult = VppListCreate(stSipMsg.hdlMemCp, MAX_NUM_EX_HEADER, 0, &pHeaderList);
        if (VPP_SUCCESS != nResult)
        {
            //ERROR_LOG("Create Extend Header List Failed On Add Extend Header.");
            return RET_CODE_FAIL;
        }

        //保存扩展头域列表指针
        stSipMsg.stHeaders.hdlExtHeaders = pHeaderList;
    }
    
    //创建扩展头域
    SipHdrIdValue* pstHeadIdValue = (SipHdrIdValue*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipHdrIdValue));
    if (SS_NULL_PTR == pstHeadIdValue)
    {
        //ERROR_LOG("Create Extend Header Failed On Add Extend Header.");
        return RET_CODE_FAIL;
    }

    //添加到扩展头域列表
    nResult = VppListInsert(pHeaderList, VppListGetCount(pHeaderList), pstHeadIdValue);
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("Insert Header To Extend Header List Failed On Add Extend Header.");
        return RET_CODE_FAIL;
    }

    //设置头域ID和值
    pstHeadIdValue->usHdrId     = usHdrId;
    pstHeadIdValue->pvHdrVal    = pvHdrVal;

    return RET_CODE_OK;
}

/**
* Description:  SetMaxForwords().   设置Max-Forwards头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  nMaxForwards    Max-Forwards值
* @return       long.       返回码
*/
long SipStack::SIP::SetMaxForwards
(
    SipMsgS&    stSipMsg,
    SS_UINT32   nMaxForwards
)
{
    //添加扩展头域Max-Forwords
    SipMaxForwards* pstMaxForwards = (SipMaxForwards*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipMaxForwards));
    if (SS_NULL_PTR == pstMaxForwards)
    {
        //ERROR_LOG("Create Max-Forwards Header Failed On Set Max-Forwards.");
        return RET_CODE_FAIL;
    }

    //设置值
    *pstMaxForwards = nMaxForwards;

    //添加扩展头域
    long lResult = SipStack::SIP::AddExHeader(stSipMsg, EX_HDR_ID_MAX_FORWARDS, pstMaxForwards);
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Add Max-Forwards Header Failed On Set Max-Forwards.");
        return RET_CODE_FAIL;
    }

    return RET_CODE_OK;
}

/**
* Description:  SetUserAgent(). 设置User-Agent头域
* @param  [in]  stSipMsg            Sip消息结构体引用
* @param  [in]  pszProductName      产品名称
* @param  [in]  pszProductVersion   产品版本
* @return       long.       返回码
*/
long SipStack::SIP::SetUserAgent
(
    SipMsgS&    stSipMsg,
    const char* pszProductName,
    const char* pszProductVersion
)
{
    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;
    //自定义函数返回值
    long lResult = RET_CODE_OK;

    SipUserAgentListHdl pUserAgentListHdl = NULL;

    //创建UserAgent列表，最多只有1个元素
    nResult = VppListCreate(stSipMsg.hdlMemCp, 1, 0, &pUserAgentListHdl);//lint !e838
    if (VPP_SUCCESS != nResult)
    {
        ERROR_LOG("Create User-Agent Header List Failed On Set User-Agent Header.");
        return RET_CODE_FAIL;
    }

    //创建UserAgent
    SipUserAgent* pstUserAgent = (SipUserAgent*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipUserAgent));
    if (SS_NULL_PTR == pstUserAgent)
    {
        ERROR_LOG("Create User-Agent Header Failed On Set User-Agent Header.");
        return RET_CODE_FAIL;
    }

    //设置User-Agent的产品名称
    lResult = CreateSipString(stSipMsg.hdlMemCp, pstUserAgent->pstrProduct, pszProductName);//lint !e838
    if (RET_CODE_OK != lResult)
    {
        ERROR_LOG("Set User-Agent Product Name Failed On Set User-Agent Header.");
        return lResult;
    }
    
    //设置User-Agent的产品版本
    lResult = CreateSipString(stSipMsg.hdlMemCp, pstUserAgent->pstrProductVersion, pszProductVersion);
    if (RET_CODE_OK != lResult)
    {
        ERROR_LOG("Set User-Agent Product Version Failed On Set User-Agent Header.");
        return lResult;
    }

    //添加UserAgent列表
    nResult = VppListInsert(pUserAgentListHdl, 0, pstUserAgent);
    if (VPP_SUCCESS != nResult)
    {
        ERROR_LOG("Insert Header To Extend Header List Failed On Set User-Agent Header.");
        return SipStack::SIP::RET_CODE_FAIL;
    }

    //添加到扩展头域列表
    lResult = SipStack::SIP::AddExHeader(stSipMsg, EX_HDR_ID_USER_AGENT, pUserAgentListHdl);
    if (RET_CODE_OK != lResult)
    {
        ERROR_LOG("Add User-Agent Header Failed On Set User-Agent Header.");
        return lResult;
    }

    return RET_CODE_OK;
}

/**
* Description:  SetRoute(). 设置Route头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  objRouteAddr    路由地址
* @param  [in]  pszDisplayName  显示名字
* @return       long.       返回码
*/
long SipStack::SIP::SetRoute
(
    SipMsgS&            stSipMsg,
    const CNetworkAddr& objRouteAddr,
    const char*         pszDisplayName
)
{
    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;
    //自定义函数返回值
    long lResult = RET_CODE_OK;

    //创建Route列表，最多只有1个元素
    nResult = VppListCreate(stSipMsg.hdlMemCp, 1, 0, &stSipMsg.stHeaders.hdlRouteList);//lint !e838
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("Create Route Header List Failed On Set Route Header.");
        return RET_CODE_FAIL;
    }

    //创建Route
    SipRoute* pstRoute = (SipRoute*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipRoute));
    if (SS_NULL_PTR == pstRoute)
    {
        //ERROR_LOG("Create Route Header Failed On Set Route Header.");
        return RET_CODE_FAIL;
    }

    //设置Route的地址
    lResult = SetUriByAddr(stSipMsg.hdlMemCp, pstRoute->stUri, NULL, objRouteAddr);//lint !e838
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Set Route URI Failed On Set Route Header.");
        return lResult;
    }

    //设置Route的显示名称
    lResult = CreateSipString(stSipMsg.hdlMemCp, pstRoute->pstrDisplayName, pszDisplayName);
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Set Route Display Name Failed On Set Route Header.");
        return lResult;
    }

    //添加到Route列表
    nResult = VppListInsert(stSipMsg.stHeaders.hdlRouteList, 0, pstRoute);
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("Insert Header To Route List Failed On Set Route Header.");
        return RET_CODE_FAIL;
    }

    return RET_CODE_OK;
}

/**
* Description:  AddExHeader().  添加扩展头域
* @param  [in]  stSipMsg                    Sip消息结构体引用
* @param  [in]  stSipAuthorization  Sip鉴权信息
* @return       long.       返回码
*/
long SipStack::SIP::AddAuthorizationHeader
(
    SipMsgS&            stSipMsg,
    SipAuthorization&   stSipAuthorization
)
{
    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;
    //自定义函数返回值
    long lResult = RET_CODE_OK;

    //已存在则直接添加
    SipAuthorizationListHdl pAuthorizationListHdl = (SipAuthorizationListHdl)SipDsmGetHdrFromMsg(EX_HDR_ID_AUTHORIZATION, &stSipMsg);
    if (VOS_NULL != pAuthorizationListHdl)
    {
        const unsigned short usAuthNum = VppListGetCount(pAuthorizationListHdl);
        //在允许个数的范围内，继续添加，否则重新创建
        if (SipStack::SIP::MAX_NUM_AUTH_HEADER > usAuthNum)
        {
            //添加Authorization到列表
            nResult = VppListInsert(pAuthorizationListHdl, usAuthNum, &stSipAuthorization);
            if (VPP_SUCCESS != nResult)
            {
                //ERROR_LOG("Insert Header To Extend Header List Failed On Add Authorization Header.");
                return RET_CODE_FAIL;
            }
            
            return RET_CODE_OK;
        }
    }

    //创建新的Authorization列表，最多只有SipStack::SIP::MAX_NUM_AUTH_HEADER个元素
    nResult = VppListCreate(stSipMsg.hdlMemCp, SipStack::SIP::MAX_NUM_AUTH_HEADER, 0, &pAuthorizationListHdl);
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("Create Authorization Header List Failed On Add Authorization Header.");
        return RET_CODE_FAIL;
    }

    //添加Authorization到列表
    nResult = VppListInsert(pAuthorizationListHdl, 0, &stSipAuthorization);
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("Insert Header To Extend Header List Failed On Add Authorization Header.");
        return RET_CODE_FAIL;
    }

    //添加到扩展头域列表
    lResult = AddExHeader(stSipMsg, EX_HDR_ID_AUTHORIZATION, pAuthorizationListHdl);//lint !e838
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Add Authorization To Extend Header Failed On Add Authorization Header.");
        return lResult;
    }

    return RET_CODE_OK;
}

/**
* Description:  AddExHeader().  添加扩展头域
* @param  [in]  stSipMsg                    Sip消息结构体引用
* @param  [in]  stSipAuthorization  Sip鉴权信息
* @return       long.       返回码
*/
long SipStack::SIP::AddWWWAuthorizationHeader
(
 SipMsgS&            stSipMsg,
 SipWWWAuthenticate&   stSipAuthorization
 )
{
    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;
    //自定义函数返回值
    long lResult = RET_CODE_OK;

    //已存在则直接添加
    SipWWWAuthenticateListHdl pAuthorizationListHdl = (SipWWWAuthenticateListHdl)SipDsmGetHdrFromMsg(EX_HDR_ID_WWW_AUTHENTICATE, &stSipMsg);
    if (VOS_NULL != pAuthorizationListHdl)
    {
        const unsigned short usAuthNum = VppListGetCount(pAuthorizationListHdl);
        //在允许个数的范围内，继续添加，否则重新创建
        if (SipStack::SIP::MAX_NUM_AUTH_HEADER > usAuthNum)
        {
            //添加Authorization到列表
            nResult = VppListInsert(pAuthorizationListHdl, usAuthNum, &stSipAuthorization);
            if (VPP_SUCCESS != nResult)
            {
                //ERROR_LOG("Insert Header To Extend Header List Failed On Add Authorization Header.");
                return RET_CODE_FAIL;
            }

            return RET_CODE_OK;
        }
    }

    //创建新的Authorization列表，最多只有SipStack::SIP::MAX_NUM_AUTH_HEADER个元素
    nResult = VppListCreate(stSipMsg.hdlMemCp, SipStack::SIP::MAX_NUM_AUTH_HEADER, 0, &pAuthorizationListHdl);
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("Create Authorization Header List Failed On Add Authorization Header.");
        return RET_CODE_FAIL;
    }

    //添加Authorization到列表
    nResult = VppListInsert(pAuthorizationListHdl, 0, &stSipAuthorization);
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("Insert Header To Extend Header List Failed On Add Authorization Header.");
        return RET_CODE_FAIL;
    }

    //添加到扩展头域列表
    lResult = AddExHeader(stSipMsg, EX_HDR_ID_WWW_AUTHENTICATE, pAuthorizationListHdl);//lint !e838
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Add Authorization To Extend Header Failed On Add Authorization Header.");
        return lResult;
    }

    return RET_CODE_OK;
}

/**
* Description:  SetAuthorization(). 设置Authorization头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  pszUserName     用户名
* @param  [in]  pszPassword     密码
* @param  [in]  stPlatAuthInfo  平台鉴权信息
* @return       long.       返回码
*/
long SipStack::SIP::SetAuthorization
(
    SipMsgS&                stSipMsg,
    char*                   pszUserName,
    char*                   pszPassword,
    const PLAT_AUTH_INFO&   stPlatAuthInfo
)
{
	//返回值
    long lResult = RET_CODE_OK;

    //获得SIP请求消息类型
    const unsigned long ulSipMethod = stSipMsg.uFirstLine.stRequestLine.stMethod.usTokenId;

    if (SIP_METHOD_BUTT <= ulSipMethod)
    {
        ERROR_LOG("Set Authorization Header Failed. SIP Method(%d) Can't be Greater Than SIP_METHOD_BUTT",ulSipMethod);
        return RET_CODE_FAIL;
    }

    //创建Authorization
    SipAuthorization* pstAuthorization = (SipAuthorization*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipAuthorization));
    if (SS_NULL_PTR == pstAuthorization)
    {
        ERROR_LOG("Create Authorization Header Failed On Set Authorization Header.");
        return RET_CODE_FAIL;
    }

    char szCNonce[MAX_LEN_AUTH_STRING] = {0};
    //cnonce复用From Tag
    strncpy(szCNonce, stSipMsg.stHeaders.pstFrom->strTag.pcData, sizeof(szCNonce) - 1);

    //转换nonce count为字符串，直接复用CSeq
    char szNonceCount[MAX_LEN_AUTH_STRING] = {0};
    (void)sprintf_s(szNonceCount, MAX_LEN_AUTH_STRING,"%08x", stSipMsg.stHeaders.pstCseq->ulSequence);

    //获取Reuqest-URI字符串
    const SipURI& stReqUri = *stSipMsg.uFirstLine.stRequestLine.stRequestURI.uri.pstSipUri;

    char* pszDigestUri = VOS_NULL;
    lResult = CreateAuthDigestUri(stReqUri, pszDigestUri);//lint !e838
    if (RET_CODE_OK != lResult)
    {
        ERROR_LOG("Create Digest URI String Failed On Set Authorization Header.");
        return lResult;
    }    

    //计算Response
    HASHHEX HA1;
    HASHHEX Response;

    DigestCalcHA1((char*)"", pszUserName, (char*)stPlatAuthInfo.strRealm.c_str(), //lint !e1773
                  pszPassword, (char*)"", (char*)"", HA1);//lint !e1773
    DigestCalcResponse(HA1, (char*)stPlatAuthInfo.strNonce.c_str(), szNonceCount, szCNonce, //lint !e1773
                       (char*)stPlatAuthInfo.strQop.c_str(), (char*)STR_ARR_SIP_METHOD[ulSipMethod],//lint !e1773
                       pszDigestUri, (char*)"", Response);//lint !e1773

    //Digest鉴权方式
    pstAuthorization->bIsAuthDigestType = SS_TRUE;

    //设置Authorization的各值
    lResult = CreateQuoteString(stSipMsg.hdlMemCp, pstAuthorization->pstrDigestUri, pszDigestUri);
    if (RET_CODE_OK != lResult)
    {
        //释放临时内存
        VOS_DELETE(pszDigestUri, MULTI);
        
        ERROR_LOG("Create Digest URI String Failed On Set Authorization Header.");
        return lResult;
    }

    //释放临时内存
    VOS_DELETE(pszDigestUri, MULTI);

    lResult = CreateQuoteString(stSipMsg.hdlMemCp, pstAuthorization->pstrUsername, pszUserName);
    if (RET_CODE_OK != lResult)
    {
        ERROR_LOG("Create User Name String Failed On Set Authorization Header.");
        return lResult;
    }

    lResult = CreateQuoteString(stSipMsg.hdlMemCp, pstAuthorization->pstrRealm, stPlatAuthInfo.strRealm.c_str());
    if (RET_CODE_OK != lResult)
    {
        ERROR_LOG("Create Realm String Failed On Set Authorization Header.");
        return lResult;
    }

    // added by dhong
	lResult = CreateSipString(stSipMsg.hdlMemCp, pstAuthorization->pstrAlgorithm, stPlatAuthInfo.stAlgorithm.c_str());
	if (RET_CODE_OK != lResult)
	{
		ERROR_LOG("Create Algorithm String Failed On Set Authorization Header.");
		return lResult;
	}

	if(!stPlatAuthInfo.strQop.empty())
	{
		lResult = CreateSipString(stSipMsg.hdlMemCp, pstAuthorization->pstrMessageQOP, stPlatAuthInfo.strQop.c_str());
		if (RET_CODE_OK != lResult)
		{
			ERROR_LOG("Create Qop String Failed On Set Authorization Header.");
			return lResult;
		}
	}


	lResult = CreateQuoteString(stSipMsg.hdlMemCp, pstAuthorization->pstrNonce, stPlatAuthInfo.strNonce.c_str());
	if (RET_CODE_OK != lResult)
	{
		ERROR_LOG("Create Nonce String Failed On Set Authorization Header.");
		return lResult;
	}

	if(!stPlatAuthInfo.strOpaque.empty())
	{

		lResult = CreateQuoteString(stSipMsg.hdlMemCp, pstAuthorization->pstrOpaque, stPlatAuthInfo.strOpaque.c_str());
		if (RET_CODE_OK != lResult)
		{
			ERROR_LOG("Create Opaque String Failed On Set Authorization Header.");
			return lResult;
		}


		lResult = CreateSipString(stSipMsg.hdlMemCp, pstAuthorization->pstrNonceCount, szNonceCount);
		if (RET_CODE_OK != lResult)
		{
			ERROR_LOG("Create Nonce Count String Failed On Set Authorization Header.");
			return lResult;
		}

		lResult = CreateQuoteString(stSipMsg.hdlMemCp, pstAuthorization->pstrCNonce, szCNonce);
		if (RET_CODE_OK != lResult)
		{
			ERROR_LOG("Create Client Nonce String Failed On Set Authorization Header.");
			return lResult;
		}
	}

    lResult = CreateQuoteString(stSipMsg.hdlMemCp, pstAuthorization->pstrDResponse, Response);
    if (RET_CODE_OK != lResult)
    {
        ERROR_LOG("Create Response String Failed On Set Authorization Header.");
        return lResult;
    }

    //添加Authrization头域到SIP消息
    lResult = AddAuthorizationHeader(stSipMsg, *pstAuthorization);
    if (RET_CODE_OK != lResult)
    {
        ERROR_LOG("Add Authorization Header Failed On Set Authorization Header.");
        return lResult;
    }
    
    return RET_CODE_OK;
}

/**
* Description:  CreateAuthDigestUri().  创建Authorization的Digest的URI
* @param  [in]      stReqUri        SipURI结构体引用
* @param  [out]     pszDigestUri    DigestUri字符串输出
* @return       long.       返回码
*/
long SipStack::SIP::CreateAuthDigestUri
(
    const SipURI&   stReqUri,
    char*&          pszDigestUri
)
{
    //用户名的长度
    unsigned long ulReqUriUserNameLen = 0;

    //Register的Request-URI没有UserName字段
    if (VOS_NULL != stReqUri.pstrUserName)
    {
        ulReqUriUserNameLen = stReqUri.pstrUserName->ulLen;
    };

    //域名长度
    const unsigned long ulReqUriDomainLen   = stReqUri.stHostPort.stHost.uHostContent.pstrHostName->ulLen;
    //摘要URI的实际长度
    const unsigned long ulDigestUriLen      = strlen(SIP_URI_SCHEME) + ulReqUriUserNameLen + ulReqUriDomainLen + 2;
    
    if (VOS_NULL == VOS_NEW(pszDigestUri, ulDigestUriLen))
    {
        ERROR_LOG("Create Digest Uri Failed On Set Authorization Header.");
        return RET_CODE_FAIL;
    }
    memset(pszDigestUri, 0, ulDigestUriLen);

	//摘要字符串的当前长度
	unsigned long ulStrlen = strlen(SIP_URI_SCHEME);
	//追加SIP的URI的方案名
	strncpy(pszDigestUri, SIP_URI_SCHEME, ulStrlen);

	//Register的Request-URI没有UserName字段
	if (0 != ulReqUriUserNameLen)
	{
		//追加用户名
		strncpy(pszDigestUri + ulStrlen, stReqUri.pstrUserName->pcData, ulReqUriUserNameLen);
		strncpy(pszDigestUri+ ulStrlen+ulReqUriUserNameLen ,"@",1);
		//加上用户名和@的长度
		ulStrlen += ulReqUriUserNameLen + 1;
	}

	//追加用户域
	strncpy(pszDigestUri + ulStrlen, stReqUri.stHostPort.stHost.uHostContent.pstrHostName->pcData, ulReqUriDomainLen);

    return RET_CODE_OK;
}

/**
* Description:  SetContact(). 设置Contact头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  objContactAddr  Contact地址
* @param  [in]  pszDisplayName  显示名字
* @return       long.       返回码
*/
long SipStack::SIP::SetContact
(
    SipMsgS&            stSipMsg,
    const CNetworkAddr& objContactAddr,
    const char*         pszDisplayName
)
{
    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;
    //自定义函数返回值
    long lResult = RET_CODE_OK;

    //创建Contact列表
    SipContactListHdl pContactList = (SipContactListHdl)SipDsmCreateHdrInMsg(SIP_BASIC_HDR_ID_CONTACT_LIST, &stSipMsg);

    //创建Contact
    SipContact* pstContact = (SipContact*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipContact));
    if (SS_NULL_PTR == pstContact)
    {
        //ERROR_LOG("Create Contact Header Failed On Set Contact Header.");
        return RET_CODE_FAIL;
    }

    //添加Contact到列表
    nResult = VppListInsert(pContactList, 0, pstContact);//lint !e838
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("Insert Header To Extend Header List Failed On Set Contact Header.");
        return RET_CODE_FAIL;
    }

    SipContact& stContact = *pstContact;

    //创建Contact的URI
    stContact.pstContact = (SipUriHeader*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipUriHeader));
    if (SS_NULL_PTR == stContact.pstContact)
    {
        //ERROR_LOG("Create Contact URI Failed On Set Contact Header.");
        return RET_CODE_FAIL;
    }

    //设置Contact URI
	//string dispalyName = "34020000002000000001";    
	//if (SIP_METHOD_INVITE == stSipMsg.pstSipMethod->usTokenId)
	//{
	//	lResult = SetUriByAddr(stSipMsg.hdlMemCp, stContact.pstContact->stUri, dispalyName.c_str(), objContactAddr);
	//	if (RET_CODE_OK != lResult)
	//	{
	//		//ERROR_LOG("Set Contact URI Failed On Set Contact Header.");
	//		return lResult;
	//	}
	//}
	//else
	//{
	lResult = SetUriByAddr(stSipMsg.hdlMemCp, stContact.pstContact->stUri, NULL, objContactAddr);//lint !e838
	if (RET_CODE_OK != lResult)
	{
		//ERROR_LOG("Set Contact URI Failed On Set Contact Header.");
		return lResult;
	}
	//}

    //Contact DisplayName
	if (SIP_METHOD_INVITE != stSipMsg.pstSipMethod->usTokenId && 0 != strlen(pszDisplayName))
	{
		/*string dispalyName = "34020000001230011";*/
		lResult = CreateSipString(stSipMsg.hdlMemCp, stContact.pstContact->pstrDisplayName, pszDisplayName);
		if (RET_CODE_OK != lResult)
		{
			//ERROR_LOG("Create Contact Display Name Failed On Set Contact Header.");
			return lResult;
		}
	}

    //stContact.bIsStarSet = TRUE;

    return RET_CODE_OK;
}

/**
* Description:  SetExpires().   设置Expires头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  nExpireTime     超时时间
* @return       long.       返回码
*/
long SipStack::SIP::SetExpires
(
    SipMsgS&    stSipMsg,
    SS_UINT32   nExpireTime
)
{
    //创建Expires头域
    SipExpires* pstExpires = (SipExpires*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipExpires));
    if (SS_NULL_PTR == pstExpires)
    {
        //ERROR_LOG("Create Expires Header Failed On Set Expires Header.");
        return RET_CODE_FAIL;
    }

    //设置值
    *pstExpires = nExpireTime;

    //添加扩展头域
    long lResult = AddExHeader(stSipMsg, EX_HDR_ID_EXPIRES, pstExpires);
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Add Expires Header Failed On Set Expires Header.");
        return RET_CODE_FAIL;
    }

    return RET_CODE_OK;
}

/**
* Description:  SetDate(). 设置自定义Date头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  pszDate          时间
* @return       long.       返回码
*/
long SipStack::SIP::SetDate
    (
    SipMsgS&            stSipMsg,
    const char*         pszDate
    )
{
    SipHdrNameValue* sipHdrNameValue = (SipHdrNameValue*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipHdrNameValue));

    long lResult = SipStack::SIP::CreateSipStringData(stSipMsg.hdlMemCp, sipHdrNameValue->strHdrName, "Date");
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Create Date Header Name Failed On Set Date Header.");
        return lResult;
    }

    lResult = SipStack::SIP::CreateSipStringData(stSipMsg.hdlMemCp, sipHdrNameValue->strHdrValue, pszDate);
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Create Date Header Value Failed On Set Date Header.");
        return lResult;
    }

    lResult = SipDsmCloneUnknownHdrToMsg(sipHdrNameValue, &stSipMsg);
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Create Date Header Failed On Set Date Header.");
        return lResult;
    }

    return RET_CODE_OK;
}

/**
* Description:  SetContentEncoding(). 设置自定义Date头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  pszDate          时间
* @return       long.       返回码
*/
long SipStack::SIP::SetContentEncoding
    (
    SipMsgS&            stSipMsg,
    const char*         pszEncoding
    )
{
    SipHdrNameValue* sipHdrNameValue = (SipHdrNameValue*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipHdrNameValue));

    long lResult = SipStack::SIP::CreateSipStringData(stSipMsg.hdlMemCp, sipHdrNameValue->strHdrName, "Content-Encoding");
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Create Date Header Name Failed On Set Date Header.");
        return lResult;
    }

    lResult = SipStack::SIP::CreateSipStringData(stSipMsg.hdlMemCp, sipHdrNameValue->strHdrValue, pszEncoding);
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Create Date Header Value Failed On Set Date Header.");
        return lResult;
    }

    lResult = SipDsmCloneUnknownHdrToMsg(sipHdrNameValue, &stSipMsg);
    if (RET_CODE_OK != lResult)
    {
        ERROR_LOG("Create Date Header Failed On Set Date Header.");
        return lResult;
    }

    return RET_CODE_OK;
}

/**
* Description:  SetServer()     设置服务器头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  pszServer       域服务器
* @return       long.           返回码
*/
long SipStack::SIP::SetServer
(
    SipMsgS&            stSipMsg,
    const char*         pszServer
)
{
    SipHdrNameValue* sipHdrNameValue = (SipHdrNameValue*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipHdrNameValue));

    long lResult = SipStack::SIP::CreateSipStringData(stSipMsg.hdlMemCp, sipHdrNameValue->strHdrName, "Server");

    if(RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Create Date Header Name Failed On Set Server Header.");
        return lResult;
    }

    lResult = SipStack::SIP::CreateSipStringData(stSipMsg.hdlMemCp, sipHdrNameValue->strHdrValue, pszServer);
    if(RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Create Date Header Value Failed On Set Server Header.");
        return lResult;
    }

    lResult = SipDsmCloneUnknownHdrToMsg(sipHdrNameValue, &stSipMsg);
    if(RET_CODE_OK != lResult)
    {
        ERROR_LOG("Create Date Header Failed On Set Server Header.");
        return lResult;
    }

    return RET_CODE_OK;
}

/**
 * Description:  SetEvent(). 设置自定义Event头域
 * @param  [in]  stSipMsg        Sip消息结构体引用
 * @param  [in]  pszEvent         事件
 * @return       long.       返回码
 */
long SipStack::SIP::SetEvent
(
 SipMsgS&         stSipMsg,
 const char*         pszEvent
 )
{
	SipHdrNameValue* sipHdrNameValue = (SipHdrNameValue*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipHdrNameValue));

	long lResult = SipStack::SIP::CreateSipStringData(stSipMsg.hdlMemCp, sipHdrNameValue->strHdrName, "Event");
	if (RET_CODE_OK != lResult)
	{
		//ERROR_LOG("Create Event Header Name Failed On Set Event Header.");
		return lResult;
	}

	lResult = SipStack::SIP::CreateSipStringData(stSipMsg.hdlMemCp, sipHdrNameValue->strHdrValue, pszEvent);
	if (RET_CODE_OK != lResult)
	{
		//ERROR_LOG("Create Event Header Value Failed On Set Event Header.");
		return lResult;
	}

	lResult = SipDsmCloneUnknownHdrToMsg(sipHdrNameValue, &stSipMsg);
	if (RET_CODE_OK != lResult)
	{
		ERROR_LOG("Create Event Header Failed On Set Event Header.");
		return lResult;
	}

	return RET_CODE_OK;
}

/**
* Description:  SetExpires().   设置Expires头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  szSubMediaType       子媒体类型
* @return       long.       返回码
*/
long SipStack::SIP::SetContentType
(
    SipMsgS&    stSipMsg,
    const char* szSubMediaType
)
{
    //创建Content-Type头域
    SipContentType* pstContentType = (SipContentType*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipContentType));
    if (SS_NULL_PTR == pstContentType)
    {
        //ERROR_LOG("Create Content-Type Header Failed On Set Content-Type Header.");
        return RET_CODE_FAIL;
    }

    //Content-Type的MediaType
    SetSipString(pstContentType->strMediaType, MEDIA_TYPE_APPLICATION);
    //Content-Type的SubMediaType
    SetSipString(pstContentType->strSubMediaType, szSubMediaType);

    //添加扩展头域
    long lResult = AddExHeader(stSipMsg, EX_HDR_ID_CONTENT_TYPE, pstContentType);
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Add Content-Type Header Failed On Set Content-Type Header.");
        return RET_CODE_FAIL;
    }

    return RET_CODE_OK;
}

/**
* Description:  SetSubject().   设置Subject头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  strSubject      Subject内容字符串
* @return       long.       返回码
*/
long SipStack::SIP::SetSubject
(
    SipMsgS&        stSipMsg,
    const string&   strSubject
)
{
    if (strSubject.empty())
    {
        //ERROR_LOG("Set Subject Header Failed. Subject Content is Empty.");
        return RET_CODE_PARA_INVALIDATE;
    }
    
    //创建Subject头域
    SipSubject* pstSubject = (SipSubject*)SipMemCpMalloc(stSipMsg.hdlMemCp, sizeof(SipSubject));
    if (SS_NULL_PTR == pstSubject)
    {
        //ERROR_LOG("Create Subject Header Failed On Set Subject Header.");
        return RET_CODE_FAIL;
    }

    //创建Subject
    long lResult = CreateSipStringData(stSipMsg.hdlMemCp, *pstSubject, strSubject.c_str());
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Create Subject Data Failed On Set Subject Header.");
        return RET_CODE_FAIL;
    }

    //添加扩展头域
    lResult = AddExHeader(stSipMsg, EX_HDR_ID_SUBJECT, pstSubject);
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Add Subject Header Failed On Set Subject Header.");
        return RET_CODE_FAIL;
    }

    return RET_CODE_OK;
}



/**
* Description:  GetSubject().   获取Subject头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  szSubject       Subject内容字符串，必须带字符串结束符
* @param  [in]  ulSubjectLen    Subject内容字符串长度，包含字符串结束符
* @return       long.       返回码
*/
long SipStack::SIP::GetSubject
(
    SipMsgS&        stSipMsg,
    char*&          pSubject,
    unsigned long&  ulSubjectLen
)
{

    SipSubject* pstSubject = (SipSubject*)SipDsmGetHdrFromMsg(EX_HDR_ID_SUBJECT, &stSipMsg);
    if (VOS_NULL == pstSubject)
    {
        //ERROR_LOG("Get Subject Header Failed On Get Subject.");
        return RET_CODE_FAIL;
    }

    if (0 == pstSubject->ulLen)
    {
        //ERROR_LOG("Get Subject Failed. Subject Content Length is 0.");
        return RET_CODE_FAIL;
    }

	if(ULONG_MAX-pstSubject->ulLen > 1)
	{
		ulSubjectLen = pstSubject->ulLen + 1;
	}
	else
	{
		ERROR_LOG("pstSubject->ulLen is too large.");
		return RET_CODE_FAIL;
	}

    if (VOS_NULL == VOS_NEW(pSubject, ulSubjectLen))
    {
        //ERROR_LOG("Create Subject Buffer Failed On Get Subject.");
        return RET_CODE_FAIL;
    }
    memset(pSubject, 0, ulSubjectLen);

    strncpy(pSubject, pstSubject->pcData, ulSubjectLen - 1);

    return RET_CODE_OK;
}


/**
* Description:  GetReason().   获取Reason头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  ulReason        ulReason 里面text的错误码
* @return       long.       返回码
*/
long SipStack::SIP::GetReason( SipMsgS& stSipMsg, unsigned long& ulReason )
{
    SipReasonListHdl* pstReasonListHdl = (SipReasonListHdl*)SipDsmGetHdrFromMsg(EX_HDR_ID_REASON, &stSipMsg);
    if (VOS_NULL == pstReasonListHdl)
    {
        //INFO_LOG("SipStack::SIP::GetReason Get Reason Header SipReasonListHdl Failed On Get Reason.");//获取不到Reason正常，因为这个函数每次都会调用
        return RET_CODE_FAIL;
    }

    const char* pszText="text";
    SipString stPar;
    stPar.pcData = (char*)pszText;//lint !e1773
    stPar.ulLen = strlen(pszText);

    SipTokenGenParams  *pstReason;
    SS_UINT16 usCount = SS_NULL_UINT16;
    SS_INT iTemp =0;
    SipGenParNameValue  *pstNameValue = SS_NULL_PTR;
    
    usCount = (SS_UINT16)VppListGetCount(pstReasonListHdl);//lint !e838
    for (iTemp = 0; iTemp < usCount; iTemp++)//lint !e838
    {
        if (VPP_SUCCESS != VppListGetData(pstReasonListHdl, (SS_UINT16)iTemp,
            (SS_VOID**)&pstReason))
        {
            SIP_RETURN(SS_NULL_PTR)
        }
        else
        {
            pstNameValue = SipDsmGetGenericParamByName(pstReason->hdlGenericParamList, &stPar);  //这个就获取到了 Text,里面包括引号
            if(NULL != pstNameValue)//找到了
            {
                char* pText = NULL;
				unsigned int uiLen = pstNameValue->pstrGenParValue->ulLen;
                if(NULL == VOS_NEW(pText,uiLen))
                {
                    //ERROR_LOG("SipStack::SIP::GetReason new pText failed.");
                    return RET_CODE_FAIL;
                }

                memset(pText,0,uiLen);
                memcpy(pText,pstNameValue->pstrGenParValue->pcData+1,uiLen);
                ulReason = (unsigned long)strtol(pText,NULL,10);//10进制，转换为正数
                VOS_DELETE(pText,uiLen);

                INFO_LOG("SipStack::SIP::GetReason Get Text Reason:%d",ulReason);//获取不到Reason正常，因为这个函数每次都会调用

                return RET_CODE_OK; 
            }
        }
        
    }

    //ERROR_LOG("SipStack::SIP::GetReason get Reason text failed.");
    return RET_CODE_FAIL;

}

/**
* Description:  ResetVarHdr().  重置可变头域
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [in]  ulSequence      CSeq值
* @param  [in]  pszFromTag      From的Tag
* @param  [in]  pszToTag        To的Tag
* @return       long.       返回码
*/
long SipStack::SIP::ResetVarHdr
(
    SipMsgS&        stSipMsg,   
    unsigned long   ulSequence,
    const std::string& strFromTag,
    const char*     pszToTag
)
{
    //Minisip函数返回值
    EN_SIP_RESULT enResult = SIP_RET_SUCCESS;
    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;
    //返回值
    long lResult = RET_CODE_OK;

    //CSeq
    stSipMsg.stHeaders.pstCseq->ulSequence = ulSequence;

    //From Tag
    if (strFromTag.empty())
    {
        enResult = SipTxnUtilGenTag(stSipMsg.hdlMemCp, &stSipMsg.stHeaders.pstFrom->strTag);
        if (SIP_RET_SUCCESS != enResult)
        {
            //ERROR_LOG("Create From Tag Failed On Reset Variable Header. Error = %s.", STR_ARR_EN_SIP_RESULT[enResult]);
            return RET_CODE_FAIL;
        }
    }
    else
    {
        lResult = CreateSipStringData(stSipMsg.hdlMemCp, stSipMsg.stHeaders.pstFrom->strTag, strFromTag.c_str());
        if (RET_CODE_OK != lResult)
        {
            //ERROR_LOG("Set From Tag Failed On Reset Variable Header.");
            return RET_CODE_FAIL;
        }
    }

    //To Tag
    if (VOS_NULL != pszToTag)
    {
        lResult = CreateSipStringData(stSipMsg.hdlMemCp, stSipMsg.stHeaders.pstTo->strTag, pszToTag);
        if (RET_CODE_OK != lResult)
        {
            //ERROR_LOG("Set To Tag Failed On Reset Variable Header.");
            return RET_CODE_FAIL;
        }
    }

    //Via
    SipVia* pstSipVia = NULL;
    nResult = VppListGetData(stSipMsg.stHeaders.hdlViaList, 0, (void**)&pstSipVia);//lint !e838
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("Get Via Header Failed On Reset Variable Header. ErrorCode = %d.", nResult);
        return RET_CODE_FAIL;
    }

    //Via Branch
    enResult = SipTxnUtilGenBranchId(stSipMsg.hdlMemCp, &pstSipVia->strBranch);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("Create Branch Failed On Reset Variable Header. Error = %s.", STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

    return RET_CODE_OK;
}//lint !e1764

/**
* Description:  SetMsgBody().   设置消息体
* @param  [in]  stSipMsg                 Sip消息结构体引用
* @param  [in]  szSubMediaType       子媒体类型
* @param  [in]  ulMsgBodyLen        消息体长度
* @param  [in]  szMsgBody           消息体字符串
* @return       long.       返回码
*/
long SipStack::SIP::SetMsgBody
(
    SipMsgS&        stSipMsg,
    const char*     szSubMediaType,
    unsigned long   ulMsgBodyLen,
    char*           szMsgBody
)
{
    //无消息体，直接返回
    if (VOS_NULL == szMsgBody || 0 == ulMsgBodyLen)
    {
        //ERROR_LOG("Message Body is Empty On Set Message Body.");
        return RET_CODE_PARA_INVALIDATE;
    }

    //消息体过大
    if (MAX_LEN_UDP_PACKET < ulMsgBodyLen)
    {
        //ERROR_LOG("Message Body Length(%d) is Too Big On Set Message Body.", ulMsgBodyLen);
        return RET_CODE_FAIL;
    }

    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;

    //创建消息体
    nResult = VppStrBufCreate(stSipMsg.hdlMemCp, (VPP_UINT16)ulMsgBodyLen, szMsgBody, ulMsgBodyLen, &stSipMsg.hdlBody);//lint !e838
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("Create String Buffer Failed On Set Message Body. ErrorCode = %d.", nResult);
        return RET_CODE_FAIL;
    }

    //ContentLength
    SipContentLength* pstContentLength = (SipContentLength*)SipDsmGetHdrFromMsg(SIP_BASIC_HDR_ID_CONTENT_LENGTH, &stSipMsg);
    if (VOS_NULL == pstContentLength)
    {
        pstContentLength = (SipContentLength*)SipDsmCreateHdrInMsg(SIP_BASIC_HDR_ID_CONTENT_LENGTH, &stSipMsg);
    }

    //重新赋值
	if (VOS_NULL != pstContentLength)
	{
		*pstContentLength = ulMsgBodyLen;
	}

    //设置Content-Type头域
    long lResult = SetContentType(stSipMsg, szSubMediaType);
    if (RET_CODE_OK != lResult)
    {
        //ERROR_LOG("Set Content-Type Header Failed On Set Message Body.");
        return RET_CODE_FAIL;
    }

    return RET_CODE_OK;
}

/**
* Description:  GetPlatAuthInfo().   获取平台鉴权信息
* @param  [in]  stSipMsg        SIP消息引用
* @param  [out] stPaltAuthInfo  获取的平台鉴权信息
* @return       long.       返回码
*/
long SipStack::SIP::GetPlatAuthInfo
(
    SipMsgS&        stSipMsg,
    PLAT_AUTH_INFO& stPaltAuthInfo
)
{
    SipWWWAuthenticateListHdl stWwwAuthListHdl = SipDsmGetHdrFromMsg(EX_HDR_ID_WWW_AUTHENTICATE, &stSipMsg);
    if (VOS_NULL == stWwwAuthListHdl)
    {
        ERROR_LOG("Get WWW-Authenticate Header Failed On Get Plat Authentication Info.");
        return RET_CODE_FAIL;
    }

    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;

    SipWWWAuthenticate* pstSipWwwAuth = NULL;
    
    nResult = VppListGetData(stWwwAuthListHdl, 0, (void**)&pstSipWwwAuth);//lint !e838
    if (VPP_SUCCESS != nResult)
    {
        ERROR_LOG("Get WWW-Authenticate Header Value On Get Plat Authentication Info. ErrorCode = %d.", nResult);
        return RET_CODE_FAIL;
    }

    //strNonce strOpaque strRealm strAlgorithm四个字段都需存在
    if (VOS_NULL == pstSipWwwAuth->pstrNonce)
    {
        ERROR_LOG("Get Plat Authentication Info Failed. Nonce is Empty.");
        return RET_CODE_FAIL;
    }

    //if (VOS_NULL == pstSipWwwAuth->pstrOpaque)
    //{
        //ERROR_LOG("Get Plat Authentication Info Failed. Opaque is Empty.";
        //return RET_CODE_FAIL;
    //}

    if (VOS_NULL == pstSipWwwAuth->pstrRealm)
    {
        ERROR_LOG("Get Plat Authentication Info Failed. Realm is Empty.");
        return RET_CODE_FAIL;
    }

    if (VOS_NULL == pstSipWwwAuth->pstrAlgorithm)
    {
		ERROR_LOG("Get Plat Authentication Info Failed. Algorithm is Empty.");
		return RET_CODE_FAIL;
    }

    //去掉双引号
    pstSipWwwAuth->pstrNonce->pcData[pstSipWwwAuth->pstrNonce->ulLen - 1]   = '\0';
	if (VOS_NULL != pstSipWwwAuth->pstrOpaque)
	{
		pstSipWwwAuth->pstrOpaque->pcData[pstSipWwwAuth->pstrOpaque->ulLen - 1] = '\0';
	}
    pstSipWwwAuth->pstrRealm->pcData[pstSipWwwAuth->pstrRealm->ulLen - 1]   = '\0';

    //检测鉴权信息是否与本地的相同
    if (   0 == strncmp(stPaltAuthInfo.strNonce.c_str(),  pstSipWwwAuth->pstrNonce->pcData + 1,  pstSipWwwAuth->pstrNonce->ulLen - 2)
        //&& 0 == strncmp(stPaltAuthInfo.strOpaque.c_str(), pstSipWwwAuth->pstrOpaque->pcData + 1, pstSipWwwAuth->pstrOpaque->ulLen - 2)
        && 0 == strncmp(stPaltAuthInfo.strRealm.c_str(),  pstSipWwwAuth->pstrRealm->pcData + 1,  pstSipWwwAuth->pstrRealm->ulLen - 2))
    {
        WARN_LOG("Get Plat Authentication Info Failed. Authentication Failed Because Challege Info is The Same.");
        return RET_CODE_OK;//修改问题单 DTS2011070502986 ，防止中途被修改，改为用次数限制
    }

    //计算buffer的最大长度
    unsigned long ulBufLen = pstSipWwwAuth->pstrNonce->ulLen;
	if(VOS_NULL != pstSipWwwAuth->pstrOpaque)
	{
		ulBufLen = ulBufLen > pstSipWwwAuth->pstrOpaque->ulLen ? ulBufLen : pstSipWwwAuth->pstrOpaque->ulLen;
	}
    ulBufLen = ulBufLen > pstSipWwwAuth->pstrRealm->ulLen  ? ulBufLen : pstSipWwwAuth->pstrRealm->ulLen;
	if (VOS_NULL != pstSipWwwAuth->pstrQOPOptions)
	{
		ulBufLen = ulBufLen > pstSipWwwAuth->pstrQOPOptions->ulLen  ? ulBufLen : pstSipWwwAuth->pstrQOPOptions->ulLen;
	}    
    ulBufLen += 1;//加上结束符的长度

    char* pBuffer = VOS_NEW(pBuffer, ulBufLen);
    if (VOS_NULL == pBuffer)
    {
        ERROR_LOG("Create Buffer Failed On Get Plat Authentication Info.");
        return RET_CODE_FAIL;
    }

    //必须带字符串结束符
    memset(pBuffer, 0, ulBufLen);
    strncpy(pBuffer, pstSipWwwAuth->pstrNonce->pcData + 1, pstSipWwwAuth->pstrNonce->ulLen - 2);
    stPaltAuthInfo.strNonce = pBuffer;
	if(VOS_NULL != pstSipWwwAuth->pstrOpaque)
	{
		memset(pBuffer, 0, ulBufLen);
		strncpy(pBuffer, pstSipWwwAuth->pstrOpaque->pcData + 1, pstSipWwwAuth->pstrOpaque->ulLen - 2);
		stPaltAuthInfo.strOpaque = pBuffer;
	}
	else
	{
		stPaltAuthInfo.strOpaque = "";
	}

    memset(pBuffer, 0, ulBufLen);
    strncpy(pBuffer, pstSipWwwAuth->pstrRealm->pcData + 1, pstSipWwwAuth->pstrRealm->ulLen - 2);
    stPaltAuthInfo.strRealm = pBuffer;

    memset(pBuffer, 0, ulBufLen);
    strncpy(pBuffer, pstSipWwwAuth->pstrAlgorithm->pcData, pstSipWwwAuth->pstrAlgorithm->ulLen);
    stPaltAuthInfo.stAlgorithm = pBuffer;

	//检测Qop字段
	if(VOS_NULL == pstSipWwwAuth->pstrQOPOptions)
	{
		WARN_LOG("Get Plat Authentication Info Failed. QOPOptions is Empty.");
		stPaltAuthInfo.strQop = "";
	}
	else
	{
		memset(pBuffer, 0, ulBufLen);
		strncpy(pBuffer, pstSipWwwAuth->pstrQOPOptions->pcData, pstSipWwwAuth->pstrQOPOptions->ulLen);
		stPaltAuthInfo.strQop = pBuffer;	
	}

    //释放buffer
    VOS_DELETE(pBuffer, MULTI);

    return RET_CODE_OK;
}

/**
* Description:  GetRedirectInfo().  获取重定向信息
* @param  [in]  stSipMsg            SIP消息引用
* @param  [out] vectorServerInfo    服务器信息向量
* @return       long.       返回码
*/
long SipStack::SIP::GetRedirectInfo
(
    SipMsgS&            stSipMsg,
    VECTOR_SERVER_INFO& vectorServerInfo
)
{
    //清空原有信息
    vectorServerInfo.clear();
    
    SipContactListHdl stContactListHdl = SipDsmGetHdrFromMsg(SIP_BASIC_HDR_ID_CONTACT_LIST, &stSipMsg);
    if (VOS_NULL == stContactListHdl)
    {
        //ERROR_LOG("Get Contact Header Failed On Get Redirect Info.");
        return RET_CODE_FAIL;
    }

    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;

    SipContact* pstSipContact = NULL;

    unsigned long ulContactNum = VppListGetCount(stContactListHdl);

    //遍历设置Contact列表
    for (unsigned long ulIndex = 0; ulIndex < ulContactNum; ++ulIndex)
    {
        nResult = VppListGetData(stContactListHdl, (VPP_UINT16)ulIndex, (void**)&pstSipContact);
        if (VPP_SUCCESS != nResult)
        {
            //ERROR_LOG("Get Contact Header Value On Get Redirect Info. ErrorCode = %d.", nResult);
            return RET_CODE_FAIL;
        }
        
        SS_UCHAR* pIp = pstSipContact->pstContact->stUri.uri.pstSipUri->stHostPort.stHost.uHostContent.ipv4;

        //构造函数中初始化
        SERVER_INFO stServerInfo;
		(void)snprintf(stServerInfo.szServerIP,MAX_LEN_IP,"%d.%d.%d.%d", pIp[0], pIp[1], pIp[2], pIp[3]);
        stServerInfo.usServerPort = (unsigned short)pstSipContact->pstContact->stUri.uri.pstSipUri->stHostPort.iPort;

        vectorServerInfo.push_back(stServerInfo);
    }
    
    return RET_CODE_OK;
}

/**
* Description:  GetPlatAuthInfo().   获取注册超时时间
* @param  [in]  stSipMsg        SIP消息引用
* @param  [out] ulExpires       获取的注册超时时间
* @return       long.       返回码
*/
long SipStack::SIP::GetExpires
(
    SipMsgS&        stSipMsg,
    unsigned long&  ulExpires
)
{
    SipExpires* stExpires = (SipExpires*)SipDsmGetHdrFromMsg(EX_HDR_ID_EXPIRES, &stSipMsg);
    if (VOS_NULL == stExpires)
    {
        //WARN_LOG("Get Expires Header Failed On Get Expires.");
        return RET_CODE_FAIL;
    }

    ulExpires = *stExpires;

    return RET_CODE_OK;
}

/**
* Description:  GetContact().   获取设备网络地址信息
* @param  [in]  stSipMsg        SIP消息引用
* @param  [out] devIP              获取设备IP
* @param  [out] devPort          获取设备Port
* @return       long.       返回码
*/
long SipStack::SIP::GetContact
(
 SipMsgS&        stSipMsg,
 string&          devIP,
 unsigned short&  devPort
 )
{
	SipContactListHdl* pContactListl = (SipContactListHdl*)SipDsmGetHdrFromMsg(SIP_BASIC_HDR_ID_CONTACT_LIST, &stSipMsg);
	if (VOS_NULL == pContactListl)
	{
		WARN_LOG("SipStack::SIP::GetContact Get Contact Header SipContactListHdl Failed On Get ContactList.");
		return RET_CODE_FAIL;
	}

	SipContact* pstContact;
	if (VPP_SUCCESS != VppListGetData(pContactListl, 0,(SS_VOID**)&pstContact))
	{
		SIP_RETURN(SS_NULL_PTR)
	}
	else
	{
		SipURI *pstSipUri = pstContact->pstContact->stUri.uri.pstSipUri;
		if (VOS_NULL == pstSipUri)
		{
			ERROR_LOG("Get Contact URI Info Failed. SIP URI is Empty.");
			return RET_CODE_FAIL;
		}

		SipHostPort& sipHostPort = pstSipUri->stHostPort;
		ACE_INET_Addr addr;
		devPort = (unsigned short&)sipHostPort.iPort;
		if (SIP_ADDR_TYPE_IPV4 == sipHostPort.stHost.enHostType)
		{
			SS_UINT8 pIP4 [SS_IPV4_ADDR_LEN];
			for (int i = 0;i<SS_IPV4_ADDR_LEN;i++)
			{
				pIP4[i] = sipHostPort.stHost.uHostContent.ipv4[(SS_IPV4_ADDR_LEN-1)-i];
			}
			addr.set_address((char*)pIP4, SS_IPV4_ADDR_LEN);
		}
		else if (SIP_ADDR_TYPE_IPV6 == sipHostPort.stHost.enHostType)
		{
			//SS_UINT8 pIP6[SS_IPV6_ADDR_LEN];
			//for (int i = 0; i<SS_IPV6_ADDR_LEN; i++)
			//{
			//	pIP6[i] = sipHostPort.stHost.uHostContent.ipv6[(SS_IPV6_ADDR_LEN-1)-i];
			//}
			addr.set_address((char*)sipHostPort.stHost.uHostContent.ipv6, SS_IPV6_ADDR_LEN);
		} 
		else
		{
			ERROR_LOG("Get Contact URI's IP Info Failed. SIP URI is Empty.");
			return RET_CODE_FAIL;
		}
		const char* chDevIP = addr.get_host_addr();
		if(NULL != chDevIP)
		{
			devIP = chDevIP;
			//INFO_LOG("SipStack::SIP::GetContact get Contact text scuessfully.");
			return RET_CODE_OK;
		}
		else
		{
			ERROR_LOG("get_host_addr Failed.");
			return RET_CODE_FAIL;
		}
	}
}

/**
* Description:  GetDate().   获取Date时间
* @param  [in]  stSipMsg        SIP消息引用
* @param  [out] dateTime          时间
* @return       long.       返回码
*/
long SipStack::SIP::GetDate
(
    SipMsgS&        stSipMsg,
	string&       dateTime
)
{
     return RET_CODE_OK;
}//lint !e1764

/**
* Description:  GetCurrentTime().   获取系统当前时间
* @return       long.       返回码
*/
string SipStack::SIP::GetCurrentTime()
{  
	time_t oCurrTime;
	struct tm *oTmCurrTime;

	ACE_OS::time( &oCurrTime );
	struct tm t_tmTmp;
	oTmCurrTime = ACE_OS::localtime_r( &oCurrTime,&t_tmTmp );

	// szCurrTime 保存当前时间的数组 形式是:2005-06-07 15:36:41
	char szCurrTime[32] = "\0";//lint !e840
	ACE_OS::strftime( szCurrTime, 32,"%Y-%m-%dT%H:%M:%S", oTmCurrTime );

	return string(szCurrTime);
}

/**
* Description:  GetNextNonce().     获取NextNonce
* @param  [in]  stSipMsg        SIP消息引用
* @param  [out] strNextNonce       获取的NextNonce
* @return       long.       返回码
*/
long SipStack::SIP::GetNextNonce
(
    SipMsgS&    stSipMsg,
    string&     strNextNonce
)
{
    SipAuthenticationInfoListHdl stAuthInfoListHdl = (SipAuthenticationInfoListHdl*)SipDsmGetHdrFromMsg(EX_HDR_ID_AUTHENTICATION_INFO, &stSipMsg);
    if (VOS_NULL == stAuthInfoListHdl)
    {
        INFO_LOG("Get Authentication-Info Header Failed On Get Next Nonce.");
        return RET_CODE_FAIL;
    }

    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;

    SipAuthenticationInfoS* pstSipAuthInfo = NULL;
    
    nResult = VppListGetData(stAuthInfoListHdl, 0, (void**)&pstSipAuthInfo);//lint !e838
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("Get Authentication-Info Header Value On Get Next Nonce. ErrorCode = %d.", nResult);
        return RET_CODE_FAIL;
    }

    SipString* pstNextNonce = pstSipAuthInfo->pstrNextNonce;
    if (VOS_NULL == pstNextNonce)
    {
        INFO_LOG("Get Next Nonce Failed. Next Nonce is Empty.");
        return RET_CODE_FAIL;
    }

    if (0 == pstNextNonce->ulLen)
    {
        //ERROR_LOG("Get Next Nonce Failed. Next Nonce Length is 0.");
        return RET_CODE_FAIL;
    }

    //去掉最后的引号，替换为结束符
    pstNextNonce->pcData[pstNextNonce->ulLen - 1] = '\0';

    //输出NextNonce，去掉前面的引号
    strNextNonce = pstNextNonce->pcData + 1;
    return RET_CODE_OK;
}

/**
* Description:  GetMsgBody().   获取消息体内容
* @param  [in]  stSipMsg        SIP消息引用
* @param  [out] pMsgBody       消息体指针
* @param  [out] ulMsgBodyLen       消息体内容长度
* @return       long.       返回码
*/
long SipStack::SIP::GetMsgBody
(
    SipMsgS&        stSipMsg,
    char*&          pMsgBody,
    unsigned long&  ulMsgBodyLen
)
{
    //获取消息体
    SipBody stBody = SipDsmGetBody(&stSipMsg);
    unsigned int nBodyLen = VppStrBufGetLength(stBody);
    char* pBody = NULL;

	if (0 == nBodyLen)
    {
        ERROR_LOG("Get Message Body Failed. Message Body Length (%d)Must be Greater than 0.",nBodyLen);
        return RET_CODE_FAIL;
    }

    if (VOS_NULL == VOS_NEW(pBody, nBodyLen+1))
    {
        ERROR_LOG("Create Message Body Buffer(length = %d) Failed On Get Message Body.",nBodyLen);
        return RET_CODE_FAIL;
    }

    memset(pBody, 0, nBodyLen+1);
    VPP_UINT32 nResult = VppStrBufCopyDataOut(stBody, 0, pBody, &nBodyLen);
    if (VPP_SUCCESS != nResult)
    {
        ERROR_LOG("Copy Data Out Failed On Get Message Body.");
		VOS_DELETE(pBody,MULTI);
        return RET_CODE_FAIL;
    }

    //保存输出
    pMsgBody        = pBody;
    ulMsgBodyLen    = nBodyLen;

    return RET_CODE_OK;
}

/**
* Description:  CopyHeader().   从SIP消息中拷贝头域
* @param  [in]  usHeaderId      头域ID
* @param  [in]  stInSipMsg      拷出的SIP消息引用
* @param  [out] stOutSipMsg     拷入的SIP消息引用
* @return       long.       返回码
*/
long SipStack::SIP::CopyHeader
(
    unsigned short  usHeaderId,
    SipMsgS&        stInSipMsg,
    SipMsgS&        stOutSipMsg
)
{
    //Minisip函数返回值
    EN_SIP_RESULT enResult = SIP_RET_SUCCESS;
    
    SS_VOID* pvHdr = SipDsmGetHdrFromMsg(usHeaderId, &stInSipMsg);
    if (VOS_NULL == pvHdr)
    {
        //ERROR_LOG("Get Header Value Failed On Copy Header.");
        return RET_CODE_FAIL;
    }

    enResult = SipDsmCopyHdrToMsg(usHeaderId, &stOutSipMsg, pvHdr);//lint !e838
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipDsmCopyHdrToMsg Message Failed On Copy Header. Error = %s.", 
            //STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }
        
    return RET_CODE_OK;
}

/**
* Description:  GetDialogInfo().  获取对话信息
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [out]  stDiagInfo     输出的对话信息
* @param  [in]  bFromLocal      消息是否来自本地
* @return       long.       返回码
*/
long SipStack::SIP::GetDialogInfo
(
    SipMsgS&        stSipMsg,
    DIALOG_INFO&    stDiagInfo,
    VOS_BOOLEAN     bFromLocal
)
{
    SipCSeq* pstCSeq = (SipCSeq*)SipDsmGetHdrFromMsg(SIP_BASIC_HDR_ID_CSEQ, &stSipMsg);
    if (VOS_NULL == pstCSeq)
    {
        ERROR_LOG("dsm header - cseq struct is null");
        return RET_CODE_FAIL;
    }
    stDiagInfo.ulCSeq = pstCSeq->ulSequence; //设置stDiagInfo中的ulCSeq

    SipFrom* pstFrom = (SipFrom*)SipDsmGetHdrFromMsg(SIP_BASIC_HDR_ID_FROM, &stSipMsg);
    if (VOS_NULL == pstFrom)
    {
        ERROR_LOG("dsm header - from struct is null");
        return RET_CODE_FAIL;
    }

    SipTo* pstTo = (SipTo*)SipDsmGetHdrFromMsg(SIP_BASIC_HDR_ID_TO, &stSipMsg);
    if (VOS_NULL == pstTo)
    {
        ERROR_LOG("dsm header - to struct is null");
        return RET_CODE_FAIL;
    }

    SipCallID* pstCallId = (SipCallID*)SipDsmGetHdrFromMsg(SIP_BASIC_HDR_ID_CALLID, &stSipMsg);
    if (VOS_NULL == pstCallId)
    {
        ERROR_LOG("dsm header - call id struct is null");
        return RET_CODE_FAIL;
    }

    //三个字段都需存在
    if (0 == pstFrom->strTag.ulLen || 0 == pstTo->strTag.ulLen || 0 == pstCallId->ulLen )
    {
		ERROR_LOG("dsm header - invalid tag or call id - From_Tag_Len=%d,To_Tag_Len=%d,Call_ID_Tag_Len=%d.",
			pstFrom->strTag.ulLen,
			pstTo->strTag.ulLen,
			pstCallId->ulLen);
        return RET_CODE_FAIL;
    }

    //计算buffer的最大长度
    unsigned long ulBufLen = pstFrom->strTag.ulLen;
    ulBufLen = ulBufLen > pstTo->strTag.ulLen ? ulBufLen : pstTo->strTag.ulLen;
    ulBufLen = ulBufLen > pstCallId->ulLen    ? ulBufLen : pstCallId->ulLen;

    //加上结束符的长度
    ulBufLen += 1;

    char* pBuffer = VOS_NEW(pBuffer, ulBufLen);
    if (VOS_NULL == pBuffer)
    {
        //ERROR_LOG("Create Buffer Failed On Get Dialog Info.");
        return RET_CODE_FAIL;
    }

    //默认来自本地
    string* pstrFromTag = &stDiagInfo.strLocalTag;
    string* pstrToTag   = &stDiagInfo.strRemoteTag;

    //来自远端，则交换Local和Remote的Tag
    if (!bFromLocal)
    {
        pstrFromTag = &stDiagInfo.strRemoteTag;
        pstrToTag   = &stDiagInfo.strLocalTag;
    }

    //必须带字符串结束符
    memset(pBuffer, 0, ulBufLen);
    strncpy(pBuffer, pstFrom->strTag.pcData, pstFrom->strTag.ulLen);
    *pstrFromTag = pBuffer;

    memset(pBuffer, 0, ulBufLen);
    strncpy(pBuffer, pstTo->strTag.pcData, pstTo->strTag.ulLen);
    *pstrToTag = pBuffer;

    memset(pBuffer, 0, ulBufLen);
    strncpy(pBuffer, pstCallId->pcData, pstCallId->ulLen);
    stDiagInfo.strCallId = pBuffer;

    //释放buffer
    VOS_DELETE(pBuffer, MULTI);

    return RET_CODE_OK;
}

/**
* Description:  GetFromTag().  获取From的Tag
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [out]  strFromTag     输出的From的Tag
* @return       long.       返回码
*/
long SipStack::SIP::GetFromTag
(
    SipMsgS&    stSipMsg,
    string&     strFromTag
)
{

    SipFrom* pstFrom = (SipFrom*)SipDsmGetHdrFromMsg(SIP_BASIC_HDR_ID_FROM, &stSipMsg);
    if (VOS_NULL == pstFrom)
    {
        //ERROR_LOG("Get From Header Failed On Get From Tag.");
        return RET_CODE_FAIL;
    }

    if (0 == pstFrom->strTag.ulLen)
    {
        //ERROR_LOG("Get From Tag Failed. From Tag is Empty.");
        return RET_CODE_FAIL;
    }

    //计算buffer的最大长度，加上结束符的长度
    unsigned long ulBufLen = pstFrom->strTag.ulLen + 1;

    char* pBuffer = VOS_NEW(pBuffer, ulBufLen);
    if (VOS_NULL == pBuffer)
    {
        //ERROR_LOG("Create Buffer Failed On Get From Tag.");
        return RET_CODE_FAIL;
    }

    //必须带字符串结束符
    memset(pBuffer, 0, ulBufLen);
    strncpy(pBuffer, pstFrom->strTag.pcData, pstFrom->strTag.ulLen);
    strFromTag = pBuffer;

    //释放buffer
    VOS_DELETE(pBuffer, MULTI);

    return RET_CODE_OK;
}

/**
* Description:  GetFromUriInfo().  获取From的Uri信息
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [out]  stFromUri     输出的From的Uri信息
* @return       long.       返回码
*/
long SipStack::SIP::GetFromUriInfo
(
    SipMsgS&        stSipMsg,
    PEER_URI_INFO&  stFromUri
)
{
    SipFrom* pstFrom = (SipFrom*)SipDsmGetHdrFromMsg(SIP_BASIC_HDR_ID_FROM, &stSipMsg);
    if (VOS_NULL == pstFrom)
    {
        ERROR_LOG("Get From Header Failed On Get From URI Info.");
        return RET_CODE_FAIL;
    }

    if (VOS_NULL == pstFrom->stUri.uri.pstSipUri)
    {
        ERROR_LOG("Get From URI Info Failed. SIP URI is Empty.");
        return RET_CODE_FAIL;
    }

    SipURI& stSipUri = *pstFrom->stUri.uri.pstSipUri;

    //保存UserName
    if (VOS_NULL == stSipUri.pstrUserName)
    {
        ERROR_LOG("Get From URI Info Failed. SIP URI User Name is Empty.");
        return RET_CODE_FAIL;
    }

    //取较小的长度
    unsigned long ulUserNameLen = sizeof(stFromUri.szUriUserName) - 1;
    ulUserNameLen = ulUserNameLen > stSipUri.pstrUserName->ulLen ? stSipUri.pstrUserName->ulLen : ulUserNameLen;
    strncpy(stFromUri.szUriUserName, stSipUri.pstrUserName->pcData, ulUserNameLen);

	INFO_LOG("Get From URI Info Success. SIP URI User Name= %s",stFromUri.szUriUserName);
    return RET_CODE_OK;
}
/**
* Description:  GetFromValue().  获取From的值
* @param  [in]  stSipMsg        Sip消息结构体引用
* @param  [out]  pszFromValue   输出的From的Value，里面会分配该内存
* @return       long.       返回码
*/
/*
long SipStack::SIP::GetFromValue
(
    SipMsgS&    stSipMsg,
    char*&      pszFromValue
)
{
    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;
    
    SipFrom* pstFrom = (SipFrom*)SipDsmGetHdrFromMsg(SIP_BASIC_HDR_ID_FROM, &stSipMsg);
    if (VOS_NULL == pstFrom)
    {
        //ERROR_LOG("Get From Header Failed On Get From Value.");
        return RET_CODE_FAIL;
    }
    
    SipStrBufHdl hdlHdrBuffer = NULL;
    nResult = VppStrBufCreate(NULL, MAX_LEN_REQUEST_URI, NULL, 0, &hdlHdrBuffer);
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("VppStrBufCreate Failed On Get From Value. ErrorCode = 0x%04X.", nResult);
        return RET_CODE_FAIL;
    }

    EN_SIP_RESULT enResult = SipEncHeader(SIP_BASIC_HDR_ID_FROM, pstFrom, hdlHdrBuffer);
    if (SIP_RET_SUCCESS != enResult)
    {
        //ERROR_LOG("SipEncHeader Failed On Get From Value. Error = %s.", STR_ARR_EN_SIP_RESULT[enResult]);
        return RET_CODE_FAIL;
    }

    VPP_UINT32 nFromValueLen = VppStrBufGetLength(hdlHdrBuffer);
    if (0 == nFromValueLen)
    {
        //ERROR_LOG("VppStrBufGetLength Failed On Get From Value.");
        return RET_CODE_FAIL;
    }

    char* pFromValue = VOS_NEW(pFromValue, nFromValueLen + 1);
    memset(pFromValue, 0, nFromValueLen + 1);

    nResult = VppStrBufCopyDataOut(hdlHdrBuffer, 0, pFromValue, &nFromValueLen);
    if (VPP_SUCCESS != nResult)
    {
        //ERROR_LOG("VppStrBufCopyDataOut Failed On Get From Value. ErrorCode = 0x%04X.", nResult);
        return RET_CODE_FAIL;
    }

    VppStrBufDestroy(&hdlHdrBuffer);

    //输出指针
    pszFromValue = pFromValue;

    //INFO_LOG("Get From Value Success. From Value: %s.", pszFromValue);
    return RET_CODE_OK;
}
*/

//}}//end namespace

//lint +e818
//lint +e438

