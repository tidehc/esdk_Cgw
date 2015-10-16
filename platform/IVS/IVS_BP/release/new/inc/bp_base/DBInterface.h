/********************************************************************
filename    :    DBInterface.h
author       :     z00193167
created      :    2012/04/13
description :    数据库接口类
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history       :     2012/04/13 初始版本
*********************************************************************/
#ifndef DB_INTERFACE_H
#define DB_INTERFACE_H

#include "bp_mutex.h"
#include "ace_header.h"
#include "db_connect.h"
#include "bp_db_config.h"
#include <string>
#include <queue>
#include <map>
#include "bp_errno.h"

#ifdef WIN32
    #define DATBASE_CONFIG_FILE_NAME ("\\db.xml")
#else
    #define DATBASE_CONFIG_FILE_NAME ("/db.xml")
#endif

#ifdef ARM_HIS3531
#define BP_ERROR                  (-1)
#define BP_CON_DATE_LEN_USUAL               (51)
#define BP_STRING_START_MINSYSTEM_FILE                ("/mnt/minsystem")
#define BP_CON_READ_DATE_LESS_THAN_DEF                 (1)
#define BP_STRING_START_MINSYSTEM_SIGN                ("-minsystem")


int BPReadFile(char *dev,char *pcBuf,int iLen);
int BpReadMinSystem(void);

#endif

#define BP_CONNPOOL_EMPTY 0
#define BP_CONNPOOL_SIZE  1 
#define BP_CONNPOOL_MAX_SIZE 10
#define LOCAL_DB  0
#define CENTER_DB 1
#define LOCAL_DB_INDEX  0
#define CENTER_DB_INDEX 1
#define LOCAL_DB_POOL_NUM 1
#define CLUSTER_DB_POOL_NUM 2
#define MAX_DB_POOL_NUM 2
typedef DLLEXPORT DB_CONNECT_TYPE BP_DBTYPE;

typedef bool (*DBExceptionCallBack)(int, const char *, const char *);

const unsigned int DB_EXCEPTION_USER_LENGTH   = 32;
const unsigned int DB_EXCEPTION_SERVER_LENGTH = 256;

typedef struct DBAlarmInfo
{
    uint32_t      uiErrorCode;                          // 错误码
    char          szUser[DB_EXCEPTION_USER_LENGTH];     // 用户名
    char          szServer[DB_EXCEPTION_SERVER_LENGTH]; // 服务器URL
} DB_ALARM_INFO;

typedef IDBConnect* (*CreateDBHandle)();
typedef void (*InitDBContextHandle)(DBExceptionCallBack);

enum
{
    DB_ALARM_FOR_CONNECT_FAIL = 600,   // 数据库连接失败
    DB_ALARM_FOR_ACCESS_FAIL,          // 数据库访问失败
    DB_ALARM_FOR_CONNECT_RESUME,       // 数据库连接恢复
    DB_ALARM_FOR_ACCESS_RESUME         // 数据库访问恢复
};

/**
 * @class 数据库连接类。
 * @brief 读取数据库连接参数，管理数据库连接。
 * 数据库连接池提供2种方案：(只能2选1，不能同时使用)
 * 1. 公用数据库连接池：
 *    1）进程启动时，创建一个数据库连接池，该连接池包括N个数据库连接。
 *    2).当某个线程要访问数据库时，从该连接池中申请分配一个数据库连接，使用完之后，再归还到连接池。
 *    接口有：Init(), GetConnection(), PutConnToPool()
 * 2. 线程专属数据库连接
 *    1).通过配置，一个线程一个数据库连接, 一个数据库连接只能被指定的线程，一个线程也只能使用指定的数据库连接。
 *       线程启动时，读取配置文件中的数据库连接标志，确定是否要连接数据库。
 *    2).一个进程中，部分线程不可以访问数据库。
 *    接口有： Init(), GetThreadDBConnection()
 */
class DLLEXPORT CDBDriverManager
{
private:
    CDBDriverManager ( void );

    CDBDriverManager ( CDBDriverManager& );

    CDBDriverManager& operator = ( CDBDriverManager& );

public:
    virtual ~CDBDriverManager ( void );

    /**********************************************************************
    * name           : Instance
    * description    : 获得CDBDriverManager的实例
    * input          : dbType, 连接池管理实例的类型，默认是0
    * return         : BP_OK - 成功初始化 BP_FAIL -初始化失败
    * remark         : NA   
    *********************************************************************/
    static CDBDriverManager& Instance ( int dbType = LOCAL_DB );

    /**********************************************************************
    * name           : IsNeedRefreshDBConn
    * description    : 是否需要定期更新数据库连接
    * input          : NA
    * return         : TRUE 需要定期更新数据库连接，FALSE不需要
    * remark         : NA   
    *********************************************************************/
    static bool IsNeedRefreshDBConn();

    /**********************************************************************
    * name           : RefreshAllConn
    * description    : 更新所有连接
    * input          : NA
    * return         : NA
    * remark         : NA   
    *********************************************************************/
    static void RefreshAllConn();

    /**********************************************************************
    * name           : Init
    * description    : 数据库连接管理类的初始化
    * input          : pDBCallBackFunc 数据库异常回调函数
    * return         : BP_OK - 成功初始化 BP_FAIL -初始化失败
    * remark         : 业务开始时，需要调用，建议在mainctrl的init函数中调用。
    *********************************************************************/
    int Init ( DBExceptionCallBack pDBCallBackFunc = NULL);

    /**********************************************************************
    * name           : Fini
    * description    : 数据库结束时调用，销毁数据库连接池
    * input          : NA
    * return         : NA
    * remark         : 业务结束时，一定要记得调用，建议在mainctrl的uninit函数中调用。
    *********************************************************************/
    int Fini ( void );

    /**********************************************************************
    * name           : GetConnection
    * description    : 取得一个数据库连接
    * input          : NA
    * return         : IDBConnect对象的一个指针
    * remark         : 调用此函数的分配的连接，必须使用PutConnToPool放回池，不能手动释放。
    *********************************************************************/
    IDBConnect* GetConnection ( void );

    /**********************************************************************
    * name           : PutConnToPool
    * description    : 将一个连接放回数据库连接池
    * input          : 对应类型的数据库连接IDBConnect*
    * return         : NA
    * remark         : 将已连接数据库的句柄放回池
    *********************************************************************/
    void PutConnToPool ( IDBConnect* );

    /**********************************************************************
    * name           : OpenConnection()
    * description    : 打开线程的专有数据库连接
    * input          : 无
    * return         : BP_OK - 打开成功 BP_FAIL - 打开失败
    * remark         : 调用GetThreadDBConnection之前，必须调用此方法。
    *********************************************************************/
    int OpenThreadDBConnection ();

    /**********************************************************************
    * name           : GetThreadDBConnect()
    * description    : 获取调用线程的特定数据库连接
    * input          : 无
    * return         : 返回数据库连接
    * remark         : 此函数分配的连接不允许手动释放，此类销毁时会自动释放。
    *********************************************************************/
    IDBConnect* GetThreadDBConnection( void );

    BP_DBTYPE GetDbType();

    /**********************************************************************
    * name           : GetServiceIp()
    * description    : 获取当前连接的数据库的ip
    * input          : 无
    * return         : 返回数据库ip
    * remark         : 这里的数据库ip其实就是从db.xml中读取出来的ip
    *********************************************************************/
	std::string GetServiceIp();
    void SetServiceIp(const std::string& strIp);

    void RefreshConn();
protected:
    /**********************************************************************
    * name           : NewConnection()
    * description    : 创建数据库连接(无连接)
    * input          : DB_ORACLE 创建ORACLE数据库 DB_PGSQL 创建Postgre SQL数据库
    * return         : 对应类型的数据库连接IDBConnect*
    * remark         : NA   
    *********************************************************************/
    IDBConnect* NewConnection(BP_DBTYPE);

    //去掉
    /**********************************************************************
    * name           : GetConnection
    * description    : 创建数据库连接(有连接)
    * input          : 服务器地址，用户名，密码
    * return         : 对应类型的数据库连接IDBConnect*
    * remark         : ORACLE格式 IP:port/db PGSQL格式 ODBC中的数据源名称
    *********************************************************************/
    IDBConnect* GetConnection(BP_DBTYPE, const char*, const char*, const char*,const char*);

    /**********************************************************************
    * name           : CreateConnPool
    * description    : 创建数据库连接池(有连接)
    * input          : 类型，服务器地址，用户名，密码，池容量（可选）
    * return         : 池内的数据库连接数
    * remark         : 池大小有默认值，可根据业务自行调整
    *********************************************************************/
    int CreateConnPool(BP_DBTYPE, const char*, const char*, const char*,const char*,
                       int size = BP_CONNPOOL_SIZE);

    /**********************************************************************
    * name           : GetConnFromPool
    * description    : 从数据库连接池中取一个连接
    * input          : NA
    * return         : 对应类型的数据库连接IDBConnect*
    * remark         : 从池中获取已连接数据库的句柄
    *********************************************************************/
    IDBConnect* GetConnFromPool ( void );


private:
    /**********************************************************************
    * name           : GetDBFcn
    * description    : 载入对应的动态库，连接数据库
    * input          : DB_ORACLE 创建ORACLE数据库 DB_PGSQL 创建Postgre SQL数据库
    * output         : NA
    * return         : 创建数据库句柄
    * remark         : NA   
    *********************************************************************/
    CreateDBHandle  GetDBFcn(const char*);

private:
#ifdef WIN32
    HINSTANCE       m_dbhandle;
#else
    void*             m_dbhandle;
#endif
    static CDBDriverManager m_Mgr[MAX_DB_POOL_NUM];                       //本地库与中心库各一个实例

    BP::CDatabaseConnectionConfig m_DatabaseConnectionConfig;             //数据库配置文件

    CreateDBHandle m_CreateDBHnd;

    std::queue<IDBConnect*> m_pool;                                       //连接池

    bool m_bInit;                                                         //初始化标志

    DBExceptionCallBack m_pDBCallBackFunc;                                //数据库异常回调函数

    int m_iPoolSize;                                                      //连接池容量大小

    ACE_Semaphore *m_Semaphore;                                           //获取连接信号量

    BP_Recursive_Thread_Mutex m_mapConnMutex;                             //连接、进程映射表

    std::map<ACE_thread_t, IDBConnect* > m_mapConn;                       //连接、进程映射表

    int m_iDbType;                                                        //当前实例的数据库类型，0本地库，1中心库

    int m_iPoolVersion;                                                   //连接池版本号，避免放错连接
    
    static int m_iRefreshTime;
};

#endif // DB_INTERFACE_H

