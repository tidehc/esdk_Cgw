#ifndef IVSCBB_SS_VOS_H
#define IVSCBB_SS_VOS_H

#include "vos_config.h"
#include "vos_basetype.h"
#include "vos_errno.h"
#include "vos_mem.h"
#include "vos_mutex.h"
#include "vos_sema.h"
#include "vos_thread.h"
#include "vos_time.h"
#include "vos_cond.h"

#ifndef  INFINITE
#define  INFINITE               0xFFFFFFFF
#endif

#define  VOS_DEFAULT_STACK_SIZE  (256*1024)
#define  VOS_THREAD_PRIOTITY     (75)

#define  VOS_MUTEX_MAXWAIT      INFINITE

#define MS_IN_SECONDS  1000
#define US_IN_MS       1000 
#define BITS_IN_BYTE   8


#endif /* VOS_H_INCLUDE  */

