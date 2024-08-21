#include "8051.h"
#include "inc/kingst.h"
#include "inc/led.h"
#include "inc/timer.h"

unsigned char __code nixie_chars[] = {                     // __code: 保存在程序存储空间, 而不是内存中
    0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
    0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e
};

/* 数码管显示缓冲区 */
unsigned char nixietube_buffer[6] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

unsigned char flag1s = 0;                               // 时间已过一秒标志

void main() {
    unsigned long sec = 0;
    init_timer(0);
    setup_timer(0, 1);
    start_timer(0);
    EA = 1;
    ET0 = 1;

    while (1) {
        if (flag1s == 1) {
            sec++;
            flag1s = 0;
            nixietube_buffer[0] = nixie_chars[sec % 10];
            // nixie_tube_buffer[1] = sec >= 10 ? nixie_chars[sec / 10 % 10] : 0xff;
            // nixie_tube_buffer[2] = sec >= 100 ? nixie_chars[sec / 100 % 10] : 0xff;
            // nixie_tube_buffer[3] = sec >= 1000 ? nixie_chars[sec / 1000 % 10] : 0xff;
            // nixie_tube_buffer[4] = sec >= 10000 ? nixie_chars[sec / 10000 % 10] : 0xff;
            // nixie_tube_buffer[5] = sec >= 100000 ? nixie_chars[sec / 100000 % 10] : 0xff;
            nixietube_buffer[1] = nixie_chars[(sec / 10) % 10];
            nixietube_buffer[2] = nixie_chars[(sec / 100) % 10];
            nixietube_buffer[3] = nixie_chars[(sec / 1000) % 10];
            nixietube_buffer[4] = nixie_chars[(sec / 10000) % 10];
            nixietube_buffer[5] = nixie_chars[(sec / 100000) % 10];
        }
    }
}

/* TF0 == 1 时进入中断函数 */
void timer0_isr(void) __interrupt (1) __using (1) {     // __interrupt 指定中断号, __using 指定 register bank
    static unsigned int cnt = 0;                        // 1ms 次数计数
    static unsigned char i = 0;                         // 当前显示的数码管下标
    reset_timer(0, 1);
    if (++cnt >= 1000) {
        cnt = 0;
        flag1s = 1;
    }
    if (++i >= 6) {i = 0;}
    P0 = 0xff;
    enable_nixietube(i);
    P0 = nixietube_buffer[i];
}