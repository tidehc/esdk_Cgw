#ifndef _db_result_h
#define _db_result_h

#include "db_parameter.h"

class DLLEXPORT  IDBResultSet
{
public:
    IDBResultSet(){};
    virtual ~IDBResultSet(){};

public:
    /**********************************************************************
    * name           : Close
    * description    : 关闭数据库流
    * input          : NA
    * return         : NA
    * remark         : NA
    *********************************************************************/
    virtual void Close(void) = 0;

    /**********************************************************************
    * name           : HasNext
    * description    : 是否到数据流结尾
    * input          : NA
    * return         : bool
    * remark         : NA
    *********************************************************************/
    virtual bool HasNext(void) = 0;

    /**********************************************************************
    * name           : GetInt
    * description    : 从流中输出一个int值
    * input          : NA
    * return         : int
    * remark         : NA
    *********************************************************************/
    virtual int GetInt(void) = 0;

    /**********************************************************************
    * name           : GetFloat
    * description    : 从流中输出一个float值
    * input          : NA
    * return         : float
    * remark         : NA
    *********************************************************************/
    virtual float GetFloat(void) = 0;

    /**********************************************************************
    * name           : GetBignum
    * description    : 从流中输出一个double值
    * input          : NA
    * return         : double
    * remark         : NA
    *********************************************************************/
    virtual double GetBignum(void) = 0;

    /**********************************************************************
    * name           : GetString
    * description    : 从流中输出一个string值
    * input          : NA
    * return         : string
    * remark         : NA
    *********************************************************************/
    virtual std::string GetString(void) = 0;
    
    /**********************************************************************
    * name           : GetLob
    * description    : 从流中输出一个lob值
    * input          : NA
    * return         : string
    * remark         : NA
    *********************************************************************/
    virtual std::string GetLob(void) = 0;

    /**********************************************************************
    * remark         : 兼容接口，仅供MAU使用。其它模块请使用Get***
    **********************************************************************/
    int AsInt(void){return GetInt();}
    std::string AsString(void){return GetString();}
    std::string AsLongString(void){return GetLob();}
};

#endif
