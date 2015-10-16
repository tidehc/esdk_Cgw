/********************************************************************
filename    :    ivs_http_client.h
author      :    yWX150774
created     :    2013/03/05
description :    HTTP客户端
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/03/05 初始版本
*********************************************************************/

#ifndef CBB_HTTP_CLIENT_H
#define CBB_HTTP_CLIENT_H


#include "ivs_uncopy.hpp"

#include <string>

#include "ace_header.h"

NAMESPACE_CBB_BEGIN

/*************************************************************
 *
 * HTTP客户端类, 是一个不可拷贝的类
 *
 ************************************************************/
class EXPORT_DLL CHttpClient : private CUnCopy
{
public:
    CHttpClient();
    ~CHttpClient();

public:
    /*********************************************************
     * 初始化环境
     ********************************************************/
    bool Init();

    /*********************************************************
     * 清理环境
     ********************************************************/
    void Exit();

    /*********************************************************
     * 创建与HTTP服务器通信的套接字
     ********************************************************/
    bool Connect(const char * pszHostName, unsigned short usHostPort);

    /*********************************************************
     * 关闭与HTTP服务器通信的套接字
     ********************************************************/
    void Close();

    /*********************************************************
     * 将本地文件上传到HTTP服务器
     ********************************************************/
    bool UploadFileByRemotePath(
        const char * pszLocalReadFilename, 
        const char * pszRemoteWritePathName);

    /*********************************************************
     * 将HTTP服务器文件下载到本地
     ********************************************************/
    bool DownloadFileByRemotePath(
        const char * pszLocalWriteFileName, 
        const char * pszRemoteReadPathName);

    /*********************************************************
     * 将本地文件上传到HTTP服务器
     ********************************************************/
    bool UploadFileByRemoteURL(
        const char * pszLocalReadFilename, 
        const char * pszRemoteURL);

    /*********************************************************
     * 将HTTP服务器文件下载到本地
     ********************************************************/
    bool DownloadFileByRemoteURL(
        const char * pszLocalWriteFileName, 
        const char * pszRemoteURL);

    /*********************************************************
     * 获知最近一次操作的错误码
     ********************************************************/
    int GetLastErrorCode();

    /*********************************************************
     * 设置用户名
     ********************************************************/
    bool SetUserName(const char * pszUserName);

    /*********************************************************
     * 设置密码
     ********************************************************/
    bool SetPassword(const char * pszPassword);

    /*********************************************************
     * 设置用户名
     ********************************************************/
    bool SetUserName(const std::string & strUserName);

    /*********************************************************
     * 设置密码
     ********************************************************/
    bool SetPassword(const std::string & strPassword);

private:
    /*********************************************************
     * 获知系统在当前线程的最后一次错误信息
     ********************************************************/
    int GetSystemErrno();

    /*********************************************************
     * 发送数据
     ********************************************************/
    bool Send(const char * pszData, unsigned int uiDataLen);

    /*********************************************************
     * 接收数据
     ********************************************************/
    int Recv(char * pszBuffer, unsigned int uiBufferLen);

    /*********************************************************
     * 解析连接HTTP服务器的URL
     ********************************************************/
    bool ParseRemoteURL(
        const char * pszRemoteURL, std::string & strRemotePathName, 
        std::string & strHostName, unsigned short & usHostPort);

    /*********************************************************
     * 解析URL并连接HTTP服务器
     ********************************************************/
    bool ParseAndConnectRemoteURL(
        const char * pszRemoteURL, std::string & strRemotePathName, 
        std::string & strHostName, unsigned short & usHostPort);

    /*********************************************************
     * 组合出上传请求
     ********************************************************/
    bool GenerateNormalUploadRequest(
        std::string & strRequestCmd, 
        const char * pszRemoteWritePathName);

    /*********************************************************
     * 组合出下载请求
     ********************************************************/
    bool GenerateNormalDownloadRequest(
        std::string & strRequestCmd, 
        const char * pszRemoteReadPathName);

    /*********************************************************
     * 组合出带认证信息的上传请求
     ********************************************************/
    bool GenerateAuthInfoUploadRequest(
        std::string & strRequestCmd, 
        const char * pszRemoteWritePathName, 
        const std::string & strRealm, 
        const std::string & strNonce, 
        const std::string & strResponse);

    /*********************************************************
     * 组合出带认证信息的下载请求
     ********************************************************/
    bool GenerateAuthInfoDownloadRequest(
        std::string & strRequestCmd, 
        const char * pszRemoteReadPathName, 
        const std::string & strRealm, 
        const std::string & strNonce, 
        const std::string & strResponse);

    /*********************************************************
     * 跳到下一行
     ********************************************************/
    bool PassToNextLine(const char *& pszData);

    /*********************************************************
     * 获取Key对应的值
     ********************************************************/
    bool GetValueOfKey(
        const char *& pszData, 
        const char * pszKey, 
        std::string & strValue);

    /*********************************************************
     * 解析出realm和nonce
     ********************************************************/
    bool ParseRealmAndNonce(
        const char * pszRecvResponse, 
        std::string & strRealm, std::string & strNonce);

    /*********************************************************
     * 发送带认证信息的上传请求
     ********************************************************/
    bool SendRequestWithAuthInfo(
        const char * pszRemotePathName, bool bUploadRequest, 
        const std::string & strRealm, const std::string & strNonce);

    /*********************************************************
     * 解析并处理接收到的上传响应
     ********************************************************/
    bool ParseAndHandleResponse(
        const char * pszRecvResponse, int uiRecvResponseLen, 
        const char *& pszData, unsigned int & uiDataLen, 
        const char * pszRemotePathName, bool bUploadRequest);

    /*********************************************************
     * 发送上传请求
     ********************************************************/
    bool SendUploadRequestAndHandleResponse(
        const char * pszRemoteWritePathName);

    /*********************************************************
     * 发送下载请求
     ********************************************************/
    bool SendDownloadRequestAndHandleResponse(
        const char * pszRemoteReadPathName, 
        char * pszBuffer, unsigned int uiBufferLen, 
        const char *& pszData, unsigned int & uiDataLen);

private:
    ACE_SOCK_Stream          m_stream;               // 与HTTP服务器通信的套接字
    bool                     m_bSocketInit;          // 是否已经正常初始化网络环境
    std::string              m_strHostName;          // 最近连接的服务器的主机名
    std::string              m_strHostPort;          // 最近连接的服务器的端口号
    unsigned short           m_usHostPort;           // 最近连接的服务器的端口号
    std::string              m_strUserName;          // 最近连接的服务器的用户名
    std::string              m_strPassword;          // 最近连接的服务器的密码
    std::string              m_strFileLength;        // 正在上传的文件的长度

    int                      m_iLastErrorCode;       // 记录最近一次操作的错误码
};

NAMESPACE_CBB_END


#endif // CBB_HTTP_CLIENT_H
