/*

MONOLED is connected to 17(Red), 18(Green), 22(Blue) GPIO of raspberry pi3

To run this example navigate to the directory containing it and run following commands:
make
sudo ./monoled
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>

#include "gpio.h"

using namespace BH;

static const uint8_t PIN_RED   = RPI_GPIO_17;
static const uint8_t PIN_GREEN = RPI_GPIO_18;
static const uint8_t PIN_BLUE  = RPI_GPIO_22;

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
 * @brief   initialize MONOLED
 * @return  true, if succeed, otherwise false.
*/
bool initLED(Pin& pin)
{
    if (!pin.init()) {
        printf("Led pin Enable not set. Are you root\n");
        return false;
    }

    pin.setMode(Pin::GpioModeOutput);
    pin.write(0);

    return true;
}

/**
 * @brief  the usage of the monoled application
 */
void usage(void)
{
    fprintf(stdout, "monoled Application:\n");
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
 * @brief  get version of the monoled application
 */
void getVersion(void)
{
    fprintf(stdout,
       "  \n"
       "  monoled version 0.1\n"
       "  Copyright MIT license\n"
       "  This program comes with ABSOLUTELY NO WARRANTY.\n");
    return;
}

/**
 * @brief lighten the LED
 */
bool lightenLed(Pin& pin, bool flag)
{
    if(flag == true) {
        pin.write(1);
    }
    else {
       pin.write(0);
    }
    return true;
}

/**
 * @brief lighten the RED LED
 */
bool lightenREDLed(Pin& red, Pin& green, Pin& blue)
{
    lightenLed(green, false);
    lightenLed(blue, false);
    lightenLed(red, true);
    return true;
}

/**
 * @brief lighten the GREEN LED
 */
bool lightenGREENLed(Pin& red, Pin& green, Pin& blue)
{
    lightenLed(red, false);
    lightenLed(blue, false);
    lightenLed(green, true);
    return true;
}

/**
 * @brief lighten the BLUE LED
 */
bool lightenBLUELed(Pin& red, Pin& green, Pin& blue)
{
    lightenLed(red, false);
    lightenLed(green, false);
    lightenLed(blue, true);
    return true;
}

/**
 * @brief unlighten ALL LEDs
 */
bool unlightenLeds(Pin& red, Pin& green, Pin& blue)
{
    lightenLed(red, false);
    lightenLed(green, false);
    lightenLed(blue, false);
    return true;
}

/**
 * @brief the usage of the monoled application
 */
bool loopRGB(Pin& red, Pin& green, Pin& blue)
{
    unlightenLeds(red, green, blue);
    while (true) {
        lightenREDLed(red, green, blue);
    sleep(1);
    lightenGREENLed(red, green, blue);
    sleep(1);
        lightenBLUELed(red, green, blue);
        sleep(1);
    }

    return true;
}

int main(int argc, char**argv)
{
    if (argc < 2) {
        fprintf(stdout, "Type './monoled -h/--help' for usage\n");
        return 0;
    }

    Pin red(PIN_RED);
    Pin green(PIN_GREEN);
    Pin blue(PIN_BLUE);

    if (!initLED(red)) {
        fprintf(stdout, "init RED led failed!\n");
        return 0;
    }
    if (!initLED(green)) {
        fprintf(stdout, "init GREEN led failed!\n");
        return 0;
    }
    if (!initLED(blue)) {
        fprintf(stdout, "init BLUE led failed!\n");
        return 0;
    }

    sleep(1);
    bool ret = true;
    while (true)
    {
        int opt = getopt_long(argc, argv, "hvrgbl", RGBOptions, NULL);
        if (-1 == opt) {
            fprintf(stdout, "Type './monoled -h/--help' for usage\n");
            break;
        }

        switch(opt) {
        case 'h':
            usage(); 
            break;
        case 'v':
            getVersion();
            break;
        case 'r':
            ret = lightenREDLed(red, green, blue);
            break;
        case 'g':
            ret = lightenGREENLed(red, green, blue);
            break;
        case 'b':
            ret = lightenBLUELed(red, green, blue);
            break;
        case 'l':
            ret = loopRGB(red, green, blue);
            break;
        default:
            break;
        }
    }

    return (ret ? 0 : 1);
}
