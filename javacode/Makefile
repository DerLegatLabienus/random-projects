JFLAGS = -g -I.
CFLAGS += -g -I. --std=c++11
CC = g++

JC = gcj 

.SUFFIXES: 
.SUFFIXES: *.java *.cc *.c *.cpp *.class *.o

%.o: %.cpp 
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
%.o: %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
%.o: %.cc
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@ $<
%.class: %.java
	$(JC) -C $(JFLAGS) $*.java

CLASSES = ${wildcard *.java} 
CPPSOURCE = ${wildcard *.cc}
JAVAOBJ :=  $(CLASSES:.java=.class)
CPPOBJ :=  $(CPPSOURCE:.cc=.o) 
CPPSOURCE = ${wildcard *.c}
CPPOBJ +=  $(CPPSOURCE:.c=.o) 
CPPSOURCE = ${wildcard *.cpp}
CPPOBJ +=  $(CPPSOURCE:.cpp=.o) 
CPPSOURCE = ${wildcard *.c}
CPPOBJ +=  $(CPPSOURCE:.c=.o) 

default: javacode

javacode: $(CPPOBJ) $(JAVAOBJ)
	$(JC) $(JLNKFLAGS) -o $@ $^

clean:
	        $(RM) *.class
		$(RM) javacode
		$(RM) *.o
