//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

#include <8051.h>
// #include "inc/serial.h"
#include "oled12864.h"
#include "kingst.h"
#include "i2c.h"
#include <stdio.h>

extern void oled_write_data(u8 * dat, u8 len);
extern void oled_write_cmd(u8 * cmd, u8 len);
void oled_continuous_scroll();

void main(void) {
    oled_init();
    oled_clear();
    oled_addr_mode(ADDR_MODE_PAGE);
    int i = 0;
    while (1) {
        for (unsigned int i = 0; i < 65; i++);
        // oled_inverse_display();
        i2c_start();
        u8 exist = oled_addr();
        printf_tiny("i = %d\n", i++);
        i2c_stop();
    }
}
