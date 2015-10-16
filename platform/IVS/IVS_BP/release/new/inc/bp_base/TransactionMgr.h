/********************************************************************
filename    :    TransactionMgr.h
author      :    s00191067
created     :    2012/03/27
description :    事务管理类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/27 初始版本
*********************************************************************/
#ifndef TRANSACTION_MGR_H
#define TRANSACTION_MGR_H
#include <map>
#include <string>
#include "ace_header.h"
#include "bp_def.h"
#include "bp_stdint.h"
#include "Cmd.h"

NAMESPACE_BP_BEGIN // namespace begin

const int TransactionTimeoutDef = 30;

typedef struct TrasactionStaticAttr
{
	std::string     szTransID;
	ACE_thread_t    iThreadID;
	std::string     szNetLinkID;
	uint32_t        uiServiceID;
	bool            bIsSync;
	TrasactionStaticAttr()
	{
		szTransID = "";
		iThreadID = 0;
		szNetLinkID = "";
		uiServiceID = 0;
		bIsSync = false;
	}
} TRANSACTION_STATIC_ATTR;

class EXPORT_DLL ITransaction
{
public:
    ITransaction(void);
    virtual ~ITransaction(void);
    // 获取TransID
    const std::string& GetTransID();

    // 设置TransID
    void SetTransID(const std::string& strTransID);

    // 获取ThreadID
    ACE_thread_t GetThreadId();

    // 设置ThreadID
    void SetThreadID(const ACE_thread_t& threadID);

	// 获取LinkID
	const std::string& GetNetLinkID();

	// 设置NetLinkID
	void SetNetLinkID(const std::string& strLinkID);

    // 获取ServiceID
    uint32_t GetServiceID();

	// 获取ServiceID
	void SetServiceID(uint32_t iServiceID);

	void SetTimeout(uint32_t uiTimeout);
	void UpdateTime();
	bool IsTimeout(time_t tCurTime);

    // 设置为同步事务
    void setSync(bool bIsSync);

	// 判断是否为同步事务
	bool isSync();

	//初始化同步事务
	void initSync(void);
    
	//设置响应消息
	void setCmd(CCmd* pCmd);

	//获取响应消息
	CCmd* getCmd();

	ACE_Auto_Event* getSyncLock();

protected:
    std::string     m_strTransID;
    ACE_thread_t    m_threadID;
	std::string     m_netLinkID;
    uint32_t        m_uiServiceID;
	uint32_t        m_uiTimeout;
	time_t          m_tUpdateTime;
	bool            m_isSync;  // 是否同步事务
	ACE_Auto_Event* m_pWait;  //同步事务锁
	CCmd* m_pCmd;              //同步响应消息
};


class EXPORT_DLL CBPTransaction : public ITransaction
{
public:
    CBPTransaction(void);
    virtual ~CBPTransaction(void);
    static CBPTransaction* NewInstance();
};

class EXPORT_DLL CBPSyncTransaction : public ITransaction
{
public:
	CBPSyncTransaction(void);
	virtual ~CBPSyncTransaction(void);
	static CBPSyncTransaction* NewInstance();
};


const time_t TRANSACTION_TIMEOUT_CHECK_INTERVAL = 10;

class EXPORT_DLL CTransactionMgr
{
public:
    ~CTransactionMgr(void);

    static CTransactionMgr& instance();

    void close();

    /**************************************************************************
    * name       : AddTransaction
    * description: 增加事务
    * input      : strTransationID 事务ID pTransation 事务对象
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
    int AddTransaction(const std::string& strTransationID, ITransaction* pTransation);

    /**************************************************************************
    * name       : GetThreadID
    * description: 根据事务ID获取线程ID
    * input      : strTransationID 事务ID
    * output     : NA
    * return     : ACE_thread_t 线程ID
    * remark     : NA
    **************************************************************************/
    ACE_thread_t GetThreadID(const std::string& strTransationID);


	/**************************************************************************
    * name       : GetThreadID
    * description: 根据事务ID获取线程ID,减少遍历map的次数
    * input      : strTransationID 事务ID, 返回的是否是同步事务
    * output     : NA
    * return     : ACE_thread_t 线程ID
    * remark     : NA
    **************************************************************************/
	void GetTransActionAttr(const std::string& strTransationID, TRANSACTION_STATIC_ATTR& stTransactionAttr);


    /**************************************************************************
    * name       : GetServiceID
    * description: 根据事务ID获取ServiceID
    * input      : strTransationID 事务ID
    * output     : NA
    * return     : 事务ID
    * remark     : NA
    **************************************************************************/
    uint32_t GetServiceID(const std::string& strTransationID);

    /**************************************************************************
    * name       : RemoveTransaction
    * description: 删除事务
    * input      : strTransationID 事务ID
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void RemoveTransaction(const std::string& strTransationID);

    /**************************************************************************
    * name       : GetLinkID
    * description: 获得链接ID
    * input      : strTransationID 事务ID
    * output     : NA
    * return     : 链接ID
    * remark     : NA
    **************************************************************************/
	const std::string& GetLinkID(const std::string& strTransationID);

    /**************************************************************************
    * name       : Begin
    * description: 开始事务处理
    * input      : pszTransID: 事务id
	*              reqID     : 请求id
	*              netType   : 网元类型
	*              strLinkId  : 连接号
	*              pPkgData  : 数据
	*              iDataLen  : 数据长度
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
	int Begin(const char* pszTransID, uint32_t reqID, int netType, const std::string& strLinkId, const char* pPkgData, unsigned int iDataLen);

    /**************************************************************************
    * name       : End
    * description: 关闭事务处理
    * input      : pszTransID: 事务id
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
	int End(const char* pszTransID);
	
	/**************************************************************************
    * name       : getTransactionSyncLock
    * description: 获取事务同步锁
    * input      : pszTransID: 事务id
    * output     : NA
    * return     : 同步事务锁
    * remark     : NA
    **************************************************************************/
	ACE_Auto_Event* getTransactionSyncLock(const std::string& strTransationID);

	/**************************************************************************
    * name       : setSyncTransactionResp
    * description: 设置同步事务的响应消息
    * input      : pszTransID: 事务id, 响应消息指针
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA
    **************************************************************************/
	int setSyncTransactionResp(const std::string& strTransationID, CCmd* pCmd);

   /**************************************************************************
    * name       : getSyncTransactionResp
    * description: 获取同步事务的响应消息
    * input      : pszTransID: 事务id, 响应消息指针
    * output     : NA
    * return     : CCmd的指针
    * remark     : NA
    **************************************************************************/
	CCmd* getSyncTransactionResp(const std::string& strTransationID);

    /**************************************************************************
    * name       : ifSyncTransaction
    * description: 判断是否是同步事务
    * input      : pszTransID: 事务id
    * output     : NA
    * return     : true - 是同步事务，false - 是异步事务
    * remark     : NA
    **************************************************************************/
	bool ifSync(const std::string& strTransationID);

	/**************************************************************************
    * name       : GetTransactionCount
    * description: 获取transaction的当前数量
    * input      : NA
    * output     : NA
    * return     : transaction的当前数量
    * remark     : NA
    **************************************************************************/
	uint32_t GetTransactionCount();
	
   /**************************************************************************
    * name       : CheckTimeout
    * description: 检查所有Transaction是否超时, 超时则清理掉
    * input      : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void CheckTimeout(time_t tCurrentTime);
	/**************************************************************************
    * name       : SetTranMapMaxSize
    * description: 设置transaction map的最大容量
    * input      : uiTranMapMaxSize transaction map的最大容量
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void SetTranMapMaxSize(uint32_t uiTranMapMaxSize){m_uiTranMapMaxSize = uiTranMapMaxSize;};

	/**************************************************************************
    * name       : GetTranMapMaxSize
    * description: 获取transaction map的最大容量
    * input      : NA
    * output     : NA
    * return     : transaction map的最大容量
    * remark     : NA
    **************************************************************************/
	uint32_t GetTranMapMaxSize(){return m_uiTranMapMaxSize;};

private:
    CTransactionMgr(void);
	// 根据TransID获取对象
	ITransaction* GetTransaction(const std::string& strTransationID);

private:
    typedef std::map<std::string, ITransaction*>     CTransactionMap;
    typedef CTransactionMap::iterator                TransactionMapIter;

    CTransactionMap m_mapTransaction;
    ACE_Thread_Mutex                    m_mutex;

	uint32_t m_uiTranMapMaxSize;
};

NAMESPACE_BP_END   // namespace end
#endif //TRANSACTION_MGR_H
