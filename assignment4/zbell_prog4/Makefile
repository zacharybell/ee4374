PROG_NAME = bankClient
OBJECTS   = bankClient.o

$(PROG_NAME): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJECTS): banking.h

clean:
	rm *.o; rm bankClient
