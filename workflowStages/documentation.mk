all: doc

doc:
	@echo -e "$(BOLD)Generating documentation...$(NC)"
	@mkdir -p $(COPY_DIR)/doc/irioCore
	@sed -i 's/{VERSION_DOXYGEN}/$(VERSION)/g' $(COPY_DIR)/main/c++/doc/Doxyfile
	$(MAKE) -C $(COPY_DIR)/main/c++/doc all	