ifndef FILES
$(error FILES variable not defined)
endif

ifndef SPEC_FILE
$(error SPEC_FILE variable not defined)
endif

ifndef PACKAGE_NAME
$(error PACKAGE_NAME variable not defined)
endif

ifndef PACKAGE_VERSION
$(error PACKAGE_VERSION variable not defined)
endif

ifdef OUTPUT_DIR
	COPY_OUTPUT_RPM:=$(OUTPUT_DIR)
endif

ifndef BASE_DIR
	BASE_DIR := .
endif

ARCH := $(shell uname -m)

INPUT_FILES := $(foreach pair,$(FILES),$(BASE_DIR)/$(word 1,$(subst :, ,$(pair))))
OUTPUT_FILES := $(foreach pair,$(FILES),$(word 2,$(subst :, ,$(pair))))

FILES_SPEC_FINAL := $(shell echo "$(OUTPUT_FILES)" | sed 's/\//\\\//g' | sed 's/ /\{NEWLINE\}/g')


ifneq ($(words $(INPUT_FILES)),$(words $(OUTPUT_FILES)))
$(error Not all inputs in FILES have an output)
endif

# Define RPM build structure
RPM_BUILD_DIR := $(BASE_DIR)/rpmbuild/$(PACKAGE_NAME)
RPM_OUTPUT_DIR := $(RPM_BUILD_DIR)/RPMS
RPM_BUILD_ROOT := $(RPM_BUILD_DIR)/BUILDROOT
RPM_SPECS_DIR := $(RPM_BUILD_DIR)/SPECS

RPM_SPEC_FILE_NAME := $(notdir $(SPEC_FILE))
RPM_SPEC_FILE_ORIG := $(SPEC_FILE)
RPM_SPEC_FILE_DEST := $(RPM_SPECS_DIR)/$(RPM_SPEC_FILE_NAME)

# Define the RPM build command
RPM_BUILD_CMD := rpmbuild
RPM_BUILD_OPTIONS := -bb

define copy_files_to_buildroot
	mkdir -p $(RPM_BUILD_ROOT)/$(dir $(2))
	cp -r $(1) $(RPM_BUILD_ROOT)/$(2);
endef

.NOTPARALLEL: clean
.PHONY: package

all: package

clean:
	@rm -rf $(RPM_BUILD_DIR)

gen_rpmbuild:
	@mkdir -p $(RPM_OUTPUT_DIR)
	@mkdir -p $(RPM_BUILD_ROOT)
	@mkdir -p $(RPM_SPECS_DIR)

package: | clean gen_rpmbuild
	$(foreach i,$(shell seq 1 $(words $(FILES))), $(call copy_files_to_buildroot,$(word $(i),$(INPUT_FILES)),$(word $(i),$(OUTPUT_FILES))))
	cp $(RPM_SPEC_FILE_ORIG) $(RPM_SPEC_FILE_DEST)
	sed -i 's/{PACKAGE_NAME}/$(PACKAGE_NAME)/g' $(RPM_SPEC_FILE_DEST)
	sed -i 's/{VERSION}/$(PACKAGE_VERSION)/g' $(RPM_SPEC_FILE_DEST)
	sed -i 's/{FILES_TO_INCLUDE}/$(FILES_SPEC_FINAL)/g' $(RPM_SPEC_FILE_DEST)
	sed -i 's/{NEWLINE}/\n/g' $(RPM_SPEC_FILE_DEST)
	$(RPM_BUILD_CMD) --define "_rpmdir $(PWD)/$(RPM_OUTPUT_DIR)" --buildroot $(PWD)/$(RPM_BUILD_ROOT) $(RPM_BUILD_OPTIONS) $(RPM_SPEC_FILE_DEST)
	@if [ -z "$$COPY_OUTPUT_RPM" ]; then\
        mkdir -p $(COPY_OUTPUT_RPM); \
		cp $(RPM_OUTPUT_DIR)/$(ARCH)/*.rpm $(COPY_OUTPUT_RPM);\
    fi
	
