SUBDIRS=legacy/examples

BOLD=\e[1m
NC=\e[0m

all:

%:
	@$(foreach dir, $(SUBDIRS), echo -e "$(BOLD)Building tests $(dir:/=)...$(NC)" && $(MAKE) -C $(dir) $@ &&) :
