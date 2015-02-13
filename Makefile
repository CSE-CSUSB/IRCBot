##
 # Makefile
 #
 # Dylan Allbee <dylan@linux.com>
 # Created in 2015
 #
 # The author disclaims copyright to this source code.
 # Code is provided with no warranty. Happy Hacking!
 #
 #####################################################
 # Builds executable by automatically acquiring sources from the include
 # directory, compiling them into object files, and linking them to the main 
 # executable.
 # 
 # Uses clang++ for better error messages during development and gnu-c++ for
 # optimized binaries in the production build.
##

CC := g++ -Werror
CC_DEBUG := clang++ -g -stdlib=libc++
CFLAGS := -std=c++11 -O3 -Wall -Wextra -Wformat=2 -Wpedantic -Wshadow \
	-Wpointer-arith -Wcast-qual -Wstrict-overflow=1 -Wformat-nonliteral \
	-Wuninitialized -fstack-protector -Wformat-security

SRC := src
INCLUDE := $(SRC)/include
MAIN := $(SRC)/main.cc
SOURCES := $(shell ls $(INCLUDE)/*.cc 2>/dev/null)
OBJECTS := $(SOURCES:.cc=.o)
TARGET := bin/$(shell basename `pwd`)

all: release
debug: CC := $(CC_DEBUG)
debug: release

release: clean message $(OBJECTS)
	@echo "  cc $(MAIN)"
	@echo "Linking..."
	@$(CC) $(CFLAGS) $(OBJECTS) -I$(INCLUDE) $(MAIN) -o $(TARGET)
	@echo "Done"

$(OBJECTS): $(SOURCES)
	@echo "  cc $(OBJECTS)"
	@$(CC) $(CFLAGS) -I$(INCLUDE) -o $@ -c $<

.PHONY: message clean

message:
	@echo "Building $(shell basename `pwd`)..."

clean:
	@echo Cleaning...
	@rm -f $(OBJECTS) $(TARGET)
