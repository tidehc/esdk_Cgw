/********************************************************************
filename    :    MKTransID.h
author      :    h00211123
created     :    2012/05/09
description :    
copyright   :    Copyright HUAWEI (C) 2011-2015
history     :    2012/05/09 初始版本
*********************************************************************/
#ifndef MK_TRANSID_H
#define MK_TRANSID_H

#include <string>
#include "ace_header.h"
#include "bp_namespace.h"
#include "bp_def.h"


NAMESPACE_BP_BEGIN

typedef ACE_Recursive_Thread_Mutex	MutexThread;

class EXPORT_DLL CMKTransID
{
public:
    virtual ~CMKTransID(void){};

    static CMKTransID& Instance();

	/**************************************************************************
    * name       : SetTransParam
    * description: 设置transid的参数
				   注意：在调用GenerateTransID之前一定要调用本接口进行设置，以区分各个模块
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : 用于各个模块生成事物id
    **************************************************************************/
    void SetTransParam(const std::string& strModName, const std::string& strDevId);

	/**************************************************************************
    * name       : GenerateTransID
    * description: 生成一个新的transid
    * input      : NA
    * output     : NA
    * return     : 新的transid
    * remark     : 用于各个模块生成事物id
    **************************************************************************/
	std::string GenerateTransID();

private:
    void FixString(std::string& str, unsigned int uiLen, char c);

private:
    CMKTransID(void);
    MutexThread  m_mutex;

	unsigned long m_ulTransIDNumFeed;
    std::string   m_strModuleName;
    std::string   m_strDevId;

};

NAMESPACE_BP_END   // namespace end
#endif //MK_TRANSID_H