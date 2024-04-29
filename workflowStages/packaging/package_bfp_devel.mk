PACKAGE_NAME := bfp_devel
INCLUDE_FOLDER := bfp

LIBRARIES := lib/libbfp.a
INCLUDES := $(patsubst $(TOP_DIR)/$(COPY_DIR)/%,%,$(wildcard $(TOP_DIR)/$(COPY_DIR)/includes/bfp/*.h))

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
