MODULE_NAME := irioCore
LIBRARIES := $(wildcard $(TOP_DIR)/$(COPY_DIR)/lib/lib$(MODULE_NAME).*)
INCLUDES += $(wildcard $(TOP_DIR)/$(COPY_DIR)/main/c++/irioCore/include/*.h)

LIB_INSTALL_DIR := $(INSTALL_DIR)/lib
INC_INSTALL_DIR := $(INSTALL_DIR)/include/$(MODULE_NAME)

all: install

install:
	@mkdir -p $(LIB_INSTALL_DIR)
	@mkdir -p $(INC_INSTALL_DIR)
	cp $(LIBRARIES) $(LIB_INSTALL_DIR)
	cp $(INCLUDES) $(INC_INSTALL_DIR)