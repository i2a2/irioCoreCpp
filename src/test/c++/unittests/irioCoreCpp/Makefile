PROGNAME=test_ut_irioCoreCpp

TARGET=../../../../../target

LIBRARIES=gtest pthread bfp irioCoreCpp
LIBRARY_DIRS=$(TARGET)/lib
INCLUDE_DIRS=. ./include ../include $(TARGET)/includes/bfp $(TARGET)/includes/irioCoreCpp

BINARY_DIR=.
SOURCE_BASE_DIR=.
SOURCES_DIR=$(SOURCE_BASE_DIR) ../common
OBJECT_DIR = $(SOURCE_BASE_DIR)/.obj

EXECUTABLE=$(BINARY_DIR)/$(PROGNAME)
INCLUDES=$(foreach inc,$(INCLUDE_DIRS),-I$(inc))
LDPATHS=$(foreach libs,$(LIBRARY_DIRS),-L$(libs) -Wl,--enable-new-dtags,-rpath,$(libs))
LDLIBS=$(foreach libs,$(LIBRARIES),-l$(libs))
SOURCES=$(foreach dir,$(SOURCES_DIR),$(wildcard $(dir)/*.cpp)) $(foreach dir,$(SOURCES_DIR),$(wildcard $(dir)/*.c))
OBJECTS=$(addprefix $(OBJECT_DIR)/,$(patsubst %.c, %.o,$(patsubst %.cpp,%.o,$(notdir $(SOURCES)))))

C=gcc
CC=g++
CFLAGS=-c -Wno-variadic-macros -Wno-class-memaccess -O0 -g
CCFLAGS=-c -Wno-variadic-macros -Wno-class-memaccess -std=c++11 -O0 -g
LDFLAGS=

ifdef CODAC_ROOT
	LIBRARIES+=NiFpga 
	INCLUDE_DIRS+=$(CODAC_ROOT)/include
	LIBRARY_DIRS+=$(CODAC_ROOT)/lib
	CFLAGS+= -DCCS_VERSION
	CCFLAGS+= -DCCS_VERSION
else
	LIBRARY_DIRS+= /usr/lib/x86_64-linux-gnu
	INCLUDE_DIRS+=$(TARGET)/main/c++/NiFpga_CD
endif

VPATH=$(SOURCES_DIR)

.PHONY: all clean run

all: $(SOURCES) $(EXECUTABLE)

clean:
	rm -rf "$(EXECUTABLE)" "$(OBJECT_DIR)"

run: $(SOURCES) $(EXECUTABLE)
	$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	mkdir -p $(BINARY_DIR)
	$(CC) $(LDFLAGS) $(LDPATHS) $(OBJECTS) -o $@ $(LDLIBS)

$(OBJECT_DIR)/%.o: %.cpp
	mkdir -p $(OBJECT_DIR)
	$(CC) $(CCFLAGS) $(INCLUDES) $< -o $@

$(OBJECT_DIR)/%.o: %.c
	mkdir -p $(OBJECT_DIR)
	$(C) $(CFLAGS) $(INCLUDES) $< -o $@
