CC=gcc
LD=gcc
PROJECT_BASE=/home/aviva/random-projects/clinuxtot

SOURCE=${wildcard *.c} $(wildcard *.h)
OBJECTS=$(subst .c,.o,$(SOURCE))
LIBS=$(GLOBAL_LIBS) -lrt -lpthread -ljson -lm -lc
LIB_PATH := -L$(PROJECT_BASE)/libs/
INCLUDE_PATH := -I$(PROJECT_BASE)/common

#derieve name of binary from folder name
NAME = $(lastword $(subst /, ,$(dir $(abspath $(lastword $(MAKEFILE_LIST))))))

.PHONY: all

%.o: %.c 
	$(CC) $(INCLUDE_PATH) $(LIBS) $(LIB_PATH)  $(COMPL_FLAGS) $(DEBUG_FLAGS) -o $@ -c $<

all: $(NAME)

$(NAME): $(OBJECTS)
	$(LD) $^ $(LIBS) $(LIB_PATH) $(GLOBAL_LDFLAGS) -o $@ 


.SILENT: clobber
clobber:
	@/bin/rm -f *.o


project: 
	cscope -Rb
	ctags *.c *.h
