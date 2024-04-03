LIBNAME=irioCoreCpp

TARGET=../../../../target

LIBRARIES=bfp niflexrio

LIBRARY_DIRS=$(TARGET)/lib
INCLUDE_DIRS=./include $(TARGET)/includes/bfp

ifndef CODAC_ROOT
	LIBRARIES+= nisyscfg
	LIBRARY_DIRS+= /usr/lib/x86_64-linux-gnu
endif

LIBRARY_DIR=$(TARGET)/lib
SOURCE_BASE_DIR=.
SOURCES_DIR=$(SOURCE_BASE_DIR) $(SOURCE_BASE_DIR)/profiles $(SOURCE_BASE_DIR)/terminals $(SOURCE_BASE_DIR)/terminals/impl
OBJECT_DIR = $(SOURCE_BASE_DIR)/.obj

SHAREDLIBRARY=$(LIBRARY_DIR)/lib$(LIBNAME).so
STATICLIBRARY=$(LIBRARY_DIR)/lib$(LIBNAME).a
INCLUDES=$(foreach inc,$(INCLUDE_DIRS),-I$(inc))
LDPATHS=-L$(CODAC_ROOT)/lib $(foreach libs,$(LIBRARY_DIRS),-L$(libs) -Wl,-rpath,$(libs)) 
LDLIBS=$(foreach libs,$(LIBRARIES),-l$(libs))
SOURCES=$(foreach dir,$(SOURCES_DIR),$(wildcard $(dir)/*.cpp)) $(foreach dir,$(SOURCES_DIR),$(wildcard $(dir)/*.c))
OBJECTS=$(addprefix $(OBJECT_DIR)/,$(patsubst %.c, %.o,$(patsubst %.cpp,%.o,$(notdir $(SOURCES)))))
HEADERSFILES = ./include

C=gcc
CC=g++
CFLAGS=-c -Wall -Wextra -Wpedantic -Wshadow -fPIC
CCFLAGS=-c -Wall -Wextra -Wpedantic -Wshadow -fPIC -std=c++11
LDFLAGS= -shared

ifeq ($(COVERAGE),true)
	CFLAGS+= -O0 -g --coverage
	CCFLAGS+= -O0 -g --coverage
	LDFLAGS+= --coverage
else
	CFLAGS+= -O3
	CCFLAGS+= -O3
endif

ifdef CODAC_ROOT
	LIBRARIES+=NiFpga
	INCLUDE_DIRS+=$(CODAC_ROOT)/include
	LIBRARY_DIRS+=$(CODAC_ROOT)/lib
	CFLAGS+= -DCCS_VERSION
	CCFLAGS+= -DCCS_VERSION
else
	INCLUDE_DIRS+=$(TARGET)/main/c++/NiFpga_CD
	SOURCES_DIR+=$(TARGET)/main/c++/NiFpga_CD
	HEADERSFILES+=$(wildcard $(TARGET)/main/c++/NiFpga_CD/*.h)
endif

VPATH=$(SOURCES_DIR)


.PHONY: all clean run 

all: copy_includes $(SOURCES) $(SHAREDLIBRARY) $(STATICLIBRARY)

copy_includes:
	mkdir -p $(TARGET)/includes/
	@for header in $(HEADERSFILES); do\
		cp -R $$header $(TARGET)/includes/$(LIBNAME)/;\
	done

clean:
	rm -rf "$(SHAREDLIBRARY)" "$(STATICLIBRARY)" "$(OBJECT_DIR)" "$(HTARGETS)"

run: $(SOURCES) $(SHAREDLIBRARY)
	$(SHAREDLIBRARY)

$(SHAREDLIBRARY): $(OBJECTS)
	mkdir -p $(LIBRARY_DIR)
	$(CC) $(LDFLAGS) $(LDPATHS) $(OBJECTS) -o $(SHAREDLIBRARY) $(LDLIBS)

$(STATICLIBRARY): $(OBJECTS)
	mkdir -p $(LIBRARY_DIR)
	$(AR) rcs $@ $^
	
$(OBJECT_DIR)/%.o: %.cpp
	mkdir -p $(OBJECT_DIR)
	$(CC) $(CCFLAGS) $(INCLUDES) $< -o $@

$(OBJECT_DIR)/%.o: %.c
	mkdir -p $(OBJECT_DIR)
	$(C) $(CFLAGS) $(INCLUDES) $< -o $@
