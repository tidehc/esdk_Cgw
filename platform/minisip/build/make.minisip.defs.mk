# GNU Make Template file for MiniSip specific definitions
# Copyrights Huawei Technologies (I) Pvt. Ltd.

# Transfrom \ to / in the path in SS_ROOT, SS_CODE_BUILD, and SS_SYS_INCLUDE
SS_ROOT        = /home/ivs_cyz_cwx148380/ivs_c02/IVS_DCG/third/SipStack/third/minisip
SS_CODE_BUILD  = $(SS_ROOT)/build
SS_SYS_INCLUDE = $(SS_ROOT)/include
SS_PDT         = ipc
SS_OS_VER      = linux
SS_CPU_VER     = x86
SS_LIB_SUFFIX  = a

# Wrap all the released file in one folder
SS_PROJ_CONFIG_DIR = release

# Generic Component Code Organization Folders
SS_INCLUDE_DIR = include
SS_H_DIR       = h
SS_INC_DIR     = inc
SS_SRC_DIR     = src


# Output directory keywords for libaries
SS_DEBUG   = debug
SS_RELEASE = release


# Library directory keywords and relative output directories
SS_LIB_DIR         = $(SS_ROOT)/lib

SS_LIB_DEBUG_DIR   = $(SS_LIB_DIR)/$(SS_OS_VER)_$(SS_CPU_VER)_$(SS_LIB_SUFFIX)/$(SS_DEBUG)
SS_LIB_RELEASE_DIR = $(SS_LIB_DIR)/$(SS_OS_VER)_$(SS_CPU_VER)_$(SS_LIB_SUFFIX)/$(SS_RELEASE)


# Temporary Object file folders
SS_OBJ_DIR          = $(SS_ROOT)/objs

SS_CORELIB_OBJ_DIR  = $(SS_OBJ_DIR)/corelib
SS_CBB_OBJ_DIR      = $(SS_OBJ_DIR)/cbb
SS_SDP_OBJ_DIR      = $(SS_OBJ_DIR)/sdp
SS_MPB_OBJ_DIR      = $(SS_OBJ_DIR)/mpb
SS_SIGCOMP_OBJ_DIR  = $(SS_OBJ_DIR)/sigcomp


# MiniSip LIBRARY NAMES
SS_TXN_LIB        = libtxn
SS_TPT_LIB        = libtpt
SS_LM_LIB         = liblm
SS_DSM_LIB        = libdsm
SS_CODEC_LIB      = libcodec

SS_CORESIP_LIB    = libsipcore
SS_CBB_LIB        = libsipcbb
SS_SIGCOMP_LIB    = libsipsigcomp
SS_SDP_LIB        = libsipsdp
SS_MPB_LIB        = libsipmpb


# ABSOLUTE Path of the source code
SS_CODE_ROOT      = $(SS_ROOT)/src
SS_CORELIB_ROOT   = $(SS_CODE_ROOT)/corelib
SS_CBB_ROOT       = $(SS_CODE_ROOT)/cbb
SS_SDP_ROOT       = $(SS_CODE_ROOT)/sdp
SS_MPB_ROOT       = $(SS_CODE_ROOT)/extlibs/mpb
SS_SIGCOMP_ROOT   = $(SS_CODE_ROOT)/extlibs/sigcomp


# MiniSip INCLUDE FOLDERS shared for compilation

SS_CORELIB_BUILD_DIR         = $(SS_CODE_BUILD)/corelib
SS_CBB_BUILD_DIR             = $(SS_CODE_BUILD)/cbb
SS_EXTLIBS_SDP_BUILD_DIR     = $(SS_CODE_BUILD)/sdp
SS_EXTLIBS_MPB_BUILD_DIR     = $(SS_CODE_BUILD)/mpb
SS_EXTLIBS_SIGCOMP_BUILD_DIR = $(SS_CODE_BUILD)/sigcomp
SS_SAMPLE_BUILD_DIR          = $(SS_CODE_BUILD)/sample

# Source Location
SS_CBB_DIR             = $(SS_CBB_ROOT)
SS_CBB_INCLUDE_DIR     = $(SS_CBB_DIR)/$(SS_INCLUDE_DIR)
SS_CBB_H_DIR           = $(SS_CBB_DIR)/$(SS_H_DIR)

SS_SIGCOMP_DIR         = $(SS_SIGCOMP_ROOT)
SS_SIGCOMP_INCLUDE_DIR = $(SS_SIGCOMP_DIR)/$(SS_INCLUDE_DIR)
SS_SIGCOMP_H_DIR       = $(SS_SIGCOMP_DIR)/$(SS_H_DIR)

SS_TXN_DIR             = $(SS_CORELIB_ROOT)/transaction
SS_TXN_INCLUDE_DIR     = $(SS_TXN_DIR)/$(SS_INCLUDE_DIR)
SS_TXN_H_DIR           = $(SS_TXN_DIR)/$(SS_H_DIR)

SS_TPT_DIR             = $(SS_CORELIB_ROOT)/transport
SS_TPT_INCLUDE_DIR     = $(SS_TPT_DIR)/$(SS_INCLUDE_DIR)
SS_TPT_H_DIR           = $(SS_TPT_DIR)/$(SS_H_DIR)

SS_SMP_DIR             = $(SS_CORELIB_ROOT)/smp
SS_SMP_INCLUDE_DIR     = $(SS_SMP_DIR)/$(SS_INCLUDE_DIR)
SS_SMP_H_DIR           = $(SS_SMP_DIR)/$(SS_H_DIR)

SS_SSP_DIR             = $(SS_CORELIB_ROOT)/ssp
SS_SSP_INCLUDE_DIR     = $(SS_SSP_DIR)/$(SS_INCLUDE_DIR)
SS_SSP_H_DIR           = $(SS_SSP_DIR)/$(SS_H_DIR)

SS_MMF_DIR             = $(SS_CORELIB_ROOT)/mmf
SS_MMF_INCLUDE_DIR     = $(SS_MMF_DIR)/$(SS_INCLUDE_DIR)

SS_DSM_DIR             = $(SS_MMF_DIR)/dsm
SS_DSM_INCLUDE_DIR     = $(SS_DSM_DIR)/$(SS_INCLUDE_DIR)
SS_DSM_H_DIR           = $(SS_DSM_DIR)/$(SS_H_DIR)
SS_DSM_INC_DIR         = $(SS_DSM_DIR)/$(SS_INC_DIR)

SS_CODEC_DIR           = $(SS_MMF_DIR)/codec
SS_CODEC_INCLUDE_DIR   = $(SS_CODEC_DIR)/$(SS_INCLUDE_DIR)
SS_CODEC_H_DIR         = $(SS_CODEC_DIR)/$(SS_H_DIR)

SS_FRM_DIR             = $(SS_MMF_DIR)/frm
SS_FRM_INCLUDE_DIR     = $(SS_FRM_DIR)/$(SS_INCLUDE_DIR)
SS_FRM_H_DIR           = $(SS_FRM_DIR)/$(SS_H_DIR)

SS_HDRS_DIR            = $(SS_MMF_DIR)/hdrs
SS_HDRS_INCLUDE_DIR    = $(SS_HDRS_DIR)/$(SS_INCLUDE_DIR)
SS_HDRS_H_DIR          = $(SS_HDRS_DIR)/$(SS_H_DIR)

SS_SDP_DIR             = $(SS_SDP_ROOT)
SS_SDP_INCLUDE_DIR     = $(SS_SDP_DIR)/$(SS_INCLUDE_DIR)
SS_SDP_H_DIR           = $(SS_SDP_DIR)/$(SS_H_DIR)

