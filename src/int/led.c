#include <8051.h>
#include "kingst.h"

/**
 * ������ͨ LED ���Ƶ�·
 */
void enable_normal_led(void) {
    ENLED = 0;
    ADDR3 = 1;

    // �����ĸ� LED ��·������, 110 ʱ������ͨ LED ��(LEDS6)
    ADDR2 = 1;
    ADDR1 = 1;
    ADDR0 = 0;
}


unsigned char disable_nixietube(void) {
    ENLED = 1;
    return 1;
}

unsigned char close_nixie_display(void) {
    P0 = 0xff;
    return 1;
}

/**
 * ��ĳ�� LED
 */
void turn_on_normal_led(unsigned char n_led) {
    P0 = ~(0x1 << n_led);
}