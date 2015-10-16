/********************************************************************
filename    :    HandleSignal.h
author      :    s00191067
created     :    2012/04/25
description :    信号管理类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/04/25 初始版本
*********************************************************************/
#ifndef HANDLE_SIGNAL_H
#define HANDLE_SIGNAL_H

#include "IService.h"
#include "Thread.h"
#include "bp_namespace.h"

#ifdef WIN32
#include <stdio.h>
#else
#include <signal.h>
#include <stdio.h>
#endif

NAMESPACE_BP_BEGIN // namespace begin

class EXPORT_DLL CHandleSignal
{
public:
    CHandleSignal(void);
    ~CHandleSignal(void);

    static void Init();

#ifdef WIN32
    
#else
    static void sig_usr(int signo);
#endif

};

NAMESPACE_BP_END   // namespace end
#endif //HANDLE_SIGNAL_H
