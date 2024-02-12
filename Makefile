BOLD=\e[1m
NC=\e[0m

SOURCE_DIR := src/
COPY_DIR := target/
LIB_MAKEFILE_DIR := target/main/c++
TEST_MAKEFILE_DIR := target/test/c++

.PHONY: all debug clean compile coverage

all: copy_and_make

copy_and_make:
	@echo "Copying $(SOURCE_DIR) to $(COPY_DIR)..."
	rsync -a --inplace $(SOURCE_DIR) $(COPY_DIR)
	@echo "Copying complete."
	@echo -e "\n$(BOLD)Building libs...$(NC)"
	@echo "Entering $(LIB_MAKEFILE_DIR) and executing make..."
	$(MAKE) -C $(LIB_MAKEFILE_DIR) $(DEBUG)
	@echo "Make in $(LIB_MAKEFILE_DIR) complete."
	
	@echo -e "\n$(BOLD)Building tests...$(NC)"
	@echo "Entering $(TEST_MAKEFILE_DIR) and executing make..."
	$(MAKE) -C $(TEST_MAKEFILE_DIR) $(DEBUG)
	@echo "Make in $(TEST_MAKEFILE_DIR) complete."
	
	@echo -e "$(BOLD)COMPILATION SUCCESSFUL!$(NC)"

clean:
	@echo -e "$(BOLD)Cleaning \"$(COPY_DIR)\"...$(NC)"
	rm -rf $(COPY_DIR)
	@echo "\"$(COPY_DIR)\" cleaned."

debug:
	@echo -e "$(BOLD)Compiling with symbols...$(NC)"
	$(MAKE) all DEBUG="COVERAGE=true"

#Targets to imitate mvn commands
coverage: debug
compile: all
