/********************************************************************
	filename	: 	xml.h
	author		:	s00194833
	created		:	2011/12/22	
	description	:	XML解析类
	copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
	history		:	2011/12/22 初始版本
*********************************************************************/
#ifndef CBB_XML_H
#define CBB_XML_H

#include <string>
#include "tinyxml.h"

#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

class EXPORT_DLL CXml
{
public:
    CXml(const char *pszFileName = NULL);
    ~CXml();

private:
    CXml(const CXml &src);

    CXml& operator=(const CXml&);

public:

    /**************************************************************************
    * name       : Load
    * description: 加载一个xml文件
    * input      : encode 编码 TIXML_ENCODING_UNKNOWN,TIXML_ENCODING_UTF8,TIXML_ENCODING_LEGACY
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
    bool Load(TiXmlEncoding encode = TIXML_ENCODING_UNKNOWN);

    /**************************************************************************
    * name       : Load
    * description: 加载一个xml文件
    * input      : pszFileName 加载的文件名
	               encode 编码 TIXML_ENCODING_UNKNOWN,TIXML_ENCODING_UTF8,TIXML_ENCODING_LEGACY
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
	bool Load(const char *pszFileName, TiXmlEncoding encode = TIXML_ENCODING_UNKNOWN);

    /**************************************************************************
    * name       : Parse
    * description: 解析一个xml流到xml文件
    * input      : pszStream xml流
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
	bool Parse(const char *pszStream, TiXmlEncoding encode = TIXML_ENCODING_LEGACY);

    /**************************************************************************
    * name       : Save
    * description: 存储xml文件
    * input      : NA
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : 与Load(const char *pszFileName)配对使用
    **************************************************************************/
    bool Save();

    /**************************************************************************
    * name       : Save
    * description: 存储xml文件
    * input      : pszFileName 存储的文件名
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
	bool Save(const char *pszFileName);

    /**************************************************************************
    * name       : IntoElem
    * description: 进入下一级
    * input      : NA
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
	bool IntoElem();

    /**************************************************************************
    * name       : OutOfElem
    * description: 退出到上一级
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void OutOfElem();

    /**************************************************************************
    * name       : NextElem
    * description: 进入同级下一个元素
    * input      : NA
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
    bool NextElem();

    /**************************************************************************
    * name       : GetRootPos
    * description: 跳至根节点
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void GetRootPos();

    /**************************************************************************
    * name       : AddDeclaration
    * description: 添加xml文件声明部分
    * input      : pszVersion 版本
                   pszEncode 编码方式
                   pszStdLone 独立属性 yes或no
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
    bool AddDeclaration(const char* pszVersion,const char* pszEncode,const char* pszStdLone);

    /**************************************************************************
    * name       : AddElem
    * description: 添加元素
    * input      : pszElemName 待添加的元素名
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
	bool AddElem(const char *pszElemName);

	/**************************************************************************
	* name       : AddElemBeforeCurNode
	* description: 在当前节点前添加元素
	* input      : pszElemName 待添加的元素名
	* output     : NA
	* return     : true - 成功，false - 失败
	* remark     : NA
	**************************************************************************/
	//lint -e429
	bool AddElemBeforeCurNode(const char *pszElemName);

    /**************************************************************************
    * name       : AddChildElem
    * description: 添加子元素
    * input      : pszElemName 待添加的子元素名
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
	bool AddChildElem(const char *pszElemName);

    /**************************************************************************
    * name       : SetElemValue
    * description: 设定当前元素值
    * input      : pszElemValue 待设定的元素值
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
	bool SetElemValue(const char* pszElemValue);
    
    /**************************************************************************
    * name       : SetElemValueCDATA
    * description: 设定当前元素CDATA,  <![CDATA[ElemValue]]> 封装
    * input      : pszElemValue 待设定的元素值
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
    bool SetElemValueCDATA(const char* pszElemValue);
    
    /**************************************************************************
    * name       : GetElem
    * description: 获取当前元素
    * input      : NA
    * output     : NA
    * return     : const char*
    * remark     : NA
    **************************************************************************/
	const char* GetElem();

    /**************************************************************************
    * name       : GetElemValue
    * description: 获取当前元素值
    * input      : NA
    * output     : NA
    * return     : const char*
    * remark     : NA
    **************************************************************************/
	const char* GetElemValue() const;

    /**************************************************************************
    * name       : FindElem
    * description: 根据相对路径查找元素
    * input      : pszElemName 待查找的元素名
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
	bool FindElem(const char *pszElemName);

	bool FindElemValue(const char *pszElemName);

    /**************************************************************************
    * name       : ModifyElemName
    * description: 修改元素
    * input      : pszElemName 元素名
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void ModifyElemName(const char*pszElemName);

    /**************************************************************************
    * name       : ModifyElemValue
    * description: 修改元素值
    * input      : pszElemValue 元素值
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void ModifyElemValue(const char*pszElemValue);

    /**************************************************************************
    * name       : RemoveElem
    * description: 删除元素
    * input      : NA
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
    bool RemoveElem();

    /**************************************************************************
    * name       : AddAttribute
    * description: 添加元素属性
    * input      : pszAttriName 待添加的属性名
                   pszAttriValue 对应的属性值
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void AddAttribute(const char*pszAttriName,const char* pszAttriValue);

    /**************************************************************************
    * name       : AddChildAttrib
    * description: 添加子元素属性
    * input      : pszAttriName 元素属性名
                   pszAttriValue 元素属性值
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
	bool AddChildAttribute(const char*pszAttriName,const char* pszAttriValue);

    /**************************************************************************
    * name       : GetAttributeValue
    * description: 获得元素属性值
    * input      : pszAttriName 元素的属性名
    * output     : NA
    * return     : const char* 元素的属性值
    * remark     : NA
    **************************************************************************/
	const char* GetAttributeValue(const char*pszAttriName) const;

    /**************************************************************************
    * name       : RemoveAttribute
    * description: 删除元素属性
    * input      : pszAttriName 元素的属性名
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void RemoveAttribute(const char*pszAttriName);

    /**************************************************************************
    * name       : ModifyAttributeValue
    * description: 修改元素属性值
    * input      : pszAttriName 待修改的属性名
                   pszAttriValue 对应的属性值
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void ModifyAttributeValue(const char*pszAttriName,const char* pszAttriValue);

    /**************************************************************************
    * name       : GetXMLStream
    * description: 获取xml流
    * input      : NA
    * output     : nXMLStreamLength
    * return     : const char* xml流
    * remark     : NA
    **************************************************************************/
    const char*  GetXMLStream(unsigned int &uiXMLStreamLength);

    /**************************************************************************
    * name       : SetChildData
    * description: 添加子元素值
    * input      : pValue 元素值
    * output     : NA
    * return     : true - 成功，false - 失败
    * remark     : NA
    **************************************************************************/
	bool SetChildData(const char *pszValue);

    /**************************************************************************
    * name       : Print
    * description: 在Win32平台打印出xml文件
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void Print()const;

    /**************************************************************************
    * name       : FindElemEx
    * description: 根据节点绝对路径获取节点
    * input      : XML节点的路径
    * output     : NA
    * return     : 是否成功
    * remark     : NA
    **************************************************************************/
    bool FindElemEx(const char *pszElemPath);
	bool MkAndFindElemEx(const char *pszElemPath);

    /**************************************************************************
    * name       : SetElemValueEx
    * description: 根据节点路径设置节点内容
    * input      : pszElemPath XML节点的路径
	               pszElemValue XML节点值
    * output     : NA
    * return     : 是否成功
    * remark     : NA
    **************************************************************************/	
	bool SetElemValueEx(const char *pszElemPath,const char *pszElemValue);
	bool MkAndSetElemValueEx(const char *pszElemPath,const char *pszElemValue);

    /**************************************************************************
    * name       : GetElemValueEx
    * description: 根据节点路径获取节点内容
    * input      : pszElemPath XML节点的路径
	               pszElemValue XML节点值
    * output     : NA
    * return     : 是否成功
    * remark     : NA
    **************************************************************************/
	const char* GetElemValueEx(const char* pszElemPath);
    // 设置基准节点
    bool SetBaseElem(const char* pszElemPath = NULL);
    bool GetBasePos();

	/**************************************************************************
    * name       : NextElemEx
    * description: 往下取count个节点
    * input      : count XML节点个数
    * output     : NA
    * return     : 是否成功
    * remark     : NA
    **************************************************************************/
	bool NextElemEx(int count);
	bool MkAndNextElemEx(int count);

	/**************************************************************************
    * name       : SetElemValueCDATAEx
    * description: 根据节点路径设置节点内容(CDATA)
    * input      : pszElemPath XML节点的路径
	               pszElemValue XML节点值
    * output     : NA
    * return     : 是否成功
    * remark     : NA
    **************************************************************************/	
	bool SetElemValueCDATAEx(const char *pszElemPath,const char *pszElemValue);
	void ModifyElemValueCDATA(const char *pszElemValue);
		
	/**************************************************************************
    * name       : SetCondenseWhiteSpace
    * description: 空格是否被简化
    * input      : bCondenseWhiteSpace true 为要简化，false为不简化
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/	
	static bool SetCondenseWhiteSpace(bool bCondenseWhiteSpace);
	static bool IsWhiteSpaceCondensed();


private:
	TiXmlDocument       *m_pXMlDoc;         //xml文件指针
    TiXmlNode           *m_pXmlNode;        //xml节点指针
	TiXmlElement        *m_pXmlElem;        //xml元素指针
    const char          *m_pszFileName;     //xml文件文件名指针
    //TiXmlPrinter        m_XmlPrinter;       //xml文件打印对象
	std::string				m_strStream;
    TiXmlNode           *m_pBaseXmlNode;    //xml基准节点指针
    TiXmlElement        *m_pBaseXmlElem;    //xml基准元素指针

	static bool         m_bFistInit;        //是否是第一次初始化
	static bool         m_bCondenseWhiteSpace; //是否需要简化空格
};

NAMESPACE_CBB_END

#endif//CBB_XML_H
