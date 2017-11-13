CC=g++
EXEC= encrypt
FLAGS= -W -Wall -Wextra -lssl -lcrypto

encrypt: encrypt.o libiacbc.o
	$(CC) encrypt.o libiacbc.o -o encrypt $(FLAGS)

encrypt.o: encrypt.cpp
	$(CC) -c encrypt.cpp

libiacbc.o: libiacbc.cpp
	$(CC) -c libiacbc.cpp

clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf *~
	rm -rf $(EXEC)
