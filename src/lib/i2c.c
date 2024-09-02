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
__bit i2c_addressing(unsigned char addr) {
    i2c_start();
    __bit ack = i2c_write(addr << 1);               // 7 bit addr + 1 bit W/R
    i2c_stop();
    return ack;
}

void i2c_start(void) {
    I2C_SCL = 1;
    I2C_SDA = 1;
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

__bit i2c_write(unsigned char dat) {
    for (unsigned char mask = 0x80; mask != 0; mask >>= 1) {     // 从高位向低位发送
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
    I2C_SDA = 1;
    i2c_delay();
    I2C_SCL = 1;
    __bit ACK = I2C_SDA;
    return ACK;
}