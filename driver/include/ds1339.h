#ifndef DS1339_H_
#define DS1339_H_

#include <stdint.h>
#include "i2c.h"

namespace BH {

#define DS1339_DEFAULT_ADDRESS 0x68

typedef struct {
    int sec;        /*!< seconds [0..59] */
    int min;        /*!< minutes {0..59] */
    int hour;       /*!< hours [0..23] */
    int wday;       /*!< weekday [1..7, where 1 = sunday, 2 = monday, ... */
    int date;       /*!< day of month [0..31] */
    int mon;        /*!< month of year [1..12] */
    int year;       /*!< year [2000..2255] */
} RtcTime;

/** 
* RateSelect specifies the valid frequency values for the square wave output                                                                
*/
typedef enum {
    RS1Hz = 0,
    RS4kHz = 1,
    RS8kHz = 2,
    RS32kHz = 3
} SqwRateSelect;

class DS1339
{
public:
    DS1339(uint8_t device_address = DS1339_DEFAULT_ADDRESS);
    ~DS1339();

    /**
     * @brief initialize DS1339
     * @return true if ok, false otherwise
    */
    bool init();

    /**
     * @brief printf Time
     * @param current time
    */
    void printfTime(RtcTime& time);

    /**
     * @brief printf Time
     * @param current time
     * @return true if ok, false otherwise
    */
    bool getTime(RtcTime& time);

    /**
     * @brief printf Time
     * @param derised time
     * @param start time
     * @param time fotmat
     * @return true if ok, false otherwise
    */	
    bool setTime(RtcTime& time, bool start, bool thm);

    /**
     * @brief start clock
     *
     * @return true if ok, false otherwise
    */
    bool startClock();

    /**
     * @brief stop clock
     *
     * @return true if ok, false otherwise
    */
    bool stopClock();

    /**
     * @brief output wave
     * @param derised time
     * @param control bit
     * @param control bit
     * @return true if ok, false otherwise
    */	
    bool setSquareWaveOutput(bool ena, SqwRateSelect rs);

    /**
     * @brief week day arrays
    */
    static const char *week_days[];

    /**
     * @brief week day to string
    */	
    const char* weekdayToString(int wday) {
        return week_days[wday%7];
    }

private:
    uint8_t dev_address;
    bool read(uint8_t address, uint8_t* buffer, int len);
    bool write(uint8_t address, uint8_t* buffer, int len);

    static int bcdToDecimal(int bcd) {
        return ((bcd & 0xF0) >> 4) * 10 + (bcd & 0x0F);
    }

    static int decimalToBcd(int dec) {
        return (dec % 10) + ((dec / 10) << 4);
    }
};

} // end of BH

#endif // DS1339_H_
