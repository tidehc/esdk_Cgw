#ifndef  __IVS_COMPAT_UTIL_H__
#define  __IVS_COMPAT_UTIL_H__

#include <time.h>     // time_t
#include <stdarg.h>   // va_list definition

#ifdef _MSC_VER
#include <WinSock.h>  // struct timeval definition
#endif 

#if defined __GNUC__ || defined LINUX
# include <sys/time.h>  // for gettimeofday
#endif


/* strings */
int IVS_vsnprintf(char *buffer, size_t count, const char *format, va_list argptr);
int IVS_snprintf(char *buffer, size_t count, const char *format, ...);
int IVS_strcasecmp(const char *string1, const char *string2);
int IVS_strncasecmp(const char *s1, const char* s2, size_t count);

/* time */
int IVS_localtime(struct tm *_tm, const time_t *_tt);
int IVS_gmtime(struct tm *_tm, const time_t *_tt);
int IVS_gettimeofday(struct timeval *tv);




#endif //  __IVS_COMPAT_UTIL_H__