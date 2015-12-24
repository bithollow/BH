/*
MS5611 driver
*/

#ifndef MS5611_H_
#define MS5611_H_

#include <math.h>
#include <unistd.h>
#include <string>
#include "i2c.h"

namespace BH {

#define MS5611_DEFAULT_ADDRESS   0x77

#define MS5611_CMD_RD_ADC        0x00
#define MS5611_CMD_RESET         0x1E

#define MS5611_CMD_D1_OSR_256    0x40
#define MS5611_CMD_D1_OSR_512    0x42
#define MS5611_CMD_D1_OSR_1024   0x44
#define MS5611_CMD_D1_OSR_2048   0x46
#define MS5611_CMD_D1_OSR_4096   0x48

#define MS5611_CMD_D2_OSR_256    0x50
#define MS5611_CMD_D2_OSR_512    0x52
#define MS5611_CMD_D2_OSR_1024   0x54
#define MS5611_CMD_D2_OSR_2048   0x56
#define MS5611_CMD_D2_OSR_4096   0x58

#define MS5611_CMD_RD_PROM_0     0xA0
#define MS5611_CMD_RD_PROM_1     0xA2
#define MS5611_CMD_RD_PROM_2     0xA4
#define MS5611_CMD_RD_PROM_3     0xA6
#define MS5611_CMD_RD_PROM_4     0xA8
#define MS5611_CMD_RD_PROM_5     0xAA
#define MS5611_CMD_RD_PROM_6     0xAC
#define MS5611_CMD_RD_PROM_7     0xAE

class MS5611
{
public:
    MS5611(uint8_t device_address = MS5611_DEFAULT_ADDRESS);
    ~MS5611();

    /** @brief initialize MS5611 Chip.
    *  @return.
    */
    bool init();

    /** @brief reset MS5611.
    *  @return.
    */
    bool reset();

    /** @brief refresh the data.
    *  @return.
    */
    void refresh();

    /** @brief  get calculated temperature value
    *  @return temperature in degrees of Celsius
    */	
    float getTemperature();	

    /** @brief  get calculated pressure value
    *  @return pressure in millibars
    */
    float getPressure();

private:
    /** @brief Calculate temperature and pressure
    *  @return.
    */
    void calPressureTemperature();

    /** @brief get the calculation coefficient
    *  @return.
    */
    void readCalCoef(void);
	
    /** @brief get the Prom value (refer to datasheet)
    *  @return prom value.
    */
    uint16_t readProm(uint8_t cmd);
	
    /** @brief get the Dx value (refer to datasheet)
    *  @return Dx value.
    */
    uint32_t readDx(uint8_t cmd);

    uint8_t dev_address;

    // Calibration data
    uint16_t cal_data[6]; 

    // Raw measurement data
    uint32_t raw_data[2]; 

    // Calculated Temperature
    float temperature; 

    // Calculated Pressure
    float pressure;    
};

} // end of BH

#endif // MS5611_H_
