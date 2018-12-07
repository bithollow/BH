/*
MS5611 driver
*/

#include "ms5611.h"

using namespace BH;

MS5611::MS5611(uint8_t device_address)
  :dev_address(device_address)
{
}

MS5611::~MS5611()
{
}

bool MS5611::init()
{
    readCalCoef(); 
    raw_data[0] = readDx(MS5611_CMD_D1_OSR_4096);
    raw_data[1] = readDx(MS5611_CMD_D2_OSR_4096);

    return true;
}

bool MS5611::reset()
{
    int8_t status = IIC::writeByte(RPI2_I2C_1, dev_address, MS5611_CMD_RESET, 0);
    if (status > 0) {
        return true;
    }
    else {
        return false;	
    }
}

uint16_t MS5611::readProm(uint8_t cmd)
{
    uint8_t buffer[2] = {0}; 
    IIC::readBytes(RPI2_I2C_1, dev_address, cmd, 2, buffer);

    uint16_t calibration = buffer[0] << 8 | buffer[1];
    return calibration;
}

void MS5611::readCalCoef(void)
{
    cal_data[0] = readProm(MS5611_CMD_RD_PROM_1);
    cal_data[1] = readProm(MS5611_CMD_RD_PROM_2);
    cal_data[2] = readProm(MS5611_CMD_RD_PROM_3);
    cal_data[3] = readProm(MS5611_CMD_RD_PROM_4);
    cal_data[4] = readProm(MS5611_CMD_RD_PROM_5);
    cal_data[5] = readProm(MS5611_CMD_RD_PROM_6);
}

uint32_t MS5611::readDx(uint8_t cmd)
{
    IIC::writeBytes(RPI2_I2C_1, dev_address, cmd, 0, 0);

    uint8_t buffer[3] = {0};
    usleep(10000);
    IIC::readBytes(RPI2_I2C_1, dev_address, MS5611_CMD_RD_ADC, 3, buffer);
    uint32_t Dx = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
    return Dx;
}

void MS5611::refresh()
{
    raw_data[0] = readDx(MS5611_CMD_D1_OSR_4096);
    raw_data[1] = readDx(MS5611_CMD_D2_OSR_4096);
    calPressureTemperature();
}

void MS5611::calPressureTemperature()
{
    float dt = raw_data[1] - cal_data[4] * pow(2, 8);
    temperature = (2000 + ((dt * cal_data[5]) / pow(2, 23)));
    float off = cal_data[1] * pow(2, 16) + (cal_data[3] * dt) / pow(2, 7);
    float sens = cal_data[0] * pow(2, 15) + (cal_data[2] * dt) / pow(2, 8);
    float temp2, off2, sens2;

    if (temperature >= 2000) {
        temp2 = 0;
        off2 = 0;
        sens2 = 0;
    }
    if (temperature < 2000) {
        temp2 = dt * dt / pow(2, 31);
        off2 = 5 * pow(temperature - 2000, 2) / 2;
        sens2 = off2 / 2;
    }
    if (temperature < -1500) {
        off2 = off2 + 7 * pow(temperature + 1500, 2);
        sens2 = sens2 + 11 * pow(temperature + 1500, 2) / 2;
    }

    temperature = temperature - temp2;
    off = off - off2;
    sens = sens - sens2;

    // Final calculations
    pressure = ((raw_data[0] * sens) / pow(2, 21) - off) / pow(2, 15) / 100;
    temperature = temperature / 100;
}

float MS5611::getTemperature()
{
    return temperature;
}

float MS5611::getPressure()
{
    return pressure;
}
