/********************************************************************
filename	: 	bp_namespace.h
author		:	s00191067
created		:	2012/03/27
description	:	提供ivs 命名空间宏定义
copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history		:	2012/03/27 初始版本
*********************************************************************/
#ifndef BP_NAMESPACE_H
#define BP_NAMESPACE_H

namespace BP 
{//lint !e129 !e10
    // 命名空间声明开始
    #define NAMESPACE_BP_BEGIN  \
    namespace BP{               \

    // 命名空间声明结束
    #define NAMESPACE_BP_END    \
    }                           \

    // 包含命名空间
    #define USING_NAMESPACE_BP using namespace BP;

}
#endif //BP_NAMESPACE_H
