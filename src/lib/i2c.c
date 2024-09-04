#include "i2c.h"

static void i2c_delay(void) {
    _nop_();
    _nop_();
    _nop_();
    _nop_();
}

/**
 * Testing if spicified addr has a device
 */
unsigned char i2c_addressing(unsigned char addr) {
    i2c_start();
    unsigned ack = i2c_write(addr << 1);               // 7 bit addr + 1 bit W/R, 0 for write
    i2c_stop();
    return ack;
}

void i2c_start(void) {
    I2C_SDA = 1;
    I2C_SCL = 1;
    i2c_delay();
    I2C_SDA = 0;
    i2c_delay();
    I2C_SCL = 0;
}

void i2c_stop(void) {
    I2C_SCL = 0;
    I2C_SDA = 0;
    i2c_delay();
    I2C_SCL = 1;
    i2c_delay();
    I2C_SDA = 1;
    i2c_delay();
}

unsigned char i2c_write(unsigned char dat) {
    for (unsigned char mask = 0x80; mask != 0; mask >>= 1) {     // MSB(Most Significant Bit)  ====  LSB(Least Significant Bit)
        if (mask & dat) {           // 1
            I2C_SDA = 1;
        } else {
            I2C_SDA = 0;
        }
        i2c_delay();
        I2C_SCL = 1;
        i2c_delay();
        I2C_SCL = 0;
    }
    I2C_SDA = 1;                    // data send finished, then relase SDA to detecting slave's response
    i2c_delay();
    I2C_SCL = 1;                    // SCL from 0 to 1 to read acknowledgement signal from slave
    i2c_delay();
    __bit ACK = I2C_SDA;
    i2c_delay();
    I2C_SCL = 0;
    if (ACK) {
        return 0;
    } else {
        return 1;
    }
}

unsigned char i2c_read_NAK() {
    unsigned char mask;
    unsigned char dat;

    I2C_SDA = 1;
    for (mask = 0x80; mask != 0; mask >>= 1) {
        i2c_delay();
        I2C_SCL = 1;
        if (I2C_SDA == 0) {
            dat &= ~mask;
        } else {
            dat |= mask;
        }
        i2c_delay();
        I2C_SCL = 0;
    }
    // i2c_delay();
    I2C_SDA = 1;                    // None ACK signal
    i2c_delay();
    I2C_SCL = 1;
    i2c_delay();
    I2C_SCL = 0;
    return dat;
}

unsigned char i2c_read_ACK() {
    unsigned char mask;
    unsigned char dat;

    I2C_SDA = 1;
    for (mask = 0x80; mask != 0; mask >>= 1) {
        i2c_delay();
        I2C_SCL = 1;
        if (I2C_SDA == 0) {
            dat &= ~mask;
        } else {
            dat |= mask;
        }
        i2c_delay();
        I2C_SCL = 0;
    }
    // i2c_delay();
    I2C_SDA = 0;                    // ACK signal
    i2c_delay();
    I2C_SCL = 1;
    i2c_delay();
    I2C_SCL = 0;
    return dat;
}
