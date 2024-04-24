all: doc

doc:
	@printf "$(BOLD)Generating documentation...$(NC)\n"
	@mkdir -p $(COPY_DIR)/doc/irioCoreCpp
	@sed -i 's/{VERSION_DOXYGEN}/$(VERSION)/g' $(COPY_DIR)/main/doc/Doxyfile
	$(MAKE) -C $(COPY_DIR)/main/doc all	