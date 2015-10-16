/********************************************************************
filename    :    bp_load_db_connection_config.h
author      :    cwx148380
created     :    2012/11/08
description :    读取数据库配置
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/08 初始版本
*********************************************************************/
#ifndef BP_LOAD_DB_CONNETION_CONFIG_H
#define BP_LOAD_DB_CONNETION_CONFIG_H

#include "ivs_xml.h"
#include "bp_db_config.h"
#include "bp_config_alarmer.h"
#include <string>

NAMESPACE_BP_BEGIN //lint !e129 !e10

class CLoadDatabaseConnectionConfig : private CConfigAlarmer
{
public:
    CLoadDatabaseConnectionConfig ( void );
    virtual ~CLoadDatabaseConnectionConfig (void );

    //读取XML配置文件
    int LoadConfigFile(const std::string& strConfigPath, int dbType = 0);
    const CDatabaseConnectionConfig& GetDatabaseConnetionConfig ( void ) const;

public:
    //读取Database节点配置信息
    int LoadDatabaseNodeConfig( const std::string& strConfigPath );

    bool GetDatabaseTypeElement ( std::string & strDatabaseType );

    bool GetDatabaseNameElement ( std::string & strDatabaseName );

    bool GetServiceIpElement ( std::string & strServiceIp );

    bool GetServicePortElement ( std::string & strServicePort );

    bool GetUserNameElement ( std::string & strUserName );

    bool GetUserPasswordElement ( std::string & strUserPassword );

    bool GetUserIvSaltElement(std::string & strIvSalt);
	
    int  GetDatabasePoolSizeElement ( void );

    int  GetDataConnRefreshTime ( void );

private:
    CBB::CXml                   m_DbConnectionXml;    // 配置文件对象
    CDatabaseConnectionConfig   m_DbConnectionConfig; // 数据库连接参数实例
};

NAMESPACE_BP_END   //

#endif //BP_LOAD_DB_CONNETION_CONFIG_H
