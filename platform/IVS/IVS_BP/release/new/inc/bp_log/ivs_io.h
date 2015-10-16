#ifndef BP_IVS_IO_H
#define BP_IVS_IO_H


#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef  _MSC_VER
# include <io.h>
# include <direct.h>
#else
# include <unistd.h>
#endif

#ifdef _MSC_VER
#  define IVS_open    _open
#  define IVS_close   _close
#  define IVS_lseek   _lseek
#  define IVS_read    _read
#  define IVS_write   _write
#  define IVS_stat    _stat
#  define IVS_STATE   struct _stat
#  define IVS_access  _access
#  define IVS_mkdir(dir)   _mkdir(dir)
#  define IVS_rename  rename
#  define IVS_unlink  _unlink
#  define IVS_sync    _commit
#else

#  define IVS_open    open
#  define IVS_close   close
#  define IVS_lseek   lseek
#  define IVS_read    read
#  define IVS_write   write
#  define IVS_stat    stat
#  define IVS_STATE   struct stat
#  define IVS_access  access
#  define IVS_mkdir(dir)   mkdir(dir, S_IRUSR|S_IWUSR|S_IXUSR/*|S_IRGRP|S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH*/)
#  define IVS_rename  rename
#  define IVS_unlink  unlink
#  define IVS_sync    fsync
#endif 


#endif // BP_IVS_IO_H
