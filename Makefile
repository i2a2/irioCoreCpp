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
PACKAGE_MK = ./workflowStages/packaging/
INSTALL_MK = ./workflowStages/install/

ifdef Debug
	COMPILE_DEBUG="COVERAGE=true"
endif

.NOTPARALLEL: copy clean test package verify

all: compile

info:
	@printf "Available recipes:\n"
	@printf "<name recipe>: [<recipes executed as prerequisites>]\n"
	@printf "\t <description>\n"
	@printf "\t\t *<defines>: <If defined, how they modify the behaviour of the recipe>\n\n"
	@printf "\t copy:\n"
	@printf "\t\t Copies $(SOURCE_DIR) into $(COPY_DIR). If it does not exist, the folder will be created\n"
	@printf "\t clean:\n"
	@printf "\t\t Clean the project. Removes $(COPY_DIR) folder\n"
	@printf "\t verify:\n"
	@printf "\t\t Enforces Google's C++ style guidelines and file name and directory structure\n"
	@printf "\t\t\t *SkipVerify: If defined, the verify stage is skipped\n"
	@printf "\t compile: [verify, copy]\n"
	@printf "\t\t Compiles the project's code\n"
	@printf "\t\t\t *Debug: If defined, the code is compiled with debugging symbols\n"
	@printf "\t debug: [compile]\n"
	@printf "\t\t Compiles the project's code with debugging symbols\n"
	@printf "\t\t Equivalent to 'make compile Debug=1'\n"
	@printf "\t test: [compile]\n"
	@printf "\t\t Run tests.\n"
	@printf "\t\t If no variable defined. Only unittests are run.\n"
	@printf "\t\t\t *SkipTests: If defined, the test stage is skipped\n"
	@printf "\t\t\t *AddTestsFunctionalIrioCore: Adds functional irioCore tests\n"
	@printf "\t\t\t *AddTestsFunctionalIrioCoreCpp: Adds functional irioCore tests\n"
	@printf "\t quality: [debug, test]\n"
	@printf "\t\t Calculates the project's coverage\n"
	@printf "\t coverage: [quality]\n"
	@printf "\t\t Alias to quality recipe\n"
	@printf "\t doc: [copy]\n"
	@printf "\t\t Generates Doxygen documentation in $(COPY_DIR)doc/irioCore\n"
	@printf "\t package: [compile]\n"
	@printf "\t\t Generates rpms\n"
	@printf "\t\t\t *INSTALL_DIR: This optional variable sets the install location.\n"
	@printf "\t\t\t\t Libraries will be placed in INSTALL_DIR/lib and headers in INSTALL_DIR/include.\n"
	@printf "\t\t\t\t If INSTALL_DIR is not set, it defaults to /usr/local, unless CODAC_ROOT is set, in which case INSTALL_DIR will use the value of CODAC_ROOT.\n"
	@printf "\t install: [compile]\n"
	@printf "\t\t Copies the libraries and headers to the INSTALL_DIR directory\n"
	@printf "\t\t\t *INSTALL_DIR: (Required) Install location. Libraries will be placed in INSTALL_DIR/lib and headers in INSTALL_DIR/include.\n"

copy:
	@echo "Copying $(SOURCE_DIR) to $(COPY_DIR)..."
	rsync -a --inplace $(SOURCE_DIR) $(COPY_DIR)
	@echo "Copy complete."

clean:
	@printf "$(BOLD)Cleaning \"$(COPY_DIR)\"...$(NC)\n"
	rm -rf $(COPY_DIR)
	@echo "\"$(COPY_DIR)\" cleaned."

verify:
	@set -e; \
	if [ -z $(SkipVerify) ] || ! [ $(SkipVerify) != 0 ] ; then \
		printf "$(BOLD)VERIFY STAGE...$(NC)\n"; \
		$(MAKE) -k -f $(VERIFY_MK); \
		printf "$(BOLD)VERIFY STAGE SUCCESS!$(NC)\n"; \
	else \
		printf "$(BOLD)$(ORANGE)VERIFY STAGE SKIPPED!$(NC)\n"; \
	fi

compile: verify copy
	@printf "$(BOLD)COMPILE STAGE...$(NC)\n"
	@$(MAKE) --no-print-directory -f $(COMPILE_MK) DEBUG=$(COMPILE_DEBUG)
	@printf "$(BOLD)COMPILE STAGE SUCCESS!$(NC)\n"

debug: COMPILE_DEBUG="COVERAGE=true"
debug: compile

test: compile 
	@set -e;\
	if [ -z $(SkipTests) ] || ! [ $(SkipTests) != 0 ]; then \
		printf "$(BOLD)TEST STAGE...$(NC)\n";\
		$(MAKE) --no-print-directory -f $(TEST_MK) AddTestsFunctionalIrioCore=$(AddTestsFunctionalIrioCore) AddTestsFunctionalIrioCoreCpp=$(AddTestsFunctionalIrioCoreCpp);\
		printf "$(BOLD)TEST STAGE SUCCESS!$(NC)\n";\
	else \
		printf "$(BOLD)$(ORANGE)TEST STAGE SKIPPED!$(NC)\n";\
		exit 0;\
	fi

quality: | debug test
	@printf "$(BOLD)QUALITY/COVERAGE STAGE...$(NC)\n"
	@$(MAKE) --no-print-directory -f $(QUALITY_MK)
	@printf "$(BOLD)QUALITY/COVERAGE STAGE SUCCESS!$(NC)\n"

coverage: quality

doc: copy
	@printf "$(BOLD)DOCUMENTATION STAGE...$(NC)\n"
	$(MAKE) --no-print-directory -f $(DOCUMENTATION_MK)
	@printf "$(BOLD)DOCUMENTATION STAGE SUCCESS!$(NC)\n"

package: | compile test doc
	@printf "$(BOLD)PACKAGING STAGE...$(NC)\n"
	$(MAKE) -C $(PACKAGE_MK)
	@printf "$(BOLD)ALL PACKAGES GENERATED!$(NC)\n"
	@printf "$(BOLD)PACKAGING STAGE SUCCESS!$(NC)\n"

install: compile test
	$(MAKE) -C $(INSTALL_MK)
	
