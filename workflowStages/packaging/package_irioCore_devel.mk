PACKAGE_NAME := irioCore_devel
INCLUDE_FOLDER := irioCore

LIBRARIES := lib/libirioCore.a
INCLUDES := $(patsubst $(TOP_DIR)/$(COPY_DIR)/%,%,$(wildcard $(TOP_DIR)/$(COPY_DIR)/includes/$(INCLUDE_FOLDER)/*.h))

LIB_INSTALL_DIR := $(BASE_LIB_INSTALL_DIR)
INC_INSTALL_DIR := $(BASE_INC_INSTALL_DIR)/$(INCLUDE_FOLDER)

FILES=$(foreach file,$(LIBRARIES),$(file):$(LIB_INSTALL_DIR)/$(notdir $(file)))
FILES+=$(foreach file,$(INCLUDES),$(file):$(INC_INSTALL_DIR)/$(notdir $(file)))

SPEC_FILE=$(realpath ./rpmspecs/$(PACKAGE_NAME).spec)

.PHONY: package

all: package

package:
	$(MAKE) -f $(PACKAGE_MK) FILES="$(FILES)" SPEC_FILE=$(SPEC_FILE)\
		PACKAGE_NAME=$(PACKAGE_NAME) PACKAGE_VERSION=$(VERSION)\
		BASE_DIR=$(TOP_DIR)/$(COPY_DIR) OUTPUT_DIR=$(TOP_DIR)/$(COPY_DIR)/packages
