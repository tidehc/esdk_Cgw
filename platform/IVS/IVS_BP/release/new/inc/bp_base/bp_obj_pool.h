/********************************************************************
filename    :    bp_obj_pool.h
author      :    j00213308
created     :    2012/05/07
description :    对象池
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/05/07 初始版本
*********************************************************************/
#ifndef _BP_OBJ_POOL_H
#define _BP_OBJ_POOL_H

#include <map>
#include <list>
#include "ace_header.h"
#include "bp_namespace.h"
#include "IObject.h"
#include "bp_def.h"

NAMESPACE_BP_BEGIN

typedef std::list<IObject*>               CListObj;
typedef std::list<IObject*>::iterator     CListObjIter;
typedef std::map<int, CListObj>           CMapObj;
typedef std::map<int, CListObj>::iterator CMapObjIter;
typedef ACE_Recursive_Thread_Mutex         MutexThread;

// 命令对象池
class EXPORT_DLL CObjPool
{
public:
    /**************************************************************************
    * name       : GetInstance
    * description: 获取对象池的单例
    * input      : NA
    * return     : CObjPool* 实例 NULL 创建单例失败
    * remark     : 静态函数，可直接调用
    **************************************************************************/
    static CObjPool* GetInstance(void);
    ~CObjPool(void);

    /**************************************************************************
    * name       : AddObjectTemplate
    * description: 向对象池注册对象类型
    * input      : IObject* 具体某类型的实例
    * return     : OK 成功 FAIL 失败
    * remark     : 先创建某种类型的实例，加入池作为模板，后续分配以第一个实例为原型
	* remark     : 加入池中作为模板的实例，可以不用手动析构，池会负责释放
    **************************************************************************/
    int AddObjectTemplate(IObject*);

    /**************************************************************************
    * name       : GetObject
    * description: 从对象池中，根据对象类型，获取一个对应的实例
    * input      : iType: 对象类型值
    * return     : IObject* 具体对象指针
    * remark     : 对象类型从IObject的GetObjectType()方法获得
    **************************************************************************/
    IObject* GetObject(int iType);

    /**************************************************************************
    * name       : ReleaseObject
    * description: 释放对象实例，放回池
    * input      : IObject* 待释放的对象指针
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void ReleaseObject(IObject*);

private:
    CObjPool(void);

private:
    static CObjPool*    m_pInstance;        // 池指针
    static MutexThread  m_mutex;            // 池锁

    CMapObj             m_freeObjMap;       // 可用的命令对象
};

NAMESPACE_BP_END

#endif //_BP_OBJ_POOL_H
