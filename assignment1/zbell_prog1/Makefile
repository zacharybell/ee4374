PROG_NAME = zbell_prog1
OBJECTS   = zbell_prog1.o zbell_argtok.o

$(PROG_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $? $(LDFLAGS) -o $@

$(OBJECTS): zbell_argtok.h

clean:
	rm -f *.o core *.core $(PROG_NAME)

