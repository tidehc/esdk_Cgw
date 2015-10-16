/********************************************************************
filename    :    db_oracle.h
author       :    z00193167
created      :    2012/04/16
description :    
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history       :    2012/04/16 初始版本
*********************************************************************/
#ifndef DB_ORACLE_H
#define DB_ORACLE_H

#include "bp_def.h"
#include "DBInterface.h"
#include "db_exception_alarm.h"

#define LOGPATH  "./"

class otl_connect;
class otl_stream;

class COracleResultSet : public IDBResultSet
{
public:
    COracleResultSet();
    ~COracleResultSet();

public:
    // 关闭结果流
    void Close(void);

    // 是否到了流结尾
    bool HasNext(void);
    
    // 从流中输出一个int
    int GetInt(void);

    // 从流中输出一个float
    float GetFloat(void);

    // 从流中输出一个double
    double GetBignum(void);

    // 从流中输出一个string
    std::string GetString(void);

    // 从流中输出一个lob
    std::string GetLob(void);

private:
    // 根据sql和输入参数打开一个流
    void Open(int iBufferSize,const char *psql,otl_connect* pConnection, CDBParam* pParam, int iParamSize);

    // 写日志
    static void WriteLog(const char* strLog);

protected:
    otl_stream* m_pStream;

    friend class COracleConnect;
};

class COracleConnect : public IDBConnect
{
public:
    COracleConnect();
    ~COracleConnect();

public:
    // 初始化数据库环境，只调用一次
    static void InitDBContext(DBExceptionCallBack pFuncExcptionCallBack = NULL);

    // 根据用户名密码和服务器实例创建一个连接
    int Connect(const char* pszDBServer, const char* pszUser, const char* pszPasswd);

    // 重新建立连接，不再重新分配资源
    int ReConnect(const char* pszDBServer, const char* pszUser, const char* pszPasswd);

    IDBResultSet* Execute(const char* pSql, CDBParam* pParam, int iParamSize);

    int SEQNextVal(const char* seqName);

    int Commit(void);

    int Rollback(void);

    int DisConnect(void);

    bool IsConnected(void);

	int getDbType(void);
	void setDbType(int dbType);
	int getPoolVersion(void);
	void setPoolVersion(int poolVersion);
	void Clear();
private:
    std::string ExgChar(const char* pszStr, char rep) const;

protected:
    static DBExceptionCallBack s_pFuncExcptionCallBack;  // 数据库访问异常回调函数指针
    static CDBExceptionAlarm   s_aDBExceptionAlarm;      // 数据库告警对象

private:
    otl_connect* m_connet;
    COracleResultSet m_resultset;
	int m_iDbType;
    int m_iPoolVersion;
};

#endif //DB_ORACLE_H
