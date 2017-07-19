# Target will compile all the file

# variable declaration
CC=g++

CFLAGS=-c -Wall

all: hello

hello: main.o function.o function1.o
	$(CC) main.o function.o function1.o -o hello

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

function.o: function.cpp
	$(CC) $(CFLAGS) function.cpp

function1.o: function1.cpp
	$(CC) $(CFLAGS) function1.cpp

clean:
	rm -rf *.o hello
	