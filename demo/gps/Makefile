CC=g++
CFLAGS=-c -Wall
LIB = ../../driver/src
INCLUDES = -I ../../driver/include

all: gps

gps: gps.o ublox.o
	$(CC) gps.o ublox.o -o gps
	rm -rf *.o

gps.o: gps.cpp
	$(CC) $(INCLUDES) $(CFLAGS) gps.cpp

ublox.o: $(LIB)/ublox.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/ublox.cpp

clean:
	rm -rf *.o gps
