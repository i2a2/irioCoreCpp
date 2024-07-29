PACKAGE_NAME := irioCoreCpp

LIBRARIES := lib/lib$(PACKAGE_NAME).so
INCLUDES :=

ENV_VAR_FILE := main/scripts/irioCore.sh

trslashes = $(if $(filter %/,$(1)),$(call trslashes,$(patsubst %/,%,$(1))),$(1))

LIB_INSTALL_DIR := $(BASE_LIB_INSTALL_DIR)
INC_INSTALL_DIR := $(BASE_INC_INSTALL_DIR)/$(PACKAGE_NAME)

FILES=$(foreach file,$(LIBRARIES),$(file):$(LIB_INSTALL_DIR)/$(notdir $(file)))
FILES+=$(foreach file,$(INCLUDES),$(file):$(INC_INSTALL_DIR)/$(notdir $(file)))
FILES+=$(ENV_VAR_FILE):/etc/profile.d/irioCore.sh

SPEC_FILE=$(realpath ./rpmspecs/$(PACKAGE_NAME).spec)

.PHONY: package

all: package

package: $(ENV_VAR_FILE)
	$(MAKE) -f $(PACKAGE_MK) FILES="$(FILES)" SPEC_FILE=$(SPEC_FILE)\
		PACKAGE_NAME=$(PACKAGE_NAME) PACKAGE_VERSION=$(VERSION)\
		BASE_DIR=$(TOP_DIR)/$(COPY_DIR) OUTPUT_DIR=$(TOP_DIR)/$(COPY_DIR)/packages

$(ENV_VAR_FILE):
	sed -i 's\{INSTALL_MODULES_DIR}\$(BASE_MODULES_INSTALL_DIR)\g' $(TOP_DIR)/$(COPY_DIR)/$@