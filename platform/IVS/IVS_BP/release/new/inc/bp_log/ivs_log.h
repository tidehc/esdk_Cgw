#ifndef  __IVS_LOG_H__
#define  __IVS_LOG_H__

#include "bp_logoutput.h"

//#ifdef WIN32
//# ifdef BP_LOG_EXPORTS
//#  define EXPORT_BP_LOG_API __declspec(dllexport)
//# else
//#  define EXPORT_BP_LOG_API __declspec(dllimport)
//# endif
//#else
//#define EXPORT_BP_LOG_API
//#endif

enum LOG_LEVEL 
{
    IVSLOG_EMERG   = 0,   /* 紧急，系统不可用 */
    IVSLOG_ALERT   = 1,   /* 必须立即采取措施 */
    IVSLOG_CRIT    = 2,   /* 致命情况         */
    IVSLOG_ERR     = 3,   /* 错误情况         */
    IVSLOG_WARNING = 4,   /* 警告情况         */
    IVSLOG_NOTICE  = 5,   /* 一般重要情况     */
    IVSLOG_INFO    = 6,   /* 普通信息         */
    IVSLOG_DEBUG   = 7,   /* 调试信息         */
};

enum LOG_TYPE
{
    LOG_TYPE_MIN,
    TYPE_SEC = LOG_TYPE_MIN, /* 安全日志 */
    TYPE_OPR,                /* 操作日志 */
    TYPE_RUN,                /* 运行日志 */
    TYPE_ITF,                /* 接口日志 */
    //TYPE_USR,   /* 用户日志 */  //去掉用户日志
    TYPE_DBG,                /* 调试日志 */

    //在这之前添加值
    LOG_TYPE_MAX
};


/* 时区设置标志 */
enum  {
    UTC_FLG_PLUS = 0,   /* UTC + */
    UTC_FLG_NEG  = 1    /* UTC - */
};


#pragma pack(push, 1)

struct BP_LOG_INFO
{
    unsigned long Size;   //日志大小
    unsigned int  Number; //日志个数
    int Level;            //日志级别
};

enum SFTP_INFO_LENGTH
{
    SFTP_WORKSPACE_LENGTH   = 768, 
    SFTP_SERVER_ADDR_LENGTH = 64, 
    SFTP_USER_NAME_LENGTH   = 64, 
    SFTP_PASSWORD_LENGTH    = 64,
    SFTP_IVSALT_LENGTH    = 64
};

struct BP_LOG_SFTP_INFO
{
    char szWorkSpace[SFTP_WORKSPACE_LENGTH];    // 备份文件一级目录
    char szRemoteAddr[SFTP_SERVER_ADDR_LENGTH]; // sftp服务器地址
    unsigned short usPort;                      // sftp服务器端口
    char szUsername[SFTP_USER_NAME_LENGTH];     // sftp登入用户名
    char szPassword[SFTP_PASSWORD_LENGTH];      // sftp登入密码
    char szIvSalt[SFTP_IVSALT_LENGTH];//sftp 登陆盐值
};

#define MODULE_TYPE_LENGTH 33
struct BP_LOG_CONF
{
    char Path[768];
    char ModuleType[MODULE_TYPE_LENGTH];
    char NodeCode[MODULE_TYPE_LENGTH];
    struct BP_LOG_INFO LogInfo[LOG_TYPE_MAX];
#ifdef ARM_HIS3531
    LOG_LEVEL LogLevel;         // 日志级别
#endif
    bool NeedOperationLog;      // 是否需要写操作日志
    bool NeedSecurityLog;       // 是否需要写安全日志
    bool BackupToDebugLog;      // 是否将别的日志内容备份到Debug日志文件中.
    bool BackupOperationLog;    // 是否进行入库操作.
    bool BackupSecurityLog;     // 是否进行入库操作.
    bool LogBufferEnable;       // 写日志方式是缓存还是直接写（true为通过缓存），只对run和dbg日志生效
    bool IsBufferDebugLog;      // 是否缓存debug日志，如果出现严重级别以上错误时，打印此前若干条debug级别的日志
    int BufferNumber;           // 缓存debug日志条数
    struct BP_LOG_SFTP_INFO SftpInfo;
};

#pragma pack(pop)


/* 工程相关设置 */
#define LOG_HAS_SFTP_UPLOAD       /* 日志sftp上传功能 注释取消该功能 */
#define LOG_HAS_CONFIG_FILE       /* 日志自有配置文件 注释取消该功能 */
#define LOG_HAS_WRITE_THREAD      /* 使用独立线程写入 注释取消该功能 */
#define LOG_HAS_STACK_TRACE       /* 级别严重的日志同时打印调用栈，注释取消该功能 */

#if defined LOG_HAS_SFTP_UPLOAD || (!defined LOG_HAS_SFTP_UPLOAD && !defined LOG_HAS_WRITE_THREAD)
#define LOG_HAS_MONITOR_THREAD   /* 监控线程 */
#endif 

#define LOG_HAS_SEC               /* 使能安全日志 注释去使能 */ 
#define LOG_HAS_OPR               /* 使能操作日志 注释去使能 */
#define LOG_HAS_ITF               /* 使能接口日志 注释去使能 */
//#define LOG_HAS_USER              /* 使能用户日志 注释去使能 */

#define FILE_COUNT_LOWER_BOUND         ( 5)  /* 本地日志文件最小保存数量 (各类型文件分别统计) */
#define FILE_COUNT_UPPER_BOUND         (10)  /* 本地日志文件最大保存数量 (各类型文件分别统计) */

#define FILE_SIZE_LOWER_BOUND          (10 * 1024 * 1024) /* 日志文件大小下限 (单位为M) */
#define FILE_SIZE_UPPER_BOUND          (30 * 1024 * 1024) /* 日志文件大小上限 (单位为M) */

/* 异常回调定义 */
#define LOG_ERR_DISKFULL       500   /* 磁盘满         */
//#define LOG_ERR_DISKRESTORE    501   /* 磁盘恢复       */
#define LOG_ERR_OPENFAIL       502   /* 打开文件失败    */
#define LOG_ERR_DBIMPORT       503   /* 导入数据库错误   */
#define LOG_ERR_RENAMEFAIL     504   /* 重命名文件失败  */
#define LOG_ERR_ZIPFAIL        505   /* 压缩文件失败    */
#define LOG_ERR_SFTPFAIL       506   /* 日志上传失败    */

/* 异常回调定义---恢复 */
#define LOG_RESUME_DISKFULL       550   /* 磁盘满恢复         */
//#define LOG_RESUME_DISKRESTORE    551   /* 磁盘恢复恢复       */
#define LOG_RESUME_OPENFAIL       552   /* 打开文件失败恢复   */
#define LOG_RESUME_DBIMPORT       553   /* 导入数据库错误恢复 */
#define LOG_RESUME_RENAMEFAIL     554   /* 重命名文件失败恢复 */
#define LOG_RESUME_ZIPFAIL        555   /* 压缩文件失败恢复   */
#define LOG_RESUME_SFTPFAIL       556   /* 日志上传失败恢复   */

#define LOG_LINE_LENGTH              2048            /* 日志行长度   */
#define LOG_ADDITIONALINFO_LENGTH    1024            /* 附加信息长度 */

/* 定义日志返回结果 */
#define LOG_RET_STR_OK ("successful")
#define LOG_RET_STR_FAIL ("failed")
/* --------------------------------------------------------------------------- */

/* 接口函数
 * IVS_logInit
 * @szDir:        日志保存目录, 必须已存在
 * @szConf:       配置文件路径, 使能配置文件则必需
 * @szModuleType: 模块类型
 * @uModuleID:    模块ID
 * @callback:     异常回调函数
 */
//extern "C" int EXPORT_BP_LOG_API IVS_LogInit(const char *szDir, char *szConf, char *szModuleType, unsigned int uModuleCode, void (*callback)(int code));
extern "C" int EXPORT_BP_LOG_API IVS_LogInit(struct BP_LOG_CONF& log_conf, bool (*callback)(int, const char *));
extern "C" void EXPORT_BP_LOG_API IVS_LogClean();

/* 接口函数
 * IVS_SetLogConf
 * @logType:        日志类别，支持安全日志(TYPE_SEC)、操作日志(TYPE_OPR)、运行日志(TYPE_RUN)
 * @logLevel:       日志级别
 * @szBeginTime:    参数生效时间，UTC时间，格式为yyyymmddHHMMss
 * @szEndTime:      参数失效时间，UTC时间，格式为yyyymmddHHMMss
 * @返回值：        -1 - 失败 0 - 成功 
 * remarks: 
 */
extern "C" int EXPORT_BP_LOG_API IVS_SetLogConf(LOG_TYPE logType, LOG_LEVEL logLevel, const char* szBeginTime, const char* szEndTime);

/* 接口函数
 * IVS_GetLogConf
 * @logType:        日志类别，支持安全日志(TYPE_SEC)、操作日志(TYPE_OPR)、运行日志(TYPE_RUN)
 * @logLevel:       日志级别
 * @返回值：        -1 - 失败 0 - 成功 
 * remarks: 
 */
extern "C" int EXPORT_BP_LOG_API IVS_GetLogConf(LOG_TYPE logType, LOG_LEVEL &logLevel);

/* 接口函数
 * IVS_LogInitOutput
 * @pOutput:        日志缓存类的指针
 * @返回值：-1 - 失败 0 - 成功 
 * remarks: 传入的指针，内部不做析构操作。
 */
extern "C" int EXPORT_BP_LOG_API IVS_LogInitOutput ( ILogOutput* pOutput );

/* 接口函数
 * IVS_LogCleanOutput
 * @返回值：-1 - 失败 0 - 成功 
 * remarks: 内部不做日志缓存类的析构操作，只是将指针置空，析构操作交给外部。
 */
extern "C" int EXPORT_BP_LOG_API IVS_LogCleanOutput ();

/* 接口函数
 * IVS_LogSetFileSize
 * @返回值：成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 * 功能描述：设置指定类型的日志文件大小
 * remarks: 日志文件大小默认为10M，10 * 1024 * 1024
 */
extern "C" int EXPORT_BP_LOG_API  IVS_LogSetFileSize(enum LOG_TYPE type, unsigned long s);

/* 接口函数
 * IVS_LogSetFileCount
 * @返回值：成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 * 功能描述：设置指定类型的日志文件个数
 * remarks: 
 */
extern "C" int EXPORT_BP_LOG_API IVS_LogSetFileCount(enum LOG_TYPE type, unsigned int c);

/* 接口函数
 * IVS_LogSetLevel
 * @返回值：成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 * 功能描述：设置指定类型的日志文件级别，level无效则设置为0
 * remarks: 如果日志文件的级别小于设置的级别，改日志文件将丢掉
 */
extern "C" int EXPORT_BP_LOG_API IVS_LogSetLevel(enum LOG_TYPE type, int iLevel);

/* 接口函数
 * IVS_LogSetDebug
 * @返回值：成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 * 功能描述：设置指定类型的日志文件是否打开调试日志
 * remarks: 如果没有打开调试日志，将不写调试日志文件信息。
 */
extern "C" int EXPORT_BP_LOG_API IVS_LogSetDebug(enum LOG_TYPE type, bool bEnable);

/* 接口函数
 * IVS_LogSetTimeZone
 * @返回值：-1 - 失败 0 - 成功 
 * 功能描述：设置时区信息
 * remarks: 设置当前时间相对于格林威治时区的偏移量
 */
extern "C" void EXPORT_BP_LOG_API IVS_LogSetTimeZone(unsigned int hour, unsigned int min, unsigned int sec, int flag);
extern "C" const char EXPORT_BP_LOG_API *IVS_GetLogTypeName(int logtype);


#ifdef LOG_HAS_SFTP_UPLOAD
extern "C" void EXPORT_BP_LOG_API IVS_LogSetSftp(const struct BP_LOG_SFTP_INFO & rSftpInfo);
#endif

/* 将错误码格式化输入至可变参数 */
int EXPORT_BP_LOG_API IVS_LogFormatErrCode(char* dest, const int len, const int errCode, const char* fmt);

/* 运行日志 */
extern "C" int EXPORT_BP_LOG_API IVS_LogRun( const int level, const char *operation, const char *file, const int line, const char *func, const char *fmt, ...);
#define IVS_LOG_RUN(level, operation, fmt, ...) \
    IVS_LogRun(level, operation, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

#define IVS_LOG(level, fmt, ...)\
    IVS_LogRun(level, "", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

/* 调试日志 */
extern "C" int EXPORT_BP_LOG_API IVS_LogDbg(const char *file, const int line, const char *func, const char *fmt, ...);
#define IVS_LOG_DBG(fmt, ...) \
    IVS_LogDbg(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)


/* 接口日志 */
#ifdef LOG_HAS_ITF 
extern "C" int EXPORT_BP_LOG_API IVS_LogInterface ( const int level,
                                                    const char *inftype, const char *infname,
                                                    const char *sender, const char *receiver,
                                                    const char *traceMark, const char *operation, const char *result,
                                                    const char *file, const int line, const char *func,
                                                    const char *fmt, ...
                                                   );

# define IVS_LOG_ITF(level, inftype, infname, sender, receiver, traceMark, operation, result, fmt, ...)\
    IVS_LogInterface(level, inftype, infname, sender, receiver, traceMark, operation, result,\
                     __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#endif 


/* 操作日志 */
#ifdef LOG_HAS_OPR
extern "C" int EXPORT_BP_LOG_API IVS_LogOperation ( const int level,
                                                    const char *user,  const char* ip, const char *object,
                                                    const int opcode, const char* operation, const char *result,
                                                    const char *file, const int line, const char *func,
                                                    const int errcode,
                                                    const char *fmt, ...
                                                   );

# define IVS_LOG_OPR(level, user, ip, object, opcode, operation, result, errcode, fmt, ...)\
    IVS_LogOperation(level, user, ip, object, opcode, operation, result,\
                     __FILE__, __LINE__, __FUNCTION__, errcode, fmt, ##__VA_ARGS__)
#endif 

/* 安全日志 */
#ifdef LOG_HAS_SEC
extern "C" int EXPORT_BP_LOG_API IVS_LogSecurity ( const int level,
                                                   const char *user, const char *ip,
                                                   const int opcode, const char* operation, const char *result,
                                                   const char *file, int line, const char *func,
                                                   const int errcode,
                                                   const char *fmt, ...
                                                 );

# define IVS_LOG_SEC(level, user, ip, opcode, operation, result, errcode, fmt, ...)\
    IVS_LogSecurity(level, user, ip, opcode, operation, result,\
                    __FILE__, __LINE__, __FUNCTION__, errcode, fmt, ##__VA_ARGS__)
#endif 


/* 用户日志 */
#ifdef LOG_HAS_USER
extern "C" int EXPORT_BP_LOG_API IVS_LogUser(int level, char *user, char *operation, char *result,
             char *file, int line, char *func, const char *fmt, ...);

# define IVS_LOG_USR(level, user, operation, result, fmt, ...)\
    IVS_LogUser(level, user, operation, result, \
        __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#endif 


/* 调试相关 */
extern "C" void EXPORT_BP_LOG_API IVS_LogAssert(const char *expr, const char *file, unsigned int line, const char *func);
#if defined(_DEBUG) || defined (DEBUG) 
# define IVS_ASSERT(expr) ((expr) ? void(0): IVS_LogAssert(#expr, __FILE__, __LINE__, __FUNCTION__))
#else
# define IVS_ASSERT()
#endif 

#ifdef __cplusplus
class EXPORT_BP_LOG_API CDbgTrace {
    CDbgTrace(const char *file, unsigned int line, const char *func)
        :m_pFunc(func), m_pFile(file), m_uLine(line)
    {
        IVS_LogDbg(m_pFile, line, m_pFunc, "Enter Function %s", func);   
    }
    ~CDbgTrace()
    {
        IVS_LogDbg(m_pFile, m_uLine, m_pFunc, "Leave Function %s", m_pFunc);
    }
private:    
    const char *m_pFunc;
    const char *m_pFile;
    unsigned int m_uLine;
};
# define IVS_TRACE()   CDbgTrace t(__FILE__, __LINE__, __FUNCTION__)
#else
# define IVS_TRACE()
#endif  // __cplusplus 

#endif // __IVS_LOG_H__

