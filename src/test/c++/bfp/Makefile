PROGNAME=test_bfp

TARGET=../../../../target

LIBRARIES=gtest bfp pugixml
LIBRARY_DIRS=$(TARGET)/lib
INCLUDE_DIRS=$(TARGET)/main/c++/bfp/include

BINARY_DIR=.
SOURCE_DIR=.
OBJECT_DIR = $(SOURCE_DIR)/.obj

EXECUTABLE=$(BINARY_DIR)/$(PROGNAME)
BASE_INCLUDES=. ../include $(TARGET)/main/c++/include
INCLUDES=$(foreach inc,$(BASE_INCLUDES),-I$(inc)) $(foreach inc,$(INCLUDE_DIRS),-I$(inc))
LDLIBS=$(foreach libs,$(LIBRARY_DIRS),-L$(libs) -Wl,-rpath,$(libs)) $(foreach libs,$(LIBRARIES),-l$(libs))
SOURCES=$(wildcard $(SOURCE_DIR)/*.cpp)
OBJECTS=$(addprefix $(OBJECT_DIR)/,$(patsubst %.cpp,%.o,$(notdir $(SOURCES))))

C=gcc
CC=g++
CFLAGS=-c -Wall -O0 -g
CCFLAGS=-c -Wall -std=c++11 -O0 -g
LDFLAGS=


.PHONY: all clean run

all: $(SOURCES) $(EXECUTABLE)

clean:
	rm -rf "$(EXECUTABLE)" "$(OBJECT_DIR)"

run: $(SOURCES) $(EXECUTABLE)
	$(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	mkdir -p $(BINARY_DIR)
	$(CC) $(LDFLAGS) $(LDLIBS) $(OBJECTS) -o $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	mkdir -p $(OBJECT_DIR)
	$(CC) $(CCFLAGS) $(INCLUDES) $< -o $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	mkdir -p $(OBJECT_DIR)
	$(C) $(CFLAGS) $(INCLUDES) $< -o $@