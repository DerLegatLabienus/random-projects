APPNAME = $(shell basename `pwd`)
SOURCE := $(wildcard *.c)
HEADERS := $(wildcard *.h)
CCFLAGS := -std=c11 -g -D_POSIX_C_SOURCE="200809L"
CC = gcc
LIB :=  -pthread
OBJECTS := $(patsubst %.c, %.o, ${SOURCE})

${APPNAME}: ${OBJECTS}
	$(CC) ${OBJECTS} ${LIB} -o $@

$(OBJECTS): $(SOURCE)
	$(CC) $(DEBUG) $(CCFLAGS) -c ${SOURCE} 



.PHONY: clean clobber

clean: clobber

clobber: 
	@-/bin/rm -r ${OBJECTS} ${APPNAME} ||:
