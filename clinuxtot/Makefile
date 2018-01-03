GLOBAL_LIB = -lpthread -lrt -L. -ljson -lm
GCC = gcc
CLIENT_SOURCE = client.c process_conf.c
DEBUG_FLAGS = -DCONFIGURATION_DEBUG
COMPL_FLAGS = -std=gnu11 -O0 -fsanitize=address
DIRS = libs client test server 
.SUFFIX = $(empty)
.PHONY: all $(DIRS)
clobber all: $(DIRS)
#	$(foreach subdir,$(DIRS),cd $(subdir) && $(MAKE) -C $(subdir)/Makefile)

$(DIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

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

	
	
