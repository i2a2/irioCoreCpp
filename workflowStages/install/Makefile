MODULES_MAKEFILES=install_bfp.mk install_irioCore.mk install_irioCoreCpp.mk

TOP_DIR=../..

ifndef INSTALL_DIR
$(error INSTALL_DIR is not defined)
endif

export INSTALL_DIR
export TOP_DIR

all:

%:
	@$(foreach mk, $(MODULES_MAKEFILES), printf "$(BOLD)Installing $(mk)...$(NC)\n" && $(MAKE) -f $(mk) $@ &&) :