#
# EE 4374 Assignment 2 Makefile
# Author: Zachary J Bell 
#

PROG_NAME = zbell_prog2
OBJECTS   = zbell_prog2.o zbell_argtok.o zbell_exec.o

$(PROG_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJECTS): zbell_argtok.h

zbell_prog2.o zbell_exec.o: zbell_exec.h

clean:
	rm *.o; rm zbell_prog2
