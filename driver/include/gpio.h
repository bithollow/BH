
#ifndef GPIO_H_
#define GPIO_H_

#include <stdint.h>

namespace BH {

/* Raspberry Pi GPIO mapping */
#define RPI_GPIO_2    2    // Pin 3     SDA
#define RPI_GPIO_3    3    // Pin 5     SCL
#define RPI_GPIO_4    4    // Pin 7     BH_PCA9685_OE

#define RPI_GPIO_7    7    // Pin 26    SPI CE1 BH_MPU9250_CS
#define RPI_GPIO_8    8    // Pin 24    SPI CE0 BH_UBLOX_CS
#define RPI_GPIO_9    9    // Pin 21    MISO
#define RPI_GPIO_10   10   // Pin 19    MOSI
#define RPI_GPIO_11   11   // Pin 23    SCLK

#define RPI_GPIO_14   14   // Pin 8     TxD
#define RPI_GPIO_15   15   // Pin 10    RxD

#define RPI_GPIO_5    5    // Pin 29    BH RC INPUT 
#define RPI_GPIO_6    6    // Pin 31    BH RC INPUT
#define RPI_GPIO_12   12   // Pin 32    BH RC INPUT
#define RPI_GPIO_13   13   // Pin 33    BH RC INPUT
#define RPI_GPIO_19   19   // Pin 35    BH RC INPUT
#define RPI_GPIO_20   20   // Pin 38    BH RC INPUT
#define RPI_GPIO_21   21   // Pin 40    BH RC INPUT
#define RPI_GPIO_26   26   // Pin 37    BH RC INPUT

class Pin {
public:
    typedef enum {
        GpioModeInput,
        GpioModeOutput
    } GpioMode;

    Pin(uint8_t pin);
    ~Pin();

    bool    init();
    void    setMode(GpioMode mode);
    uint8_t read() const;
    void    write(uint8_t value);
    void    toggle();

private:
    int getRaspberryPiVersion() const;
    Pin (const Pin&);
    Pin& operator=(const Pin&);

    uint8_t _pin;
    volatile uint32_t *_gpio;
    GpioMode _mode;

    bool    _deinit();
};

} // end of BH

#endif // GPIO_H_
