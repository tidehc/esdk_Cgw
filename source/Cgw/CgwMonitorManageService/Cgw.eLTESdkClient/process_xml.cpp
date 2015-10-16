#include "stdafx.h"
#include "process_xml.h"
#include "securec.h"


bool CXml::m_bFistInit = true;
bool CXml::m_bCondenseWhiteSpace = true;

CXml::CXml(const char *pszFileName)
    : m_pXMlDoc(NULL)
    , m_pXmlNode(NULL)
    , m_pXmlElem(NULL)
    , m_pszFileName(pszFileName)
    , m_strStream()
    , m_pBaseXmlNode(NULL)
    , m_pBaseXmlElem(NULL)
{
	// 取消空格缩进,该设置只会在第一次使用xml的时候进来
	if (m_bFistInit)
	{
		TiXmlBase::SetCondenseWhiteSpace(m_bCondenseWhiteSpace);
		m_bFistInit = false;
	}	
}

CXml::~CXml()
{
    m_pXmlNode = NULL;
    m_pXmlElem = NULL;
    m_pszFileName = NULL;
	m_pBaseXmlNode = NULL;
	m_pBaseXmlElem = NULL;
	SAFE_DELETE(m_pXMlDoc);
}

bool CXml::SetCondenseWhiteSpace(bool bCondenseWhiteSpace)
{
	//若已经初始化过了，则不能再设置
	if (!m_bFistInit)
	{
		return false;
	}
	m_bCondenseWhiteSpace = bCondenseWhiteSpace;
	return true;
}

bool CXml::IsWhiteSpaceCondensed()
{
	return m_bCondenseWhiteSpace;
}

/**************************************************************************
* name       : Load
* description: 加载一个xml文件
* input      : NA
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::Load(TiXmlEncoding encode)
{
    CHECK_POINTER(m_pszFileName, false);

    SAFE_NEW(m_pXMlDoc, TiXmlDocument);//lint !e774

    // 判断指针是否为空
    CHECK_POINTER(m_pXMlDoc, false); //lint !e774

    if (m_pXMlDoc->LoadFile(m_pszFileName, encode))
    {
        GetRootPos();

        return true;
    }
	
    return false;
}

/**************************************************************************
* name       : Load
* description: 加载一个xml文件
* input      : pszFileName 加载的文件名
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::Load(const char *pszFileName, TiXmlEncoding encode)
{
    CHECK_POINTER(pszFileName, false);

    m_pszFileName = pszFileName;
	
    return Load(encode);
}

/**************************************************************************
* name       : Parse
* description: 解析一个xml流到xml文件
* input      : pszStream xml流
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::Parse(const char *pszStream, TiXmlEncoding encode)
{
    CHECK_POINTER(pszStream, false);

    //songyiqun
	SAFE_DELETE(m_pXMlDoc);
    SAFE_NEW(m_pXMlDoc, TiXmlDocument);//lint !e774

    CHECK_POINTER(m_pXMlDoc, false);//lint !e774

    //TIXML_ENCODING_LEGACY为Tinyxml使用的编码方式
    (void)(m_pXMlDoc->Parse(pszStream, NULL, encode));
    if (0 != m_pXMlDoc->Error())
    {
        return false;
    }

    GetRootPos();

    return true;
}

/**************************************************************************
* name       : Save
* description: 存储xml文件
* input      : pszFileName 存储的文件名
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::Save(const char *pszFileName)
{
    CHECK_POINTER(pszFileName, false);

    m_pszFileName = pszFileName;
	
    return Save();
}

/**************************************************************************
* name       : Save
* description: 存储xml文件
* input      : NA
* output     : NA
* return     : true - 成功，false - 失败
* remark     : 与Load(const char *pszFileName)配对使用
**************************************************************************/
bool CXml::Save()
{
    CHECK_POINTER(m_pszFileName, false);

    //SAFE_NEW(m_pXMlDoc, TiXmlDocument);

    CHECK_POINTER(m_pXMlDoc, false);//lint !e774
	
    return m_pXMlDoc->SaveFile(m_pszFileName);
}//lint !e1762

/**************************************************************************
* name       : Print
* description: 在Win32平台打印出xml文件
* input      : NA
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void CXml::Print() const
{
    CHECK_POINTER_VOID(m_pXMlDoc);

    // m_pXMlDoc->Print(stdout); 去掉XML 打印，打印容易产生程序异常
}

/**************************************************************************
* name       : AddElem
* description: 添加元素
* input      : pszElemName 待添加的元素名
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::AddElem(const char *pszElemName)
{
    CHECK_POINTER(pszElemName, false);

    //Tinyxml会管理所有的申请的内存，不需要调用者delete
    TiXmlElement *pXmlAddElem = NULL;
    try
    {
        pXmlAddElem = new TiXmlElement(pszElemName);
    }
    catch (...)
    {
        pXmlAddElem = NULL;

        return false;//lint !e438
    }
    CHECK_POINTER(pXmlAddElem, false);//lint !e774

    if (NULL == m_pXmlNode)
    {
        if (NULL == m_pXMlDoc)
        {
            SAFE_NEW(m_pXMlDoc, TiXmlDocument);//lint !e774
        }

        CHECK_POINTER(m_pXMlDoc, false);//lint !e774
        (void)(m_pXMlDoc->LinkEndChild(pXmlAddElem));
        GetRootPos();

        return true;
    }

    m_pXmlNode = m_pXmlNode->Parent();

	CHECK_POINTER(m_pXmlNode,false);

    // 判断指针是否为空
    CHECK_POINTER(m_pXmlNode, false); //lint !e774

    (void)(m_pXmlNode->LinkEndChild(pXmlAddElem));
    
	m_pXmlNode = m_pXmlNode->LastChild();
	CHECK_POINTER(m_pXmlNode,false);

    m_pXmlElem = pXmlAddElem;

    return true;
}

/**************************************************************************
* name       : AddElemBeforeCurNode
* description: 在当前节点前添加元素
* input      : pszElemName 待添加的元素名
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::AddElemBeforeCurNode(const char *pszElemName)
{
	CHECK_POINTER(pszElemName, false);

	//Tinyxml会管理所有的申请的内存，不需要调用者delete
	TiXmlElement *pXmlAddElem = NULL;
	try
	{
		pXmlAddElem = new TiXmlElement(pszElemName);
	}
	catch (...)
	{
		pXmlAddElem = NULL;

		return false;//lint !e438
	}
    CHECK_POINTER(pXmlAddElem, false);//lint !e774

	if (NULL == m_pXmlNode)
	{
		if (NULL == m_pXMlDoc)
		{
			SAFE_NEW(m_pXMlDoc, TiXmlDocument);//lint !e774
		}

		CHECK_POINTER(m_pXMlDoc, false);//lint !e774
		(void)(m_pXMlDoc->LinkEndChild(pXmlAddElem));
		GetRootPos();

		return true;
	}

	// 判断指针是否为空
	CHECK_POINTER(m_pXmlNode, false); //lint !e774

	TiXmlNode *pParent = m_pXmlNode->Parent();

	CHECK_POINTER(pParent,false);	

	TiXmlNode *pXmlNode = pParent->InsertBeforeChild(m_pXmlNode,(*pXmlAddElem));

	if (NULL == pXmlNode)
	{
		return false;
	}
	m_pXmlNode = pXmlNode;
	m_pXmlElem = m_pXmlNode->ToElement();

	return true;
}

/**************************************************************************
* name       : AddChildElem
* description: 添加子元素
* input      : pszElemName 待添加的子元素名
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::AddChildElem(const char *pszElemName)
{
    CHECK_POINTER(pszElemName, false);

    CHECK_POINTER(m_pXmlElem, false);

    //Tinyxml会管理所有的申请的内存，不需要调用者delete
    TiXmlElement *pXmlElem = NULL;
    try
    {
        pXmlElem = new TiXmlElement(pszElemName);
    }
    catch (...)
    {
        pXmlElem = NULL;

        return false;//lint !e438
    }
    CHECK_POINTER(pXmlElem, false);//lint !e774

    (void)(m_pXmlElem->LinkEndChild(pXmlElem));

    return true;//lint !e429
}

/**************************************************************************
* name       : SetElemValue
* description: 设定当前元素值
* input      : pszElemValue 待设定的元素值
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::SetElemValue(const char* pszElemValue)
{
    CHECK_POINTER(pszElemValue, false);

    CHECK_POINTER(m_pXmlElem, false);

    //Tinyxml会管理所有的申请的内存，不需要调用者delete
    TiXmlText *pXmlText = NULL;

    try
    {
        pXmlText = new TiXmlText(pszElemValue);
    }
    catch (...)
    {
        pXmlText = NULL;

        return false;//lint !e438
    }
    CHECK_POINTER(pXmlText, false);//lint !e774

    (void)(m_pXmlElem->LinkEndChild(pXmlText));

    return true;//lint !e429
}


/**************************************************************************
* name       : SetElemValueCDATA
* description: 设定当前元素值 <![CDATA[ElemValue]]>
* input      : pszElemValue 待设定的元素值
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::SetElemValueCDATA(const char* pszElemValue)
{
    CHECK_POINTER(pszElemValue, false);
    CHECK_POINTER(m_pXmlElem, false);

    TiXmlText *pXmlText = NULL;

    try
    {
        pXmlText = new TiXmlText(pszElemValue);
        CHECK_POINTER(pXmlText, false);//lint !e774
        pXmlText->SetCDATA(true);  // 设置该文本为CDATA,无需解析
    }
    catch (...)
    {
        pXmlText = NULL;
        return false;//lint !e438
    }
    CHECK_POINTER(pXmlText, false);//lint !e774

    (void)(m_pXmlElem->LinkEndChild(pXmlText));
    return true;//lint !e429
}

/**************************************************************************
* name       : GetElemValue
* description: 获取当前元素值
* input      : NA
* output     : NA
* return     : const char*
* remark     : NA
**************************************************************************/
const char* CXml::GetElemValue() const
{
    CHECK_POINTER(m_pXmlElem, NULL);

	const char * pRet = m_pXmlElem->GetText();
	if (NULL == pRet)
	{
		return "";//如果找到是空，返回临时的空串
	}
    return pRet;
}

/**************************************************************************
* name       : GetElem
* description: 获取当前元素
* input      : NA
* output     : NA
* return     : const char*
* remark     : NA
**************************************************************************/
const char* CXml::GetElem()
{
    if (NULL == m_pXmlNode)
    {
        return NULL;
    }

    //查找元素属性项
    //NULL指当前节点不是元素，若不是继续向下查找
    TiXmlNode *pXmlNode = NULL;
    while (NULL == m_pXmlNode->ToElement())
    {
        pXmlNode = m_pXmlNode->NextSibling();
        if (NULL == pXmlNode)
        {
            return NULL;
        }

        m_pXmlNode = pXmlNode;
    }

    m_pXmlElem = m_pXmlNode->ToElement();

    // 判断指针是否为空
    CHECK_POINTER(m_pXmlNode, NULL); //lint !e774

    return m_pXmlNode->Value();
}

/**************************************************************************
* name       : IntoElem
* description: 进入下一级
* input      : NA
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::IntoElem()
{
    CHECK_POINTER(m_pXmlNode, false);

    if ((NULL == m_pXmlNode->FirstChild())
        || m_pXmlNode->FirstChild()->ToText())
    {
        return false;
    }

    m_pXmlNode = m_pXmlNode->FirstChild();
    m_pXmlElem = m_pXmlNode->ToElement();

    return true;
}

/**************************************************************************
* name       : OutOfElem
* description: 退出到上一级
* input      : NA
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void CXml::OutOfElem()
{
    CHECK_POINTER_VOID (m_pXmlNode);

    m_pXmlNode = m_pXmlNode->Parent();
    m_pXmlElem = m_pXmlNode->ToElement();
}

/**************************************************************************
* name       : FindElem
* description: 查找元素
* input      : pszElemName 待查找的元素名
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::FindElem(const char *pszElemName)
{
    CHECK_POINTER(pszElemName, false);
    CHECK_POINTER(m_pXmlNode, false);

    TiXmlNode* pTmpNode = m_pXmlNode;

	// 将指针指向第一个元素
	m_pXmlNode = m_pXmlNode->Parent()->FirstChild();

    // 判断指针是否为空
    CHECK_POINTER(m_pXmlNode, false);

	m_pXmlElem = m_pXmlNode->ToElement();

	// 增加一个变量检查控制情况
	const char* pChkTmpNode = NULL;
    while (((pChkTmpNode = GetElem()) != NULL) && (0 != strcmp(pszElemName, pChkTmpNode))) //lint !e838
    {
        if (NULL == m_pXmlNode->NextSibling())
        {
            break;
        }

        m_pXmlNode = m_pXmlNode->NextSibling();
        m_pXmlElem = m_pXmlNode->ToElement();
    }
/*
	while (0 != strcmp(pszElemName, GetElem()))
    {
        if (NULL == m_pXmlNode->PreviousSibling())
        {
            return false;
        }

        m_pXmlNode = m_pXmlNode->PreviousSibling();
        m_pXmlElem = m_pXmlNode->ToElement();
    }
*/
	const char* pNextTmpElem = this->GetElem();
	CHECK_POINTER(pNextTmpElem, false);
    if (0 == strcmp(pszElemName, pNextTmpElem))
    {
        return true;
    }

	// 如果找不到匹配节点，指针还原
	m_pXmlNode = pTmpNode;
	m_pXmlElem = m_pXmlNode->ToElement();
	
    return false;
}



/**************************************************************************
* name       : AddAttribute
* description: 添加元素属性
* input      : pszAttriName 待添加的属性名
               pszAttriValue 对应的属性值
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void CXml::AddAttribute(const char *pszAttriName, const char *pszAttriValue)
{
    if ((NULL == pszAttriName) || (NULL == pszAttriValue))
    {
        return;
    }

    CHECK_POINTER_VOID(m_pXmlElem);

    m_pXmlElem->SetAttribute(pszAttriName, pszAttriValue);
}

/**************************************************************************
* name       : GetAttributeValue
* description: 获得元素属性值
* input      : pszAttriName 元素的属性名
* output     : NA
* return     : const char* 元素的属性值
* remark     : NA
**************************************************************************/
const char* CXml::GetAttributeValue(const char*pszAttriName) const
{
    if (NULL == m_pXmlElem)
    {
        return NULL;
    }
	
    return m_pXmlElem->Attribute(pszAttriName);
}

/**************************************************************************
* name       : GetRootPos
* description: 跳至根节点
* input      : NA
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void CXml::GetRootPos()
{
    if (NULL == m_pXMlDoc)
    {
        return;
    }

    m_pXmlNode = m_pXMlDoc->RootElement();

    if (NULL == m_pXmlNode)
    {
        return;
    }

    m_pXmlElem = m_pXmlNode->ToElement();
}

/**************************************************************************
* name       : NextElem
* description: 进入同级下一个元素
* input      : NA
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::NextElem()
{
    if (NULL == m_pXmlNode)
    {
        return false;
    }
	
	//过滤掉注释，NextSibling会读到注释，类型为TINYXML_COMMENT
	do 
	{
		if (NULL == m_pXmlNode->NextSibling())
		{
			return false;
		}

		m_pXmlNode = m_pXmlNode->NextSibling();
	} while (m_pXmlNode->Type() == m_pXmlNode->TINYXML_COMMENT);
    
    m_pXmlElem = m_pXmlNode->ToElement();

    return true;
}

/**************************************************************************
* name       : RemoveAttribute
* description: 删除元素属性
* input      : pszAttriName 元素的属性名
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void CXml::RemoveAttribute(const char*pszAttriName)
{
    if (NULL == pszAttriName)
    {
        return;
    }

    if (NULL == m_pXmlElem)
    {
        return;
    }

    m_pXmlElem->RemoveAttribute(pszAttriName);
}

/**************************************************************************
* name       : ModifyAttributeValue
* description: 修改元素属性值
* input      : pszAttriName 待修改的属性名
               pszAttriValue 对应的属性值
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void CXml::ModifyAttributeValue(const char *pszAttriName, const char *pszAttriValue)
{
    if ((NULL == pszAttriName) || (NULL == pszAttriValue))
    {
        return;
    }

    if (NULL == m_pXmlElem)
    {
        return;
    }

    if (NULL != GetAttributeValue(pszAttriName))
    {
        m_pXmlElem->SetAttribute(pszAttriName, pszAttriValue);
    }
}

/**************************************************************************
* name       : ModifyElemValue
* description: 修改元素值
* input      : pszElemValue 元素值
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void CXml::ModifyElemValue(const char *pszElemValue)
{
    if (NULL == pszElemValue)
    {
        return;
    }

    if (NULL == m_pXmlNode)
    {
        return;
    }

	if (NULL == m_pXmlNode->FirstChild())
	{
		(void)SetElemValue(pszElemValue);
		return;
	}

    TiXmlText *pXmlText = m_pXmlNode->FirstChild()->ToText();

    //判断指针是否为空
    if (NULL == pXmlText)
    {
        return;
    }

    pXmlText->SetValue(pszElemValue);
}

/**************************************************************************
* name       : ModifyElemName
* description: 修改元素
* input      : pszElemName 元素名
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void CXml::ModifyElemName(const char *pszElemName)
{
    if (NULL == pszElemName)
    {
        return;
    }

    if (NULL == m_pXmlNode)
    {
        return;
    }

    m_pXmlNode->SetValue(pszElemName);
}

/**************************************************************************
* name       : RemoveElem
* description: 删除元素
* input      : NA
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::RemoveElem()
{
    if (NULL == m_pXmlNode)
    {
        return false;
    }

    TiXmlNode* pTempParentNode = NULL;
    TiXmlNode* pTempNode = NULL;

    pTempParentNode = m_pXmlNode->Parent();//lint !e838

    if ((m_pXmlNode->PreviousSibling()) && (m_pXmlNode->PreviousSibling()->ToElement()))
    {
        pTempNode = m_pXmlNode->PreviousSibling();
    }
    else if ((m_pXmlNode->NextSibling()) && (m_pXmlNode->NextSibling()->ToElement()))
    {
        pTempNode = m_pXmlNode->NextSibling();
    }
    else
    {
        pTempNode = pTempParentNode;
    }

    //判断指针是否为空
    CHECK_POINTER(pTempParentNode,false);

    if (!pTempParentNode->RemoveChild(m_pXmlNode))
    {
        return false;
    }

    m_pXmlNode = pTempNode;

    return true;
}

/**************************************************************************
* name       : GetXMLStream
* description: 获取xml流
* input      : NA
* output     : nXMLStreamLength
* return     : const char* xml流
* remark     : NA
**************************************************************************/
const char*  CXml::GetXMLStream(unsigned int &uiXMLStreamLength)
{
	TiXmlPrinter xmlPrinter;
	xmlPrinter.SetStreamPrinting();		

	if (NULL == m_pXMlDoc)
	{
		SAFE_NEW(m_pXMlDoc, TiXmlDocument);//lint !e774
	}

	CHECK_POINTER(m_pXMlDoc, NULL);//lint !e774
	if (!m_pXMlDoc->Accept(&xmlPrinter))
	{
		return NULL;
	}

    uiXMLStreamLength = xmlPrinter.Size();

	m_strStream = xmlPrinter.CStr();
    return m_strStream.c_str();
}

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
bool CXml::AddDeclaration(const char* pszVersion, const char* pszEncode, const char* pszStdLone)
{
    if ((NULL == pszVersion) || (NULL == pszEncode) || (NULL == pszStdLone))
    {
        return false;
    }

    if (NULL == m_pXMlDoc)
    {
        SAFE_NEW(m_pXMlDoc, TiXmlDocument);//lint !e774
    }

    CHECK_POINTER(m_pXMlDoc, false);//lint !e774

    TiXmlDeclaration *pXmlDeclaration;
    try
    {
        pXmlDeclaration = new TiXmlDeclaration(pszVersion, pszEncode, pszStdLone);
    }
    catch (...)
    {
        pXmlDeclaration = NULL;

        return false;//lint !e438
    }
    CHECK_POINTER(pXmlDeclaration, false);//lint !e774

    (void)(m_pXMlDoc->LinkEndChild(pXmlDeclaration));

    return true;//lint !e429
}

/**************************************************************************
* name       : AddChildAttribute
* description: 添加子元素属性
* input      : pszAttriName 元素属性名
               pszAttriValue 元素属性值
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::AddChildAttribute( const char*pszAttriName, const char* pszAttriValue )
{
    if ((NULL == pszAttriName) || (NULL == pszAttriValue))
    {
        return false;
    }
    
    if (IntoElem())
    {
        AddAttribute(pszAttriName, pszAttriValue);
        OutOfElem();

        return true;
    }
	
    return false;
}

/**************************************************************************
* name       : SetChildData
* description: 添加子元素值
* input      : pValue 元素值
* output     : NA
* return     : true - 成功，false - 失败
* remark     : NA
**************************************************************************/
bool CXml::SetChildData( const char *pszValue)
{
    if (NULL == pszValue)
    {
        return false;
    }

    if (IntoElem())
    {
        bool bRet = SetElemValue(pszValue);
        OutOfElem();
        if (bRet)
        {
            return true;
        }
    }
	
    return false;
}

bool CXml::FindElemEx(const char *pszElemPath)
{
	const char* seps = "/"; // 字符串分隔符
	bool returnFlag = true;

	// 检查传入的路径是否为空
	CHECK_POINTER(pszElemPath,false);
    unsigned int iNewStrLen = strlen(pszElemPath) + 1;

	// 创建临时字符串
    char* pszPath = NULL;
    SAFE_NEW_A(pszPath, char, iNewStrLen);
    CHECK_POINTER(pszPath,false);
   
	memset_s(pszPath,iNewStrLen,0,iNewStrLen);
    char* pszTmpPath = pszPath;

    // 复制字符串
	strncpy_s(pszTmpPath, iNewStrLen, pszElemPath, iNewStrLen);
    
    if(*pszTmpPath == '/')
    {
        ++pszTmpPath;   //lint !e810
        this->GetRootPos();
        (void)this->IntoElem();
    }
    else
    {
        // 将xml的指针指向根节点
        (void)this->GetBasePos();
    }
    char *saveptr = NULL;
    char* token = STRTOK( pszTmpPath, seps, &saveptr); 
	while( token != NULL )
    {//lint !e525
	   const char* tmpStr = strstr(token,"[");
	   if(NULL != tmpStr)
	   {   
		    // 处理多个同名节点的情况
            int index = atoi(tmpStr + 1);
			std::string  strTemp(token,static_cast<unsigned int>(tmpStr - token));
			if(!this->FindElem(strTemp.c_str()))
			{
				returnFlag = false;
				break;
			}

			if(!this->NextElemEx(index))
			{
				returnFlag = false;
				break;
			}
	   }
	   else
	   {
		   // 处理单节点的情况
		   if(!this->FindElem(token))
		   {
			   returnFlag = false;
			   break;
		   }
	   }

	   // 进入该节点
       (void)this->IntoElem();
	  // Get next token: 
	  token = STRTOK( NULL, seps, &saveptr); 
    }
    SAFE_DELETE_A(pszPath);//lint !e774 !e1775
    return returnFlag;//lint !e438
}

bool CXml::FindElemValue( const char *pszElemName )
{
	CHECK_POINTER(pszElemName, false);
	CHECK_POINTER(m_pXmlNode, false);

	const char* pElem = GetElem();
	while ((NULL != pElem) && (0 != strcmp(pszElemName, pElem)))
	{
		if (NULL == m_pXmlNode->NextSibling())
		{
			return false;
		}

		m_pXmlNode = m_pXmlNode->NextSibling();
		m_pXmlElem = m_pXmlNode->ToElement();
	}

	pElem = GetElem();
	if (NULL == pElem)
	{
		return false;
	}

	if (0 == strcmp(pszElemName, pElem))
	{
		return true;
	}
	
	return false;
}

bool CXml::NextElemEx(int count)
{
	const char* pszNodeName = this->GetElem();
	CHECK_POINTER(pszNodeName, false);
	for(int i = 0;i < count;++i)
	{
		const char* pTmpElem = this->GetElem();
		CHECK_POINTER(pTmpElem, false);
		if (!this->NextElem() || 0 != strcmp(pszNodeName,pTmpElem))
		{
			return false;
		}
	}
	return true;
}

bool CXml::SetElemValueEx(const char *pszElemPath,const char *pszElemValue)
{
    //指针判断
    CHECK_POINTER(pszElemPath, false);
    CHECK_POINTER(pszElemValue, false);

     if(FindElemEx(pszElemPath))
	 {
		 const char *str_value = GetElemValue();
		 if(NULL != str_value && 0 != strcmp("",str_value))
		 {
			 this->ModifyElemValue(pszElemValue);
			 return true;
		 }
         else
		 {
			 if (!this->SetElemValue(pszElemValue))
			 {
				 return false;
			 }
			 
			 //这句是为了防止本来的值为空串的时候，set成功，值却没加进去
			 this->ModifyElemValue(pszElemValue);
             return true;
		 }
	 }
	 return false;
}

bool CXml::SetElemValueCDATAEx(const char *pszElemPath,const char *pszElemValue)
{
    //指针判断
    CHECK_POINTER(pszElemPath, false);
    CHECK_POINTER(pszElemValue, false);

	if(FindElemEx(pszElemPath))
	{
		const char *str_value = GetElemValue();
		if(NULL != str_value && 0 != strcmp("",str_value))
		{
			this->ModifyElemValueCDATA(pszElemValue);
			return true;
		}else
		{			
			if (!this->SetElemValueCDATA(pszElemValue))
			{
				return false;
			}

			//这句是为了防止本来的值为空串的时候，set成功，值却没加进去
			this->ModifyElemValueCDATA(pszElemValue);

			return true;
		}
	}
	return false;
}

void CXml::ModifyElemValueCDATA(const char *pszElemValue)
{
	if (NULL == pszElemValue)
	{
		return;
	}

	if (NULL == m_pXmlNode)
	{
		return;
	}

	TiXmlText *pXmlText = m_pXmlNode->FirstChild()->ToText();

    //判断指针是否为空
    CHECK_POINTER_VOID(pXmlText);

	pXmlText->SetCDATA(true);  // 添加CDATA属性
	pXmlText->SetValue(pszElemValue);
}

const char* CXml::GetElemValueEx(const char *pszElemPath)
{
    //判断指针是否为空
    if (NULL == pszElemPath)
    {
        return NULL;
    }

	if(FindElemEx(pszElemPath))
	{
		return this->GetElemValue();
	}
	return NULL;
}

bool CXml::MkAndNextElemEx(int count)
{
	const char* pszNodeName = this->GetElem();
	CHECK_POINTER(pszNodeName, false);
	int j = 0;
	for(int i = 0;i < count;++i)
	{
		const char* pTmpElem = this->GetElem();
		CHECK_POINTER(pTmpElem, false);
		if (this->NextElem() && 0 == strcmp(pszNodeName,pTmpElem))
		{
			++j;
		}
	}
	for(;j < count;++j)
	{
		(void)this->IntoElem();
		if(this->AddElem(pszNodeName))
		{
			const char* pTmpElem = this->GetElem();
			CHECK_POINTER(pTmpElem, false);
			//return false;
		}
	}
	return true;
}

bool CXml::SetBaseElem(const char* pszElemPath)
{
    if(NULL != pszElemPath)
    {
        (void)this->FindElemEx(pszElemPath);
    }
    m_pBaseXmlNode = m_pXmlNode;
    m_pBaseXmlElem = m_pXmlElem;
    return true;
}

bool CXml::GetBasePos()
{
    if (NULL == m_pBaseXmlNode)
    {
        GetRootPos();
    }
    else
    {
        m_pXmlNode = m_pBaseXmlNode;
        m_pXmlElem = m_pBaseXmlElem;
    }
    return true;
}

bool CXml::MkAndFindElemEx(const char *pszElemPath)
{
	// 创建临时变量
	
	char* token;
	const char* seps = "/"; // 字符串分隔符
	bool returnFlag = true;

	// 检查传入的路径是否为空
	CHECK_POINTER(pszElemPath,true);

	// 将xml的指针指向根节点
	(void)this->GetBasePos();

	// 创建临时字符串
    unsigned int iNewStrLen = strlen(pszElemPath) + 1;
    char* pszPath = NULL;
    SAFE_NEW_A(pszPath, char, iNewStrLen);
    CHECK_POINTER(pszPath, false);
    
	memset_s(pszPath,iNewStrLen,0,iNewStrLen);
    char* pszTmpPath = pszPath;    

	// 复制字符串
	strncpy_s(pszTmpPath, iNewStrLen, pszElemPath, iNewStrLen);
	
    char *saveptr = NULL;
	token = STRTOK( pszTmpPath, seps, &saveptr);
	std::string pszParentNode = "";
	while( token != NULL )
	{
		// 当前指针指向的节点名称
		const char* curNodeName = this->GetElem();
		char* pszTmpNode = strstr(token,"[");
		if(NULL != pszTmpNode)
		{   
			// 处理多个同名节点的情况
			int index = atoi(pszTmpNode + 1);
			std::string  strTemp(token,static_cast<unsigned int>(pszTmpNode - token));
            if(NULL == curNodeName)
            {
                 (void)this->AddElem(strTemp.c_str());
            }
			if(!this->FindElem(strTemp.c_str()))
			{
				returnFlag = false;
				break;

				//// 如果发现没有此节点则创建该节点
    //            if(0 != strcmp(curNodeName,pszParentNode.c_str()))//lint !e527
				//{
    //                 (void)this->AddElem(strTemp.c_str());
				//}else
				//{
    //                 (void)this->AddChildElem(strTemp.c_str());
				//}
    //            (void)this->FindElem(strTemp.c_str());
			}

			if(!this->MkAndNextElemEx(index))
			{
				returnFlag = false;
				break;
			}
			// 记录当前节点为上一节点的父节点
		    pszParentNode = strTemp;
		}
		else
		{
            if(NULL == curNodeName)
            {
                (void)this->AddElem(token);
            }
			// 处理单节点的情况
			if(!this->FindElem(token))
			{
                if (NULL != curNodeName)
                {
				    if(0 != strcmp(curNodeName,pszParentNode.c_str()))
				    {
                         (void)this->AddElem(token);
				    }else
				    {
                         (void)this->AddChildElem(token);
				    }
                }
                (void)this->FindElem(token);
			}
			// 记录当前节点为上一节点的父节点
		    pszParentNode = token;
		}

		// 进入该节点
		(void)this->IntoElem();

		// Get next token: 
		token = STRTOK( NULL, seps, &saveptr ); 
	}
    SAFE_DELETE_A(pszPath);//lint !e424 !e774 !e1775
    return returnFlag;//lint !e438
  //return false;
}

bool CXml::MkAndSetElemValueEx(const char *pszElemPath,const char *pszElemValue)
{
    //入参指针判断
    if ((NULL == pszElemPath) || (NULL == pszElemValue))
    {
        return false;
    }
	if(MkAndFindElemEx(pszElemPath))
	{
		const char *str_value = GetElemValue();
		if(NULL != str_value && 0 != strcmp("",str_value))
		{
			this->ModifyElemValue(pszElemValue);
			return true;
		}
        else
		{
			if (!this->SetElemValue(pszElemValue))
			{
				return false;
			}

			//这句是为了防止本来的值为空串的时候，set成功，值却没加进去
			this->ModifyElemValue(pszElemValue);
			return true;
		}
	}
	return false;
}

//lint -e1554
CXml::CXml(const CXml &src)
{
    m_pBaseXmlElem = src.m_pBaseXmlElem;
    m_pBaseXmlNode = src.m_pBaseXmlNode;
    m_pXMlDoc = src.m_pXMlDoc;
    m_pXmlElem = src.m_pXmlElem;
    m_pXmlNode = src.m_pXmlNode;
    m_pszFileName = src.m_pszFileName;
}//lint -e1554

CXml& CXml::operator=(const CXml&)
{
    return *this;
}//lint !e1529 !e1745
