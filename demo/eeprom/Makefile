CC = g++
CFLAGS=-c -Wall
LIB = ../../driver/src
INCLUDES = -I ../../driver/include

all: eeprom

eeprom: eeprom.o cat24c256.o i2c.o
	$(CC) eeprom.o cat24c256.o i2c.o -o eeprom
	rm -rf *.o

eeprom.o: eeprom.cpp
	$(CC) $(INCLUDES) $(CFLAGS) eeprom.cpp

cat24c256.o: $(LIB)/cat24c256.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/cat24c256.cpp

i2c.o: $(LIB)/i2c.cpp
	$(CC) $(INCLUDES) $(CFLAGS) $(LIB)/i2c.cpp

clean:
	rm -rf *.o eeprom