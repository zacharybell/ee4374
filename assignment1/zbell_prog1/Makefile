zbell_prog1: zbell_prog1.c zbell_argtok.c
	$(CC) $(CFLAGS) $? $(LDFLAGS) -o $@

zbell_prog1.o: zbell_argtok.h
zbell_argtok.o: zbell_argtok.h

clean:
	rm -f *.o core *.core $@

