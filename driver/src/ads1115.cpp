/*
ADS1115 driver
*/

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "ads1115.h"
#include "i2c.h"

using namespace BH;

ADS1115::ADS1115(uint8_t device_address)
  : dev_address(device_address)
{
}

ADS1115::~ADS1115()
{
}

bool ADS1115::init()
{
    uint8_t data;
    int8_t status = IIC::readByte(RPI2_I2C_1, dev_address, ADS1115_RA_CONFIG, &data);
    if (status <= 0) {
        return false;
    }

    memset(&config, 0, sizeof(config));
    setGain(ADS1115_PGA_4P096);
    setMultiplexer(ADS1115_MUX_P0_NG);
    setMode(ADS1115_MODE_SINGLESHOT);
    setComparatorQueueMode(ADS1115_COMP_QUE_DISABLE);

    return true;
}

void ADS1115::updateConfigRegister()
{
    uint16_t c;

    /* New config */
    c = config.status | config.mux | config.gain |
        config.mode | config.rate | config.comparator |
        config.polarity | config.latch | config.queue;

    if (IIC::writeWord(RPI2_I2C_1, dev_address, ADS1115_RA_CONFIG, c) < 0) {
        fprintf(stderr, "Error while writing config\n");
    }
}

int16_t ADS1115::getConversion()
{
    union {
        uint16_t w;
        uint8_t b[2];
    } word;
    word.w = 0;

    if (config.mode == ADS1115_MODE_SINGLESHOT ) {
        /* Check for Operation Status. If it is 0 then we are ready to get data. Otherwise wait. */
        setOpStatus(ADS1115_OS_ACTIVE);
        while ((word.w & 0x80) == 0) {
            if (IIC::readWord(RPI2_I2C_1, dev_address, ADS1115_RA_CONFIG, &word.w) < 0 )
                fprintf(stderr, "Error while reading config\n");
        }
    }

    if ((IIC::readWord(RPI2_I2C_1, dev_address, ADS1115_RA_CONVERSION, &word.w)) < 0 ) {
        fprintf(stderr, "Error while reading\n");
    }
    /* Exchange MSB and LSB */
    word.w = word.b[0] << 8 | word.b[1];
    return (int16_t) word.w;
}

void ADS1115::setOpStatus(uint16_t status)
{
    config.status = status;
    updateConfigRegister();
}

uint16_t ADS1115::getMultiplexer()
{
    return config.mux;
}

void ADS1115::setMultiplexer(uint16_t mux)
{
    if (config.mux != mux) {
        config.mux = mux;
        updateConfigRegister();
    }
}

uint16_t ADS1115::getGain()
{
    return config.gain;
}

void ADS1115::setGain(uint16_t gain)
{
    if (config.gain != gain) {
        config.gain = gain;
        updateConfigRegister();
    }
}

uint16_t ADS1115::getMode() {
    return config.mode;
}

void ADS1115::setMode(uint16_t mode)
{
    if (config.mode != mode) {
        config.mode = mode;
        updateConfigRegister();
    }
}

uint16_t ADS1115::getRate()
{
    return config.rate;
}

void ADS1115::setRate(uint16_t rate)
{
    if (config.rate != rate) {
        config.rate = rate;
        updateConfigRegister();
    }
}

void ADS1115::showConfigRegister()
{
    union {
        uint16_t w;
        uint8_t b[2];
    } buf;
    IIC::readWord(RPI2_I2C_1, dev_address, ADS1115_RA_CONFIG, &buf.w);

    printf("Config Register: 0x%04x | 0x%02x 0x%02x", buf.w, buf.b[0], buf.b[1]);
}

float ADS1115::getMilliVolts() {
  switch (config.gain) {
    case ADS1115_PGA_6P144:
      return (getConversion() * ADS1115_MV_6P144);
      break;
    case ADS1115_PGA_4P096:
      return (getConversion() * ADS1115_MV_4P096);
      break;
    case ADS1115_PGA_2P048:
      return (getConversion() * ADS1115_MV_2P048);
      break;
    case ADS1115_PGA_1P024:
      return (getConversion() * ADS1115_MV_1P024);
      break;
    case ADS1115_PGA_0P512:
      return (getConversion() * ADS1115_MV_0P512);
      break;
    case ADS1115_PGA_0P256:
    case ADS1115_PGA_0P256B:
    case ADS1115_PGA_0P256C:
      return (getConversion() * ADS1115_MV_0P256);
      break;
    default:
      fprintf(stderr, "Wrong gain\n");
      return -1;
      break;
  }
}

void ADS1115::setComparatorMode(uint16_t comparator)
{
    if (config.comparator != comparator) {
        config.comparator = comparator;
        updateConfigRegister();
    }
}

void ADS1115::setComparatorPolarity(uint16_t polarity)
{
    if (config.polarity != polarity) {
        config.polarity = polarity;
        updateConfigRegister();
    }
}

void ADS1115::setComparatorLatchEnabled(uint16_t latch)
{
    if (config.latch != latch) {
        config.latch = latch;
        updateConfigRegister();
    }
}

void ADS1115::setComparatorQueueMode(uint16_t queue)
{
    if (config.queue != queue) {
        config.queue = queue;
        updateConfigRegister();
    }
}
