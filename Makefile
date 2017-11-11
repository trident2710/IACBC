CC=gcc
CFLAGS=-Wall
EXEC= encrypt

encrypt:encrypt.o libiacbc.o
	$(CC) $(CFLAGS) -o $@ $<

encrypt.o: encrypt.c
	$(CC) $(CFLAGS) -c $< -o $@

libiacbc.o:libiacbc.c libiacbc.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf *~
	rm -rf $(EXEC)
