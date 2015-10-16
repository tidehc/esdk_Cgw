/********************************************************************
 filename    :    bp_memoutput.h
 author      :    cwx148380
 created     :    2012/11/23
 description :    日志信息缓存类
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/23 初始版本
*********************************************************************/
#ifndef BP_MEMOUTPUT_H
#define BP_MEMOUTPUT_H

#include "bp_namespace.h"
#include "bp_stdint.h"
#include "bp_logoutput.h"
#include <string>
#include <list>
#include "bp_mutex.h"


NAMESPACE_BP_BEGIN //lint !e129 !e10

#define MEM_OPER_ACCOUNT_LENGTH          (32)
#define MEM_OPER_IP_LENGTH               (15)
#define MEM_OPER_SERVICECODE_LENGTH      (24)
#define MEM_OPER_MODULE_LENGTH           (24)
#define MEM_OPER_MODULE_TYPE_LENGTH      (24)
#define MEM_OPER_NODE_CODE_LENGTH        (32)
#define MEM_OPER_OPERATION_CODE_LENGTH   (6)
#define MEM_OPER_OBJECT_LENGTH           (1024)
#define MEM_OPER_OCCURTIME_LENGTH        (17)
#define MEM_OPER_ERRCODE_LENGTH          (9)
#define MEM_OPER_RESULT_LENGTH           (10)
#define MEM_OPER_ADDINFO_LENGTH          (1024)


#define MEM_SEC_ACCOUNT_LENGTH           (32)
#define MEM_SEC_IP_LENGTH                (15)
#define MEM_SEC_SERVICECODE_LENGTH       (24)
#define MEM_SEC_MODULE_LENGTH            (24)
#define MEM_SEC_MODULE_TYPE_LENGTH       (24)
#define MEM_SEC_NODE_CODE_LENGTH         (32)
#define MEM_SEC_OPERATION_CODE_LENGTH    (6)
#define MEM_SEC_OBJECT_LENGTH            (1024)
#define MEM_SEC_OCCURTIME_LENGTH         (17)
#define MEM_SEC_ERRCODE_LENGTH           (9)
#define MEM_SEC_RESULT_LENGTH            (10)
#define MEM_SEC_ADDINFO_LENGTH           (1024)

#pragma  pack(push)
#pragma  pack(1)

//操作日志数据结构
typedef struct StOperationLog
{
    char szUser[MEM_OPER_ACCOUNT_LENGTH + 1];                     //用户
    char szIP[MEM_OPER_IP_LENGTH + 1];                            //IP地址
    int32_t iLevel;                                           //日志等级
    char szServiceCode[MEM_OPER_SERVICECODE_LENGTH + 1];          //业务代码，"IVS"
    char szModuleType[MEM_OPER_MODULE_TYPE_LENGTH + 1];           //OMU、SMU、SCU
    char szNodeCode[MEM_OPER_NODE_CODE_LENGTH + 1];
    char szOperationCode[MEM_OPER_OPERATION_CODE_LENGTH + 1];     //操作码
    char szObject[MEM_OPER_OBJECT_LENGTH + 1];                    //操作对象
    char szOccurTime[MEM_OPER_OCCURTIME_LENGTH + 1];              //操作时间
    long iUTCOffset;
    char szErrCode[MEM_OPER_ERRCODE_LENGTH + 1];                  //错误码
    char szResult[MEM_OPER_RESULT_LENGTH + 1];                    //操作结果
    char szAdditionalInfo[MEM_OPER_ADDINFO_LENGTH + 1];           //附加信息
}ST_OPERATION_LOG;

//安全日志数据结构
typedef struct StSecurityLog
{
    char szUser[MEM_SEC_ACCOUNT_LENGTH + 1];                  //用户
    char szIP[MEM_SEC_IP_LENGTH + 1];                         //IP地址
    int32_t iLevel;                                           //日志等级
    char szServiceCode[MEM_SEC_SERVICECODE_LENGTH + 1];       //业务代码，"IVS"
    char szModuleType[MEM_SEC_MODULE_TYPE_LENGTH + 1];        //OMU、SMU、SCU
    char szNodeCode[MEM_SEC_NODE_CODE_LENGTH + 1];
    char szOperationCode[MEM_SEC_OPERATION_CODE_LENGTH + 1];  //操作码
    char szObject[MEM_OPER_OBJECT_LENGTH + 1];               //操作对象
    char szOccurTime[MEM_SEC_OCCURTIME_LENGTH + 1];          //操作时间
    long iUTCOffset;
    char szErrCode[MEM_SEC_ERRCODE_LENGTH + 1];              //错误码
    char szResult[MEM_SEC_RESULT_LENGTH + 1];                //操作结果
    char szAdditionalInfo[MEM_SEC_ADDINFO_LENGTH + 1];       //附加信息
}ST_SECURITY_LOG;
#pragma pack(pop)


class CMemOutput : public ILogOutput
{
public:
    CMemOutput();
    virtual ~CMemOutput();

   /**********************************************************************
   * name           : OperationOutput
   * description    : 操作日志输出缓存
   * input          : @pUser:           用户
                      @pIp:             IP地址
                      @iLevel:          日志等级
                      @pServiceCode:    业务代码
                      @pModuleType:     模块类型
                      @pNodeCode:     模块码
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
                                );

   /**********************************************************************
   * name           : SecurityOutput
   * description    : 安全日志输出缓存
   * input          : @pUser:           用户
                      @pIp:             IP地址
                      @iLevel:          日志等级
                      @pServiceCode:    业务代码
                      @pModuleType:     模块类型
                      @pNodeCode:     模块码
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
                               );

    /**********************************************************************
    * name           : GetOperation
    * description    : 取出一条操作日志缓存
    * input          : NA
    * output         : 操作日志结构体
    * return         : BP_OK - 成功 B_FAIL - 失败
    * remark         : NA
    *********************************************************************/
    virtual int GetOperation ( ST_OPERATION_LOG& stOperLog );

    /**********************************************************************
    * name           : GetSecurity
    * description    : 取出一条安全日志缓存
    * input          : NA
    * output         : 操作日志结构体
    * return         : BP_OK - 成功 B_FAIL - 失败
    * remark         : NA
    *********************************************************************/
    virtual int GetSecurity ( ST_SECURITY_LOG& stSecLog );

private:
    BP_Thread_Mutex m_mutexOperLog;             //操作日志线程锁
    std::list<ST_OPERATION_LOG> m_lstOperLog;   //操作日志队列

    BP_Thread_Mutex m_mutexSecLog;              //安全日志线程锁
    std::list<ST_SECURITY_LOG> m_lstSecLog;     //安全日志队列
};

NAMESPACE_BP_END

#endif //bp_memoutput.h

