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
    unsigned char ack = i2c_addressing(0x50);
    printf_tiny("device 0x51 exist: %c\n", ack + '0'); 
    unsigned char dat = i2c_read_byte_256(300);
    printf_tiny("i2c addr 300: %d\n", (unsigned int) dat);
    __bit is_write_suc = i2c_write_byte_256(300, ++dat);
    if (is_write_suc) {
        printf_tiny("write to addr-300 succeed!\n");
    } else {
        printf_tiny("write to addr-300 failed!\n");
    }
    while (1);
}
