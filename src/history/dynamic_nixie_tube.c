#include "8051.h"
#include "inc/kingst.h"
#include "inc/led.h"
#include "inc/timer.h"

unsigned char __code led_chars[] = {                     // __code: 保存在程序存储空�?, 而不是内存中
    0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
    0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e
};

/* 数码管显示缓冲区 */
unsigned char nixie_tube_buffer[6] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

void main() {
    unsigned int cnt = 0;
    unsigned long sec = 0;
    init_timer(0);
    setup_timer(0, 1);
    start_timer(0);

    int i = 0;
    while (1) {
        if (TF0 == 1) {
            TF0 = 0;
            reset_timer(0, 1);
            if (++cnt >= 1000) {       // 1s
                cnt = 0;
                sec++;
                nixie_tube_buffer[0] = led_chars[sec % 10];
                nixie_tube_buffer[1] = led_chars[sec / 10 % 10];
                nixie_tube_buffer[2] = led_chars[sec / 100 % 10];
                nixie_tube_buffer[3] = led_chars[sec / 1000 % 10];
                nixie_tube_buffer[4] = led_chars[sec / 10000 % 10];
                nixie_tube_buffer[5] = led_chars[sec / 100000 % 10];
            }
        }
        if (++i > 5) {i = 0;}
        // disable_nixie_tube();
        enable_nixietube(i);
        P0 = nixie_tube_buffer[i];
    }
}
