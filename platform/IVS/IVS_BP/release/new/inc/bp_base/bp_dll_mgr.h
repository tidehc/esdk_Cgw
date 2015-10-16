/********************************************************************
filename    :    bp_dll_mgr.h
author      :    w90004666
created     :    2012/9/20
description :	 加载动态库管理类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/20 初始版本
*********************************************************************/
#ifndef BP_DLL_MGR_H
#define BP_DLL_MGR_H

#include <map>
#include "bp_namespace.h"
#include "bp_dll.h"
#include "bp_def.h"

NAMESPACE_BP_BEGIN // namespace begin


typedef std::map<std::string, CDllLoad*> CDllMap; 

class EXPORT_DLL CDllLoadMgr
{
public:
	virtual ~CDllLoadMgr() throw();

	/**************************************************************************
	 * name       : Instance
	 * description: 单例函数
	 * input      : void
	 * output     : NA
	 * return     : CDllLoadMgr&
	 * remark     :               
	 **************************************************************************/
	static CDllLoadMgr& Instance(void);

	/**************************************************************************
	 * name       : LoadDll
	 * description: 加载动态库，并加入管理
	 * input      : const std::string & strName：动态库名称，不带扩展名
	 * input      : bool bService：是否是Service库
	 * output     : 
	 * return     : int
	 * remark     : 获取到的动态库对象不需要外部释放，由Mgr统一释放              
	 **************************************************************************/
	int LoadDll(const std::string& strName, bool bService = true);

	/**************************************************************************
	 * name       : GetFunctionPtr
	 * description: 获取函数指针
	 * input      : const std::string & strDllName
	 * input      : const std::string & strFunctionName
	 * output     : NA
	 * return     : void*
	 * remark     :               
	 **************************************************************************/
	void* GetFunctionPtr(const std::string& strDllName, const std::string& strFunctionName);

	/**************************************************************************
	 * name       : UnloadAllDlls
	 * description: 卸载所有dll
	 * input      : NA
	 * output     : NA
	 * return     : 
	 * remark     :               
	 **************************************************************************/
	void UnloadAllDlls();

private:
	CDllLoadMgr(void);

	//通过动态库名称获取对应动态库
	CDllLoad* GetDll(const std::string& strName);

private:
	static CDllLoadMgr m_Instance;

	CDllMap m_mapDll;
};

NAMESPACE_BP_END   // namespace end
#endif //BP_DLL_MGR_H
