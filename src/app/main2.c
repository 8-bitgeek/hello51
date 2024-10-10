#include "kingst.h"
#include <8052.h>

unsigned char array1[1] = {1};
unsigned char array2[2] = {1, 2};
unsigned char array3[3] = {1, 2, 3};
unsigned char array4[4] = {1, 2, 3, 4};
unsigned char array8[8] = {1, 2, 3, 4, 5, 6, 7, 8};

__bit command_arrived = 0;
unsigned char cmd;
unsigned char * ptr;
unsigned char cnt = 0;

void config_uart(unsigned int baud) {
    SCON = 0x50;
    TMOD &= 0x0F;
    TMOD |= 0x20;
    TH1 = 256 - (CRYSTYLE_FREQ / 12 / 32) / baud;
    TL1 = TH1;
    TR1 = 1;
}

void main(void) {
    EA = 1;
    config_uart(9600);
    while (1) {
        if (command_arrived) {
            switch (cmd) {
                case 1:
                    ptr = array1;
                    cnt = sizeof(array1);
                    break;
                case 2:
                    ptr = array2;
                    cnt = sizeof(array2);
                    break;
                case 3:
                    ptr = array3;
                    cnt = sizeof(array3);
                    break;
                case 4:
                    ptr = array4;
                    cnt = sizeof(array4);
                    break;
                case 8:
                    ptr = array4;
                    cnt = sizeof(array8);
                    TI = 1;
                    break;
                default:
                    break;
            }
        }
    }
}

void uart_isr(void) __interrupt(4) __using(4) {
    if (RI) {
        RI = 0;
        cmd = SBUF;
        command_arrived = 1;
    } else {
        TI = 0;
        if (cnt-- > 0) {
            SBUF = *ptr;
            ptr++;
        }
    }
}