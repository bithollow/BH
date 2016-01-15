/*

RGB LED is connected to 13,14,15 channels of PWM controller PCA9685.

To run this example navigate to the directory containing it and run following commands:
make
sudo ./rgbled
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include "gpio.h"
#include "pca9685.h"

using namespace BH;

static const uint8_t outputEnablePin = RPI_GPIO_4;
static const uint8_t RED_LED_CHANNEL = 13;
static const uint8_t GREEN_LED_CHANNEL = 14;
static const uint8_t BLUE_LED_CHANNEL = 15;

static struct option RGBOptions[] =
{
    {"help"        , 0, 0, 'h'},
    {"version"     , 0, 0, 'v'},
    {"red"         , 0, 0, 'r'},
    {"green"       , 0, 0, 'g'},
    {"blue"        , 0, 0, 'b'},
    {"looprgb"     , 0, 0, 'l'},
    {0, 0, 0, 0},
};

/**
 * @brief   initialize RGBLED
 * @return  true, if succeed, otherwise false.
*/
bool initRGBLED(Pin& pin, PCA9685& led)
{
    if (!pin.init()) {
        printf("Output Enable not set. Are you root\n");
        return false;
    }

    pin.setMode(Pin::GpioModeOutput);
    pin.write(0); /* drive Output Enable low */

    if (!led.init()) {
        return false;
    }

    return true;
}

/**
 * @brief  the usage of the rgbled application
 */
void usage(void)
{
    fprintf(stdout, "rgbled Application:\n");
    fprintf(stdout, "[Usage]: rgbleg [function]\n");
    fprintf(stdout,
        "    -h | --help      :Output this application`s usage\n"
        "    -v | --version   :Output this application`s version\n"
        "    -r | --red       :Lighten Red led\n"
        "    -g | --green     :Lighten Green led\n"
	"    -b | --blue      :Lighten Blue led\n"
	"    -l | --looprgb   :Loop lighten RGB led\n"
	"\n"
    );
    return;
}

/**
 * @brief  get version of the rgbled application
 */
void getVersion(void)
{
    fprintf(stdout,
       "  \n"
       "  regled version 0.2\n"
       "  Copyright MIT license\n"
       "  This program comes with ABSOLUTELY NO WARRANTY.\n");
    return;
}

/**
 * @brief the lighten LED
 */
bool lightenRed(PCA9685& led)
{
    uint16_t R = 2000, G = 0, B = 0;
    led.setPWM(RED_LED_CHANNEL, 0, R);
    led.setPWM(GREEN_LED_CHANNEL, 0, G);
    led.setPWM(BLUE_LED_CHANNEL, 0, B);

    sleep(3);
    return true;
}

/**
 * @brief the usage of the rgbled application
 */
bool lightenGreen(PCA9685& led)
{
    uint16_t R = 0, G = 2000, B = 0;
    led.setPWM(RED_LED_CHANNEL, 0, R);
    led.setPWM(GREEN_LED_CHANNEL, 0, G);
    led.setPWM(BLUE_LED_CHANNEL, 0, B);

    sleep(3);
    return true;
}

/**
 * @brief the usage of the rgbled application
 */
bool lightenBlue(PCA9685& led)
{
    uint16_t R = 0, G = 0, B = 2000;
    led.setPWM(RED_LED_CHANNEL, 0, R);
    led.setPWM(GREEN_LED_CHANNEL, 0, G);
    led.setPWM(BLUE_LED_CHANNEL, 0, B);

    sleep(3);
    return true;
}

/**
 * @brief the usage of the rgbled application
 */
bool loopRGB(PCA9685& led)
{
    uint16_t R = 2000, G = 0, B = 0;
	while (true) {
        for (R = 0; R < 4095; R++)
            led.setPWM(RED_LED_CHANNEL, 0, R);
        for (B = 4095; B > 0; B--)
            led.setPWM(BLUE_LED_CHANNEL, 0, B);
        for (G = 0; G < 4095; G++)
            led.setPWM(GREEN_LED_CHANNEL, 0, G);

        for (R = 4095; R > 0; R--)
            led.setPWM(RED_LED_CHANNEL, 0, R);
        for (B = 0; B < 4095; B++)
            led.setPWM(BLUE_LED_CHANNEL, 0, B);
        for (G = 4095; G > 0; G--)
            led.setPWM(GREEN_LED_CHANNEL, 0, G);
    }

    return true;
}

int main(int argc, char**argv)
{
    if (argc < 2) {
        fprintf(stdout, "Type './rgbled -h/--help' for usage\n");
        return 0;   
    }

    Pin pin(outputEnablePin);
    PCA9685 led;
	
    if (!initRGBLED(pin, led)) {
        fprintf(stdout, "init failed!\n");
        return 0;
    }

    sleep(1);
    bool ret = true;
    while (true)
    {
        int opt = getopt_long(argc, argv, "hvrgbl", RGBOptions, NULL);
        if (-1 == opt) {
            fprintf(stdout, "Type './rgbled -h/--help' for usage\n");
            break;
        }

        switch(opt)
        {
        case 'h':
            usage(); 
            break;
        case 'v':
            getVersion();
		break;
	    case 'r':
	        ret = lightenRed(led);
		break;
	    case 'g':
	        ret = lightenGreen(led);
	        break;
            case 'b':
	        ret = lightenBlue(led);
		break;
	    case 'l':
	        ret = loopRGB(led);
                break;
	    default:
		break;
        }
    }

    return 0;		
}
