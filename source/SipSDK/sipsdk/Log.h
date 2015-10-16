/********************************************************************
filename		: 	Log.h
author			:	z00233055
created			:	2015/01/04
description		:	日志模块
copyright		:	Copyright (C) 2014-2016
history			:	2015/01/04 初始版本
*********************************************************************/

#ifndef _LOG_H_
#define _LOG_H_

#include <stdio.h>
#include <string>
#include <stdarg.h>
#include <WinBase.h>
#include "eSDKLogAPI.h"

#define MSG_LEN 4096
#define TIME_LEN 256
#define PRODUCT_NAME "TP-sip"

typedef enum
{
	TRACE_INFO,
	TRACE_DEBUG
}TRACE_LEVEL_E;

//lint -e438
class CLogTrace
{
public:
	CLogTrace(void);
	CLogTrace(TRACE_LEVEL_E level, const char *file, int line, const char *func, const char *fmt, ...)
		:m_Func(func), m_File(file), m_Line(line), m_Level(level)
	{
		va_list args;
		va_start(args, fmt);
		memset(m_Msg, 0, MSG_LEN);
		(void)_vsnprintf_s(m_Msg, MSG_LEN - 1, fmt, args);
		va_end(args);

		std::string msg("Enter ");
		msg.append(m_Func);
		if (0 != strlen(m_Msg))
		{
			msg.append(" ");
			msg.append(m_Msg);
		}
		Log_Run_Debug(PRODUCT_NAME,msg.c_str());

		GetLocalTime(&m_ReqTime);
		GetLocalTime(&m_RspTime);
	}

	~CLogTrace()
	{
		try 
		{
			std::string msg("Leave ");
			msg.append(m_Func);
			Log_Run_Debug(PRODUCT_NAME,msg.c_str());

			if (TRACE_INFO == m_Level)
			{
				GetLocalTime(&m_RspTime);
				char reqTime[TIME_LEN] = {0};
				char rspTime[TIME_LEN] = {0};
				(void)_snprintf_s(reqTime, TIME_LEN-1, "%04d-%02d-%02d %02d:%02d:%02d.%03d", m_ReqTime.wYear, m_ReqTime.wMonth, m_ReqTime.wDay, m_ReqTime.wHour, m_ReqTime.wMinute, m_ReqTime.wSecond, m_ReqTime.wMilliseconds);
				(void)_snprintf_s(rspTime, TIME_LEN-1, "%04d-%02d-%02d %02d:%02d:%02d.%03d", m_RspTime.wYear, m_RspTime.wMonth, m_RspTime.wDay, m_RspTime.wHour, m_RspTime.wMinute, m_RspTime.wSecond, m_RspTime.wMilliseconds);
				Log_Interface_Info(PRODUCT_NAME, "1", "C", m_Func, "", "", "", reqTime, rspTime, "", m_Msg);
			}
		} 
		catch(...){};

		m_Func = NULL;
		m_File = NULL;
	}
private:
	const char *m_Func;
	const char *m_File;
	int         m_Line;
	TRACE_LEVEL_E m_Level;
	SYSTEMTIME m_ReqTime;
	SYSTEMTIME m_RspTime;
	char m_Msg[MSG_LEN];
};

#define INFO_TRACE(fmt, ...) CLogTrace trace(TRACE_INFO,__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#define DEBUG_TRACE(fmt, ...) CLogTrace trace(TRACE_DEBUG,__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

static std::string FormatedMsg(const char *fmt, ...)
{
	char msg[MSG_LEN] = {0};
	va_list args;
	va_start(args, fmt);
	(void)_vsnprintf_s(msg, MSG_LEN - 1, fmt, args);
	va_end(args);
	return std::string(msg);
}

#define DEBUG_LOG(fmt, ...)		Log_Run_Debug(PRODUCT_NAME,FormatedMsg(fmt, ##__VA_ARGS__).c_str())
#define INFO_LOG(fmt, ...)		Log_Run_Info(PRODUCT_NAME,FormatedMsg(fmt, ##__VA_ARGS__).c_str())
#define WARN_LOG(fmt, ...)		Log_Run_Warn(PRODUCT_NAME,FormatedMsg(fmt, ##__VA_ARGS__).c_str())
#define ERROR_LOG(fmt, ...)		Log_Run_Error(PRODUCT_NAME,FormatedMsg(fmt, ##__VA_ARGS__).c_str())
//lint +e438

#endif //_LOG_H_
