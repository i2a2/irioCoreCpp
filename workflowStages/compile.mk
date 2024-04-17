LIB_MAKEFILE_DIR := $(COPY_DIR)/main/c++
TEST_MAKEFILE_DIR := $(COPY_DIR)/test/c++
LEGACY_EXAMPLES_DIR := $(COPY_DIR)/test/c

all: compile

compile:
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

	@echo -e "$(BOLD)COMPILATION SUCCESS!$(NC)"
