/********************************************************************
filename    :    Cmd.h
author      :    s00191067
created     :    2012/03/27
description :    命令封装类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/27 初始版本
*********************************************************************/
#ifndef BP_CMD_H
#define BP_CMD_H

#include <string>
#include "ace_header.h"
#include "bp_def.h"
#include "bp_stdint.h"

NAMESPACE_BP_BEGIN // namespace begin

#pragma  pack(push)
#pragma  pack(1)
typedef struct stNetElementInfo
{
	stNetElementInfo(void)
	{
		HW_MEMSET(PeerIP, 0, sizeof(PeerIP));
		HW_MEMSET(LocalIP, 0, sizeof(LocalIP));
		PeerPort  = 0;
		LocalPort = 0;
		NeedSwitchId = true;
        uiTimeout = 0;
        bAsyncCreateLink = true;
	}

	char   PeerIP[IP_LENGTH];			// 远端服务器IP 
	char   LocalIP[IP_LENGTH];			// 本地IP
	unsigned short PeerPort;    // 远端服务器PORT
	unsigned short LocalPort;   // 本地PORT
	bool NeedSwitchId;			// 是否用对端ID更新本端所持有的连接ID
    uint32_t uiTimeout;         // 创建连接超时时间，单位为秒
    bool   bAsyncCreateLink;        // 是否异步方式创建连接，默认为true，若为false，则表示采用同步方式创建连接
} NET_ELEMENT_CREATE_INFO;

#pragma pack(pop)

class CCmd
{
public:
    CCmd(void)
		: m_bCompress(false)
        , m_uiReqID(0)
        , m_threadID(0)
        , m_uiServiceID(0)
        , m_enCmdType(CMD_UNDEF)
        , m_enNetElemType(NET_ELE_SCU_NONE)
        , m_strNetLinkID("")
        , m_strTransID("")
        , m_pMsgBlock(NULL)
    {
    }

    ~CCmd(void)
    {
        // 析构掉消息缓冲区 TODO：确认是否这样实现
        ReleaseMsgBlock();
    }

	void Compress(bool bCompress)
	{
		this->m_bCompress = bCompress;
	}

	bool Compress()
	{
		return this->m_bCompress;
	}

    uint32_t GetReqID() const{ return m_uiReqID; }

    void SetReqID( uint32_t uiReqID ){ m_uiReqID = uiReqID; }

    uint32_t GetServiceID() const{ return m_uiServiceID; }

    void SetServiceID( uint32_t uiServiceID ){ m_uiServiceID = uiServiceID; }

    ACE_thread_t GetThreadID() const{ return m_threadID; }

    void SetThreadID( const ACE_thread_t& threadID ){ m_threadID = threadID; }

    CMD_TYPE GetCmdType() const{ return m_enCmdType; }

    void SetCmdType( CMD_TYPE enCmdType ){ m_enCmdType = enCmdType; }
    
    int GetNetElemType() const{ return m_enNetElemType; }

    void SetNetElemType( int enNetElemType ){ m_enNetElemType = enNetElemType; }

    const std::string& GetNetLinkID() const{ return m_strNetLinkID; }

    void SetNetLinkID(const std::string& strNetLinkID ){ m_strNetLinkID = strNetLinkID; }

    const std::string& GetTransID() const { return m_strTransID; }

    void SetTransID( const std::string& strTransID ) { m_strTransID = strTransID; }

    ACE_Message_Block* GetMsgBlock() { return m_pMsgBlock; }

    void SetMsgBlock(ACE_Message_Block* pMsgBlock) { m_pMsgBlock = pMsgBlock; }

	//重新克隆一份出来
	CCmd* NewClone()
	{
		CCmd* pCmdClone = NULL;
		HW_NEW(pCmdClone, CCmd);
		if(NULL == pCmdClone)
		{
			return NULL;
		}

        pCmdClone->m_uiReqID = m_uiReqID;
        pCmdClone->m_threadID = m_threadID;
        pCmdClone->m_uiServiceID = m_uiServiceID;
        pCmdClone->m_enCmdType = m_enCmdType;
        pCmdClone->m_enNetElemType = m_enNetElemType;
        pCmdClone->m_strNetLinkID = m_strNetLinkID;
        pCmdClone->m_strTransID = m_strTransID;

        if (NULL != m_pMsgBlock)
        {
			ACE_Message_Block* pMsgBlock = NULL;
			HW_NEW(pMsgBlock, ACE_Message_Block(m_pMsgBlock->total_size()));
			if(NULL == pMsgBlock)
			{
				HW_DELETE(pCmdClone);
				return NULL;
			}
			pMsgBlock->copy(m_pMsgBlock->base(), m_pMsgBlock->total_size());
			pCmdClone->m_pMsgBlock = pMsgBlock;
		}

		return pCmdClone;
	}

    void CopyMsgBlockData(const char* pszMsgBlockData,unsigned int uiMsgLength)
    {
        CHECK_POINTER_VOID(pszMsgBlockData);

        // 首先判断原来是否有数据
        ReleaseMsgBlock();
        HW_NEW(m_pMsgBlock, ACE_Message_Block(uiMsgLength));
        CHECK_POINTER_VOID(m_pMsgBlock);
        m_pMsgBlock->copy(pszMsgBlockData, uiMsgLength);
    }

	bool SaveData(const char* pData, unsigned int uiDataLen)
	{
		bool bRet = false;

		do 
		{
			if ((NULL == m_pMsgBlock) || (NULL == pData) || (0 == uiDataLen) || (m_pMsgBlock->capacity() < uiDataLen))
			{
				break;
			}

			m_pMsgBlock->copy(pData, uiDataLen);
			bRet = true;

		} while ( false );

		return bRet;
	}

    static CCmd* NewInstance()
	{
		CCmd* cmd = NULL;
		HW_NEW(cmd, CCmd);
		return cmd;
	}

    /**************************************************************************
    * name       : PopMsgBlock
    * description: 弹出MsgBlock
    * input      : NA
    * output     : NA
    * return     : 返回MsgBlock指针，后续由调用方管理该内存
    * remark     : NA
    **************************************************************************/
    ACE_Message_Block* PopMsgBlock()
    {
        ACE_Message_Block* pMsgBlock = m_pMsgBlock;
        m_pMsgBlock = NULL;
        return pMsgBlock;
    }

private:
    CCmd(const CCmd& cmd)
        : m_uiReqID(cmd.GetReqID())
        , m_threadID(cmd.GetThreadID())
        , m_uiServiceID(cmd.GetServiceID())
        , m_enCmdType(cmd.GetCmdType())
        , m_enNetElemType(cmd.GetNetElemType())
        , m_strNetLinkID(cmd.GetNetLinkID())
        , m_strTransID(cmd.GetTransID())
    {
        CCmd* pCmd = const_cast<CCmd*>(&cmd);
        if (NULL != pCmd)
        {
            CopyMsgBlockData(pCmd->GetMsgBlock()->rd_ptr(), pCmd->GetMsgBlock()->length());
        } 
    }

    CCmd& operator = (const CCmd& cmd)
    {
        this->m_uiReqID = cmd.GetReqID();
        this->m_threadID = cmd.GetThreadID();
        this->m_uiServiceID = cmd.GetServiceID();
        this->m_enCmdType = cmd.GetCmdType();
        this->m_enNetElemType = cmd.GetNetElemType();
        this->m_strNetLinkID = cmd.GetNetLinkID();
        this->m_strTransID = cmd.GetTransID();

        if (NULL != m_pMsgBlock)
        {
            CCmd* pCmd = const_cast<CCmd*>(&cmd);
            if (NULL != pCmd)
            {
                CopyMsgBlockData(pCmd->GetMsgBlock()->rd_ptr(), pCmd->GetMsgBlock()->length());
            }      
        }
        return *this;
    }
    /**************************************************************************
    * name       : ReleaseMsgBlock
    * description: 释放数据块
    * input      : 
    * output     : NA
    * return     : 
    * remark     : NA
    **************************************************************************/
    void ReleaseMsgBlock()
    {
        if (NULL != this->m_pMsgBlock)
        {
            if (m_pMsgBlock->reference_count() > 1)
            {
                m_pMsgBlock->release();
                //return;
            }

            // 没有人在使用该消息块了
            m_pMsgBlock->reset();
            HW_DELETE(m_pMsgBlock);
        }
    }

private:
	bool m_bCompress;   //true: 压缩消息, false: 不压缩消息
    uint32_t                m_uiReqID;          // 请求类型
	ACE_thread_t            m_threadID;         // 处理任务所在的线程ID
    uint32_t                m_uiServiceID;      // 任务类型ID（线程路由必填）
    CMD_TYPE                m_enCmdType;        // 命令类型(网元通信必填)
    int           m_enNetElemType;    // 向外发送时指定网元类型(网元创建必填)
    std::string             m_strNetLinkID;     // 向外发送时指定网元连接ID(网元通信必填)
    std::string             m_strTransID;       // 事务ID
    ACE_Message_Block*      m_pMsgBlock;        // 消息缓冲区  
};

NAMESPACE_BP_END   // namespace end
#endif //BP_CMD_H
