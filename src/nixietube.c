#include "kingst.h"

#ifndef KEIL
unsigned char __code nixie_chars[] = {                     // __code: 保存在程序存储空间 ROM, 而不是内存 RAM 中
#else
unsigned char code nixie_chars[] = {                     // __code: 保存在程序存储空间, 而不是内存中
#endif
    0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
    0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e
};

unsigned char nixietube_buffer[6] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

unsigned char nixietube_enable(unsigned char num_tube) {
    ADDR3 = 1;
    // �����ĸ� LED ��·������, 110 ʱ��������ܵ�(LEDS0)
    switch (num_tube) {
        case 0:                                 // LEDS0
            ADDR2 = 0; ADDR1 = 0; ADDR0 = 0;
            break;
        case 1:                                 // LEDS1
            ADDR2 = 0; ADDR1 = 0; ADDR0 = 1;
            break;
        case 2:                                 // LEDS2
            ADDR2 = 0; ADDR1 = 1; ADDR0 = 0;
            break;
        case 3:                                 // LEDS3
            ADDR2 = 0; ADDR1 = 1; ADDR0 = 1;
            break;
        case 4:                                 // LEDS4
            ADDR2 = 1; ADDR1 = 0; ADDR0 = 0;
            break;
        case 5:                                 // LEDS5
            ADDR2 = 1; ADDR1 = 0; ADDR0 = 1;
            break;
        default:
            break;
            // return 0;
    }
    return 1;
}

/**
 * �����������ʾ����
 */
void nixie_show_number(unsigned int num) {
    unsigned char buf[6];
    for (int i = 0; i < 6; i++) {
        buf[i] = num % 10;
        num = num / 10;
    }

    int i;
    for (i = 5; i >= 1; i--) {
        if (buf[i] != 0) {
            break;
        }
    }
    for (; i >= 0; i--) {
        nixietube_buffer[i] = nixie_chars[buf[i]];
    }
}

/**
 * ���ж���, ��������ܶ�̬ɨ����ʾ����
 */
void nixietube_scan(void) {
    static unsigned char i = 0;
    ENLED = 1;
    nixietube_enable(i);
    P0 = nixietube_buffer[i];
    ENLED = 0;
    if (++i >= 6) {
        i = 0;
    }
}
