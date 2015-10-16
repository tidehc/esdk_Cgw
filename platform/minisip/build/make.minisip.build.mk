# GNU Makefile definitions template for Compiling and linking MiniSip
# This makefile should make the rules practically portable all around


# Generic function for compiling and Archiving.
# $(call SS_COMPILE, debug-or-release-flags, include-dir, source-dir, object-dir, filename-without-extension)
# $(call SS_ARCHIVE, debug-or-release-lib-dir, debug-or-release-objects, libname-without-extension)

# This is to add path for system include files
ifneq ($(SS_SYS_INCLUDE), $(EMPTY))
	SS_SYS_FLAGS = $(SS_CC_INCLUDE)$(SS_SYS_INCLUDE)
endif

# Microsoft Visual C++ 6.0 Command Line Compiler and Linker tools
ifeq ($(SS_OS_VER), win32)

# compiling source files
define SS_COMPILE
	$(SS_WIN32_CC) $(SS_CFLAGS) $(SS_CFLAGS_FEATURES) $(SS_SYS_FLAGS) $(1) $(2) $(SS_WIN32_CC_OUTPUT)$(4)/$(5).$(SS_OBJ_SUFFIX) $(SS_CC_COMPILE) $(3)/$(5).$(SS_SRC_SUFFIX)
endef

# Archiving objects to generate static libraries
define SS_ARCHIVE
	$(SS_WIN32_LIB) $(SS_WIN32_LIBFLAGS) $(SS_WIN32_LIB_OUTPUT)$1/$3.$(SS_WIN32_LIB_SUFFIX) $2
endef


else
# GNU CC and binutils style tools for Tornado, Linux and other UNIX flavors

# compiling source files
define SS_COMPILE
	@mkdir -p $(4)
	$(SS_CC) $(SS_CFLAGS) $(SS_CFLAGS_FEATURES) $(SS_SYS_FLAGS) $(1) $(2) $(SS_CC_COMPILE) $(3)/$(5).$(SS_SRC_SUFFIX) $(SS_CC_OUTPUT) $(4)/$(5).$(SS_OBJ_SUFFIX)
endef

# Archiving objects to generate static libraries
define SS_ARCHIVE
	@mkdir -p $(1)
	$(SS_AR) $(SS_ARFLAGS) $(1)/$(3).$(SS_LIB_SUFFIX) $(2)
endef

endif
