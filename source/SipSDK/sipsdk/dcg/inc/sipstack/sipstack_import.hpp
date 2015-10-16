#ifndef IVSCBB_SS_SIPSTACK_IMPORT_HPP
#define IVSCBB_SS_SIPSTACK_IMPORT_HPP

#include <string>
#include <vector>
#include <list>
#include <map>
#include <iostream>
#include <sstream>

#include <ace_header.h>

#ifndef SIP_NEW
#define SIP_NEW(var, type)  do{ \
    if (NULL==var) {var = new type;} \
    }while(0)
#endif

#ifndef SIP_DELETE
#define SIP_DELETE(var)  do{ \
  delete var; var=NULL; }while(0)
#endif

#ifndef SIP_ASSERT_VOID
#define SIP_ASSERT_VOID(var)  do{ \
    if (!(var)) { return ;} \
    }while(0)
#endif

#ifndef SIP_ASSERT
#define SIP_ASSERT(var, ret) do{ \
    if (!(var)) { return ret;} \
    }while(0)
#endif

#define SIP_STRING_NULL     ""
#define SIP_INT_NULL        0
#define SIP_STRING_INIT     " "
#define SIP_INT_INIT        -1

#define SIP_STACK_SUCCESS  0
#define SIP_STACK_FAIL     -1
#define SIP_STACK_TIMEOUT  -2 

#define SIP_STACK_TIMEOUT_VALUE_DEFAULT  4
#define SIP_STACK_TIMEOUT_VALUE_REGISTER   60
#define SIP_STACK_TIMEOUT_VALUE_OPTION     10
#define SIP_STACK_TIMEOUT_VALUE_MESSAGE    (20)

#define SIP_STACK_EXPIRES              31536000

#endif //sipstack_import.hpp

