CC=g++
CFLAGS=-c -Wall

all: main

main:	main.o functions.o
	$(CC) main.cpp functions.cpp -o main

main.o:	main.cpp headers.h
	$(CC) $(CFLAGS) -c main.cpp

functions.o: functions.cpp headers.h
	$(CC) $(CFLAGS) -c functions.cpp

clean:
	rm -rf *.o main
