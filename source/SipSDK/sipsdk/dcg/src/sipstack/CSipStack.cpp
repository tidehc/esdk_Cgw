#include <cerrno>
#include <iostream>
#include <ctime>
#include <algorithm>
#include "tinyxml.h"
#include "vos_config.h"
#include "digcalc.h"
#include "CLockGuard.h"
#include "sipstack_msg_ctrl.h"
#include "sipstack_msg_wrap.hpp"
#include "sipstack_task.h"
#include "sipstack_agent_server_impl.hpp"
#include "CSipStack.hpp"
#include "Log.h"

#ifdef WIN32
#pragma comment(lib, "ws2_32.lib")
#else
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#ifndef TRUE
#define TRUE true
#endif

#ifndef CONN_ERRNO
#define CONN_ERRNO errno
#endif


#define RESEND_INTERVAL             (10)

#pragma warning(push)
#pragma warning(disable:4996)

//lint -e429
namespace SipStack{

//初始化全局SIP协议栈指针
CSipStack* g_pSipStack = NULL;

/**
* Description:  触发定时事件处理
* @param  [in]  pArg        具体处理类
* @param  [in]  ullScales   定时器指针
* @param  [in]  enStyle     触发类型
*/
void CSipStackTimerTrigger::OnTrigger
(
    void *pArg, 
    ULONGLONG ullScales, 
    TriggerStyle enStyle
)
{
    //INFO_LOG("CSipTimerTrigger::onTrigger.");

    CSipStack* pSipStack = VOS_CAST((CSipStack*)pArg);

    if (NULL == pSipStack)
    {
        ERROR_LOG("timer trigger - on trigger - failure to dync-cast sip stack class - pArg=%p.", pArg);
        return ;
    }

    SIP::EVENT_INFO stEventInfo(SIP::EVENT_TYPE_STACK_TIMER_TRIGGER);
    //添加定时器触发事件
    long lResult = g_pSipStack->AddEvent(stEventInfo);
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("timer trigger - on trigger - failure to add event for timer = Ret=%d.", lResult);
        return ;
    }

    //下述参数不需处理
    ullScales   = ullScales;
    enStyle     = enStyle;
}//lint !e438

/**
* Description:  触发定时事件处理
* @param  [in]  pArg        具体处理类
* @param  [in]  ullScales   定时器指针
* @param  [in]  enStyle     触发类型
*/
void CSipStackMsgTimerTrigger::OnTrigger
(
    void *pArg, 
    ULONGLONG ullScales, 
    TriggerStyle enStyle
)
{
    //INFO_LOG("CSipTimerTrigger::onTrigger.");

    CSipStack* pSipStack = VOS_CAST((CSipStack*)pArg);

    //ERROR_LOG("Enter CSipMsgTimerTrigger::onTrigger";

    if (NULL == pSipStack)
    {
        ERROR_LOG("timer trigger - on trigger - failure to dync-cast sip stack class - ");
        return ;
    }

    pSipStack->CheckReqMsg();

    //下述参数不需处理
    ullScales   = ullScales;
    enStyle     = enStyle;
}//lint !e438

/**
* Description:    CSipUdpHandle(). Default constructor
*/
CSipUdpHandle::CSipUdpHandle()
{
}

/**
* Description:    ~CSipUdpHandle(). Destructor
*/
CSipUdpHandle::~CSipUdpHandle()
{
}

//// modified by dhong
static string  g_recvStr;

/**
* Description:  handle_recv(). 当有数据到达时调用此函数
* @return       void.
*/
void CSipUdpHandle::handle_recv(void)
{
    DEBUG_LOG("*********************handle_recv:begin*********************");

    char szRecvBuf[SIP::MAX_LEN_UDP_PACKET] = {0};

    sockaddr_in stPeerAddr = {0};
    socklen_t nPeerAddrLen = sizeof(stPeerAddr);

    int nResult = recvfrom((unsigned long)m_lSockFD, szRecvBuf, sizeof(szRecvBuf), 0, (sockaddr*)&stPeerAddr, &nPeerAddrLen);

    //设置继续接收数据
    setHandleRecv(VOS_TRUE);

    if (0 >= nResult)
    {
        WARN_LOG("handle_recv:can't receive data from socket.");
        return ;
    }
	if(nResult >= SIP::MAX_LEN_UDP_PACKET)
	{
		WARN_LOG("handle_recv:receive too long data from socket.");
		return ;
	}

    //设置结束符
    szRecvBuf[nResult] = 0;
    char* recv_data_buf_p = new char[(unsigned int)nResult];
    ACE_OS::memcpy(recv_data_buf_p, szRecvBuf, sizeof(char)*(unsigned int)nResult);
    if ( NULL == recv_data_buf_p )
    {
        WARN_LOG("handle_recv:can't malloc recv data buffer.");
        return ;
    }

    const unsigned char* addr_ipv4_p = (unsigned char*)&(stPeerAddr.sin_addr.s_addr);
    const int   addr_port_i = ntohs(stPeerAddr.sin_port);
	DEBUG_LOG( "handle_recv:src_addr=%u.%u.%u.%u:%u,length=%d,data=\n%s",
		(unsigned int)addr_ipv4_p[0],
		(unsigned int)addr_ipv4_p[1],
		(unsigned int)addr_ipv4_p[2],
		(unsigned int)addr_ipv4_p[3],
		addr_port_i,
		nResult, 
		szRecvBuf);

	POST_TO_APP_ST* post_to_pu_p = new POST_TO_APP_ST; //释放交给协议栈工作线程
    ACE_OS::memset(post_to_pu_p, 0x0, sizeof(POST_TO_APP_ST));

    post_to_pu_p->message_string_st.pcData = recv_data_buf_p;
    post_to_pu_p->message_string_st.ulLen  = (unsigned int)nResult;

    post_to_pu_p->tpt_network_info_st.ucAddressType              = SIP_ADDR_TYPE_IPV4;
    post_to_pu_p->tpt_network_info_st.ucProtocol                 = SIP_TRANSPORT_UDP;

    post_to_pu_p->tpt_network_info_st.stSrcAddr.u.aucIPv4Addr[0] = (unsigned char)addr_ipv4_p[0];
    post_to_pu_p->tpt_network_info_st.stSrcAddr.u.aucIPv4Addr[1] = (unsigned char)addr_ipv4_p[1];
    post_to_pu_p->tpt_network_info_st.stSrcAddr.u.aucIPv4Addr[2] = (unsigned char)addr_ipv4_p[2];
    post_to_pu_p->tpt_network_info_st.stSrcAddr.u.aucIPv4Addr[3] = (unsigned char)addr_ipv4_p[3];
    post_to_pu_p->tpt_network_info_st.stSrcAddr.iPort            = addr_port_i;

    //TODO:DstAddr应该使用当前协议栈监听的地址，但之前协议栈源地址和目的地址使用同一个地址。故此处
    //     保留原协议栈代码，留待后续解决。
    post_to_pu_p->tpt_network_info_st.stDstAddr.u.aucIPv4Addr[0] = (unsigned char)addr_ipv4_p[0];
    post_to_pu_p->tpt_network_info_st.stDstAddr.u.aucIPv4Addr[1] = (unsigned char)addr_ipv4_p[1];
    post_to_pu_p->tpt_network_info_st.stDstAddr.u.aucIPv4Addr[2] = (unsigned char)addr_ipv4_p[2];
    post_to_pu_p->tpt_network_info_st.stDstAddr.u.aucIPv4Addr[3] = (unsigned char)addr_ipv4_p[3];
    post_to_pu_p->tpt_network_info_st.stDstAddr.iPort            = addr_port_i;

    if ( !CSipStackTask::Instance().PostToApp(post_to_pu_p) )
    {
        delete [] recv_data_buf_p;
        recv_data_buf_p = NULL;
        post_to_pu_p->message_string_st.pcData = NULL;
        post_to_pu_p->message_string_st.ulLen  = 0;
        delete post_to_pu_p;
        post_to_pu_p = NULL;

        ERROR_LOG("handle_recv:failure to post to app.");
        return ;//lint !e438
    }

    DEBUG_LOG("handle_recv:success.");
}

/**
* Description:  handle_send(). 当可以发送数据时调用此函数
* @return       void.
*/
void CSipUdpHandle::handle_send(void)
{
}

/**
* Description:    CSipStack(). Default constructor
*/
CSipStack::CSipStack():m_mapKey(0), m_reserved(0)
{
    m_pfnNotify      = VOS_NULL; //回调函数指针
    m_pNotifyFnParams         = VOS_NULL; //用户数据指针
    m_pProceEventThread = VOS_NULL; //处理事件线程  
    m_pTimerTrigger     = VOS_NULL; //定时器
    m_pMsgTimerTrigger  = VOS_NULL; // MSG timer
    m_pMapMutex         = VOS_NULL; //map锁

    m_bRegister             = VOS_FALSE;    //是否已注册
    m_bProcEventThreadRun   = VOS_FALSE;    //处理事件线程是否运行
    m_bHasRedirect          = VOS_FALSE;    //是否有重定向
    m_bRedirected           = VOS_FALSE;    //是否重定向过
    m_bTempRedirect         = VOS_FALSE;    //是否为临时重定向

    m_ulTuObjId                 = 0;    //TU对象ID
    m_ulCSeq                    = 0;    //SIP消息的CSeq
    m_ulTimerId                 = 0;    //定时器ID
    m_ulPlatAuthInfoChangeNum   = 0;    //平台鉴权信息变更次数

    //注册信息
    memset(&m_stRegisterInfo, 0, sizeof(m_stRegisterInfo));
    //密码的MD5的16进制字符串
    memset(m_szPwdMd5, 0, sizeof(m_szPwdMd5));

    //BEGIN R002C01SPC001 ADD 2011-08-24 ligengqiang 00124479 for 增加线程锁，MiniSIP只支持单线程
    //创建线程锁
    m_pThreadMutex = VOS_CreateMutex();
    //END   R002C01SPC001 ADD 2011-08-24 ligengqiang 00124479 for 增加线程锁，MiniSIP只支持单线程

    // added by dhong 
    m_pThreadlistSvrInfoMutex = VOS_CreateMutex();
    m_pThreadServerInfoMutex = VOS_CreateMutex();
    // end by dhong

    // 创建消息缓存锁 // added by dhong
    m_pReqMsgMutex = VOS_CreateMutex();

    ///////////////////////////// add by w00207027 2012-11-02 Begin ///////////////////////////////////////////////////
    m_objServerAddrEx.m_lIpAddr =0;
    m_objServerAddrEx.m_usPort = 0;
    m_bServerAddrEx = false;
    ///////////////////////////// add by w00207027 2012-11-02 End /////////////////////////////////////////////////////

    //初始化前端重发处理数据结构
    m_mapReqResend[SIP_METHOD_REGISTER] = DEAL_RESEND_LIST();
    m_mapReqResend[SIP_METHOD_OPTIONS]  = DEAL_RESEND_LIST();
    m_mapReqResend[SIP_METHOD_INVITE]   = DEAL_RESEND_LIST();
    m_mapReqResend[SIP_METHOD_MESSAGE]  = DEAL_RESEND_LIST();

    m_mapRspResend[SIP_METHOD_INVITE]   = DEAL_RESEND_LIST();
    m_mapRspResend[SIP_METHOD_ACK]      = DEAL_RESEND_LIST();
    m_mapRspResend[SIP_METHOD_BYE]      = DEAL_RESEND_LIST();
    m_mapRspResend[SIP_METHOD_MESSAGE]  = DEAL_RESEND_LIST();

	//
	m_usPort = 0;
}

/**
* Description:    ~CSipStack(). Destructor
*/
CSipStack::~CSipStack()
{
    m_pfnNotify      = VOS_NULL; //回调函数指针
    m_pNotifyFnParams         = VOS_NULL; //用户数据指针
    m_pProceEventThread = VOS_NULL; //处理事件线程
    m_pTimerTrigger     = VOS_NULL; //定时器
    m_pMapMutex         = VOS_NULL; //map锁

    //BEGIN R002C01SPC001 ADD 2011-08-24 ligengqiang 00124479 for 增加线程锁，MiniSIP只支持单线程
    //所有可能访问MiniSIP函数的入口处，都加上线程锁
    if (VOS_NULL != m_pThreadMutex)
    {
        (void)VOS_DestroyMutex(m_pThreadMutex);
        m_pThreadMutex = VOS_NULL;
    }

    // added by dhong
    if (VOS_NULL != m_pReqMsgMutex)
    {
        (void)VOS_DestroyMutex(m_pReqMsgMutex);
        m_pReqMsgMutex = VOS_NULL;
    }

    // added by dhong
    if (NULL != m_pThreadlistSvrInfoMutex)
    {
        (void)VOS_DestroyMutex(m_pThreadlistSvrInfoMutex);
        m_pThreadlistSvrInfoMutex = NULL;
    }

    if (NULL != m_pThreadServerInfoMutex)
    {
        (void)VOS_DestroyMutex(m_pThreadServerInfoMutex) ;
        m_pThreadServerInfoMutex = NULL;
    }
    // end by dhong

    while(!m_msgMap.empty())
    {
        m_msgItertor = m_msgMap.begin();
        SIP::REQ_MSG_INFO* pReqMsgInfo = m_msgItertor->second;
        ACE_Guard<ACE_Recursive_Thread_Mutex> locker(CSipStackTask::MiniSipMutex()); //无论是否加锁成功都必须对MINISIP初始化
        if ( !locker.locked() )
        {
            ERROR_LOG("sipstack class - ~CSipStack - someting wrong with minisip locker.");
        }
        SipDsmReleaseMsgRef(&(pReqMsgInfo->pstSipReqMsg));
        VOS_DELETE(pReqMsgInfo);
        m_msgMap.erase(m_msgItertor);
    }
    //END   R002C01SPC001 ADD 2011-08-24 ligengqiang 00124479 for 增加线程锁，MiniSIP只支持单线程
}

long CSipStack::Init(SIP::PCALLBACK_NOTIFY pfnNotify, void* pNotifyFnParams, const unsigned short local_port )
{
    INFO_LOG("sip stack - init - begin");

    //回调函数指针不能为空
    if (VOS_NULL == pfnNotify)
    {
        ERROR_LOG("sip stack - init - notify function pointer is null");
        return SIP::RET_CODE_PARA_INVALIDATE;
    }

    //设置通知回调函数状态值
    m_pfnNotify        = pfnNotify;
    m_pNotifyFnParams  = pNotifyFnParams;

    //创建Map锁
    m_pMapMutex = VOS_CreateMutex();

    //清空CSeq号与SIP消息信息的映射表
    m_mapCSeqToSipMsgInfo.clear();

    //初始化MiniSip
    long lResult = VOS_FAIL;
    {
        ACE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, locker, CSipStackTask::MiniSipMutex(), VOS_FALSE);
        lResult = SIP::InitMiniSip();//lint !e838
        if (SIP::RET_CODE_OK != lResult)
        {
            ERROR_LOG("sip stack - init - failure to init minisip");
            return lResult;
        }
    }

    //获得通信管理器实例
    CConnMgr& rConnMgr = CONN_MGR;

    if (VOS_SUCCESS != rConnMgr.init(100, VOS_TRUE, VOS_FALSE, VOS_FALSE))
    {
        ERROR_LOG("sip stack - init - failure to init communication manager.");
        return SIP::RET_CODE_FAIL;
    }
    rConnMgr.run();

    //设置本地地址
    // SetLocalAddr(local_port);
    m_objLocalAddr.m_usPort = htons(local_port);

    //创建UDP socket
    lResult = rConnMgr.regUdpSocket(&m_objLocalAddr, &m_objSipUdpHandle, NULL);
    if(VOS_OK != lResult)
    {
        ERROR_LOG("sip stack - init - failure to create udp socket - ErrorCode = %d",lResult);    
        return SIP::RET_CODE_FAIL;
    }


    //设置全局变量
    g_ulSipUdpSocket = (unsigned long)m_objSipUdpHandle.m_lSockFD;

    //初始化事件同步队列
    lResult = m_objEventSyncQueue.init(SIP::EVENT_SYNC_QUEUE_NUM);
    if (VOS_OK != lResult)
    {
        ERROR_LOG("sip stack - init - failure to init event synchronous queue.");
        return SIP::RET_CODE_FAIL;
    }

    //启动处理事件线程
    m_bProcEventThreadRun = VOS_TRUE;
    unsigned long ulResult = VOS_CreateThread((VOS_THREAD_FUNC)ProcEventThreadEntry, this, &m_pProceEventThread, VOS_DEFAULT_STACK_SIZE);
    if (VOS_OK != ulResult)
    {
        ERROR_LOG("sip stack - init - failure to create process event thread - ErrorCode = %d",ulResult); 
        return SIP::RET_CODE_FAIL;
    }

    //创建定时器
    if (NULL == VOS_NEW(m_pTimerTrigger))
    {
        ERROR_LOG("sip stack - init - failure to alloc timer trigger. ");
        return SIP::RET_CODE_FAIL;
    }

    // 初始化定时器
    if (VOS_SUCCESS != NVS_TIMER.init(100))
    {
        ERROR_LOG("sip stack - init - failure to init timer");
        return SIP::RET_CODE_FAIL;
    }
    //注册定时器
    if (VOS_OK != NVS_TIMER.registerTimer(m_pTimerTrigger, this, SIP::TIMER_INTERVAL * 10, enRepeated))
    {
        ERROR_LOG("sip stack - init - failure to register timer trigger. ");
        return SIP::RET_CODE_FAIL;
    }

    //创建定时器 msg
    if (NULL == VOS_NEW(m_pMsgTimerTrigger))
    {
        ERROR_LOG("sip stack - init - failure to alloc message timer trigger. ");
        return SIP::RET_CODE_FAIL;
    }

    //注册定时器 msg
    if (VOS_OK != NVS_TIMER.registerTimer(m_pMsgTimerTrigger, this, SIP::TIMER_INTERVAL * 100, enRepeated))
    {
        ERROR_LOG("sip stack - init - failure to register timer trigger.");
        return SIP::RET_CODE_FAIL;
    }

    NVS_TIMER.run();

    INFO_LOG("sip stack - init - success");
    return SIP::RET_CODE_OK;
}

/**
* Description:  ReleaseSipStack(). 释放SipStack
* @return       long.       返回码
*/
long CSipStack::Fini()
{
    INFO_LOG("sipstack - finish - begin.");
    //释放定时器
    if (VOS_NULL != m_pTimerTrigger)
    {
        (void)NVS_TIMER.cancelTimer(m_pTimerTrigger);

        VOS_DELETE(m_pTimerTrigger);
    }

    if (VOS_NULL != m_pMsgTimerTrigger)
    {
        (void)NVS_TIMER.cancelTimer(m_pMsgTimerTrigger);

        VOS_DELETE(m_pMsgTimerTrigger);
    }

    //退出处理事件线程
    m_bProcEventThreadRun = VOS_FALSE;

    if (VOS_NULL != m_pProceEventThread)
    {
        //生成退出线程事件
        SIP::EVENT_INFO stEventInfo(SIP::EVENT_TYPE_EXIT_THREAD);

        //往事件同步队列插入退出线程事件，不需处理错误
        (void)AddEvent(stEventInfo);

        //最多等待30秒，如尚未退出，则程序逻辑有问题
        (void)VOS_ThreadJoin(m_pProceEventThread);
        //释放内存
        VOS_free(m_pProceEventThread);
        m_pProceEventThread = VOS_NULL;
    }

    //设置全局变量为无效值
    g_ulSipUdpSocket = (unsigned long)InvalidSocket;

    //获得通信管理器实例
    CConnMgr& objConnMgr = CONN_MGR;

    //从通信管理器中移除SIP的UDP的Socket
    objConnMgr.removeUdpSocket(&m_objSipUdpHandle);
    objConnMgr.exit();

    //清空鉴权信息
    m_stPlatAuthInfo.strRealm = "";
    m_stPlatAuthInfo.strNonce = "";
    m_stPlatAuthInfo.strOpaque = "";

    //清理所有消息信息
    ClearAllMsgInfo();

    //释放MiniSip
    {
        ACE_Guard<ACE_Recursive_Thread_Mutex> locker(CSipStackTask::MiniSipMutex()); //无论是否加锁成功都必须对MINISIP初始化
        if ( !locker.locked() )
        {
            ERROR_LOG("sipstack class - release sipstack - someting wrong with minisip locker.");
        }
        SipLmCoreLibDeInit();
    }

    m_mapDialogIdToInfo.clear();
    m_mapDialogInfoToId.clear();

    //释放Map锁
    if (VOS_NULL != m_pMapMutex)
    {
        (void)VOS_DestroyMutex(m_pMapMutex);
        m_pMapMutex = VOS_NULL;
    }

	NVS_TIMER.exit();
    INFO_LOG("sipstack - finish - end.");
    return SIP::RET_CODE_OK;
}

/**
* Description:  ProcEventThreadEntry().  处理事件线程入口
* @param  [in]  pPara   线程入口参数
* @return       unsigned long.   返回码
*/
unsigned long STDCALL CSipStack::ProcEventThreadEntry(void* pPara)
{
    //参数指针安全转换
    CSipStack* pThis = VOS_CAST((CSipStack*)pPara);
    if (NULL == pThis)
    {
        ERROR_LOG("Process Event Thread Parameter Invalidate. Parameter Address is %x",pPara);
        return SIP::RET_CODE_PARA_INVALIDATE;
    }

    //执行处理事件线程
    pThis->ProcEventThread();

    return SIP::RET_CODE_OK;
}

/**
* Description:  ProcEventThread().  处理事件线程
* @return       void.
*/
void CSipStack::ProcEventThread()
{
    //清空定时器信息map
    m_mapTimerIdToInfo.clear();

    //返回值
    long lResult = SIP::RET_CODE_OK;
    //事件信息指针
    SIP::EVENT_INFO* pstEventInfo = VOS_NULL;
    const ACE_Time_Value rest_time((long long)0, 50 * 1000);

    while (m_bProcEventThreadRun)
    {
        pstEventInfo = VOS_NULL;
        //队列为空时会死等，直到获取一个事件。故若需退出该线程，需往队列插入EVENT_TYPE_EXIT_THREAD事件
        if (m_objEventSyncQueue.empty())
        {
            ACE_OS::sleep(rest_time);
            continue;
        }
        lResult = m_objEventSyncQueue.popFrontEv(pstEventInfo);
        if (VOS_OK != lResult || NULL == pstEventInfo )
        {
            ACE_OS::sleep(rest_time);
            continue;
        }

        //分别处理不同的事件
        switch (pstEventInfo->enEventType)
        {
        case SIP::EVENT_TYPE_STACK_TIMER_TRIGGER:  //定时器触发事件
            {
                HandleEventTimerTrigger();
                break;
            }

        case SIP::EVENT_TYPE_STACK_TIMER_REGISTER:  //定时器注册事件
            {
                if ( NULL != pstEventInfo->pEventInfo )
                {
                    lResult = HandleEventTimerRegister(pstEventInfo->pEventInfo, pstEventInfo->ulEventInfoLen);
                }
                break;
            }

        case SIP::EVENT_TYPE_STACK_TIMER_CANCEL:    //定时器取消事件
            {
                if ( NULL != pstEventInfo->pEventInfo )
                {
                    lResult = HandleEventTimerCancel(pstEventInfo->pEventInfo, pstEventInfo->ulEventInfoLen);
                }
                break;
            }

        //case SIP::EVENT_TYPE_STACK_REDIRECT:        //重定向事件
        //    {
        //        if ( NULL != pstEventInfo->pEventInfo )
        //        {
        //            HandleEventRedirect(pstEventInfo->pEventInfo, pstEventInfo->ulEventInfoLen);
        //        }
        //        break;
        //    }

        case SIP::EVENT_TYPE_EXIT_THREAD:    //线程退出
            {
                INFO_LOG("process event thread - exit thread -begin.");
                m_bProcEventThreadRun = VOS_FALSE;
                break;
            }

        case SIP::EVENT_TYPE_STACK_INVALID:    //无效类型
        default:    //不支持的事件类型
            {
                WARN_LOG("process event thread - unsupport event type -  Event= %s",SIP::STR_ARR_EVENT_TYPE[pstEventInfo->enEventType]);
                //防止出现非法值
                pstEventInfo->enEventType = SIP::EVENT_TYPE_STACK_INVALID;
                break;
            }
        }

        if (SIP::RET_CODE_OK != lResult)
        {
			ERROR_LOG(" process event thread - failure to handle - Error=0x%04X, Event=%s.",
				lResult, SIP::STR_ARR_EVENT_TYPE[pstEventInfo->enEventType]);
        }

        //释放此次事件
        if ( NULL != pstEventInfo->pEventInfo )
        {
			VOS_DELETE(pstEventInfo->pEventInfo);
        }
        VOS_DELETE(pstEventInfo);

    }

    //线程已退出，清空事件同步队列。
    while (VOS_OK == m_objEventSyncQueue.popFrontEv(pstEventInfo, 0, QUEUE_MODE_NOWAIT))
    {
        //释放事件
        if ( NULL != pstEventInfo )
        {
            if ( NULL != pstEventInfo->pEventInfo )
            {
				VOS_DELETE(pstEventInfo->pEventInfo);
            }
            VOS_DELETE(pstEventInfo);
        }
        pstEventInfo = VOS_NULL;
    }

    INFO_LOG("process event thread - exit.");
}

/**
* Description:  ClearAllMsgInfo().  清理所有消息信息
* @return       void.
*/
void CSipStack::ClearAllMsgInfo()
{
    //加上锁
    INFO_LOG("sipstack class - clear all msg info");
    ACE_Guard<ACE_Recursive_Thread_Mutex> locker(CSipStackTask::MiniSipMutex());
    if ( !locker.locked() )
    {
        ERROR_LOG("sipstack class - clear all msg info - some wrong with minisip lock.");
    }

    //遍历中止所有事务
    MAP_C_SEQ_TO_SIP_MSG_INFO::iterator iter = m_mapCSeqToSipMsgInfo.begin();
    for (; iter != m_mapCSeqToSipMsgInfo.end(); ++iter)
    {
        SIP_MSG_INFO& stMsgInfo = *iter->second;

        //强制中止事务
        (void)SipTxnHiTerminateTxn(stMsgInfo.ulTxnObjId, stMsgInfo.ulTuObjId, SIP_TERM_MODE_FORCEFUL_NO_CANCEL_RSP);
        //释放SIP消息
        SipDsmReleaseMsgRef(&stMsgInfo.pstSipMsg);
        //释放内存
        VOS_DELETE(iter->second);
    }

    //清空map
    m_mapCSeqToSipMsgInfo.clear();
}

/**
* Description:  AddEvent(). 添加事件
* @param  [in]  stEventInfo     事件信息引用
* @return       long.   返回码
*/
long CSipStack::AddEvent(const SIP::EVENT_INFO& stEventInfo)
{
    if ( SIP::MAX_STR_ARR_EVENT_TYPE_COUNT <= stEventInfo.enEventType )
    {
        ERROR_LOG("add event - invalid event type - %d",stEventInfo.enEventType);
        return SIP::RET_CODE_FAIL;
    }

    INFO_LOG("add event - begin. %s.", SIP::STR_ARR_EVENT_TYPE[stEventInfo.enEventType]);

    long lResult = SIP::RET_CODE_OK;

    //创建事件
    SIP::EVENT_INFO* pstEventInfo = VOS_NEW(pstEventInfo);
    //创建失败
    if (NULL == pstEventInfo)
    {
        //退出线程时，必须往事件同步队列插入一个事件此时使用空事件
        if (SIP::EVENT_TYPE_EXIT_THREAD == stEventInfo.enEventType)
        {
            //往事件同步队列插入一个空事件
            lResult = m_objEventSyncQueue.pushBackEv(NULL);
            if (SIP::RET_CODE_OK != lResult)
            {
                ERROR_LOG("add event - failure to  push null event - event= %s",SIP::STR_ARR_EVENT_TYPE[stEventInfo.enEventType]);
            }
            else
            {
                if ( NULL != stEventInfo.pEventInfo )
                {
                    void* reclaim_p = stEventInfo.pEventInfo;
                    VOS_DELETE(reclaim_p);
                }
            }
            return lResult;
        }

        //其他情况，返回失败
        ERROR_LOG("add event - failure to create event - event= %s",SIP::STR_ARR_EVENT_TYPE[stEventInfo.enEventType]);
        return SIP::RET_CODE_ALLOC_MEM_FAIL;
    }

    //复制事件信息
    *pstEventInfo = stEventInfo;

    //往事件同步队列插入事件
    lResult = m_objEventSyncQueue.pushBackEv(pstEventInfo);
    if (SIP::RET_CODE_OK != lResult)
    {
        //释放临时内存
        VOS_DELETE(pstEventInfo);

        ERROR_LOG("add event - push event - event= %s",SIP::STR_ARR_EVENT_TYPE[stEventInfo.enEventType]);

		return SIP::RET_CODE_FAIL;
    }

    INFO_LOG("Add Event To Synchronous Queue Success. %s.", SIP::STR_ARR_EVENT_TYPE[stEventInfo.enEventType]);
    return SIP::RET_CODE_OK;
}

/**
* Description:  GetTimerId().   获取定时器ID
* @return       unsigned long.  定时器ID
*/
unsigned long CSipStack::GetTimerID()
{
    return ++m_ulTimerId;
}

/**
* Description:  HandleEventTimerTrigger().  处理定时器触发事件
* @return       void.
*/
void CSipStack::HandleEventTimerTrigger()
{
    unsigned long ulCurTimeTick = VOS_GetTicks();

    ACE_READ_GUARD(ACE_Recursive_Thread_Mutex, mapLocker, m_mutexTimerMap);

    for ( MAP_TIMER_ID_TO_INFO::iterator iter = m_mapTimerIdToInfo.begin();
          iter != m_mapTimerIdToInfo.end();
          ++iter
        )
    {
        SIP::TIMER_INFO& stTimerInfo = (SIP::TIMER_INFO)(iter->second);

        if ( stTimerInfo.bStop || stTimerInfo.ulTimerLength > ulCurTimeTick - stTimerInfo.ulStartTimeTick )
        {
            continue;
        }

        stTimerInfo.bStop = true;

        //停止定时器
        SipStopTimer(stTimerInfo.ulTimerId);

        SS_UINT32 ulUser = 0;

        //获取模块ID
        ulUser = ulUser | stTimerInfo.ulTimerPara >> SIP::TIMER_USER_RIGHT_SHIFT;
        switch (ulUser)
        {
        case SIP_COMP_TXN:
            {
                ACE_GUARD(ACE_Recursive_Thread_Mutex, locker, CSipStackTask::MiniSipMutex());
                SipTxnTimeoutHandler(stTimerInfo.ulTimerPara, stTimerInfo.ulTimerId);
                break;
            }

        default:
            {
                break;
            }
        }
    }
}

/**
* Description:  HandleEventTimerRegister(). 处理定时器注册事件
* @param  [in]  pEventInfo      事件信息指针
* @param  [in]  ulEventInfoLen  事件信息内容长度
* @return       long.       返回码
*/
long CSipStack::HandleEventTimerRegister
(
    void*           pEventInfo, 
    unsigned long   ulEventInfoLen
)
{
	if (VOS_NULL == pEventInfo || sizeof(SIP::EVENT_INFO_TIMER_REGISTER) != ulEventInfoLen )
	{
		ERROR_LOG("handle to register timer event -  failure - invalid param.");
		return SIP::RET_CODE_PARA_INVALIDATE;
	}

	SIP::EVENT_INFO_TIMER_REGISTER* pstEventInfo = (SIP::EVENT_INFO_TIMER_REGISTER*)pEventInfo;

	//加入映射表
	ACE_WRITE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, mapLocker, m_mutexTimerMap, SIP::RET_CODE_FAIL);
	(void)m_mapTimerIdToInfo.insert( std::make_pair< unsigned long, SIP::TIMER_INFO >
												(
													pstEventInfo->ulTimerId,
													*pstEventInfo
												)
									);
	//if ( !ret.second )
	//{
	//    ERROR_LOG("handle to register timer event - failure to insert.";
	//}
	//m_mapTimerIdToInfo[pstEventInfo->ulTimerId] = *pstEventInfo;

	return SIP::RET_CODE_OK;
}

/**
* Description:  HandleEventTimerCancel().   处理定时器取消事件
* @param  [in]  pEventInfo      事件信息指针
* @param  [in]  ulEventInfoLen  事件信息内容长度
* @return       long.       返回码
*/
long CSipStack::HandleEventTimerCancel
(
    void*           pEventInfo, 
    unsigned long   ulEventInfoLen
)
{
    if (VOS_NULL == pEventInfo)
    {
        ERROR_LOG("Handle Timer Register Event Failed. Event Info is Empty.");
        return SIP::RET_CODE_PARA_INVALIDATE;
    }

    if (sizeof(SIP::EVENT_INFO_TIMER_CANCEL) != ulEventInfoLen)
    {
        ERROR_LOG("Handle Timer Register Event Failed. Event Info Length(%d) Must be %d",ulEventInfoLen,sizeof(SIP::EVENT_INFO_TIMER_REGISTER));
        return SIP::RET_CODE_PARA_INVALIDATE;
    }

    SIP::EVENT_INFO_TIMER_CANCEL* pstEventInfo = (SIP::EVENT_INFO_TIMER_CANCEL*)pEventInfo;

    ACE_WRITE_GUARD_RETURN(ACE_Recursive_Thread_Mutex, mapLocker, m_mutexTimerMap, SIP::RET_CODE_FAIL)
    (void)m_mapTimerIdToInfo.erase(pstEventInfo->ulTimerId);

    return SIP::RET_CODE_OK;
}

/**
* Description:  HandleEventRedirect().  处理重定向事件
* @param  [in]  pEventInfo      事件信息指针
* @param  [in]  ulEventInfoLen  事件信息内容长度
* @return       void.
*/
void CSipStack::HandleEventRedirect
(
    void*           pEventInfo, 
    unsigned long   ulEventInfoLen
)
{
    if (VOS_NULL == pEventInfo)
    {
        ERROR_LOG("Handle Redirect Event Failed. Event Info is Empty.");    
        return ;
    }

    if (sizeof(SIP_MSG_INFO) != ulEventInfoLen)
    {
        ERROR_LOG("Handle Redirect Event Failed. Event Info Length(%d) Must be %d",ulEventInfoLen,sizeof(SIP_MSG_INFO));
        return ;
    }

    SIP_MSG_INFO* pstMsgInfo = (SIP_MSG_INFO*)pEventInfo;

    SIP::TEXT_MSG stTextMsg = {0};
    stTextMsg.ulMsgSeq      = pstMsgInfo->ulMsgSeq;
    stTextMsg.enTextMsgType = pstMsgInfo->enTextMsgType;
    stTextMsg.ulDialogId    = pstMsgInfo->ulDialogId;
    stTextMsg.pUserData     = pstMsgInfo->pstSipMsg;

    CSipStackMsgWrapRedirect objSipMsg(*this, stTextMsg);

    long lResult = VOS_FAIL;
    {
        ACE_GUARD_REACTION(ACE_Recursive_Thread_Mutex, locker, CSipStackTask::MiniSipMutex(), ;);
        lResult = objSipMsg.SendMsg();//lint !e838
    }

    //重定向重发消息，不能释放原消息
    //SipDsmReleaseMsgRef(pstMsgInfo->pstSipMsg);

    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("Send SIP Request Message Failed On Handle Redirect Event.");
        return ;
    }
}

/**
* Description:  GetDialogIdBySipMsg().    根据SIP消息找到对话ID
* @param  [in]      stSipMsg    SIP消息结构体引用
* @param  [out] ulDialogId      对话ID
* @param  [in]      bErase      是否删除
* @return       long.       返回码
*/
long CSipStack::GetDialogIdBySipMsg
(
    SipMsgS&        stSipMsg,
    unsigned long&  ulDialogId,
    VOS_BOOLEAN     bErase
)
{
    //构造函数中初始化
    SIP::DIALOG_INFO stDialogInfo;

    //获取对话信息，请求消息来自远端
    long lResult = SIP::GetDialogInfo(stSipMsg, stDialogInfo, VOS_FALSE);
    switch (lResult)
    {
    case SIP::RET_CODE_OK:  //成功则继续
        {
            break;
        }

    case SIP::RET_CODE_NO_TO_TAG:   //无To的Tag，则返回成功
        {
            INFO_LOG("Get Dialog ID By SIP Message End. Invite Message No To Tag.");
            return SIP::RET_CODE_OK;
            //break;
        }

    default:    //其他错误，直接返回失败
        {
            WARN_LOG("Get Dialog Info Failed On Get Dialog ID By SIP Message.");
            return SIP::RET_CODE_FAIL;
            //break;
        }
    }

    //根据对话信息找到对话ID
    lResult = GetDialogIdByInfo(stDialogInfo, ulDialogId, bErase);
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("Get Dialog ID By Dialog Info Failed On Get Dialog ID By SIP Message.");
        return SIP::RET_CODE_FAIL;
    }

    return SIP::RET_CODE_OK;
}

/**
* Description:  GetDialogIdBySipMsg().    根据对话信息找到对话ID
* @param  [in]      stDialogInfo    对话信息
* @param  [out] ulDialogId      对话ID
* @param  [in]      bErase      是否删除，其中会同时m_mapDialogIdToInfo删除该映射
* @return       long.       返回码
*/
long CSipStack::GetDialogIdByInfo
(
    const SIP::DIALOG_INFO& stDialogInfo,
    unsigned long&          ulDialogId,
    VOS_BOOLEAN             bErase
)
{
    //加上锁
    CLockGuard objLocker(m_pMapMutex);

    MAP_DIALOG_INFO_TO_ID::iterator iter = find_if(m_mapDialogInfoToId.begin(),
        m_mapDialogInfoToId.end(),
        FindDialogInfo(stDialogInfo));
    //未找到匹配的立即返回错误
    if (iter == m_mapDialogInfoToId.end())
    {
		ERROR_LOG("Get Dialog ID By Dialog Info Failed. Dialog Info Invalidate. "
			"Call-ID = %s. LocalTag = %s. RemoteTag = %s.", stDialogInfo.strCallId.c_str(),
			stDialogInfo.strLocalTag.c_str(), stDialogInfo.strRemoteTag.c_str());
        return SIP::RET_CODE_FAIL;
    }

    //设置对话 ID
    ulDialogId = (unsigned long)iter->second;

    if (bErase)
    {
        (void)m_mapDialogInfoToId.erase(iter);
        (void)m_mapDialogIdToInfo.erase(ulDialogId);
    }

	INFO_LOG("Get Dialog ID By Dialog Info Success. Dialog Info( DialogID = %d ): "
		"Call-ID = %s. LocalTag = %s. RemoteTag = %s.", ulDialogId, stDialogInfo.strCallId.c_str(),
		stDialogInfo.strLocalTag.c_str(), stDialogInfo.strRemoteTag.c_str());

    return SIP::RET_CODE_OK;
}//lint !e1788

/**
* Description:  GetDialogInfoById().      根据对话ID找到对话信息
* @param  [in]      ulDialogId      对话ID
* @param  [out]     stDialogInfo    对话信息
* @return       long.       返回码
*/
long CSipStack::GetDialogInfoById
(
    unsigned long       ulDialogId,
    SIP::DIALOG_INFO&   stDialogInfo
)
{
    //加上锁
    CLockGuard objLocker(m_pMapMutex);

    //查找对话信息
    MAP_DIALOG_ID_TO_INFO::iterator iter = m_mapDialogIdToInfo.find(ulDialogId);
    if (m_mapDialogIdToInfo.end() == iter)
    {
        ERROR_LOG("Get Dialog Info By ID Failed. Dialog ID(%d) Not Exist." ,ulDialogId);
        return SIP::RET_CODE_FAIL;
    }

    stDialogInfo = iter->second;
    return SIP::RET_CODE_OK;
}//lint !e1788

/**
* Description:  SetLocalAddr().  设置本地地址
* @return       void.
*/
void CSipStack::SetLocalAddr(const unsigned short local_port)
{
    sockaddr_in stAddr = {0};
    //socklen_t nAddrLen = sizeof(stAddr);
    //UDP方式只能获取端口
    // comment by dhong
    /*   int nResult = getsockname((unsigned long)m_objSipUdpHandle.m_lSockFD, (struct sockaddr*)&stAddr, &nAddrLen);
    if (SOCKET_ERROR == nResult)
    {
    ERROR_LOG("getsockname Failed On Get Local Address. ErrorCode = %d.", CONN_ERRNO);    
    return;
    }*/
    int nResult=0;

    //保存本地端口
    // modified by dhong
    m_objLocalAddr.m_usPort = htons(local_port);//stAddr.sin_port;

    //获取本机IP
    char szHostName[SIP::MAX_LEN_FILE_PATH] = {0};
    //获取主机名
    nResult = gethostname(szHostName, sizeof(szHostName));//lint !e838
    if (SOCKET_ERROR == nResult)
    {
        ERROR_LOG("gethostname Failed On Get Local Address. ErrorCode = %d.",CONN_ERRNO);
        return;
    }

    //获取本机地址
    struct hostent* pstHostent = NULL;
    pstHostent = gethostbyname(szHostName);   //lint !e838
    if (pstHostent)
    {
        stAddr.sin_addr.s_addr = *(unsigned long*)(void*)pstHostent->h_addr;
    }

    INFO_LOG("SIP Local Adress: IP = %.15s. Port = %d.", inet_ntoa(stAddr.sin_addr), ntohs(m_objLocalAddr.m_usPort));

    //保存本地IP地址
    m_objLocalAddr.m_lIpAddr = (long)stAddr.sin_addr.s_addr;
}//lint !e438

/**
* Description:  SetRegisterInfo().  设置注册信息
* @param  [in]  stRegisterInfo  注册信息
* @return       long.           返回码
*/
long CSipStack::SetRegisterInfo(const SIP::REGISTER_INFO& stRegisterInfo)
{
	INFO_LOG("Set Register Info In Sip Stack Begin. LoginDomain = %.128s. "
		"LoginName = %.32s. LocalIP = %.16s. ProductName = %.128s. ProductVersion = %.64s. "
		"ExpireTime = %d. ServerInfoNum = %d.",
		stRegisterInfo.szLoginDomain, 
		stRegisterInfo.szLoginName, stRegisterInfo.szLocalIp, stRegisterInfo.szProductName, 
		stRegisterInfo.szProductVersion, stRegisterInfo.ulExpireTime, stRegisterInfo.ulServerInfoNum);

    // SetRegisterInfo的时候，禁止对这两个链表的任何操作，所以把这两个锁放在这儿
    // 这个函数不应该频繁调用，需要优化  dhong
    CLockGuard objListServerLocker(m_pThreadlistSvrInfoMutex);
    CLockGuard objServerLocker(m_pThreadServerInfoMutex);

    //至少必须有一个服务器信息
    if (0 == stRegisterInfo.ulServerInfoNum)
    {
        ERROR_LOG("Set Register Info In Sip Stack Failed. Server Info Num Must Greater Than 0.");
        return SIP::RET_CODE_PARA_INVALIDATE;
    }

    //begin modify DTS2011042301313 删除上一个用户登录的信息

    m_listSvrInfoUsed.clear();
    m_vectorServerInfo.clear();

    //end of modify DTS2011042301313

    //保存注册信息
    m_stRegisterInfo = stRegisterInfo;
    //每次重新登录，需重置CallID
    m_strRegisterCallId = "";
    //此处不保存服务器信息，在m_objServerAddr中单独保存
    m_stRegisterInfo.ulServerInfoNum = 0;

    //使用设置的本地IP
    if (0 != stRegisterInfo.szLocalIp[0])
    {
        //重新设置本地IP地址
        m_objLocalAddr.m_lIpAddr = (long)ACE_OS::inet_addr(stRegisterInfo.szLocalIp);
    }
    else
    {
        m_objLocalAddr.m_lIpAddr = (long)ACE_OS::inet_addr("127.0.0.1");
    }

    //保存第一个服务器的IP和端口，网络序
    m_objServerAddr.m_lIpAddr = (long)ACE_OS::inet_addr(stRegisterInfo.stServerInfoArray[0].szServerIP);
    m_objServerAddr.m_usPort  = htons(stRegisterInfo.stServerInfoArray[0].usServerPort);

    ///////////////////////////// add by w00207027 2012-11-02 Begin ///////////////////////////////////////////////////
    m_objServerAddrEx.m_lIpAddr =0;
    m_objServerAddrEx.m_usPort = 0;
    m_bServerAddrEx = false;
    ///////////////////////////// add by w00207027 2012-11-02 End /////////////////////////////////////////////////////

    //记录使用过的服务器信息
    {
        m_listSvrInfoUsed.push_back(stRegisterInfo.stServerInfoArray[0]);
    }

	INFO_LOG("Set Register Info In Sip Stack In. ServerInfo1 = %.16s:%d.", 
		stRegisterInfo.stServerInfoArray[0].szServerIP, 
		stRegisterInfo.stServerInfoArray[0].usServerPort);

    //保存重定向列表
    m_vectorServerInfo.push_back(stRegisterInfo.stServerInfoArray[0]);
    for (unsigned long ulIndex = 1; ulIndex < stRegisterInfo.ulServerInfoNum; ++ulIndex)
    {
        //设置为重定向
        m_bHasRedirect = VOS_TRUE;
        //设置为临时重定向
        m_bTempRedirect = VOS_TRUE;   

        m_vectorServerInfo.push_back(stRegisterInfo.stServerInfoArray[ulIndex]);    //lint !e661 可变数组
		INFO_LOG("Set Register Info In Sip Stack In. ServerInfo%d = %.16s:%d.", 
			ulIndex + 1,
			stRegisterInfo.stServerInfoArray[ulIndex].szServerIP,    //lint !e661 可变数组
			stRegisterInfo.stServerInfoArray[ulIndex].usServerPort); //lint !e661 !e662 可变数组
    } 

    //计算出密码的MD5值
    Md5Hex(m_stRegisterInfo.szPassword, m_szPwdMd5);

    INFO_LOG("Set Register Info In Sip Stack Success.");
    return SIP::RET_CODE_OK;
}//lint !e1788

///////////////////////////////////////////  add by w00207012 2012-11-02 Begin //////////////////////////////////////////////////
/**
* Description:  SetRegisterInfo().  设置注册信息
* @param  [in]  bServerAddrEx  是否使用服务器地址
* @param  [in]  serverIP              服务器IP
* @param  [in]  serverPort          服务器Port
* @return       long.           返回码
*/
long CSipStack::SetServerAddrEx(bool bServerAddrEx, const string& serverIP, unsigned short serverPort)
{
    //if ((serverIP.empty()) || (0==serverPort))
    //{
    //	INFO_LOG("Set ServerAddrEx Info In Sip Stack Failed, serverIP[%s] serverPort[%d].", serverIP.c_str(), serverPort;
    //	return SIP::RET_CODE_FAIL;
    //}

    if (bServerAddrEx)
    {
        //保存服务器的IP和端口，网络序
        m_objServerAddrEx.m_lIpAddr = (long)ACE_OS::inet_addr(serverIP.c_str());
        m_objServerAddrEx.m_usPort  = htons(serverPort);
    }

    m_bServerAddrEx = bServerAddrEx;

    //modify-cwx148380-begin
    m_strIP = serverIP;
    m_usPort = serverPort;
    //modify-cwx148380-end
    INFO_LOG("Set ServerAddrEx Info In Sip Stack Success.");
    return SIP::RET_CODE_OK;
}

/**
* Description:  isServerAddrEx().  判断是否与前端交互，便于封装消息
* @return       bool.
*/
bool CSipStack::IsServerAddrEx(void)
{
    return m_bServerAddrEx;
}
///////////////////////////////////////////  add by w00207012 2012-11-02 End ////////////////////////////////////////////////////

/**
* Description:  GetTuObjId().   获取TU对象ID
* @return       unsigned long.           TU对象ID
*/
unsigned long CSipStack::GetTuObjId()
{
    return m_ulTuObjId++;
}

/**
* Description:  SendSipMsg().   发送Sip消息
* @param  [in]  stTextMsg   文本消息
* @return       long.       返回码
*/
long CSipStack::SendSipMsg(const SIP::TEXT_MSG& stTextMsg, const string& callID)
{
    //加上线程锁
    //CLockGuard objLocker(m_pThreadMutex); //统一在SendMsg中加线程锁

    //返回值
    long lResult = SIP::RET_CODE_OK;

    CSipStackMsgWrap* pSipMsg = VOS_NULL;

    try
    {
        switch ((long)stTextMsg.enTextMsgType)
        {
        case SIP::TEXT_MSG_TYPE_SIP_MESSAGE:
            {
                pSipMsg = new CSipStackMsgWrapMessage(*this, stTextMsg,callID);
                break;
            }

        case SIP::TEXT_MSG_TYPE_SIP_OPTIONS:
            {
                pSipMsg = new CSipStackMsgWrapOptions(*this, stTextMsg);
                break;
            }

        case SIP::TEXT_MSG_TYPE_SIP_INVITE:
            {
                pSipMsg = new CSipStackMsgWrapInvite(*this, stTextMsg, callID);
                break;
            }

        case SIP::TEXT_MSG_TYPE_SIP_ACK:
            {
                pSipMsg = new CSipStackMsgWrapAck(*this, stTextMsg);
                break;
            }

        case SIP::TEXT_MSG_TYPE_SIP_BYE:
            {
                pSipMsg = new CSipStackMsgWrapBye(*this, stTextMsg);
                break;
            }

        case SIP::TEXT_MSG_TYPE_SIP_REGISTER:
            {
                pSipMsg = new CSipStackMsgWrapRigister(*this, stTextMsg,callID);
                break;
            }

        case SIP::TEXT_MSG_TYPE_SIP_UNREGISTER:
            {
                pSipMsg = new CSipStackMsgWrapUnRigister(*this, stTextMsg, callID);
                break;
            }

            //             case SIP::TEXT_MSG_TYPE_SIP_INVITE_RSP:
            //                 {
            //                     pSipMsg = new CSipMsgInviteResp(*this, stTextMsg, msgSeq);
            //                     break;
            //                 }

        case SIP::TEXT_MSG_TYPE_SIP_SUBSCRIBE:
            {
                pSipMsg = new CSipStackMsgWrapSubscribe(*this, stTextMsg);
                break;
            }

        default:
            {
                pSipMsg = new CSipStackMsgWrap(*this, stTextMsg);
                break;
            }
        }
    }
    catch(...)
    {
        pSipMsg= VOS_NULL;

        ERROR_LOG("[ MsgSeq = %d] Create Sip Message Object Failed On Send SIP Message.",stTextMsg.ulMsgSeq);
        return SIP::RET_CODE_ALLOC_MEM_FAIL;//lint !e438
    }

    lResult = pSipMsg->SendMsg();//lint !e838

    //释放消息类
	delete pSipMsg;
	pSipMsg = NULL;

    return lResult;
}

unsigned long CSipStack::GetCSeq()
{
    return ++m_ulCSeq;
}

void CSipStack::GetFromSipString(const SipString &sipString, string &str)
{
    if (NULL==sipString.pcData || 0==sipString.ulLen)
    {
        str = "";
        return ;
    }

    char buf[256]={0};
    unsigned long ulLen = sipString.ulLen;
    if (ulLen > 255)
        ulLen = 255;

    strncpy(buf, sipString.pcData, ulLen);
    buf[255] = '\0';

    str = buf;
}

SIP_HEADER *CSipStack::GetSipHeader(const SipMsgS &sipMsg, bool bReq)
{
    SIP_HEADER *pHeader = new SIP_HEADER;
	INFO_LOG("New SIP_HEADER Addr = [0X%x]",pHeader);

	// get sequence
    pHeader->seq = (unsigned long)sipMsg.stHeaders.pstCseq->ulSequence;

    // get firstLine
    if (!bReq)
    {
        pHeader->iStatusCode = (int)sipMsg.uFirstLine.stStatusLine.iSipStatusCode;       
    }

    // get callID
    GetFromSipString(*(sipMsg.stHeaders.pstCallID), pHeader->callID);

    return pHeader;
}

/**
* Description:  HandleSipTxnHiSfRspInd().   处理有状态Sip响应消息
* @param  [in]  ulTuObjId   对应的TU对象ID
* @param  [in]  ulTxnObjId  对应的事务对象ID
* @param  [in]  stSipMsg    收到的SIP响应消息
* @return       void. 
*/
void CSipStack::HandleSipTxnHiSfRspInd
(
    SS_UINT32            ulTuObjId,
    SS_UINT32            ulTxnObjId,
    const SipTptNwInfoS& stTptNwInf,
    SipMsgS&             stSipMsg
)
{
    ulTuObjId   = ulTuObjId;
    ulTxnObjId  = ulTxnObjId;

    if ( VOS_NULL == stSipMsg.stHeaders.pstCseq )
    {
        ERROR_LOG("handle sip stateful respond message - cseq is null.");
        return ;
    }


    //if ( VOS_SUCCESS != HandleRspResend(stSipMsg.stHeaders.pstCseq->stMethod.usTokenId, stTptNwInf, stSipMsg.stHeaders.pstCseq->ulSequence) )
    //{
    //    WARN_LOG("handle sip stateful respond message - failure to handle respond resend.");
    //    return;
    //}

    const unsigned long ulCSeq      = stSipMsg.stHeaders.pstCseq->ulSequence;
    const unsigned long ulRespCode  = (unsigned long)stSipMsg.uFirstLine.stStatusLine.iSipStatusCode;

    //modify-123456789-begin test
     if (!m_bRegister)
     {
         WARN_LOG("[ CSeq = %d] Has Unregistered On Handle SIP stateful Response Message.",ulCSeq);
         return;
     }

     if (501 == ulRespCode)
     {
        WARN_LOG("[ CSeq = %d ] Has Unregistered On Handle SIP stateful Response Message.",ulCSeq);
     }
    //modify-123456789-end test

     
    //直接丢弃临时响应消息
    if (SIP_STATUS_SUCC_OK > ulRespCode)
    {
        INFO_LOG("[ CSeq = %d ] Receive Temporary Response. RespCode = %d",ulCSeq,ulRespCode);
        return;
    }

    INFO_LOG("[ CSeq = %d ] Handle SIP Stateful Response Message Begin. Response Code is %d",ulCSeq,ulRespCode);

    //自定义函数返回值
    long lResult = SIP::RET_CODE_OK;

    SIP_MSG_INFO* pstMsgInfo = VOS_NULL;

    //获取SIP消息信息，并将其从MAP中删除
    lResult = GetSipMsgInfoByCSeq(ulCSeq, pstMsgInfo, VOS_TRUE);//lint !e838
    if (SIP::RET_CODE_OK != lResult)
    {
        WARN_LOG("[ CSeq = %d ] Handle SIP Stateful Response Message End. Message Already Expired.",ulCSeq);
        return;
    }

    //通知消息返回码
    unsigned long ulRetCode = SIP::RET_CODE_OK;

    SIP::NOTIFY_TEXT_MSG_RESP stNotifyTextMsgResp = {0};
    //提取消息序号
    stNotifyTextMsgResp.stTextMsg.ulMsgSeq = pstMsgInfo->ulMsgSeq;

    unsigned long ulReason = 0;
    ulRetCode = (unsigned long)SIP::GetReason(stSipMsg,ulReason);//lint !e838
    if(ulRetCode == SIP::RET_CODE_OK)
    {
        stNotifyTextMsgResp.ulReason = ulReason;
    }

    // default retCode is OK. added by dhong
    ulRetCode = SIP::RET_CODE_OK;
    switch (ulRespCode)
    {
    case SIP_STATUS_SUCC_OK:
        // modified by dhong
        {
            switch ((long)pstMsgInfo->enTextMsgType)
            {
            case SIP::TEXT_MSG_TYPE_SIP_REGISTER:
            //case SIP::TEXT_MSG_TYPE_SIP_INVITE:
                {
                    ulRetCode = (unsigned long)HandleSuccessResp(stSipMsg, *pstMsgInfo, stNotifyTextMsgResp);
                    break;
                }
            default:
                break;
            }
            break;
        }

    case SIP_STATUS_CLIENT_ERR_UNAUTHORIZED:
        {
            //处理未授权响应
            lResult = HandleUnAuthResp(stSipMsg, *pstMsgInfo);

            //重发成功，则直接退出本函数
            if (SIP::RET_CODE_OK == lResult)
            {              
                //重发消息不需要释放原消息
                //SipDsmReleaseMsgRef(&stMsgInfo.pstSipMsg);
                //释放SIP消息信息结构体
                VOS_DELETE(pstMsgInfo);
                return;
            }

            ulRetCode = SIP::RET_CODE_AUTHENTICATE_ERROR;
            break;
        }

    case SIP_STATUS_CLIENT_ERR_REQUEST_TIMEOUT:
        {
            INFO_LOG("***********deal with timeout");
            ulRetCode = SIP::RET_CODE_REQUEST_TIME_OUT;
            break;
        }

    case SIP_STATUS_REDIR_MOVED_TEMPORARILY:
        {
            m_bTempRedirect = VOS_TRUE;
            INFO_LOG("***********deal with temporarily");
            //break;
        }
        //lint -fallthrough 此处处理同SIP_STATUS_REDIR_MOVED_PERMANENTLY，但不许保存重定向列表
    case SIP_STATUS_REDIR_MOVED_PERMANENTLY:
        {
            //重定向失败时的返回码
            ulRetCode = SIP::RET_CODE_FAIL;

            //获取重定向信息
            {
                CLockGuard objLocker(m_pThreadServerInfoMutex);
                lResult = SIP::GetRedirectInfo(stSipMsg, m_vectorServerInfo);
            }//lint !e1788
            if (SIP::RET_CODE_OK != lResult)
            {
                WARN_LOG("Get Redirect Info Failed On Handle SIP Stateful Response Message.");
            }

            //需要重定向
            m_bHasRedirect = VOS_TRUE;

            break;
        }

    default:
        {
            ulRetCode = SIP::RET_CODE_FAIL;

            break;
        }
    }

    //鉴权信息变更次数清零
    m_ulPlatAuthInfoChangeNum = 0;

    //进行重定向处理
    if (HasRedirectEvent(*pstMsgInfo))
    {
        VOS_DELETE(pstMsgInfo);
        pstMsgInfo = NULL;
        INFO_LOG("[ CSeq = %d] Handle SIP Stateful Response Message Success. Have Added Redirect Event.",ulCSeq);
        return;//lint !e438
    }

    //设置通知消息的返回码
    stNotifyTextMsgResp.ulRetCode = ulRetCode;
    stNotifyTextMsgResp.stTextMsg.enTextMsgType = pstMsgInfo->enTextMsgType;

    SIP_HEADER *pHeader = GetSipHeader(stSipMsg, false);
    pHeader->sipMsg = g_recvStr;
    SIP::PEER_URI_INFO  stFromUri;
    memset(&stFromUri,0x0, sizeof(SIP::PEER_URI_INFO));
    SIP::GetFromUriInfo(stSipMsg, stFromUri);
    pHeader->from = stFromUri.szUriUserName;

    ////////////////////////////////////////// add by w00207027 2012-11-03 Begin ///////////////////////////////
    if ((SIP_STATUS_SUCC_OK==ulRespCode)
        && (SIP::TEXT_MSG_TYPE_SIP_INVITE == (long)pstMsgInfo->enTextMsgType))
    {
        char* pMsgBody = VOS_NULL;
        unsigned long ulMsgBodyLen = 0;

        //获取消息体
        lResult = SIP::GetMsgBody(stSipMsg, pMsgBody, ulMsgBodyLen);
        if (SIP::RET_CODE_OK != lResult)
        {
            VOS_DELETE(pstMsgInfo);
            pstMsgInfo = NULL;
            ERROR_LOG("[ MsgSeq = %d] Get Message Body Failed On Handle SIP Invite Response Message.",ulCSeq);
            return ;//lint !e438
        }

        pHeader->pMsgBody = pMsgBody;

        unsigned long userSeq = InsertReqMsg(stSipMsg, ulTuObjId, ulTxnObjId, pstMsgInfo->ulDialogId);
        pHeader->userSeq  = userSeq;
   
    }

    //if ( SIP::TEXT_MSG_TYPE_SIP_MESSAGE == (long)pstMsgInfo->enTextMsgType )
    //{
    //    char* pMsgBody = VOS_NULL;
    //    unsigned long ulMsgBodyLen = 0;

    //    //获取消息体
    //    lResult = SIP::GetMsgBody(stSipMsg, pMsgBody, ulMsgBodyLen);
    //    if (SIP::RET_CODE_OK != lResult)
    //    {
    //        ERROR_LOG("[ MsgSeq = %d ] Get Message Body Failed On Handle SIP Invite Response Message.", ulCSeq);
    //        return ;
    //    }

    //    pHeader->pMsgBody = pMsgBody;

    //    unsigned long userSeq = insertReqMsg(stSipMsg, ulTuObjId, ulTxnObjId, pstMsgInfo->ulDialogId);
    //    pHeader->userSeq  = userSeq; 
    //}

    ////////////////////////////////////////// add by w00207027 2012-11-03 End ////////////////////////////////

    // modified by dhong
    stNotifyTextMsgResp.stTextMsg.pUserData = pHeader;    
    INFO_LOG("HandleSipTxnHiSfRspInd sipHeader seq = %d, from = %s",pHeader->seq,pHeader->from.c_str());

    //上报文本响应消息
    lResult = NotifyTextMsgResp(stNotifyTextMsgResp);

    //释放SIP消息
    SipDsmReleaseMsgRef(&pstMsgInfo->pstSipMsg);
    //释放SIP消息信息结构体内存
    VOS_DELETE(pstMsgInfo);
    pstMsgInfo = NULL;
    //释放通知信息中的内存
    //VOS_DELETE((char*&)stNotifyTextMsgResp.stTextMsg.pszMsgBody, MULTI);
    //VOS_DELETE((char*&)stNotifyTextMsgResp.stTextMsg.pMsgHeader, MULTI);

    if (SIP::RET_CODE_OK != lResult)
    {
		ERROR_LOG("[ CSeq = %d ] Handle SIP Stateful Response Message Failed. "
			"Notify Text Message Response Has Error(0x%04X).", ulCSeq, lResult);
        return;
    }

    INFO_LOG("[ CSeq = %d ] Handle SIP Stateful Response Message Success.",ulCSeq);
}//lint !e438

/**
* Description:  HandleSuccessResp().     处理成功响应
* @param  [in]      stSipMsg            SIP消息引用
* @param  [in]  stReqMsgInfo  请求消息信息引用
* @param  [out] stNotifyResp    通知文本响应结构体引用
* @return       long.       返回码
*/
long CSipStack::HandleSuccessResp
(
    SipMsgS&                    stSipMsg,
    const SIP_MSG_INFO&         stReqMsgInfo,
    SIP::NOTIFY_TEXT_MSG_RESP&  stNotifyResp
)
{
    //自定义函数返回值
    long lResult = SIP::RET_CODE_OK;

    //更新Nonce
    (void)SIP::GetNextNonce(stSipMsg, m_stPlatAuthInfo.strNonce);

    //已不需要重定向了
    m_bHasRedirect = VOS_FALSE;

    switch ((long)stReqMsgInfo.enTextMsgType)
    {
    case SIP::TEXT_MSG_TYPE_SIP_REGISTER:
        {
            lResult = HandleSuccessRespRegister(stSipMsg, stNotifyResp);
            break;
        }

    case SIP::TEXT_MSG_TYPE_SIP_INVITE:
        {
            lResult = HandleSuccessRespInvite(stSipMsg, stReqMsgInfo, stNotifyResp);
            break;
        }

        // added by dhong
    case SIP::TEXT_MSG_TYPE_SIP_OPTIONS:
        // keep alive success
        {
            //do something to indicate that keep alive success
            break;
        }

    default:
        {

            break;
        }
    }

    return lResult;
}

/**
* Description:  HandleSuccessRespRegister().     处理Register成功响应
* @param  [in]  stSipMsg        SIP消息引用
* @param  [out] stNotifyResp    通知文本响应结构体引用
* @return       long.       返回码
*/
long CSipStack::HandleSuccessRespRegister
(
    SipMsgS&                    stSipMsg,
    SIP::NOTIFY_TEXT_MSG_RESP&  stNotifyResp
)
{
    unsigned long ulExpires = 0;
    long lResult = SIP::GetExpires(stSipMsg, ulExpires);
    if (SIP::RET_CODE_OK != lResult)
    {
        WARN_LOG("Get Expires Header Failed On Handle Register Success Response.");
        //没有Expire头域，直接赋值为0，表示使用原值
        ulExpires = 0;

        //return SIP::RET_CODE_FAIL;
    }    

    //服务器信息个数
    unsigned long ulSvrInfoNum = 0;

    if (m_bRedirected)
    {
        CLockGuard objLocker(m_pThreadServerInfoMutex);
        ulSvrInfoNum = m_vectorServerInfo.size();
    }//lint !e1788

	unsigned long ulHeaderRegLen = 0;
	if(0 != ulSvrInfoNum)
	{	
		if( ULONG_MAX-sizeof(SIP::RESP_HEADER_REGISTER) > (ulSvrInfoNum-1)*sizeof(SIP::SERVER_INFO) )
		{
			ulHeaderRegLen = sizeof(SIP::RESP_HEADER_REGISTER)+(ulSvrInfoNum-1)*sizeof(SIP::SERVER_INFO);
		}
		else
		{
			ERROR_LOG("ulSvrInfoNum is too large");
			return SIP::RET_CODE_FAIL;
		}
	}
	else
	{
		ulHeaderRegLen = sizeof(SIP::RESP_HEADER_REGISTER);
	}
	    
    char* pHeaderReg = VOS_NEW(pHeaderReg, ulHeaderRegLen);
    if (VOS_NULL == pHeaderReg)
    {
        ERROR_LOG("Create Register Header Failed On Handle Register Success Response.");
        return SIP::RET_CODE_FAIL;
    }
    memset(pHeaderReg, 0, ulHeaderRegLen);

    SIP::RESP_HEADER_REGISTER* pstHeaderReg = (SIP::RESP_HEADER_REGISTER*)pHeaderReg; //lint !e826 可变数组

    pstHeaderReg->ulExpires = ulExpires;    //超时时间
    pstHeaderReg->bRedirect = m_bRedirected;//是否重定向
    //保存鉴权挑战中的Opaque
    strncpy(pstHeaderReg->szOpaque, m_stPlatAuthInfo.strOpaque.c_str(), sizeof(pstHeaderReg->szOpaque) - 1);

    if (m_bRedirected)
    {
        //设置服务器信息
        for (unsigned long ulIndex = 0; ulIndex < ulSvrInfoNum; ++ulIndex)
        {
            CLockGuard objLocker(m_pThreadServerInfoMutex);
            pstHeaderReg->stServerInfoArray[ulIndex] = m_vectorServerInfo[ulIndex];
        }//lint !e1788

        pstHeaderReg->ulServerInfoNum = ulSvrInfoNum;
    }

    //设置通知响应的消息头域
    stNotifyResp.stTextMsg.pMsgHeader       = pHeaderReg;
    stNotifyResp.stTextMsg.ulMsgHeaderLen   = ulHeaderRegLen;

    INFO_LOG("Register Expires: Old Value = %d,. New Value = %d", m_stRegisterInfo.ulExpireTime,ulExpires);

    //保存新的超时时长
    m_stRegisterInfo.ulExpireTime = ulExpires;
    //设置为已注册
    INFO_LOG("-----------------------Register success here");
    m_bRegister = VOS_TRUE;

    return SIP::RET_CODE_OK; 
}

/**
* Description:  HandleSuccessRespInvite().     处理Invite成功响应
* @param  [in]  stSipMsg        SIP消息引用
* @param  [in]  stReqMsgInfo    请求消息信息引用
* @param  [out] stNotifyResp    通知文本响应结构体引用
* @return       long.       返回码
*/
long CSipStack::HandleSuccessRespInvite
(
    SipMsgS&                    stSipMsg,
    const SIP_MSG_INFO&         stReqMsgInfo,
    SIP::NOTIFY_TEXT_MSG_RESP&  stNotifyResp
)
{
    INFO_LOG("[ MsgSeq = %d ] Handle SIP Invite Response Message Begin.",stReqMsgInfo.ulMsgSeq);

    long lResult = SIP::RET_CODE_OK;

    char* pMsgBody = VOS_NULL;
    unsigned long ulMsgBodyLen = 0;

    //获取消息体
    lResult = SIP::GetMsgBody(stSipMsg, pMsgBody, ulMsgBodyLen);//lint !e838
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("[ MsgSeq = %d ] Get Message Body Failed On Handle SIP Invite Response Message.",stReqMsgInfo.ulMsgSeq);
        return lResult;
    }

    stNotifyResp.stTextMsg.pszMsgBody   = pMsgBody;
    stNotifyResp.stTextMsg.ulMsgBodyLen = ulMsgBodyLen;

	//SIP::REQ_MSG_INFO* pReqMsgInfo=GetReqMsg(stNotifyResp.stTextMsg., false);

    //创建ACK消息对象
    SIP::TEXT_MSG stTextMsg = {0};
    stTextMsg.ulMsgSeq      = stReqMsgInfo.ulMsgSeq;
    stTextMsg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_ACK;
    stTextMsg.ulDialogId    = stReqMsgInfo.ulDialogId;
    stTextMsg.pUserData     = &stSipMsg;

    ////构造函数中初始化
    //SIP::DIALOG_INFO stDialogInfo;

    ////获取对话信息，请求消息来自远端
    //lResult = SIP::GetDialogInfo(stSipMsg, stDialogInfo, VOS_FALSE);
    //if (SIP::RET_CODE_OK != lResult)
    //{
    //	ERROR_LOG("Get Dialog Info Failed On Receive Invite Response Message.");
    //	return lResult;
    //}

    ////必须在发送消息前添加到映射表中，否则响应消息过快就会找不到该对话
    ////必须在添加映射表之间设置，添加映射时会有拷贝
    //stDialogInfo.bRecvOK = VOS_TRUE;

    //(void)VOS_MutexLock(m_pMapMutex);
    ////添加对话ID信息表映射
    //// modified by dhong 
    //// because find invite has no dialogID
    //
    ////if (0 != pstMsgInfo->ulDialogId)
    ////{
    //	m_mapDialogInfoToId[stDialogInfo] = stReqMsgInfo.ulDialogId;
    //	m_mapDialogIdToInfo[stReqMsgInfo.ulDialogId] = stDialogInfo;
    ////}
    //(void)VOS_MutexUnlock(m_pMapMutex);

    CSipStackMsgWrapAck objSipMsgAck(*this, stTextMsg);

    //发送ACK
    lResult = objSipMsgAck.SendMsg();
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("[ MsgSeq = %d] Send ACK Message Failed On Handle SIP Invite Response Message.",stReqMsgInfo.ulMsgSeq);
        return lResult;
    }

    INFO_LOG("[ MsgSeq = %d] Handle SIP Invite Response Message Success.",stReqMsgInfo.ulMsgSeq);
    return SIP::RET_CODE_OK;
}

/**
* Description:  HasRedirect().  检查是否有重定向事件
* @param  [in]  stSipMsgInfo    SIP消息信息引用
* @return       VOS_BOOLEAN.    是否  有重定向
*/
VOS_BOOLEAN CSipStack::HasRedirectEvent
(
    const SIP_MSG_INFO& stSipMsgInfo
)
{
    //进行重定向处理
    if (m_bHasRedirect)
    {
        m_bHasRedirect = VOS_FALSE;
        {
            CLockGuard objLocker(m_pThreadServerInfoMutex);

            for (unsigned long ulIndex = 0; ulIndex < m_vectorServerInfo.size(); ++ulIndex)
            {
                {
                    CLockGuard listobjLocker(m_pThreadlistSvrInfoMutex);

                    SIP::SERVER_INFO& stSvrInfo = m_vectorServerInfo[ulIndex];
                    LIST_SVR_INFO::iterator iter1 = find_if(m_listSvrInfoUsed.begin(),  m_listSvrInfoUsed.end(), FindServerInfo(stSvrInfo));
                    if (iter1 != m_listSvrInfoUsed.end())
                    {
                        continue;
                    }

                    m_listSvrInfoUsed.push_back(m_vectorServerInfo[ulIndex]);
                }//lint !e1788


                //保存第一个服务器的IP和端口，网络序
                m_objServerAddr.m_lIpAddr = (long)ACE_OS::inet_addr(m_vectorServerInfo[ulIndex].szServerIP);
                m_objServerAddr.m_usPort  = htons(m_vectorServerInfo[ulIndex].usServerPort);

                m_bHasRedirect  = VOS_TRUE;

                //永久重定向，才设置为重定向过
                if (!m_bTempRedirect)
                {
                    m_bRedirected   = VOS_TRUE; //已重定向过
                }

                //找到直接退出
                break;
            }
        }//lint !e1788

        if (m_bHasRedirect)
        {
            SIP::EVENT_INFO stEventInfo(SIP::EVENT_TYPE_STACK_REDIRECT, (void*)&stSipMsgInfo, sizeof(stSipMsgInfo));//lint !e1773
            //添加重定向事件
            long lResult = AddEvent(stEventInfo);
            if (SIP::RET_CODE_OK != lResult)
            {
                WARN_LOG("Add Redirect Event Failed On Handle SIP Stateful Response Message.");
                m_bHasRedirect = VOS_FALSE;
            }
        }
    }

    return m_bHasRedirect;
}

/**
* Description:  HandleUnAuthResp().     处理未授权响应
* @param  [in]  stSipMsg            SIP消息引用
* @param  [in]  stReqMsgInfo  请求消息信息引用
* @return       long.       返回码
*/
long CSipStack::HandleUnAuthResp
(
    SipMsgS&        stSipMsg,
    SIP_MSG_INFO&   stReqMsgInfo,
    bool            bFirst
)
{
    long lResult = SIP::RET_CODE_OK;

    //获取鉴权信息
    lResult = SIP::GetPlatAuthInfo(stSipMsg, m_stPlatAuthInfo);//lint !e838

    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("Get Plat Authentication Info Failed On Handle UnAuthentication Response.");
        return lResult;
    }

    //同一次鉴权信息不能超过指定次数
    if (SIP::MAX_NUM_AUTH_INFO_CHANGE < m_ulPlatAuthInfoChangeNum)
    {
        ERROR_LOG("Handle UnAuthentication Response Failed. Plat Authentication Info Changed More Than %d",SIP::MAX_NUM_AUTH_INFO_CHANGE); 
        return SIP::RET_CODE_AUTHENTICATE_ERROR;
    }

    //鉴权信息变更次数累计
    m_ulPlatAuthInfoChangeNum++;

    SIP::TEXT_MSG stTextMsg = {0};
    stTextMsg.ulMsgSeq      = stReqMsgInfo.ulMsgSeq;
    stTextMsg.enTextMsgType = stReqMsgInfo.enTextMsgType;
    stTextMsg.ulDialogId    = stReqMsgInfo.ulDialogId;
    stTextMsg.pUserData     = stReqMsgInfo.pstSipMsg;

    CSipStackMsgWrapReAuth objSipMsg(*this, stTextMsg);

    lResult = objSipMsg.SendMsg();
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("Send Request Message With Authentication Failed On Handle UnAuthentication Response Message.");
        return lResult;
    }


    return lResult;
}//lint !e1764

/**
* Description:  HandleUnAuthResp().     处理未授权响应
* @param  [in]  stSipMsg            SIP消息引用
* @param  [in]  stReqMsgInfo  请求消息信息引用
* @return       long.       返回码
*/
// not used now added by dhong
long CSipStack::HandleReRegister(void)
{
    return 0;

    //     long lResult = SIP::RET_CODE_OK;
    // 
    //     //获取鉴权信息
    //     //    lResult = SIP::GetPlatAuthInfo(m_authMsg, m_stPlatAuthInfo);
    // 
    //     if (SIP::RET_CODE_OK != lResult)
    //     {
    //         ERROR_LOG("Get Plat Authentication Info Failed On Handle Unauthentication Response.");
    //         return lResult;
    //     }
    // 
    //     //同一次鉴权信息不能超过指定次数
    //     if (SIP::MAX_NUM_AUTH_INFO_CHANGE < m_ulPlatAuthInfoChangeNum)
    //     {
    //         ERROR_LOG("Handle Unauthentication Response Failed. Plat Authentication Info Changed More Than %d.", 
    //                 SIP::MAX_NUM_AUTH_INFO_CHANGE);
    //         return SIP::RET_CODE_AUTHENTICATE_ERROR;
    //     }
    // 
    //     //鉴权信息变更次数累计
    //     m_ulPlatAuthInfoChangeNum++;
    // 
    //     SIP::TEXT_MSG stTextMsg = {0};
    //     stTextMsg.ulMsgSeq      = m_msgInfo.ulMsgSeq;
    //     stTextMsg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_REGISTER;//stReqMsgInfo.enTextMsgType;
    //     stTextMsg.ulDialogId    = m_msgInfo.ulDialogId;
    //     stTextMsg.pUserData     = m_msgInfo.pstSipMsg;
    // 
    //     CSipMsgReAuth objSipMsg(*this, stTextMsg);
    // 
    //     lResult = objSipMsg.SendMsg();
    //     if (SIP::RET_CODE_OK != lResult)
    //     {
    //         ERROR_LOG("Send Request Message With Authenication Failed "
    //                 "On Handle Unauthentication Response Message.");
    //         return lResult;
    //     }
    // 
    //     return lResult;
}

// added by dhong
long CSipStack::HandleAuthOnLineResp
(
    const SipMsgS&        stSipMsg,
    SIP_MSG_INFO&   stReqMsgInfo
)
{
    long lResult = SIP::RET_CODE_OK;

    //......
    /*
    lResult = SIP::GetPlatAuthInfo(stSipMsg, m_stPlatAuthInfo);

    if (SIP::RET_CODE_OK != lResult)
    {
    ERROR_LOG("Get Plat Authentication Info Failed On Handle Unauthentication Response.");
    return lResult;
    }
    */

    //
    if (SIP::MAX_NUM_AUTH_INFO_CHANGE < m_ulPlatAuthInfoChangeNum)
    {
        ERROR_LOG("Handle Unauthentication Response Failed. Plat Authentication Info Changed More Than %d",SIP::MAX_NUM_AUTH_INFO_CHANGE); 
        return SIP::RET_CODE_AUTHENTICATE_ERROR;
    }

    //..........
    m_ulPlatAuthInfoChangeNum++;

    SIP::TEXT_MSG stTextMsg = {0};
    stTextMsg.ulMsgSeq      = stReqMsgInfo.ulMsgSeq;
    stTextMsg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_REGISTER;//stReqMsgInfo.enTextMsgType;
    stTextMsg.ulDialogId    = stReqMsgInfo.ulDialogId;
    stTextMsg.pUserData     = stReqMsgInfo.pstSipMsg;

    CSipStackMsgWrapReAuth objSipMsg(*this, stTextMsg);

    lResult = objSipMsg.SendMsg();//lint !e838
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("Send Request Message With Authenication Failed  On Handle Unauthentication Response Message.");
        return lResult;
    }

    return lResult;
}//lint !e1764
/**
* Description:  HandleSipTxnHiSfReqInd().   处理有状态Sip请求消息
* @param  [in]  ulTuObjId   对应的TU对象ID
* @param  [in]  ulTxnObjId  对应的事务对象ID
* @param  [in]  stSipMsg    收到的SIP请求消息
* @return       void.
*/
// dhong modified tomorrow here 2012-5-16
void CSipStack::HandleSipTxnHiSfReqInd
(
    SS_UINT32            ulTuObjId,
    SS_UINT32            ulTxnObjId,
    const SipTptNwInfoS& stTptNwInf,
    SipMsgS&             stSipMsg
)
{
    if ( VOS_NULL == stSipMsg.stHeaders.pstCseq )
    {
        ERROR_LOG("handle sip stateful request message - cseq is null.");
        return ;
    }
	
    if ( SS_CTRL_MSG_MGR_INS_REF.IsControl2Drop(stSipMsg) )
    {
        WARN_LOG("handle sip stateful request message - message control to drop.");
        //生成响应消息
        CSipStackMsgWrapResp objSipRspMsg(*this, stSipMsg);
        //发送响应消息，不需处理返回值
        (void)objSipRspMsg.SendRspMsg(SIP_STATUS_SERVER_ERR_SERVICE_UNAVAILABLE, ulTuObjId, ulTxnObjId);
        return ;
    }

    INFO_LOG("handle sip stateful request message - begin - tu_id=%d,txn_id=%d",ulTuObjId,ulTxnObjId);

    long lResult = SIP::RET_CODE_OK;

    const unsigned long ulSipMethod = stSipMsg.uFirstLine.stRequestLine.stMethod.usTokenId;
    if (SIP_METHOD_BUTT <= ulSipMethod)
    {
        ERROR_LOG("handle sip stateful request message - unsupported sip method - method=%d",ulSipMethod);
        return;
    }

    //文本消息类型
    SIP::NOTIFY_TEXT_MSG_REQ stNotifyTextMsgReq = {0};
    stNotifyTextMsgReq.ulMsgSeq                 = stSipMsg.stHeaders.pstCseq->ulSequence;
    stNotifyTextMsgReq.enTextMsgType            = SIP::ARR_SIP_METHOD_TO_TEXT_TYPE[ulSipMethod];
	stNotifyTextMsgReq.pUserData = NULL;

    switch (ulSipMethod)
    {
    case SIP_METHOD_REGISTER:
        {
            lResult = HandleReqRegister(ulTuObjId, ulTxnObjId, stTptNwInf, stSipMsg, stNotifyTextMsgReq);
			if(NULL != stNotifyTextMsgReq.pUserData)
			{
				SipStack::SIP_HEADER *pHeader = (SipStack::SIP_HEADER*)(stNotifyTextMsgReq.pUserData);
				INFO_LOG("delete pHeader = [0X%x]",pHeader);
				delete pHeader;
				pHeader = NULL;
			}
            break;
        }

    case SIP_METHOD_NOTIFY:
        {
        	lResult = HandleReqNotify(ulTuObjId, ulTxnObjId, stTptNwInf, stSipMsg, stNotifyTextMsgReq);
        	break;
        }

    case SIP_METHOD_OPTIONS:
        {
            lResult = HandleReqOption(ulTuObjId, ulTxnObjId, stTptNwInf, stSipMsg, stNotifyTextMsgReq);
            break;
        }

    case SIP_METHOD_INVITE:
        {   
            lResult = HandleReqInvite(ulTuObjId, ulTxnObjId, stSipMsg, stNotifyTextMsgReq);
            break;
        }

    case SIP_METHOD_MESSAGE:
        {
            lResult = HandleReqMessage(ulTuObjId, ulTxnObjId, stTptNwInf, stSipMsg, stNotifyTextMsgReq);
            break;
        }

        // Cance 和Bye一起处理 added by dhong
    case SIP_METHOD_BYE: 
    case SIP_METHOD_CANCEL:
        {
            lResult = HandleReqBye(ulTuObjId, ulTxnObjId, stSipMsg, stNotifyTextMsgReq);
            break;
        }

    default:
        {
			//生成响应消息
			CSipStackMsgWrapResp objSipRspMsg(*this, stSipMsg);
			//发送响应消息，不需处理返回值
			(void)objSipRspMsg.SendRspMsg(SIP_STATUS_SUCC_OK, ulTuObjId, ulTxnObjId);
			break;
        }
    }

    if (SIP::RET_CODE_OK != lResult)
    {
        WARN_LOG("handle sip stateful request message - failure to handle - Error= %d",lResult);
        return;
    }

    INFO_LOG("handle sip stateful request message - end");
}

/**
* Description:  HandleSipTxnHiSlReqInd().   处理无状态Sip请求消息
* @param  [in]  stSipMsg    收到的SIP请求消息
* @return       void.
*/
void CSipStack::HandleSipTxnHiSlReqInd
(
    const SipTptNwInfoS& stTptNwInf,
    SipMsgS&             stSipMsg
)
{
    if ( VOS_NULL == stSipMsg.stHeaders.pstCseq )
    {
        ERROR_LOG("handle sip stateless request message - cseq is null.");
        return ;
    }
    //if ( VOS_SUCCESS != HandleReqResend(stSipMsg.uFirstLine.stRequestLine.stMethod.usTokenId, stTptNwInf, stSipMsg.stHeaders.pstCseq->ulSequence) )
    //{
    //    WARN_LOG("handle sip stateless request message - failure to handle resend.");
    //    return;
    //}

    INFO_LOG("handle sip stateless request message - begin.");

    long lResult = SIP::RET_CODE_OK;

    const unsigned long ulSipMethod = stSipMsg.uFirstLine.stRequestLine.stMethod.usTokenId;
    const unsigned long ulCSeq = stSipMsg.stHeaders.pstCseq->ulSequence;

    if (SIP_METHOD_BUTT <= ulSipMethod)
    {
        ERROR_LOG("handle sip stateless request message - unsupported sip method - method=%d",ulSipMethod);
        return;
    }

    //文本消息类型
    const SIP::TEXT_MSG_TYPE enTextMsgType = SIP::ARR_SIP_METHOD_TO_TEXT_TYPE[ulSipMethod];

    SIP::NOTIFY_TEXT_MSG_REQ stNotifyTextMsgReq = {0};
    stNotifyTextMsgReq.ulMsgSeq         = ulCSeq;
    stNotifyTextMsgReq.enTextMsgType    = enTextMsgType;
	stNotifyTextMsgReq.pUserData = NULL;

    lResult = GetDialogIdBySipMsg(stSipMsg, stNotifyTextMsgReq.ulDialogId);//lint !e838
    if (SIP::RET_CODE_OK != lResult)
    {
        //invite's ack has no dialog ID
        INFO_LOG("handle sip stateless request message - can't find dialog id.");
    }

    // added by dhong
    SIP_HEADER *pHeader = GetSipHeader(stSipMsg, true);
    pHeader->sipMsg = g_recvStr;
    // modified by dhong
    stNotifyTextMsgReq.pUserData = pHeader;

    //上报文本响应消息
    lResult = NotifyTextMsgReq(stNotifyTextMsgReq);
	stNotifyTextMsgReq.pUserData = NULL;
	INFO_LOG("delete pHeader = [0X%x]",pHeader); 
	delete pHeader;		
	pHeader = NULL;


    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("handle sip stateless request message - failure to handle - Error= %d",lResult);
        return;
    }

    INFO_LOG("handle sip stateful request message - end");
}

//////////////////////////////////////// add by wx153027 2012-12-18 begin  ///////////////////////////////////
/**
* Description:  HandleReqOption().  处理Option请求消息
* @param  [in]  ulTuObjId   对应的TU对象ID
* @param  [in]  ulTxnObjId  对应的事务对象ID
* @param  [in]      stSipMsg            SIP消息引用
* @param  [out] stNotifyReq     通知文本请求结构体引用
* @return       long.       返回码
*/
long CSipStack::HandleReqOption
(
    SS_UINT32                   ulTuObjId,
    SS_UINT32                   ulTxnObjId,
    const SipTptNwInfoS&        stTptNwInf,
    SipMsgS&                    stSipMsg,
    SIP::NOTIFY_TEXT_MSG_REQ&   stNotifyReq
)
{
    INFO_LOG("handle option request - begin.");

    unsigned long userSeq = InsertReqMsg(stSipMsg, ulTuObjId, ulTxnObjId, stNotifyReq.ulDialogId);

    SIP_HEADER *pHeader = GetSipHeader(stSipMsg, true);

    if ( VOS_NULL == pHeader )
    {
        ERROR_LOG("handle options request - header is null.");
        return VOS_FAIL;
    }

    pHeader->userSeq = userSeq;
    if ( VOS_SUCCESS != GetAddrWithSipTptIpPortS(stTptNwInf.stSrcAddr, pHeader->devIP, pHeader->devPort) )
    {
        ERROR_LOG("handle options request - failure to get address info");
        return VOS_FAIL;
    }
    //SipVia* pstSipVia = NULL;
    //long nResult = VppListGetData(stSipMsg.stHeaders.hdlViaList, 0, (void**)&pstSipVia);
    //pHeader->devPort = pstSipVia->stSentBy.iPort;
    //{
    //    ACE_INET_Addr addr;
    //    if (SIP_ADDR_TYPE_IPV4 == pstSipVia->stSentBy.stHost.enHostType)
    //    {
    //        SS_UINT8 pIP4 [SS_IPV4_ADDR_LEN];
    //        for (int i = 0;i<SS_IPV4_ADDR_LEN;i++)
    //        {
    //            pIP4[i] = pstSipVia->stSentBy.stHost.uHostContent.ipv4[SS_IPV4_ADDR_LEN-1-i];
    //        }
    //        addr.set_address((char*)pIP4, SS_IPV4_ADDR_LEN);
    //    }
    //    else if (SIP_ADDR_TYPE_IPV6 == pstSipVia->stSentBy.stHost.enHostType)
    //    {
    //        SS_UINT8 pIP6 [SS_IPV6_ADDR_LEN];
    //        for (int i = 0; i<SS_IPV6_ADDR_LEN; i++)
    //        {
    //            pIP6[i] = pstSipVia->stSentBy.stHost.uHostContent.ipv6[SS_IPV6_ADDR_LEN-1-i];
    //        }
    //        addr.set_address((char*)pIP6, SS_IPV6_ADDR_LEN);
    //    } 
    //    else
    //    {
    //        ERROR_LOG("Get Contact URI's IP Info Failed. SIP URI is Empty.");
    //        return SIP::RET_CODE_FAIL;
    //    }
    //    pHeader->devIP = addr.get_host_addr();
    //}

    SIP::PEER_URI_INFO  stFromUri;
    memset(&stFromUri,0x0, sizeof(SIP::PEER_URI_INFO));
    SIP::GetFromUriInfo(stSipMsg, stFromUri);
    pHeader->from = stFromUri.szUriUserName;
    pHeader->sipMsg = g_recvStr;
    stNotifyReq.pUserData = pHeader;

    INFO_LOG("handle option request - get uri from - From= %s",pHeader->from.c_str());

    //上报文本响应消息
    long lResult = NotifyTextMsgReq(stNotifyReq);


    return lResult;
}
//////////////////////////////////////// add by wx153027 2012-12-18 end  ///////////////////////////////////

//////////////////////////////////////// add by w00207027 2012-10-27 Begin  ///////////////////////////////////
/**
* Description:  HandleReqRegister().  处理Register请求消息
* @param  [in]  ulTuObjId   对应的TU对象ID
* @param  [in]  ulTxnObjId  对应的事务对象ID
* @param  [in]      stSipMsg            SIP消息引用
* @param  [out] stNotifyReq     通知文本请求结构体引用
* @return       long.       返回码
*/
long CSipStack::HandleReqRegister
(
    SS_UINT32                   ulTuObjId,
    SS_UINT32                   ulTxnObjId,
    const SipTptNwInfoS&        stTptNwInf,
    SipMsgS&                    stSipMsg,
    SIP::NOTIFY_TEXT_MSG_REQ&   stNotifyReq
)
{
    INFO_LOG("handle register request - begin.");

    SipAuthorization * pSipAuthorization = NULL;

	if(0 == strlen(m_stRegisterInfo.szRemoteLoginName))
	{
		CSipStackMsgWrapRegisterResp objSipRspMsg(*this, stSipMsg, true);
		(void)objSipRspMsg.SendRspMsg(SIP_STATUS_SUCC_OK, ulTuObjId, ulTxnObjId);
		INFO_LOG("handle register request - no authenticate header.");
		return SIP::RET_CODE_OK;
	}

    if(  ( SIP::RET_CODE_OK != CheckRegistAuthInfo(stSipMsg, pSipAuthorization) )
      || ( NULL == pSipAuthorization )
      || ( !pSipAuthorization->bIsAuthDigestType )
      )
    {
        CSipStackMsgWrapRegisterResp objSipRspMsg(*this, stSipMsg, true);
        (void)objSipRspMsg.SendRspMsg(SIP_STATUS_CLIENT_ERR_UNAUTHORIZED, ulTuObjId, ulTxnObjId);
        INFO_LOG("handle register request - can't find authenticate header .");
        return SIP::RET_CODE_FAIL;
    }

    long lResult = SIP::RET_CODE_FAIL;

    // 若收到摘要认证消息，则将消息上报业务层处理
    unsigned long ulUserSeq = InsertReqMsg(stSipMsg, ulTuObjId, ulTxnObjId, stNotifyReq.ulDialogId);

    SIP_HEADER *pHeader = GetSipHeader(stSipMsg, true);
    pHeader->userSeq = ulUserSeq;
    SIP::GetExpires(stSipMsg, pHeader->expires);
    //SIP::GetContact(stSipMsg, pHeader->devIP, pHeader->devPort);
    if ( VOS_SUCCESS != GetAddrWithSipTptIpPortS( stTptNwInf.stSrcAddr,pHeader->devIP,pHeader->devPort) )
    {
        ERROR_LOG("handle register request - failure to get address info.");
        return VOS_FAIL;
    }
    SIP::PEER_URI_INFO  stFromUri;
    memset(&stFromUri,0x0, sizeof(SIP::PEER_URI_INFO));
    SIP::GetFromUriInfo(stSipMsg, stFromUri	);
    pHeader->from = stFromUri.szUriUserName;	
    INFO_LOG("Handle REGISTER Message sipHeader from = %s",pHeader->from.c_str());
    GetStringFromQuoteString(*(pSipAuthorization->pstrUsername), pHeader->digestUserName);
    GetStringFromQuoteString(*(pSipAuthorization->pstrRealm), pHeader->digestRealm);
    GetStringFromQuoteString(*(pSipAuthorization->pstrNonce), pHeader->digestNonce);
    GetStringFromQuoteString(*(pSipAuthorization->pstrDigestUri), pHeader->digestUri);
    GetStringFromQuoteString(*(pSipAuthorization->pstrDResponse), pHeader->digestResponse);
    //getFromSipString(*(sipAuthorization->pstrOpaque), pHeader->digestOpaque);
    //getFromSipString(*(sipAuthorization->pstrCNonce), pHeader->digestCNonce);
    //getFromSipString(*(sipAuthorization->pstrNonceCount), pHeader->digestNonceCount);
    //GetStringFromQuoteString(*(pSipAuthorization->pstrMessageQOP), pHeader->digestQop);
    //pHeader->digestUserName = "34020000001230000001";
    //pHeader->digestRealm = "huawei.com";
    //pHeader->digestNonce = "oc1mA0GPoEjkOhZ1sjDFhlkezSw8TIK7";
    //pHeader->digestUri = "sip:340200000020000000001@3402000000";
    //modify-123456789-cwx148380-begin
    if ( NULL != pSipAuthorization->pstrOpaque )
    {
        GetStringFromQuoteString(*(pSipAuthorization->pstrOpaque), pHeader->digestOpaque);
    }
    
    if ( NULL != pSipAuthorization->pstrCNonce )
    {
        GetStringFromQuoteString(*(pSipAuthorization->pstrCNonce), pHeader->digestCNonce);
    }
    
    if ( NULL != pSipAuthorization->pstrNonceCount )
    {
        GetFromSipString(*(pSipAuthorization->pstrNonceCount), pHeader->digestNonceCount);
    }
    
    if ( NULL != pSipAuthorization->pstrMessageQOP )
    {
         GetFromSipString(*(pSipAuthorization->pstrMessageQOP), pHeader->digestQop);
    }
   
    //若algorithm为空，则默认使用MD5算法。
    if ( NULL != pSipAuthorization->pstrAlgorithm )
    {
        GetStringFromQuoteString(*(pSipAuthorization->pstrAlgorithm), pHeader->digestAlgorithm);
    }
    else
    {
        pHeader->digestAlgorithm = "MD5";
    }

    //计算Response
    HASHHEX HA1;
    HASHHEX Response;

    DigestCalcHA1((char*)"",//lint !e1773
                  m_stRegisterInfo.szRemoteLoginName, (char*)pHeader->digestRealm.c_str(), //lint !e1773
    	          m_stRegisterInfo.szRemotePassword, (char*)"", (char*)"", HA1);//lint !e1773
    DigestCalcResponse(HA1, (char*)pHeader->digestNonce.c_str(), (char*)pHeader->digestNonceCount.c_str(),//lint !e1773
        (char*)pHeader->digestCNonce.c_str(), //lint !e1773
    	(char*)pHeader->digestQop.c_str(), (char*)"REGISTER",//lint !e1773
    	(char*)pHeader->digestUri.c_str(), (char*)"", Response);//lint !e1773
	std::string strResponse(Response);
	if(strResponse == pHeader->digestResponse)
	{
		CSipStackMsgWrapRegisterResp objSipRspMsg(*this, stSipMsg, true);
		(void)objSipRspMsg.SendRspMsg(SIP_STATUS_SUCC_OK, ulTuObjId, ulTxnObjId);
		INFO_LOG("handle register request - 200 OK .");
		return SIP::RET_CODE_OK;
	}
	else
	{
		CSipStackMsgWrapRegisterResp objSipRspMsg(*this, stSipMsg, true);
		(void)objSipRspMsg.SendRspMsg(SIP_STATUS_CLIENT_ERR_FORBIDDEN, ulTuObjId, ulTxnObjId);
		WARN_LOG("Receive Response[%s] is not equal %s",pHeader->digestResponse.c_str(),strResponse.c_str());
		return SIP::RET_CODE_FAIL;
	}

    pHeader->sipMsg = g_recvStr;
    stNotifyReq.pUserData = pHeader;

    //上报文本响应消息
    lResult = NotifyTextMsgReq(stNotifyReq);//lint !e838

    if (SIP::RET_CODE_OK != lResult)
    {
        CSipStackMsgWrapRegisterResp objSipRspMsg(*this, stSipMsg, false);
        (void)objSipRspMsg.SendRspMsg(SIP_STATUS_CLIENT_ERR_BAD_REQUEST, ulTuObjId, ulTxnObjId);
        ERROR_LOG("handle register request - bad request - Error= %d",lResult);
        return SIP::RET_CODE_FAIL;
    }

    INFO_LOG("handle register request - end.");
    return SIP::RET_CODE_OK;
}


/**
* Description:  getStringFromQuoteString().  校验Regist请求的Authenticate信息
* @param  [in]      sipString           SIP消息引用
* @param  [out]    str                      提出后Quote的string
* @return    long   .       返回码
*/
long  CSipStack::GetStringFromQuoteString(const SipString &sipString, string &str)
{
    if (NULL==sipString.pcData || 0==sipString.ulLen)
    {
        ERROR_LOG("Fail to parse string from SIP's Quote String.");
        return SIP::RET_CODE_FAIL;
    }

    //去掉末尾双引号
    sipString.pcData[sipString.ulLen -1] = '\0';

    //计算buffer的最大长度
    unsigned long ulBufLen =sipString.ulLen;

    //加上结束符的长度
    ulBufLen += 1;

    char* pBuffer = VOS_NEW(pBuffer, ulBufLen);
    if (VOS_NULL == pBuffer)
    {
        ERROR_LOG("Create Buffer Failed On SIP's Quote String.");
        return SIP::RET_CODE_FAIL;
    }

    //必须带字符串结束符
    memset(pBuffer, 0, ulBufLen);
    strncpy(pBuffer, sipString.pcData + 1, sipString.ulLen - 2);
    str = pBuffer;

    //释放buffer
    VOS_DELETE(pBuffer, MULTI);

    return SIP::RET_CODE_OK;
}

/**
* Description:  CheckRegistAuthInfo().  校验Regist请求的Authenticate信息
* @param  [in]      stSipMsg            SIP消息引用
* @param  [out]  sipAuthorization   Regist请求的Authenticate信息
* @return       long.       返回码
*/
long CSipStack::CheckRegistAuthInfo
(
    SipMsgS&                    stSipMsg,
    SipAuthorization*&          pstSipAuthorization
)
{
    long lResult = SIP::RET_CODE_OK;

    SipAuthorizationListHdl pAuthorizationListHdl = (SipAuthorizationListHdl)SipDsmGetHdrFromMsg(SIP::EX_HDR_ID_AUTHORIZATION, &stSipMsg);

    // 若不带鉴权信息，则认为消息不合法
    if (VOS_NULL == pAuthorizationListHdl)
    {
        ERROR_LOG("Fail to get Authenticate Header Info.");
        return SIP::RET_CODE_FAIL;
    }

    //VPP函数返回值
    VPP_UINT32 nResult = VPP_SUCCESS;

    nResult = VppListGetData(pAuthorizationListHdl, 0, (void**)&pstSipAuthorization);//lint !e838
    if ((VPP_SUCCESS != nResult) || (NULL == pstSipAuthorization))
    {
        ERROR_LOG("Fail to get Authenticate Header Value Info. ErrorCode = %d",nResult);
        return SIP::RET_CODE_FAIL;
    }

    //modify-cwx148380-begin
    //if (VOS_NULL == pstSipAuthorization->pstrAlgorithm)
    //{
    //    ERROR_LOG("Fail to get Authentication Info Failed. Algorithm is Empty.";
    //    return SIP::RET_CODE_FAIL;
    //}
    //modify-cwx148380-end

    // 若为Digest类型，则需要username、realm、nonce、uri、response、algorithm
    if (pstSipAuthorization->bIsAuthDigestType)
    {
        if (VOS_NULL == pstSipAuthorization->pstrUsername)
        {
            ERROR_LOG("Fail to get Authentication Info Failed. Digest Username is Empty.");
            return SIP::RET_CODE_FAIL;
        }

        if (VOS_NULL == pstSipAuthorization->pstrRealm)
        {
            ERROR_LOG("Fail to get Authentication Info Failed. Digest Realm is Empty.");
            return SIP::RET_CODE_FAIL;
        }

        if (VOS_NULL == pstSipAuthorization->pstrNonce)
        {
            ERROR_LOG("Fail to get Authentication Info Failed. Digest Nonce is Empty.");
            return SIP::RET_CODE_FAIL;
        }

        if (VOS_NULL == pstSipAuthorization->pstrDigestUri)
        {
            ERROR_LOG("Fail to get Authentication Info Failed. Digest Uri is Empty.");
            return SIP::RET_CODE_FAIL;
        }

        if (VOS_NULL == pstSipAuthorization->pstrDResponse)
        {
            ERROR_LOG("Fail to get Authentication Info Failed. Digest Response is Empty.");
            return SIP::RET_CODE_FAIL;
        }

        //if (VOS_NULL == pstSipAuthorization->pstrAuthScheme)
        //{
        //	ERROR_LOG("Fail to get Authentication Info Failed. Digest Algorithm is Empty.";
        //	return SIP::RET_CODE_FAIL;
        //}
    }

    return lResult;
}

/**
* Description:  HandleReqNotify().  处理Notify请求消息
* @param  [in]  ulTuObjId   对应的TU对象ID
* @param  [in]  ulTxnObjId  对应的事务对象ID
* @param  [in]      stSipMsg            SIP消息引用
* @param  [out] stNotifyReq     通知文本请求结构体引用
* @return       long.       返回码
*/
long CSipStack::HandleReqNotify
(
    SS_UINT32                   ulTuObjId,
    SS_UINT32                   ulTxnObjId,
    const SipTptNwInfoS&        stTptNwInf,
    SipMsgS&                    stSipMsg,
    SIP::NOTIFY_TEXT_MSG_REQ&   stNotifyReq
)
{
    INFO_LOG("Handle Notify Message Begin.");

    long lResult = SIP::RET_CODE_OK;

    char* pMsgBody = VOS_NULL;
    unsigned long ulMsgBodyLen = 0;

    //获取消息体
    lResult = SIP::GetMsgBody(stSipMsg, pMsgBody, ulMsgBodyLen);//lint !e838
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("Get Message Body Failed On Handle NOTIFY Message.");
        return lResult;
    }

    //设置消息体
    // added by dhong
    SIP_HEADER *pHeader = GetSipHeader(stSipMsg, true);
    pHeader->sipMsg = g_recvStr;
    pHeader->pMsgBody = pMsgBody;

    if ( VOS_SUCCESS != GetAddrWithSipTptIpPortS( stTptNwInf.stSrcAddr,
        pHeader->devIP,
        pHeader->devPort
        )
        )
    {
        ERROR_LOG("handle register request - failure to get address info.");
        return VOS_FAIL;
    }

    SIP::PEER_URI_INFO  stFromUri;
    memset(&stFromUri,0x0, sizeof(SIP::PEER_URI_INFO));
    SIP::GetFromUriInfo(stSipMsg, stFromUri);
    pHeader->from = stFromUri.szUriUserName;
    INFO_LOG("Handle NOTIFY Message sipHeader seq = %d,from = %s",pHeader->seq,pHeader->from.c_str());
    //pHeader->digestRealm = stFromUri.szUriHostName;
    pHeader->devDomain = "huawei.com";
    // modified by dhong
    stNotifyReq.pUserData = pHeader;

    stNotifyReq.pszMsgBody = NULL; // pMsgBody;
    stNotifyReq.ulMsgBodyLen = ulMsgBodyLen;

    ///////////////////////////////////  modified by w00207027 2012-11-03  Begin /////////////////////////////////////
    //modified by cwx153028 begin
    if (SIP::RET_CODE_OK == IsRespDirect(pMsgBody))
    {
        //生成响应消息
        CSipStackMsgWrapResp objSipRspMsg(*this, stSipMsg);
        //发送响应消息，不需处理返回值
        (void)objSipRspMsg.SendRspMsg(SIP_STATUS_SUCC_OK, ulTuObjId, ulTxnObjId);
    }
    else
    {
        unsigned long userSeq = InsertReqMsg(stSipMsg, ulTuObjId, ulTxnObjId, stNotifyReq.ulDialogId);
        pHeader->userSeq = userSeq;
        //SIP::GetContact(stSipMsg, pHeader->devIP, pHeader->devPort);
    }
    //modified by cwx153028 end
    ///////////////////////////////////  modified by w00207027 2012-11-03  End /////////////////////////////////////

    ////生成响应消息
    //CSipRspMsg objSipRspMsg(*this, stSipMsg);
    ////发送响应消息，不需处理返回值
    //(void)objSipRspMsg.SendRspMsg(SIP_STATUS_SUCC_OK, ulTuObjId, ulTxnObjId);

    //上报文本响应消息
    INFO_LOG("============Notify sipHeader seq = %d, from = %s",pHeader->seq,pHeader->from.c_str());
    lResult = NotifyTextMsgReq(stNotifyReq);

    //释放消息体内存
    // dhong I think this should be invoked
    // VOS_DELETE(pMsgBody, MULTI);

    INFO_LOG("Handle Notify Message End.");
    return lResult;
}

//////////////////////////////////////// add by w00207027 2012-10-27 End //////////////////////////////////////
/**
* Description:  HandleReqInvite().  处理Invite请求消息
* @param  [in]  ulTuObjId   对应的TU对象ID
* @param  [in]  ulTxnObjId  对应的事务对象ID
* @param  [in]      stSipMsg            SIP消息引用
* @param  [out] stNotifyReq     通知文本请求结构体引用
* @return       long.       返回码
*/
long CSipStack::HandleReqInvite
(
    SS_UINT32                   ulTuObjId,
    SS_UINT32                   ulTxnObjId,
    SipMsgS&                    stSipMsg,
    SIP::NOTIFY_TEXT_MSG_REQ&   stNotifyReq
)
{
    INFO_LOG("Handle Invite Message Begin.");

    long lResult = SIP::RET_CODE_OK;

    lResult = GetDialogIdBySipMsg(stSipMsg, stNotifyReq.ulDialogId);//lint !e838
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("Get Dialog ID Failed On Handle Invite Message.");
        return lResult;
    }

    char* pMsgBody = VOS_NULL;
    unsigned long ulMsgBodyLen = 0;

    //获取消息体
    lResult = SIP::GetMsgBody(stSipMsg, pMsgBody, ulMsgBodyLen);
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("Get Message Body Failed On Handle Invite Message.");
        return lResult;
    }

    //设置消息体
    stNotifyReq.pszMsgBody       = pMsgBody;
    stNotifyReq.ulMsgBodyLen     = ulMsgBodyLen;

    char* pSubject = VOS_NULL;
    unsigned long ulSubjectLen = 0;

    //获取Subject内容
    lResult = SIP::GetSubject(stSipMsg, pSubject, ulSubjectLen);
    if (SIP::RET_CODE_OK != lResult)
    {
        //释放消息体内存
        VOS_DELETE(pMsgBody, MULTI);

        ERROR_LOG("Get Subject Value Failed On Handle Invite Message.");
        return lResult;
    }

    //     //保存请求消息信息,此内存在发送响应消息中释放
    //     SIP::REQ_MSG_INFO* pReqMsgInfo = VOS_NEW(pReqMsgInfo);
    //     if (NULL != pReqMsgInfo)
    //     {
    //         pReqMsgInfo->pstSipReqMsg   = &stSipMsg;
    //         pReqMsgInfo->ulTuObjId      = ulTuObjId;
    //         pReqMsgInfo->ulTxnObjId     = ulTxnObjId;
    //         pReqMsgInfo->ulDialogId     = stNotifyReq.ulDialogId;
    //         pReqMsgInfo->secs = time(NULL);
    //     }   
    // 
    //     {
    //         CLockGuard objGard(m_pReqMsgMutex);
    //         unsigned long mapKey = (unsigned long)stSipMsg.stHeaders.pstCseq->ulSequence;
    //         m_msgMap[mapKey] = pReqMsgInfo;
    //     }
    unsigned long userSeq = InsertReqMsg(stSipMsg, ulTuObjId, ulTxnObjId, stNotifyReq.ulDialogId);

    stNotifyReq.pMsgHeader       = pSubject;
    stNotifyReq.ulMsgHeaderLen   = ulSubjectLen;

    // added by dhong
    SIP_HEADER *pHeader = GetSipHeader(stSipMsg, true);
    pHeader->sipMsg = g_recvStr;
    pHeader->userSeq = userSeq;
    pHeader->pMsgBody = pMsgBody;
    if (NULL != pSubject)
    {
        pHeader->subJect = pSubject;
    }
    // modified by dhong
    stNotifyReq.pUserData = pHeader;

    //上报文本响应消息
    lResult = NotifyTextMsgReq(stNotifyReq);

    //释放消息体内存
    // VOS_DELETE(pMsgBody, MULTI);
    //释放Subject内存
    VOS_DELETE(pSubject, MULTI);

    INFO_LOG("Handle Invite Message Success.");
    return SIP::RET_CODE_OK;//lint !e438
}

/**
* Description:  HandleReqMessage().     处理Message请求消息
* @param  [in]  ulTuObjId   对应的TU对象ID
* @param  [in]  ulTxnObjId  对应的事务对象ID
* @param  [in]      stSipMsg            SIP消息引用
* @param  [out] stNotifyReq     通知文本请求结构体引用
* @return       long.       返回码
*/
long CSipStack::HandleReqMessage
(
    SS_UINT32                   ulTuObjId,
    SS_UINT32                   ulTxnObjId,
    const SipTptNwInfoS&        stTptNwInf,
    SipMsgS&                    stSipMsg,
    SIP::NOTIFY_TEXT_MSG_REQ&   stNotifyReq
)
{
    INFO_LOG("Handle MESSAGE Message Begin.");

    long lResult = SIP::RET_CODE_OK;

    char* pMsgBody = VOS_NULL;
    unsigned long ulMsgBodyLen = 0;

    //获取消息体
    lResult = SIP::GetMsgBody(stSipMsg, pMsgBody, ulMsgBodyLen);//lint !e838
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("Get Message Body Failed On Handle MESSAGE Message.");
        return lResult;
    }

    //设置消息体
    // added by dhong
    SIP_HEADER *pHeader = GetSipHeader(stSipMsg, true);
    pHeader->sipMsg = g_recvStr;
    pHeader->pMsgBody = pMsgBody;

    if ( VOS_SUCCESS != GetAddrWithSipTptIpPortS( stTptNwInf.stSrcAddr,
        pHeader->devIP,
        pHeader->devPort
        )
        )
    {
        ERROR_LOG("handle register request - failure to get address info.");
		VOS_DELETE(pMsgBody,MULTI);
        return VOS_FAIL;
    }

    SIP::PEER_URI_INFO  stFromUri;
    memset(&stFromUri,0x0, sizeof(SIP::PEER_URI_INFO));
    SIP::GetFromUriInfo(stSipMsg, stFromUri);
    pHeader->from = stFromUri.szUriUserName;
    INFO_LOG("Handle MESSAGE Message sipHeader seq = %d,from = %s",pHeader->seq,pHeader->from.c_str());
    //pHeader->digestRealm = stFromUri.szUriHostName;
    pHeader->devDomain = "huawei.com";
    // modified by dhong
    stNotifyReq.pUserData = pHeader;

    stNotifyReq.pszMsgBody = NULL; // pMsgBody;
    stNotifyReq.ulMsgBodyLen = ulMsgBodyLen;

    ///////////////////////////////////  modified by w00207027 2012-11-03  Begin /////////////////////////////////////
    //modified by cwx153028 begin
    int nRet = IsRespDirect(pMsgBody);
    if (0 == nRet)
    {
        //生成响应消息
        CSipStackMsgWrapResp objSipRspMsg(*this, stSipMsg);
        //发送响应消息，不需处理返回值
        (void)objSipRspMsg.SendRspMsg(SIP_STATUS_SUCC_OK, ulTuObjId, ulTxnObjId);
        INFO_LOG("Handle MESSAGE Message sipHeader seq = %d, from = %s,sipStack send 200 OK to pu.",pHeader->seq,pHeader->from.c_str());
    }
	else if(1 == nRet)
	{
		//生成响应消息
		CSipStackMsgWrapResp objSipRspMsg(*this, stSipMsg);
		(void)objSipRspMsg.SendRspMsg(SIP_STATUS_SUCC_OK, ulTuObjId, ulTxnObjId);
		delete pHeader;
		pHeader = NULL;
		INFO_LOG("delete pHeader = [0X%x]",pHeader); 
		stNotifyReq.pUserData = NULL;
		VOS_DELETE(pMsgBody, MULTI);
		INFO_LOG("Keep Alive Message");
		return SIP::RET_CODE_OK;//lint !e438
	}
    else if (2 == nRet)
    {
        //生成响应消息
        CSipStackMsgWrapResp objSipRspMsg(*this, stSipMsg);
        //发送响应消息，不需处理返回值
        (void)objSipRspMsg.SendRspMsg(SIP_STATUS_SUCC_OK, ulTuObjId, ulTxnObjId);
    }
    else
    {
        unsigned long userSeq = InsertReqMsg(stSipMsg, ulTuObjId, ulTxnObjId, stNotifyReq.ulDialogId);
        pHeader->userSeq = userSeq;
        //SIP::GetContact(stSipMsg, pHeader->devIP, pHeader->devPort);
     }
    //modified by cwx153028 end
    ///////////////////////////////////  modified by w00207027 2012-11-03  End /////////////////////////////////////

    ////生成响应消息
    //CSipRspMsg objSipRspMsg(*this, stSipMsg);
    ////发送响应消息，不需处理返回值
    //(void)objSipRspMsg.SendRspMsg(SIP_STATUS_SUCC_OK, ulTuObjId, ulTxnObjId);

    //上报文本响应消息
    INFO_LOG("============Message sipHeader seq = %d, from = %s",pHeader->seq,pHeader->from.c_str());
    lResult = NotifyTextMsgReq(stNotifyReq);

    //释放消息体内存
    // dhong I think this should be invoked
    //VOS_DELETE(pMsgBody, MULTI);

    INFO_LOG("Handle MESSAGE Message Success.");
    return SIP::RET_CODE_OK;//lint !e438
}

//////////////////////////////////////////// add by w00207027 2012-11-03 Begin /////////////////////////////////////
/**
* Description:  isRespDirect().     判断是否直接会响应消息
* @param  [in]      pMsgBody            SIP消息体
* @return   bool.       返回码
*/
long CSipStack::IsRespDirect
(
    const char* pMsgBody
)
{
    TiXmlDocument doc;
    doc.Parse(pMsgBody,0,TIXML_ENCODING_LEGACY);

    if (doc.Error())
    {
        return 1; 
    }

    TiXmlElement *root = doc.RootElement();
    if (NULL == root)
    {
        return 1; 
    }

    TiXmlElement *pList = root->FirstChildElement(T28181_XML_CMD_TYPE);
    if (NULL == pList)
    {
        return 0;
    }

    const char *pCmd = pList->GetText();
    if ((NULL != pCmd)
        &&((0==strncmp(T28181_XML_CMD_TYPE_KEEP_ALIVE, pCmd,  T28181_XML_CMD_TYPE_LENGTH))))
    {
        return 1;
    }

    if ((NULL != pCmd)
        &&((0==strncmp( T28181_XML_CMD_TYPE_ALARM, pCmd,  T28181_XML_CMD_TYPE_LENGTH))))
    {
        return 2;
    }

    return 0;
}
//////////////////////////////////////////// add by w00207027 2012-11-03 End /////////////////////////////////////

/**
* Description:  HandleReqBye().     处理Bye请求消息
* @param  [in]  ulTuObjId   对应的TU对象ID
* @param  [in]  ulTxnObjId  对应的事务对象ID
* @param  [in]      stSipMsg            SIP消息引用
* @param  [out] stNotifyReq     通知文本请求结构体引用
* @return       long.       返回码
*/
long CSipStack::HandleReqBye
(
    SS_UINT32                   ulTuObjId,
    SS_UINT32                   ulTxnObjId,
    SipMsgS&                    stSipMsg,
    SIP::NOTIFY_TEXT_MSG_REQ&   stNotifyReq
)
{
    INFO_LOG("Handle BYE Message Begin.");

    long lResult = SIP::RET_CODE_OK;

    //获取对话ID，如找到则将其从MAP中删除
    lResult = GetDialogIdBySipMsg(stSipMsg, stNotifyReq.ulDialogId, VOS_TRUE);//lint !e838
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("Get Dialog ID Failed On Handle BYE Message.");
        //   return lResult;
    }

    SIP_HEADER *pHeader = GetSipHeader(stSipMsg, true);
    // modified by dhong
    pHeader->sipMsg = g_recvStr;
    // pHeader->pData = NULL;
    stNotifyReq.pUserData = pHeader;

    //上报文本响应消息
    lResult = NotifyTextMsgReq(stNotifyReq);

    //生成响应消息
    CSipStackMsgWrapResp objSipRspMsg(*this, stSipMsg);
    //发送响应消息，不需处理返回值
    (void)objSipRspMsg.SendRspMsg(SIP_STATUS_SUCC_OK, ulTuObjId, ulTxnObjId);
    INFO_LOG("Handle BYE Message End. Result = 0x%04X.", lResult);
    return SIP::RET_CODE_OK;
}
/*
SS_UINT32                   ulTuObjId,
SS_UINT32                   ulTxnObjId,
*/
unsigned long CSipStack::InsertReqMsg(SipMsgS &sipMsg, const SS_UINT32 &ulTuObjId, const SS_UINT32 &ulTxnObjId, const unsigned long &ulDialogId)
{
    INFO_LOG("CSipStack::insertReqMsg entered");
    //保存请求消息信息,此内存在发送响应消息中释放
    SIP::REQ_MSG_INFO* pReqMsgInfo = VOS_NEW(pReqMsgInfo);
    if (NULL == pReqMsgInfo)
    {
        INFO_LOG("CSipStack::insertReqMsg insert failed: lack memory");
        return 0;
    }

	if(SIP_RET_SUCCESS != SipDsmCopyMsgRef (&sipMsg, &(pReqMsgInfo->pstSipReqMsg)))   // 引用次数+1
        //if(SIP::RET_CODE_OK != SipDsmCloneMsg (&sipMsg, &(pReqMsgInfo->pstSipReqMsg)))  // 完全复制一份
    {
        INFO_LOG("CSipStack::insertReqMsg insert failed: lack memory");
        VOS_DELETE(pReqMsgInfo);
        return 0;
    }

    pReqMsgInfo->ulTuObjId      = ulTuObjId;
    pReqMsgInfo->ulTxnObjId     = ulTxnObjId;
    pReqMsgInfo->ulDialogId     = ulDialogId;
    pReqMsgInfo->secs = (unsigned long)time(NULL); 
    pReqMsgInfo->state = SIP::INIT;

    if (0 == m_mapKey)
    {
        ++m_mapKey;
    }

    {
        //  cout << "CSipStack::insertReqMsg insert: " << m_mapKey << endl;
        CLockGuard objGard(m_pReqMsgMutex);
        m_msgMap[m_mapKey++] = pReqMsgInfo;
    }//lint !e1788

    INFO_LOG("CSipStack::insertReqMsg insert success");

    return (m_mapKey-1);
}

void CSipStack::CheckReqMsg(void)
{
    unsigned long curSec = (unsigned long)time(NULL);

    CLockGuard objGard(m_pReqMsgMutex);

    for(m_msgItertor = m_msgMap.begin(); m_msgItertor != m_msgMap.end();)
    {
        if ((curSec - 40) > m_msgItertor->second->secs )    
        {
            //    cout << SipStack::genCurTime() << "   " << "CSipStack::checkReqMsg timeout:" << endl;
            SIP::REQ_MSG_INFO *pInfo = m_msgItertor->second;
            if((NULL!=pInfo)
                && (SIP::OCCUPIED!=pInfo->state))
            {
                m_msgMap.erase(m_msgItertor++);

                SipMsgS *stSipMsg = pInfo->pstSipReqMsg;

                if (NULL != stSipMsg)
                {
                    //强制中止事务
                    /*(void)SipTxnHiTerminateTxn(stSipMsg->, stMsgInfo.ulTuObjId, SIP_TERM_MODE_FORCEFUL_NO_CANCEL_RSP);*/
                    //释放SIP消息
                    SipDsmReleaseMsgRef(&stSipMsg);
                    VOS_DELETE(pInfo);
                }

                continue;
            }
        }

        ++m_msgItertor;
    }
}//lint !e1788

SIP::REQ_MSG_INFO* CSipStack::GetReqMsg(unsigned long seq, bool bGetAndDelete)
{
    CLockGuard objGard(m_pReqMsgMutex);

    m_msgItertor = m_msgMap.find(seq);
    if (m_msgItertor == m_msgMap.end())
    {
        return NULL;
    }

    SIP::REQ_MSG_INFO *pInfo = m_msgItertor->second;
    if (bGetAndDelete)
    {
        m_msgMap.erase(m_msgItertor);
    }
    else
    {
        pInfo->state = SIP::OCCUPIED;
    }

    return pInfo;
}//lint !e1788

long CSipStack::SendInviteResp(const string &msg, const unsigned long seq, const unsigned long retCode)
{
    long lResult = SIP::RET_CODE_FAIL;
    // cout<<SipStack::genCurTime()<<" | " << "CSipStack::sendInviteResp Entered:" << seq << endl;
    SIP::REQ_MSG_INFO* pReqMsgInfo = GetReqMsg(seq);
    if (NULL == pReqMsgInfo)
    {
        // cout<<SipStack::genCurTime()<<" | " << "CSipStack::sendInviteResp get ReqMsg failed:" << seq << endl;
        return lResult;
    }

    SIP::TEXT_MSG _msg;
    memset(&_msg, 0, sizeof(_msg));
    _msg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_INVITE_RSP;
    _msg.ulMsgSeq=2;
    _msg.ulMsgBodyLen = msg.length();
    _msg.pszMsgBody = (char *)msg.c_str();//lint !e1773
    _msg.pUserData = pReqMsgInfo;

    CSipStackMsgWrap* pSipMsg = new CSipStackMsgWrapInviteRespEx(*this, _msg, retCode);
    lResult = pSipMsg->SendMsg();
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("Send Message Failed On Handle SIP Register Response Message.");
    }
    else
    {
        INFO_LOG("CSipStack::sendInviteResp Success");
    }

	delete pSipMsg;
	pSipMsg = NULL;

    return lResult;
}

long CSipStack::SendRegisterResp(const unsigned long seq, const unsigned long retCode)
{
    long lResult = SIP::RET_CODE_FAIL;
    SIP::REQ_MSG_INFO* pReqMsgInfo=GetReqMsg(seq);
    if (NULL == pReqMsgInfo)
    {
        // cout<<SipStack::genCurTime()<<" | " << "CSipStack::SendRegisterResp get ReqMsg failed:" << seq << endl;
        return lResult;
    }

    CSipStackMsgWrapRegisterResp* pSipMsg = new CSipStackMsgWrapRegisterResp(*this, *(pReqMsgInfo->pstSipReqMsg), false);
    lResult = pSipMsg->SendRspMsg(retCode, pReqMsgInfo->ulTuObjId, pReqMsgInfo->ulTxnObjId);
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG(" Send Message Failed On Handle SIP Register Response Message.");
    }
    else
    {
        INFO_LOG("CSipStack::SendRegisterResp Success");
    }

	delete pSipMsg;
	pSipMsg = NULL;
    SipDsmReleaseMsgRef(&(pReqMsgInfo->pstSipReqMsg));
	delete pReqMsgInfo;
	pReqMsgInfo = NULL;

    return lResult;
}

long CSipStack::SendKeepaliveResp(const unsigned long seq, const unsigned long retCode)
{
    long lResult = SIP::RET_CODE_FAIL;
    SIP::REQ_MSG_INFO* pReqMsgInfo=GetReqMsg(seq);
    if (NULL == pReqMsgInfo)
    {
        // cout<<SipStack::genCurTime()<<" | " << "CSipStack::SendKeepaliveResp get ReqMsg failed:" << seq << endl;
        return lResult;
    }

    CSipStackMsgWrapResp* pSipMsg = new CSipStackMsgWrapResp(*this, *(pReqMsgInfo->pstSipReqMsg));
    lResult = pSipMsg->SendRspMsg(retCode, pReqMsgInfo->ulTuObjId, pReqMsgInfo->ulTxnObjId);
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("Send Message Failed On Handle SIP Keepalive Response Message.");
    }
    else
    {
        INFO_LOG("CSipStack::SendKeepaliveResp Success");
    }

	delete pSipMsg;
	pSipMsg = NULL;
    SipDsmReleaseMsgRef(&(pReqMsgInfo->pstSipReqMsg));
	delete pReqMsgInfo;
	pReqMsgInfo = NULL;

    return lResult;
}

long CSipStack::SendOptionResp(const unsigned long seq, const unsigned long retCode)
{
    long lResult = SIP::RET_CODE_FAIL;
    SIP::REQ_MSG_INFO* pReqMsgInfo=GetReqMsg(seq);
    if (NULL == pReqMsgInfo)
    {
        // cout<<SipStack::genCurTime()<<" | " << "CSipStack::SendOptionResp get ReqMsg failed:" << seq << endl;
        return lResult;
    }

    CSipStackMsgWrapOptionsResp* pSipMsg = new CSipStackMsgWrapOptionsResp(*this, *(pReqMsgInfo->pstSipReqMsg));
    lResult = pSipMsg->SendRspMsg(retCode, pReqMsgInfo->ulTuObjId, pReqMsgInfo->ulTxnObjId);
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("Send Message Failed On Handle SIP Option Response Message.");
    }
    else
    {
        INFO_LOG("CSipStack::SendOptionResp Success");
    }

	delete pSipMsg;
	pSipMsg = NULL;
    SipDsmReleaseMsgRef(&(pReqMsgInfo->pstSipReqMsg));
	delete pReqMsgInfo;
	pReqMsgInfo = NULL;

    return lResult;
}

long CSipStack::SendInviteAck(const unsigned long seq)
{
    long lResult = SIP::RET_CODE_FAIL;
    SIP::REQ_MSG_INFO* pReqMsgInfo=GetReqMsg(seq, false);
    if ((NULL == pReqMsgInfo)
        || (NULL ==pReqMsgInfo->pstSipReqMsg)
        || (NULL ==pReqMsgInfo->pstSipReqMsg->stHeaders.pstCseq))
    {
        // cout<<SipStack::genCurTime()<<" | " << "CSipStack::SendInviteAck get ReqMsg failed:" << seq << endl;
        return lResult;
    }

    MAP_DIALOG_ID_TO_INFO::iterator iter_id_inf = m_mapDialogIdToInfo.find(pReqMsgInfo->ulDialogId);
    if ( m_mapDialogIdToInfo.end() == iter_id_inf )
    {
        return lResult;
    }


    //创建ACK消息对象
    SIP::TEXT_MSG stTextMsg = {0};
    stTextMsg.ulMsgSeq     = pReqMsgInfo->pstSipReqMsg->stHeaders.pstCseq->ulSequence;
    stTextMsg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_ACK;
    stTextMsg.ulDialogId    = pReqMsgInfo->ulDialogId;
    stTextMsg.pUserData     = pReqMsgInfo->pstSipReqMsg;

    CSipStackMsgWrapAck objSipMsgAck(*this, stTextMsg);

    //发送ACK
    lResult = objSipMsgAck.SendMsg();
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("[ MsgSeq = %d ] Send ACK Message Failed On Handle SIP Invite Response Message.",stTextMsg.ulMsgSeq);
    }
    else
    {
        INFO_LOG("CSipStack::SendInviteAck Success, msgType=%d",stTextMsg.enTextMsgType);
    }

    return lResult;
}

long CSipStack::SendInviteBye(const unsigned long seq)
{
    long lResult = SIP::RET_CODE_FAIL;
    SIP::REQ_MSG_INFO* pReqMsgInfo=GetReqMsg(seq);
    if ((NULL == pReqMsgInfo)
        || (NULL ==pReqMsgInfo->pstSipReqMsg)
        || (NULL ==pReqMsgInfo->pstSipReqMsg->stHeaders.pstCseq))
    {
        // cout<<SipStack::genCurTime()<<" | " << "CSipStack::SendInviteBye get ReqMsg failed:" << seq << endl;
        return lResult;
    }

    //创建BYE消息对象
    SIP::TEXT_MSG stTextMsg = {0};
    stTextMsg.ulMsgSeq     = (unsigned long)(pReqMsgInfo->pstSipReqMsg->stHeaders.pstCseq->ulSequence + 1);
    stTextMsg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_BYE;
    stTextMsg.ulDialogId    = pReqMsgInfo->ulDialogId;
    stTextMsg.pUserData     = pReqMsgInfo->pstSipReqMsg;

    CSipStackMsgWrapBye objSipMsgBye(*this, stTextMsg);

    //发送BYE
    lResult = objSipMsgBye.SendMsg();
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("[ MsgSeq = %d ] Send BYE Message Failed On Handle SIP Invite Response Message.",stTextMsg.ulMsgSeq);
    }
    else
    {
        INFO_LOG("CSipStack::SendInviteBye Success");
    }

    SipDsmReleaseMsgRef(&(pReqMsgInfo->pstSipReqMsg));
    VOS_DELETE(pReqMsgInfo);

    return lResult;
}

long CSipStack::SendInviteCancel(const unsigned long seq)
{
    long lResult = SIP::RET_CODE_FAIL;
    SIP::REQ_MSG_INFO* pReqMsgInfo=GetReqMsg(seq);
    if ((NULL == pReqMsgInfo)
        || (NULL ==pReqMsgInfo->pstSipReqMsg)
        || (NULL ==pReqMsgInfo->pstSipReqMsg->stHeaders.pstCseq))
    {
        // cout<<SipStack::genCurTime()<<" | " << "CSipStack::SendInviteCancel get ReqMsg failed:" << seq << endl;
        return lResult;
    }

    //创建Cancel消息对象
    SIP::TEXT_MSG stTextMsg = {0};
    stTextMsg.ulMsgSeq     = pReqMsgInfo->pstSipReqMsg->stHeaders.pstCseq->ulSequence;
    stTextMsg.enTextMsgType = SIP::TEXT_MSG_TYPE_SIP_CANCEL;
    stTextMsg.ulDialogId    = pReqMsgInfo->ulDialogId;
    stTextMsg.pUserData     = pReqMsgInfo->pstSipReqMsg;

    CSipStackMsgWrapAck objSipMsgAck(*this, stTextMsg);   //  TODO  Cancel 消息待封装

    //发送Cancel
    lResult = objSipMsgAck.SendMsg();
    if (SIP::RET_CODE_OK != lResult)
    {
        ERROR_LOG("[ MsgSeq = %d ] Send Cancel Message Failed On Handle SIP Invite Response Message.",stTextMsg.ulMsgSeq);
    }
    else
    {
        INFO_LOG("CSipStack::SendInviteCancel Success");
    }

    SipDsmReleaseMsgRef(&(pReqMsgInfo->pstSipReqMsg));
    VOS_DELETE(pReqMsgInfo);

    return lResult;
}

long CSipStack::SendResp(const unsigned long seq)
{
    //  cout<<SipStack::genCurTime()<<" | " << "CSipStack::sendResp Entered:" << seq << endl;

    SIP::REQ_MSG_INFO* pReqMsgInfo=GetReqMsg(seq);
    if (NULL == pReqMsgInfo)
    {
        //    cout<<SipStack::genCurTime()<<" | " << "CSipStack::sendResp get REQ_MSG_INFO failed:" << seq << endl;
        return -1;
    }

    SipMsgS &stSipMsg = *(pReqMsgInfo->pstSipReqMsg);

    CSipStackMsgWrapResp objSipRspMsg(*this, stSipMsg);
    (void)objSipRspMsg.SendRspMsg(SIP_STATUS_SUCC_OK, pReqMsgInfo->ulTuObjId, pReqMsgInfo->ulTxnObjId);

    // 暂时不加，后面加上 dhong
    SipDsmReleaseMsgRef(&(pReqMsgInfo->pstSipReqMsg));
    VOS_DELETE(pReqMsgInfo);

    INFO_LOG("CSipStack::sendResp Success");

    return 0;
}

long CSipStack::AnsMsg( const string& ans_code, const string& data)
{
    unsigned long i_ans_code = (unsigned long)ACE_OS::atoi(ans_code.c_str());
    SIP::REQ_MSG_INFO* req_msg_info_ptr = GetReqMsg(i_ans_code);
    if ( NULL == req_msg_info_ptr )
    {
        return -1;
    }

    SipMsgS &stSipMsg = *(req_msg_info_ptr->pstSipReqMsg);

    CSipStackMsgWrapResp objSipRspMsg(*this, stSipMsg, data);
    (void)objSipRspMsg.SendRspMsg(SIP_STATUS_SUCC_OK, req_msg_info_ptr->ulTuObjId, req_msg_info_ptr->ulTxnObjId);

    // 暂时不加，后面加上 dhong
    SipDsmReleaseMsgRef(&(req_msg_info_ptr->pstSipReqMsg));
    VOS_DELETE(req_msg_info_ptr);

    INFO_LOG("CSipStack::sendResp Success");

    return 0;
}

/**
* Description:  NotifyCallBack(). 调用回调函数
* @param  [in]  stInfo  通知信息结构体
* @return       long.   返回码
*/
long CSipStack::NotifyCallBack(SIP::NOTIFY_INFO& stInfo)
{
    //指针非空检查
    if (NULL == m_pfnNotify)
    {
        return SIP::RET_CODE_MEMORY_NULL;
    }

    //调用回调函数
    long lResult = m_pfnNotify(&stInfo, m_pNotifyFnParams);

    return lResult;
}

/**
* Description:  NotifyTextMsgResp(). 通知文本响应消息
* @param  [in]  stNotifyInfo    文本响应消息通知信息结构体
* @return       long.   返回码
*/
long CSipStack::NotifyTextMsgResp(SIP::NOTIFY_TEXT_MSG_RESP& stNotifyInfo)
{
    SIP::NOTIFY_INFO stInfo = {0};

    stInfo.ulNotifyType     = SIP::NOTIFY_TYPE_TEXT_MSG_RESP;
    stInfo.pNotifyInfo      = &stNotifyInfo;
    stInfo.ulNotifyInfoLen  = sizeof(stNotifyInfo);

    long lResult = NotifyCallBack(stInfo);

    return lResult;
}

/**
* Description:  NotifyTextMsgReq(). 通知文本请求消息, 设备发给平台的请求消息
* @param  [in]  stNotifyInfo        文本请求消息通知信息结构体
* @return       long.   返回码
*/
long CSipStack::NotifyTextMsgReq(SIP::NOTIFY_TEXT_MSG_REQ& stNotifyInfo)
{
    SIP::NOTIFY_INFO stInfo = {0};

    stInfo.ulNotifyType     = SIP::NOTIFY_TYPE_TEXT_MSG_REQ;
    stInfo.pNotifyInfo      = &stNotifyInfo;
    stInfo.ulNotifyInfoLen  = sizeof(stNotifyInfo);

    long lResult = NotifyCallBack(stInfo);

    return lResult;
}

/**
* Description:  GetSipMsgInfoByCSeq().      根据CSeq找到SIP消息的信息
* @param  [in]      ulCSeq              SIP消息的CSeq
* @param  [out]     pstSipMsgInfo    SIP消息信息指针
* @param  [in]  bErase      是否删除
* @return       long.       返回码
*/
long CSipStack::GetSipMsgInfoByCSeq
(
    unsigned long   ulCSeq,
    SIP_MSG_INFO*&  pstSipMsgInfo,
    VOS_BOOLEAN     bErase
)
{
    //加上锁
    CLockGuard objLocker(m_pMapMutex);

    //查找数据包
    CSipStack::MAP_C_SEQ_TO_SIP_MSG_INFO::iterator iter = m_mapCSeqToSipMsgInfo.find(ulCSeq);
    //没有找到请求
    if (m_mapCSeqToSipMsgInfo.end() == iter)
    {  
        WARN_LOG("[ CSeq = %d ] SIP Message Info Not Found.",ulCSeq);
        return SIP::RET_CODE_FAIL;
    }

    pstSipMsgInfo = iter->second;

    if (bErase)
    {
        //从map表中删除该项
        (void)m_mapCSeqToSipMsgInfo.erase(iter);
    }

    return SIP::RET_CODE_OK;
}//lint !e1788

//BEGIN R002C01SPC001 ADD 2011-08-24 ligengqiang 00124479 for 增加线程锁，MiniSIP只支持单线程
/**
* Description:  GetThreadMutex().   获取线程锁
* @return       VOS_Mutex*.     锁指针
*/
VOS_Mutex* CSipStack::GetThreadMutex()
{
    return m_pThreadMutex;
}
//END   R002C01SPC001 ADD 2011-08-24 ligengqiang 00124479 for 增加线程锁，MiniSIP只支持单线程

int CSipStack::GetAddrWithSipTptIpPortS( IN  const SipTptIpPortS& stTptAddr,
                                         OUT std::string&         strIPv4,
                                         OUT unsigned short&      usPort
                                       )
{
    char buf[24] = {0};
    int pos = snprintf( buf, sizeof(buf), "%u.%u.%u.%u",
                        stTptAddr.u.aucIPv4Addr[0],
                        stTptAddr.u.aucIPv4Addr[1],
                        stTptAddr.u.aucIPv4Addr[2],
                        stTptAddr.u.aucIPv4Addr[3]
                      );
    if ( 0 >= pos )
    {
        ERROR_LOG("failure to get address - snprintf");
        return VOS_FAIL;
    }
    buf[pos] = '\0';
    strIPv4 = buf;
    usPort  = (unsigned short)stTptAddr.iPort;

    return VOS_SUCCESS;
}

int CSipStack::HandleReqResend( const unsigned long enHeader, const SipTptNwInfoS& stTptNwInf, const unsigned long nCSeq )
{
    if ( m_mapRspResend.empty() )
    {
        ERROR_LOG("handle request resend - resend map is empty.");
        return VOS_FAIL;
    }

    std::string    strIPv4 = "";
    unsigned short usPort  = 0;
    if (  VOS_SUCCESS != GetAddrWithSipTptIpPortS(stTptNwInf.stDstAddr, strIPv4, usPort)
       || strIPv4.empty()	||	0 == usPort
       )
    {
        ERROR_LOG("handle request resend - resend address is invalid.");
        return VOS_FAIL;
    }

    DEAL_RESEND_HEADER_MAP::iterator hit = m_mapReqResend.find((int)enHeader);
    if ( hit == m_mapReqResend.end() )
    {
        ERROR_LOG("handle request resend - unsupport header - Header_Type=%d, CSeq=%d.",enHeader,nCSeq);
        return VOS_FAIL;
    }

    CSipStack::CDealResendData aDealRespndData(strIPv4, usPort, nCSeq);
	DEAL_RESEND_LIST& dList = (DEAL_RESEND_LIST)hit->second;
    if ( !dList.empty() )
    {
        for ( DEAL_RESEND_LIST::iterator iter = dList.begin();
            iter != dList.end();

            )
        {
            if ( NULL == *iter )
            {
                dList.erase(iter++);
                continue;
            }

            if ( !(*iter)->IsTimeout() )
            {
                if ( (*iter)->IsTheSame( aDealRespndData ) )
                {
					WARN_LOG("handle request resend - drop resend- Header_Type=%d, IPv4=%s, Port=%d, CSeq=%d.",
						enHeader, (*iter)->GetIPv4().c_str(), (*iter)->GetPort(), nCSeq);
                    return VOS_FAIL;
                }
                else
                {
                    iter ++;
                }
            }
            else
            {
				DEBUG_LOG( "handle requst resend - earse - Header_Type=%d, IPv4=%s, Port=%d, CSeq=%d.",
					enHeader, (*iter)->GetIPv4().c_str(), (*iter)->GetPort(), nCSeq);
                
                delete (*iter);
                (*iter) = NULL;
                dList.erase(iter++);
                continue;
            }
        }
    }

	DEBUG_LOG( "handle request resend - push - Header_Type=%d, IPv4=%s, Port=%d, CSeq=%d.",
		enHeader, aDealRespndData.GetIPv4().c_str(), aDealRespndData.GetPort(), nCSeq);

	CSipStack::CDealResendData* pData = new CSipStack::CDealResendData(strIPv4, usPort, nCSeq);
    dList.push_back(pData);
    return VOS_SUCCESS;
}

int CSipStack::HandleRspResend( const unsigned long enHeader, const SipTptNwInfoS& stTptNwInf, const unsigned long nCSeq )
{
    if ( m_mapRspResend.empty() )
    {
        ERROR_LOG("handle respond resend - resend map is empty.");
        return VOS_FAIL;
    }

    std::string    strIPv4 = "";
    unsigned short usPort  = 0;
    if (  VOS_SUCCESS != GetAddrWithSipTptIpPortS(stTptNwInf.stDstAddr, strIPv4, usPort)
       || strIPv4.empty()
       || 0 == usPort
       )
    {
        ERROR_LOG("handle respond resend - resend address is invalid.");
        return VOS_FAIL;
    }

    DEAL_RESEND_HEADER_MAP::iterator hit = m_mapRspResend.find((int)enHeader);
    if ( hit == m_mapRspResend.end() )
    {
        ERROR_LOG("handle respond resend - unsupport header - Header_Type=%d, CSeq=%d",enHeader,nCSeq);
        return VOS_FAIL;
    }

    CSipStack::CDealResendData aDealRespndData(strIPv4, usPort, nCSeq);
	DEAL_RESEND_LIST & dList = (DEAL_RESEND_LIST)hit->second;
    if ( !dList.empty() )
    {
        for ( DEAL_RESEND_LIST::iterator iter = dList.begin();
            iter != dList.end();

            )
        {
            if ( NULL == *iter )
            {
                dList.erase(iter++);
                continue;
            }

            if ( !(*iter)->IsTimeout() )
            {
                if ( (*iter)->IsTheSame(aDealRespndData) )
                {
					WARN_LOG("handle respond resend - drop resend- Header_Type=%d, IPv4=%s, Port=%d, CSeq=%d.",
						enHeader, (*iter)->GetIPv4().c_str(), (*iter)->GetPort(), nCSeq);
                    return VOS_FAIL;
                }
                else
                {
                    iter ++;
                }
            }
            else
            {
                delete (*iter);
                (*iter) = NULL;
                dList.erase(iter++);
                continue;
            }
        }
    }

    CSipStack::CDealResendData* pData = new CSipStack::CDealResendData(strIPv4, usPort, nCSeq);
    dList.push_back(pData);
    return VOS_SUCCESS;
}

CSipStack::CDealResendData::CDealResendData ( const std::string& strIPv4, unsigned short usPort,  unsigned long nCSeq )
                           :m_strIPv4(strIPv4.c_str())
                           ,m_usPort(usPort)
                           ,m_nCSeq(nCSeq)
                           ,m_tResendTime(ACE_OS::time())
{
}

const std::string& CSipStack::CDealResendData::GetIPv4() const
{
    return m_strIPv4;
}

unsigned short CSipStack::CDealResendData::GetPort() const
{
    return m_usPort;
}

unsigned long CSipStack::CDealResendData::GetCSeq() const
{
    return m_nCSeq;
}

bool CSipStack::CDealResendData::IsTheSame ( const CDealResendData& other )
{
    return ( m_strIPv4 == other.m_strIPv4 && m_usPort == other.m_usPort && m_nCSeq == other.m_nCSeq );
}

bool CSipStack::CDealResendData::IsTimeout ()
{
    return (RESEND_INTERVAL < (ACE_OS::time() - m_tResendTime));
}

}//end namespace

//lint +e429

#pragma warning(pop)

