CC=gcc
MV=mv
PROJECT_BASE=/home/aviva/random-projects/clinuxtot
SOURCE=${wildcard *.c}
OBJECTS=$(subst .c,.o,$(SOURCE))
INCLUDE_PATH:=-I$(PROJECT_BASE)/common -I.
.SUFFIX = $(empty)

#derieve name of binary from folder name
NAME = $(lastword $(subst /, ,$(dir $(abspath $(lastword $(MAKEFILE_LIST))))))

.PHONY: all clobber

%.o: %.c 
	$(CC) $(INCLUDE_PATH) $(COMPL_FLAGS) $(DEBUG_FLAGS) -o $@ -c $<

all: $(NAME).a

$(NAME).a: $(OBJECTS)
	$(AR) rc $@  $^
	$(MV) $@ $(PROJECT_BASE)/libs/


.SILENT: clobber
clobber:
	@/bin/rm -f *.o $(PROJECT_BASE)/libs/$(NAME).a

project: 
	cscope -Rb
	ctags *.c *.h
