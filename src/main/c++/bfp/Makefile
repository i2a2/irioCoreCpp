LIBNAME=bfp

TARGET=../../../../target

LIBRARIES=pugixml
LIBRARY_DIRS=
INCLUDE_DIRS=./include ../irioCoreCpp/include

LIBRARY_DIR=$(TARGET)/lib
SOURCE_DIR=.
OBJECT_DIR = $(SOURCE_DIR)/.obj

SHAREDLIBRARY=$(LIBRARY_DIR)/lib$(LIBNAME).so
STATICLIBRARY=$(LIBRARY_DIR)/lib$(LIBNAME).a
INCLUDES=$(foreach inc,$(INCLUDE_DIRS),-I$(inc))
LDPATHS=-L$(CODAC_ROOT)/lib $(foreach libs,$(LIBRARY_DIRS),-L$(libs) -Wl,-rpath,$(libs)) 
LDLIBS=$(foreach libs,$(LIBRARIES),-l$(libs))
SOURCES=$(SOURCE_DIR)/$(wildcard *.cpp)
OBJECTS=$(addprefix $(OBJECT_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SOURCES))))
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
	
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(OBJECT_DIR)
	$(CC) $(CCFLAGS) $(INCLUDES) $< -o $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	mkdir -p $(OBJECT_DIR)
	$(C) $(CFLAGS) $(INCLUDES) $< -o $@
