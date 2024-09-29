#include <kingst.h>

extern void i2c_start(void);
extern void i2c_stop(void);
extern unsigned char i2c_write_byte(unsigned char byt);
extern unsigned char i2c_read_byte(void);

#ifndef I2C_SCL
#define I2C_SCL P1_0
#endif
#ifndef I2C_SDA
#define I2C_SDA P1_1
#endif
