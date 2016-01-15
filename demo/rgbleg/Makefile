CC = g++
CFLAGS=-c -Wall
LIB = ../../driver/src
INCLUDES = -I ../../driver/include

all: rgbled

rgbled: rgbled.o pca9685.o gpio.o i2c.o
	$(CC) rgbled.o pca9685.o gpio.o i2c.o -o rgbled
	rm -rf *.o

rgbled.o: rgbled.cpp
	$(CC) $(INCLUDES) $(CFLAGS) rgbled.cpp

pca9685.o: $(LIB)/pca9685.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/pca9685.cpp

gpio.o: $(LIB)/gpio.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/gpio.cpp

i2c.o: $(LIB)/i2c.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/i2c.cpp

clean:
	rm -rf *.o rgbled
