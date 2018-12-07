/*
If you want to visualize IMU data on another machine pass it's address and port
make
sudo ./ahrs ipaddress portnumber
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include "mpu9250.h"
#include "ahrs.h"

using namespace BH;

#define BUFFER_SIZE 300
#define DEFAULT_SERVER_PORT 16666

static struct option AHRSOptions[] =
{
    {"help"        , 0, 0, 'h'},
    {"version"     , 0, 0, 'v'},
    {"loopIMU"     , 1, 0, 'l'},
    {0, 0, 0, 0},
};

/**
 * @brief   initialize AHRS
 * @return  true, if succeed, otherwise false.
*/
bool initAHRS(MPU9250& imu, AHRS& ahrs)
{
    imu.init();

    float ax, ay, az;
    float gx, gy, gz;
    float offset[3];

    for(int i = 0; i<100; i++)
    {
        imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        offset[0] += (-gx*0.0175);
        offset[1] += (-gy*0.0175);
        offset[2] += (-gz*0.0175);
        usleep(10000);
    }
    
    offset[0]/=100.0;
    offset[1]/=100.0;
    offset[2]/=100.0;

    // printf("Offsets are: %f %f %f\n", offset[0], offset[1], offset[2]);
    ahrs.setGyroOffset(offset[0], offset[1], offset[2]);
	
    return true;
}

/**
 * @brief  the usage of the ahrs application
 */
void usage(void)
{
    fprintf(stdout, "ahrs Application:\n");
    fprintf(stdout, "[Usage]: ahrs [function]\n");
    fprintf(stdout,
        "    -h | --help             :Output this application`s usage\n"
        "    -v | --version          :Output this application`s version\n"
    	"    -l | --loopimu ip port  :Loop AHRS\n"
	    "\n"
    );
    return;
}

/**
 * @brief  get version of the ahrs application
 */
void getVersion(void)
{
    fprintf(stdout,
       "  \n"
       "  ahrs version 0.2\n"
       "  Copyright MIT license\n"
       "  This program comes with ABSOLUTELY NO WARRANTY.\n");
    return;
}

/**
 * @brief  loop IMU data
 */
void loopIMU(MPU9250& imu, AHRS& ahrs, char* server_ip, uint32_t server_port)
{
    float ax, ay, az;
    float gx, gy, gz;
    float mx, my, mz;
    float roll, pitch, yaw;

    printf("server_ip=%s, server_port=%d\n", server_ip, server_port);
    // create socket(UDP)connect
    int sockfd;
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    struct sockaddr_in servaddr = {0};
    memset((char*)&servaddr, 0 ,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(server_port);
    inet_aton(server_ip, &servaddr.sin_addr);

    // Timing data
    struct timeval tv;
    unsigned long previoustime, currenttime;
    float sample_period = 0;     // ms UNIT
    float max_sample_period = sample_period;
    float min_sample_period = 0.01;
    float sample_period_sum = 0;
    int32_t isFirst = 1;
    char send_buffer[BUFFER_SIZE];	

    while(1) {
        gettimeofday(&tv, NULL);
        previoustime = currenttime;
        currenttime = 1000000 * tv.tv_sec + tv.tv_usec;
        sample_period = (currenttime - previoustime) / 1000000.0;
	
        if(sample_period < 1/1300.0) {
            usleep((1/1300.0 - sample_period)*1000000);
        }

        gettimeofday(&tv,NULL);
        currenttime = 1000000 * tv.tv_sec + tv.tv_usec;
        sample_period = (currenttime - previoustime) / 1000000.0;

        // Accel + gyro + mag. 
        // Soft and hard iron calibration required for proper function.
        imu.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
        ahrs.update((1.0f / sample_period), ax, ay, az, gx*0.0175, gy*0.0175, gz*0.0175, my, mx, -mz);
        ahrs.getEuler(&roll, &pitch, &yaw);
//        printf("roll=%f, pitch=%f, yaw=%f\n", roll, pitch, yaw);

        if (!isFirst) {
            if (sample_period > max_sample_period) {
                max_sample_period = sample_period;
            }
            if (sample_period < min_sample_period) {
                min_sample_period = sample_period;
            }
        }
        
        isFirst = 0;
        sample_period_sum += sample_period;
        if(sample_period_sum > 0.05) {
            // Network output
            sprintf(send_buffer, "%f %f %f %f %f %f %f %f %f %f %f %f\n",
                                  ax,ay,az,gx,gy,gz,mx,my,mz,roll,pitch,yaw); 
            printf("send data = %s\n", send_buffer);
            sendto(sockfd, send_buffer, sizeof(send_buffer), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
            sample_period_sum = 0;
	}
    }
}

int main(int argc, char**argv)
{
    if (argc < 2) {
        fprintf(stdout, "Type './ahrs -h/--help' for usage\n");
    }

    MPU9250 imu;
    AHRS ahrs;
    char server_ip[17] = {0};
    memcpy(server_ip, "127.0.0.1", strlen("127.0.0.1"));
    uint32_t server_port = DEFAULT_SERVER_PORT;

    if (!initAHRS(imu, ahrs)) {
        fprintf(stdout, "init failed!\n");
        return 0;
    }
    sleep(1);

    while (true)
    {
        int opt = getopt_long(argc, argv, "hvl:", AHRSOptions, NULL);
        if (-1 == opt) {
            fprintf(stdout, "Type './ahrs -h/--help' for usage\n");
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
        case 'l':
 	{
	    printf("argc = %d, argv[2] = %s, argv[3]=%s\n", argc, argv[2], argv[3]);
            if (argc == 4)  {
                memset(server_ip, 0, sizeof(server_ip));
		memcpy(server_ip, argv[2], sizeof(server_ip));
		server_port = atoi(argv[3]);
	    }
            printf("server_ip = %s server_port=%d\n", server_ip, server_port);
            loopIMU(imu, ahrs, server_ip, server_port);
            break;
        }
        default:
            break;
        }
    }
    return 0;		
}
