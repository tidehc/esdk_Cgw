#ifndef _IVS_BP_SESSION_MGR_H
#define _IVS_BP_SESSION_MGR_H

#include <map>
#include "bp_stdint.h"
#include "ISession.h"
#include "bp_def.h"
#include "ace_header.h"

NAMESPACE_BP_BEGIN // namespace begin
class EXPORT_DLL CSessionMgr {
public:
    CSessionMgr();
    ~CSessionMgr();

    /**************************************************************************
    * name       : AddSession
    * description: 将会话ID和会话实例做唯一绑定，加入会话管理器
    * input      : 
    *               strSessionId 会话ID
    *               pSession     会话实例
    * return     : 
    *               OK 成功 FAIL 失败
    * remark     : NA
    **************************************************************************/
    int AddSession(const char *strSessionId, ISession* pSession);

    /**************************************************************************
    * name       : CheckTimeout
    * description: 检查所有会话是否超时
    * input      : NA
    * return     : NA
    * remark     : 超时的检测方法和超时后的动作，由各会话类型自行指定
    **************************************************************************/
    int CheckTimeout();

    /**************************************************************************
    * name       : GetSession
    * description: 获取会话实例
    * input      : 
    *               strSessionId 会话ID
    * return     : ISession* 会话ID的对应实例
    * remark     : NA
    **************************************************************************/
    ISession* GetSession(const char *strSessionId);

    /**************************************************************************
    * name       : RemoveSession
    * description: 移除会话实例
    * input      : 
    *               strSessionId 会话ID
    * return     : NA
    * remark     : NA
    **************************************************************************/
    int RemoveSession(const char *strSessionId);

	/**************************************************************************
    * name       : RoutingAll
    * description: 
    * input      : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    int RoutingAll();

	/**************************************************************************
    * name       : SetSessionMapMaxSize
    * description: 设置会话map的最大容量
    * input      : ulSessionMapMaxSize 最大容量
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void SetSessionMapMaxSize(uint32_t uiSessionMapMaxSize) {m_uiSessionMapMaxSize =uiSessionMapMaxSize;};

	/**************************************************************************
    * name       : GetSessionMapMaxSize
    * description: 获取会话map的最大容量
    * input      : NA 
    * return     : 最大容量
    * remark     : NA
    **************************************************************************/
	uint32_t GetSessionMapMaxSize(){return m_uiSessionMapMaxSize;};


protected:
    typedef std::map<const std::string, ISession*> CSessionMap;
    typedef CSessionMap::iterator SessionIter;

    CSessionMap m_SessionMap;

	uint32_t m_uiSessionMapMaxSize;    // 会话map的大小，超过则不能再增加会话
	ACE_Thread_Mutex                    m_mutex;  // 线程负载列表锁;
    time_t m_lastPrintTime;
};

NAMESPACE_BP_END   // namespace end
#endif //_IVS_BP_SESSION_MGR_H
