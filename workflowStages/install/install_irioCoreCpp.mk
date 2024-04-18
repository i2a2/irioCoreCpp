MODULE_NAME := irioCoreCpp
LIBRARIES := $(wildcard $(TOP_DIR)/$(COPY_DIR)/lib/lib$(MODULE_NAME).*)
INCLUDES_BASE_DIR := $(TOP_DIR)/$(COPY_DIR)/main/c++/irioCoreCpp/include
INCLUDES_ALL = $(wildcard $(INCLUDES_BASE_DIR)/*.h)
EXCLUDE = utils.h
EXCLUDE += rioDiscovery.h
EXCLUDE += parserManager.h
INCLUDES =  $(filter-out $(addprefix $(INCLUDES_BASE_DIR)/,$(EXCLUDE)), $(INCLUDES_ALL))

INCLUDES_TERMINALS = $(wildcard $(INCLUDES_BASE_DIR)/terminals/*.h)

LIB_INSTALL_DIR := $(INSTALL_DIR)/lib
INC_INSTALL_DIR := $(INSTALL_DIR)/include/$(MODULE_NAME)

all: install

install:
	@mkdir -p $(LIB_INSTALL_DIR)
	@mkdir -p $(INC_INSTALL_DIR)
	@mkdir -p $(INC_INSTALL_DIR)/terminals
	cp $(LIBRARIES) $(LIB_INSTALL_DIR)
	cp $(INCLUDES) $(INC_INSTALL_DIR)
	cp $(INCLUDES_TERMINALS) $(INC_INSTALL_DIR)/terminals