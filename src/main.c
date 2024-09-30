//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <8051.h>
// #include "inc/serial.h"
#include "inc/oled12864.h"
#include "kingst.h"
#include "i2c.h"
#include <stdio.h>

extern void oled_write_data(u8 * dat, u8 len);
extern void oled_write_cmd(u8 * cmd, u8 len);
void oled_continuous_scroll();

void main(void) {
    // serial_init(9600);
    oled_init();
    oled_clear();
    oled_addr_mode(ADDR_MODE_PAGE);
    // oled_print("hello Gldwolf!\n", 0x12);
    // oled_print("My name is Gldwolf!\n", 0x12);
    // oled_print("You guys suck!\n", 0x12);
    // oled_print("Six GOD!\n", 0x12);
    int i = 0;
    while (1) {
        for (unsigned int i = 0; i < 65; i++);
        // oled_inverse_display();
        i2c_start();
        u8 exist = oled_addr();
        if (exist) {
            printf_tiny("i = %d\n", i++);
            // printf_tiny("oled screen exist!\n");
        } else {
            // printf_tiny("oled screen not exist!\n");
        }
        i2c_stop();
    }
}
