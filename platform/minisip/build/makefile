# GNU make Makefile template for Top level integrtaed build process for MiniSip
# Copyrights(c) Huawei Technologies (I) Pvt. Ltd.

#include the common MiniSip code organization definitions
include make.minisip.defs.mk
include makedefs.minisip.mk

# Common Targets
# Note that this Top-level makefuile is dependent on the GNU make command line syntax. It is a requirement that
# for buulding MiniSip versions for different platforms, the GNU make tool be used to make the build. It is a
# decison that instead of putting effort on developing Portable makefiles, we use a PORTABLE MAKE ;-)
# One entry must be added for every MiniSip library

all:	
	$(MAKE) --directory=$(SS_CORELIB_BUILD_DIR)          all
	$(MAKE) --directory=$(SS_CBB_BUILD_DIR)              all
	$(MAKE) --directory=$(SS_EXTLIBS_MPB_BUILD_DIR)      all
	$(MAKE) --directory=$(SS_EXTLIBS_SDP_BUILD_DIR)      all
	$(MAKE) --directory=$(SS_EXTLIBS_SIGCOMP_BUILD_DIR)  all
#	$(MAKE) --directory=$(SS_SAMPLE_BUILD_DIR)           all


sample:
	$(MAKE) --directory=$(SS_SAMPLE_BUILD_DIR)           all

clean:
	$(MAKE) --directory=$(SS_CORELIB_BUILD_DIR)          clean
	$(MAKE) --directory=$(SS_CBB_BUILD_DIR)              clean
	$(MAKE) --directory=$(SS_EXTLIBS_MPB_BUILD_DIR)      clean
	$(MAKE) --directory=$(SS_EXTLIBS_SDP_BUILD_DIR)      clean
	$(MAKE) --directory=$(SS_EXTLIBS_SIGCOMP_BUILD_DIR)  clean
#	$(MAKE) --directory=$(SS_SAMPLE_BUILD_DIR)           clean

cleanlib:
	$(MAKE) --directory=$(SS_CORELIB_BUILD_DIR)          cleanlib
	$(MAKE) --directory=$(SS_CBB_BUILD_DIR)              cleanlib
	$(MAKE) --directory=$(SS_EXTLIBS_MPB_BUILD_DIR)      cleanlib
	$(MAKE) --directory=$(SS_EXTLIBS_SDP_BUILD_DIR)      cleanlib
	$(MAKE) --directory=$(SS_EXTLIBS_SIGCOMP_BUILD_DIR)  cleanlib
#	$(MAKE) --directory=$(SS_SAMPLE_BUILD_DIR)           cleanlib
