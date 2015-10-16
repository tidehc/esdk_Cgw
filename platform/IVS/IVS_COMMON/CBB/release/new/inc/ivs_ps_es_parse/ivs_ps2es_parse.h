/*****************************************************************************
  Copyright (C), 2014-2020, Huaweisymantec Tech. Co., Ltd.
  FileName: ivs_ps2es_parse.h
  Author: yWX153026
  Version : v0.1
  Date:
  Description: Header File of ivs_ps2es_parse.cpp
  Function List:
  History:
*****************************************************************************/
#ifndef IVS_PS2ES_PARSE_H
#define IVS_PS2ES_PARSE_H

#include "ivs_ps_head.h"
#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

class EXPORT_DLL CPs2EsParse
{
public:
    CPs2EsParse();
    ~CPs2EsParse();

    /**************************************************************************
    * name       : parsePSPack
    * description: 把一帧PS数据转成ES数据
    * input      : pFrameDate PS码流信息
                   iFrameLength PS码流长度
    * output     : pVideoData 解析得到视频数据
                   uiVideoLength 解析得到视频数据长度
                   pAudioData 解析得到音频数据
                   uiAudioLength 解析得到音频数据长度
                   pPSHeader 解析完成之后PS包头的数据
    * return     : 成功：IVS_SUCCESS 失败：IVS_FAIL
    * remark     : 方法
    **************************************************************************/
    int parsePSPack(const char* pFrameDate, int iFrameLength, char*& pVideoData, 
        unsigned int& uiVideoLength, char*& pAudioData, unsigned int& uiAudioLength, LPPS_HEADER_TOTALL_S& pPSHeader);

private:
    /**************************************************************************
    * name       : parsePSHeader
    * description: 解析PS头
    * input      : pPSHeader PS码流信息
                   iLeftLength 剩余码流长度
    * output     : iLeftLength 输出解析完之后还剩下码流长度
                   pPSHeaderInput 指向解析当前头后，指向头后下一个字节
    * return     : 成功：IVS_SUCCESS 失败：IVS_FAIL
    * remark     : 方法
    **************************************************************************/
    int parsePSHeader(const char*& pPSHeaderInput, int& iLeftLength);

    /**************************************************************************
    * name       : parsePSSystemHeader
    * description: 解析ps流，系统头
    * input      : pSystemHeader PS码流信息
                   iLeftLength 剩余码流长度
    * output     : iLeftLength 解析完后，余下的码流长度
                   pSystemHeaderInput 解析完当前头后，指向下一个未处理的字节
    * return     : 成功：IVS_SUCCESS 失败：IVS_FAIL
    * remark     : 方法
    **************************************************************************/
    int parsePSSystemHeader(const char*& pSystemHeaderInput, int& iLeftLength);

    /**************************************************************************
    * name       : parsePSPESPack
    * description: 解析PES流
    * input      : pPESHeader PS码流信息
                   iLeftLength 剩余码流长度
    * output     : iLeftLength 解析完后，余下的码流长度
                   pPESHeaderInput 解析完当前头后，指向下一个未处理的字节
    * return     : 成功：IVS_SUCCESS 失败：IVS_FAIL
    * remark     : 方法
    **************************************************************************/
    int parsePSPESPack(const char*& pPESHeaderInput, int& iLeftLength);

    /**************************************************************************
    * name       : getPtsDtsValue
    * description: 获取PES头中PTS和DTS数据
    * input      : pPESParam 指向PTS数据的指针
                   ucPtsDtsFlags PTS DTS标志位
    * output     : NA
    * return     : 成功：IVS_SUCCESS 失败：IVS_FAIL
    * remark     : 方法
    **************************************************************************/
    int getPtsDtsValue(char*& pPESParam, unsigned char ucPtsDtsFlags);

    /**************************************************************************
    * name       : NtoH
    * description: 把网络字节序转成主机字节序
    * input      : pValue 需要转换的开始字节地址
                   ucLen 需要转换的长度
    * output     : NA
    * return     : NA
    * remark     : 方法
    **************************************************************************/
    void NtoH(char* pValue, unsigned char ucLen);

private:
    char* m_pESVideoData;           //解析得到的视频数据
    char* m_pESAudioData;           //解析得到的音频数据
    unsigned int m_usESVidoeLength; //解析得到的视频数据长度
    unsigned int m_usESAudioLength; //解析得到的音频数据长度
    PS_HEADER_TOTALL_S m_PSHeaderInfo;
};

NAMESPACE_CBB_END

#endif // IVS_PS2ES_PARSE_H