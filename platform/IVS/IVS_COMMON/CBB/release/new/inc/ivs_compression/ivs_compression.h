/********************************************************************
filename    :    ivs_compression.h
author      :    yWX150774
created     :    2012/12/26
description :    压缩、解压缩文件接口封装
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/12/26 初始版本
*********************************************************************/

#ifndef CBB_COMPRESSION_H
#define CBB_COMPRESSION_H


#include "ivs_namespace.h"

#include "zlib.h"

NAMESPACE_CBB_BEGIN

/*********************************************
 * 压缩、解压缩文件类, 暂时只提供zip格式
*********************************************/
class EXPORT_DLL CCompression
{
public:
    enum enCompressionLevel
    {
        ZIP_COMPRESS_NO      = Z_NO_COMPRESSION,     // 不压缩
        ZIP_COMPRESS_FAST    = Z_BEST_SPEED,         // 最快
        ZIP_COMPRESS_BEST    = Z_BEST_COMPRESSION,   // 最优
        ZIP_COMPRESS_DEFAULT = Z_DEFAULT_COMPRESSION // 默认
    };

public:
    /*****************************************************************
     * zip格式压缩文件
     * pszSrcFilename: 待压缩文件名
     * pszDstFilename: 压缩输出文件名 (为NULL则自动生成文件名)
     * enZipLevel    : 压缩等级
     * 返回值         : 压缩是否成功
     ****************************************************************/
    static bool Zip(
        const char * pszSrcFilename, 
        const char * pszDstFilename, 
        enum enCompressionLevel enZipLevel = ZIP_COMPRESS_DEFAULT
    );

    /*****************************************************************
     * zip格式解压缩文件
     * pszSrcFilename: 待解压缩文件名
     * pszDstFilename: 解压缩输出文件名 (为NULL时, 直接返回失败)
     * 返回值         : 解压缩是否成功
     ****************************************************************/
    static bool UnZip(
        const char * pszSrcFilename, 
        const char * pszDstFilename
    );

    /*****************************************************************
     * zip格式压缩文件
     * pSrcFile  : 待压缩文件句柄    (传入句柄不会在此释放)
     * pDstFile  : 压缩输出文件句柄   (传入句柄不会在此释放)
     * enZipLevel: 压缩等级
     * 返回值     : 压缩是否成功
     ****************************************************************/
    static bool Zip(
        FILE * pSrcFile, 
        FILE * pDstFile, 
        enum enCompressionLevel enZipLevel = ZIP_COMPRESS_DEFAULT
    );

    /*****************************************************************
     * zip格式解压缩文件
     * pSrcFile: 待解压缩文件句柄    (传入句柄不会在此释放)
     * pDstFile: 解压缩输出文件句柄   (传入句柄不会在此释放)
     * 返回值   : 解压缩是否成功
     ****************************************************************/
    static bool UnZip(
        FILE * pSrcFile, 
        FILE * pDstFile
    );

    /*****************************************************************
     * 获知zip格式压缩后的数据缓存长度
     * pszSrcData      : 待压缩的数据
     * uiSrcDataLength : 待压缩的数据的大小
     * uiNeedBufferSize: 输出数据所需的缓存大小 (输出参数)
     * enZipLevel      : 压缩等级
     * 返回值           : 压缩是否成功
     ****************************************************************/
    static bool GetZipBufferLength(
        const unsigned char * pszSrcData, 
        unsigned int uiSrcDataLength, 
        unsigned int & uiNeedBufferSize, 
        enum enCompressionLevel enZipLevel = ZIP_COMPRESS_DEFAULT
    );

    /*****************************************************************
     * 获知zip格式解压缩后的数据缓存长度
     * pszSrcData      : 待解压缩的数据
     * uiSrcDataLength : 待解压缩的数据的大小
     * uiNeedBufferSize: 输出数据所需的缓存大小 (输出参数)
     * 返回值           : 解压缩是否成功
     ****************************************************************/
    static bool GetUnZipBufferLength(
        const unsigned char * pszSrcData, 
        unsigned int uiSrcDataLength, 
        unsigned int & uiNeedBufferSize
    );

    /*****************************************************************
     * zip格式压缩数据
     * pszSrcData      : 待压缩的数据
     * uiSrcDataLength : 待压缩的数据的大小
     * pszDstData      : 输出数据   (输出参数)
     * uiDstDataLength : 输入参数时: 输出缓存大小; 输出参数时: 输出数据的大小
     * enZipLevel      : 压缩等级
     * 返回值           : 压缩是否成功
     ****************************************************************/
    static bool Zip(
        const unsigned char * pszSrcData, 
        unsigned int uiSrcDataLength, 
        unsigned char * pszDstData, 
        unsigned int & uiDstDataLength, 
        enum enCompressionLevel enZipLevel = ZIP_COMPRESS_DEFAULT
    );

    /*****************************************************************
     * zip格式解压缩数据
     * pszSrcData      : 待解压缩的数据
     * uiSrcDataLength : 待解压缩的数据的大小
     * pszDstData      : 输出数据   (输出参数)
     * uiDstDataLength : 输入参数时: 输出缓存大小, 输出参数时: 输出数据的大小
     * 返回值           : 解压缩是否成功
     ****************************************************************/
    static bool UnZip(
        const unsigned char * pszSrcData, 
        unsigned int uiSrcDataLength, 
        unsigned char * pszDstData, 
        unsigned int & uiDstDataLength
    );

private:
    /*****************************************************************
     * 尝试zip格式压缩数据
     * pszSrcData      : 待压缩的数据
     * uiSrcDataLength : 待压缩的数据的大小
     * pszDstData      : 输出数据, 为NULL时, 只获知输出数据所需的缓存大小
     * uiDstDataLength : 输出数据所需的缓存大小
     * enZipLevel      : 压缩等级
     * 返回值           : 压缩是否成功
     ****************************************************************/
    static bool TryZip(
        const unsigned char * pszSrcData, 
        unsigned int uiSrcDataLength, 
        unsigned char * pszDstData, 
        unsigned int & uiDstDataLength, 
        enum enCompressionLevel enZipLevel = ZIP_COMPRESS_DEFAULT
    );

    /*****************************************************************
     * 尝试zip格式解压缩数据
     * pszSrcData      : 待解压缩的数据
     * uiSrcDataLength : 待解压缩的数据的大小
     * pszDstData      : 输出数据, 为NULL时, 只获知输出数据所需的缓存大小
     * uiDstDataLength : 输出数据所需的缓存大小
     * 返回值           : 解压缩是否成功
     ****************************************************************/
    static bool TryUnZip(
        const unsigned char * pszSrcData, 
        unsigned int uiSrcDataLength, 
        unsigned char * pszDstData, 
        unsigned int & uiDstDataLength
    );

private:
    static const unsigned int ZIP_COMPRESSION_MAX_SIZE; // 每次zip压缩文件的最大大小
};

NAMESPACE_CBB_END


#endif // CBB_COMPRESSION_H
