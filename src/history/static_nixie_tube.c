#include "8051.h"
#include "inc/kingst.h"
#include "inc/led.h"
#include "inc/timer.h"

unsigned char __code led_chars[] = {                     // __code: 保存在程序存储空�?, 而不是内存中
    0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
    0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e
};

void main() {
    unsigned int cnt, sec = 0;
    int i = 0;
    enable_nixietube(0);
    init_timer(0);
    setup_timer(0, 20);
    start_timer(0);
    while (1) {
        if (TF0 == 1) {
            TF0 = 0;
            reset_timer(0, 20);
            if (++cnt >= 50) {
                cnt = 0;
                P0 = led_chars[sec];
                // if (++i > 5) {i = 0;}
                // enable_nixietube(i);
                if (++sec > 0xf) {
                    sec = 0;
                }
            }

        }
    }
}
