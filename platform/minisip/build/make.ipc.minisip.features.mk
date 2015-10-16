# GNU make Common MiniSip Feature definitions file
# Copyrights (c) Huawei Technologies (I) Pvt. Ltd.

SS_CFLAGS_FEATURES =

# LOG support in Release Mode
#SS_CFLAGS_FEATURES += $(SS_CC_DEFINE)SS_LOG

# ASSERT support in Release Mode
#SS_CFLAGS_FEATURES += $(SS_CC_DEFINE)SS_DBG

# Compression support in TPT_D sample provided
#SS_CFLAGS_FEATURES += $(SS_CC_DEFINE)SS_SIGCOMP

# Support IPV6 Functionality.
#SS_CFLAGS_FEATURES += $(SS_CC_DEFINE)SS_IPV6_SUPPORT

# Dont Support non-2xx final response on receiving 200 for CANCEL Functionality.
#SS_CFLAGS_FEATURES += $(SS_CC_DEFINE)SS_DISABLE_NON_2XX_FINAL_RESP_TIMER

# Support for Multi threading in SDP library
#SS_CFLAGS_FEATURES += $(SS_CC_DEFINE)MULTI_THREAD

# Support for Response Retry in TptD
SS_CFLAGS_FEATURES += $(SS_CC_DEFINE)FEATURE_RESPOSNE_RETRY

# Support for Little Endian System
SS_CFLAGS_FEATURES += $(SS_CC_DEFINE)SS_LITTLE_ENDIAN_SYS

# Support Maintain APIs
SS_CFLAGS_FEATURES += $(SS_CC_DEFINE)SS_MAINTAIN