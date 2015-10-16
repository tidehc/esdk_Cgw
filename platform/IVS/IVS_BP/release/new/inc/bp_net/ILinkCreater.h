/********************************************************************
filename    :    LinkCreater.h
author      :    j00213308
created     :    2012/04/06
description :    LinkCreater接口头文件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/04/06 初始版本
*********************************************************************/
#ifndef LINK_CREATER_H
#define LINK_CREATER_H

#include "bp_def.h"
#include "bp_namespace.h"

#include <string>
#include <list>
#include <map>

NAMESPACE_BP_BEGIN		// IVS NAMESPACE

// 前置声明
class ILink;
class IMsgHandle;
class IProtocol;
class ILinkCreater;

// LinkCreater接口
class EXPORT_DLL ILinkCreater
{
public:
	ILinkCreater(void);
	virtual ~ILinkCreater(void);

    /**************************************************************************
    * name       : CreateLink
    * description: 生成ILink对象
    * input      : NA
    * output     : NA
    * return     : 非NULL - 成功，NULL - 失败
    * remark     : 生成ILink完整对象，包含其协议栈对象
    **************************************************************************/
	ILink* CreateLink(void);

    /**************************************************************************
    * name       : SetLinkProperty
    * description: 设置LinkCreater的属性
    * input      : iLinkType    : 连接类型，  0 - UDP
	*										  1 - TCP
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void SetLinkProperty(int iLinkType);
	
	/**************************************************************************
	* name       : SetTimeout
	* description: 设置超时时间
	* input      : uiTimeout：超时时间
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
	void SetTimeout(unsigned int uiTimeout);

	/**************************************************************************
	* name       : SetHeartBeat
	* description: 设置心跳间隔
	* input      : uiHeartBeat：心跳间隔
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
	void SetHeartBeat(unsigned int uiHeartBeat);

	/**************************************************************************
	* name       : SetSecuritySocket
	* description: 设置是否需创建安全套接字
	* input      : bSecuritySocket：是否需创建安全套接字
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
	void SetSecuritySocket(bool bSecuritySocket);

	/**************************************************************************
	* name       : GetSecuritySocket
	* description: 获知是否需创建安全套接字
	* input      : NA
	* output     : NA
	* return     : bool：是否需创建安全套接字
	* remark     : NA
	**************************************************************************/
	bool GetSecuritySocket(void) const;

	/**************************************************************************
	* name       : SetPriPemFileKey
	* description: 设置SSL服务端的私钥文件的key(加密过的)
	* input      : strPriPemFileKey：SSL服务端的私钥文件的key(加密过的)
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
	void SetPriPemFileKey(const std::string & strPriPemFileKey);

	/**************************************************************************
	* name       : GetPriPemFileKey
	* description: 获知SSL服务端的私钥文件的key(加密过的)
	* input      : NA
	* output     : NA
	* return     : const std::string &：SSL服务端的私钥文件的key(加密过的)
	* remark     : NA
	**************************************************************************/
	const std::string & GetPriPemFileKey(void) const;

    /**************************************************************************
	* name       : SetAuthType
	* description: 设置鉴权类型
	* input      : NA
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
    void SetAuthType(ProtocolAuthType enAuthType);

    /**************************************************************************
	* name       : GetAuthType
	* description: 获取鉴权类型
	* input      : NA
	* output     : NA
	* return     : ProtocolAuthType
	* remark     : NA
	**************************************************************************/
    ProtocolAuthType GetAuthType() const;

    /**************************************************************************
	* name       : SetAuthMessageList
	* description: 设置鉴权消息列表
	* input      : NA
	* output     : NA
	* return     : ProtocolAuthType
	* remark     : NA
	**************************************************************************/
    void SetAuthMessageList(const std::list<uint32_t>& authMessageList);

    /**************************************************************************
	* name       : GetAuthMessageList
	* description: 获取鉴权消息列表
	* input      : NA
	* output     : NA
	* return     : const std::list<uint32_t>& 鉴权消息列表
	* remark     : NA
	**************************************************************************/
    const std::list<uint32_t>& GetAuthMessageList();

    /**************************************************************************
	* name       : SetAuthKey
	* description: 接入的key
	* input      : const std::string& strAuthKey
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
    void SetAuthKey(const std::string& strAuthKey);

    /**************************************************************************
	* name       : GetAuthKey
	* description: 接入的key
	* input      : NA
	* output     : NA
	* return     : const std::string& 
	* remark     : NA
	**************************************************************************/
    const std::string& GetAuthKey() const;
protected:
    /**************************************************************************
    * name       : BuildMsgHandle
    * description: 创建消息处理者
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : 特定link creater各自实现,获取对应的消息处理者
    **************************************************************************/
	virtual IMsgHandle* BuildMsgHandle(void) = 0;

    /**************************************************************************
    * name       : BuildProtocol
    * description: 创建协议栈
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	virtual IProtocol* BuildProtocol(void) = 0;


private:
    /**************************************************************************
    * name       : BuildLink
    * description: 依据连接类型,创建连接
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	ILink* BuildLink(void);

protected:
    int          m_iLinkType;         // 连接类型

    unsigned int m_uiTimeout;         // 超时时间
    unsigned int m_uiHeartBeat;       // 心跳间隔

    bool         m_bSecuritySocket;   // 是否需要使用SSL协议传输数据(只支持TCP)
    std::string  m_strPriPemFileKey;  // SSL服务端的私钥文件的key(加密过的)

    ProtocolAuthType m_enAuthType;
    std::string  m_strAuthKey;
    std::list<uint32_t> m_authMessageList;
};

NAMESPACE_BP_END

#endif // LINK_CREATER_H
