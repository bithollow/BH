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
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "ms5611.h"

using namespace BH;
#define OUTPUT_2_TTY (0)

#if (OUTPUT_2_TTY == 1)
namespace{
const char *port_name = "/dev/ttyAMA0";
int port_fd = -1;

int set_interface_attribs (int fd, int speed, int parity)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0)
	{
		fprintf(stdout, "error %d from tcgetattr", errno);
		return -1;
	}

	cfsetospeed (&tty, speed);
	cfsetispeed (&tty, speed);

	tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
	// disable IGNBRK for mismatched speed tests; otherwise receive break
	// as \000 chars
	tty.c_iflag &= ~IGNBRK;         // disable break processing
	tty.c_lflag = 0;                // no signaling chars, no echo,
									// no canonical processing
	tty.c_oflag = 0;                // no remapping, no delays
	tty.c_cc[VMIN]  = 0;            // read doesn't block
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

	tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
									// enable reading
	tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
	tty.c_cflag |= parity;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CRTSCTS;

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
	{
		fprintf(stdout, "error %d from tcsetattr", errno);
		return -1;
	}
	return 0;
}

int set_blocking (int fd, int should_block)
{
	struct termios tty;
	memset (&tty, 0, sizeof tty);
	if (tcgetattr (fd, &tty) != 0) {
		fprintf(stdout, "error %d from tggetattr", errno);
		return -1;
	}

	tty.c_cc[VMIN]  = should_block ? 1 : 0;
	tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

	if (tcsetattr (fd, TCSANOW, &tty) != 0)
        fprintf(stdout, "error %d setting term attributes", errno);
	return 0;
}


int openSerialPort()
{
	port_fd = open (port_name, O_RDWR | O_NOCTTY | O_SYNC);
	if (port_fd < 0)
	{
		fprintf(stdout, "error %d opening %s: %s", errno, port_name, strerror (errno));
		return -1;
	}
	if (set_interface_attribs (port_fd, B115200, 0) < 0) // set speed to 115,200 bps, 8n1 (no parity)
	{
		return -1;
	}
	if (set_blocking (port_fd, 0) < 0)					// set no blocking
	{
		return -1;
	}
	return 0;
}

void writePort(float temp, float pressure)
{
	char msg[256] ={0};
	sprintf(msg, "%f,%f\n", temp, pressure);
	int size = strlen(msg);
	write (port_fd, &msg, size);
}

} // end of anonymous namespace
#endif //end of OUTPUT_2_TTY == 1

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
#if (OUTPUT_2_TTY == 1)
		writePort(baro.getTemperature(), baro.getPressure());
#endif
        sleep(1);
    }
}

int main(int argc, char**argv)
{
#if (OUTPUT_2_TTY == 1)
    int ret = openSerialPort();
	if (ret < 0)
	{
		fprintf(stdout, "Open serial port failed!\n");
	}
#endif

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
