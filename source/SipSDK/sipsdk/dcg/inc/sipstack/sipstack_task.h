/********************************************************************
filename    :    sipstack_task.h
author      :    cwx148380
created     :    2013/4/1
description :    为解决协议栈多线程堵塞，与MINISIP只支持多线程的问题。添加此类作为协议栈主线程。
                 其他线程使用消息队列与协议栈主线程通信。
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/4/1 初始版本
*********************************************************************/

#ifndef IVSCBB_SS_SIPSTACK_WORKER_H
#define IVSCBB_SS_SIPSTACK_WORKER_H

#include <list>
#include "ace_header.h"
#include "MiniSip.h"

/*******************************************************/
//#define SS_ZERO                       (0)
//#define SS_ONE                        (1)
#define SS_TWO                        (2)
#define SS_MAX_REPEAT_TIMES           (3)
#define SS_MAX_QUEUE_COUNT            (2048)

#define SS_ZERO_D                     (0.0)
#define SS_SIXTY_SECS_D               (60.0)

#define SS_AGENT_NAME_DFLT            ("huawei.com")
#define SS_MIN_PORT_NUM               (0)
#define SS_MAX_PORT_NUM               (65535)
/*******************************************************/

/*******************************************************/
//ace
#define ACE_SPAWN_FAIL                (-1)
#define ACE_Q_FAIL                    (-1)

typedef ACE_Thread_Manager            ACE_THD_MGR;
typedef ACE_Thread_Mutex              ACE_THD_MUTEX;
typedef ACE_RW_Thread_Mutex           ACE_RW_THD_MUTEX;
typedef ACE_Message_Block             ACE_MSG_BLK;
typedef ACE_Message_Queue<ACE_SYNCH>  ACE_MSG_SYN_Q;

typedef ACE_thread_t                  ACE_THD_ID;
typedef ACE_hthread_t                 ACE_HTHD;

#define ACE_TIME_ONE_SEC              (1)
typedef ACE_Time_Value                ACE_TIME_VALUE;

#define ACE_SNPRINTF                  ACE_OS::snprintf
#define ACE_VSNPRINTF                 ACE_OS::vsnprintf

#define ACE_ATOI                      ACE_OS::atoi

#define ACE_LOCALTIME                 ACE_OS::localtime
#define ACE_GET_TIME_OF_DAY           ACE_OS::gettimeofday

#define ACE_INET_ADDR                 ACE_OS::inet_addr

#define ACE_SLEEP                     ACE_OS::sleep
/*******************************************************/


namespace SipStack{

#pragma pack(push, 1)
typedef struct POST_TO_APP_STU
{
    SipString       message_string_st;
    SipTptNwInfoS   tpt_network_info_st;
}POST_TO_APP_ST;
#pragma pack(pop)

class CPostToPu
{
public:
    CPostToPu ( const std::string&   ipv4_str,
                const unsigned short port_us
              );
    virtual ~CPostToPu ();

    virtual bool IsValid ();

    const std::string& GetIPv4 () const;
    unsigned short     GetPort () const;

    virtual int Execute();

private:
    std::string     m_strIPv4;
    unsigned short  m_usPort;
};

/**********************************************************************/
class CRequestToPu : public CPostToPu
{
public:
    CRequestToPu ( const std::string& strIPv4, const unsigned short usPort,
                   const std::string& strUriUserInfo, const std::string& strUriHostport
                 );
    virtual ~CRequestToPu ();

    const std::string& GetUriUserInfo () const;
    const std::string& GetUriHostport () const;
    const std::string& GetAgentName   () const;
    void SetSync(bool bSync = true );
    bool IsSync () const;

    static int ParseUri ( const std::string& strUri,
                          std::string& strUriUserInfo, std::string& strUriHostport
                        );

    virtual bool IsValid ();

private:
    bool            m_bSync;           //同步标志
    std::string     m_strUriUserInfo;
    std::string     m_strUriHostport;
    std::string     m_strAgentName;
};

class CRequestInviteToPu : public CRequestToPu
{
public:
    CRequestInviteToPu( const std::string& strIPv4, const unsigned short usPort,
                        const std::string& strUriUserInfo, const std::string& strUriHostport,
                        const std::string& strCallID, const std::string& strSubject, const std::string& strBody
                      );
    virtual ~CRequestInviteToPu();

    const std::string& GetCallID()  const;
    const std::string& GetSubject() const;
    const std::string& GetBody()    const;

    virtual bool IsValid ();
    virtual int Execute();

private:
    std::string m_strCallID;
    std::string m_strSubject;
    std::string m_strBody;
};

class CRequestAckToPu : public CRequestToPu
{
public:
    CRequestAckToPu( const std::string& strIPv4, const unsigned short usPort,
                     const unsigned long nRespID
                   );
    virtual ~CRequestAckToPu();

    unsigned int GetRespID () const;

    virtual bool IsValid ();
    virtual int Execute();
private:
    unsigned int m_nRespID;
};

class CRequestByeToPu : public CRequestToPu
{
public:
    CRequestByeToPu( const std::string& strIPv4, const unsigned short usPort,
                     const unsigned long nRespID
                   );
    virtual ~CRequestByeToPu();

    unsigned int GetRespID () const;

    virtual bool IsValid ();
    virtual int Execute();
private:
    unsigned int m_nRespID;
};

class CRequestMessageToPu : public CRequestToPu
{
public:
    CRequestMessageToPu( const std::string& strIPv4, const unsigned short usPort,
                         const std::string& strUriUserInfo, const std::string& strUriHostport,
                         const std::string& strBody,const std::string& strCallID
                       );
    virtual ~CRequestMessageToPu();

    const std::string& GetBody () const;

    virtual bool IsValid ();
    virtual int Execute();
private:
	std::string m_strCallID;
private:
    std::string m_strBody;
};

class CRequestSubScribeToPu : public CRequestToPu
{
public:
    CRequestSubScribeToPu( const std::string& strIPv4, const unsigned short usPort,
                           const std::string& strUriUserInfo, const std::string& strUriHostport,
                           const std::string& strBody
                         );
    virtual ~CRequestSubScribeToPu();

    const std::string GetBody() const;

    virtual bool IsValid();
    virtual int Execute();

private:
    std::string m_strBody;
};

class CRequestRegisterToPu : public CRequestToPu
{
public:
	CRequestRegisterToPu( const std::string& strIPv4, const unsigned short usPort,
		const std::string& strUriUserInfo, const std::string& strUriHostport,const std::string& strCallID);
	virtual ~CRequestRegisterToPu();
	
	virtual bool IsValid();
	virtual int Execute();

	const std::string& GetCallID()  const;
private:
	std::string m_strCallID;
};

class CRequestUnRegisterToPu : public CRequestToPu
{
public:
	CRequestUnRegisterToPu( const std::string& strIPv4, const unsigned short usPort,
		const std::string& strUriUserInfo, const std::string& strUriHostport,const std::string& strCallID);
	virtual ~CRequestUnRegisterToPu();
	virtual bool IsValid();
	virtual int Execute();
	const std::string& GetCallID()  const;
private:
	std::string m_strCallID;
};

/**********************************************************************/

/**********************************************************************/
class CRespondToPu : public CPostToPu
{
public:
    CRespondToPu ( const unsigned long  resp_id_ui,
                   const unsigned int   resp_code_ui,
                   const std::string&   ipv4_str,
                   const unsigned short port_us
                 );
    virtual ~CRespondToPu();

    unsigned long GetRespID () const;
    unsigned int GetRespCore () const;

private:
    unsigned long    m_uiRespID;      //回复ID
    unsigned int     m_uiRespCode;    //响应码
    std::string      m_strIPv4;       //IPv4地址
    unsigned short   m_usPort;        //端口
};

class CRepondRegisterToPu : public CRespondToPu
{
public:
    CRepondRegisterToPu ( const unsigned long  resp_id_ui,
                          const unsigned int   resp_code_ui,
                          const std::string&   ipv4_str,
                          const unsigned short port_us
                        );
    virtual ~CRepondRegisterToPu();

    virtual int Execute();
};

class CRepondMessageToPu : public CRespondToPu
{
public:
    CRepondMessageToPu ( const unsigned long  resp_id_ui,
                         const unsigned int   resp_code_ui,
                         const std::string&   ipv4_str,
                         const unsigned short port_us
                       );
    virtual ~CRepondMessageToPu();

    virtual int Execute();
};

class CRepondOptionsToPu : public CRespondToPu
{
public:
    CRepondOptionsToPu ( const unsigned long  resp_id_ui,
                         const unsigned int   resp_code_ui,
                         const std::string&   ipv4_str,
                         const unsigned short port_us
                       );
    virtual ~CRepondOptionsToPu();

    virtual int Execute();
};
/**********************************************************************/

typedef std::list<CRequestToPu* >    REQ_TO_PU_Q;
typedef std::list<CRespondToPu* >    RESP_TO_PU_Q;
typedef std::list<POST_TO_APP_ST* >  POST_TO_APP_Q;

class CSipStackTask : public ACE_Task<ACE_MT_SYNCH>
{
private:
    CSipStackTask();
    virtual ~CSipStackTask();
	CSipStackTask (const CSipStackTask& );
    CSipStackTask& operator=(CSipStackTask& );

public:
    static CSipStackTask& Instance();
    static ACE_Recursive_Thread_Mutex& MiniSipMutex();

    bool Init ();

    void Fini ();

    virtual int svc();

    //线程安全的函数
    bool RequestToPu( IN CRequestToPu* request_to_pu_msg_p );

    void HandleRequestMession( IN CRequestToPu* respond_to_pu_msg_p );

    bool RespondToPu( IN CRespondToPu* respond_to_pu_msg_p );

    void HandleRespondMession( IN CRespondToPu* respond_to_pu_msg_p );

    //线程安全的函数
    bool PostToApp( IN POST_TO_APP_ST* post_msg_p );

private:
    void HandleRecvMession( IN POST_TO_APP_ST* post_msg_p );

private:
    static CSipStackTask     m_instance;

    bool                       m_init_b;
    ACE_Recursive_Thread_Mutex m_init_mutex;               //仅用于初始化的锁

    ACE_THD_ID                 m_thd_id ;                  //协议栈工作主线程的线程ID
    bool                       m_work_b;                   //协议栈处理线程工作标示

    static ACE_Recursive_Thread_Mutex m_minisip_mutex;     //用于锁住MINISIP，保障MINISIP原子操作。

    //发送给PU的请求消息队列
    REQ_TO_PU_Q                m_req_to_pu_q[SS_TWO];
    unsigned int               m_req_to_pu_q_read_ui;
    ACE_Recursive_Thread_Mutex m_req_to_pu_q_mutex;

    //发送给PU的响应消息队列
    RESP_TO_PU_Q               m_resp_to_pu_q[SS_TWO];
    unsigned int               m_resp_to_pu_q_read_ui;
    ACE_Recursive_Thread_Mutex m_resp_to_pu_q_mutex;

    //发送给APP的消息队列
    POST_TO_APP_Q              m_post_to_app_q;
    unsigned int               m_post_to_app_q_ui;
    ACE_Recursive_Thread_Mutex m_post_to_app_mutex;

};

}

#endif //sipstack_main_thread.h
