BOLD=\e[1m
NC=\e[0m

PACKAGE_NAME := irioCoreCpp

LIBRARIES := $(TOP_DIR)/$(COPY_DIR)/lib/lib$(PACKAGE_NAME).a
INCLUDES_BASE_DIR := $(TOP_DIR)/$(COPY_DIR)/main/c++/irioCoreCpp/include
INCLUDES_ALL = $(wildcard $(INCLUDES_BASE_DIR)/*.h)
EXCLUDE = utils.h
EXCLUDE += rioDiscovery.h
EXCLUDE += parserManager.h
INCLUDES =  $(filter-out $(EXCLUDE), $(INCLUDES_ALL))

INCLUDES_TERMINALS = $(wildcard $(INCLUDES_BASE_DIR)/terminals/*.h)

LIB_INSTALL_DIR := $(BASE_LIB_INSTALL_DIR)
INC_INSTALL_DIR := $(BASE_INC_INSTALL_DIR)/$(PACKAGE_NAME)

FILES_SPEC = $(foreach file,$(notdir $(LIBRARIES)),"$(LIB_INSTALL_DIR)/$(file)"\n)
FILES_SPEC += $(foreach file,$(notdir $(INCLUDES)),"$(INC_INSTALL_DIR)/$(file)"\n)
FILES_SPEC += $(foreach file,$(notdir $(INCLUDES_TERMINALS)),"$(INC_INSTALL_DIR)/terminals/$(file)"\n)

RPM_BUILD_DIR := $(TOP_DIR)/$(COPY_DIR)/rpmbuild/$(PACKAGE_NAME)_devel

RPM_OUTPUT_DIR := $(RPM_BUILD_DIR)/RPMS
RPM_SOURCE_DIR := $(RPM_BUILD_DIR)/SOURCES
RPM_BUILD_ROOT := $(RPM_BUILD_DIR)/BUILDROOT
RPM_SPECS_DIR := $(RPM_BUILD_DIR)/SPECS

RPM_SPEC_FILE := $(PACKAGE_NAME)_devel.spec
RPM_SPEC_FILE_ORIG := rpmspecs/$(RPM_SPEC_FILE)
RPM_SPEC_FILE_DEST := $(RPM_SPECS_DIR)/$(RPM_SPEC_FILE)

# Define the RPM build command
RPM_BUILD_CMD := rpmbuild

# Define the RPM build options
RPM_BUILD_OPTIONS := -bb

all: package

clean:
	@rm -rf $(BUILD_DIR) $(RPM_BUILD_DIR)

package: clean gen_rpmbuild
	@cp $(LIBRARIES) $(RPM_BUILD_ROOT)/$(LIB_INSTALL_DIR)
	@if [ -n "$(INCLUDES)" ]; then cp $(INCLUDES) $(RPM_BUILD_ROOT)/$(INC_INSTALL_DIR); fi
	@if [ -n "$(INCLUDES_TERMINALS)" ]; then cp $(INCLUDES_TERMINALS) $(RPM_BUILD_ROOT)/$(INC_INSTALL_DIR)/terminals; fi
	@cp $(RPM_SPEC_FILE_ORIG) $(RPM_SPEC_FILE_DEST)
	@sed -i 's/{VERSION}/$(VERSION)/g' $(RPM_SPEC_FILE_DEST)
	@sed -i 's/{FILES_TO_INCLUDE}/$(shell echo "$(FILES_SPEC)" | sed 's/\//\\\//g')/g' $(RPM_SPEC_FILE_DEST)
	$(RPM_BUILD_CMD) --define "_rpmdir $(PWD)/$(TOP_DIR)/$(COPY_DIR)/packages" --buildroot $(PWD)/$(RPM_BUILD_ROOT) $(RPM_BUILD_OPTIONS) $(RPM_SPEC_FILE_DEST)

gen_rpmbuild:
	echo -e "$(BOLD)Generating $(PACKAGE_NAME)_devel package...$(NC)"
	@mkdir -p $(RPM_BUILD_DIR)/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}
	@mkdir -p $(RPM_BUILD_ROOT)/$(LIB_INSTALL_DIR)
	@mkdir -p $(RPM_BUILD_ROOT)/$(INC_INSTALL_DIR)
	@mkdir -p $(RPM_BUILD_ROOT)/$(INC_INSTALL_DIR)/terminals


.PHONY: clean build