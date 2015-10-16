/********************************************************************
filename    :    bp_os.h
author      :    s90003596
created     :    2012.9.18
description :    操作系统系统调用或者API适配层, 只是将ACE_OS定义一个别名BP_OS,
                 或者将ACE的类定义一个别名。
                 IVS应用程序使用不直接使用这些ACE的函数或者类，而是使用其别名，
                 以便在需要替换ACE的场景下，可以干净利索地替换ACE。
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012.9.18 初始版本
*********************************************************************/


#ifndef _bp_os_h
#define _bp_os_h

#include "ace_header.h"

#define BP_OS ACE_OS


#endif //_bp_os_h

