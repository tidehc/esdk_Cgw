#include the common MiniSip code organization definitions
include $(SS_CODE_BUILD)/make.minisip.defs.mk

# Include a common definition file
include $(SS_CODE_BUILD)/makedefs.minisip.mk

SS_SAMPLE_BUILD_DIR = $(SS_CODE_BUILD)/sample

SS_TPTD_BUILD_DIR   = $(SS_SAMPLE_BUILD_DIR)/tptd
SS_UTIL_BUILD_DIR   = $(SS_SAMPLE_BUILD_DIR)/util


all:
	$(MAKE) --directory=$(SS_TPTD_BUILD_DIR)    all
	$(MAKE) --directory=$(SS_UTIL_BUILD_DIR)    all

clean:
	$(MAKE) --directory=$(SS_TPTD_BUILD_DIR)    clean
	$(MAKE) --directory=$(SS_UTIL_BUILD_DIR)    clean

cleanlib:
	$(MAKE) --directory=$(SS_TPTD_BUILD_DIR)    cleanlib
	$(MAKE) --directory=$(SS_UTIL_BUILD_DIR)    cleanlib
