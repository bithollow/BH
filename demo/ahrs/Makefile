CC = g++
CFLAGS=-c -Wall
LIB = ../../driver/src
INCLUDES = -I ../../driver/include

all: ahrs

ahrs: ahrs.o mpu9250.o i2c.o
	$(CC) ahrs.o mpu9250.o i2c.o -o ahrs
	rm -rf *.o

ahrs.o: ahrs.cpp
	$(CC) $(INCLUDES) $(CFLAGS) ahrs.cpp -lrt -lpthread

mpu9250.o: $(LIB)/mpu9250.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/mpu9250.cpp

i2c.o: $(LIB)/i2c.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/i2c.cpp

clean:
	rm -rf *.o ahrs