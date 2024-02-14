LIBNAME=irio_v2_cpp

TARGET=../../../../target

LIBRARIES=bfp
LIBRARY_DIRS=$(TARGET)/lib
INCLUDE_DIRS=. ./include ../include $(TARGET)/main/c++/bfp/include


ifdef CODAC_ROOT
	LIBRARIES+=NiFpga
	INCLUDE_DIRS+=$(CODAC_ROOT)/include
	LIBRARY_DIRS+=$(CODAC_ROOT)/lib
else
	INCLUDE_DIRS+=$(TARGET)/main/c++/irio_v2_cpp/NiFpga_CD
endif

LIBRARY_DIR=$(TARGET)/lib
SOURCE_DIR=.
OBJECT_DIR = $(SOURCE_DIR)/.obj

SHAREDLIBRARY=$(LIBRARY_DIR)/lib$(LIBNAME).so
STATICLIBRARY=$(LIBRARY_DIR)/lib$(LIBNAME).a
INCLUDES=$(foreach inc,$(INCLUDE_DIRS),-I$(inc))
LDLIBS=-L$(CODAC_ROOT)/lib $(foreach libs,$(LIBRARY_DIRS),-L$(libs) -Wl,-rpath,$(libs)) $(foreach libs,$(LIBRARIES),-l$(libs))
SOURCES=$(SOURCE_DIR)/$(wildcard *.cpp)
OBJECTS=$(addprefix $(OBJECT_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SOURCES))))
HFILES=$(wildcard *.h)
HTARGETS=$(addprefix $(TARGET)/include/, $(HFILES))

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

.PHONY: all clean run 

all: $(HTARGETS) $(SOURCES) $(SHAREDLIBRARY) $(STATICLIBRARY)

$(TARGET)/include/%.h: %.h
	@mkdir -p $(TARGET)/include
	cp $< $@

clean:
	rm -rf "$(SHAREDLIBRARY)" "$(STATICLIBRARY)" "$(OBJECT_DIR)" "$(HTARGETS)"

run: $(SOURCES) $(SHAREDLIBRARY)
	$(SHAREDLIBRARY)

$(SHAREDLIBRARY): $(OBJECTS)
	mkdir -p $(LIBRARY_DIR)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJECTS) -o $(SHAREDLIBRARY) 

$(STATICLIBRARY): $(OBJECTS)
	mkdir -p $(LIBRARY_DIR)
	$(AR) rcs $@ $^
	
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(OBJECT_DIR)
	$(CC) $(CCFLAGS) $(INCLUDES) $< -o $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	mkdir -p $(OBJECT_DIR)
	$(C) $(CFLAGS) $(INCLUDES) $< -o $@
