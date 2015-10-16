/********************************************************************
filename    :    ivs_namespace.h
author      :    yWX150774
created     :    2012/12/20
description :    CBB模块名字空间
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/12/20 初始版本
*********************************************************************/

#ifndef CBB_BASE_NAMESPACE_H
#define CBB_BASE_NAMESPACE_H


#include <cstdio>

#define NAMESPACE_CBB_BEGIN \
namespace CBB \
{

#define NAMESPACE_CBB_END   \
}

#define USING_NAMESPACE_CBB \
using namespace CBB;

#ifdef _MSC_VER
    #define EXPORT_DLL __declspec(dllexport)
#else
    #define EXPORT_DLL
#endif


#endif // CBB_BASE_NAMESPACE_H
