PACKAGES_MAKEFILES=package_bfp.mk package_bfp_devel.mk 
PACKAGES_MAKEFILES+=package_iriov2cpp.mk package_iriov2cpp_devel.mk
PACKAGES_MAKEFILES+=package_iriov2cpp_adapter.mk package_iriov2cpp_adapter_devel.mk

ifdef CODAC_ROOT
	BASE_LIB_INSTALL_DIR := $(CODAC_ROOT)/lib
	BASE_INC_INSTALL_DIR := $(CODAC_ROOT)/include
else
	BASE_LIB_INSTALL_DIR := /usr/local/lib
	BASE_INC_INSTALL_DIR := /usr/local/include
endif

export BASE_LIB_INSTALL_DIR
export BASE_INC_INSTALL_DIR


all:

%:
	@$(foreach mk, $(PACKAGES_MAKEFILES), echo -e "$(BOLD)Packaging $(mk)...$(NC)" && $(MAKE) -f $(mk) $@ &&) :
