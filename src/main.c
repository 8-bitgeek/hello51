#include <8052.h>
#include <kingst.h>
#include "./inc/serial.h"
#include <stdio.h>
#include "ds1302.h"

#define I2C_SCL P3_7
#define I2C_SDA P3_6


void main(void) {
    serial_init(9600);
    ds1302_init();
    
    while (1) {
        ds1302_init();
        for (unsigned int i = 0; i < 65534; i++);
        unsigned char dat = ds1302_read(0);
        printf_tiny("dat is %d\n", dat);
        unsigned char sec = dat & 0x0f;
        unsigned char psec = (dat & 0x70) >> 4 ;
        printf_tiny("psec = %d, sec = %d\n", psec, sec);
        SCLK = 1;
    }
}
