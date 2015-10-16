#ifndef _ESDKTOOL_H
#define _ESDKTOOL_H
#include "StdAfx.h"
#include <string>
using namespace std;

//工具类
class eSDKTool
{
private:
	eSDKTool(void);
	~eSDKTool(void);
public:
	static void getCurrentPath(std::string& cstrPath);	//获取当前程序可执行路径	
	static void Int10ToStr(std::string& strNum,int num);//获取10进制的字符串
	static unsigned short StrToInt10(const std::string& strNum);	//字符串转数字
	static std::string CreateGuidStr();

};
#endif

