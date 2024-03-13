SUBDIRS=bfp irio_v2_cpp irio_v2_cpp_compatibility

BOLD=\e[1m
NC=\e[0m

all:

%:
	@$(foreach dir, $(SUBDIRS), echo -e "$(BOLD)Building $(dir:/=)...$(NC)" && $(MAKE) -C $(dir) $@ &&) :
