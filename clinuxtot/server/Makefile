SOURCE=${wildcard *.c} $(PROJECT_BASE)/common/*.c
OBJECTS=$(subst .c,.o,$(SOURCE))
LIBS=$(GLOBAL_LIBS)
LIB_PATH := -L$(PROJECT_BASE)/libs/
INCLUDE_PATH := -I$(PROJECT_BASE)/common
DEBUG_FLAGS = -DCONFIGURATION_DEBUG
COMPL_FLAGS = -std=gnu11 -O0 

#derieve name of binary from folder name
NAME = $(lastword $(subst /, ,$(dir $(abspath $(lastword $(MAKEFILE_LIST))))))

.PHONY: all

%.o: %.c 
	$(CC) $(INCLUDE_PATH) $(COMPL_FLAGS) $(DEBUG_FLAGS) -o $@ -c $<

all: $(NAME)

$(NAME): $(OBJECTS)
	$(LD) $^ $(LIBS) $(LIB_PATH) $(GLOBAL_LDFLAGS) -o $(BIN_OUTPUT_DIRECTORY)/$@ 


.SILENT: clobber
clobber:
	@/bin/rm -f *.o


project: 
	cscope -Rb
	ctags *.c *.h
