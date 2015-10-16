# MASTER GNU Make Template file for a Particular OS and Hardware Platform (CPU)
# Copyrights(c) Huawei Technologies (I) Pvt. Ltd.

#********************h70907: Home gateway Toolchain environment**********
#TOOLCHAIN=/opt/arm-2009q1
#CROSS_COMPILE = $(TOOLCHAIN)/bin/arm-none-linux-gnueabi-

AR              = $(CROSS_COMPILE)ar
AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
CC              = $(CROSS_COMPILE)gcc -m32
CXX             = $(CROSS_COMPILE)g++ -m32
CPP             = $(CROSS_COMPILE)cpp
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
SSTRIP          = $(CROSS_COMPILE)sstrip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump
RANLIB          = $(CROSS_COMPILE)ranlib

SS_CC = $(CC)
SS_AR = $(AR)
SS_LD = $(LD)

#************************************************************************


# ******************* BASIC SHELL TOOLS *****************************
SS_MKDIR = mkdir
SS_RMDIR = rm -Rf
SS_COPY = cp
SS_RMV = rm -rf

# ******************* COMPILER DEFINITIONS **************************
# The Following variable points to the compiler tool on the target
# platform. Please set the PATH varaible proprly anfd not give it
# here in order to maintain readability

#SS_CC = $(CROSS_COMPILE)$(CXX)
SS_CC_COMPILE = -c
SS_CC_OUTPUT = -o
SS_CC_INCLUDE = -I
SS_SRC_SUFFIX = c
SS_OBJ_SUFFIX = obj
SS_CC_DEFINE = -D


# The following define is the nornal CFLAGS that are used for
# generating the build and is to be used in both debug and release
# versions
# -Wstrict-prototypes

SS_CFLAGS += -Wall -Dlinux -D__linux__ -Dunix -D__uClinux__ -DEMBED -fno-builtin  -DLINUX -fomit-frame-pointer -Wall -Wl,-allow-shlib-undefined -fomit-frame-pointer -fno-strict-aliasing -fno-exceptions


# The following defines are for the DEBUG and RELEASE versions of the
# library. Do not put optimization flags in the debug version and the
# debug flags in the release version

SS_CFLAGS_DEBUG = -g -DSS_LOG -DSS_DBG
SS_CFLAGS_RELEASE = -Os


# ******************* ARCHIVER DEFINITIONS **********************
#SS_AR = ar
SS_ARFLAGS = rcs
SS_AROUTPUT =
# SS_LIB_SUFFIX = a

# ******************* LINKER DEFINITIONS *********************

#SS_LD = ld
SS_LDFLAGS =
