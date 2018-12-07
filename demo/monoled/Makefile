CC = g++
CFLAGS=-c -Wall
LIB = ../../driver/src
INCLUDES = -I ../../driver/include

all: monoled

monoled: monoled.o gpio.o
	$(CC) monoled.o gpio.o -o monoled
	rm -rf *.o

monoled.o: monoled.cpp
	$(CC) $(INCLUDES) $(CFLAGS) monoled.cpp

gpio.o: $(LIB)/gpio.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/gpio.cpp

clean:
	rm -rf *.o monoled
