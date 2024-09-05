BOLD=\e[1m
NC=\e[0m

TARGET=../../../../target

LIBRARIES=pthread irioCore
LIBRARY_DIRS=$(TARGET)/lib
INCLUDE_DIRS=. $(TARGET)/includes/irioCore

ifdef IRIOCORECPP_CCS_VERSION
	LIBRARIES+=NiFpga 
	INCLUDE_DIRS+=$(CODAC_ROOT)/include
	LIBRARY_DIRS+=$(CODAC_ROOT)/lib
else
	LIBRARY_DIRS+= /usr/lib/x86_64-linux-gnu
	INCLUDE_DIRS+=$(TARGET)/main/c++/NiFpga_CD
endif

BINARY_DIR=.
SOURCE_DIR=.

INCLUDES=$(foreach inc,$(INCLUDE_DIRS),-I$(inc))
LDPATHS=$(foreach libs,$(LIBRARY_DIRS),-L$(libs) -Wl,--enable-new-dtags,-rpath,$(libs)) 
LDLIBS=$(foreach libs,$(LIBRARIES),-l$(libs))
SOURCES=$(wildcard $(SOURCE_DIR)/*.c)

C=gcc
CC=g++
CFLAGS=-Wall -O0 -g -D_GNU_SOURCE
CCFLAGS=-Wall -std=c++11 -O0 -g -D_GNU_SOURCE
LDFLAGS=

EXECUTABLES=$(basename $(SOURCES))

.PHONY: all clean run

all: $(EXECUTABLES)

clean:
	rm -rf "$(EXECUTABLES)"

%: %.c
	$(C) $(CFLAGS) $(LDFLAGS) $(LDPATHS) $(INCLUDES) $< -o $@ $(LDLIBS)