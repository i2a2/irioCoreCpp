all: doc

doc:
	@printf "$(BOLD)Generating documentation...$(NC)\n"
	@mkdir -p $(COPY_DIR)/doc/irioCore
	@sed -i 's/{VERSION_DOXYGEN}/$(VERSION)/g' $(COPY_DIR)/main/c++/doc/Doxyfile
	$(MAKE) -C $(COPY_DIR)/main/c++/doc all	