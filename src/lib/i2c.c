#include "i2c.h"

/** 
 * Because 51 mcu is low speed device, so we don't need delay to satisify i2c protocol demand.
 */

/**
 * send start signal to all slave device.
 */
void i2c_start(void) {
    I2C_SCL = 1;
    I2C_SDA = 1;
    I2C_SDA = 0;
    I2C_SCL = 0;
}

/**
 * send stop signal to stop communication with slave .
 */
void i2c_stop(void) {
    I2C_SDA = 0;
    I2C_SCL = 1;
    I2C_SDA = 1;
}

/**
 * send byte to slave device and return 1 if send succeed, otherwise return 0.
 * @byt: the data send to slave device
 * Return: if write succeed
 */
unsigned char i2c_write_byte(unsigned char byt) {
    for (unsigned char i = 0x80; i != 0; i >>= 1) {                        // write start with MSB
        I2C_SDA = (byt & i) == 0 ? 0 : 1;
        I2C_SCL = 1;
        I2C_SCL = 0;
    }

    // i2c protocol claim that all device in i2c bus should in open-drain mode.
    // receieve ack signal from slave device
    I2C_SDA = 1;                                                // master must release SDA
    I2C_SCL = 1;
    __bit ack = I2C_SDA;
    I2C_SCL = 0;
    return ack ? 0 : 1;
}

/**
 * read 1 byte from slave device, and send ack to slave.
 * Return: 1 byte data from slave device
 */
unsigned char i2c_read_byte(void) {
    unsigned char dat = 0;
    for (unsigned char i = 0x80; i != 0; i >>= 1) {                        // read from MSB
        I2C_SCL = 1;
        if (I2C_SDA) {
            dat |= i;
        }
        I2C_SCL = 0;
    }
    // send ack signal to slave
    I2C_SDA = 0;
    I2C_SCL = 1;
    I2C_SCL = 0;
    return dat;
}
