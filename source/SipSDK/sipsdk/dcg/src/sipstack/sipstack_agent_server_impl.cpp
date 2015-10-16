#include <iostream>
#include <limits>

#include "sipstack_import.hpp"
#include "sipstack_agent_msg_impl.hpp"
#include "sipstack_agent_server_impl.hpp" 
#include "sipstack_agent_client_impl.hpp"
#include "sipstack_agent_msg_type.hpp"
#include "sipstack_config.hpp"
#include "CDcgStack.hpp"
#include "sipstack_task.h"
#include "Log.h"

//#define SIP_AGENT_SERVER_THREADS_MIN                      (5)
#define SIP_AGENT_SERVER_THREADS_DFLT                     (10)
//#define SIP_AGENT_SERVER_THREADS_MAX                      (20)

//lint -e429
//lint -e423
namespace SipStack{

const DATA_TYPE_IN_XML_ST xml_type_st_arr[] = {  { DATA_TYPE_IN_XML_ATTR, "MsgType" }
                                                ,{ DATA_TYPE_IN_XML_ELEM, "CmdType" }
                                              };

const unsigned int xml_type_n = sizeof(xml_type_st_arr) / sizeof(DATA_TYPE_IN_XML_ST);

const DATA_TYPE_IN_XML_ST xml_sn_st_arr[] = {  { DATA_TYPE_IN_XML_ATTR, "MsgSeq" }
                                              //,{ DATA_TYPE_IN_XML_ELEM, "SN" }
                                            };

const unsigned int xml_sn_n = sizeof(xml_sn_st_arr) / sizeof(DATA_TYPE_IN_XML_ST);

SipStackAgentServerImpl::SipStackAgentServerImpl(SipStackAgentCfg const & reg_info, unsigned long max_queue_num)
                          : SipStackAgentServer()
                          , m_init_b(false)
                          , m_run_b(false)
                          , m_client_p(NULL)
                          , m_reg_info(reg_info)
                          , m_max_queue_num_cul(max_queue_num)
                          , m_cur_queue_num_ul(0)
                          , m_thread_num_cul(SIP_AGENT_SERVER_THREADS_DFLT)
{
}

SipStackAgentServerImpl::~SipStackAgentServerImpl()
{
    //Fini ();
	INFO_LOG("delete SipStackAgentServerImpl");
}

bool SipStackAgentServerImpl::Init (SipStackAgentClient* sipstack_agent_client_p)
{
    if ( m_init_b )
    {
        WARN_LOG("sip agent server - init - init again") ;
        return true;
    }

    m_client_p = sipstack_agent_client_p;

    GetDcgStack().setRegInfo(m_reg_info);
    long lRet = GetDcgStack().Init(this); 
	if(SIP::RET_CODE_OK !=  lRet)
	{
		return false;
	}

	if(NULL == msg_queue())
	{
		ERROR_LOG("msg queue is empty.");
		return (m_run_b = false);
	}

    m_run_b = true;
    if (  -1 == msg_queue()->open( 8*1024*50, 8*1024*50 )
       || -1 == activate(THR_NEW_LWP  | THR_JOINABLE, (int)m_thread_num_cul ) //lint !e835
       || !CSipStackTask::Instance().Init()
       )
    {
        ERROR_LOG("sip agent server - init - failure to init.");
        return (m_run_b = false);
    }

    m_init_b = true;
    return true;
}

void SipStackAgentServerImpl::Fini ()
{
    if ( !m_init_b )
    {
        return ;
    }

    CSipStackTask::Instance().Fini();

    m_run_b = false;
    wait();

	if(NULL == msg_queue() )
	{
		return;
	}
    msg_queue()->deactivate();
    GetDcgStack().Fini();
    m_client_p = NULL; //此指针是外部创建传入的，不可此处进行释放。
    m_init_b = false;
}

SipStackAgentCfg & SipStackAgentServerImpl::GetAgentConfig()
{
    return m_reg_info;
}

auto_ptr<SipStackAgentMsg> SipStackAgentServerImpl::GetSipStackMsg(const SIP_HEADER *sipHeader)
{
    auto_ptr<SipStackMsgImplMiniSip> pMsg(new SipStackMsgImplMiniSip);
    pMsg->SetType(sipHeader->msgType);
    pMsg->SetSequence(sipHeader->seq);

    pMsg->SetRequestFlag(sipHeader->reqMsg);
    pMsg->SetMessage(sipHeader->sipMsg);
    pMsg->SetCallID(sipHeader->callID);
    pMsg->SetSubject(sipHeader->subJect);
    pMsg->SetFrom(sipHeader->from);
    pMsg->SetDigestRealm(sipHeader->digestRealm);
    pMsg->SetDomain(sipHeader->devDomain);

    pMsg->SetRespondID(sipHeader->userSeq);

    if (sipHeader->reqMsg)
    {
        if (NULL != sipHeader->pMsgBody)
        {
            pMsg->SetBody(sipHeader->pMsgBody);

            pMsg->SetBodyCode ( FindDataFromXml(sipHeader->pMsgBody, xml_type_st_arr, xml_type_n) );
            pMsg->SetBodySequence( FindDataFromXml(sipHeader->pMsgBody, xml_sn_st_arr, xml_sn_n) );
            DEBUG_LOG("Message_Type=%s, Message_SN=%s ",pMsg->GetBodyCode().c_str(),pMsg->GetBodySequence().c_str());
        }

        if (!sipHeader->devIP.empty())
        {
            pMsg->SetIP(sipHeader->devIP);
            pMsg->SetPort(sipHeader->devPort);
        }

        if (SIP_STACK_TYPE_REGISTER == sipHeader->msgType)
        {
            //pMsg->setDataCmdCode("REGISTER");
            pMsg->SetExpires(sipHeader->expires);
            pMsg->SetDigestUserName(sipHeader->digestUserName) ;
            //pMsg->setDigestRealm(sipHeader->digestRealm);
            pMsg->SetDigestNonce(sipHeader->digestNonce);
            pMsg->SetDigestNonceCount(sipHeader->digestNonceCount);
            pMsg->SetDigestCNonce(sipHeader->digestCNonce);
            pMsg->SetDigestQop(sipHeader->digestQop);
            pMsg->SetDigestUri(sipHeader->digestUri);
            pMsg->SetDigestResponse(sipHeader->digestResponse);
            pMsg->SetDigestAlgorithm(sipHeader->digestAlgorithm);
        }

        pMsg->SetStatusCode((unsigned long)sipHeader->iStatusCode);

        switch(sipHeader->msgType)
        {
        case SIP_STACK_TYPE_REGISTER:
            pMsg->SetCmd("REGISTER");
            break;
        case SIP_STACK_TYPE_UNREGISTER:
            pMsg->SetCmd("REGISTER");
            break;
        case SIP_STACK_TYPE_OPTIONS:
            pMsg->SetCmd("OPTIONS");
            break;
        case SIP_STACK_TYPE_MESSAGE:
            pMsg->SetCmd("MESSAGE");
            break;
        default:
            break;
        }
        //获取字符串Rigister，Options等字符串
        //pMsg->setMsgCmd(method);
    }
    else
    {
        cout<< "sipHeader->msgType:"<< (long)(sipHeader->msgType) << endl;
        cout<< "sipHeader->iStatusCode:" <<sipHeader->iStatusCode <<endl;
        //cout<<"sipHeader->pMsgBody:"<<sipHeader->pMsgBody<<endl;
        if ((SIP_STACK_TYPE_INVITE == sipHeader->msgType))
            //	|| (SIP_STACK_TYPE_INVITE_RSP == sipHeader->msgType))
        {
            if (NULL != sipHeader->pMsgBody)
            {
                pMsg->SetBody(sipHeader->pMsgBody);
            }
        }

        pMsg->SetStatusCode((unsigned long)sipHeader->iStatusCode);
    }

    return (auto_ptr<SipStackAgentMsg>)pMsg;
}

std::string SipStackAgentServerImpl::FindDataFromXml( const std::string&        strBody,
                                                      const DATA_TYPE_IN_XML_ST data_type_in_xml_st_arr[],
                                                      const int                 data_type_count_n
                                                    )
{
    size_t      pos_b_n        = 0;
    size_t      pos_e_n        = std::string::npos;
    const char* find_data_p    = NULL;

    for ( int i = 0; i < data_type_count_n; i ++ )
    {
        find_data_p    = data_type_in_xml_st_arr[i].data_type_name_sz;
        if ( '\0' == find_data_p[0] )
        {
            continue;
        }

        pos_b_n = 0;
        pos_e_n = std::string::npos;
        switch ( data_type_in_xml_st_arr[i].data_type_in_xml_n )
        {
        case DATA_TYPE_IN_XML_ELEM:
            {
                if (  std::string::npos == ( pos_b_n = strBody.find( find_data_p, pos_b_n ) )
                   || std::string::npos == ( pos_b_n = strBody.find( ">", pos_b_n ) )
                   || std::string::npos == ( pos_e_n = strBody.find( "<", ++pos_b_n ) )
                   )
                {
                    break;
                }

                return strBody.substr(pos_b_n, pos_e_n - pos_b_n);
            }
            //break;
        case DATA_TYPE_IN_XML_ATTR:
            {
                if (  std::string::npos == ( pos_b_n = strBody.find( find_data_p, pos_b_n ) )
                   || std::string::npos == ( pos_b_n = strBody.find( "\"", pos_b_n ) )
                   || std::string::npos == ( pos_e_n = strBody.find( "\"", ++pos_b_n ) )
                   )
                {
                    break;
                }

                return strBody.substr(pos_b_n, pos_e_n - pos_b_n);
            }
            //break;
        default:
            break;
        }
    }

    return "";
}

void SipStackAgentServerImpl::DispatchRespMsg(SIP_STACK_MSG_TYPE type_func, SIP_HEADER *sipHeader, const unsigned long retCode)
{ 
    INFO_LOG("enter response message");

    sipHeader->msgType = type_func;

    RcvMsg(sipHeader);
	return ;
}

void SipStackAgentServerImpl::DispatchReqMsg(SIP_STACK_MSG_TYPE type_func, SIP_HEADER *sipHeader, const unsigned long retCode)
{ 
    INFO_LOG("enter request message");

    sipHeader->msgType = type_func;
    sipHeader->reqMsg = true;

	if(NULL != m_reg_info.m_SIPCallBackFunc)
	{
		if(SIP_STACK_TYPE_MESSAGE == sipHeader->msgType)
		{
			INFO_LOG("Enter UICallBack Function");
			INFO_LOG("SIP_REQUEST_MSG:Type=[%d],Body=[%s]",sipHeader->msgType ,sipHeader->pMsgBody);
			m_reg_info.m_SIPCallBackFunc(SIP_REQUEST_MSG,SIP_REQ_MESSAGE,sipHeader->pMsgBody);
			INFO_LOG("Leave UICallBack Function");
		}
	}

    RcvMsg(sipHeader);
    return ;
}

SipStackAgentClient& SipStackAgentServerImpl::GetClient()
{
    return *m_client_p;
}

CDcgStack& SipStackAgentServerImpl::GetDcgStack()
{
    return m_dcg_stack;
}

void SipStackAgentServerImpl::RcvMsg(SIP_HEADER *sipHeader /*auto_ptr<SipStackMsg> msgInfo*/)
{
    if ( NULL == sipHeader )
    {
        ERROR_LOG("receive message - invalid param");
        return ;
    }
    INFO_LOG("receive message -  begin - Seq = %d, From = %s",sipHeader->seq,sipHeader->from.c_str());

    bool nice_b = false;
    do
    {
        {
            ACE_Read_Guard<ACE_RW_Mutex> guard(m_queue_num_rw_mutex);
            if ( m_cur_queue_num_ul > m_max_queue_num_cul )
            {
                break;
            }
            ++ m_cur_queue_num_ul;
        }//lint !e1788

        ACE_Message_Block * mb_p = new ACE_Message_Block((const char*)(sipHeader));

        ACE_Time_Value timeout = ACE_OS::gettimeofday() + ACE_Time_Value((long long)10, 0);
        if ( -1 == putq( mb_p, &timeout ) )
        {
            ERROR_LOG("receive message - failure to put queue");
            mb_p->release();
            mb_p = NULL;
            break ;
        }

        nice_b = true;
    } while (false);

    if ( !nice_b )
    {
        if ( NULL != sipHeader->pMsgBody )
        {
            delete [] sipHeader->pMsgBody;
            sipHeader->pMsgBody = NULL;
        }
        delete sipHeader;
        sipHeader = NULL;
    }
}//lint !e438

int SipStackAgentServerImpl::svc()
{
    INFO_LOG("server svc - begin.");

    SipStackAgentClientImpl& client_impl_r = dynamic_cast<SipStackAgentClientImpl&>(GetClient());

    const unsigned int max_running_times_for_rest = 10;
    unsigned int running_times_ui = SS_ZERO;
    const ACE_Time_Value too_much_msgs_waiting_tv((long long)0, 50 * 1000);
    const ACE_Time_Value none_msg_waiting_tv((long long)0, 100 * 1000);
    ACE_Message_Block * mb_p    = NULL;
	//lint -e438
    while (m_run_b)
    {
        if ( max_running_times_for_rest < running_times_ui++ )
        {
            ACE_OS::sleep(too_much_msgs_waiting_tv);
        }

		if(NULL == msg_queue())
		{
			WARN_LOG(" msg queue is null");
			continue;
		}

        if ( msg_queue()->is_empty() )
        {
            running_times_ui = SS_ZERO;
            ACE_OS::sleep(none_msg_waiting_tv);
            continue;
        }
        else if ( -1 == getq( mb_p ) )
        {
            WARN_LOG("server svc- failure to getq");
            continue;
        }
        else if ( NULL == mb_p )
        {
            WARN_LOG("server svc- message block is null");
            continue;
        }
        else if ( ACE_Message_Block::MB_DATA != mb_p->msg_type() )
        {
            WARN_LOG("server svc- message block's type is not data.");
            continue;
        }

        {
            ACE_Write_Guard<ACE_RW_Mutex> guard(m_queue_num_rw_mutex);
            -- m_cur_queue_num_ul;
        }//lint !e1788

        //读取内存，并释放
        SIP_HEADER* sip_header_p = (SIP_HEADER*)mb_p->rd_ptr();//lint !e826
		if ( NULL != sip_header_p )
		{
			INFO_LOG("server svc - receive one sip message - Msg_Type=%d, User_Info=%s" ,sip_header_p->msgType,sip_header_p->from.c_str());
			//取得消息，并释放
			auto_ptr<SipStackAgentMsg> sipstack_msg_ptr = GetSipStackMsg( sip_header_p );
			client_impl_r.HandleRcvMsg(sipstack_msg_ptr);
		
			//释放内存
			if ( NULL != sip_header_p->pMsgBody )
            {
				delete [] sip_header_p->pMsgBody;
				sip_header_p->pMsgBody = NULL;
			}
			delete sip_header_p;
			sip_header_p = NULL;
        }		

        mb_p->release();
        mb_p = NULL;
    }
	//lint +e438

    INFO_LOG("server svc - end.");
    return SIP_STACK_SUCCESS;
}

}
//lint +e423
//lint +e429
