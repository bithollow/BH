#include <stdio.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include "i2c.h"
#include "cat24c256.h"

// 32768 bytes >> 0x8000 >> 15 bit address

using namespace BH;

static struct option EEPROMOptions[] =
{
    {"help"        , 0, 0, 'h'},
    {"version"     , 0, 0, 'v'},
    {"test"        , 0, 0, 't'},
    {0, 0, 0, 0},
};

/**
 * @brief   initialize CAT24C256
 * @return  true, if succeed, otherwise false.
*/
bool initEEPROM(CAT24C256& eeprom)
{
    return true;
}

/**
 * @brief  the usage of the rgbled application
 */
void usage(void)
{
    fprintf(stdout, "eeprom Application:\n");
    fprintf(stdout, "[Usage]: eeprom [function]\n");
    fprintf(stdout,
        "    -h | --help      :Output this application`s usage\n"
        "    -v | --version   :Output this application`s version\n"
        "    -t | --test      :Test eeprom read/write data\n"
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
void testEEPROM(CAT24C256& eeprom)
{
    uint16_t reg_address = 0x0;
    uint8_t write_data[3] = {0xAA, 0xBB, 0xCC};
   
    printf("Writing data...\n");
    eeprom.writeBytes(reg_address, 3, write_data);

    sleep(2);
    uint8_t read_data[10] = {0};
    printf("Reading data...\n");
    eeprom.readBytes(reg_address, 10, read_data);
    printf("Read Data, read from address 0,1,2 is %x,%x,%x\n", read_data[0], read_data[1], read_data[2]);
}

int main(int argc, char**argv)
{
    if (argc < 2) {
        fprintf(stdout, "Type './eeprom -h/--help' for usage\n");
        return 0;   
    }

    CAT24C256 eeprom;

    if (!initEEPROM(eeprom)) {
        fprintf(stdout, "init failed!\n");
        return 0;
    }

    sleep(1);
    while (true)
    {
        int opt = getopt_long(argc, argv, "hvt", EEPROMOptions, NULL);
        if (-1 == opt) {
//            fprintf(stdout, "Type './eeprom -h/--help' for usage\n");
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
	        testEEPROM(eeprom);
	        break;
	    default:
		break;
        }
    }
    return 0;	
}
