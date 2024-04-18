PACKAGES_MAKEFILES=package_bfp.mk package_bfp_devel.mk 
PACKAGES_MAKEFILES+=package_irioCoreCpp.mk package_irioCoreCpp_devel.mk
PACKAGES_MAKEFILES+=package_irioCore.mk package_irioCore_devel.mk

TOP_DIR=../..

ifndef INSTALL_DIR
$(warning INSTALL_DIR is not defined. Installation directory by default.)
ifdef CODAC_ROOT
	INSTALL_DIR := $(CODAC_ROOT)
	INSTALL_DIR := $(CODAC_ROOT)
else
	INSTALL_DIR := /usr/local
	INSTALL_DIR := /usr/local
endif
endif

$(info Install directory: $(INSTALL_DIR))
BASE_LIB_INSTALL_DIR := $(INSTALL_DIR)/lib
BASE_INC_INSTALL_DIR := $(INSTALL_DIR)/include

export BASE_LIB_INSTALL_DIR
export BASE_INC_INSTALL_DIR

export TOP_DIR


all:

%:
	@$(foreach mk, $(PACKAGES_MAKEFILES), echo -e "$(BOLD)Packaging $(mk)...$(NC)" && $(MAKE) -f $(mk) $@ &&) :
	@echo -e "$(BOLD)Install directory: $(INSTALL_DIR)$(NC)"
