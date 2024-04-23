SUBDIRS=bfp irioCoreCpp irioCore

BOLD=\e[1m
NC=\e[0m

all:

%:
	@$(foreach dir, $(SUBDIRS), printf "$(BOLD)Building $(dir:/=)...$(NC)\n" && $(MAKE) -C $(dir) $@ &&) :
