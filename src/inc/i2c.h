#include <8052.h>

#ifndef I2C_SCL
#define I2C_SCL P3_7
#endif

#ifndef I2C_SDA
#define I2C_SDA P3_6
#endif

#define _nop_() __asm nop __endasm

void i2c_start(void);
void i2c_stop(void);
unsigned char i2c_addressing(unsigned char addr);
unsigned char i2c_write(unsigned char dat);
unsigned char i2c_read_NAK();
unsigned char i2c_read_ACK();
unsigned char i2c_read_byte(unsigned char addr);
__bit i2c_write_byte(unsigned char addr, unsigned char dat);
unsigned char i2c_read_byte_256(unsigned int addr);
__bit i2c_write_byte_256(unsigned int addr, unsigned char dat);