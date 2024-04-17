PACKAGES_MAKEFILES=package_bfp.mk package_bfp_devel.mk 
PACKAGES_MAKEFILES+=package_irioCoreCpp.mk package_irioCoreCpp_devel.mk
PACKAGES_MAKEFILES+=package_irioCore.mk package_irioCore_devel.mk

TOP_DIR=../..

ifdef CODAC_ROOT
	BASE_LIB_INSTALL_DIR := $(CODAC_ROOT)/lib
	BASE_INC_INSTALL_DIR := $(CODAC_ROOT)/include
else
	BASE_LIB_INSTALL_DIR := /usr/local/lib
	BASE_INC_INSTALL_DIR := /usr/local/include
endif

export BASE_LIB_INSTALL_DIR
export BASE_INC_INSTALL_DIR

export TOP_DIR


all:

%:
	@$(foreach mk, $(PACKAGES_MAKEFILES), echo -e "$(BOLD)Packaging $(mk)...$(NC)" && $(MAKE) -f $(mk) $@ &&) :
