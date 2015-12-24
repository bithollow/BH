/*
MPU9250 Driver
*/

#include "mpu9250.h"

using namespace BH;

MPU9250::MPU9250()
{
}

bool MPU9250::init(int sample_rate_div, int low_pass_filter)
{
    uint8_t i = 0;
    uint32_t init_reg_num = 16;
    uint8_t MPU_Init_Data[init_reg_num][2] = {
        //{0x80, MPUREG_PWR_MGMT_1},     // Reset Device - Disabled because it seems to corrupt initialisation of AK8963
        {0x01, MPUREG_PWR_MGMT_1},     // Clock Source
        {0x00, MPUREG_PWR_MGMT_2},     // Enable Acc & Gyro
        {low_pass_filter, MPUREG_CONFIG},         // Use DLPF set Gyroscope bandwidth 184Hz, temperature bandwidth 188Hz
        {0x18, MPUREG_GYRO_CONFIG},    // +-2000dps
        {0x08, MPUREG_ACCEL_CONFIG},   // +-4G
        {0x09, MPUREG_ACCEL_CONFIG_2}, // Set Acc Data Rates, Enable Acc LPF , Bandwidth 184Hz
        {0x30, MPUREG_INT_PIN_CFG},    //
      //{0x40, MPUREG_I2C_MST_CTRL},   // I2C Speed 348 kHz
      //{0x20, MPUREG_USER_CTRL},      // Enable AUX
        {0x20, MPUREG_USER_CTRL},       // I2C Master mode
        {0x0D, MPUREG_I2C_MST_CTRL}, //  I2C configuration multi-master  IIC 400KHz

        {AK8963_I2C_ADDR, MPUREG_I2C_SLV0_ADDR},  //Set the I2C slave addres of AK8963 and set for write.
        //{0x09, MPUREG_I2C_SLV4_CTRL},
        //{0x81, MPUREG_I2C_MST_DELAY_CTRL}, //Enable I2C delay

        {AK8963_CNTL2, MPUREG_I2C_SLV0_REG}, //I2C slave 0 register address from where to begin data transfer
        {0x01, MPUREG_I2C_SLV0_DO}, // Reset AK8963
        {0x81, MPUREG_I2C_SLV0_CTRL},  //Enable I2C and set 1 byte

        {AK8963_CNTL1, MPUREG_I2C_SLV0_REG}, //I2C slave 0 register address from where to begin data transfer
        {0x12, MPUREG_I2C_SLV0_DO}, // Register value to continuous measurement in 16bit
        {0x81, MPUREG_I2C_SLV0_CTRL}  //Enable I2C and set 1 byte

    };
    //spi.format(8,0);
    //spi.frequency(1000000);

    for(i=0; i<init_reg_num; i++) {
        writeReg(MPU_Init_Data[i][1], MPU_Init_Data[i][0]);
        usleep(100000);  //I2C must slow down the write speed, otherwise it won't work
    }

    setAccScale(BITS_FS_16G);
    setGyroScale(BITS_FS_2000DPS);

    calibMag();
    return 0;
}

uint32_t MPU9250::setAccScale(int scale)
{
    uint32_t temp_scale;
    writeReg(MPUREG_ACCEL_CONFIG, scale);

    switch (scale)
    {
        case BITS_FS_2G:
            acc_divider=16384;
        break;
        case BITS_FS_4G:
            acc_divider=8192;
        break;
        case BITS_FS_8G:
            acc_divider=4096;
        break;
        case BITS_FS_16G:
            acc_divider=2048;
        break;
    }

    temp_scale=writeReg(MPUREG_ACCEL_CONFIG|READ_FLAG, 0x00);
    switch (temp_scale)
    {
        case BITS_FS_2G:
            temp_scale=2;
        break;
        case BITS_FS_4G:
            temp_scale=4;
        break;
        case BITS_FS_8G:
            temp_scale=8;
        break;
        case BITS_FS_16G:
            temp_scale=16;
        break;
    }
    return temp_scale;
}

uint32_t MPU9250::setGyroScale(int scale)
{
    uint32_t temp_scale;
    writeReg(MPUREG_GYRO_CONFIG, scale);
    switch (scale)
    {
        case BITS_FS_250DPS:
            gyro_divider=131;
        break;
        case BITS_FS_500DPS:
            gyro_divider=65.5;
        break;
        case BITS_FS_1000DPS:
            gyro_divider=32.8;
        break;
        case BITS_FS_2000DPS:
            gyro_divider=16.4;
        break;
    }

    temp_scale = writeReg(MPUREG_GYRO_CONFIG|READ_FLAG, 0x00);
    switch (temp_scale)
    {
        case BITS_FS_250DPS:
            temp_scale=250;
        break;
        case BITS_FS_500DPS:
            temp_scale=500;
        break;
        case BITS_FS_1000DPS:
            temp_scale=1000;
        break;
        case BITS_FS_2000DPS:
            temp_scale=2000;
        break;
    }
    return temp_scale;
}


uint32_t MPU9250::whoAmI()
{
    uint32_t response;
    response = writeReg(MPUREG_WHOAMI|READ_FLAG, 0x00);
    return response;
}

void MPU9250::readAcc()
{
    uint8_t response[6];
    int16_t bit_data;
    float data;
    int i;
    readRegs(MPUREG_ACCEL_XOUT_H,response,6);
    for(i=0; i<3; i++) {
        bit_data=((int16_t)response[i*2]<<8)|response[i*2+1];
        data=(float)bit_data;
        acce_raw_data[i] = bit_data;
        accelerometer_data[i]=data/acc_divider;
    }
}

void MPU9250::readGyro()
{
    uint8_t response[6];
    int16_t bit_data;
    float data;
    int i;
    readRegs(MPUREG_GYRO_XOUT_H,response,6);
    for(i=0; i<3; i++) {
        bit_data=((int16_t)response[i*2]<<8)|response[i*2+1];
        data=(float)bit_data;
        gyro_raw_data[i] = bit_data;
        gyroscope_data[i]=data/gyro_divider;
    }
}

void MPU9250::readTemp()
{
    uint8_t response[2];
    int16_t bit_data;
    float data;
    readRegs(MPUREG_TEMP_OUT_H,response,2);

    bit_data=((int16_t)response[0]<<8)|response[1];
    data=(float)bit_data;
    temperature=(data/340)+36.53;  // in °C
}

void MPU9250::calibAcc()
{
    uint8_t response[4];
    int temp_scale;
    //READ CURRENT ACC SCALE
    temp_scale=writeReg(MPUREG_ACCEL_CONFIG|READ_FLAG, 0x00);
    setAccScale(BITS_FS_8G);
    //ENABLE SELF TEST need modify
    //temp_scale=writeReg(MPUREG_ACCEL_CONFIG, 0x80>>axis);

    readRegs(MPUREG_SELF_TEST_X,response,4);
    calib_data[0]=((response[0]&11100000)>>3)|((response[3]&00110000)>>4);
    calib_data[1]=((response[1]&11100000)>>3)|((response[3]&00001100)>>2);
    calib_data[2]=((response[2]&11100000)>>3)|((response[3]&00000011));
    setAccScale(temp_scale);
}

void MPU9250::calibMag()
{
    uint8_t response[3];
    float data;
    int i;

    writeReg(MPUREG_I2C_SLV0_ADDR,AK8963_I2C_ADDR|READ_FLAG); //Set the I2C slave addres of AK8963 and set for read.
    writeReg(MPUREG_I2C_SLV0_REG, AK8963_ASAX); //I2C slave 0 register address from where to begin data transfer
    writeReg(MPUREG_I2C_SLV0_CTRL, 0x83); //Read 3 bytes from the magnetometer

    //writeReg(MPUREG_I2C_SLV0_CTRL, 0x81);    //Enable I2C and set bytes
    usleep(10000);
    //response[0]=writeReg(MPUREG_EXT_SENS_DATA_01|READ_FLAG, 0x00);    //Read I2C
    readRegs(MPUREG_EXT_SENS_DATA_00,response,3);

    //response=writeReg(MPUREG_I2C_SLV0_DO, 0x00);    //Read I2C
    for(i=0; i<3; i++) {
        data=response[i];
        magnetometer_ASA[i]=((data-128)/256+1)*Magnetometer_Sensitivity_Scale_Factor;
	    //printf("magnetometer_ASA[%d] = %f\n", i, magnetometer_ASA[i]);
    }
}

uint8_t MPU9250::AK8963WhoAmI()
{
    uint8_t response;
    writeReg(MPUREG_I2C_SLV0_ADDR,AK8963_I2C_ADDR|READ_FLAG); //Set the I2C slave addres of AK8963 and set for read.
    writeReg(MPUREG_I2C_SLV0_REG, AK8963_WIA); //I2C slave 0 register address from where to begin data transfer
    writeReg(MPUREG_I2C_SLV0_CTRL, 0x81); //Read 1 byte from the magnetometer

    //writeReg(MPUREG_I2C_SLV0_CTRL, 0x81);    //Enable I2C and set bytes
    usleep(10000);
    response=writeReg(MPUREG_EXT_SENS_DATA_00|READ_FLAG, 0x00);    //Read I2C
    //readRegs(MPUREG_EXT_SENS_DATA_00,response,1);
    //response=writeReg(MPUREG_I2C_SLV0_DO, 0x00);    //Read I2C

    return response;
}

void MPU9250::readMag()
{
    uint8_t response[7];
    int16_t bit_data;
    float data;
    int i;

    writeReg(MPUREG_I2C_SLV0_ADDR,AK8963_I2C_ADDR|READ_FLAG); //Set the I2C slave addres of AK8963 and set for read.
    writeReg(MPUREG_I2C_SLV0_REG, AK8963_HXL); //I2C slave 0 register address from where to begin data transfer
    writeReg(MPUREG_I2C_SLV0_CTRL, 0x87); //Read 6 bytes from the magnetometer

    usleep(10000);
    readRegs(MPUREG_EXT_SENS_DATA_00,response,7);
    //must start your read from AK8963A register 0x03 and read seven bytes so that upon read of ST2 register 0x09 the AK8963A will unlatch the data registers for the next measurement.
    for(i=0; i<3; i++) {
        bit_data=((int16_t)response[i*2+1]<<8)|response[i*2];
        data=(float)bit_data;
        mag_raw_data[i] = bit_data;
        magnetometer_data[i]=data*magnetometer_ASA[i];
    }
}

void MPU9250::readAll()
{
    uint8_t response[21];
    int16_t bit_data;
    float data;
    int i;

    //Send I2C command at first
    writeReg(MPUREG_I2C_SLV0_ADDR,AK8963_I2C_ADDR|READ_FLAG); //Set the I2C slave addres of AK8963 and set for read.
    writeReg(MPUREG_I2C_SLV0_REG, AK8963_HXL); //I2C slave 0 register address from where to begin data transfer
    writeReg(MPUREG_I2C_SLV0_CTRL, 0x87); //Read 7 bytes from the magnetometer
    //must start your read from AK8963A register 0x03 and read seven bytes so that upon read of ST2 register 0x09 the AK8963A will unlatch the data registers for the next measurement.

    //wait(0.001);
    readRegs(MPUREG_ACCEL_XOUT_H,response,21);
    //Get accelerometer value
    for(i=0; i<3; i++) {
        bit_data=((int16_t)response[i*2]<<8)|response[i*2+1];
        data=(float)bit_data;
        acce_raw_data[i] = bit_data;
        accelerometer_data[i]=data/acc_divider;
    }
    //Get temperature
    bit_data=((int16_t)response[i*2]<<8)|response[i*2+1];
    data=(float)bit_data;
    temperature=((data-21)/333.87)+21;
    //Get gyroscope value
    for(i=4; i<7; i++) {
        bit_data=((int16_t)response[i*2]<<8)|response[i*2+1];
        data=(float)bit_data;
        gyro_raw_data[i] = bit_data;
        gyroscope_data[i-4]=data/gyro_divider;
    }
    //Get Magnetometer value
    for(i=7; i<10; i++) {
        bit_data=((int16_t)response[i*2+1]<<8)|response[i*2];
        data=(float)bit_data;
        mag_raw_data[i] = bit_data;
        magnetometer_data[i-7]=data*magnetometer_ASA[i-7];
    }
}

void MPU9250::getMotionRaw9(int16_t *ax, int16_t *ay, int16_t *az,
                            int16_t *gx, int16_t *gy, int16_t *gz,
                            int16_t *mx, int16_t *my, int16_t *mz)
{
    readAcc();
    readGyro();
    readMag();
    *ax = acce_raw_data[0];
    *ay = acce_raw_data[1];
    *az = acce_raw_data[2];

    *gx = gyro_raw_data[0];
    *gy = gyro_raw_data[1];
    *gz = gyro_raw_data[2];

    *mx = mag_raw_data[0];
    *my = mag_raw_data[1];
    *mz = mag_raw_data[2];
}                          

void MPU9250::getMotionRaw6(int16_t *ax, int16_t *ay, int16_t *az,
                            int16_t *gx, int16_t *gy, int16_t *gz)
{
    readAcc();
    readGyro();
    readMag();
    *ax = acce_raw_data[0];
    *ay = acce_raw_data[1];
    *az = acce_raw_data[2];

    *gx = gyro_raw_data[0];
    *gy = gyro_raw_data[1];
    *gz = gyro_raw_data[2];
}         

void MPU9250::getMotion9(float *ax, float *ay, float *az, float *gx, float *gy, float *gz, float *mx, float *my, float *mz)
{
    readAcc();
    readGyro();
    readMag();
    *ax = accelerometer_data[0];
    *ay = accelerometer_data[1];
    *az = accelerometer_data[2];
    *gx = gyroscope_data[0];
    *gy = gyroscope_data[1];
    *gz = gyroscope_data[2];
    *mx = magnetometer_data[0];
    *my = magnetometer_data[1];
    *mz = magnetometer_data[2];
}

void MPU9250::getMotion6(float *ax, float *ay, float *az, float *gx, float *gy, float *gz)
{
    readAcc();
    readGyro();
    *ax = accelerometer_data[0];
    *ay = accelerometer_data[1];
    *az = accelerometer_data[2];
    *gx = gyroscope_data[0];
    *gy = gyroscope_data[1];
    *gz = gyroscope_data[2];
}

uint32_t MPU9250::writeReg(uint8_t address, uint8_t data)
{
    uint32_t temp_val;
    uint8_t tx[2] = {address, data};
    uint8_t rx[2] = {0};

    SPI::transfer("/dev/spidev0.1", tx, rx, 2);
    return rx[1];
}

uint32_t MPU9250::readReg(uint8_t address, uint8_t data)
{
    return writeReg(address | READ_FLAG, data);
}

void MPU9250::readRegs(uint8_t address, uint8_t *read_buffer, uint32_t buffer_length)
{
    uint32_t i = 0;
    uint8_t tx[255] = {0};
    uint8_t rx[255] = {0};
    tx[0] = address | READ_FLAG;

    SPI::transfer("/dev/spidev0.1", tx, rx, buffer_length + 1);

    for(i=0; i<buffer_length; i++)
    	read_buffer[i] = rx[i + 1];

    usleep(50);
}
