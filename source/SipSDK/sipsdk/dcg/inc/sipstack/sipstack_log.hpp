#ifndef IVSCBB_SS_SIPSTACK_LOG_HPP
#define IVSCBB_SS_SIPSTACK_LOG_HPP

#include <stdarg.h>
#include <string>
#include <fstream>

using namespace std;

namespace SipStack{

typedef enum SIPSTACK_LOGER_LEVEL_ENUM
{
    SIPSTACK_LOGER_DBG_LEVEL = 0,
    SIPSTACK_LOGER_INF_LEVEL = 1,
    SIPSTACK_LOGER_WAR_LEVEL = 2,
    SIPSTACK_LOGER_ERR_LEVEL = 3
}SIPSTACK_LOGER_LEVEL_EN;

class SipStackLoger
{
public:
    void inf(const char *file, const char* func, const unsigned int line,
             const char *format, ...
            );
    void dbg(const char *file, const char* func, const unsigned int line,
             const char *format, ...
            );
    void war(const char *file, const char* func, const unsigned int line,
             const char *format, ...
            );
    void err(const char *file, const char* func, const unsigned int line,
             const char *format, ...
            );

    static SipStackLoger& Instance();

private:
    SipStackLoger(void);
    virtual ~SipStackLoger();

    string GetLogLevel(SIPSTACK_LOGER_LEVEL_EN level_en);

    void write(SIPSTACK_LOGER_LEVEL_EN level_en,
               const char *file, const char* func, const unsigned int line,
               const char * format, va_list argp
              );

private:
    static SipStackLoger m_instance;
};

#define LOGER_DBG(format, ...)                                                                    \
    SipStackLoger::Instance().dbg(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define LOGER_INF(format, ...)                                                                    \
    SipStackLoger::Instance().inf(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define LOGER_WAR(format, ...)                                                                    \
    SipStackLoger::Instance().war(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define LOGER_ERR(format, ...)                                                                    \
    SipStackLoger::Instance().err(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

}

#define SIPSTACK_LOGER_DBG(format, ...)                                                                         \
    SipStack::SipStackLoger::Instance().dbg(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define SIPSTACK_LOGER_INF(format, ...)                                                                         \
    SipStack::SipStackLoger::Instance().inf(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define SIPSTACK_LOGER_WAR(format, ...)                                                                         \
    SipStack::SipStackLoger::Instance().war(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define SIPSTACK_LOGER_ERR(format, ...)                                                                         \
    SipStack::SipStackLoger::Instance().err(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#endif /* __CLOG_H__ */
