CC=g++
EXEC= asi331examen
FLAGS= -W -Wall -Wextra -lssl -lcrypto

asi331examen: iacbc.o libiacbc.o readinput.o
	$(CC) iacbc.o libiacbc.o readinput.o -o asi331examen $(FLAGS)

iacbc.o: iacbc.cpp
	$(CC) -c iacbc.cpp

libiacbc.o: libiacbc.cpp
	$(CC) -c libiacbc.cpp

readinput.o: readinput.cpp
		$(CC) -c readinput.cpp

clean:
	rm -rf *.o
	rm -rf *.a
	rm -rf *~
	rm -rf $(EXEC)
