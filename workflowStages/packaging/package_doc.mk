BOLD=\e[1m
NC=\e[0m

PACKAGE_NAME := irioCoreCpp_doc

HTML_DOC_FOLDER := doc/irioCoreCpp/html


FILES=$(HTML_DOC_FOLDER):$(BASE_DOC_INSTALL_DIR)/irioCoreCpp/

SPEC_FILE=$(realpath ./rpmspecs/$(PACKAGE_NAME).spec)

.PHONY: package

all: package

package:
	$(MAKE) -f $(PACKAGE_MK) FILES=$(FILES) SPEC_FILE=$(SPEC_FILE)\
		PACKAGE_NAME=$(PACKAGE_NAME) PACKAGE_VERSION=$(VERSION)\
		BASE_DIR=$(TOP_DIR)/$(COPY_DIR) OUTPUT_DIR=$(TOP_DIR)/$(COPY_DIR)/packages
