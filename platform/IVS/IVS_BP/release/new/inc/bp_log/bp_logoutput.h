/********************************************************************
filename    :    bp_logoutput.h
author      :    cwx148380
created     :    2012/11/22
description :    日志信息缓存类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/22 初始版本
*********************************************************************/

#ifndef BP_LOG_OUTPUT_H
#define BP_LOG_OUTPUT_H

#ifdef WIN32
# ifdef BP_LOG_EXPORTS
#  define EXPORT_BP_LOG_API __declspec(dllexport)
# else
#  define EXPORT_BP_LOG_API __declspec(dllimport)
# endif
#else
#define EXPORT_BP_LOG_API
#endif

#define LOG_OUTPUT_FAIL (-1)
//lint -e526 -e628
class EXPORT_BP_LOG_API ILogOutput //lint !e129
{//lint !e129 !e10
public:
    ILogOutput() {};
    virtual ~ILogOutput() {};

   /**********************************************************************
   * name           : OperationOutput
   * description    : 操作日志输出缓存
   * input          : @pUser:           用户
                      @pIp:             IP地址
                      @iLevel:          日志等级
                      @pServiceCode:    业务代码
                      @pModuleType:     模块类型
                      @pNodeCode:       机器码
                      @pOpcode:         操作码
                      @pOperation:      操作信息
                      @pObject:         操作对象
                      @pOccurTime:      日志发生时间
                      @iUTCOffset:      UTC时间偏移量
                      @pResult:         结果
                      @pErrCode:        错误码
                      @pAdditionalInfo: 附加信息
   * return         : 成功：返回当前操作日志缓存数
                      失败：返回LOG_OUTPUT_FAIL
   * remark         : NA
   *********************************************************************/
    virtual int OperationOutput ( const char *pUser, const char* pIp,
                                  const int iLevel,
                                  const char *pServiceCode, const char *pModuleType, const char* pNodeCode,
                                  const char *pOpcode, /*const char *pOperation,*/ const char *pObject,
                                  const char *pOccurTime, const long iUTCOffset,
                                  const char *pResult, const char *pErrCode, const char *pAdditionalInfo
                                ) = 0;

   /**********************************************************************
   * name           : SecurityOutput
   * description    : 安全日志输出缓存
   * input          : @pUser:           用户
                      @pIp:             IP地址
                      @iLevel:          日志等级
                      @pServiceCode:    业务代码
                      @pModuleType:     模块类型
                      @pNodeCode:       机器码
                      @pOpcode:         操作码
                      @pOperation:      操作信息
                      @pOccurTime:      日志发生时间
                      @iUTCOffset:      UTC时间偏移量
                      @pResult:         结果
                      @pErrCode:        错误码
                      @pAdditionalInfo: 附加信息
   * return         : 成功: 返回当前操作日志缓存数
                      失败：返回LOG_OUTPUT_FAIL
   * remark         : NA
   *********************************************************************/
    virtual int SecurityOutput ( const char *pUser, const char *pIp,
                                 const int iLevel,
                                 const char* pServiceCode,
                                 const char* pModuleType, const char* pNodeCode,
                                 const char *pOpcode, /* const char* pOperation,*/
                                 const char* pOccurTime, const long iUTCOffset,
                                 const char* pResult, const char* pErrCode, const char *pAdditionalInfo
                               ) = 0;
};
//lint +e526 +e628
#endif //bp_logoutput.h
