/********************************************************************
filename    :     bp_dispatcher
author      :     s69859
created     :     2012/09/20
description :     分发器管理类
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/09/20 初始版本
*********************************************************************/
#ifndef BP_DISPATCHER_HREAD_H_
#define BP_DISPATCHER_HREAD_H_

#include "bp_namespace.h"
#include "Cmd.h"
#include "bp_def.h"
#include "Thread.h"
#include "TransactionMgr.h"
#include "bp_map.h"

NAMESPACE_BP_BEGIN // namespace begin

typedef std::map<string,CCmd *> CmdMap;
typedef CmdMap::iterator        CmdMapIterator;

class EXPORT_DLL CDispatcher
{
public:
	virtual ~CDispatcher(void);
	
	static CDispatcher& instance();

    /**
     * 异步消息接收
	 * bWithTransation 是否需要创建Transaction；
	 * uiTimeout ： 如果需要创建Transation，该transaction的超时时间;
     * 成功返回0, 失败返回错误码.
     */
	int dispatcherMsgIn(CCmd* &pCmd, bool bWithTransation, uint32_t uiTimeout, bool bIsBetweenService = false);
	// 异步分发只服务处理service，需要关联后续消息进入同一个service（ID为uiServiceID），底层将创建一个Transaction，建立transid和serviceid的关联关系;
	int dispatcherInWithTransaction(int iNetElemType, const char* pszNetLinkID, 
		uint16_t usiMsgType, const char* pszTransID, ACE_Message_Block* &pMsgBlock, uint32_t uiTimeout = TransactionTimeoutDef);

	// 无需将后续消息关联至当前的service
	int dispatcherInWithoutTransaction(int iNetElemType, const char* pszNetLinkID, 
		uint16_t usiMsgType, const char* pszTransID, ACE_Message_Block* &pMsgBlock);

	 // bWithTransation 是否需要创建Transaction;
	 // uiTimeout ： 如果需要创建Transation，该transaction的超时时间;	
	int dispatcherMsgOut(CCmd* &pCmd, bool bWithTransation, uint32_t uiTimeout);   //异步消息发送
	
	// 异步发送，需要关联后续消息进入同一个service（ID为uiServiceID），底层将创建一个Transaction，建立transid和serviceid的关联关系;
	int dispatcherOutWithTransaction(uint32_t uiServiceID, int iNetElemType, const char* pszNetLinkID, 
		const char* pszTransID, uint32_t uiReqID, ACE_Message_Block* &pMsgBlock, uint32_t uiTimeout = TransactionTimeoutDef, bool bCompress = false);   //异步消息发送

	// 异步发送，无需将后续消息关联至当前的service
	int dispatcherOutWithoutTransaction(int iNetElemType, const char* pszNetLinkID, const char* pszTransID, 
		uint32_t uiReqID, ACE_Message_Block* &pMsgBlock, bool bCompress = false);   //异步消息发送

	CCmd* dispatcherSyncMsgOut(CCmd*& pCmd, uint32_t uiTimeout = 5);  //同步消息发送，返回值为同步消息的返回
	CCmd* dispatcherSyncMsgOut(uint32_t uiServiceID, int iNetElemType, const char* pszNetLinkID, 
		const char* pszTransID, uint32_t uiReqID, ACE_Message_Block*& pMsgBlock, bool bCompress = false, uint32_t uiTimeout = 5);   //同步消息发送，返回值为同步消息的返回

	CCmd* dispatcherSyncMsgIn(CCmd*& pCmd, uint32_t uiTimeout = 5);  //同步向内发送消息
	CCmd* dispatcherSyncMsgIn(uint32_t uiServiceID, uint16_t usiMsgType, const char* pszTransID, ACE_Message_Block* &pMsgBlock, uint32_t uiTimeout = 5);

    /**
     * 同一进程内一个服务向另外一个服务发送请求
     * 成功返回0, 失败返回错误码.
     */
	int dispatcherMsgBetweenService(uint32_t uiServiceID, uint16_t usiMsgType, const char* pszTransID, ACE_Message_Block* &pMsgBlock);

	int dispatcherSyncMsgOutBatch(CmdMap &cmdMap, uint32_t uiTimeout);

		/**
     *分发至ServiceId指定的线程
     * 成功返回0, 失败返回错误码.
     */	
	int dispatcherMsgInByServiceId(CCmd* pCmd);			//异步发送
protected:
    /**
     * 处理新事务请求消息.
     * 成功返回0, 失败返回错误码.
     */
    int NewTransaction(const std::string& strTransID, 
		const std::string& strNetLinkID, uint32_t uiServiceID, const ACE_thread_t& threadID, uint32_t uiTimeout);

    /**
     * 处理同步请求消息的响应消息
     * 成功返回0, 失败返回错误码.
     */
    int HandleSyncRespond(CCmd& cmd);
private:
	CDispatcher(void);

    static CDispatcher oDispatcher;
};
NAMESPACE_BP_END   // namespace end

#endif /* BP_DISPATCHER_HREAD_H_ */

