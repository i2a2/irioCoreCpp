PACKAGE_NAME := irioCoreCpp

LIBRARIES := lib/lib$(PACKAGE_NAME).so
INCLUDES :=

LIB_INSTALL_DIR := $(BASE_LIB_INSTALL_DIR)
INC_INSTALL_DIR := $(BASE_INC_INSTALL_DIR)/$(PACKAGE_NAME)

FILES=$(foreach file,$(LIBRARIES),$(file):$(LIB_INSTALL_DIR)/$(notdir $(file)))
FILES+=$(foreach file,$(INCLUDES),$(file):$(INC_INSTALL_DIR)/$(notdir $(file)))

SPEC_FILE=$(realpath ./rpmspecs/$(PACKAGE_NAME).spec)

.PHONY: package

all: package

package:
	$(MAKE) -f $(PACKAGE_MK) FILES=$(FILES) SPEC_FILE=$(SPEC_FILE)\
		PACKAGE_NAME=$(PACKAGE_NAME) PACKAGE_VERSION=$(VERSION)\
		BASE_DIR=$(TOP_DIR)/$(COPY_DIR) OUTPUT_DIR=$(TOP_DIR)/$(COPY_DIR)/packages
