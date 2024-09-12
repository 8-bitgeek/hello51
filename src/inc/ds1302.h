// SPI SCLK PIN
#ifndef SCLK
#define SCLK P3_5
#endif

#ifndef SIO
#define SIO P3_4
#endif

#ifndef S1302_CE
#define S1302_CE P1_7
#endif


unsigned char ds1302_read(unsigned char reg);
void ds1302_write(unsigned char reg, unsigned char dat);
void ds1302_init();