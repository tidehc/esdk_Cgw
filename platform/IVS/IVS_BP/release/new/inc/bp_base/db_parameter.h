#ifndef _db_parameter_h
#define _db_parameter_h

#ifdef WIN32
#include <windows.h>
# define DLLEXPORT __declspec (dllexport)
#else
# define DLLEXPORT
#endif
#include <string>
#include "bp_stdint.h"

 typedef DLLEXPORT enum
{
    DB_PARAM_TYPE_INT    = 0,
    DB_PARAM_TYPE_FLOAT  = 1,
    DB_PARAM_TYPE_STRING = 2,
    DB_PARAM_TYPE_LOB    = 3,
    DB_PARAM_TYPE_BIGNUM = 4,
}DB_PARAM_TYPE;

class DLLEXPORT CDBParam
{
public:
   /**********************************************************************
   * name           : GetType
   * description    : 获取参数类型
   * input          : NA
   * return         : DB_Param_Type
   * remark         : NA
   *********************************************************************/
    DB_PARAM_TYPE GetType(){return m_type;}

   /**********************************************************************
   * name           : AsInt
   * description    : 获取int类型参数
   * input          : NA
   * return         : int
   * remark         : NA
   *********************************************************************/
    int AsInt(){return u.m_iValue;}

   /**********************************************************************
   * name           : AsFloat
   * description    : 获取Float类型参数
   * input          : NA
   * return         : float
   * remark         : NA
   *********************************************************************/
    float AsFloat(){return u.m_fValue;}

   /**********************************************************************
   * name           : AsBignum
   * description    : 获取大数据
   * input          : NA
   * return         : double
   * remark         : NA
   *********************************************************************/
    double AsBignum(){return u.m_dValue;}

   /**********************************************************************
   * name           : AsString
   * description    : 获取String类型参数
   * input          : NA
   * return         : string
   * remark         : NA
   *********************************************************************/
    std::string AsString(){return m_strValue;}

   /**********************************************************************
   * name           : AsLob
   * description    : 获取Lob类型参数
   * input          : NA
   * return         : string
   * remark         : NA
   *********************************************************************/
    std::string AsLob(){return m_strValue;}

   /**********************************************************************
   * name           : SetInt
   * description    : 设置int类型参数
   * input          : int
   * return         : NA
   * remark         : NA
   *********************************************************************/
    void SetInt(int iValue){m_type = DB_PARAM_TYPE_INT; u.m_iValue = iValue;}

   /**********************************************************************
   * name           : SetFloat
   * description    : 设置float类型参数
   * input          : float
   * return         : NA
   * remark         : NA
   *********************************************************************/
    void SetFloat(float fValue){m_type = DB_PARAM_TYPE_FLOAT; u.m_fValue = fValue;}

   /**********************************************************************
   * name           : SetFloat
   * description    : 设置float类型参数
   * input          : float
   * return         : NA
   * remark         : NA
   *********************************************************************/
    void SetBignum(double dValue){m_type = DB_PARAM_TYPE_BIGNUM; u.m_dValue = dValue;}

   /**********************************************************************
   * name           : SetString
   * description    : 设置string类型参数
   * input          : string
   * return         : NA
   * remark         : NA
   *********************************************************************/
    void SetString(std::string strValue){m_type = DB_PARAM_TYPE_STRING; m_strValue = strValue;}
   
   /**********************************************************************
   * name           : SetLob
   * description    : 设置Lob类型参数
   * input          : string
   * return         : NA
   * remark         : NA
   *********************************************************************/
    void SetLob(std::string lobValue){m_type = DB_PARAM_TYPE_LOB; m_strValue = lobValue;}

   /**********************************************************************
   * remark         : 兼容接口，仅供MAU使用。其它模块请使用Set***
   **********************************************************************/
   std::string AsLongString(){return AsLob();}
   void SetAsInt(int iValue){return SetInt(iValue);}
   void SetAsString(std::string strValue){return SetString(strValue);}
   void SetAsLongString(std::string strValue){return SetLob(strValue);}
   
   private:
   DB_PARAM_TYPE m_type;
   union {
       int    m_iValue;
       float  m_fValue;
       double m_dValue;
   } u;
   std::string m_strValue; //lob复用该值
};

#endif
