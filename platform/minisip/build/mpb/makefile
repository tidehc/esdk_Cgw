# GNU make Makefile template for individual componets
# Copyrights (c) Huawei Technologies (I) Pvt. Ltd.

#include the common MiniSip code organization definitions
#include $(SS_CODE_BUILD)/make.minisip.defs.mk
include ../make.minisip.defs.mk

# Include a common definition file
#include $(SS_CODE_BUILD)/makedefs.minisip.mk
include ../makedefs.minisip.mk

# External Interface headers files exposed by this component
SS_MPB_LIB_DEBUG_DIR   = $(SS_LIB_DEBUG_DIR)
SS_MPB_LIB_RELEASE_DIR = $(SS_LIB_RELEASE_DIR)
SS_MPB_OBJ_DEBUG_DIR   = $(SS_MPB_OBJ_DIR)/debug
SS_MPB_OBJ_RELEASE_DIR = $(SS_MPB_OBJ_DIR)/release
SS_MPB_SRC_DIR         = $(SS_MPB_ROOT)/src

# Set up the include folder directory with Internal headers of the component
SS_MPB_INCLUDES = $(SS_CC_INCLUDE)$(SS_MPB_ROOT)/include\
                  $(SS_CC_INCLUDE)$(SS_MPB_ROOT)/h
                  
# Append the include folder DIRECTORY with external includes
# This section gives a clear picture of header dependency for the component
SS_MPB_INCLUDES += $(SS_CC_INCLUDE)$(SS_SSP_INCLUDE_DIR)\
                   $(SS_CC_INCLUDE)$(SS_SSP_H_DIR)\
	               $(SS_CC_INCLUDE)$(SS_SMP_INCLUDE_DIR)\
	               $(SS_CC_INCLUDE)$(SS_SMP_H_DIR)\
	               $(SS_CC_INCLUDE)$(SS_MMF_INCLUDE_DIR)\
	               $(SS_CC_INCLUDE)$(SS_DSM_INCLUDE_DIR)\
	               $(SS_CC_INCLUDE)$(SS_DSM_H_DIR)\
	               $(SS_CC_INCLUDE)$(SS_CBB_INCLUDE_DIR)\
	               $(SS_CC_INCLUDE)$(SS_CBB_H_DIR)\
	               $(SS_CC_INCLUDE)$(SS_CODEC_INCLUDE_DIR)\
	               $(SS_CC_INCLUDE)$(SS_CODEC_H_DIR)\
	               $(SS_CC_INCLUDE)$(SS_FRM_INCLUDE_DIR)\
	               $(SS_CC_INCLUDE)$(SS_HDRS_INCLUDE_DIR)\
	               $(SS_CC_INCLUDE)$(SS_FRM_H_DIR)\
	               $(SS_CC_INCLUDE)$(SS_UTIL_INCLUDE_DIR)\
	               $(SS_CC_INCLUDE)$(SS_UTIL_H_DIR)\
	               $(SS_CC_INCLUDE)$(SS_TPT_INCLUDE_DIR)\
	               $(SS_CC_INCLUDE)$(SS_TPT_H_DIR)\
  	               $(SS_CC_INCLUDE)$(SS_TXN_INCLUDE_DIR)\
	               $(SS_CC_INCLUDE)$(SS_TXN_H_DIR)


# MPB Object List
SS_MPB_DEBUG_OBJS =  \
	$(SS_MPB_OBJ_DEBUG_DIR)/ssmultipartbodycodec.$(SS_OBJ_SUFFIX) \
	$(SS_MPB_OBJ_DEBUG_DIR)/ssmpbsipcnthdrutils.$(SS_OBJ_SUFFIX)


SS_MPB_RELEASE_OBJS =  \
	$(SS_MPB_OBJ_RELEASE_DIR)/ssmultipartbodycodec.$(SS_OBJ_SUFFIX) \
	$(SS_MPB_OBJ_RELEASE_DIR)/ssmpbsipcnthdrutils.$(SS_OBJ_SUFFIX)


# Common Targets

all: $(SS_MPB_DEBUG_OBJS) $(SS_MPB_RELEASE_OBJS)
	$(call SS_ARCHIVE,$(SS_MPB_LIB_DEBUG_DIR),$(SS_MPB_DEBUG_OBJS),$(SS_MPB_LIB))
	$(call SS_ARCHIVE,$(SS_MPB_LIB_RELEASE_DIR),$(SS_MPB_RELEASE_OBJS),$(SS_MPB_LIB))

clean:
	$(SS_RMV) $(SS_MPB_OBJ_DEBUG_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_MPB_OBJ_RELEASE_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_MPB_LIB_DEBUG_DIR)/*.$(SS_LIB_SUFFIX)
	$(SS_RMV) $(SS_MPB_LIB_RELEASE_DIR)/*.$(SS_LIB_SUFFIX)

cleanlib:
	$(SS_RMV) $(SS_MPB_LIB_DEBUG_DIR)/*.$(SS_LIB_SUFFIX)
	$(SS_RMV) $(SS_MPB_LIB_RELEASE_DIR)/*.$(SS_LIB_SUFFIX)


# DEBUG OBJECTS BUILDING RULES
$(SS_MPB_OBJ_DEBUG_DIR)/ssmultipartbodycodec.$(SS_OBJ_SUFFIX): $(SS_MPB_SRC_DIR)/ssmultipartbodycodec.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_MPB_INCLUDES),$(SS_MPB_SRC_DIR),$(SS_MPB_OBJ_DEBUG_DIR),ssmultipartbodycodec)

$(SS_MPB_OBJ_DEBUG_DIR)/ssmpbsipcnthdrutils.$(SS_OBJ_SUFFIX): $(SS_MPB_SRC_DIR)/ssmpbsipcnthdrutils.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_MPB_INCLUDES),$(SS_MPB_SRC_DIR),$(SS_MPB_OBJ_DEBUG_DIR),ssmpbsipcnthdrutils)

# Release Object building rules
$(SS_MPB_OBJ_RELEASE_DIR)/ssmultipartbodycodec.$(SS_OBJ_SUFFIX): $(SS_MPB_SRC_DIR)/ssmultipartbodycodec.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_MPB_INCLUDES),$(SS_MPB_SRC_DIR),$(SS_MPB_OBJ_RELEASE_DIR),ssmultipartbodycodec)

$(SS_MPB_OBJ_RELEASE_DIR)/ssmpbsipcnthdrutils.$(SS_OBJ_SUFFIX): $(SS_MPB_SRC_DIR)/ssmpbsipcnthdrutils.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_MPB_INCLUDES),$(SS_MPB_SRC_DIR),$(SS_MPB_OBJ_RELEASE_DIR),ssmpbsipcnthdrutils)
