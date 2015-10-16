#ifndef _config_h
#define _config_h

#ifndef WIN32
    # include "config-linux.h"
    # define ACE_DEFAULT_BACKLOG 256
    # define ACE_HAS_EVENT_POLL
    # define ACE_HAS_LIMITED_SELECT
#else  //WIN32
    # include "config-win32.h"
#endif //WIN32

#endif //_config_h

