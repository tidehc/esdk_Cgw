/********************************************************************
filename    :    ivs_uncopy.hpp
author      :    yWX150774
created     :    2012/11/05
description :    定义不能拷贝的基类CUnCopy
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/05 初始版本
*********************************************************************/

#ifndef CBB_BASE_UNCOPY_H
#define CBB_BASE_UNCOPY_H


#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

/*********************************************
 * 定义不能拷贝的类CUnCopy
 * 只要继承自它的类不实现拷贝函数(拷贝构造、拷贝赋值)
 * 也会是不能拷贝的类
*********************************************/
class EXPORT_DLL CUnCopy
{
protected:
    CUnCopy()
    {

    }

    ~CUnCopy()
    {

    }

private:
    CUnCopy(const CUnCopy &);
    CUnCopy & operator = (const CUnCopy &);
};

NAMESPACE_CBB_END


#endif // CBB_BASE_UNCOPY_H
