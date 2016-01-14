/*
GPS driver
*/

#ifndef UBLOX_H_
#define UBLOX_H_

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
#include <iostream>
#include <string>
#include <vector>
#include "spi.h"

namespace BH {

static const int32_t buffer_length = 1024;

class UBXScanner {
public:
    enum State
    {
        Sync1,
        Sync2,
        Class,
        ID,
        Length1,
        Length2,
        Payload,
        CK_A,
        CK_B,
        Done
    };

    uint8_t message[buffer_length];   // Buffer for UBX message
    uint32_t message_length;    // Length of the received message

private:
    uint32_t position;          // Indicates current buffer offset
    uint32_t payload_length;  // Length of current message payload
    State state;                    // Current scanner state

public:
    UBXScanner();
    uint8_t* getMessage();
    uint32_t getMessageLength();
    uint32_t getPosition();
    void reset();
    int32_t update(uint8_t data);
};

class UBXParser{
private:
    UBXScanner* scanner; // pointer to the scanner, which finds the messages in the data stream
    uint8_t* message; // pointer to the scanner's message buffer
    uint32_t length; // current message length
    uint32_t position; // current message end position

public:
    UBXParser(UBXScanner* ubxsc);
    void updateMessageData();
    int32_t decodeMessage(std::vector<double>& data);
    int32_t checkMessage();
};

class Ublox
{
public:
enum message_t
{
    NAV_POSLLH = 0x0102,
    NAV_STATUS = 0x0103
};

private:
    std::string spi_device_name;
    UBXScanner* scanner;
    UBXParser* parser;

public:
    Ublox(std::string name = "/dev/spidev0.0");
    Ublox(std::string name, UBXScanner* scan, UBXParser* pars);
    int32_t enableNAV_POSLLH();
    int32_t enableNAV_STATUS();
    int32_t testConnection();
    int32_t decodeMessages();
    int32_t decodeSingleMessage(message_t msg, std::vector<double>& position_data);
}; // end of ublox class def

} // end of BH

#endif // UBLOX_H
