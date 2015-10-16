#ifndef IVSCBB_SS_SIPSTACK_AGENT_SERVER_IMPL_HPP
#define IVSCBB_SS_SIPSTACK_AGENT_SERVER_IMPL_HPP

#include <tinyxml.h>

#include "sipstack_agent_server.hpp"
#include "sipstack_agent_client.hpp"
#include "sipstack_agent_header.hpp"
#include "sipstack_agent_msg_type.hpp"

#include "CDcgStack.hpp"

//#define TEYES_PU_TYPE                           ("MsgType")
//#define TEYES_PU_REGISTER                       ("REGISTER")
//#define TEYES_PU_TYPE_KEEP_ALIVE                ("Keepalive")
//#define TEYES_PU_TYPE_ALARM                     ("Alarm")
//#define TEYES_PU_INVITE_RESP                    ("InviteResp")
//#define TEYES_PU_GET_PLATFORM_INFO              ("MSG_VCU_GET_PLATFORM_INFO_REQ")
//#define TEYES_PU_TYPE_LENGTH                    (30)

#define T28181_XML_CMD_TYPE                         "CmdType"
#define T28181_XML_CMD_TYPE_LENGTH                  (20)
#define T28181_XML_CMD_TYPE_ALARM                   "Alarm"
#define T28181_XML_CMD_TYPE_KEEP_ALIVE              "Keepalive"


//#define T28181_CMD_CONTROL                      "Control"
//#define T28181_CMD_QUERY                        "Query"
//#define T28181_CMD_NOTIFY                       "Notify"
//#define T28181_CMD_RESPONSE                     "Response"
//#define T28181_CMD_REGISTER                     "REGISTER"
//#define T28181_CMD_INVITE_RESP                  "InviteResp"
//#define T28181_CMD_TYPE_DEVICE_CONTROL          "DeviceControl"

//#define T28181_CMD_TYPE_CATALOG                 "Catalog"
//#define T28181_CMD_TYPE_DEVICE_INFO             "DeviceInfo"
//#define T28181_CMD_TYPE_DEVICE_STATUS           "DeviceStatus"
//#define T28181_CMD_TYPE_RECORD_INFO             "RecordInfo"
//#define T28181_CMD_TYPE_MEDIA_STATUS            "MediaStatus"


namespace SipStack{

class SipStackAgentClient;
class SipStackAgentCfg; 
class SipStackMsgImplMiniSip;
class DcgStack;

typedef enum DATA_TYPE_IN_XML_ENUM
{
    DATA_TYPE_IN_XML_ELEM,
    DATA_TYPE_IN_XML_ATTR
}DATA_TYPE_IN_XML_EN;

#define SIZE_OF_DATA_TYPE_NAME        (16)
typedef struct DATA_TYPE_IN_XML_STU
{
    const int    data_type_in_xml_n;
    const char   data_type_name_sz[SIZE_OF_DATA_TYPE_NAME];
}DATA_TYPE_IN_XML_ST;

class SipStackAgentServerImpl: public SipStackAgentServer, ACE_Task<ACE_MT_SYNCH>
{
public:
    SipStackAgentServerImpl(SipStackAgentCfg const & regInfo, unsigned long maxNum);
    virtual ~SipStackAgentServerImpl();

public:
    bool Init ( SipStackAgentClient* sipstack_agent_client_p );
    void Fini ();
    virtual SipStackAgentCfg & GetAgentConfig(); //impl

public:
    virtual int svc();

public:
    SipStackAgentClient& GetClient();
    CDcgStack& GetDcgStack();

    void DispatchRespMsg(SIP_STACK_MSG_TYPE type_func, SIP_HEADER *sipHeader, const unsigned long retCode);
    void DispatchReqMsg(SIP_STACK_MSG_TYPE type_func, SIP_HEADER *sipHeader, const unsigned long retCode);

public:
    void RcvMsg(SIP_HEADER* msgInfo);

protected:
    auto_ptr<SipStackAgentMsg> GetSipStackMsg(const SIP_HEADER *sipHeader);

    unsigned long GetThreadNum();

    std::string FindDataFromXml ( const std::string&        strBody,
                                  const DATA_TYPE_IN_XML_ST data_type_in_xml_st_arr[],
                                  const int                 data_type_count_n
                                );

    //bool praseIvsMsgInfo(TiXmlElement *root, string &msgCmd, string childElement, string childAttribute, string subchildElement);
    //bool praseTeyesMsgInfo(TiXmlElement *root, string &msgCmd, string childElement);

private:
    bool                            m_init_b;
    bool                            m_run_b;

    SipStackAgentCfg                 m_reg_info;
    SipStackAgentClient*                 m_client_p;
    CDcgStack                       m_dcg_stack;

    ACE_RW_Mutex                    m_queue_num_rw_mutex;
    const unsigned long             m_max_queue_num_cul;
    unsigned long                   m_cur_queue_num_ul;

    const unsigned long             m_thread_num_cul;
    //ACE_Message_Queue<ACE_SYNCH>    m_sip_queue;
};

}


#endif


