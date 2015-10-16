######### CoreLib makefile includes TXN, TPT, CODEC, DSM,and LM #############3

#1. Include makefile definitions
#include the common MiniSip code organization definitions
#include $(SS_CODE_BUILD)/make.minisip.defs.mk
include ../make.minisip.defs.mk

# Include a common definition file
#include $(SS_CODE_BUILD)/makedefs.minisip.mk
include ../makedefs.minisip.mk

#2. Define directory macros
SS_TXN_BUILD_DIR         = $(SS_CORELIB_BUILD_DIR)/transaction
SS_TPT_BUILD_DIR         = $(SS_CORELIB_BUILD_DIR)/transport
SS_CODEC_BUILD_DIR       = $(SS_CORELIB_BUILD_DIR)/codec
SS_LM_BUILD_DIR          = $(SS_CORELIB_BUILD_DIR)/lm
SS_DSM_BUILD_DIR         = $(SS_CORELIB_BUILD_DIR)/dsm
SS_CORELIB_DEBUG_DIR     = $(SS_LIB_DEBUG_DIR)
SS_CORELIB_RELEASE_DIR   = $(SS_LIB_RELEASE_DIR)

SS_CODEC_OBJ_DEBUG_DIR   = $(SS_CORELIB_OBJ_DIR)/codec/debug
SS_CODEC_OBJ_RELEASE_DIR = $(SS_CORELIB_OBJ_DIR)/codec/release
SS_DSM_OBJ_DEBUG_DIR     = $(SS_CORELIB_OBJ_DIR)/dsm/debug
SS_DSM_OBJ_RELEASE_DIR   = $(SS_CORELIB_OBJ_DIR)/dsm/release
SS_LM_OBJ_DEBUG_DIR      = $(SS_CORELIB_OBJ_DIR)/lm/debug
SS_LM_OBJ_RELEASE_DIR    = $(SS_CORELIB_OBJ_DIR)/lm/release
SS_TXN_OBJ_DEBUG_DIR     = $(SS_CORELIB_OBJ_DIR)/transaction/debug
SS_TXN_OBJ_RELEASE_DIR   = $(SS_CORELIB_OBJ_DIR)/transaction/release
SS_TPT_OBJ_DEBUG_DIR     = $(SS_CORELIB_OBJ_DIR)/transport/debug
SS_TPT_OBJ_RELEASE_DIR   = $(SS_CORELIB_OBJ_DIR)/transport/release

# Codec Source Location
SS_FRM_SRC_DIR           = $(SS_FRM_DIR)/$(SS_SRC_DIR)
SS_CODEC_SRC_DIR         = $(SS_CODEC_DIR)/$(SS_SRC_DIR)
SS_HDRS_SRC_DIR          = $(SS_HDRS_DIR)/$(SS_SRC_DIR)
SS_FRM_INCLUDE_DIR       = $(SS_FRM_DIR)/$(SS_INCLUDE_DIR)
SS_CODEC_INCLUDE_DIR     = $(SS_CODEC_DIR)/$(SS_INCLUDE_DIR)
SS_HDRS_INCLUDE_DIR      = $(SS_HDRS_DIR)/$(SS_INCLUDE_DIR)
SS_CODEC_LIB_DEBUG_DIR   = $(SS_LIB_DEBUG_DIR)
SS_CODEC_LIB_RELEASE_DIR = $(SS_LIB_RELEASE_DIR)
SS_CODEC_OBJ_DEBUG_DIR   = $(SS_CORELIB_OBJ_DIR)/codec/debug
SS_CODEC_OBJ_RELEASE_DIR = $(SS_CORELIB_OBJ_DIR)/codec/release

# DSM Source Location
SS_DSM_SRC_DIR           = $(SS_DSM_DIR)/$(SS_SRC_DIR)
SS_DSM_INCLUDE_DIR       = $(SS_DSM_DIR)/$(SS_INCLUDE_DIR)
SS_DSM_LIB_DEBUG_DIR     = $(SS_LIB_DEBUG_DIR)
SS_DSM_LIB_RELEASE_DIR   = $(SS_LIB_RELEASE_DIR)
SS_DSM_OBJ_DEBUG_DIR     = $(SS_CORELIB_OBJ_DIR)/dsm/debug
SS_DSM_OBJ_RELEASE_DIR   = $(SS_CORELIB_OBJ_DIR)/dsm/release

# LM Source Location
SS_LM_SRC_DIR            = $(SS_SMP_DIR)/src
SS_LM_LIB_DEBUG_DIR      = $(SS_LIB_DEBUG_DIR)
SS_LM_LIB_RELEASE_DIR    = $(SS_LIB_RELEASE_DIR)
SS_LM_OBJ_DEBUG_DIR      = $(SS_CORELIB_OBJ_DIR)/lm/debug
SS_LM_OBJ_RELEASE_DIR    = $(SS_CORELIB_OBJ_DIR)/lm/release

# TXN Source Location
SS_TXN_SRC_DIR           = $(SS_TXN_DIR)/src
SS_TXN_LIB_DEBUG_DIR     = $(SS_LIB_DEBUG_DIR)
SS_TXN_LIB_RELEASE_DIR   = $(SS_LIB_RELEASE_DIR)
SS_TXN_OBJ_DEBUG_DIR     = $(SS_CORELIB_OBJ_DIR)/transaction/debug
SS_TXN_OBJ_RELEASE_DIR   = $(SS_CORELIB_OBJ_DIR)/transaction/release

# TPT Source Location
SS_TPT_SRC_DIR = $(SS_TPT_DIR)/$(SS_SRC_DIR)
SS_TPT_INCLUDE_DIR       = $(SS_TPT_DIR)/$(SS_INCLUDE_DIR)
SS_TPT_LIB_DEBUG_DIR     = $(SS_LIB_DEBUG_DIR)
SS_TPT_LIB_RELEASE_DIR   = $(SS_LIB_RELEASE_DIR)
SS_TPT_OBJ_DEBUG_DIR     = $(SS_CORELIB_OBJ_DIR)/transport/debug

#3. Include files for CoreLib modules
# Codec Internal Header dependencies
SS_CODEC_INCLUDES        = $(SS_CC_INCLUDE)$(SS_FRM_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_FRM_DIR)/$(SS_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_FRM_DIR)/$(SS_INC_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CODEC_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CODEC_DIR)/$(SS_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CODEC_DIR)/$(SS_INC_DIR)\
                           $(SS_CC_INCLUDE)$(SS_HDRS_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_HDRS_DIR)/$(SS_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_HDRS_DIR)/$(SS_INC_DIR)

# Codec External Header dependencies
SS_CODEC_INCLUDES       += $(SS_CC_INCLUDE)$(SS_SSP_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_SSP_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_SMP_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_SMP_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_DSM_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_DSM_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CBB_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CBB_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TXN_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TXN_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TPT_INCLUDE_DIR)

# DSM Internal Header dependencies
SS_DSM_INCLUDES          = $(SS_CC_INCLUDE)$(SS_DSM_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_DSM_DIR)/$(SS_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_DSM_DIR)/$(SS_INC_DIR)

# DSM External Header dependencies
SS_DSM_INCLUDES         += $(SS_CC_INCLUDE)$(SS_SSP_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_SSP_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_SMP_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_SMP_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_DSM_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CBB_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_FRM_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_FRM_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TXN_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TXN_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_UTIL_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_UTIL_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TPT_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TPT_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_HDRS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CODEC_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CODEC_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_DSM_INC_DIR)

# LM Internal Header dependencies
SS_LM_INCLUDES           = $(SS_CC_INCLUDE)$(SS_SMP_DIR)/$(SS_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_SMP_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_SSP_DIR)/$(SS_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_SSP_DIR)/$(SS_INCLUDE_DIR)

# LM External Header dependencies
SS_LM_INCLUDES          += $(SS_CC_INCLUDE)$(SS_TXN_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TXN_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_MMF_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_DSM_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_DSM_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CBB_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CBB_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CODEC_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CODEC_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_FRM_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_FRM_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_UTIL_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_UTIL_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TPT_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TPT_H_DIR)

# TXN Internal Header dependencies
SS_TXN_INCLUDES          = $(SS_CC_INCLUDE)$(SS_TXN_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TXN_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TXN_DIR)/$(SS_INC_DIR)

# TXN External Header dependencies
SS_TXN_INCLUDES         += $(SS_CC_INCLUDE)$(SS_SSP_INCLUDE_DIR)\
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
                           $(SS_CC_INCLUDE)$(SS_FRM_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TPT_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TPT_H_DIR)

# Set up the include folder directory with Internal headers of the component
SS_TPT_INCLUDES          = $(SS_CC_INCLUDE)$(SS_TPT_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TPT_DIR)/$(SS_H_DIR)

SS_TPT_INCLUDES         += $(SS_CC_INCLUDE)$(SS_SSP_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_SSP_DIR)/$(SS_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_SMP_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_SMP_DIR)/$(SS_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_DSM_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_DSM_DIR)/$(SS_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_MMF_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CODEC_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_FRM_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_FRM_DIR)/$(SS_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_CBB_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TXN_DIR)/$(SS_INCLUDE_DIR)\
                           $(SS_CC_INCLUDE)$(SS_TXN_DIR)/$(SS_H_DIR)\
                           $(SS_CC_INCLUDE)$(SS_UTIL_DIR)/$(SS_H_DIR)

#4. CoreLib OBJ file definitions
# Codec Debug Object List
SS_CODEC_DEBUG_OBJS      = $(SS_CODEC_OBJ_DEBUG_DIR)/ssfrmreg.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sscodeclevel1.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sscodeclevel2.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sscodecutil.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrallow.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrauthinfo.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrauthrzclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrcntnttypeclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrcseq.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrdate.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrevntclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrfromclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrgenparamslistclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrmimever.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrnumclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrprivacy.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrpxyauthenclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrrack.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrretryafterclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrserverclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrstringclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrstringgenparamsclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrtimestamp.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrtokenclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrtokengenparamsclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrurihdrclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrvia.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_DEBUG_DIR)/sshdrwarning.$(SS_OBJ_SUFFIX)

# Codec Release Object List
SS_CODEC_RELEASE_OBJS    = $(SS_CODEC_OBJ_RELEASE_DIR)/ssfrmreg.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sscodeclevel1.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sscodeclevel2.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sscodecutil.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrallow.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrauthinfo.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrauthrzclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrcntnttypeclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrcseq.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrdate.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrevntclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrfromclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrgenparamslistclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrmimever.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrnumclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrprivacy.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrpxyauthenclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrrack.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrretryafterclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrserverclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrstringclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrstringgenparamsclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrtimestamp.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrtokenclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrtokengenparamsclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrurihdrclass.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrvia.$(SS_OBJ_SUFFIX) \
                           $(SS_CODEC_OBJ_RELEASE_DIR)/sshdrwarning.$(SS_OBJ_SUFFIX)


# DSM Debug Object List
SS_DSM_DEBUG_OBJS        = $(SS_DSM_OBJ_DEBUG_DIR)/ssdsmhdrs.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_DEBUG_DIR)/ssdsmmetadata.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_DEBUG_DIR)/ssdsmmetadataop.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_DEBUG_DIR)/ssdsmmsgcloneutil.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_DEBUG_DIR)/ssdsmmsgs.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_DEBUG_DIR)/ssdsmunknownhdrs.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_DEBUG_DIR)/ssdsmmsgutil.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_DEBUG_DIR)/ssdsmutil.$(SS_OBJ_SUFFIX)

# DSM Release Object List
SS_DSM_RELEASE_OBJS      = $(SS_DSM_OBJ_RELEASE_DIR)/ssdsmhdrs.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_RELEASE_DIR)/ssdsmmetadata.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_RELEASE_DIR)/ssdsmmetadataop.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_RELEASE_DIR)/ssdsmmsgcloneutil.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_RELEASE_DIR)/ssdsmmsgs.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_RELEASE_DIR)/ssdsmunknownhdrs.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_RELEASE_DIR)/ssdsmmsgutil.$(SS_OBJ_SUFFIX) \
                           $(SS_DSM_OBJ_RELEASE_DIR)/ssdsmutil.$(SS_OBJ_SUFFIX)

# LM Object List
SS_LM_DEBUG_OBJS         = $(SS_LM_OBJ_DEBUG_DIR)/sslminit.$(SS_OBJ_SUFFIX) \
                           $(SS_LM_OBJ_DEBUG_DIR)/sslmlog.$(SS_OBJ_SUFFIX) \
                           $(SS_LM_OBJ_DEBUG_DIR)/sslmmaintain.$(SS_OBJ_SUFFIX)

SS_LM_RELEASE_OBJS       = $(SS_LM_OBJ_RELEASE_DIR)/sslminit.$(SS_OBJ_SUFFIX) \
                           $(SS_LM_OBJ_RELEASE_DIR)/sslmlog.$(SS_OBJ_SUFFIX) \
                           $(SS_LM_OBJ_RELEASE_DIR)/sslmmaintain.$(SS_OBJ_SUFFIX)

# Transaction Debug Object List
SS_TXN_DEBUG_OBJS        = $(SS_TXN_OBJ_DEBUG_DIR)/sstxnhiintf.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_DEBUG_DIR)/sstxncancel.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_DEBUG_DIR)/sstxnclientfsm.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_DEBUG_DIR)/sstxninit.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_DEBUG_DIR)/sstxnliintf.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_DEBUG_DIR)/sstxnmgmt.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_DEBUG_DIR)/sstxnserverfsm.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_DEBUG_DIR)/sstxntimer.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_DEBUG_DIR)/sstxnutil.$(SS_OBJ_SUFFIX)

# Transaction Release Object List
SS_TXN_RELEASE_OBJS      = $(SS_TXN_OBJ_RELEASE_DIR)/sstxnhiintf.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_RELEASE_DIR)/sstxncancel.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_RELEASE_DIR)/sstxnclientfsm.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_RELEASE_DIR)/sstxninit.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_RELEASE_DIR)/sstxnliintf.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_RELEASE_DIR)/sstxnmgmt.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_RELEASE_DIR)/sstxnserverfsm.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_RELEASE_DIR)/sstxntimer.$(SS_OBJ_SUFFIX) \
                           $(SS_TXN_OBJ_RELEASE_DIR)/sstxnutil.$(SS_OBJ_SUFFIX)

# Transport Debug Object List
SS_TPT_DEBUG_OBJS        = $(SS_TPT_OBJ_DEBUG_DIR)/sstptcore.$(SS_OBJ_SUFFIX) \
                           $(SS_TPT_OBJ_DEBUG_DIR)/sstptoptfeatures.$(SS_OBJ_SUFFIX)

# Transport Release Object List
SS_TPT_RELEASE_OBJS      = $(SS_TPT_OBJ_RELEASE_DIR)/sstptcore.$(SS_OBJ_SUFFIX) \
                           $(SS_TPT_OBJ_RELEASE_DIR)/sstptoptfeatures.$(SS_OBJ_SUFFIX)

SS_CORELIB_DEBUG_OBJS    = $(SS_CODEC_DEBUG_OBJS) $(SS_DSM_DEBUG_OBJS) $(SS_LM_DEBUG_OBJS) $(SS_TXN_DEBUG_OBJS) $(SS_TPT_DEBUG_OBJS)

SS_CORELIB_RELEASE_OBJS  = $(SS_CODEC_RELEASE_OBJS)\
                           $(SS_DSM_RELEASE_OBJS)\
                           $(SS_LM_RELEASE_OBJS)\
                           $(SS_TXN_RELEASE_OBJS)\
                           $(SS_TPT_RELEASE_OBJS)

#5. Make Commands
all: $(SS_CORELIB_DEBUG_OBJS) $(SS_CORELIB_RELEASE_OBJS)
	$(call SS_ARCHIVE,$(SS_CORELIB_DEBUG_DIR),$(SS_CORELIB_DEBUG_OBJS),$(SS_CORESIP_LIB))

	@echo SS_ARCHIVE

	$(call SS_ARCHIVE,$(SS_CORELIB_RELEASE_DIR),$(SS_CORELIB_RELEASE_OBJS),$(SS_CORESIP_LIB))

cleansublibs:
	$(SS_RMV) $(SS_CORELIB_DEBUG_LIBS)
	$(SS_RMV) $(SS_CORELIB_RELEASE_LIBS)

clean:
	$(SS_RMV) $(SS_CODEC_OBJ_DEBUG_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_CODEC_OBJ_RELEASE_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_DSM_OBJ_DEBUG_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_DSM_OBJ_RELEASE_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_LM_OBJ_DEBUG_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_LM_OBJ_RELEASE_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_TXN_OBJ_DEBUG_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_TXN_OBJ_RELEASE_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_TPT_OBJ_DEBUG_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_TPT_OBJ_RELEASE_DIR)/*.$(SS_OBJ_SUFFIX)
	$(SS_RMV) $(SS_CORELIB_DEBUG_DIR)/*.$(SS_LIB_SUFFIX)
	$(SS_RMV) $(SS_CORELIB_RELEASE_DIR)/*.$(SS_LIB_SUFFIX)

cleanlib:
	$(SS_RMV) $(SS_CORELIB_DEBUG_DIR)/*.$(SS_LIB_SUFFIX)
	$(SS_RMV) $(SS_CORELIB_RELEASE_DIR)/*.$(SS_LIB_SUFFIX)


#6. CoreLib OBJ generation rules

# Codec DEBUG OBJECTS BUILDING RULES
$(SS_CODEC_OBJ_DEBUG_DIR)/ssfrmreg.$(SS_OBJ_SUFFIX): $(SS_FRM_SRC_DIR)/ssfrmreg.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_FRM_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),ssfrmreg)

$(SS_CODEC_OBJ_DEBUG_DIR)/sscodeclevel1.$(SS_OBJ_SUFFIX): $(SS_CODEC_SRC_DIR)/sscodeclevel1.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_CODEC_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sscodeclevel1)

$(SS_CODEC_OBJ_DEBUG_DIR)/sscodeclevel2.$(SS_OBJ_SUFFIX): $(SS_CODEC_SRC_DIR)/sscodeclevel2.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_CODEC_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sscodeclevel2)

$(SS_CODEC_OBJ_DEBUG_DIR)/sscodecutil.$(SS_OBJ_SUFFIX): $(SS_CODEC_SRC_DIR)/sscodecutil.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_CODEC_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sscodecutil)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrallow.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrallow.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrallow)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrauthinfo.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrauthinfo.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrauthinfo)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrauthrzclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrauthrzclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrauthrzclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrcntnttypeclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrcntnttypeclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrcntnttypeclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrcseq.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrcseq.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrcseq)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrdate.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrdate.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrdate)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrevntclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrevntclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrevntclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrfromclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrfromclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrfromclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrgenparamslistclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrgenparamslistclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrgenparamslistclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrmimever.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrmimever.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrmimever)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrnumclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrnumclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrnumclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrprivacy.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrprivacy.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrprivacy)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrpxyauthenclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrpxyauthenclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrpxyauthenclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrrack.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrrack.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrrack)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrretryafterclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrretryafterclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrretryafterclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrserverclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrserverclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrserverclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrstringclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrstringclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrstringclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrstringgenparamsclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrstringgenparamsclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrstringgenparamsclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrtimestamp.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrtimestamp.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrtimestamp)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrtokenclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrtokenclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrtokenclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrtokengenparamsclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrtokengenparamsclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrtokengenparamsclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrurihdrclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrurihdrclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrurihdrclass)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrvia.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrvia.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrvia)

$(SS_CODEC_OBJ_DEBUG_DIR)/sshdrwarning.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrwarning.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_DEBUG_DIR),sshdrwarning)

# Codec RELEASE OBJECTS BUILDING RULES
$(SS_CODEC_OBJ_RELEASE_DIR)/ssfrmreg.$(SS_OBJ_SUFFIX): $(SS_FRM_SRC_DIR)/ssfrmreg.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_FRM_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),ssfrmreg)

$(SS_CODEC_OBJ_RELEASE_DIR)/sscodeclevel1.$(SS_OBJ_SUFFIX): $(SS_CODEC_SRC_DIR)/sscodeclevel1.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_CODEC_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sscodeclevel1)

$(SS_CODEC_OBJ_RELEASE_DIR)/sscodeclevel2.$(SS_OBJ_SUFFIX): $(SS_CODEC_SRC_DIR)/sscodeclevel2.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_CODEC_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sscodeclevel2)

$(SS_CODEC_OBJ_RELEASE_DIR)/sscodecutil.$(SS_OBJ_SUFFIX): $(SS_CODEC_SRC_DIR)/sscodecutil.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_CODEC_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sscodecutil)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrallow.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrallow.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrallow)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrauthinfo.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrauthinfo.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrauthinfo)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrauthrzclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrauthrzclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrauthrzclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrcntnttypeclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrcntnttypeclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrcntnttypeclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrcseq.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrcseq.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrcseq)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrdate.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrdate.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrdate)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrevntclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrevntclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrevntclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrfromclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrfromclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrfromclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrgenparamslistclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrgenparamslistclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrgenparamslistclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrmimever.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrmimever.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrmimever)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrnumclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrnumclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrnumclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrprivacy.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrprivacy.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrprivacy)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrpxyauthenclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrpxyauthenclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrpxyauthenclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrrack.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrrack.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrrack)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrretryafterclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrretryafterclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrretryafterclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrserverclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrserverclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrserverclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrstringclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrstringclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrstringclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrstringgenparamsclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrstringgenparamsclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrstringgenparamsclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrtimestamp.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrtimestamp.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrtimestamp)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrtokenclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrtokenclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrtokenclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrtokengenparamsclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrtokengenparamsclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrtokengenparamsclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrurihdrclass.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrurihdrclass.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrurihdrclass)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrvia.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrvia.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrvia)

$(SS_CODEC_OBJ_RELEASE_DIR)/sshdrwarning.$(SS_OBJ_SUFFIX): $(SS_HDRS_SRC_DIR)/sshdrwarning.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_CODEC_INCLUDES),$(SS_HDRS_SRC_DIR),$(SS_CODEC_OBJ_RELEASE_DIR),sshdrwarning)

# DSM DEBUG OBJECTS BUILDING RULES
$(SS_DSM_OBJ_DEBUG_DIR)/ssdsmhdrs.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmhdrs.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_DEBUG_DIR),ssdsmhdrs)

$(SS_DSM_OBJ_DEBUG_DIR)/ssdsmmetadata.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmmetadata.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_DEBUG_DIR),ssdsmmetadata)

$(SS_DSM_OBJ_DEBUG_DIR)/ssdsmmetadataop.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmmetadataop.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_DEBUG_DIR),ssdsmmetadataop)

$(SS_DSM_OBJ_DEBUG_DIR)/ssdsmmsgcloneutil.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmmsgcloneutil.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_DEBUG_DIR),ssdsmmsgcloneutil)

$(SS_DSM_OBJ_DEBUG_DIR)/ssdsmmsgs.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmmsgs.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_DEBUG_DIR),ssdsmmsgs)

$(SS_DSM_OBJ_DEBUG_DIR)/ssdsmunknownhdrs.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmunknownhdrs.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_DEBUG_DIR),ssdsmunknownhdrs)

$(SS_DSM_OBJ_DEBUG_DIR)/ssdsmmsgutil.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmmsgutil.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_DEBUG_DIR),ssdsmmsgutil)

$(SS_DSM_OBJ_DEBUG_DIR)/ssdsmutil.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmutil.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_DEBUG_DIR),ssdsmutil)

# DSM RELEASE OBJECTS BUILDING RULES
$(SS_DSM_OBJ_RELEASE_DIR)/ssdsmhdrs.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmhdrs.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_RELEASE_DIR),ssdsmhdrs)

$(SS_DSM_OBJ_RELEASE_DIR)/ssdsmmetadata.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmmetadata.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_RELEASE_DIR),ssdsmmetadata)

$(SS_DSM_OBJ_RELEASE_DIR)/ssdsmmetadataop.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmmetadataop.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_RELEASE_DIR),ssdsmmetadataop)

$(SS_DSM_OBJ_RELEASE_DIR)/ssdsmmsgcloneutil.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmmsgcloneutil.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_RELEASE_DIR),ssdsmmsgcloneutil)

$(SS_DSM_OBJ_RELEASE_DIR)/ssdsmmsgs.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmmsgs.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_RELEASE_DIR),ssdsmmsgs)

$(SS_DSM_OBJ_RELEASE_DIR)/ssdsmunknownhdrs.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmunknownhdrs.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_RELEASE_DIR),ssdsmunknownhdrs)

$(SS_DSM_OBJ_RELEASE_DIR)/ssdsmmsgutil.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmmsgutil.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_RELEASE_DIR),ssdsmmsgutil)

$(SS_DSM_OBJ_RELEASE_DIR)/ssdsmutil.$(SS_OBJ_SUFFIX): $(SS_DSM_SRC_DIR)/ssdsmutil.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_DSM_INCLUDES),$(SS_DSM_SRC_DIR),$(SS_DSM_OBJ_RELEASE_DIR),ssdsmutil)

# LM DEBUG OBJECTS BUILDING RULES
$(SS_LM_OBJ_DEBUG_DIR)/sslminit.$(SS_OBJ_SUFFIX): $(SS_LM_SRC_DIR)/sslminit.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_LM_INCLUDES),$(SS_LM_SRC_DIR),$(SS_LM_OBJ_DEBUG_DIR),sslminit)

$(SS_LM_OBJ_DEBUG_DIR)/sslmlog.$(SS_OBJ_SUFFIX): $(SS_LM_SRC_DIR)/sslmlog.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_LM_INCLUDES),$(SS_LM_SRC_DIR),$(SS_LM_OBJ_DEBUG_DIR),sslmlog)

$(SS_LM_OBJ_DEBUG_DIR)/sslmmaintain.$(SS_OBJ_SUFFIX): $(SS_LM_SRC_DIR)/sslmmaintain.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_LM_INCLUDES),$(SS_LM_SRC_DIR),$(SS_LM_OBJ_DEBUG_DIR),sslmmaintain)


# LM RELEASE OBJECTS BUILDING RULES
$(SS_LM_OBJ_RELEASE_DIR)/sslminit.$(SS_OBJ_SUFFIX): $(SS_LM_SRC_DIR)/sslminit.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_LM_INCLUDES),$(SS_LM_SRC_DIR),$(SS_LM_OBJ_RELEASE_DIR),sslminit)

$(SS_LM_OBJ_RELEASE_DIR)/sslmlog.$(SS_OBJ_SUFFIX): $(SS_LM_SRC_DIR)/sslmlog.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_LM_INCLUDES),$(SS_LM_SRC_DIR),$(SS_LM_OBJ_RELEASE_DIR),sslmlog)

$(SS_LM_OBJ_RELEASE_DIR)/sslmmaintain.$(SS_OBJ_SUFFIX): $(SS_LM_SRC_DIR)/sslmmaintain.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_LM_INCLUDES),$(SS_LM_SRC_DIR),$(SS_LM_OBJ_RELEASE_DIR),sslmmaintain)

# TXN DEBUG OBJECTS BUILDING RULES
$(SS_TXN_OBJ_DEBUG_DIR)/sstxnhiintf.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxnhiintf.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_DEBUG_DIR),sstxnhiintf)

$(SS_TXN_OBJ_DEBUG_DIR)/sstxncancel.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxncancel.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_DEBUG_DIR),sstxncancel)

$(SS_TXN_OBJ_DEBUG_DIR)/sstxnclientfsm.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxnclientfsm.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_DEBUG_DIR),sstxnclientfsm)

$(SS_TXN_OBJ_DEBUG_DIR)/sstxninit.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxninit.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_DEBUG_DIR),sstxninit)

$(SS_TXN_OBJ_DEBUG_DIR)/sstxnliintf.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxnliintf.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_DEBUG_DIR),sstxnliintf)

$(SS_TXN_OBJ_DEBUG_DIR)/sstxnmgmt.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxnmgmt.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_DEBUG_DIR),sstxnmgmt)

$(SS_TXN_OBJ_DEBUG_DIR)/sstxnserverfsm.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxnserverfsm.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_DEBUG_DIR),sstxnserverfsm)

$(SS_TXN_OBJ_DEBUG_DIR)/sstxntimer.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxntimer.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_DEBUG_DIR),sstxntimer)

$(SS_TXN_OBJ_DEBUG_DIR)/sstxnutil.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxnutil.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_DEBUG_DIR),sstxnutil)

# TXN RELEASE OBJECTS BUILDING RULES
$(SS_TXN_OBJ_RELEASE_DIR)/sstxnhiintf.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxnhiintf.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_RELEASE_DIR),sstxnhiintf)

$(SS_TXN_OBJ_RELEASE_DIR)/sstxncancel.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxncancel.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_RELEASE_DIR),sstxncancel)

$(SS_TXN_OBJ_RELEASE_DIR)/sstxnclientfsm.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxnclientfsm.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_RELEASE_DIR),sstxnclientfsm)

$(SS_TXN_OBJ_RELEASE_DIR)/sstxninit.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxninit.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_RELEASE_DIR),sstxninit)

$(SS_TXN_OBJ_RELEASE_DIR)/sstxnliintf.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxnliintf.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_RELEASE_DIR),sstxnliintf)

$(SS_TXN_OBJ_RELEASE_DIR)/sstxnmgmt.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxnmgmt.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_RELEASE_DIR),sstxnmgmt)

$(SS_TXN_OBJ_RELEASE_DIR)/sstxnserverfsm.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxnserverfsm.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_RELEASE_DIR),sstxnserverfsm)

$(SS_TXN_OBJ_RELEASE_DIR)/sstxntimer.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxntimer.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_RELEASE_DIR),sstxntimer)

$(SS_TXN_OBJ_RELEASE_DIR)/sstxnutil.$(SS_OBJ_SUFFIX): $(SS_TXN_SRC_DIR)/sstxnutil.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_TXN_INCLUDES),$(SS_TXN_SRC_DIR),$(SS_TXN_OBJ_RELEASE_DIR),sstxnutil)

# TPT DEBUG OBJECTS BUILDING RULES
$(SS_TPT_OBJ_DEBUG_DIR)/sstptcore.$(SS_OBJ_SUFFIX): $(SS_TPT_SRC_DIR)/sstptcore.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_TPT_INCLUDES),$(SS_TPT_SRC_DIR),$(SS_TPT_OBJ_DEBUG_DIR),sstptcore)

$(SS_TPT_OBJ_DEBUG_DIR)/sstptoptfeatures.$(SS_OBJ_SUFFIX): $(SS_TPT_SRC_DIR)/sstptoptfeatures.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_DEBUG),$(SS_TPT_INCLUDES),$(SS_TPT_SRC_DIR),$(SS_TPT_OBJ_DEBUG_DIR),sstptoptfeatures)

# TPT RELEASE OBJECTS BUILDING RULES
$(SS_TPT_OBJ_RELEASE_DIR)/sstptcore.$(SS_OBJ_SUFFIX): $(SS_TPT_SRC_DIR)/sstptcore.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_TPT_INCLUDES),$(SS_TPT_SRC_DIR),$(SS_TPT_OBJ_RELEASE_DIR),sstptcore)

$(SS_TPT_OBJ_RELEASE_DIR)/sstptoptfeatures.$(SS_OBJ_SUFFIX): $(SS_TPT_SRC_DIR)/sstptoptfeatures.$(SS_SRC_SUFFIX)
	$(call SS_COMPILE,$(SS_CFLAGS_RELEASE),$(SS_TPT_INCLUDES),$(SS_TPT_SRC_DIR),$(SS_TPT_OBJ_RELEASE_DIR),sstptoptfeatures)
