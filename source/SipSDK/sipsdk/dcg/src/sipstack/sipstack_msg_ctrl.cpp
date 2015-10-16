#include "tinyxml.h"
#include "sipstack_msg_ctrl.h"
#include "Log.h"

namespace SipStack{

#define SIPSTACK_DEQUE_LENGTH_DFLT                       (3000)

CSipStackCtrlMsg::CSipStackCtrlMsg( unsigned int uiID, const std::string strName ):m_uiID(uiID),m_strName(strName),m_pCtrlMsgBody(NULL)
{
}
CSipStackCtrlMsg::~CSipStackCtrlMsg()
{
    if ( NULL != m_pCtrlMsgBody )
    {
        delete m_pCtrlMsgBody;
        m_pCtrlMsgBody = NULL;
    }
}
unsigned int CSipStackCtrlMsg::GetID() const
{
    return m_uiID;
}
CSipStackCtrlMsgBody* CSipStackCtrlMsg::GetBody() const
{
    return m_pCtrlMsgBody;
}
void CSipStackCtrlMsg::SetCtrlMsgBody( CSipStackCtrlMsgBody* pCtrlMsgBody )
{
    m_pCtrlMsgBody = pCtrlMsgBody;
}
const std::string& CSipStackCtrlMsg::GetName() const
{
    return m_strName;
}

CSipStackCtrlMsgBody::CSipStackCtrlMsgBody( unsigned int uiBodyType )
    : m_uiBodyType(uiBodyType)
{

}
CSipStackCtrlMsgBody::~CSipStackCtrlMsgBody()
{

}
unsigned int CSipStackCtrlMsgBody::GetBodyType() const
{
    return m_uiBodyType;
}

/*******************************************************/
//用于接收消息体为XML格式的对象
CSipStackCtrlMsgBodyXML::CSipStackCtrlMsgBodyXML()
    : CSipStackCtrlMsgBody(SIP_MSG_BODY_TYPE_XML)
{
}
CSipStackCtrlMsgBodyXML::~CSipStackCtrlMsgBodyXML()
{
}
void CSipStackCtrlMsgBodyXML::SetElement( const std::string& strElem )
{
    m_strElem = strElem;
}
void CSipStackCtrlMsgBodyXML::SetAttribute( const std::string& strAttr )
{
    m_strAttr = strAttr;
}
void CSipStackCtrlMsgBodyXML::SetValue( const std::string& strValue )
{
    m_strValue = strValue;
}
bool CSipStackCtrlMsgBodyXML::IsFound( const std::string& strBody )
{
    std::string strValue("");
    if ( !ParseBody2Find(strBody, strValue) )
    {
        return false;
    }

    if ( 0 != m_strValue.compare(strValue) )
    {
        return false;
    }

    return true;
}
bool CSipStackCtrlMsgBodyXML::ParseBody2Find( const std::string& strBody, std::string& strRet )
{
    if ( strBody.empty() || m_strElem.empty() )
    {
        ERROR_LOG("find value in xml - invalid param");
        return false;
    }

    //1.解析XML
    TiXmlDocument aXmlDoc;
    aXmlDoc.Parse(strBody.c_str(),0,TIXML_ENCODING_LEGACY);

    //2.查找节点
    std::string strTmp("");

    TiXmlHandle aXmlHandle(&aXmlDoc);
    for ( unsigned int nposB = 0, nposE = 0;
          std::string::npos != ( nposE = m_strElem.find('/', nposB) ) || ( std::string::npos != nposB );//lint !e838
          nposB = (std::string::npos == nposE) ? std::string::npos : ( nposE + 1 )
        )
    {
        strTmp = m_strElem.substr(nposB, nposE - nposB );
        if ( strTmp.empty() )
        {
            continue;
        }
        aXmlHandle = aXmlHandle.Child(strTmp.c_str(), 0);
    }

    //3.获得值
    TiXmlElement* pElem = aXmlHandle.ToElement();
    if ( NULL == pElem )
    {
        ERROR_LOG("find value in xml - failure to find element - Element=%s",m_strElem.c_str());
        return false;
    }

    const char* pchTmp = NULL;

    if ( m_strAttr.empty() )
    {
        if ( NULL != ( pchTmp = pElem->GetText() ) )
        {
            strRet = pchTmp;
        }
        else
        {
            ERROR_LOG("find value in xml - failure to get element value - Element=%s",m_strElem.c_str());
			return false;
        }
    }
    else
    {
        if ( NULL != ( pchTmp = pElem->Attribute(m_strAttr.c_str()) ) )
        {
            strRet = pchTmp;
        }
        else
        {
            ERROR_LOG("find value in xml - failure to get attribute value - Element=%s",m_strElem.c_str());
			return false;
        }
    }

    return true;
}

/*******************************************************/

SipStack::CSipStackCtrlMsgMgr CSipStackCtrlMsgMgr::m_instance;

CSipStackCtrlMsgMgr::CSipStackCtrlMsgMgr()
    :m_nQueueLength(SIPSTACK_DEQUE_LENGTH_DFLT)
    ,m_itCurrentCondition(m_mapCtrlMsg.end())
{
    m_mapCtrlMsg[0] = m_mapCtrlMsg[m_nQueueLength];   //同时产生一个队列长度为空与达到最大时的容器数据
}

CSipStackCtrlMsgMgr::CSipStackCtrlMsgMgr( CSipStackCtrlMsgMgr&)
	:m_nQueueLength(SIPSTACK_DEQUE_LENGTH_DFLT)
{

}
CSipStackCtrlMsgMgr::~CSipStackCtrlMsgMgr()
{
    m_itCurrentCondition = m_mapCtrlMsg.end();
    if ( !m_mapCtrlMsg.empty() )
    {
        for ( SS_CTRL_MSG_MAP_T::iterator iterCtrlMap = m_mapCtrlMsg.begin();
              m_mapCtrlMsg.end() != iterCtrlMap;
              iterCtrlMap++
            )
        {
			SS_CTRL_MSG_VEC_T& SSVec = (SS_CTRL_MSG_VEC_T)iterCtrlMap->second;
            if ( SSVec.empty() )
            {
                continue;
            }

            for ( SS_CTRL_MSG_VEC_T::iterator iterCtrlQueue = SSVec.begin();
                  SSVec.end() != iterCtrlQueue;
                  iterCtrlQueue++
                )
            {
                if ( NULL != (*iterCtrlQueue) )
                {
                    delete (*iterCtrlQueue);
                    (*iterCtrlQueue) = NULL;
                }
            }

            SSVec.clear();
        }

        m_mapCtrlMsg.clear();
    }
}
CSipStackCtrlMsgMgr& CSipStackCtrlMsgMgr::operator=( CSipStackCtrlMsgMgr& )
{
    return *this;
}

CSipStackCtrlMsgMgr& CSipStackCtrlMsgMgr::Instance()
{
    return m_instance;
}

SS_CTRL_MSG_MAP_T::key_type CSipStackCtrlMsgMgr::GetQueueLength() const
{
    return m_nQueueLength;
}

void CSipStackCtrlMsgMgr::UpdateCondition ( const SS_CTRL_MSG_MAP_T::key_type nCondition )
{
    if ( m_mapCtrlMsg.empty() )
    {
		INFO_LOG("update condition - control message map is empty.");
		return ;
    }

    if ( m_mapCtrlMsg.end() == m_itCurrentCondition )
    {
        m_itCurrentCondition = m_mapCtrlMsg.begin();
		WARN_LOG("update condition - exception - current condition is invalid.");
	}

    //if (  ( m_mapCtrlMsg.end() == m_itCurrentCondition + 1 )
    //   && ( m_mapCtrlMsg.begin() == m_itCurrentCondition )
    //   )
    //{
    //    INFO_LOG("update condition - only one control message queue." ;
    //    return ;
    //}

    SS_CTRL_MSG_MAP_T::iterator itOther = m_itCurrentCondition;
    if ( nCondition > m_itCurrentCondition->first )
    {
        itOther++;
        if ( m_mapCtrlMsg.end() == itOther )
        {
            //已经遍历到最后一个控制消息队列
            return ;
        }

        if ( nCondition < itOther->first )
        {
            //下一个控制消息队列的条件未满足
            return ;
        }
        else
        {
            //达到下一个控制消息队列条件
            m_itCurrentCondition = itOther;
            return ;
        }
    }
    else
    {
        if ( m_mapCtrlMsg.begin() == itOther )
        {
            //已经遍历到第一个控制消息队列
            return ;
        }

        if ( nCondition >= itOther->first )
        {
            //下一个控制消息队列的条件未满足
            return ;
        }
        else
        {
            //达到下一个控制消息队列条件
            m_itCurrentCondition = itOther;
            return ;
        }
    }
}

bool CSipStackCtrlMsgMgr::IsControl2Drop( SipMsgS& stSipMsg )
{
    if ( m_mapCtrlMsg.empty() )
    {
        //特别地：无控制映射信息，则认为不进行控制。
        INFO_LOG("judge message whether to drop - control message map is empty.") ;
        return false;
    }

    if ( m_mapCtrlMsg.end() == m_itCurrentCondition )
    {
        //迭代器指向非法，默认不进行控制。
        WARN_LOG("judge message whether to drop - exception - current condition is invalid.");
        return false;
    }

	SS_CTRL_MSG_VEC_T& SSVec = (SS_CTRL_MSG_VEC_T)m_itCurrentCondition->second;
    if (  ( 0 == m_itCurrentCondition->first )
       && ( SSVec.empty() )
       )
    {
        //特别地：若消息队列长度为0时，可输出消息队列也为空，则所有消息不进行控制。
        INFO_LOG("judge message whether to drop - needless control.");
        return false;
    }

    for ( SS_CTRL_MSG_MAP_T::iterator iterCtrlMap = m_itCurrentCondition;
          m_mapCtrlMsg.end() != iterCtrlMap;
          iterCtrlMap ++
        )
    {
		SS_CTRL_MSG_VEC_T& SSCMsgVec= (SS_CTRL_MSG_VEC_T)iterCtrlMap->second;
        if ( SSCMsgVec.empty() )
        {
            //可输出消息队列为空，表示所有消息都必须丢弃
            continue;
        }

        for ( SS_CTRL_MSG_VEC_T::iterator iterCtrlQueue = SSCMsgVec.begin();
              SSCMsgVec.end() != iterCtrlQueue;
              iterCtrlQueue++
            )
        {
            if ( FindCtrlMsg(*iterCtrlQueue, stSipMsg) )
            {
                //找到此消息,此消息不进行丢弃。
                return false;
            }
        }
    }

    //可输出队列找不到此消息，丢弃
    return true;
}

bool CSipStackCtrlMsgMgr::FindCtrlMsg( const CSipStackCtrlMsg* pSipStackCtrlMsg, SipMsgS& stSipMsg )
{
    if ( NULL == pSipStackCtrlMsg )
    {
        //无效的指针
        return false;
    }

    if ( stSipMsg.enMsgType != pSipStackCtrlMsg->GetID() )
    {
        //消息类型不同
        return false;
    }

    do 
    {
        if ( NULL == pSipStackCtrlMsg->GetBody() )
        {
            break;
        }

        SipBody    stBody = SipDsmGetBody(&stSipMsg);
        unsigned int nLen = VppStrBufGetLength(stBody);
        if ( 0 == nLen )
        {
            return false;
        }

        std::vector<char> vecBuf(nLen + 1, '\0');

        if ( VPP_SUCCESS != VppStrBufCopyDataOut(stBody, 0, &vecBuf[0], &nLen) )
        {
            return false;
        }

        if ( !pSipStackCtrlMsg->GetBody()->IsFound(&vecBuf[0]) )
        {
            return false;
        }

    } while (false);
	INFO_LOG("message control manager - find message - Name=%s",pSipStackCtrlMsg->GetName().c_str());
	return true;
}

}

