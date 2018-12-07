/*

MPU9250 Example: Read accelerometer, gyroscope and magnetometer values from
MPU9250 through /dev/spidev0.1

To run this example navigate to the directory containing it and run following commands:
make
sudo ./imu
*/

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "mpu9250.h"

using namespace BH;

#define BUFFER_LENGTH 125  //Max length of buffer

static struct option IMUOptions[] =
{
    {"help"        , 0, 0, 'h'},
    {"version"     , 0, 0, 'v'},
    {"get"         , 0, 0, 'g'},
    {0, 0, 0, 0},
};

/**
 * @brief   initialize mpu9250
 * @return  true, if succeed, otherwise false.
*/
bool initIMU(MPU9250& imu)
{
    imu.init();
    return true;
}

/**
 * @brief  the usage of the imu application
 */
void usage(void)
{
    fprintf(stdout, "imu Application:\n");
    fprintf(stdout, "[Usage]: imu [function]\n");
    fprintf(stdout,
        "    -h | --help      :Output this application`s usage\n"
        "    -v | --version   :Output this application`s version\n"
        "    -g | --get       :Get the imu raw data\n"
        "\n"
    );
    return;
}

/**
 * @brief  get version of the imu application
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
void getIMUData(MPU9250& imu)
{
    float ax, ay, az, gx, gy, gz, mx, my, mz;
    char buffer[BUFFER_LENGTH] = {0};
    while(1) {
        imu.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
        sprintf(buffer, "%f %f %f %f %f %f %f %f %f\n", 
		                 ax,ay,az,gx,gy,gz,mx,my,mz);
        printf("buffer = %s\n", buffer);
        sleep(1);
    }
}

int main(int argc, char**argv)
{
    MPU9250 imu;
    initIMU(imu);

    while (true)
    {
        int opt = getopt_long(argc, argv, "hvg", IMUOptions, NULL);
        if (-1 == opt) {
            fprintf(stdout, "Type './imu -h/--help' for usage\n");
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
            getIMUData(imu);
            break;
        default:
            break;
        }
    }
    return 0;
}
