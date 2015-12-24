/*
Example: Get pressure from MS5611 barometer

To run this example navigate to the directory containing it and run following commands:
make
sudo ./baro
*/

#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <stdint.h>
#include "ms5611.h"

using namespace BH;

static struct option BAROOptions[] =
{
    {"help"        , 0, 0, 'h'},
    {"version"     , 0, 0, 'v'},
    {"get"         , 0, 0, 'g'},
    {0, 0, 0, 0},
};

/**
 * @brief   initialize MS5611
 * @return  true, if succeed, otherwise false.
*/
bool initBarometer(MS5611& baro)
{
    baro.init();
    return true;
}

/**
 * @brief  the usage of the imu application
 */
void usage(void)
{
    fprintf(stdout, "baro Application:\n");
    fprintf(stdout, "[Usage]: baro [function]\n");
    fprintf(stdout,
        "    -h | --help      :Output this application`s usage\n"
        "    -v | --version   :Output this application`s version\n"
        "    -g | --get       :Get the baro data\n"
        "\n"
    );
    return;
}

/**
 * @brief  get version of the baro application
 */
void getVersion(void)
{
    fprintf(stdout,
       "  \n"
       "  imu version 0.2\n"
       "  Copyright MIT license\n"
       "  This program comes with ABSOLUTELY NO WARRANTY.\n");
    return;
}

/**
 * @brief  get mpu9250 raw data and print them.
 */
void getBarometerData(MS5611& baro)
{
    while (true) {
        baro.refresh();
        usleep(10000); // Waiting for pressure data ready

        printf("Temperature(C): %f Pressure(millibar): %f\n", 
                baro.getTemperature(), baro.getPressure());
        sleep(1);
    }
}

int main(int argc, char**argv)
{
    MS5611 baro;
    initBarometer(baro);

    while (true)
    {
        int opt = getopt_long(argc, argv, "hvg", BAROOptions, NULL);
        if (-1 == opt) {
            fprintf(stdout, "Type './baro -h/--help' for usage\n");
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
        case 'g':
        {
            getBarometerData(baro);
            break;
        }
        default:
            break;
        }
    }
    return 0;
}
