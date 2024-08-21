#include "8051.h"
#include "inc/kingst.h"
#include "inc/led.h"
#include "inc/timer.h"
#include "inc/delay.h"

unsigned char __code nixie_chars[] = {                     // __code: 保存在程序存储空�?, 而不是内存中
    0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
    0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e
};

__bit key_status = 1;                                       // 按键目前的状�?

void main() {
    unsigned char cnt = 0;
    __bit backup = 1;                                      // 记录上一次的按键状�?: 1 - 弹起, 0 - 按下
    init_timer(0);
    setup_timer(0, 1);
    start_timer(0);
    EA = 1;
    ET0 = 1;
    P2 = 0xFF;
    K_OUT1 = 0;

    enable_nixietube(1);

    while (1) {
        if (backup != key_status) {
            if (backup == 0) {                              // 原来是按�?, 目前是弹起状�?
                if (++cnt >= 10) {
                    cnt = 0;
                }
                P0 = nixie_chars[cnt];
            }
            backup = key_status;
        }
    }
}

void timer0_isr(void) __interrupt (1) __using (1) {
    static unsigned char key_buf = 0xff;                    // 缓存按键状�?, 每位代表一个状�?, 初始为弹起状�?, �? 8 位全�? 1

    reset_timer(0, 1);

    key_buf = key_buf << 1 | K_IN1;
    if ((key_buf & 0x0f) == 0x0f) {                           // �� 4 λ���� 1
        key_status = 1;
    } else if ((key_buf & 0x0f) == 0x00) {                    // �� 4 λ���� 0
        key_status = 0;
    }
}