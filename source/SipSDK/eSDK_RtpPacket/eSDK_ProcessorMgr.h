
#include <map>
#include "Ps2EsProcessor.h"

class CProcessorMgr
{
private:
	// 单实例，构造函数设为私有;
	CProcessorMgr(void); 


public:
	   /******************************************************************
     function   : instance
     description: 单实例
     output     : NA
     return     : CProcessorMgr& 单实例对象;
    *******************************************************************/
	static CProcessorMgr& instance()
	{
		static CProcessorMgr ProcessorMgr;
		return ProcessorMgr;
	}

	    /******************************************************************
     function   : ~CProcessorMgr
     description: 析构函数;
     input      : void
     output     : NA
     return     : NA
    *******************************************************************/
    ~CProcessorMgr(void);

	  /******************************************************************
     function   : Init
     description: 初始化
     input      : void
     output     : NA
     return     : void
    *******************************************************************/
    void Init(void);

	  /******************************************************************
     function   : UnInit
     description: 清空;
     input      : void
     output     : NA
     return     : void
    *******************************************************************/
    void UnInit(void);

	  /******************************************************************
     function   : GetProcessor
     description: 获取处理类
     input      : NA
     output     : unsigned long & ulServiceId
     return     : int 成功:IVS_SUCCEED 失败:IVS_FAIL;
    *******************************************************************/
    int GetProcessor(unsigned long & ulServiceId);

	  /******************************************************************
     function   : FreeProcessor
     description: 释放指定处理类
     input      : unsigned long ulServiceId
     output     : NA
     return     : int 成功:IVS_SUCCEED 失败:IVS_FAIL;
    *******************************************************************/
    int FreeProcessor(unsigned long ulServiceId);


private:
	  
	//ulServiceId与转换处理类;
	typedef std::map<unsigned long, CPs2EsProcessor*> ServiceProcessorMap;
	typedef ServiceProcessorMap::iterator ServiceProcessorMapIter;
	ServiceProcessorMap m_HanleRealPlayMap;
}