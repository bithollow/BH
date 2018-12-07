CC = g++
CFLAGS=-c -Wall
LIB = ../../driver/src
INCLUDES = -I ../../driver/include

all: adc

adc: adc.o ads1115.o i2c.o
	$(CC) adc.o ads1115.o i2c.o -o adc
	rm -rf *.o

adc.o: adc.cpp
	$(CC) $(INCLUDES) $(CFLAGS) adc.cpp

ads1115.o: $(LIB)/ads1115.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/ads1115.cpp

i2c.o: $(LIB)/i2c.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/i2c.cpp

clean:
	rm -rf *.o adc
