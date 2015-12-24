CC = g++
CFLAGS=-c -Wall
LIB = ../../driver/src
INCLUDES = -I ../../driver/include

all: baro

baro: baro.o ms5611.o i2c.o
	$(CC) baro.o ms5611.o i2c.o -o baro
	rm -rf *.o

baro.o: baro.cpp
	$(CC) $(INCLUDES) $(CFLAGS) baro.cpp

ms5611.o: $(LIB)/ms5611.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/ms5611.cpp

i2c.o: $(LIB)/i2c.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/i2c.cpp

clean:
	rm -rf *.o baro