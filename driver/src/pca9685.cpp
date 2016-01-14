/*
PCA9685 driver
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include "i2c.h"
#include "pca9685.h"

using namespace BH;

PCA9685::PCA9685(uint8_t device_address)
  : dev_address(device_address)
{
}

PCA9685::~PCA9685()
{
}

bool PCA9685::init()
{
    frequency = getFrequency();
    IIC::writeBit(RPI2_I2C_1, dev_address, PCA9685_MODE1, PCA9685_MODE1_BIT_AI, 1);

    uint8_t data;
    int8_t status = IIC::readByte(RPI2_I2C_1, dev_address, PCA9685_PRE_SCALE, &data);
    if (status > 0) {
        return true;
    } else {
        return false;
    } 
	restart();
}

void PCA9685::sleep() 
{
    IIC::writeBit(RPI2_I2C_1, dev_address, PCA9685_MODE1, PCA9685_MODE1_BIT_SLEEP, 1);
}

bool PCA9685::restart() 
{
    IIC::writeByte(RPI2_I2C_1, dev_address, PCA9685_MODE1, (1 << PCA9685_MODE1_BIT_SLEEP));
    IIC::writeByte(RPI2_I2C_1, dev_address, PCA9685_MODE1, ((1 << PCA9685_MODE1_BIT_RESTART) 
                                                                | (1 << PCA9685_MODE1_BIT_AI)));
    return true;
}

float PCA9685::getFrequency()
{
    uint8_t data = 0;
    IIC::readByte(RPI2_I2C_1, dev_address, PCA9685_PRE_SCALE, &data);
    return 24576000.f / 4096.f / (data + 1);
}

void PCA9685::setFrequency(float frequency)
{
    sleep();
    usleep(10000);
    uint8_t prescale = roundf(24576000.f / 4096.f / frequency)  - 1;
    IIC::writeByte(RPI2_I2C_1, dev_address, PCA9685_PRE_SCALE, prescale);
    frequency = getFrequency();
    usleep(10000); 
    restart();
}

void PCA9685::setPWM(uint8_t channel, uint16_t delay, uint16_t length)
{
    uint8_t reg_data[4] = {0};

    if(0 == length) {
        reg_data[3] = 0x10;
    } else if(length >= PCA9685_PWM_COUNT) {
        reg_data[1] = 0x10;
    } else {
        reg_data[0] = delay & 0xFF;
        reg_data[1] = delay >> 8;
        reg_data[2] = length & 0xFF;
        reg_data[3] = length >> 8;
    }

    IIC::writeBytes(RPI2_I2C_1, dev_address, PCA9685_LED0_ON_L + 4 * channel, 4, reg_data);
}

void PCA9685::setPWMS(uint8_t channel, uint16_t delay_time, float length_time) {
    uint16_t delay = round((delay_time * 4096.f) / (1000.f / frequency));
    float length = round((length_time * 4096.f) / (1000.f / frequency));
    setPWM(channel, delay, length);
}
