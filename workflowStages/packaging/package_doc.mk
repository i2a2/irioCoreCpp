BOLD=\e[1m
NC=\e[0m

PACKAGE_NAME := irioCore_doc

HTML_DOC_FOLDER := $(TOP_DIR)/$(COPY_DIR)/doc/irioCore/html/*

DOC_INSTALL_DIR := $(BASE_DOC_INSTALL_DIR)

FILES_SPEC = $(DOC_INSTALL_DIR)/irioCore/*
FILES_SPEC_FINAL := $(shell echo "$(FILES_SPEC)" | sed 's/\//\\\//g' | sed 's/ /\{NEWLINE\}/g')

RPM_BUILD_DIR := $(TOP_DIR)/$(COPY_DIR)/rpmbuild/$(PACKAGE_NAME)

RPM_OUTPUT_DIR := $(RPM_BUILD_DIR)/RPMS
RPM_SOURCE_DIR := $(RPM_BUILD_DIR)/SOURCES
RPM_BUILD_ROOT := $(RPM_BUILD_DIR)/BUILDROOT
RPM_SPECS_DIR := $(RPM_BUILD_DIR)/SPECS

RPM_SPEC_FILE := $(PACKAGE_NAME).spec
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
	cp -r $(HTML_DOC_FOLDER) $(RPM_BUILD_ROOT)/$(DOC_INSTALL_DIR)/irioCore
	cp $(RPM_SPEC_FILE_ORIG) $(RPM_SPEC_FILE_DEST)
	sed -i 's/{VERSION}/$(VERSION)/g' $(RPM_SPEC_FILE_DEST)
	sed -i 's/{FILES_TO_INCLUDE}/$(FILES_SPEC_FINAL)/g' $(RPM_SPEC_FILE_DEST)
	sed -i 's/{NEWLINE}/\n/g' $(RPM_SPEC_FILE_DEST)
	$(RPM_BUILD_CMD) --define "_rpmdir $(PWD)/$(TOP_DIR)/$(COPY_DIR)/packages" --buildroot $(PWD)/$(RPM_BUILD_ROOT) $(RPM_BUILD_OPTIONS) $(RPM_SPEC_FILE_DEST)

gen_rpmbuild:
	echo -e "$(BOLD)Generating $(PACKAGE_NAME) package...$(NC)"
	@mkdir -p $(RPM_BUILD_DIR)/BUILD
	@mkdir -p $(RPM_BUILD_DIR)/BUILDROOT
	@mkdir -p $(RPM_BUILD_DIR)/RPMS
	@mkdir -p $(RPM_BUILD_DIR)/SOURCES
	@mkdir -p $(RPM_BUILD_DIR)/SPECS
	@mkdir -p $(RPM_BUILD_DIR)/SRPMS
	@mkdir -p $(RPM_BUILD_ROOT)/$(DOC_INSTALL_DIR)/irioCore

.PHONY: clean build