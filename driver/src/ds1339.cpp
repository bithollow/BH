/*
DS1339 driver.
*/

#include <stdio.h>
#include <unistd.h>
#include "ds1339.h"

using namespace BH;

const char *DS1339::week_days[] = {"Dg", "Dl", "Dt", "Dc", "Dj", "Dv", "Ds"};

DS1339::DS1339(uint8_t device_address)
  : dev_address(device_address)
{
}

DS1339::~DS1339()
{
}

bool DS1339::init()
{
    return true;
}

bool DS1339::setTime(RtcTime& time, bool start, bool thm)
{
    uint8_t buffer[7];
    printf("reading clock registers to write the new time : %d:%d:%d\n", time.hour, time.min, time.sec);

    if (!read(0, buffer, 7)) {
        printf("Failed to read from RTC\n");
        return false;
    }

    //  Now update only the time part (saving the existing flags)
    if (start) {
        buffer[0] &= 0x7F;
    }
    else {
        buffer[0] |= 0x80;
    }

    buffer[0] = (buffer[0]&0x80) | (decimalToBcd(time.sec)& 0x7f);
    buffer[1] = decimalToBcd(time.min);

    if (thm) {
        //  AM PM format
        buffer[2] = (buffer[2]& 196) | (time.hour>12 ? (0x20 | ((decimalToBcd(time.hour-12)))) : decimalToBcd(time.hour));
    }
    else {
        // 24 hours format
        buffer[2] = (buffer[2]& 196) | (decimalToBcd(time.hour) & 0x3F);
    }

    buffer[3] = time.wday;
    buffer[4] = decimalToBcd(time.date);
    buffer[5] = decimalToBcd(time.mon);
    buffer[6] = decimalToBcd(time.year-2000);
    printf("Writing new time and date data to RTC\n");

    if (!write(0, buffer, 7) ) {
        printf("Failed to write the data to RTC!\n");
        return false;
    }
  
    return true;
}

void DS1339::printfTime(RtcTime& time)
{
    printf("current time is : year=%d, mon=%d, date=%d, wday=%d, hour=%d, min=%d, sec=%d\n",
    	    time.year, time.mon, time.date, time.wday, time.hour, time.min, time.sec);
}

bool DS1339::getTime(RtcTime& time)
{
    uint8_t buffer[7];
    bool thm = false;

    printf("Getting time from RTC\n");
    if (!read(0, buffer, 7) ) {
        //  Failed to read
        printf("Failed to read from RTC\n");
        return false;
    }

    thm = ((buffer[2] & 64) == 64);
    time.sec = bcdToDecimal(buffer[0]&0x7F);
    time.min = bcdToDecimal(buffer[1]);
    if (thm) {
        // in 12-hour-mode, we need to add 12 hours if PM bit is set
        time.hour = DS1339::bcdToDecimal( buffer[2] & 31 );
        if ((buffer[2] & 32) == 32)
           time.hour += 12;
    }
    else {
        time.hour = DS1339::bcdToDecimal( buffer[2] & 63 );
    }
    
    time.wday = buffer[3];
    time.date = DS1339::bcdToDecimal( buffer[4]);
    time.mon = DS1339::bcdToDecimal( buffer[5]);
    time.year = DS1339::bcdToDecimal(buffer[6]) + 2000;   //  plus hundret is because RTC is giving the years since 2000, but std c struct tm needs years since 1900

    return true;
}

bool DS1339::startClock()
{
    uint8_t strtStop = 0;

    printf ("Reading clock start/stop register value\n");
    if (!read(0XE0, &strtStop, 1)) {
        printf("Failed to read clock start stop register !\n");
        return false;
    }

    printf("current start/stop register value = %d\n", strtStop);
    strtStop &= 0x7F;
    printf("Writing back start/stop register value\n");
    if (!write(0xE0, &strtStop, 1)) {
        printf("Faied to write the start stop register !\n");
        return false;
    }

    printf("Start/stop register value successfully written\n");
    return true;
}

bool DS1339::stopClock()
{
    uint8_t strtStop;

    printf ("Reading clock start/stop register value\n");
    if (!read(0, &strtStop, 1)) {
        printf("Failed to read clock start stop register !\n");
        return false;
    }

    strtStop |= 0x80;
    printf("Writing back start/stop register value\n");
    if (!write(0, &strtStop, 1)) {
        printf("Failed to write the start stop register !\n");
        return false;
    }

    printf("Start/stop register value successfully written\n");
    return true;
}

bool DS1339::setSquareWaveOutput(bool ena, SqwRateSelect rs)
{
    uint8_t reg;
    printf("Reading register value first\n");

    if (!read(7,&reg, 1)) {
        printf("Failed to read register value !\n");
        return false;
    }
    printf("[Reg:0x07] = %02x\n", reg);

    //  preserve the OUT control bit while writing the frequency and enable bits
    reg = (reg & 0x80) | (ena ? 0x10 : 0) | ((uint8_t)rs & 0x03);

    printf("Writing back register value\n");
    printf("[Reg:0x07] = %02x\n", reg);

    if (!write(7, &reg, 1)) {
        printf("Failed to write register value !\n");
        return false;
    }

    printf("Successfully changed the square wave output.\n");
    return true;
}

bool DS1339::read(uint8_t address, uint8_t *buffer, int len)
{
    if (IIC::readBytes(RPI2_I2C_1, dev_address, address, len, buffer) == 0) {
        printf("Failed to read register !\n");
        return false;
    }
  
    printf("Successfully read %d registers from RTC\n", len);
    return true;
}

bool DS1339::write(uint8_t address, uint8_t *buffer, int len)
{
    if (IIC::writeBytes(RPI2_I2C_1, dev_address, address, len, buffer) == 0) {
        printf("Failed to write data to rtc\n");
        return false;
    }

    return true;
}
