/********************************************************************
filename    :    ivs_sftp_client_c_api.h
author      :    yWX150774
created     :    2013/03/04
description :    SFTP客户端上传、下载的C接口
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/03/04 初始版本
*********************************************************************/
#ifndef IVS_SFTP_CLIENT_C_API_H
#define IVS_SFTP_CLIENT_C_API_H

#include "ivs_namespace.h"

/* 下面接口不加入CBB命名空间的原因是, C#要用下面接口, 不能使用命名空间 */

/*********************************************************
 * 功能:   从SFTP服务器下载文件
 * 返回值: 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ********************************************************/
extern "C" EXPORT_DLL int IVS_Sftp_Client_DownloadFile(
    const char * pszHostname, unsigned short usHostPort, 
    const char * pszUsername, const char * pszPassword, 
    const char * pszRemoteReadFilename, const char * pszLocalWriteFilename);

/*********************************************************
 * 功能:   上传文件到SFTP服务器
 * 返回值: 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ********************************************************/
extern "C" EXPORT_DLL int IVS_Sftp_Client_UploadFile(
    const char * pszHostname, unsigned short usHostPort, 
    const char * pszUsername, const char * pszPassword, 
    const char * pszLocalReadFilename, const char * pszRemoteWriteFilename);

#endif // IVS_SFTP_CLIENT_C_API_H
