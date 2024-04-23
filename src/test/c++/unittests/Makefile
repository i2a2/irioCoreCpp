SUBDIRS=$(dir $(wildcard */Makefile))

BOLD=\e[1m
NC=\e[0m

all:

%:
	@$(foreach dir, $(SUBDIRS), printf "$(BOLD)Building unittests $(dir:/=)...$(NC)\n" && $(MAKE) -C $(dir) $@ &&) :
