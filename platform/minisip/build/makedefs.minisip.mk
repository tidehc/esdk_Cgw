# GNU make Makefile template for MiniSip
# Copyrights (c) Huawei Technologies (I) Pvt. Ltd.

# Include the correct platform specific definitions file

include $(SS_CODE_BUILD)/env.$(SS_PDT).$(SS_OS_VER).$(SS_CPU_VER).$(SS_LIB_SUFFIX).mk

# Include the feature definitions file for compile time feature support
include $(SS_CODE_BUILD)/make.$(SS_PDT).minisip.features.mk

# Include the Build file
include $(SS_CODE_BUILD)/make.minisip.build.mk
