/*
SPI driver
*/

#ifndef SPI_H_
#define SPI_H_

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace BH {

class SPI
{
public:
    SPI() { }
    ~SPI() { }
   
    static int transfer(const char *spidev,
                        unsigned char *tx,
                        unsigned char *rx,
                        unsigned int length,
                        unsigned int speed_hz = 1000000,
                        unsigned char bits_per_word = 8,
                        unsigned short delay_usecs = 0)
    {
        spi_ioc_transfer spi_transfer;
        memset(&spi_transfer, 0, sizeof(spi_ioc_transfer));

        spi_transfer.tx_buf = (unsigned long)tx;
        spi_transfer.rx_buf = (unsigned long)rx;
        spi_transfer.len = length;
        spi_transfer.speed_hz = speed_hz;
        spi_transfer.bits_per_word = bits_per_word;
        spi_transfer.delay_usecs = delay_usecs;

        int spi_fd = ::open(spidev, O_RDWR);
        if (spi_fd < 0 ) {
            printf("Error: Can not open SPI device\n");
            return -1;
        }

        int status = ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi_transfer);
        ::close(spi_fd);

        // Debug information
	/*
        printf("Tx: ");
        for (int i = 0; i < length; i++)
            printf("%x ", tx[i]);
        printf("\n");
        printf("Rx: ");
        for (int i = 0; i < length; i++)
            printf("%x ", rx[i]);
        printf("\n");
        */

        return status;
    }
};

} // end of BH

#endif //SPI_H_
