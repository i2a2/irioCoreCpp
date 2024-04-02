PACKAGES_MAKEFILES=package_bfp.mk package_bfp_devel.mk package_iriov2cpp.mk package_iriov2cpp_devel.mk

BOLD=\e[1m
NC=\e[0m

all:

%:
	@$(foreach mk, $(PACKAGES_MAKEFILES), echo -e "$(BOLD)Packaging $(mk)...$(NC)" && $(MAKE) -f $(mk) $@ &&) :
