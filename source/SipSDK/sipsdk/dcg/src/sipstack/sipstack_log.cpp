#if defined(__MSC_VER) || defined(WIN32)
#include <sys/timeb.h>
#endif
#include <ace/OS_NS_time.h>
#include <ace/OS_Memory.h>
#include <ace/config-macros.h>
#include <ace/Recursive_Thread_Mutex.h>
#include <ace/Global_Macros.h>
#include <ace_header.h> 

#include <cstdio>
#include <string>
//#include <sstream>
#include <queue>

#include "sipstack_log.hpp"
#include "sipstack_config.hpp"
#include "eSDKTool.h"

//#define SIPSTACK_LOGER_MAX_FILE_SIZE         (10*1024*1024)
//#define SIPSTACK_LOGER_MAX_FILE_COUT         (10)
#define SIPSTACK_LOGER_LINE_BUF_SIZE         (4*1024)
#define SIPSTACK_LOGER_FILE_NAME_BUFF_SIZE   (256*4)

#ifdef WIN32
//#define OS_SYS_SEP_CH              ('\\')
#else
#define OS_SYS_SEP_CH              ('/')
#endif

namespace SipStack{

//日志文件名
#define  SIPSTACK_LOGER_DIR          "\\log\\"
#define  SIPSTACK_LOGER_NAME         "sipstack"
#define  SIPSTACK_LOGER_SUFIX        "log"
static char g_loger_file[SIPSTACK_LOGER_FILE_NAME_BUFF_SIZE] = { 0 };

typedef std::queue<std::string> LOGER_QUEUE;
static LOGER_QUEUE g_loger_queue;


static ACE_Thread_Mutex  g_log_mutex;
static char              g_log_dclr_buf[SIPSTACK_LOGER_LINE_BUF_SIZE] = { 0 };

SipStackLoger SipStackLoger::m_instance;

SipStackLoger::SipStackLoger(void)
{
	std::string path;
	eSDKTool::getCurrentPath(path);
	path.append(SIPSTACK_LOGER_DIR);
	(void)_mkdir(path.c_str());
    size_t pos = (size_t)ACE_OS::snprintf( g_loger_file, SIPSTACK_LOGER_FILE_NAME_BUFF_SIZE,
                                   "%s%s.%s",
                                   path.c_str(), SIPSTACK_LOGER_NAME, SIPSTACK_LOGER_SUFIX
                                  );
    g_loger_file[pos] = '\0';
}

SipStackLoger& SipStackLoger::Instance()
{
    return m_instance;
}

SipStackLoger::~SipStackLoger(void)
{
}

string SipStackLoger::GetLogLevel(SIPSTACK_LOGER_LEVEL_EN level_en)
{
    switch(level_en)
    {
    case SIPSTACK_LOGER_DBG_LEVEL:
        return "DBG";
    case SIPSTACK_LOGER_INF_LEVEL:
        return "INF";
    case SIPSTACK_LOGER_ERR_LEVEL:
        return "ERR";
    case SIPSTACK_LOGER_WAR_LEVEL:
        return "WAR";
    default:
        return "UNK";
    }
}

void SipStackLoger::write( SIPSTACK_LOGER_LEVEL_EN level_en,
                           const char *file, const char* func, const unsigned int line,
                           const char * format, va_list argp
                         )
{
    if (1 == CSipStackConfigMgr::Instance().GetServiceCfg().GetLogSwitch())
    {
#ifdef SIPSTACK_DEBUG_BY_LOG
        if (NULL == format || NULL == file || NULL == func )
        {
            std::cerr << "sipstack logger - invalid args" << std::endl;
            return;
        }

        ACE_Guard<ACE_Thread_Mutex> guard(g_log_mutex);

        //取得日志等级
        const string logLevel = GetLogLevel(level_en);
        if ( logLevel.empty() )
        {
            std::cerr << "sipstack logger - unknown level" << std::endl;
            return;
        }

        //取得格式化附加内容
        size_t pos = ACE_OS::vsnprintf(g_log_dclr_buf, SIPSTACK_LOGER_LINE_BUF_SIZE, format, argp);
        if ( 0 > pos )
        {
            std::cerr << "sipstack logger - format additional string" << std::endl;
            return;
        }
        g_log_dclr_buf[pos] = '\0';

        //取得当前时间结构体
        time_t cur_tt       = time(NULL);
        struct tm *cur_tm_p = ::localtime(&cur_tt); //单线程执行线程安全

        struct timeval cur_tv = { 0, 0 };

        {
#if defined(_MSC_VER) || defined(WIN32)
            struct _timeb cur_tb = { 0, 0, 0, 0 };
            _ftime(&cur_tb);
            cur_tv.tv_sec  = (long)(cur_tb.time + cur_tb.timezone);
            cur_tv.tv_usec = cur_tb.millitm * 1000; //毫秒级
#elif defined(__GNC__) || defined(LINUX)
            gettimeofday(&cur_tv, NULL);
#else
#endif
        }

        //打开文件
        FILE* flog = fopen(g_loger_file,"a");
        if ( NULL == flog )
        {
            std::cerr << "sipstack logger - open file" << std::endl;
            return;
        }

        const char* src_file_p = file + strlen(file);
        while ( file != src_file_p && OS_SYS_SEP_CH != *src_file_p )
        {
            src_file_p --;
        }

        //输入至文件
        fprintf ( flog,
            "\n|%04d-%02d-%02d %02d:%02d:%02d.%03d|0x%016llx|     %s|%s:%d %s|%s|\n",
            cur_tm_p->tm_year + 1900, cur_tm_p->tm_mon + 1, cur_tm_p->tm_mday,
            cur_tm_p->tm_hour, cur_tm_p->tm_min, cur_tm_p->tm_sec, cur_tv.tv_usec / 1000,
            (unsigned long long)ACE_OS::thr_self(),
            logLevel.c_str(),
            src_file_p == file ? src_file_p : src_file_p + 1, line, func,
            g_log_dclr_buf
            );

        //统计文件的长度，若超过文件最大容量则绕接
        fseek(flog, 0L, SEEK_END);
        unsigned long fsize_ul = ftell(flog);
        fclose(flog);
        flog = NULL;

        if ( SIPSTACK_LOGER_MAX_FILE_SIZE >= fsize_ul )
        {
            return;
        }

        //格式化绕接后的文件路径名
        char buff_name[SIPSTACK_LOGER_FILE_NAME_BUFF_SIZE] = { '\0' };
        int name_pos = ACE_OS::snprintf( buff_name, SIPSTACK_LOGER_FILE_NAME_BUFF_SIZE,
            "%s%s_%04d%02d%02d%02d%02d%02d.log",
            SIPSTACK_LOGER_DIR, SIPSTACK_LOGER_NAME,
            cur_tm_p->tm_year + 1900, cur_tm_p->tm_mon+1, cur_tm_p->tm_mday,
            cur_tm_p->tm_hour, cur_tm_p->tm_min, cur_tm_p->tm_sec,
            SIPSTACK_LOGER_SUFIX
            );
        if ( 0 > name_pos || '\0' == buff_name[0] )
        {
            std::cerr << "sipstack logger - format backup file name" << std::endl;
            return;
        }
        buff_name[name_pos] = '\0';

        if ( NULL != rename ( g_loger_file, buff_name ) )
        {
            remove ( buff_name );
            if ( NULL != rename ( g_loger_file, buff_name ) )
            {
                std::cerr << "sipstack logger - rename - " << g_loger_file << std::endl;
                return;
            }
        }

        g_loger_queue.push(buff_name);

        if ( SIPSTACK_LOGER_MAX_FILE_COUT < g_loger_queue.size() )
        {
            remove(g_loger_queue.front().c_str());
            g_loger_queue.pop();
        }

#endif
    }
}//lint !e818

void SipStackLoger::inf( const char *file, const char* func, const unsigned int line,
                         const char *format, ...
                       )
{
#ifdef SIPSTACK_DEBUG_BY_LOG
    va_list argp;
    va_start(argp, format);
    write(SIPSTACK_LOGER_INF_LEVEL, file, func, line, format, argp);
    va_end(argp);
#endif
}

void SipStackLoger::war( const char *file, const char* func, const unsigned int line,
                         const char *format, ...
                       )
{
#ifdef SIPSTACK_DEBUG_BY_LOG
    va_list argp;
    va_start(argp, format);
    write(SIPSTACK_LOGER_WAR_LEVEL, file, func, line, format, argp);
    va_end(argp);
#endif
}

void SipStackLoger::err(const char *file, const char* func, const unsigned int line,
                        const char *format, ...)
{
#ifdef SIPSTACK_DEBUG_BY_LOG
    va_list argp;
    va_start(argp, format);
    write(SIPSTACK_LOGER_ERR_LEVEL, file, func, line, format, argp);
    va_end(argp);
#endif
}

void SipStackLoger::dbg(const char *file, const char* func, const unsigned int line,
                        const char *format, ...
                       )
{
#ifdef SIPSTACK_DEBUG_BY_LOG
    va_list argp;
    va_start(argp, format);
    write(SIPSTACK_LOGER_DBG_LEVEL, file, func, line, format, argp);
    va_end(argp);
#endif
}

}

