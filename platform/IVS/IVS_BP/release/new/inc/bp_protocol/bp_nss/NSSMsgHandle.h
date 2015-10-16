/********************************************************************
filename    :    NSSMsgHandle.h
author      :    w90004666
created     :    2012/9/20
description :	 NSS协议回调处理类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/20 初始版本
*********************************************************************/
#ifndef NSS_MSG_HANDLE_H
#define NSS_MSG_HANDLE_H

#include "bp_namespace.h"
#include "IMsgHandle.h"

NAMESPACE_BP_BEGIN

using namespace std;

class CNSSMsgHandle: public IMsgHandle
{
private:
    CNSSMsgHandle(const CNSSMsgHandle& obj);
    CNSSMsgHandle& operator=(const CNSSMsgHandle& obj);
public:
    CNSSMsgHandle(void);
    virtual ~CNSSMsgHandle(void);
    
    // 处理数据包
	virtual void HandlePkg(const string& strTransId, int netElementType, const std::string& strLinkId, const char* pPkgData, unsigned int uiDataLen);
};

NAMESPACE_BP_END
#endif //NSS_MSG_HANDLE_H

