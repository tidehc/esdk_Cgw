/********************************************************************
filename    :    ivs_sql.h
author      :    yWX150774
created     :    2013/03/07
description :    SQL语法导出器
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/03/07 初始版本
*********************************************************************/

#ifndef CBB_SQL_H
#define CBB_SQL_H


#include "ivs_uncopy.hpp"

#include <string>
#include <map>

NAMESPACE_CBB_BEGIN

/*************************************************************
 *
 * SQL语法导出类, 是一个不可拷贝的类
 *
 ************************************************************/
class EXPORT_DLL CSQLExporter : private CUnCopy
{
public:
    CSQLExporter();
    ~CSQLExporter();

public:
    /*********************************************************
     * 初始化环境, 可以重复调用
     ********************************************************/
    bool Init(const char * pszConfigFileName);

    /*********************************************************
     * 获取SQL表达式, 成功返回 IVS_SUCCEED
     ********************************************************/
    int GetSQL(const char * pszKey, char * pszSQLBuffer, 
        int iSQLBufferSize, va_list vArgs) const;

    /*********************************************************
     * 获取SQL表达式, 成功返回 IVS_SUCCEED
     ********************************************************/
    int GetSQL(const char * pszKey, 
        char * pszSQLBuffer, int iSQLBufferSize, ...) const;

    /*********************************************************
     * 清理环境, 非必须调用接口
     ********************************************************/
    void Clean();

private:
    bool                                m_bInit;            // 是否已经成功初始化
    std::map<std::string, std::string>  m_mapSQLExpression; // 保存SQL表达式的映射表
};

NAMESPACE_CBB_END


#endif // CBB_SQL_H
