SUBDIRS=legacy_examples

BOLD=\e[1m
NC=\e[0m

all:

%:
	@$(foreach dir, $(SUBDIRS), printf "$(BOLD)Building tests $(dir:/=)...$(NC)\n" && $(MAKE) -C $(dir) $@ &&) :
