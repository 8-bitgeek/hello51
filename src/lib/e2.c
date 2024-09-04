#include "i2c.h"
#include "e2.h"

unsigned char e2_read_byte(unsigned char addr) {
    do {                                                // wait until e2prom ready
        i2c_start();
        if (i2c_write(E2PROM_ADDR << 1)) {
            break;                                      // check if e2prom ready
        }
        i2c_stop();
    } while (1);

    i2c_write(addr);
    i2c_start();
    i2c_write((E2PROM_ADDR << 1) | 0x01);               // read operation
    unsigned char dat = i2c_read_NAK();
    i2c_stop();
    return dat;
}

__bit e2_write_byte(unsigned char addr, unsigned char dat) {
    i2c_start();
    i2c_write(E2PROM_ADDR << 1);
    i2c_write(addr);
    __bit is_write_suc = i2c_write(dat);
    i2c_stop();
    return is_write_suc;
}

void e2_read(unsigned char addr, unsigned char * buf, unsigned int len) {
    do {                                                // wait until e2prom ready
        i2c_start();
        if (i2c_write(E2PROM_ADDR << 1)) {
            break;                                      // check if e2prom ready
        }
        i2c_stop();
    } while (1);

    i2c_write(addr);
    i2c_start();
    i2c_write((E2PROM_ADDR << 1) | 0x01);               // read operation
    while (len-- > 1) {
        *buf++ = i2c_read_ACK();                        // read 1 byte data into buffer
    }
    *buf = i2c_read_NAK();                              // the last byte no need to reply ack
    i2c_stop();
}

void e2_write(unsigned int addr, unsigned char * dat, unsigned char len) {
    while (len--) {
        do {                                                // wait until e2prom ready
            i2c_start();
            if (i2c_write(E2PROM_ADDR << 1)) {
                break;                                      // check if e2prom ready
            }
            i2c_stop();
        } while (1);
        i2c_write(addr++);
        i2c_write(*dat++);
        i2c_stop();
    }
}

unsigned char e2c_read_byte_256(unsigned int addr) {
    i2c_start();
    i2c_write(E2PROM_ADDR << 1);                       // write operation
    i2c_write(addr >> 8);
    i2c_write(addr &= 0x00ff);
    i2c_start();
    i2c_write((E2PROM_ADDR << 1) | 0x01);              // read operation
    unsigned char dat = i2c_read_NAK();
    i2c_stop();
    return dat;
}

__bit e2c_write_byte_256(unsigned int addr, unsigned char dat) {
    i2c_start();
    i2c_write(E2PROM_ADDR << 1);
    i2c_write(addr >> 8);
    i2c_write(addr & 0x00ff);
    __bit is_write_suc = i2c_write(dat);
    i2c_stop();
    return is_write_suc;
}

void e2_read_256(unsigned char addr, unsigned char * buf, unsigned int len) {
    do {                                                // wait until e2prom ready
        i2c_start();
        if (i2c_write(E2PROM_ADDR << 1)) {
            break;                                      // check if e2prom ready
        }
        i2c_stop();
    } while (1);

    i2c_write(addr >> 8);
    i2c_write(addr & 0x00FF);
    i2c_start();
    i2c_write((E2PROM_ADDR << 1) | 0x01);               // read operation
    while (len-- > 1) {
        *buf++ = i2c_read_ACK();                        // read 1 byte data into buffer
    }
    *buf = i2c_read_NAK();                              // the last byte no need to reply ack
    i2c_stop();
}

void e2_write_256(unsigned int addr, unsigned char * dat, unsigned char len) {
    while (len--) {
        do {                                                // wait until e2prom ready
            i2c_start();
            if (i2c_write(E2PROM_ADDR << 1)) {
                break;                                      // check if e2prom ready
            }
            i2c_stop();
        } while (1);
        i2c_write(addr >> 8);
        i2c_write(addr & 0x00ff);
        addr++;
        i2c_write(*dat++);
        i2c_stop();
    }
}

void e2_page_write(unsigned int addr, unsigned char * dat, unsigned char len) {
    while (len--) {
        do {                                                // wait until e2prom ready
            i2c_start();
            if (i2c_write(E2PROM_ADDR << 1)) {
                break;                                      // check if e2prom ready
            }
            i2c_stop();
        } while (1);
        i2c_write(addr);
        while (len-- > 1) {
            addr++;
            i2c_write(*dat++);
            if ((addr & 0x07) == 0) {                       // if arrive ends of page, need recheck e2prom ack flag
                break;
            }
        }
        i2c_stop();
    }
}

void e2_page_write_256(unsigned int addr, unsigned char * dat, unsigned char len) {
    while (len--) {
        do {                                                // wait until e2prom ready
            i2c_start();
            if (i2c_write(E2PROM_ADDR << 1)) {
                break;                                      // check if e2prom ready
            }
            i2c_stop();
        } while (1);
        i2c_write(addr >> 8);
        i2c_write(addr & 0x00ff);
        while (len-- > 1) {
            addr++;
            i2c_write(*dat++);
            if ((addr & 0x07) == 0) {                       // if arrive ends of page, need recheck e2prom ack flag
                break;
            }
        }
        i2c_stop();
    }
}
