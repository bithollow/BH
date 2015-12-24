/*
To run this example navigate to the directory containing it and run following commands:
make
sudo ./adc
*/

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>
#include "ads1115.h"

using namespace BH;

#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

static struct option ADCOptions[] =
{
    {"help"        , 0, 0, 'h'},
    {"version"     , 0, 0, 'v'},
    {"get"         , 0, 0, 'g'},
    {0, 0, 0, 0},
};

/**
 * @brief   initialize ADS1115
 * @return  true, if succeed, otherwise false.
*/
bool initADS1115(ADS1115& adc)
{
    adc.init();
    adc.setMode(ADS1115_MODE_SINGLESHOT);
    adc.setRate(ADS1115_RATE_860); 

    return true;
}

/**
 * @brief  the usage of the rgbled application
 */
void usage(void)
{
    fprintf(stdout, "adc Application:\n");
    fprintf(stdout, "[Usage]: adc [function]\n");
    fprintf(stdout,
        "    -h | --help      :Output this application`s usage\n"
        "    -v | --version   :Output this application`s version\n"
        "    -g | --get       :Get ADC data\n"
    	"\n"
    );
    return;
}

/**
 * @brief  get version of the adc application
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
 * @brief  get ADS1115 data and print them.
 */
void getADCData(ADS1115& adc)
{
    uint16_t muxes[] = {ADS1115_MUX_P0_NG, 
                        ADS1115_MUX_P1_NG,
                        ADS1115_MUX_P2_NG,
                        ADS1115_MUX_P3_NG};
    float results[ARRAY_SIZE(muxes)] = {0.0f};
    int i = 0;

    while (true) {
        adc.setMultiplexer(muxes[i]);

        float conversion = adc.getMilliVolts();
        results[i] = conversion;

        i = (i + 1) % ARRAY_SIZE(muxes);

        for (int j = 0; j < ARRAY_SIZE(muxes); j++) {
            printf("A%d: %.4fV ", j, results[j] / 1000);
        }
        printf("\n");
    }
}

int main(int argc, char**argv)
{
    if (argc < 2) {
        fprintf(stdout, "Type './adc -h/--help' for usage\n");
        return 0;   
    }

    ADS1115 adc;
    if (!initADS1115(adc)) {
        fprintf(stdout, "init failed!\n");
        return 0;
    }

    sleep(1);
    while (true)
    {
        int opt = getopt_long(argc, argv, "hvg", ADCOptions, NULL);
        if (-1 == opt) {
            fprintf(stdout, "Type './adc -h/--help' for usage\n");
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
            getADCData(adc);
            break;
        default:
            break;
        }
    }
    return 0;	
}
