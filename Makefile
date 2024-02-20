BOLD=\e[1m
NC=\e[0m

VERSION=0.5

SOURCE_DIR := src/
COPY_DIR := target/
LIB_MAKEFILE_DIR := target/main/c++
TEST_MAKEFILE_DIR := target/test/c++

LIB_INSTALL_DIR := /usr/local/lib
INC_INSTALL_DIR := /usr/local/include

ifeq ($(PREFIX),)
    PREFIX := /usr/local
endif


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

package: all
	mkdir -p $(COPY_DIR)/rpmbuild/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}
	mkdir -p $(COPY_DIR)/rpmbuild/BUILDROOT/iriov2cpp/$(LIB_INSTALL_DIR)
	cp $(COPY_DIR)/lib/*.so $(COPY_DIR)/rpmbuild/BUILDROOT/iriov2cpp/$(LIB_INSTALL_DIR)
	cp rpmspecs/iriov2cpp.spec $(COPY_DIR)/rpmbuild/SPECS/
	sed -i 's/{VERSION}/$(VERSION)/g' $(COPY_DIR)/rpmbuild/SPECS/iriov2cpp.spec
	sed -i 's/{LIB_INSTALL_DIR}/$(shell echo "$(LIB_INSTALL_DIR)" | sed 's/\//\\\//g')/g' $(COPY_DIR)/rpmbuild/SPECS/iriov2cpp.spec
	rpmbuild --define "_rpmdir $(PWD)/$(COPY_DIR)" --buildroot $(PWD)/$(COPY_DIR)/rpmbuild/BUILDROOT/iriov2cpp -bb $(COPY_DIR)/rpmbuild/SPECS/iriov2cpp.spec 
	
	mkdir -p $(COPY_DIR)/rpmbuild/BUILDROOT/iriov2cpp_devel/$(INC_INSTALL_DIR)/iriov2cpp
	mkdir -p $(COPY_DIR)/rpmbuild/BUILDROOT/iriov2cpp_devel/$(LIB_INSTALL_DIR)
	cp -a $(COPY_DIR)/main/c++/bfp/include/. $(COPY_DIR)/rpmbuild/BUILDROOT/iriov2cpp_devel/$(INC_INSTALL_DIR)/iriov2cpp
	cp -a $(COPY_DIR)/main/c++/irio_v2_cpp/include/. $(COPY_DIR)/rpmbuild/BUILDROOT/iriov2cpp_devel/$(INC_INSTALL_DIR)/iriov2cpp
	cp $(COPY_DIR)/lib/*.a $(COPY_DIR)/rpmbuild/BUILDROOT/iriov2cpp_devel/$(LIB_INSTALL_DIR)
	cp rpmspecs/iriov2cpp_devel.spec $(COPY_DIR)/rpmbuild/SPECS/
	sed -i 's/{VERSION}/$(VERSION)/g' $(COPY_DIR)/rpmbuild/SPECS/iriov2cpp_devel.spec
	sed -i 's/{LIB_INSTALL_DIR}/$(shell echo "$(LIB_INSTALL_DIR)" | sed 's/\//\\\//g')/g' $(COPY_DIR)/rpmbuild/SPECS/iriov2cpp_devel.spec
	sed -i 's/{INC_INSTALL_DIR}/$(shell echo "$(INC_INSTALL_DIR)" | sed 's/\//\\\//g')/g' $(COPY_DIR)/rpmbuild/SPECS/iriov2cpp_devel.spec
	rpmbuild --define "_rpmdir $(PWD)/$(COPY_DIR)" --buildroot $(PWD)/$(COPY_DIR)/rpmbuild/BUILDROOT/iriov2cpp_devel -bb $(COPY_DIR)/rpmbuild/SPECS/iriov2cpp_devel.spec 
	
#Targets to imitate mvn commands
coverage: debug
compile: all
