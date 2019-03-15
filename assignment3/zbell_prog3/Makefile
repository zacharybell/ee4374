PROG_NAME = zbell_prog3
OBJECTS   = zbell_prog3.o zbell_prime.o
CFLAGS    = -pthread
LDFLAGS   = -lm

$(PROG_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJECTS): zbell_prime.h

clean:
	rm *.o *.txt; rm zbell_prog3
