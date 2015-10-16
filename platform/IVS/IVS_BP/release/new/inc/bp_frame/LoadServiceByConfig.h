/********************************************************************
filename    :    LoadServiceByConfig.h
author      :    z00189721
created     :    2012/09/20
description :    主线程控制模块
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/09/20 初始版本
*********************************************************************/
#ifndef _LOAD_SERVICE_BY_CONFIG_H_
#define _LOAD_SERVICE_BY_CONFIG_H_

#include "bp_config_alarmer.h"
#include "IService.h"
#include "ivs_xml.h"
#include <map>

NAMESPACE_BP_BEGIN // namespace begin

// 创建service对象函数定义
typedef IService* (*CREATE_SERVICE)();

typedef std::map<int, std::string>           CMD_MAP;
typedef std::map<int, std::string>::iterator CMD_MAP_ITER;

class CServiceInfo
{
public:
	CServiceInfo(void);
	virtual ~CServiceInfo(void);

public:
	/**************************************************************************
    * name       : SetServiceName
    * description: 设置服务名
    * input      : strServiceName 服务名
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
	void SetServiceName(const std::string &strServiceName);

	/**************************************************************************
    * name       : SetCommandID
    * description: 设置消息ID和消息名
    * input      : iCommandID      消息ID
	               strCommandName  消息名
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
	void SetCommandID(int iCommandID, const std::string &strCommandName);

	/**************************************************************************
    * name       : SetDllName
    * description: 设置服务库名
    * input      : strDllName  服务库名
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
	void SetDllName(const std::string &strDllName);

	/**************************************************************************
    * name       : SetDllName
    * description: 创建服务对象
    * input      : NA
    * output     : NA
    * return     : 服务对象
    * remark     : NA    
    **************************************************************************/
	IService* CreateBusinessService();

	/**************************************************************************
    * name       : RegCapability
    * description: 注册消息能力
    * input      : uiServiceID 服务ID
	               iThreadID   线程ID
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA    
    **************************************************************************/
	int RegCapability(uint32_t uiServiceID, ACE_thread_t iThreadID);

    void SetResendIntervalOffset(uint32_t uiResendIntervalOffset);

    uint32_t GetResendIntervalOffset() const;

    void SetResendTimes(uint32_t uiResendTimes);

    uint32_t GetResendTimes() const;

private:
	std::string m_strServiceName;  //服务名
	std::string m_strDllName;      //服务库名
	CMD_MAP     m_mapCommand;      //服务支持的消息command表，因为单线程访问该map表，所以不需要加锁保护
    uint32_t    m_uiResendIntervalOffset;
    uint32_t    m_uiResendTimes;
};

typedef std::map<std::string, CServiceInfo*>           SERVICE_MAP;
typedef std::map<std::string, CServiceInfo*>::iterator SERVICE_MAP_ITER;

class CLoadServiceByConfig : private CConfigAlarmer
{
public:
	CLoadServiceByConfig(void);
	virtual ~CLoadServiceByConfig(void);

public:
	/**************************************************************************
    * name       : Init
    * description: 初始化
    * input      : strFileName 配置文件名
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA    
    **************************************************************************/
	int Init(const std::string &strFileName);

	/**************************************************************************
    * name       : CreateAllService
    * description: 创建所有的服务
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA    
    **************************************************************************/
	int CreateAllService();	

	/**************************************************************************
    * name       : Release
    * description: 释放资源
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
	void Release();

    /**************************************************************************
    * name       : GetNextServiceID
    * description: 获取下一个服务ID
    * input      : NA
    * output     : NA
    * return     : 返回服务ID
    * remark     : NA    
    **************************************************************************/
    int GetNextServiceID();
private:
	/**************************************************************************
    * name       : GetAllService
    * description: 获取所有的服务信息
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA    
    **************************************************************************/
	int GetAllService();

	/**************************************************************************
    * name       : GetThreadNum
    * description: 获取线程数
    * input      : NA
    * output     : NA
    * return     : 线程数
    * remark     : NA    
    **************************************************************************/
	int GetThreadNum();

	/**************************************************************************
    * name       : GetThreadName
    * description: 获取线程名称
    * input      : NA
    * output     : NA
    * return     : 线程名称
    * remark     : NA    
    **************************************************************************/
	std::string GetThreadName();

	/**************************************************************************
    * name       : GetThreadConnectDb
    * description: 获取线程是否创建数据库连接
    * input      : NA
    * output     : NA
    * return     : 1 - 需要创建数据库连接， 0 - 不需要创建数据库连接
    * remark     : NA    
    **************************************************************************/
	int GetThreadConnectDb();
	
	/**************************************************************************
    * name       : GetCmdQueueLength
    * description: 获取线程消息队列长度
    * input      : NA
    * output     : NA
    * return     : 消息队列长度
    * remark     : NA    
    **************************************************************************/
	int GetCmdQueueLength();

	/**************************************************************************
    * name       : GetCommand
    * description: 获取command消息
    * input      : pServiceInfo ： 服务信息对象
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA    
    **************************************************************************/
	int GetCommand(CServiceInfo *&pServiceInfo);

	/**************************************************************************
    * name       : CreateBusinessService
    * description: 创建线程和服务
    * input      : iThreadNum ：           线程数
	*              strThreadName：         线程名称
	*              bThreadConnectDatabase  线程是否连接数据库
    * output     : NA
    * return     : IVS_SUCCEED - 成功，IVS_FAIL - 失败
    * remark     : NA    
    **************************************************************************/
	int CreateBusinessService(int iThreadNum, const std::string &strThreadName, bool bThreadConnectDatabase, int iCmdQueueLength);

private:
	CBB::CXml             m_ServiceXML;     // XML对象
	bool                  m_bInit;          // 初始化标记
	int                   m_iServiceID;     // 服务ID
	SERVICE_MAP           m_mapService;     // 服务map表
	unsigned int          m_uiThreadTimeout;// 线程超时时间
};

NAMESPACE_BP_END   // namespace end

#endif //_LOAD_SERVICE_BY_CONFIG_H_
