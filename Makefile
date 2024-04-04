export BOLD=\e[1m
export NC=\e[0m

export VERSION=1.3.0

export COPY_DIR := target/
SOURCE_DIR := src/
LIB_MAKEFILE_DIR := target/main/c++
TEST_MAKEFILE_DIR := target/test/c++
LEGACY_EXAMPLES_DIR := target/test/c

LIB_INSTALL_DIR := /usr/local/lib
INC_INSTALL_DIR := /usr/local/include

COVERAGE_DIR := $(COPY_DIR)/coverage
COVERAGE_INITIAL_FILE := initial.coverage.info
COVERAGE_MAIN_FILE := main.coverage.info
COVERAGE_FILE := coverage.info
COVERAGE_EXCLUDE := '*/NiFpga_CD/*' '*/O.*'

PACKAGE_DIR = packaging/

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif


.PHONY: all debug clean compile coverage package doc

.NOTPARALLEL: copy clean test package

all: copy build

copy:
	@echo "Copying $(SOURCE_DIR) to $(COPY_DIR)..."
	rsync -a --inplace $(SOURCE_DIR) $(COPY_DIR)
	@echo "Copying complete."

build:
	@echo -e "\n$(BOLD)Building libs...$(NC)"
	@echo "Entering $(LIB_MAKEFILE_DIR) and executing make..."
	$(MAKE) -C $(LIB_MAKEFILE_DIR) $(DEBUG)
	@echo "Make in $(LIB_MAKEFILE_DIR) complete."
	
	@echo -e "\n$(BOLD)Building tests...$(NC)"
	@echo "Entering $(TEST_MAKEFILE_DIR) and executing make..."
	$(MAKE) -C $(TEST_MAKEFILE_DIR) $(DEBUG)
	@echo "Make in $(TEST_MAKEFILE_DIR) complete."

	@echo -e "\n$(BOLD)Building legacy examples...$(NC)"
	@echo "Entering $(LEGACY_EXAMPLES_DIR) and executing make..."
	$(MAKE) -C $(LEGACY_EXAMPLES_DIR) $(DEBUG)
	@echo "Make in $(LEGACY_EXAMPLES_DIR) complete."

	@echo -e "$(BOLD)COMPILATION SUCCESSFUL!$(NC)"

clean:
	@echo -e "$(BOLD)Cleaning \"$(COPY_DIR)\"...$(NC)"
	rm -rf $(COPY_DIR)
	@echo "\"$(COPY_DIR)\" cleaned."

debug:
	@echo -e "$(BOLD)Compiling with symbols...$(NC)"
	$(MAKE) all DEBUG="COVERAGE=true"

gen_rpmbuild: copy
	@echo -e "$(BOLD)Generating packages...$(NC)"
	@mkdir -p $(COPY_DIR)/rpmbuild/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}
	@mkdir -p $(COPY_DIR)/rpmbuild/BUILDROOT/iriov2cpp/$(LIB_INSTALL_DIR)
		
doc: copy
	@echo -e "$(BOLD)Generating documentation...$(NC)"
	@mkdir -p $(COPY_DIR)/doc/irioCore
	@sed -i 's/{VERSION_DOXYGEN}/$(VERSION)/g' $(COPY_DIR)/main/c++/doc/Doxyfile
	$(MAKE) -C $(COPY_DIR)/main/c++/doc all	

package: compile
	$(MAKE) -C $(PACKAGE_DIR)
	@echo -e "$(BOLD)ALL PACKAGES GENERATED!$(NC)"
		
test:
	@cd $(COPY_DIR)/test; ./test.sh

coverage: debug test
	@mkdir -p $(COVERAGE_DIR)
	lcov -q --capture --directory $(COPY_DIR)/main/c++ --no-external --initial -o $(COVERAGE_DIR)/$(COVERAGE_INITIAL_FILE)	
	lcov -q --capture --directory $(COPY_DIR)/main/c++ --no-external -o $(COVERAGE_DIR)/$(COVERAGE_MAIN_FILE)	
	lcov -q -a $(COVERAGE_DIR)/$(COVERAGE_INITIAL_FILE) -a $(COVERAGE_DIR)/$(COVERAGE_MAIN_FILE) -o $(COVERAGE_DIR)/$(COVERAGE_FILE)
	lcov -q -r $(COVERAGE_DIR)/$(COVERAGE_FILE) $(COVERAGE_EXCLUDE) -o $(COVERAGE_DIR)/$(COVERAGE_FILE) 
	genhtml $(COVERAGE_DIR)/$(COVERAGE_FILE) --function-coverage --demangle-cpp --output-directory $(COVERAGE_DIR)

compile: all
