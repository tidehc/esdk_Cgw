/********************************************************************
filename    :    bp_dll.h
author      :    w90004666
created     :    2012/9/20
description :	 加载动态库类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/20 初始版本
*********************************************************************/
#ifndef BP_DLL_H
#define BP_DLL_H

#include "ace/DLL.h"

NAMESPACE_BP_BEGIN // namespace begin


class CDllLoad
{
public:
	CDllLoad(const std::string& strName, bool bService);
	virtual ~CDllLoad() throw();

	/**************************************************************************
	 * name       : Load
	 * description: 根据平台，加载动态库
	 * output     : NA
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int Load(void);

	/**************************************************************************
	 * name       : GetFunctionPtr
	 * description: 从动态库中获取函数指针
	 * input      : const std::string & strFunctionName
	 * output     : NA
	 * return     : void*
	 * remark     :               
	 **************************************************************************/
	void* GetFunctionPtr(const std::string& strFunctionName);

private:
	CDllLoad();

	void Init(void);


private:
	bool m_bService;		//是否是Serivce插件库
	std::string m_strName;	//动态库名称，不包括扩展名
	std::string m_strPath;	//动态库全路径，包括扩展名

	ACE_DLL m_DLL;			//动态库对象

	bool m_bOpenFlag;		//动态库是否已加载
};

NAMESPACE_BP_END   // namespace end
#endif //BP_DLL_H
