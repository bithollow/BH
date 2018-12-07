/*
PCA9685 driver
*/

#ifndef PCA9685_H_
#define PCA9685_H_

namespace BH {

#define PCA9685_DEFAULT_ADDRESS   0x55 
#define PCA9685_PWM_COUNT         4096

#define PCA9685_MODE1             0x00
#define PCA9685_MODE2             0x01

#define PCA9685_LED0_ON_L         0x06
#define PCA9685_LED0_ON_H         0x07
#define PCA9685_LED0_OFF_L        0x08
#define PCA9685_LED0_OFF_H        0x09

#define PCA9685_LED13_ON_L        0x3A
#define PCA9685_LED13_ON_H        0x3B
#define PCA9685_LED13_OFF_L       0x3C
#define PCA9685_LED13_OFF_H       0x3D

#define PCA9685_LED14_ON_L        0x3E
#define PCA9685_LED14_ON_H        0x3F
#define PCA9685_LED14_OFF_L       0x40
#define PCA9685_LED14_OFF_H       0x41

#define PCA9685_LED15_ON_L        0x42
#define PCA9685_LED15_ON_H        0x43
#define PCA9685_LED15_OFF_L       0x44
#define PCA9685_LED15_OFF_H       0x45

#define PCA9685_MODE1_BIT_RESTART   7
#define PCA9685_MODE1_BIT_EXTCLK    6
#define PCA9685_MODE1_BIT_AI        5
#define PCA9685_MODE1_BIT_SLEEP     4
#define PCA9685_MODE1_BIT_SUB1      3
#define PCA9685_MODE1_BIT_SUB2      2
#define PCA9685_MODE1_BIT_SUB3      1
#define PCA9685_MODE1_BIT_ALLCALL   0

#define PCA9685_MODE2_BIT_INVRT     4
#define PCA9685_MODE2_BIT_OCH       3
#define PCA9685_MODE2_BIT_OUTDRV    2
#define PCA9685_MODE2_BIT_OUTNE1    1
#define PCA9685_MODE2_BIT_OUTNE0    0

#define PCA9685_PRE_SCALE           0xFE

class PCA9685
{
public:
    PCA9685(uint8_t device_address = PCA9685_DEFAULT_ADDRESS);
    ~PCA9685();

    /** @brief initialize PCA9685 Chip.
    *  @return true if success, otherwise false.
    */
    bool init();
	
    /** @brief initialize PCA9685 Chip.
    *  @return current PCA9685 frequency.
    */
    float getFrequency();
	
    /** @brief set frequency.
    *  @param frequency setting frequency 
    *  @return.
    */
    void setFrequency(float frequency);

    /** @brief sleep.
    *  @return.
    */
    void sleep();
	
    /** @brief  restart the PCA9685 Chip.
    *  @return.
    */
    bool restart();

    /** @brief set PWM output.
    *  @param channel between 0-15
    *  @param delay delay length of 4096 (0-4096) 
    *  @param length pulse length out of 4096 		 
    *  @return.
    */
    void setPWM(uint8_t channel, uint16_t delay, uint16_t length);

    /** @brief set PWM output.
    *  @param channel between 0-15
    *  @param delay_time pulse delay time(in ms)
    *  @param length_time pulse length time(in mS) 		 
    *  @return.
    */
    void setPWMS(uint8_t channel, uint16_t delay_time, float length_time);

private:

    uint8_t dev_address;
    float frequency;
};

} // end of BH

#endif // PCA9685_H_
