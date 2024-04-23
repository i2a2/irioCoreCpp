TOP_FOLDER = ./src

# Linter variables
LINTER := cpplint
LINTER_ARGS := --quiet --recursive
LINTER_SCAN_FOLDER := $(TOP_FOLDER)

# Folder structure variables
MAIN_FOLDER=$(TOP_FOLDER)/main

IRIOCORE_FOLDER=$(MAIN_FOLDER)/c++/irioCore
IRIOCORE_REGEX=\/irio[A-Z][a-zA-Z0-9]+.(cpp|h)$

IRIOCORECPP_FOLDER=$(MAIN_FOLDER)/c++/irioCoreCpp
IRIOCORECPP_INCLUDE_FOLDER=$(IRIOCORECPP_FOLDER)/include

IRIOCORECPP_INCLUDE_PROFILES_FOLDER=$(IRIOCORECPP_INCLUDE_FOLDER)/profiles
IRIOCORECPP_INCLUDE_PROFILES_REGEX=\/profile(s|[A-Z])[a-zA-Z0-9]*.h$

IRIOCORECPP_INCLUDE_TERMINALS_FOLDER=$(IRIOCORECPP_INCLUDE_FOLDER)/terminals
IRIOCORECPP_INCLUDE_TERMINALS_REGEX=\/terminals([A-Z][a-zA-Z0-9]*)?.h$

IRIOCORECPP_INCLUDE_TERMINALS_IMPL_FOLDER=$(IRIOCORECPP_INCLUDE_TERMINALS_FOLDER)/impl
IRIOCORECPP_INCLUDE_TERMINALS_IMPL_REGEX=\/terminals[A-Z][a-zA-Z0-9]+Impl.h$

IRIOCORECPP_INCLUDE_TERMINALS_NAMES_FOLDER=$(IRIOCORECPP_INCLUDE_TERMINALS_FOLDER)/names
IRIOCORECPP_INCLUDE_TERMINALS_NAMES_REGEX=\/namesTerminals[A-Z][a-zA-Z0-9]+.h$

IRIOCORECPP_PROFILES_FOLDER=$(IRIOCORECPP_FOLDER)/profiles
IRIOCORECPP_PROFILES_REGEX=\/profile[A-Z][a-zA-Z0-9]*.cpp$

IRIOCORECPP_TERMINALS_FOLDER=$(IRIOCORECPP_FOLDER)/terminals
IRIOCORECPP_TERMINALS_REGEX=\/terminals[A-Z][a-zA-Z0-9]*.cpp$

IRIOCORECPP_TERMINALS_IMPL_FOLDER=$(IRIOCORECPP_TERMINALS_FOLDER)/impl
IRIOCORECPP_TERMINALS_IMPL_REGEX=\/terminals[A-Z][a-zA-Z0-9]+Impl.cpp$

NIFPGACD_FOLDER=$(MAIN_FOLDER)/c++/NiFpga_CD
NIFPGACF_REGEX=\/NiFpga\.(h|c)$

define check_files_regex
	@printf "$(BOLD)Checking $(1) file structure...$(NC)\n"
	@files_found=$$(find $(1) $(2) -type f $(3)); \
	if [ -n "$$files_found" ]; then \
		invalid_files=$$(echo "$$files_found" | grep -v -E "$(4)"); \
		if [ -n "$$invalid_files" ]; then \
			>&2 printf "$(BOLD)$(RED)Files in $(1) do not match the expected pattern $(4)\n"; \
			>&2 printf "$$invalid_files$(NC)\n"; \
			>&2 printf "$(BOLD)$(1) file structure failed...$(NC)\n"; \
			exit 2; \
		fi \
	else \
		>&2 printf "$(BOLD)$(RED)Files not found in $(1)$(NC)\n"; \
		>&2 printf "$(BOLD)$(1) file structure failed...$(NC)\n"; \
		exit 3; \
	fi
endef

all: folder_structure linting

linting:
	$(LINTER) $(LINTER_ARGS) $(LINTER_SCAN_FOLDER)

folder_structure: | main_structure iriocore_structure iriocorecpp_structure nifpgacd

main_structure:
	@printf "$(BOLD)Checking $(MAIN_FOLDER) file structure...$(NC)\n"
	@files_found=$$(find $(MAIN_FOLDER) -maxdepth 1 -type f \( -name '*.h' -o -name '*.c' -o -name '*.cpp' \)); \
	if [ -n "$$files_found" ]; then \
		>&2 printf "$(BOLD)$(RED)Error: Found .h or .c or .cpp files in $(MAIN_FOLDER)\n"; \
		>&2 printf "$$files_found$(NC)\n"; \
		>&2 printf "$(BOLD)$(MAIN_FOLDER) file structure failed...$(NC)\n"; \
		exit 1; \
	fi

iriocore_structure:
	$(call check_files_regex,$(IRIOCORE_FOLDER),,\( -name '*.h' -o -name '*.c' -o -name '*.cpp' \),$(IRIOCORE_REGEX))

iriocorecpp_structure: | iriocorecpp_include iriocorecpp_profiles iriocorecpp_terminals iriocorecpp_terminals_impl

iriocorecpp_include: | iriocorecpp_include_base iriocorecpp_include_profiles iriocorecpp_include_terminals iriocorecpp_include_terminals_imp iriocorecpp_include_terminals_names

iriocorecpp_include_base:
	@printf "$(BOLD)Checking $(IRIOCORECPP_INCLUDE_FOLDER) file structure...$(NC)\n"
	@files_found=$$(find $(IRIOCORECPP_INCLUDE_FOLDER) -type f ! -name '*.h'); \
	if [ -n "$$files_found" ]; then \
		>&2 printf "$(BOLD)$(RED)Error: Found other files than .h in $(IRIOCORECPP_INCLUDE_FOLDER) and their subfolders\n"; \
		>&2 printf "$$files_found$(NC)\n"; \
		>&2 printf "$(BOLD)$(IRIOCORECPP_INCLUDE_FOLDER) file structure failed...$(NC)\n"; \
		exit 4; \
	fi

iriocorecpp_include_profiles:
	$(call check_files_regex,$(IRIOCORECPP_INCLUDE_PROFILES_FOLDER),,\( -name '*.h' \),$(IRIOCORECPP_INCLUDE_PROFILES_REGEX))

iriocorecpp_include_terminals:
	$(call check_files_regex,$(IRIOCORECPP_INCLUDE_TERMINALS_FOLDER),-maxdepth 1,\( -name '*.h' \),$(IRIOCORECPP_INCLUDE_TERMINALS_REGEX))

iriocorecpp_include_terminals_imp: | iriocorecpp_include_terminals_imp_check_names iriocorecpp_include_terminals_imp_equivalent_files

iriocorecpp_include_terminals_imp_check_names:
	$(call check_files_regex,$(IRIOCORECPP_INCLUDE_TERMINALS_IMPL_FOLDER),-maxdepth 1,\( -name '*.h' \),$(IRIOCORECPP_INCLUDE_TERMINALS_IMPL_REGEX))

iriocorecpp_include_terminals_imp_equivalent_files:
	@printf "$(BOLD)Checking if the non Impl files exists in $(IRIOCORECPP_INCLUDE_TERMINALS_IMPL_FOLDER) file structure...$(NC)\n"
	@files_top_folder=$$(find $(IRIOCORECPP_INCLUDE_TERMINALS_IMPL_FOLDER)/.. -maxdepth 1 -type f -name '*.h'); \
	files_to_find=$$(find $(IRIOCORECPP_INCLUDE_TERMINALS_IMPL_FOLDER) -maxdepth 1 -type f -name '*.h'); \
	files_to_find=$$(echo $$files_to_find | grep -Po "\w*.h" | sed -n 's|\(\w*\)Impl\.h|\1|p'); \
	for file in $$files_to_find; do \
		file_found=$$(echo $$files_top_folder | grep -Po /$$file.h); \
		if [ -z "$$file_found" ]; then \
			>&2 printf "$(BOLD)$(RED)Not found $$file equivalent for $${file}Impl.h $(NC)\n"; \
		fi; \
	done

iriocorecpp_include_terminals_names:
	$(call check_files_regex,$(IRIOCORECPP_INCLUDE_TERMINALS_NAMES_FOLDER),-maxdepth 1,\( -name '*.h' \),$(IRIOCORECPP_INCLUDE_TERMINALS_NAMES_REGEX))

iriocorecpp_profiles:
	$(call check_files_regex,$(IRIOCORECPP_PROFILES_FOLDER),-maxdepth 1,\( -name '*.cpp' \),$(IRIOCORECPP_PROFILES_REGEX))

iriocorecpp_terminals:
	$(call check_files_regex,$(IRIOCORECPP_TERMINALS_FOLDER),-maxdepth 1,\( -name '*.cpp' \),$(IRIOCORECPP_TERMINALS_REGEX))

iriocorecpp_terminals_impl: | iriocorecpp_terminals_imp_check_names iriocorecpp_terminals_imp_equivalent_files

iriocorecpp_terminals_imp_check_names:
	$(call check_files_regex,$(IRIOCORECPP_TERMINALS_IMPL_FOLDER),-maxdepth 1,\( -name '*.cpp' \),$(IRIOCORECPP_TERMINALS_IMPL_REGEX))

iriocorecpp_terminals_imp_equivalent_files:
	@printf "$(BOLD)Checking if the non Impl files exists in $(IRIOCORECPP_TERMINALS_IMPL_FOLDER) file structure...$(NC)\n"
	@files_top_folder=$$(find $(IRIOCORECPP_TERMINALS_IMPL_FOLDER)/.. -maxdepth 1 -type f -name '*.cpp'); \
	files_to_find=$$(find $(IRIOCORECPP_TERMINALS_IMPL_FOLDER) -maxdepth 1 -type f -name '*cpp'); \
	files_to_find=$$(echo $$files_to_find | grep -Po "\w*.cpp" | sed -n 's|\(\w*\)Impl\.cpp|\1|p'); \
	for file in $$files_to_find; do \
		file_found=$$(echo $$files_top_folder | grep -Po /$$file.cpp); \
		if [ -z "$$file_found" ]; then \
			>&2 printf "$(BOLD)$(RED)Not found $$file equivalent for $${file}Impl.cpp $(NC)\n"; \
		fi; \
	done

nifpgacd:
	$(call check_files_regex,$(NIFPGACD_FOLDER),-maxdepth 1,\( -name '*.c' -o -name '*.h' -o -name '*.cpp' \),$(NIFPGACF_REGEX))
