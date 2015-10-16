/********************************************************************
filename    :    CommuService.h
author      :    j00213308
created     :    2012/04/07
description :    网元通讯服务类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/04/07 初始版本
*********************************************************************/
#ifndef COMMU_SERVICE_H
#define COMMU_SERVICE_H

#include "IService.h"
#include "net_element.h"

NAMESPACE_BP_BEGIN

class CCmd;
class CNetElementMgr;

// 网元通讯服务类
class EXPORT_DLL CCommuService : public IService
{
public:
	CCommuService(void);
	virtual ~CCommuService(void);
    
	/**************************************************************************
    * name       : HandleCmd
    * description: 处理命令消息
    * input      : pCmd : 需处理的命令消息
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	virtual int HandleCmd(CCmd* pCmd);
	    
	/**************************************************************************
    * name       : Routing
    * description: 列程处理
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	virtual uint32_t Routing(void);

	/**************************************************************************
    * name       : SetElementMgr
    * description: 设置网元管理者
    * input      : pElementMgr:网元管理者
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void SetElementMgr(CNetElementMgr* pElementMgr);

	/**************************************************************************
    * name       : GetElement
    * description: 获取网元
    * input      : iElementType:网元类型
    * output     : NA
    * return     : INetElement*
    * remark     : NA
    **************************************************************************/
	INetElement* GetElement(int iElementType);

    /**************************************************************************
    * name       : AddNetElement()
    * description: 添加网元
    * input      : pNetElement --- 网元对象
    * output     : NA
    * return     : 成功返回0，失败返回错误码
    * remark     : NA
    **************************************************************************/
    int AddNetElement(INetElement *pNetElement);
private:
	CNetElementMgr* m_pElementMgr;
};

NAMESPACE_BP_END

#endif	// end of COMMU_SERVICE_H
