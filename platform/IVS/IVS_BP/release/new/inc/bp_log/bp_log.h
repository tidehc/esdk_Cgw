/********************************************************************
filename    :    bp_log.h
author      :    w90004666
created     :    2012/9/18
description :	 日志模块接口文件
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/18 初始版本
*********************************************************************/
#ifndef BP_LOG_H
#define BP_LOG_H

#define LOG_HAS_ITF
#define LOG_HAS_OPR
#define LOG_HAS_SEC

#include "ivs_error.h"
#include "ivs_log.h"
#include "bp_mem.h"

// 运行日志：Emergency(0) 级别日志；Critical(2) 级别日志；Error(3) 级别日志；Warning(4) 级别日志；Informational(6) 级别日志
// PS: EMR,CRT,ERR级别必须要求传入错误码
#define BP_RUN_LOG_EMR(errCode, operation, fmt, ...)                                                                 \
    {                                                                                                                \
        char strMacroLogBuff[LOG_ADDITIONALINFO_LENGTH] = {0};                                                       \
        int iMacroLogPos = IVS_LogFormatErrCode(strMacroLogBuff, LOG_ADDITIONALINFO_LENGTH, errCode, fmt);           \
        strMacroLogBuff[iMacroLogPos] = '\0';                                                                        \
        (void)IVS_LogRun(IVSLOG_EMERG, operation, __FILE__, __LINE__, __FUNCTION__, strMacroLogBuff, ##__VA_ARGS__); \
    }
#define BP_RUN_LOG_CRT(errCode, operation, fmt, ...)                                                                 \
    {                                                                                                                \
        char strMacroLogBuff[LOG_ADDITIONALINFO_LENGTH] = {0};                                                       \
        int iMacroLogPos = IVS_LogFormatErrCode(strMacroLogBuff, LOG_ADDITIONALINFO_LENGTH,  errCode, fmt);          \
        strMacroLogBuff[iMacroLogPos] = '\0';                                                                        \
        (void)IVS_LogRun(IVSLOG_CRIT, operation, __FILE__, __LINE__, __FUNCTION__, strMacroLogBuff, ##__VA_ARGS__);  \
    }
#define BP_RUN_LOG_ERR(errCode, operation, fmt, ...)                                                                 \
    {                                                                                                                \
        char strMacroLogBuff[LOG_ADDITIONALINFO_LENGTH] = {0};                                                       \
        int iMacroLogPos = IVS_LogFormatErrCode(strMacroLogBuff, LOG_ADDITIONALINFO_LENGTH, errCode, fmt);           \
        strMacroLogBuff[iMacroLogPos] = '\0';                                                                        \
        (void)IVS_LogRun(IVSLOG_ERR, operation, __FILE__, __LINE__, __FUNCTION__, strMacroLogBuff, ##__VA_ARGS__);   \
    }
#define BP_RUN_LOG_WAR(operation, fmt, ...) \
    (void)IVS_LogRun(IVSLOG_WARNING, operation, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define BP_RUN_LOG_INF(operation, fmt, ...) \
    (void)IVS_LogRun(IVSLOG_INFO, operation, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);

// 接口日志: Emergency(0) 级别的接口日志；Critical(2) 级别日志；Error(3) 级别的接口日志； Warning(4) 级别的运行日志；Informational(6) 级别的接口日志。
// PS: 接口日志不强制要求传入错误码
#define BP_ITF_LOG_EMR(inftype, infname, sender, receiver, traceMark, operation, result, fmt, ...) \
    (void)IVS_LogInterface(IVSLOG_EMERG, inftype, infname, sender, receiver, traceMark, operation, result, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define BP_ITF_LOG_CRT(inftype, infname, sender, receiver, traceMark, operation, result, fmt, ...) \
    (void)IVS_LogInterface(IVSLOG_CRIT, inftype, infname, sender, receiver, traceMark, operation, result, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define BP_ITF_LOG_ERR(inftype, infname, sender, receiver, traceMark, operation, result, fmt, ...) \
    (void)IVS_LogInterface(IVSLOG_ERR, inftype, infname, sender, receiver, traceMark, operation, result, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define BP_ITF_LOG_WAR(inftype, infname, sender, receiver, traceMark, operation, result, fmt, ...) \
    (void)IVS_LogInterface(IVSLOG_WARNING, inftype, infname, sender, receiver, traceMark, operation, result, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define BP_ITF_LOG_INF(inftype, infname, sender, receiver, traceMark, operation, result, fmt, ...) \
    (void)IVS_LogInterface(IVSLOG_INFO, inftype, infname, sender, receiver, traceMark, operation, result, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);

//操作日志：Error(3) 级别日志；Informational(6) 级别日志
#define BP_OPR_LOG_ERR(errCode, user, ip, object, opcode, operation, result, fmt, ...) \
    (void)IVS_LogOperation(IVSLOG_ERR, user, ip, object, opcode, operation, result, __FILE__, __LINE__, __FUNCTION__, errCode, fmt, ##__VA_ARGS__);
#define BP_OPR_LOG_INF(user, ip, object, opcode, operation, result, fmt, ...) \
    (void)IVS_LogOperation(IVSLOG_INFO, user,  ip, object, opcode, operation, result, __FILE__, __LINE__, __FUNCTION__, (IVS_FAIL), fmt, ##__VA_ARGS__);

// 安全日志：Error(3) 级别日志；Informational(6) 级别日志
#define BP_SEC_LOG_ERR(errCode, user, ip, opcode, operation, result, fmt, ...) \
    (void)IVS_LogSecurity(IVSLOG_ERR, user, ip, opcode, operation, result, __FILE__, __LINE__, __FUNCTION__, errCode, fmt, ##__VA_ARGS__);
#define BP_SEC_LOG_INF(user, ip, opcode, operation, result, fmt, ...) \
    (void)IVS_LogSecurity(IVSLOG_INFO, user, ip, opcode, operation, result, __FILE__, __LINE__, __FUNCTION__, (IVS_FAIL), fmt, ##__VA_ARGS__);

// 调试日志:Debug(7) 不分级别
#define BP_DBG_LOG(fmt, ...) \
    (void)IVS_LogDbg(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

/*
//下列宏定义供BP过渡使用,请业务模块不用使用.

//#define IVS_RUN_LOG_EMR(Operation, OperationResult, fmt, ...) \
//    IVS_LogRun(IVSLOG_EMERG, Operation, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
//#define IVS_RUN_LOG_ERR(Operation, OperationResult, fmt, ...) \
//    IVS_LogRun(IVSLOG_ERR, Operation, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
//#define IVS_RUN_LOG_WAR(Operation, OperationResult, fmt, ...) \
//    IVS_LogRun(IVSLOG_WARNING, Operation, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
//#define IVS_RUN_LOG_INF(Operation, OperationResult, fmt, ...) \
//    IVS_LogRun(IVSLOG_INFO, Operation, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);


//#define IVS_DBG_LOG(fmt, ...) \
//    IVS_LogDbg(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
*/

#endif // BP_LOG_H

