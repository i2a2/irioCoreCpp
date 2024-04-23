LIB_MAKEFILE_DIR := $(COPY_DIR)/main/c++
TEST_MAKEFILE_DIR := $(COPY_DIR)/test/c++
LEGACY_EXAMPLES_DIR := $(COPY_DIR)/test/c

all: compile

compile:
	@printf "\n$(BOLD)Building libs...$(NC)\n"
	@echo "Entering $(LIB_MAKEFILE_DIR) and executing make..."
	$(MAKE) -C $(LIB_MAKEFILE_DIR) $(DEBUG)
	@echo "Make in $(LIB_MAKEFILE_DIR) complete."
	
	@printf "\n$(BOLD)Building tests...$(NC)\n"
	@echo "Entering $(TEST_MAKEFILE_DIR) and executing make..."
	$(MAKE) -C $(TEST_MAKEFILE_DIR) $(DEBUG)
	@echo "Make in $(TEST_MAKEFILE_DIR) complete."

	@printf "\n$(BOLD)Building legacy examples...$(NC)\n"
	@echo "Entering $(LEGACY_EXAMPLES_DIR) and executing make..."
	$(MAKE) -C $(LEGACY_EXAMPLES_DIR) $(DEBUG)
	@echo "Make in $(LEGACY_EXAMPLES_DIR) complete."

	@printf "$(BOLD)COMPILATION SUCCESS!$(NC)\n"
