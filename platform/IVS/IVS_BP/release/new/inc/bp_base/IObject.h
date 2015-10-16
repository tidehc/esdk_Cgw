/********************************************************************
filename    :    IObject.h 
author      :    b00209723
created     :    2012/04/12
description :    媒体会话接口
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd 
history     :    2012/04/12 初始版本
*********************************************************************/
#ifndef  I_BP_OBJECT_H
#define  I_BP_OBJECT_H

class IObject
{
public:
    virtual ~IObject(){};

    /**********************************************************************
    * name           : GetObjectType
    * description    : 获取类型值
    * input          : NA
    * output         : NA
    * return         : 对象类型值
    * remark         : 
    *********************************************************************/
    int GetObjectType() const {return m_iType;}

    /**********************************************************************
    * name           : NewSelf
    * description    : 复制一个该对象的实例
    * input          : NA
    * output         : NA
    * return         : IObject* 对象实例
    * remark         : 纯虚函数，各类型实现如何创建该对象的一个复制品
    *********************************************************************/
    virtual IObject* NewSelf() = 0;

    /**********************************************************************
    * name           : Reset
    * description    : 重置对象状态
    * input          : NA
    * output         : NA
    * return         : NA
    * remark         : 纯虚函数，各子类实现具体功能
    *********************************************************************/
    virtual void Reset() = 0;

protected:
    int     m_iType;         // 存储对象类型值
};

#endif //I_BP_OBJECT_H
