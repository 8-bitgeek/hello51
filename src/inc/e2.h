#ifndef E2PROM_ADDR
#define E2PROM_ADDR 0x50
#endif

unsigned char e2_read_byte(unsigned char addr);
__bit e2_write_byte(unsigned char addr, unsigned char dat);
void e2_read(unsigned char addr, unsigned char * buf, unsigned int len);
void e2_write(unsigned int addr, unsigned char * dat, unsigned char len);
unsigned char e2c_read_byte_256(unsigned int addr);
__bit e2c_write_byte_256(unsigned int addr, unsigned char dat);
void e2_read_256(unsigned char addr, unsigned char * buf, unsigned int len);
void e2_write_256(unsigned int addr, unsigned char * dat, unsigned char len);
void e2_page_write(unsigned int addr, unsigned char * dat, unsigned char len);
void e2_page_write_256(unsigned int addr, unsigned char * dat, unsigned char len);