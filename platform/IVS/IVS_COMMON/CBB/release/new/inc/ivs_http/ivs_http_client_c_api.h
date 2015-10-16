/********************************************************************
filename    :    ivs_http_client_c_api.h
author      :    yWX150774
created     :    2013/03/06
description :    HTTP客户端上传、下载的C接口
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/03/06 初始版本
*********************************************************************/
#ifndef IVS_HTTP_CLIENT_C_API_H
#define IVS_HTTP_CLIENT_C_API_H

#include "ivs_namespace.h"

/* 下面接口不加入CBB命名空间的原因是, 可能C#要用下面接口, 所以不使用命名空间 */

/*********************************************************
 * 功能:   从HTTP服务器下载文件
 * 返回值: 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ********************************************************/
extern "C" EXPORT_DLL int IVS_Http_Client_DownloadFile(
    const char * pszLocalWriteFileName, 
    const char * pszRemoteURL, 
    const char * pszUserName = NULL, 
    const char * pszPassword = NULL);

/*********************************************************
 * 功能:   上传文件到HTTP服务器
 * 返回值: 成功返回 IVS_SUCCEED, 失败返回 IVS_FAIL
 ********************************************************/
extern "C" EXPORT_DLL int IVS_Http_Client_UploadFile(
    const char * pszLocalReadFilename, 
    const char * pszRemoteURL, 
    const char * pszUserName = NULL, 
    const char * pszPassword = NULL);

#endif // IVS_HTTP_CLIENT_C_API_H
