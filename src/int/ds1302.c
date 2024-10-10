#include "ds1302.h"
#include "kingst.h"
#include <stdio.h>


static void delay(void) {
    _nop_();
    _nop_();
}

static unsigned char ds1302_read_byte(void) {
    unsigned char data = 0;
    SCLK = 0;
    for (unsigned char mask = 0x01; mask != 0; mask <<= 1) {
        if (SIO) {
            data |= mask;
        }                                                   // read data from SIO then pullup the SCLK, otherwise may cause read wrong signal
        SCLK = 1;
        SCLK = 0;
    }
    return data;
}

static unsigned char ds1302_write_byte(unsigned char reg) {
    // For data inputs, data must be valid during the rising edge of the clock and data bits are output on the falling edge of clock
    SCLK = 0;
    for (unsigned char mask = 0x01; mask != 0; mask <<= 1) {         // LSB, write from low bit
        if (mask & reg) {
            SIO = 1;
        } else {
            SIO = 0;
        }
        SCLK = 1;
        SCLK = 0;                                           // ds1302 read signal
    }
    SIO = 1;                                                // release io pin
    return 0;
}

unsigned char ds1302_read(unsigned char reg) {
    S1302_CE = 1;                                               // rising high to enable ds1302 read/write
    ds1302_write_byte((reg << 1) | 0x81);                                     // send command to ds1302
    unsigned char dat = ds1302_read_byte();                     // read data from ds1302
    S1302_CE = 0;
    return dat;
}

void ds1302_write(unsigned char reg, unsigned char dat) {
    // CE signal must be asserted high during a read or a write
    S1302_CE = 1;
    ds1302_write_byte((reg << 1) | 0x80);
    ds1302_write_byte(dat);
    S1302_CE = 0;
}

void ds1302_init(void) {
    unsigned char __code init_time[] = {0x00, 0x30, 0x15, 0x12, 0x09, 0x20, 0x24};      // initial time is 2024-09-12T15:30:00+8
    unsigned char dat = ds1302_read(0);
    if (dat & 0x80) {                                           // ds1302 stopped if high bit is 1
        printf_tiny("initialize RTC...\n");
        ds1302_write(7, 0x00);                                  // disable write protect(WP) flag
        for (unsigned char i = 0; i < 7; i++) {
            ds1302_write(i, init_time[i]);
        }
    } else {
        printf_tiny("RTC is running...\n");
    }
}