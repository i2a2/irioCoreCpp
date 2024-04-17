export BOLD=\e[1m
export RED=\e[0;31m
export ORANGE=\e[0;33m
export NC=\e[0m


export VERSION=1.3.0

export COPY_DIR := target/
export SOURCE_DIR := src/

VERIFY_MK = ./workflowStages/verify.mk
COMPILE_MK = ./workflowStages/compile.mk
TEST_MK = ./workflowStages/test.mk
QUALITY_MK = ./workflowStages/quality.mk
DOCUMENTATION_MK = ./workflowStages/documentation.mk
PACKAGE_DIR = ./workflowStages/packaging/

ifdef Debug
	COMPILE_DEBUG="COVERAGE=true"
endif

.NOTPARALLEL: copy clean test package verify

all: compile

info:
	@echo -e "Available recipes:"
	@echo -e "<name recipe>: [<recipes executed as prerequisites>]"
	@echo -e "\t <description>"
	@echo -e "\t\t *<defines>: <If defined, hwo they modify the behaviour of the recipe>\n"
	@echo -e "\t copy:"
	@echo -e "\t\t Copies $(SOURCE_DIR) into $(COPY_DIR). If it does not exist, the folder will be created"
	@echo -e "\t clean:"
	@echo -e "\t\t Clean the project. Removes $(COPY_DIR) folder"
	@echo -e "\t verify:"
	@echo -e "\t\t Enforces Google's C++ style guidelines and file name and directory structure"
	@echo -e "\t\t\t *SkipVerify: If defined, the verify stage is skipped"
	@echo -e "\t compile: [verify, copy]"
	@echo -e "\t\t Compiles the project's code"
	@echo -e "\t\t\t *Debug: If defined, the code is compiled with debugging symbols"
	@echo -e "\t debug: [compile]"
	@echo -e "\t\t Compiles the project's code with debugging symbols"
	@echo -e "\t\t Equivalent to 'make compile Debug=1'"
	@echo -e "\t test: [compile]"
	@echo -e "\t\t Run tests."
	@echo -e "\t\t If no variable defined. Only unittests are run."
	@echo -e "\t\t\t *SkipTests: If defined, the test stage is skipped"
	@echo -e "\t\t\t *AddTestsFunctionalIrioCore: Adds functional irioCore tests"
	@echo -e "\t\t\t *AddTestsFunctionalIrioCoreCpp: Adds functional irioCore tests"
	@echo -e "\t quality: [debug, test]"
	@echo -e "\t\t Calculates the project's coverage"
	@echo -e "\t coverage: [quality]"
	@echo -e "\t\t Alias to quality recipe"
# TODO: Add missin recipes (doc, package and any extra ones)


copy:
	@echo "Copying $(SOURCE_DIR) to $(COPY_DIR)..."
	rsync -a --inplace $(SOURCE_DIR) $(COPY_DIR)
	@echo "Copying complete."

clean:
	@echo -e "$(BOLD)Cleaning \"$(COPY_DIR)\"...$(NC)"
	rm -rf $(COPY_DIR)
	@echo "\"$(COPY_DIR)\" cleaned."

verify:
	@set -e; \
	if ! [ $(SkipVerify) -ne 0 ]; then \
		echo -e "$(BOLD)VERIFY STAGE...$(NC)"; \
		$(MAKE) -k -f $(VERIFY_MK); \
		echo -e "$(BOLD)VERIFY STAGE SUCCESS!$(NC)"; \
	else \
		echo -e "$(BOLD)$(ORANGE)VERIFY STAGE SKIPPED!$(NC)"; \
	fi

compile: verify copy
	@echo -e "$(BOLD)COMPILE STAGE...$(NC)"
	$(MAKE) -f $(COMPILE_MK) DEBUG=$(COMPILE_DEBUG)
	@echo -e "$(BOLD)COMPILE STAGE SUCCESS!$(NC)"

debug:
	@echo -e "$(BOLD)Compiling with debugging symbols...$(NC)"
	$(MAKE) compile Debug="COVERAGE=true"

test: compile 
	@set -e;\
	if ! [ $(SkipTests) -ne 0 ]; then \
		echo -e "$(BOLD)TEST STAGE...$(NC)";\
		$(MAKE) --no-print-directory -f $(TEST_MK) AddTestsFunctionalIrioCore=$(AddTestsFunctionalIrioCore) AddTestsFunctionalIrioCoreCpp=$(AddTestsFunctionalIrioCoreCpp);\
		echo -e "$(BOLD)TEST STAGE SUCCESS!$(NC)";\
	else \
		echo -e "$(BOLD)$(ORANGE)TEST STAGE SKIPPED!$(NC)";\
		exit 0;\
	fi

quality: | debug test
	@echo -e "$(BOLD)QUALITY/COVERAGE STAGE...$(NC)"
	$(MAKE) --no-print-directory -f $(QUALITY_MK)
	@echo -e "$(BOLD)QUALITY/COVERAGE STAGE SUCCESS!$(NC)"

coverage: quality

doc: copy
	@echo -e "$(BOLD)DOCUMENTATION STAGE...$(NC)"
	$(MAKE) --no-print-directory -f $(DOCUMENTATION_MK)
	@echo -e "$(BOLD)DOCUMENTATION STAGE SUCCESS!$(NC)"

package: compile
	@echo -e "$(BOLD)PACKAGING STAGE...$(NC)"
	$(MAKE) -C $(PACKAGE_DIR)
	@echo -e "$(BOLD)ALL PACKAGES GENERATED!$(NC)"
	@echo -e "$(BOLD)PACKAGING STAGE SUCCESS!$(NC)"

package_debug: debug
	$(MAKE) -C $(PACKAGE_DIR)
	@echo -e "$(BOLD)ALL PACKAGES GENERATED (WITH DEBUG SYMBOLS)!$(NC)"
