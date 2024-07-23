PACKAGE_NAME := irioCoreCpp_devel
INCLUDE_FOLDER := irioCoreCpp

LIBRARIES := lib/libirioCoreCpp.a

LIB_INSTALL_DIR := $(BASE_LIB_INSTALL_DIR)
INC_INSTALL_DIR := $(BASE_INC_INSTALL_DIR)/$(INCLUDE_FOLDER)

INCLUDES_BASE_DIR := $(TOP_DIR)/$(COPY_DIR)/main/c++/irioCoreCpp/include
INCLUDES_ALL = $(wildcard $(INCLUDES_BASE_DIR)/*.h)
EXCLUDE = utils.h
EXCLUDE += rioDiscovery.h
EXCLUDE += parserManager.h
EXCLUDE += irioCoreCppVersion.h
INCLUDES =  $(patsubst $(TOP_DIR)/$(COPY_DIR)/%,%,$(filter-out $(addprefix $(INCLUDES_BASE_DIR)/,$(EXCLUDE)), $(INCLUDES_ALL)))
INCLUDES_TERMINALS = $(patsubst $(TOP_DIR)/$(COPY_DIR)/%,%,$(wildcard $(INCLUDES_BASE_DIR)/terminals/*.h))

FILES=$(foreach file,$(LIBRARIES),$(file):$(LIB_INSTALL_DIR)/$(notdir $(file)))
FILES=$(foreach file,$(INCLUDES),$(file):$(INC_INSTALL_DIR)/$(notdir $(file)))
FILES+=$(foreach file,$(INCLUDES_TERMINALS),$(file):$(INC_INSTALL_DIR)/terminals/$(notdir $(file)))

SPEC_FILE=$(realpath ./rpmspecs/$(PACKAGE_NAME).spec)

.PHONY: package

all: package

package:
	$(MAKE) -f $(PACKAGE_MK) FILES="$(FILES)" SPEC_FILE=$(SPEC_FILE)\
		PACKAGE_NAME=$(PACKAGE_NAME) PACKAGE_VERSION=$(VERSION)\
		BASE_DIR=$(TOP_DIR)/$(COPY_DIR) OUTPUT_DIR=$(TOP_DIR)/$(COPY_DIR)/packages
