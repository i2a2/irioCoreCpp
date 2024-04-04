SUBDIRS=$(dir $(wildcard */Makefile))

BOLD=\e[1m
NC=\e[0m

all:

%:
	@$(foreach dir, $(SUBDIRS), echo -e "$(BOLD)Building unittests $(dir:/=)...$(NC)" && $(MAKE) -C $(dir) $@ &&) :
