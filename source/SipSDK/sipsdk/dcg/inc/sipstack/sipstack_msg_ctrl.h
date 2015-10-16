#ifndef IVSCBB_SS_SIPSTACK_AGENT_MSG_CTRL_H
#define IVSCBB_SS_SIPSTACK_AGENT_MSG_CTRL_H

#include <string>
#include <vector>
#include <map>
#include "ssincludeall.h"
#include <ace_header.h>

namespace SipStack{

typedef enum SIP_MSG_BODY_TYPE_ENUM
{
    SIP_MSG_BODY_TYPE_XML,
    SIP_MSG_BODY_TYPE_SDP
}SIP_MSG_BODY_TYPE_EN;

/*******************************************************/
//消息控制类的基础类型
class CSipStackCtrlMsgBody;

class CSipStackCtrlMsg
{
    unsigned int          m_uiID;
    std::string           m_strName;
    CSipStackCtrlMsgBody* m_pCtrlMsgBody;

public:
    CSipStackCtrlMsg ( unsigned int uiID, const std::string strName );
    virtual ~CSipStackCtrlMsg ();
    void SetCtrlMsgBody ( CSipStackCtrlMsgBody* pCtrlMsgBody );

    unsigned int GetID () const;
    const std::string& GetName () const;
    CSipStackCtrlMsgBody* GetBody () const;
};

class CSipStackCtrlMsgBody
{
    const unsigned int m_uiBodyType;

public:
    CSipStackCtrlMsgBody ( unsigned int uiBodyType );
    virtual ~CSipStackCtrlMsgBody ();

    unsigned int GetBodyType () const;
    virtual bool IsFound ( const std::string& strBody ) = 0;
};

/*******************************************************/
//用于接收消息体为XML格式的对象
class CSipStackCtrlMsgBodyXML : public CSipStackCtrlMsgBody
{
    std::string m_strElem;
    std::string m_strAttr;
    std::string m_strValue;
public:
    CSipStackCtrlMsgBodyXML ();
    virtual ~CSipStackCtrlMsgBodyXML ();

    void SetElement ( const std::string& strElem );
    void SetAttribute ( const std::string& strAttr );
    void SetValue ( const std::string& strValue );

    virtual bool IsFound ( const std::string& strBody );
    bool ParseBody2Find ( const std::string& strBody, std::string& strRet );
};
/*******************************************************/
typedef std::vector<CSipStackCtrlMsg*>             SS_CTRL_MSG_VEC_T;
typedef std::map<unsigned int, SS_CTRL_MSG_VEC_T>  SS_CTRL_MSG_MAP_T;
//流控管理类
class CSipStackCtrlMsgMgr
{
    friend class CSipStackConfigMgr;

    static CSipStackCtrlMsgMgr m_instance;
    SS_CTRL_MSG_MAP_T          m_mapCtrlMsg;

    SS_CTRL_MSG_MAP_T::key_type      m_nQueueLength;

    SS_CTRL_MSG_MAP_T::iterator      m_itCurrentCondition;

private:
    CSipStackCtrlMsgMgr ();
    ~CSipStackCtrlMsgMgr();
    CSipStackCtrlMsgMgr ( CSipStackCtrlMsgMgr& );
    CSipStackCtrlMsgMgr& operator = ( CSipStackCtrlMsgMgr& );

    bool FindCtrlMsg ( const CSipStackCtrlMsg* pSipStackCtrlMsg,
                       SipMsgS& stSipMsg
                     );

public:
    static CSipStackCtrlMsgMgr& Instance();

    SS_CTRL_MSG_MAP_T::key_type GetQueueLength () const;

    void UpdateCondition ( const SS_CTRL_MSG_MAP_T::key_type nCondition );

    bool IsControl2Drop ( SipMsgS& stSipMsg);
};

#define  SS_CTRL_MSG_MGR_INS_REF  (CSipStackCtrlMsgMgr::Instance())

}

#endif //sipstack_msg_ctrl.h
