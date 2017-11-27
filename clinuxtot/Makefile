LIB = -lpthread -lrt -L. -ljson -lm
NAME = server
NAME_CLIENT = client
GCC = gcc
HEADERS = main.h 
SERVER_SOURCE = main.c process_conf.c
TEST_SOURCE = test.c
CLIENT_SOURCE = client.c process_conf.c
DEBUG_FLAGS = -DCONFIGURATION_DEBUG
COMPL_FLAGS = -std=gnu11 -O0 
#COMPL_FLAGS = -std=gnu11 -S
JSON_SOURCE = json.c json.h
JSON_OBJECT = json.o

.PHONY: all clobber project
all: project libjson server client test
server: $(SERVER_SOURCE) $(HEADERS)
	$(GCC) $(COMPL_FLAGS) $(DEBUG_FLAGS) -g $(SERVER_SOURCE) -o $@  $(LIB)

client: $(CLIENT_SOURCE) $(HEADERS)
	$(GCC) $(COMPL_FLAGS) -g $(CLIENT_SOURCE) -o $@  $(LIB)

test: $(TEST_SOURCE) $(HEADERS)
	$(GCC) $(COMPL_FLAGS) -g $(TEST_SOURCE) -o $@  $(LIB)


JSON_OBJECT: $(JSON_SOURCE)
	$(GCC) -c $^ -o $(JSON_OBJECT)

libjson: $(JSON_OBJECT)
	ar rcs $@.a $(JSON_OBJECT) 


.SILENT: clobber
clobber:
	@/bin/rm -f libjson server client test tags cscope.out 


project: 
	cscope -Rb
	ctags *.c *.h
