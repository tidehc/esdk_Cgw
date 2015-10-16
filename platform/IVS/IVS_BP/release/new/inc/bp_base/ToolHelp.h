/********************************************************************
filename    :    ToolsMsgMethod.h
author      :    yKF70958
created     :    2012/04/12
description :    消息ID帮助工具
copyright   :    Copyright (C) 2011-2015
history     :    2012/04/12 初始版本
*********************************************************************/
#ifndef TOOL_HELP_H
#define TOOL_HELP_H

#include <string.h>
#include <map>
#include <list>
#include <vector>
#include <bp_os.h>
#include "ace_header.h"
#include "bp_def.h"

NAMESPACE_BP_BEGIN  //namespace begin

#define  STR_LENGTH_SIZE	20
#define	 INT_FORMAT_D		10
#define  INT_FORMAT_X		16
#define  STR_FORMAT_D		"%d"
#define  STR_FORMAT_X		"%x"
static const int MAX_INT    = 2147483647;
static const signed int MIN_INT    = -2147483647-1;

typedef std::map<std::string,std::string> IvspNodeMap;
typedef IvspNodeMap::iterator IvspNodeMapIter;

class EXPORT_DLL CToolHelp 
{
public:
    /***********************************************************************************
    * name       : IntToStr
    * description: 将整数转化为字符串
    * input      : iValue:要转化的整型数值; bHex:是否是十六进制整型数
    * output     : NA
    * return     : 转化后的字符串
    * remark     : NA
    ***********************************************************************************/
    static std::string IntToStr ( int iValue, bool bHex = false );

	/***********************************************************************************
    * name       : StrToInt
    * description: 将整数转化为字符串
    * input      : strSrc:要转化的字符串; bHex:是否是十六进制整型数
    * output     : NA
    * return     : 转化后的整型数
    * remark     : NA
    ***********************************************************************************/
    static int StrToInt ( const std::string& strSrc, bool bHex = false );
    
    //返回一个结构体，其数据（比如日期，星期）可能是非法的！若调用mktime（sttm&），则可以返回合法的数值。
    static struct tm ymdhmsToTime(const std::string& strDt);
   
    //返回一个time_t，在数值上是本地时间（参数）对应的格林威治时间相对（1970-1-1 00:00:00GMT）的秒数。
    //如果要生成本地的时间，需要调用localtime（）
    static time_t ymdhmsToTime_t(const std::string& strDt);
    //unit=0(日)1时2分，4秒，无3可能是笔误！
    static void timeadd(struct tm& stTime,int unit,int num);

    /***********************************************************************************
    * name       : IsLegalParam
    * description: 判断xml参数是否超过最大长度
    * input      : pcParam:xml结点参数字符串值; uiMaxLen:数据库中参数长度
    * output     : NA
    * return     : IVS_SUCCEED - 合法，IVS_FAIL - 非法
    * remark     : NA
    ***********************************************************************************/
    static int IsLegalParam(const char* pcParam, unsigned int uiMaxLen);

    /***********************************************************************************
    * name       : IsLegalint
    * description: 判断xml参数是否为int类型
    * input      : pcParam:xml结点参数字符串值; iMax:参数最大值
    * output     : NA
    * return     : IVS_SUCCEED - 合法，IVS_FAIL - 非法
    * remark     : NA
    ***********************************************************************************/
    static int IsLegalInt(const char* pcParam, int iMax = MAX_INT);

    static bool IsLegalUIntR(unsigned int &Result,const char* pcParam, unsigned int iMax = -1,unsigned int iMin = 0);
    static bool IsLegalSIntR(int &Result,const char* pcParam,int iMax =MAX_INT , int iMin = MIN_INT);

    /***********************************************************************************
    * name       : IsLegalEnum
    * description: 判断xml参数是否为合法的枚举类型值
    * input      : pcParam:xml结点参数字符串值; iLow:低值; iHigh:高值; 
    * output     : NA
    * return     : IVS_SUCCEED - 合法，IVS_FAIL - 非法
    * remark     : NA
    ***********************************************************************************/
    static int IsLegalEnum(const char* pcParam, int iLow, int iHigh);
    
    /***********************************************************************************
    * name       : IsLegalDate
    * description: 判断pcParaml参数是否为合法的8位日期类型值
    * input      : pcParam 参数字符串值; 
    * output     : NA
    * return     : IVS_SUCCEED - 合法，IVS_FAIL - 非法
    * remark     : NA
    ***********************************************************************************/
    static int IsLegalDate(const char* pcParam);

    /***********************************************************************************
    * name       : IsLegalTime
    * description: 判断pcParaml参数是否为合法的6位时间类型值
    * input      : pcParam 参数字符串值; 
    * output     : NA
    * return     : IVS_SUCCEED - 合法，IVS_FAIL - 非法
    * remark     : NA
    ***********************************************************************************/
    static int IsLegalTime(const char* pcParam);

    /***********************************************************************************
    * name       : IsLegalDateTime
    * description: 判断pcParaml参数是否为合法的14位日期时间类型值
    * input      : pcParam 参数字符串值; 
    * output     : NA
    * return     : IVS_SUCCEED - 合法，IVS_FAIL - 非法
    * remark     : NA
    ***********************************************************************************/
    static int IsLegalDateTime(const char* pcParam);
    static int IsLegalDateTimeMs(const char* pcParam);

    /***********************************************************************************
    * name       : SplitString
    * description: src被dlim分割成为slRet列表。
    * input      : src,dlim; 
    * output     : slRet
    * return     : NA
    * remark     : NA 限ascii字符,输入参数src不能是输出参数slRet的元素，因为slRet在函数执行过程中会被覆盖写，从而破坏输入参数的值。
    ***********************************************************************************/
    static void SplitString(const char* src,const char* dlim,std::vector<std::string>& slRet );
    static void SplitString(const std::string &src,const std::string &dlim,std::vector<std::string>& slRet );

    /***********************************************************************************
    * name       : ToUpper
    * description: src转换成大写。
    * input      : src; 
    * output     : 
    * return     : 返回大写的string
    * remark     : NA
    ***********************************************************************************/
    static std::string ToUpper(const std::string& strSrc);

    /***********************************************************************************
    * name       : LTrim
    * description: strSrc去除左空格。
    * input      : src; 
    * output     : 
    * return     : 返回大写的string
    * remark     : NA
    ***********************************************************************************/
    static void LTrim(std::string& strSrc);

    /***********************************************************************************
    * name       : LTrim
    * description: strSrc去除右空格。
    * input      : src; 
    * output     : 
    * return     : 返回大写的string
    * remark     : NA
    ***********************************************************************************/
    static void RTrim(std::string& strSrc);

    /***********************************************************************************
    * name       : LTrim
    * description: strSrc去除左右空格。
    * input      : src; 
    * output     : 
    * return     : 返回大写的string
    * remark     : NA
    ***********************************************************************************/
    static std::string Trim(std::string& strSrc);

    /************************************************************************/
    /*                                                                      */
    /************************************************************************/
    static void ReadIvspHeader(const std::string& strXmlData,IvspNodeMap& outHeaderMap);

    static void ReadIvspHeader(const char* pszXmlData,IvspNodeMap& outHeaderMap);

    static void GetValueFromMap(IvspNodeMap& inHeaderMap,const std::string& strKey, std::string& strOutValue);
    /**************************************************************************
    * name       : ReadNextXmlNode
    * description: 顺序读取xml的下个节点
    * input      :  pszXmlData xml数据
                    pszStartTag 开始标签
                    pszEndTag 结束标签
                    pszTagName 节点名称（map中的key）
    * output     : outHeaderMap 输出的节点键值对
    * return     : xml结束节点结束后面的指针
    * remark     : NA
    **************************************************************************/    
    static const char* ReadNextXmlNode(const char* pszXmlData,const char* pszStartTag,const char* pszEndTag,const char* pszTagName,IvspNodeMap& outHeaderMap);
};

NAMESPACE_BP_END   // namespace end

#endif //CAPABILITY_MGR_H
