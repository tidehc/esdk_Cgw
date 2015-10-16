#ifndef IVSCBB_SS_SIPSTACK_DEFS_H
#define IVSCBB_SS_SIPSTACK_DEFS_H

#define MACRO_NULL                                                       ;

#define MACRO_CHECK_POINTER_ACT(one_pointer, act)                        \
    if ( NULL == (one_pointer) )                                         \
    {                                                                    \
        ERROR_LOG( "the '"#one_pointer"' is null.");                 \
        act;                                                             \
    }

#define MACRO_CHECK_POINTER_RETURN(one_pointer, ret)                     \
    if ( NULL == (one_pointer) )                                         \
    {                                                                    \
        ERROR_LOG("the '"#one_pointer"' is null.");                  \
        return ret;                                                      \
    }

#define MACRO_CHECK_SZ_POINTER_RETURN(one_sz_pointer, ret)               \
    if ( NULL == ( one_sz_pointer ) || '\0' == (one_sz_pointer)[0] )     \
    {                                                                    \
        ERROR_LOG("the array '"#one_sz_pointer"' is emtpy.");        \
        return ret;                                                      \
    }

#endif //sipstack_defs.h
