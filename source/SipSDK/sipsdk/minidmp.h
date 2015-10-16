#ifndef _MINIDUMP_H_
#define _MINIDUMP_H_
#include <windows.h>
#include <imagehlp.h>
#include <stdlib.h>
#pragma comment(lib, "dbghelp.lib")

BOOL IsDataSectionNeeded(const WCHAR* pModuleName);
BOOL CALLBACK MiniDumpCallback(PVOID, const PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput);
void CreateMiniDump(EXCEPTION_POINTERS* pep, std::string strFileName);

#endif

