/*
CAT24C256 driver code.
*/

#ifndef CAT24C256_H_
#define CAT24C256_H_

namespace BH {

#define CAT24C256_DEFAULT_ADDRESS   0x50

class CAT24C256
{
public:
    CAT24C256(uint8_t device_address = CAT24C256_DEFAULT_ADDRESS);
    ~CAT24C256();

    /** 
     * @brief read byte from CAT24C256.
     * @param read address
     * @param readed data
     * @return read count
    */
    uint8_t readByte(uint16_t read_address, uint8_t* data);

    /** 
     * @brief read bytes from CAT24C256.
     * @param read address
     * @param read buffer length
     * @param read buffer
     * @return read count
    */
    uint8_t readBytes(uint16_t read_address, uint8_t length, uint8_t* data); 

    /** 
     * @brief write byte to CAT24C256.
     * @param write address
     * @param writed data
     * @return write count
    */
    uint8_t writeByte(uint16_t write_address, uint8_t data);

    /** 
     * @brief write byte to CAT24C256.
     * @param write address
     * @param write length
     * @param write buffer
     * @return write count
    */
    uint8_t writeBytes(uint16_t write_address, uint8_t length, uint8_t* data);

private:
    uint8_t dev_address;
};

} // end of BH

#endif // CAT24C256_H_
