CC = g++
CFLAGS=-c -Wall
LIB = ../../driver/src
INCLUDES = -I ../../driver/include

all: rtc

rtc: rtc.o ds1339.o i2c.o
	$(CC) rtc.o ds1339.o i2c.o -o rtc
	rm -rf *.o

rtc.o: rtc.cpp
	$(CC) $(INCLUDES) $(CFLAGS) rtc.cpp

i2c.o: $(LIB)/i2c.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/i2c.cpp

ds1339.o: $(LIB)/ds1339.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/ds1339.cpp

clean:
	rm -rf *.o rtc
