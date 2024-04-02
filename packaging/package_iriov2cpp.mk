BOLD=\e[1m
NC=\e[0m

PACKAGE_NAME := irio_v2_cpp

LIBRARIES := ../$(COPY_DIR)/lib/lib$(PACKAGE_NAME).so
INCLUDES :=

LIB_INSTALL_DIR := /usr/local/lib
INC_INSTALL_DIR := /usr/local/include/$(PACKAGE_NAME)

FILES_SPEC = $(foreach file,$(notdir $(LIBRARIES)),"$(LIB_INSTALL_DIR)/$(file)"\n)
FILES_SPEC += $(foreach file,$(notdir $(INCLUDES)),"$(INC_INSTALL_DIR)/$(file)"\n)

RPM_BUILD_DIR := ../$(COPY_DIR)/rpmbuild

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
	@cp $(LIBRARIES) $(RPM_BUILD_ROOT)/$(LIB_INSTALL_DIR)
	@cp $(RPM_SPEC_FILE_ORIG) $(RPM_SPEC_FILE_DEST)
	@sed -i 's/{VERSION}/$(VERSION)/g' $(RPM_SPEC_FILE_DEST)
	@sed -i 's/{FILES_TO_INCLUDE}/$(shell echo "$(FILES_SPEC)" | sed 's/\//\\\//g')/g' $(RPM_SPEC_FILE_DEST)
	$(RPM_BUILD_CMD) --define "_rpmdir $(PWD)/../$(COPY_DIR)/packages" --buildroot $(PWD)/$(RPM_BUILD_ROOT) $(RPM_BUILD_OPTIONS) $(RPM_SPEC_FILE_DEST)

gen_rpmbuild:
	echo -e "$(BOLD)Generating $(PACKAGE_NAME) package...$(NC)"
	@mkdir -p $(RPM_BUILD_DIR)/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}
	@mkdir -p $(RPM_BUILD_ROOT)/$(LIB_INSTALL_DIR)
	@mkdir -p $(RPM_BUILD_ROOT)/$(INC_INSTALL_DIR)


.PHONY: clean build