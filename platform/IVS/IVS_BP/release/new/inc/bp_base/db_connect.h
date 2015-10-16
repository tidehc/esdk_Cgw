#ifndef _db_connect_h
#define _db_connect_h


#include "db_parameter.h"
#include "db_result.h"


class DLLEXPORT  IDBConnect
{
public:
    IDBConnect():m_tCreateTime(time(NULL)){}
    virtual ~IDBConnect(){}

public:
    /**********************************************************************
    * name           : Connect
    * description    : 数据库连接函数，新建一个连接
    * input          : pszDBServer-数据库服务名，pszUser-用户名，pszPasswd-密码
    * output         : NA
    * return         : IVS_SUCCEED -连接成功  IVS_FAIL -连接失败
    * remark         : oracle和pgsql的数据库服务名不一致,如果需要对这个连接重新连接，使用ReConnect
    *********************************************************************/
    virtual int Connect(const char* pszDBServer, const char* pszUser, const char* pszPasswd) = 0;

    /**********************************************************************
    * name           : ReConnect
    * description    : 重置连接
    * input          : pszDBServer-数据库服务名，pszUser-用户名，pszPasswd-密码
    * output         : NA
    * return         : IVS_SUCCEED -连接成功  IVS_FAIL -连接失败
    * remark         : oracle和pgsql的数据库服务名不一致   
    *********************************************************************/
    virtual int ReConnect(const char* pszDBServer, const char* pszUser, const char* pszPasswd) = 0;

    /**********************************************************************
    * name           : Execute
    * description    : 执行SQL语句
    * input          : pSql-SQL语句，pParam-动态参数，iParamSize-参数个数
    * output         : NA
    * return         : IDBResultSet* 数据集
    * remark         : NA   
    *********************************************************************/
    virtual IDBResultSet* Execute(const char *pSql, CDBParam* pParam, int iParamSize) = 0;
    
    /**********************************************************************
    * remark         : 兼容接口，仅供MAU使用。其它模块请使用Execute
    **********************************************************************/
    IDBResultSet* Insert(const char *pSql, CDBParam* pParam, int iParamSize)
                            {return Execute(pSql, pParam, iParamSize);}
    IDBResultSet* Delete(const char *pSql, CDBParam* pParam, int iParamSize)
                            {return Execute(pSql, pParam, iParamSize);}
    IDBResultSet* Update(const char *pSql, CDBParam* pParam, int iParamSize)
                            {return Execute(pSql, pParam, iParamSize);}
    IDBResultSet* Select(const char *pSql, CDBParam* pParam, int iParamSize)
                            {return Execute(pSql, pParam, iParamSize);}

    /**********************************************************************
    * name           : SEQNextVal
    * description    : 返回当前的sequence值，并将sequence自动加1
    * input          : seqName-seqence名称
    * output         : NA
    * return         : long
    * remark         : NA   
    *********************************************************************/
    virtual int SEQNextVal(const char *seqName) = 0;

    /**********************************************************************
    * name           : Commit
    * description    : 事务提交
    * input          : NA
    * output         : NA
    * return         : IVS_SUCCEED -提交成功  IVS_FAIL -提交失败
    * remark         : NA   
    *********************************************************************/
    virtual int Commit(void) = 0;

    /**********************************************************************
    * name           : Rollback
    * description    : 回滚数据
    * input          : NA
    * output         : NA
    * return         : IVS_SUCCEED -回滚成功  IVS_FAIL -回滚失败
    * remark         : NA   
    *********************************************************************/
    virtual int Rollback(void) = 0;

    /**********************************************************************
    * name           : DisConnect
    * description    : 断开数据库连接
    * input          : NA
    * output         : NA
    * return         : IVS_SUCCEED -断开成功  IVS_FAIL -断开失败
    * remark         : NA   
    *********************************************************************/
    virtual int DisConnect(void) = 0;

    /**********************************************************************
    * name           : IsConnected
    * description    : 是否还在连接状态
    * input          : NA
    * output         : NA
    * return         : true -连接  false -断开
    * remark         : NA   
    *********************************************************************/
    virtual bool IsConnected(void) = 0;

    /**********************************************************************
    * name           : getDbType
    * description    : 获取连接到的数据库类型
    * input          : NA
    * output         : NA
    * return         : int, 0本地库, 1中心库
    * remark         : NA   
    *********************************************************************/
	virtual int getDbType(void) = 0;

    /**********************************************************************
    * name           : setDbType
    * description    : 设置连接到的数据库类型
    * input          : dbType, 0本地库, 1中心库
    * output         : NA
    * return         : void
    * remark         : NA   
    *********************************************************************/
	virtual void setDbType(int dbType) = 0;

    /**********************************************************************
    * name           : getPoolVersion
    * description    : 获取连接版本号
    * input          : NA
    * output         : NA
    * return         : int
    * remark         : NA   
    *********************************************************************/
	virtual int getPoolVersion(void) = 0;

    /**********************************************************************
    * name           : setPoolVersion
    * description    : 设置连接版本号
    * input          : NA
    * output         : NA
    * return         : void
    * remark         : NA   
    *********************************************************************/
	virtual void setPoolVersion(int poolVersion) = 0;

	virtual void Clear() = 0;

    time_t GetCreateTime(){return m_tCreateTime;}

protected:
    time_t m_tCreateTime;
};

#endif
