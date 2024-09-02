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
__bit i2c_addressing(unsigned char addr);
__bit i2c_write(unsigned char dat);