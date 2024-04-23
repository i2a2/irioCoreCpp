BOLD=\e[1m
NC=\e[0m

PACKAGE_NAME := bfp


LIBRARIES := $(TOP_DIR)/$(COPY_DIR)/lib/lib$(PACKAGE_NAME).so
INCLUDES :=

LIB_INSTALL_DIR := $(BASE_LIB_INSTALL_DIR)
INC_INSTALL_DIR := $(BASE_INC_INSTALL_DIR)/$(PACKAGE_NAME)

FILES_SPEC = $(foreach file,$(notdir $(LIBRARIES)),"$(LIB_INSTALL_DIR)/$(file)")
FILES_SPEC += $(foreach file,$(notdir $(INCLUDES)),"$(INC_INSTALL_DIR)/$(file)")
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
	cp $(LIBRARIES) $(RPM_BUILD_ROOT)/$(LIB_INSTALL_DIR)
	if [ -n "$(INCLUDES)" ]; then cp $(INCLUDES) $(RPM_BUILD_ROOT)/$(INC_INSTALL_DIR); fi
	cp $(RPM_SPEC_FILE_ORIG) $(RPM_SPEC_FILE_DEST)
	sed -i 's/{VERSION}/$(VERSION)/g' $(RPM_SPEC_FILE_DEST)
	sed -i 's/{FILES_TO_INCLUDE}/$(FILES_SPEC_FINAL)/g' $(RPM_SPEC_FILE_DEST)
	sed -i 's/{NEWLINE}/\n/g' $(RPM_SPEC_FILE_DEST)
	$(RPM_BUILD_CMD) --define "_rpmdir $(PWD)/$(TOP_DIR)/$(COPY_DIR)/packages" --buildroot $(PWD)/$(RPM_BUILD_ROOT) $(RPM_BUILD_OPTIONS) $(RPM_SPEC_FILE_DEST)

gen_rpmbuild:
	printf "$(BOLD)Generating $(PACKAGE_NAME) package...$(NC)\n"
	@mkdir -p $(RPM_BUILD_DIR)/BUILD
	@mkdir -p $(RPM_BUILD_DIR)/BUILDROOT
	@mkdir -p $(RPM_BUILD_DIR)/RPMS
	@mkdir -p $(RPM_BUILD_DIR)/SOURCES
	@mkdir -p $(RPM_BUILD_DIR)/SPECS
	@mkdir -p $(RPM_BUILD_DIR)/SRPMS
	@mkdir -p $(RPM_BUILD_ROOT)/$(LIB_INSTALL_DIR)
	@mkdir -p $(RPM_BUILD_ROOT)/$(INC_INSTALL_DIR)


.PHONY: clean build