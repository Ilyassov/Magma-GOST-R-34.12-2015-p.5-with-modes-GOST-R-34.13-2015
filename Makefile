CC=g++
CFLAGS=-c -Wall

all: magma

magma:	magma.o functions.o
	$(CC) magma.cpp functions.cpp -o magma

magma.o:	magma.cpp headers.h
	$(CC) $(CFLAGS) -c magma.cpp

functions.o: functions.cpp headers.h
	$(CC) $(CFLAGS) -c functions.cpp

clean:
	rm -rf *.o magma
