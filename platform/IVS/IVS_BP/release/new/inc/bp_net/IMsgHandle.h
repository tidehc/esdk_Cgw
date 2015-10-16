/********************************************************************
filename    :    IMsgHandle.h
author      :    j00213308
created     :    2012/04/06
description :    协议栈消息处理接口
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/04/06 初始版本
*********************************************************************/
#ifndef IMSG_HANDLE_H
#define IMSG_HANDLE_H

#include "bp_def.h"
#include "ace_header.h"

NAMESPACE_BP_BEGIN

// 前置申明
class CCmd;

// 协议栈消息处理接口
class IMsgHandle
{
public:
    IMsgHandle(void){};
    virtual ~IMsgHandle(void){};

    /**************************************************************************
    * name       : HandlePkg
    * description: 处理数据包
    * input      : strTransId     : 事务ID
    *              netElementType : 接收到数据的网元类型
    *              strLinkId         : 接收到数据连接Id
    *              pPkgData       : 指向数据指针
    *               uiDataLen     : 数据大小
    * output     : NA
    * return     : NA
    * remark     : 消息处理者必须自己分配内存保存数据包
    **************************************************************************/
    virtual void HandlePkg(const string& strTransId, int netElementType, const std::string& strLinkId, const char* pPkgData, unsigned int uiDataLen) = 0;

	virtual bool OnLoginServer(const string& /*strTransId*/, int /*netElementType*/, 
	    const std::string& /*strLinkId*/, const char* /*pPkgData*/, unsigned int /*uiDataLen*/)
	{
		return true;
	}
};

NAMESPACE_BP_END

#endif    // IMSG_HANDLE_H
