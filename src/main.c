#include <8052.h>
#include <kingst.h>
#include "./inc/serial.h"
#include <stdio.h>
#include "i2c.h"

#define I2C_SCL P3_7
#define I2C_SDA P3_6

void main(void) {
    lcd_init();
    serial_init(9600);
    lcd_init();
    __bit ack = i2c_addressing(0x50);
    printf_tiny("接收到 0x50 的信息: %c\n", ack + '0'); 
    unsigned char str[7];
    ack = i2c_addressing(0x51);
    printf_tiny("接收到 0x51 的信息: %c\n", ack + '0');
    ack = i2c_addressing(0x52);
    printf_tiny("接收到 0x51 的信息: %c\n", ack + '0');
    while (1);
}