/********************************************************************
filename    :    bp_log_conf.h
author      :    w90004666
created     :    2012/9/18
description :	 日志模块接口文件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/18 初始版本
*********************************************************************/
#ifndef BP_LOG_CONF_H
#define BP_LOG_CONF_H

#include "bp_log.h"
#include "bp_def.h"

#include <limits.h>


extern "C" bool EXPORT_DLL read_log_config(const char * strConfigFile, struct BP_LOG_CONF & log_conf);

extern "C" bool EXPORT_DLL read_log_sftp_conf(struct BP_LOG_SFTP_INFO & sftp_info);
extern "C" bool EXPORT_DLL write_log_sftp_conf(const struct BP_LOG_SFTP_INFO & sftp_info);


#endif // BP_LOG_CONF_H

