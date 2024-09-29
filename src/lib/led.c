#include <8051.h>
#include "kingst.h"

/**
 * 激活普通 LED 控制电路
 */
void enable_normal_led(void) {
    ENLED = 0;
    ADDR3 = 1;

    // 决定哪个 LED 电路被激活, 110 时激活普通 LED 灯(LEDS6)
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
 * 打开某个 LED
 */
void turn_on_normal_led(unsigned char n_led) {
    P0 = ~(0x1 << n_led);
}