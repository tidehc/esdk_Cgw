#ifndef IVSCBB_SS_VOS_CONFIG_H
#define IVSCBB_SS_VOS_CONFIG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#define   VOS_OS_LINUX                   1
#define   VOS_OS_WIN32                   2
#define   VOS_OS_VXWORKS                 3

#define   VOS_BIG_ENDIAN                     0
#define   VOS_LITTLE_ENDIAN                  1


#ifdef ENV_LINUX
#define   VOS_APP_OS                     VOS_OS_LINUX
#define   VOS_BYTE_ORDER                 VOS_LITTLE_ENDIAN
#endif

#ifdef WIN32
#define   VOS_APP_OS                     VOS_OS_WIN32
#define   VOS_BYTE_ORDER                 VOS_LITTLE_ENDIAN
#endif

#ifdef VXWORKS
#define   VOS_APP_OS                     VOS_OS_VXWORKS
#define   VOS_BYTE_ORDER                 VOS_LITTLE_ENDIAN
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#if (VOS_APP_OS == VOS_OS_LINUX)
// header file
#include <memory.h>
#include <ctype.h>
#include <syslog.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <pthread.h>
#include <fcntl.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dlfcn.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/wait.h>
#include <sys/epoll.h>
#include <netdb.h>

#elif  (VOS_APP_OS == VOS_OS_WIN32)
#include <memory.h>
#include <WINSOCK2.H>
#elif (VOS_APP_OS == VOS_OS_VXWORKS)
#include "sysLib.h"
#include "semLib.h"
#include "vxWorks.h"
#include "sockLib.h"
#include "taskLib.h"
#include "inetLib.h"
#include "stdioLib.h"
#include "ioLib.h"
#include "sioLib.h" 
#include "string.h"
#include "stdLib.h"
#include "stdio.h"
#include "pthread.h"
#include "tickLib.h"
#include "selectLib.h"

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#endif /*VOS_CONFIG_H_INCLUDE  */
