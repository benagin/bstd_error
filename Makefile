# Configuration options ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

debug ?= 0

# Component/target names ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

EXAMPLES ?= examples

# Directory Layout ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

$(shell mkdir -p bin)
BIN_DIR ?= ./bin

EXAMPLES_SRC_DIR  ?= ./examples

$(shell mkdir -p build/dependencies)
BUILD_DIR      ?= ./build
DEPENDENCY_DIR := $(BUILD_DIR)/dependencies
DEPENDENCIES   := $(DEPENDENCY_DIR)/dependencies
D_FILES        := $(DEPENDENCY_DIR)/$*.d

INC  := -Iinclude/

# Compiler Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

CXX 	  = g++
CXXFLAGS  = -std=c++2a -Wall -Werror -pedantic

ifeq ($(debug), 1)
	CXXFLAGS += -g
endif

DEPS = -MMD -MF $(D_FILES)

# File Configuration ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

EXAMPLES_SRCS := $(shell find $(EXAMPLES_SRC_DIR) -path "*.cpp")
EXAMPLES      := $(basename $(EXAMPLES_SRCS))

# Object File Recipes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

.cpp.o: $(SRCS)
	@echo Compiling $<...
	@$(CXX) -c $(CXXFLAGS) $(DEPS) $(INC) $< -o $@
	@cat $(D_FILES) >> $(DEPENDENCIES)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

# Executable Recipes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

all:	$(EXAMPLES)

# Build all examples.
.PHONY: $(EXAMPLES)
$(EXAMPLES):	%: %.cpp
		@echo Compiling $<...
		@$(CXX) $(CXXFLAGS) $(DEPS) $(INC) $< -o $@
		@cat $(D_FILES) >> $(DEPENDENCIES)

# Cleanup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

# TODO: clean examples and test
.PHONY: clean
clean:
	@echo Cleaning...
	@rm -f $(shell find $(DEPENDENCY_DIR) -path "*.d")
	@rm -f $(shell find . -path "*.o")
	@rm -f $(EXAMPLES)
	@rm -f $(DEPENDENCIES)
	@rm -rf $(BUILD_DIR)
	@rm -rf $(BIN_DIR)

# Other stuff ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#

# include automatically generated dependencies
-include $(DEPENDENCIES)

#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~#
