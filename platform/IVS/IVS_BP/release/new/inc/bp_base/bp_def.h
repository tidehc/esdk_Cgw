/********************************************************************
    filename    :     bp_def.h
    author        :    s00191067
    created        :    2012/03/27
    description    :    提供MAU类型声明
    copyright    :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
    history        :    2012/03/27 初始版本
*********************************************************************/
#ifndef BP_DEF_H
#define BP_DEF_H

#include <string>
#include <iostream>
#include "bp_log.h"
#include "bp_namespace.h"
#include "bp_mem.h"
#include "bp_stdint.h"

NAMESPACE_BP_BEGIN // namespace begin

#ifdef WIN32
#define EXPORT_DLL __declspec(dllexport)
#else
#define EXPORT_DLL
#endif

// 定义数据库动态连接库文件名称
#if defined(_DEBUG) || defined (DEBUG)
# define DB_DLL_NAME_ORACLE "db_oracled"
# define DB_DLL_NAME_PG "db_pgsqld"
#else
# define DB_DLL_NAME_ORACLE "db_oracle"
# define DB_DLL_NAME_PG "db_pgsql"
#endif

#define TRANSACTIONNO_LEN 64	//事务ID
#define RESERVE_LEN 32		//保留字段长度
#define DEVICE_CODE_LEN	40	//网元设备编码

#define LOOP_RETRY             (0)     // link循环重连
#define IP_LENGTH              (65)    // IP地址长度
#define NET_ELEMENT_ID_LEGTH   (41)    // 网元ID长度

//主服务线程处理的消息，一般用于上报事件（告警或状态改变）
#define LINK_STATUS_REPORT_CMD 0x8001   //LINK状态上报命令
#define FLUX_CONTROL_WARN_CMD  0x8002   //流控告警上报
#define LOG_WARN_CMD           0x8003   //日志告警上报
#define DB_WARN_CMD            0x8004   //数据库告警上报
#define CONFIG_WARN_CMD        0x8005   //配置告警上报

// 判断指针是否为空
#define CHECK_POINTER( pPoint, retValue )    \
if ( NULL == ( pPoint ) )                    \
{                                            \
    BP_DBG_LOG("Point " #pPoint " is null.");\
    return retValue;                         \
}                                            \

// 判断指针是否为空,无返回值,可能会提示多了分号
#define CHECK_POINTER_VOID( pPoint )        \
if ( NULL == ( pPoint ) )                   \
{                                           \
    BP_DBG_LOG("Point " #pPoint" is null.");\
    return ;                                \
}                                           \

// 判断执行是否成功，失败则return RetValue,有资源待释放时慎用
#define CHECK_SUCCESS_RET( ifPcLint, retValue )   \
do{                                               \
    if ( !(ifPcLint) ) {return retValue;}         \
}while(0)

// 判断AB是否相等  （==）
#define CHECK_EQUAL( A, B, retValue ) \
if ( (B) == (A) )                     \
{                                     \
    BP_DBG_LOG("A Equal B.");        \
    return retValue;                  \
}

#define NOLINT_MAIN if (false){ACE_Main acem;(void)ace_os_main_i(acem, argc, argv);}

// MAU消息块相关定义
typedef enum enMsgBlock
{
    MSG_BLOCK_SIZE          = 2 * 1024,     // 定义消息块大小
    NSS_BLOCK_SIZE          = 1800,         // 定义一个NSS包大小
    BLOCK_NUM_PER_CHANNEL   = 200,          // 每路视频分配的消息块个数
    MAX_QUEUE_MSG_NUM       = 1 * 1024,     // 定义消息队列最大消息个数
    MAX_QUEUE_BYTE_SIZE     = (MSG_BLOCK_SIZE * MAX_QUEUE_MSG_NUM)  // 定义消息队列最大SIZE
} MSG_BLOCK;

// 定义线程堆栈大小
#define DEFAULT_THREAD_STACK_SIZE   (128 * 1024)

// 队列保护 定义队列默认最大容量
#define DEFAULT_QUEUE_PROTECT_SIZE  (2 * 1024)

// Transaction的最大值
#define DEFAULT_TRANSACTION_MAP_MAX_SIZE 2000000

// Session的最大容量
#define DEFAULT_SESSION_MAP_MAX_SIZE     200000

// 线程消息队列的最大容量
#define DEFAULT_CMD_QUEUE_LENGTH    3000

// 队列保护 针对Sequence队列的容量
#define MAX_QUEUE_PROTECT_SIZE  (8 * 1024)

const int     SERVICE_COMMU = 1;

//告警消息重复
const unsigned int RESEND_INTERVAL_OFFSET_MIN        = 1;     // 最小重发时间间隔
const unsigned int RESEND_INTERVAL_OFFSET_DEFAULT    = 5;     // 默认重发时间间隔
const unsigned int RESEND_INTERVAL_OFFSET_MAX        = 40;    // 最大重发时间间隔

const unsigned int RESEND_TIMES_MIN                  = 0;     // 最小重发次数
const unsigned int RESEND_TIMES_DEFAULT              = 10;    // 默认重复次数
const unsigned int RESEND_TIMES_MAX                  = 15;    // 最大重发次数

// CmdType的枚举定义
typedef enum enCmdType
{
    CMD_UNDEF   = 0,
    CMD_SEND,
    CMD_RECEIVE,
    CMD_CREATE_LINK,
    CMD_DESTROY_LINK,
	CMD_CREATE_LINK_SYNC,
	CMD_DESTROY_LINK_SYNC,
    CMD_SET_LINK_AUTH,
	CMD_LIMIT,

} CMD_TYPE;


typedef enum enSubsysType
{
    SUBSYS_NONE,  //无效值
    SUBSYS_SCU = 1,
    SUBSYS_SMU = 2,
    SUBSYS_MU  = 3,
    SUBSYS_DCG = 4,
    SUBSYS_PCG = 5,
    SUBSYS_OMU = 6,
    SUBSYS_CMU = 7,
    SUBSYS_SDK = 8,
	SUBSYS_MULM = 9,
    SUBSYS_MAU = 16,
    //在此之前添加类型值
    SUBSYS_MAX
} SUBSYS_TYPE;


// 连接类型
typedef enum enLinkType
{
    LTYPE_TCP,                    // TCP连接
    LTYPE_UDP,                    // UDP连接
}LINK_TYPE;

// 应用层协议类型
typedef enum enProtocolType
{
    PROTOCOL_UNDEF = 0,
    PROTOCOL_IVSP  = 1,      // 私有协议
    PROTOCOL_RTSP  = 2,      // RTSP
    PROTOCOL_RTP   = 3,       // RTP协议
    PROTOCOL_RTCP  = 4,      // RTCP协议
    PROTOCOL_NSS   = 5,
	PROTOCOL_LIMIT
}PROTOCOL_TYPE;

enum ProtocolAuthType
{
    AUTH_NONE,
    AUTH_PASSWD
};

/*
 * 生成网元类型
 * 网元类型为有符号32位整数, 最大值为2147483647.
  xx xx xx
        |--- 协议类型,参见PROTOCOL_TYPE
  |   |--- 进程类型, 由业务定义，但要汇总到BP
  |--- 子系统类型, 参见IVS_SUBSYS_TYPE
*/
#define MAKE_NET_ELEMENT_TYPE(subsytem, process, protocl_type) ((int)(subsytem) * 10000 + (int)(process) * 100 + (int)(protocl_type) )

#define NET_ELE_SCU_NONE    0
#define NET_ELE_SCU_NSS     10105
#define NET_ELE_SMU_NSS     20105
#define NET_ELE_MU_NSS      30105
#define NET_ELE_DCG_NSS     40105
#define NET_ELE_PCG_NSS     50105
#define NET_ELE_OMU_NSS     60105
#define NET_ELE_CMU_NSS     70105
#define NET_ELE_SDK_NSS		80105
#define NET_ELE_SUB_SMU_NSS 90105
#define NET_ELE_MAU_NSS     160105


using std::string;

// 线程名称
#define THREAD_SERVICE_NAME "Service"
#define THREAD_COMMU_NAME   "Commu"
#define THREAD_ROUTE_LOCUS_SERVICE_NAME "RouteLocusService"

//reinterpret_cast类型转换
#ifdef WIN32
#define MAU_REINTEPERET_CAST(dstType) reinterpret_cast<dstType>
#else
#define MAU_REINTEPERET_CAST(dstType) (dstType)
#endif

// snprintf定义
#ifdef  WIN32
#define SNPRINTF  _snprintf_s
#else
#define SNPRINTF  snprintf
#endif

#ifdef WIN32
#define STRTOK  strtok_s
#else
#define STRTOK  strtok_r
#endif

NAMESPACE_BP_END   // namespace end
#endif //BP_DEF_H
