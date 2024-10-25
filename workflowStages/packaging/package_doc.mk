BOLD=\e[1m
NC=\e[0m

PACKAGE_NAME := irioCoreCpp_doc

HTML_DOC_FOLDER := doc/irioCoreCpp/html


INPUT_FILES := $(shell find $(TOP_DIR)/$(COPY_DIR)/$(HTML_DOC_FOLDER) -type f)
INPUT_FILES := $(subst $(TOP_DIR)/$(COPY_DIR)/,,$(INPUT_FILES))

FILES := $(foreach file,$(INPUT_FILES),$(file):$(subst $(HTML_DOC_FOLDER)/,$(BASE_DOC_INSTALL_DIR)/irioCoreCpp/,$(file)))

SPEC_FILE=$(realpath ./rpmspecs/$(PACKAGE_NAME).spec)

.PHONY: package

all: package

package:
	$(MAKE) -f $(PACKAGE_MK) FILES="$(FILES)" SPEC_FILE=$(SPEC_FILE)\
		PACKAGE_NAME=$(PACKAGE_NAME) PACKAGE_VERSION=$(VERSION)\
		BASE_DIR=$(TOP_DIR)/$(COPY_DIR) OUTPUT_DIR=$(TOP_DIR)/$(COPY_DIR)/packages
