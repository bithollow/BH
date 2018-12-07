/*
GPS demo
*/

#include <stdio.h>
#include <getopt.h>
#include <stdint.h>
#include <unistd.h>
#include "ublox.h"

using namespace std;
using namespace BH;

static struct option GPSOptions[] =
{
    {"help"        , 0, 0, 'h'},
    {"version"     , 0, 0, 'v'},
    {"test"        , 0, 0, 't'},
    {0, 0, 0, 0},
};

/**
 * @brief   initialize GPS
 * @return  true, if succeed, otherwise false.
*/
bool initGPS(Ublox& gps)
{
    gps.init();
    return true;
}

/**
 * @brief  the usage of the gps application
 */
void usage(void)
{
    fprintf(stdout, "gps Application:\n");
    fprintf(stdout, "[Usage]: gps [function]\n");
    fprintf(stdout,
        "    -h | --help      :Output this application`s usage\n"
        "    -v | --version   :Output this application`s version\n"
        "    -t | --test      :Test GPS\n"
        "\n"
    );
    return;
}

/**
 * @brief  get version of the gps application
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
 * @brief  test GPS.
 */
int testGPS(Ublox& gps)
{
    std::vector<double> pos_data;

    // Here we test connection with the receiver. Function testConnection() waits for a ubx protocol message and checks it.
    // If there's at least one correct message in the first 300 symbols the test is passed
    // gps.decodeMessages();
    // You can use this function to decode all messages, incoming from the GPS receiver. The function starts an infinite loop.
    // In this example we can only decode NAV-POSLLH messages, the others are simply ignored.
    // You can add new message types in function decodeMessage() of class UBXParser(see ublox.h)
    // Here, however we use a different approach. Instead of trying to extract info from every message(as done in decodeMessages()),
    // this function waits for a message of a specified type and gets you just the information you need
    // In this example we decode NAV-POSLLH messages, adding new types, however is quite easy

    while (true)
    {
        if (gps.decodeSingleMessage(Ublox::NAV_POSLLH, pos_data) == 1) {
            // after desired message is successfully decoded, we can use the information stored in pos_data vector
            // right here, or we can do something with it from inside decodeSingleMessage() function(see ublox.h).
            // the way, data is stored in pos_data vector is specified in decodeMessage() function of class UBXParser(see ublox.h)
            printf("Current location data:\n");
            printf("iTOW: %lf\n", pos_data[0]/1000);
            printf("Latitude: %lf\n", pos_data[2]/10000000);
            printf("Longitude: %lf\n", pos_data[1]/10000000);
            printf("Height: %lf\n\n", pos_data[3]/1000);
        } else {
            // printf("Message not captured\n");
            // use this to see, how often you get the right messages
            // to increase the frequency you can turn off the undesired messages or tweak ublox settings
            // to increase internal receiver frequency
        }

        if (gps.decodeSingleMessage(Ublox::NAV_STATUS, pos_data) == 1) {
            printf("Current GPS status:\n");
            printf("gpsFixOk: %d\n", ((int)pos_data[1] & 0x01));
            printf("gps Fix status: ");
            switch((int)pos_data[0])
            {
                case 0x00:
                    printf("no fix\n");
                    break;
                case 0x01:
                    printf("dead reckoning only\n");
                    break;
                case 0x02:
                    printf("2D-fix\n");
                    break;
                case 0x03:
                    printf("3D-fix\n");
                    break;
                case 0x04:
                    printf("GPS + dead reckoning combined\n");
                    break;
                case 0x05:
                    printf("Time only fix\n");
                    break;
                default:
                    printf("Reserved value. Current state unknown\n");
                    break;
            }
        } else {
            printf("Status Message not captured\n");
        }
        usleep(200);
    }

    return 0;
}

int main(int argc, char**argv)
{    
    if (argc < 2) {
        fprintf(stdout, "Type './gps -h/--help' for usage\n");
        return 0;   
    }               

    Ublox gps;                            
    if (!initGPS(gps)) {
        fprintf(stdout, "init failed!\n");
        return 0;
    }

    sleep(1);
    while (true)
    {
        int opt = getopt_long(argc, argv, "hvt", GPSOptions, NULL);
        if (-1 == opt) {
            fprintf(stdout, "Type './gps -h/--help' for usage\n");
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
            testGPS(gps);
            break;
        default:
            break;
        }
    }
    return 0;
}

