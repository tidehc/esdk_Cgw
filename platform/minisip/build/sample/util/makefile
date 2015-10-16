# GNU make Makefile template for individual componets
# Copyrights (c) Huawei Technologies (I) Pvt. Ltd.

#include the common MiniSip code organization definitions
include $(SS_CODE_BUILD)/make.minisip.defs.mk

# Include a common definition file
include $(SS_CODE_BUILD)/makedefs.minisip.mk

SS_UTIL_LIB = libutil

# External Interface headers files exposed by this component
SS_UTIL_LIB_DEBUG_DIR   = $(SS_LIB_DEBUG_DIR)
SS_UTIL_LIB_RELEASE_DIR = $(SS_LIB_RELEASE_DIR)
SS_UTIL_OBJ_DEBUG_DIR   = $(SS_OBJ_DIR)/sample/util/debug
SS_UTIL_OBJ_RELEASE_DIR = $(SS_OBJ_DIR)/sample/util/release
SS_UTIL_SRC_DIR         = $(SS_ROOT)/../$(SS_RELEASE_DIR)/sample/util/src
SS_UTIL_H_DIR           = $(SS_ROOT)/../$(SS_RELEASE_DIR)/sample/util/h

# Set up the include folder directory with Internal headers of the component
SS_UTIL_INCLUDES = $(SS_CC_INCLUDE)$(SS_UTIL_H_DIR)

SS_UTIL_INCLUDES += $(SS_CC_INCLUDE)$(SS_SSP_INCLUDE_DIR)\
                    $(SS_CC_INCLUDE)$(SS_SSP_H_DIR)\
	                $(SS_CC_INCLUDE)$(SS_SMP_INCLUDE_DIR)\
	                $(SS_CC_INCLUDE)$(SS_SMP_H_DIR)\
	                $(SS_CC_INCLUDE)$(SS_DSM_INCLUDE_DIR)\
	                $(SS_CC_INCLUDE)$(SS_CBB_INCLUDE_DIR)

# random util Object List
SS_UTIL_DEBUG_OBJS = $(SS_UTIL_OBJ_DEBUG_DIR)/rand.$(SS_OBJ_SUFFIX) \
                     $(SS_UTIL_OBJ_DEBUG_DIR)/randutil.$(SS_OBJ_SUFFIX)

SS_UTIL_RELEASE_OBJS = $(SS_UTIL_OBJ_RELEASE_DIR)/rand.$(SS_OBJ_SUFFIX) \
	                   $(SS_UTIL_OBJ_RELEASE_DIR)/randutil.$(SS_OBJ_SUFFIX)


# Common Targets

all: $(SS_UTIL_DEBUG_OBJS) $(SS_UTIL_RELEASE_OBJS)
	$(call SS_ARCHIVE,$(SS_UTIL_LIB_DEBUG_DIR),$(SS_UTIL_DEBUG_OBJS),$(SS_UTIL_LIB))
	$(call SS_ARCHIVE,$(SS_UTIL_LIB_RELEASE_DIR),$(SS_UTIL_RELEASE_OBJS),$(SS_UTIL_LIB))

clean:
	$(SS_RMV) $(SS_UTIL_OBJ_DEBUG_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_UTIL_OBJ_RELEASE_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_UTIL_LIB_DEBUG_DIR)/*.$(SS_LIB_SUFFIX)
	$(SS_RMV) $(SS_UTIL_LIB_RELEASE_DIR)/*.$(SS_LIB_SUFFIX)

cleanlib:
	$(SS_RMV) $(SS_UTIL_LIB_DEBUG_DIR)/*.$(SS_LIB_SUFFIX)
	$(SS_RMV) $(SS_UTIL_LIB_RELEASE_DIR)/*.$(SS_LIB_SUFFIX)


# DEBUG OBJECTS BUILDING RULES
$(SS_UTIL_OBJ_DEBUG_DIR)/rand.$(SS_OBJ_SUFFIX): $(SS_UTIL_SRC_DIR)/rand.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_UTIL_INCLUDES),$(SS_UTIL_SRC_DIR),$(SS_UTIL_OBJ_DEBUG_DIR),rand)

$(SS_UTIL_OBJ_DEBUG_DIR)/randutil.$(SS_OBJ_SUFFIX): $(SS_UTIL_SRC_DIR)/randutil.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_UTIL_INCLUDES),$(SS_UTIL_SRC_DIR),$(SS_UTIL_OBJ_DEBUG_DIR),randutil)


# Release Object building rules
$(SS_UTIL_OBJ_RELEASE_DIR)/rand.$(SS_OBJ_SUFFIX): $(SS_UTIL_SRC_DIR)/rand.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_UTIL_INCLUDES),$(SS_UTIL_SRC_DIR),$(SS_UTIL_OBJ_RELEASE_DIR),rand)

$(SS_UTIL_OBJ_RELEASE_DIR)/randutil.$(SS_OBJ_SUFFIX): $(SS_UTIL_SRC_DIR)/randutil.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_UTIL_INCLUDES),$(SS_UTIL_SRC_DIR),$(SS_UTIL_OBJ_RELEASE_DIR),randutil)
