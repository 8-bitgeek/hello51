#include "8051.h"
#include "kingst.h"
#include "timer.h"
#include "led.h"
#include "keyboard.h"

unsigned char __code nixie_chars[] = {                     // __code: 淇濆瓨鍦ㄧ▼搴忓瓨鍌ㄧ┖闂?, 鑰屼笉鏄唴瀛樹腑
    0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
    0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e
};

unsigned char key_status[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1} };

void main() {
    unsigned char backups[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1} };

    EA = 1;
    enable_nixietube(1);
    init_timer(0);
    setup_timer(0, 1);
    start_timer(0);
    ET0 = 1;

    while (1) {
        for (char i = 0; i < 4; i++) {
            for (char j = 0; j < 4; j++) {
                if (backups[i][j] != key_status[i][j]) {
                    if (backups[i][j] == 0) {               // 按键弹起
                        P0 = nixie_chars[i * 4 + j];        // 显示按键值
                    }
                    backups[i][j] = key_status[i][j];
                }
            }
        }
    }
}


void timer0_isr(void) __interrupt(1) __using(1) {
    static unsigned char key_buffer[4][4] = {{0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}};
    static unsigned char k_out = 1;
    reset_timer(0, 1);
    key_buffer[k_out][0] = key_buffer[k_out][0] << 1 | K_IN1;
    key_buffer[k_out][1] = key_buffer[k_out][1] << 1 | K_IN2;
    key_buffer[k_out][2] = key_buffer[k_out][2] << 1 | K_IN3;
    key_buffer[k_out][3] = key_buffer[k_out][3] << 1 | K_IN4;
    for (char i = 0; i < 4; i++) {
        if ((key_buffer[k_out][i] & 0x0f) == 0x0f) {
            key_status[k_out][i] = 1;
        } else if ((key_buffer[k_out][i] & 0x0f) == 0x00) {
            key_status[k_out][i] = 0;
        }
    }
    k_out = ++k_out & 0x03;                 // 到 4 归 0
    enable_k_out(k_out);
}