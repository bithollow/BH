/*
CAT24C256 driver.
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <linux/i2c-dev.h>
#include "cat24c256.h"
#include "i2c.h"

using namespace BH;

CAT24C256::CAT24C256(uint8_t device_address)
  :dev_address(device_address)
{
}

CAT24C256::~CAT24C256()
{
}

uint8_t CAT24C256::readByte(uint16_t read_address, uint8_t* data)
{
    return readBytes(read_address, 1, data);
}

uint8_t CAT24C256::writeByte(uint16_t write_address, uint8_t data)
{
    return writeBytes(write_address, 1, &data);
}

uint8_t CAT24C256::writeBytes(uint16_t write_address, uint8_t length, uint8_t* data)
{
   uint8_t msg[length + 2] = {0};
   int file = open("/dev/i2c-0", O_RDWR);
   if (file < 0) {
	printf("open error\n");
   }
    
   if (ioctl(file, I2C_SLAVE, 0x50) < 0) {
	printf("ioctl error\n");
   }

   msg[0] = write_address >> 8;
   msg[1] = write_address & 0xFF;
   memcpy(&msg[2], data, length);

   int count = write(file, msg, length + 2);
   printf("write...count=%d\n", count);
   if (count != (length + 2)) {
      printf("write error \n");   
   }
   close(file);
}

uint8_t CAT24C256::readBytes(uint16_t read_address, uint8_t length, uint8_t* data)
{
    uint8_t reg_address_low = read_address & 0xFF; // lower part of the address
    uint8_t reg_address_high = read_address >> 8; // higher part of the address
       
    uint8_t reg_data[10] = {0};
    reg_data[0] = read_address >> 8;
    reg_data[1] = read_address & 0xff;      
    printf("CAT24C256::readBytes reg_data[0]=%d, reg_data[1]=%d\n", reg_data[0], reg_data[1]);    
 
    int fd = open("/dev/i2c-0", O_RDWR);
    if (fd < 0) {
        fprintf(stderr, "Failed to open device: %s\n", strerror(errno));
        return(-1);
    }
    
    if (ioctl(fd, I2C_SLAVE, 0x50) < 0) {
        fprintf(stderr, "Failed to select device: %s\n", strerror(errno));
        close(fd);
        return(-1);
    }
    
    int count = write(fd, reg_data, 2);
    printf("read write count=%d\n", count);
    if (count != 2) {
        fprintf(stderr, "Failed to write reg: %s\n", strerror(errno));
        close(fd);
        return(-1);
    }

    count = read(fd, data, length);
    if (count < 0) {
        fprintf(stderr, "Failed to read device(%d): %s\n", count, ::strerror(errno));
        close(fd);
        return(-1);
    } else if (count != length) {
        fprintf(stderr, "Short read from device, expected %d, got %d\n", length, count);
        close(fd);
        return(-1);
    }

    close(fd);   
    printf("read..cont=%d\n", count);
    return count;
}
