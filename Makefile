export BOLD=\e[1m
export RED=\e[0;31m
export NC=\e[0m

export VERSION=1.3.0

export COPY_DIR := target/
export SOURCE_DIR := src/

VERIFY_MK = ./workflowStages/verify.mk
COMPILE_MK = ./workflowStages/compile.mk
TEST_MK = ./workflowStages/test.mk
QUALITY_MK = ./workflowStages/quality.mk
PACKAGE_DIR = packaging/

.NOTPARALLEL: copy clean test package verify

all: compile

copy:
	@echo "Copying $(SOURCE_DIR) to $(COPY_DIR)..."
	rsync -a --inplace $(SOURCE_DIR) $(COPY_DIR)
	@echo "Copying complete."

clean:
	@echo -e "$(BOLD)Cleaning \"$(COPY_DIR)\"...$(NC)"
	rm -rf $(COPY_DIR)
	@echo "\"$(COPY_DIR)\" cleaned."

verify:
	@echo -e "$(BOLD)VERIFY STAGE...$(NC)"
	$(MAKE) -k -f $(VERIFY_MK)
	@echo -e "$(BOLD)VERIFY STAGE SUCCESS!$(NC)"

compile: copy
	@if [ "$(skipVerify)" -ne "0" ]; then \
		echo -e "$(BOLD)Skipping verify stage...$(NC)"; \
	else \
		$(MAKE) verify; \
	fi
	@echo -e "$(BOLD)COMPILE STAGE...$(NC)"
	$(MAKE) -f $(COMPILE_MK) DEBUG=$(DEBUG)
	@echo -e "$(BOLD)COMPILE STAGE SUCCESS!$(NC)"

debug:
	@echo -e "$(BOLD)Compiling with debugging symbols...$(NC)"
	$(MAKE) compile DEBUG="COVERAGE=true"

test:
	@echo -e "$(BOLD)TEST STAGE...$(NC)"
	$(MAKE) --no-print-directory -f $(TEST_MK) AddTestsFunctionalIrioCore=$(AddTestsFunctionalIrioCore) AddTestsFunctionalIrioCoreCpp=$(AddTestsFunctionalIrioCoreCpp)
	@echo -e "$(BOLD)TEST STAGE SUCCESS!$(NC)"

quality: | debug test
	@echo -e "$(BOLD)QUALITY/COVERAGE STAGE...$(NC)"
	$(MAKE) --no-print-directory -f $(QUALITY_MK)
	@echo -e "$(BOLD)QUALITY/COVERAGE STAGE SUCCESS!$(NC)"

coverage: quality

doc: copy
	@echo -e "$(BOLD)Generating documentation...$(NC)"
	@mkdir -p $(COPY_DIR)/doc/irioCore
	@sed -i 's/{VERSION_DOXYGEN}/$(VERSION)/g' $(COPY_DIR)/main/c++/doc/Doxyfile
	$(MAKE) -C $(COPY_DIR)/main/c++/doc all	

package: compile
	$(MAKE) -C $(PACKAGE_DIR)
	@echo -e "$(BOLD)ALL PACKAGES GENERATED!$(NC)"

package_debug: debug
	$(MAKE) -C $(PACKAGE_DIR)
	@echo -e "$(BOLD)ALL PACKAGES GENERATED (WITH DEBUG SYMBOLS)!$(NC)"

