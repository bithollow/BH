/*
To run this example navigate to the directory containing it and run following commands:
make
sudo ./rtc
*/

#include <stdio.h>
#include <getopt.h>
#include <stdint.h>
#include <unistd.h>
#include "ds1339.h"

using namespace BH;

static struct option RTCOptions[] =
{
    {"help"        , 0, 0, 'h'},
    {"version"     , 0, 0, 'v'},
    {"test"        , 0, 0, 't'},
    {0, 0, 0, 0},
};

/**
 * @brief   initialize RTC
 * @return  true, if succeed, otherwise false.
*/
bool initRTC(DS1339& rtc)
{
    return true;
}

/**
 * @brief  the usage of the rgbled application
 */
void usage(void)
{
    fprintf(stdout, "rtc Application:\n");
    fprintf(stdout, "[Usage]: rtc [function]\n");
    fprintf(stdout,
        "    -h | --help      :Output this application`s usage\n"
        "    -v | --version   :Output this application`s version\n"
        "    -t | --test      :Test RTC\n"
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
 * @brief  test DS1339.
 */
void testRTC(DS1339& rtc)
{
    RtcTime time_1339;
    rtc.getTime(time_1339);
    rtc.printfTime(time_1339);  

    time_1339.year += 1;
    rtc.setTime(time_1339, true, true);
    rtc.getTime(time_1339);
    rtc.printfTime(time_1339);
}

int main(int argc, char**argv)
{
    if (argc < 2) {
        fprintf(stdout, "Type './rtc -h/--help' for usage\n");
        return 0;   
    }

    DS1339 ds1339;
    if (!initRTC(ds1339)) {
        fprintf(stdout, "init failed!\n");
        return 0;
    }

    sleep(1);
    while (true)
    {
        int opt = getopt_long(argc, argv, "hvt", RTCOptions, NULL);
        if (-1 == opt) {
            fprintf(stdout, "Type './rtc -h/--help' for usage\n");
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
        case 't':
            testRTC(ds1339);
            break;
        default:
            break;
        }
    }
    return 0;	
}
