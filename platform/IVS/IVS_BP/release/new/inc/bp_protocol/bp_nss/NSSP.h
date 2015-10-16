/********************************************************************
filename    :    NSSP.h
author      :    w90004666
created     :    2012/9/19
description :	 NSS协议头文件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/19 初始版本
*********************************************************************/
#ifndef NSSP_H
#define NSSP_H

#include <map>
#include <time.h>
#include "ace_header.h"
#include "IProtocol.h"
#include "bp_namespace.h"
#include "bp_stdint.h"
#include "bp_nss_def.h"

NAMESPACE_BP_BEGIN // namespace begin




class CNSSP : public IProtocol
{
public:
	// 构造函数
	CNSSP();
	// 析构函数
	virtual ~CNSSP();
private:
    CNSSP(const CNSSP &src);

    CNSSP& operator=(const CNSSP&);

public:
	
	/**************************************************************************
    * name       : NotifyOpen
    * description: 网元打开后，Open方法可以初始化一些必要数据
    * input      : void
    * output     : NA
    * return     : void
    * remark     : NA
    **************************************************************************/
	virtual void NotifyOpen(void);

	/**************************************************************************
    * name       : NotifyClose
    * description: 网元链接断开后，做一些必要的清理工作
    * input      : void
    * output     : NA
    * return     : void
    * remark     : NA
    **************************************************************************/
	virtual void NotifyClose(void);

    /**************************************************************************
    * name       : NotifyTimeout
    * description: 通知协议栈连接超时
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void NotifyTimeout(void);

	/**************************************************************************
    * name       : NotifyRecv
    * description: 从网元获取数据进行处理
    * input      : void
    * output     : NA
    * return     : IVS_SUCCESS 成功 IVS_FAIL 失败
    * remark     : NA
    **************************************************************************/
	virtual int NotifyRecv(void);

	virtual void HandleCmd(CCmd* pCmd);
		
	// 列程处理
	virtual void Routing(void);

    inline void SetHeartBeat(unsigned int uiHeartBeat) { m_uiHeartBeat = uiHeartBeat; }

    inline void SetTimeout(unsigned int uiTimeout) { m_uiTimeout = uiTimeout; }

	virtual void LinkStatusReport(enum enLinkState linkState, const char* pszTransID);
private:


    typedef std::map<string,string> HTTPHeaderMap;
    typedef HTTPHeaderMap::iterator HTTPHeaderMapIter;

    /**************************************************************************
    * name       : ResetProtocol
    * description: 重置协议栈状态
    * input      : void
    * output     : NA
    * return     : void
    * remark     : NA
    **************************************************************************/
    void ResetProtocol();

    /**************************************************************************
    * name       : Login
    * description: NSSP登录操作
    * input      : 
    * output     : NA
    * return     : int
    * remark     : NA
    **************************************************************************/
	int Login();

    /**************************************************************************
    * name       : ProcessLogin
    * description: 登录协议解析
    * input      : 
    * output     : NA
    * return     : 是否能解析登录协议
    * remark     : NA
    **************************************************************************/

    int ProcessLoginReq(const char* pszMsg, uint32_t uiMsgLength);
    int ProcessLoginRsp(const char* pszMsg, uint32_t uiMsgLength);
    int ProcessHeartbeatReq(const char* pszMsg, uint32_t uiMsgLength);
	int ProcessHeartbeatRsp(const char* pszMsg, uint32_t uiMsgLength);	


    /**************************************************************************
    * name       : SendHeartbeat
    * description: 发起心跳
    * input      : 
    * output     : NA
    * return     : 
    * remark     : NA
    **************************************************************************/    
	int SendHeartbeat(); 

	/**************************************************************************
    * name       : SendSystemBusyMsg
    * description: 发送该系统忙消息
    * input      : 
    * output     : NA
    * return     : 
    * remark     : NA
    **************************************************************************/  
	void SendErrorRsp(TNssMsgHeader* nssMgsHeader, int iErrorType);

    /**************************************************************************
    * name       : SendRequest
    * description: 发送请求消息
    * input      : pszSendData 要发送的数据缓冲区指针
                   uiDataLength 消息长度
                   bCompress    是否要压缩消息
    * output     : NA
    * return     : 发送成功失败
    * remark     : NA
    **************************************************************************/    
    int SendRequest(const char* pszSendData,unsigned int uiDataLength, bool bNeedCompress, bool bIsHeartBeatMsg = false);

    /**************************************************************************
    * name       : ProcessData
    * description: 处理数据包
    * input      : 
    * output     : 
    * return     : 返回成功失败
    * remark     : NA
    **************************************************************************/
	int ProcessData();

	bool SendRegistReqMsg(void);

	bool SendRegistRespMsg(const TNssMsgHeader* pReqMsg, uint32_t uRespCode);

	bool UpdateLinkId(const std::string& strLinkId);

    // 通知上层建立连接(同步方式创建连接)
    bool NotifyCreateLinkSync(bool bSuccess);

	void Close();

    void CalcRegisterDigest(std::string& strRegDigest, const std::string& strLinkId, const std::string& strTransID);

    bool CheckRegDigest(const std::string& strRegDigest, const std::string& strLinkId, const std::string& strTransID);
private:
    ACE_Message_Block * CreateMsgBlock(unsigned int uiLength);
    void DestroyMsgBlock(ACE_Message_Block *& pMsgBlock);
    ACE_Message_Block * MakeRespMsgBlock(const char * pszRecvData, unsigned int uiRecvLength);

	int BufPosTurnToBegin();  // 缓存归位
	int ProcessRecvData();
	bool UnZipMsgBlock();       // 处理消息压缩

private:
	std::string       m_strDevName;	  // 设备名称

    std::string       m_strDevID;	  // 设备id

    bool              m_bConnnected;       // 连接成功标志
    NSSP_STAT_FIAG    m_LoginFlag;         // 登录状态标志
	
	char *m_pBuffer;				//接收数据的内存指针	
	unsigned int m_bufferSize;		//接收数据缓冲区的大小	
	unsigned int m_curDataLen;		//当前接收数据的大小(一个消息已接收的字节数)
	unsigned int m_curPos;          //正在接收消息的头部位置

	unsigned int m_uiHeartBeat;	      // 发送心跳的间隔
	unsigned int m_uiTimeout;		  // 超时时间

    time_t       m_HeartbeatTimestamp;// 心跳发送时间戳  本机发给对端
    time_t       m_RecvHeartbeatTimestamp;// 心跳接收时间戳时间戳  对端发给本机
	
	ACE_Message_Block * m_pMsgBlock;   // 收到消息保存在这里
	unsigned int m_uiWaitForRecvLen;     // 保存当前消息总长度

};

NAMESPACE_BP_END   // namespace end

#endif // NSSP_H

