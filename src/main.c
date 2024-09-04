#include <8052.h>
#include <kingst.h>
#include "./inc/serial.h"
#include <stdio.h>
#include "i2c.h"
#include "e2.h"

#define I2C_SCL P3_7
#define I2C_SDA P3_6


void main(void) {
    serial_init(9600);
    unsigned char ack = i2c_addressing(E2PROM_ADDR);
    printf_tiny("device exist: %c\n", ack + '0'); 
    unsigned char * str = "Closed the serial port!";
    e2_page_write_256(0, str, 24);
    unsigned char hello[18];
    e2_read_256(0, hello, 18);
    printf_tiny("%s\n", hello);
    while (1);
}
