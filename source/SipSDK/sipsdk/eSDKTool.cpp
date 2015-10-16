#include "StdAfx.h"
#include "eSDKTool.h"
#include "objbase.h"

eSDKTool::eSDKTool(void)
{
}

eSDKTool::~eSDKTool(void)
{
}

void eSDKTool::getCurrentPath(std::string& strPath)
{
	HMODULE hModule = GetModuleHandleA(("sipsdk.dll"));
	char path[MAX_PATH+1] = {0};
	::GetModuleFileNameA(hModule, path, MAX_PATH);
	std::string strModulePath = path;
	unsigned int loc = strModulePath.find_last_of("\\");
	if( loc != string::npos )
	{
		strPath = strModulePath.substr(0,loc);
	}
}

void eSDKTool::Int10ToStr(std::string& strNum,int num)
{
	const int LENGTH = 10;
	char buf[LENGTH] = {0};
	_itoa_s(num,buf,LENGTH,10);
	buf[LENGTH-1] = 0;
	strNum = buf;
}
unsigned short eSDKTool::StrToInt10(const std::string& strNum)
{
	return (unsigned short)atoi(strNum.c_str());
}

std::string eSDKTool::CreateGuidStr()
{
	GUID guid;
	(void)::CoCreateGuid(&guid);

	char buf[64];
	(void)::sprintf_s(buf, 
		"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X", 
		(unsigned int)guid.Data1, guid.Data2, guid.Data3, 
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3], 
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	return buf;
}




