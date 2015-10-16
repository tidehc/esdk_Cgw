#include "stdafx.h"
#include "minidmp.h"
#include "eSDKTool.h"
#include "Log.h"

LONG WINAPI GPTUnhandledExceptionFilter(PEXCEPTION_POINTERS pExceptionInfo)
{
	//得到当前时间
	INFO_LOG("Core dump.");
	SYSTEMTIME st;
	::GetLocalTime(&st);
	std::string strPath;
	eSDKTool::getCurrentPath(strPath);
	const int PATH_LENGTH = 2048;
	char szFileName[PATH_LENGTH]={0};
	(void)sprintf_s(szFileName,PATH_LENGTH-1,("%s\\core-%04d%02d%02d-%02d%02d-%02d-%02d.dmp"),strPath.c_str(), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	CreateMiniDump(pExceptionInfo, szFileName);
	//std::cerr << "unknown error：" << (*pExceptionInfo->ExceptionRecord) << std::endl;
	if(NULL != pExceptionInfo)
	{
		if(NULL != pExceptionInfo->ExceptionRecord)
		{
			exit((int)(pExceptionInfo->ExceptionRecord->ExceptionCode));
		}
		else
		{
			exit(0);
		}
		
	}
	else
	{
		exit(0);
	}
	
}

