GLOBAL_LIBS= -lpthread -lrt -L. -ljson -lm
CC=gcc
LD=gcc
PROJECT_BASE=/home/aviva/random-projects/clinuxtot
BIN_OUTPUT_DIRECTORY:=$(PROJECT_BASE)/bin
DEBUG_FLAGS = -DCONFIGURATION_DEBUG
COMPL_FLAGS = -std=gnu11 -O0 #-fsanitize=address
PROJECT_BASE:=/home/aviva/random-projects/clinuxtot
BIN_OUTPUT_DIRECTORY:=$(PROJECT_BASE)/bin/
DIRS = libs client test server 
.SUFFIX = $(empty)
.PHONY: all $(DIRS)
clobber all: $(DIRS)
#	$(foreach subdir,$(DIRS),cd $(subdir) && $(MAKE) -C $(subdir)/Makefile)
export BIN_OUTPUT_DIRECTORY
export PROJECT_BASE
export CC
export LD
export COMPL_FLAGS
export DEBUG_FLAGS
export GLOBAL_LIBS

$(DIRS):
	cd $@ && $(MAKE) -C ./ $(MAKECMDGOALS)

.SILENT: clobber
	
#server: $(SERVER_SOURCE) $(HEADERS)
#	$(GCC) $(COMPL_FLAGS) $(DEBUG_FLAGS) -g $(SERVER_SOURCE) -o $@  $(LIB)
#
#client: $(CLIENT_SOURCE) $(HEADERS)
#	$(GCC) $(COMPL_FLAGS) -g $(CLIENT_SOURCE) -o $@  $(LIB)
#
#test: $(TEST_SOURCE) $(HEADERS)
#	$(GCC) $(COMPL_FLAGS) -g $(TEST_SOURCE) -o $@  $(LIB)
#
#
#JSON_OBJECT: $(JSON_SOURCE)
#	$(GCC) -c $^ -o $(JSON_OBJECT)
#
#libjson: $(JSON_OBJECT)
#	ar rcs $@.a $(JSON_OBJECT) 
