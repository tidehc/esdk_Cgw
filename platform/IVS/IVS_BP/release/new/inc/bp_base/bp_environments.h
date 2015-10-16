/********************************************************************
filename    :    bp_enviroments.h
author      :    w90004666
created     :    2012/9/18
description :	 获取IVS环境变量
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/18 初始版本
*********************************************************************/

#ifndef _bp_enviroments_h
#define _bp_enviroments_h

#include <string>
#include "bp_namespace.h"
#include "bp_def.h"

NAMESPACE_BP_BEGIN

	extern "C" int EXPORT_DLL get_home_directory(std::string& dir);
	extern "C" void EXPORT_DLL set_home_directory(std::string dir);
	extern "C" int EXPORT_DLL get_bin_directory(std::string& dir);
	extern "C" int EXPORT_DLL get_log_directory(std::string& dir);
	extern "C" int EXPORT_DLL get_config_directory(std::string& dir);
	extern "C" int EXPORT_DLL get_service_directory(std::string& dir);
	extern "C" int EXPORT_DLL get_protocol_directory(std::string& dir);
	extern "C" int EXPORT_DLL get_lib_directory(std::string& dir);

	/************************************************************************
	* 修改Linux下进程打开的文件上限，只对Linux有效
	*************************************************************************/
	extern "C" void EXPORT_DLL set_open_files_limits(int value);
	extern "C" int EXPORT_DLL get_open_files_limits();

	/************************************************************************
	* 将NULL结束的字符串<src>中的环境变量替换为环境变量的值，保存在<dst>中返回。
	* 成功返回0，失败返回错误码.
	*************************************************************************/
	extern int replace_env_var(const char *src, std::string& dst);

    /*************************************************************************
    * FUNCTION: time_t str2time()
    * DESCRIPTION: 形如20090629182030的时间串转换成以秒为单位的日历时间,
    *              即自国际标准时间公元1970年1月1日00:00:00以来经过的秒数。
    * INPUT :  const char *pStr -- 时间字符串
    * OUTPUT:
    *      自国际标准时间公元1970年1月1日00:00:00以来经过的秒数
    *      只支持"YYYYMMDDhhmmss"格式的时间转换
    *************************************************************************/
    extern time_t EXPORT_DLL str2time(const char *pStr);

    /**************************************************************************
    * 支持"YYYYMMDDhhmmss"，"YYYYMMDDThhmmssZ"和"YYYY-MM-DD hh:mm:ss"
    * 三种时间格式，默认采用UTC表示法
    *************************************************************************/
    extern EXPORT_DLL char* time2str(char *pDestBuf, int nbuflen, const time_t t, const char *pFormat = "YYYYMMDDhhmmss");

    enum TIME_STRUCT_CONST
    {
        UTILITY_YEAR_LEN   = 4,   //型如2010的长度
        UTILITY_MDHMS_LEN  = 2,   //月日时分秒所占长度
        CBB_YEAR_BASE     = 1900, //系统时间原点
        CBB_INVALID_TIME  = -1,   //无效时间
    };

    #pragma pack(push, 1)
    
    //型式为20101208010108的时间结构
    struct TIME_YYYYMMDDH24MISS
    {
        char  year[UTILITY_YEAR_LEN];
        char month[UTILITY_MDHMS_LEN];
        char   day[UTILITY_MDHMS_LEN];
        char  hour[UTILITY_MDHMS_LEN];
        char   min[UTILITY_MDHMS_LEN];
        char   sec[UTILITY_MDHMS_LEN];
    };

    #pragma pack(pop)

    // 提供64位字长的字节序转换宏
    #if __BYTE_ORDER == __LITTLE_ENDIAN || defined(WIN32)
    #define BP_ntohll(net, host) \
    (\
        (host) \
             = \
        (\
            (((net) >> 56) & 0x00000000000000ffLL) | \
            (((net) >> 40) & 0x000000000000ff00LL) | \
            (((net) >> 24) & 0x0000000000ff0000LL) | \
            (((net) >>  8) & 0x00000000ff000000LL) | \
            (((net) <<  8) & 0x000000ff00000000LL) | \
            (((net) << 24) & 0x0000ff0000000000LL) | \
            (((net) << 40) & 0x00ff000000000000LL) | \
            (((net) << 56) & 0xff00000000000000LL)\
        )\
    )
    #define BP_htonll(host, net) BP_ntohll(host, net)
    #else
    #define BP_ntohll(net, host) (host) = (net)
    #define BP_htonll(host, net) (net) = (host)
    #endif

NAMESPACE_BP_END

#endif //_bp_enviroments_h

