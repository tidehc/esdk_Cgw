/********************************************************************
filename    :    bp_mutex.h
author      :    s90003596
created     :    2012.9.18
description :    互斥锁类，只是将ACE锁系列类定义别名。
                 IVS应用程序使用不直接使用这些ACE的函数或者类，而是使用其别名，
                 以便在需要替换ACE的场景下，可以干净利索地替换ACE。
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012.9.18 初始版本
*********************************************************************/


#ifndef _bp_mutex_h
#define _bp_mutex_h

#include <ace/Global_Macros.h>
#include <ace/Guard_T.h>
#include <ace/Null_Mutex.h>
#include <ace/Thread_Mutex.h>
#include <ace/Recursive_Thread_Mutex.h>

typedef ACE_Thread_Mutex BP_Thread_Mutex;
typedef ACE_Recursive_Thread_Mutex BP_Recursive_Thread_Mutex;
typedef ACE_Null_Mutex BP_Null_Mutex;
typedef ACE_RW_Thread_Mutex BP_RW_Thread_Mutex;

#define BP_GUARD_RETURN ACE_GUARD_RETURN
#define BP_GUARD ACE_GUARD

#endif //_bp_mutex_h

