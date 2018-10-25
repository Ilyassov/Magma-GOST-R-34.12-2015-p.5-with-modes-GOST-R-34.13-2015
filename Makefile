CC=g++
CFLAGS=-c -Wall

all: main
# vector.o
main:	main.o functions.o
	$(CC) main.cpp functions.cpp -o main
# key.cpp vector.cpp
main.o:	main.cpp headers.h
	$(CC) $(CFLAGS) -c main.cpp

#vector.o: vector.cpp vector.h
#	$(CC) $(CFLAGS) -c vector.cpp

functions.o: functions.cpp headers.h
	$(CC) $(CFLAGS) -c functions.cpp

#key.o : key.cpp key.h
#	$(CC) $(CFLAGS) -c key.cpp

clean:
	rm -rf *.o main
