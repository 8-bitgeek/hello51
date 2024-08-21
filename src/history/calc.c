#include "8051.h"
#include "kingst.h"
#include "timer.h"
#include "led.h"
#include "keyboard.h"

unsigned char __code nixie_chars[] = {                     // __code: 保存在程序存储空间, 而不是内存中
    0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8,
    0x80, 0x90, 0x88, 0x83, 0xc6, 0xa1, 0x86, 0x8e
};

unsigned char nixietube_buffer[6] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};
unsigned char nixietube_buffer2[6] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

unsigned char key_status[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1} };

// unsigned char key_code_map[4][4] = {                 // 矩阵按键编号到标准键盘键码的映射表
//     {0x31, 0x32, 0x33, 0x26},                               // 数字键 1, 2, 3, 向上键
//     {0x34, 0x35, 0x36, 0x25},                               // 数字键 4, 5, 6, 向左键
//     {0x37, 0x38, 0x39, 0x28},                               // 数字键 7, 8, 9, 向下键
//     {0x30, 0x1b, 0x0d, 0x27}                                // 数字键 0, ESC 键, 回车键, 向右键
// };

void key_driver();
void key_action(unsigned char key_code);

void main() {
    init_timer(0);
    setup_timer(0, 1);
    start_timer(0);
    EA = 1;
    enable_timer_int(0);

    P0 = nixie_chars[0];
    enable_nixietube(0);

    while (1) {
        key_driver();
    }
}

void show_number(unsigned long number);
void key_driver() {
    static unsigned char backups[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1} };
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (backups[i][j] != key_status[i][j]) {        // 按键发生变化
                if (backups[i][j] == 0) {                   // 弹起动作
                    // P0 = nixie_chars[i * 3 + j];
                    show_number(123);
                    // key_action(key_code_map[i][j]);
                }
                backups[i][j] = key_status[i][j];           // 更新状态
            }
        }
    }
}

void show_number(unsigned long number) {
    unsigned char buf[6];
    for (int i = 0; i < 6; i++) {
        buf[i] = number % 10;
        number /= 10;
    }
    int i = 5;
    for (i = 5; i >= 1; i--) {                          // 高位 0 不显示
        if (buf[i] == 0) {
            nixietube_buffer[i] = 0xff;
        } else {
            break;
        }
    }
    for (; i >= 0; i--) {
        nixietube_buffer[i] = nixie_chars[buf[i]];
    }
}

/**
 * 执行键码(按键)对应的动作
 */
void key_action(unsigned char key_code) {
    static unsigned long result = 0, addend = 0;            // 结果和加数
    if (key_code >= 0x30 && key_code <= 0x39) {             // 按下的是数字键
        addend = (addend * 10) + (key_code - 0x30);
        show_number(addend);                                // 显示在数码管上
    } else if (key_code == 0x26) {                          // 输入 '+'
        result += addend;
        addend = 0;
        show_number(result);
    } else if (key_code == 0x0d) {
        result += addend;
        addend = 0;
        show_number(result);
    } else if (key_code == 0x1b) {
        addend = 0;
        result = 0;
        show_number(addend);
    }
}

void timer0_isr() __interrupt(1) __using(1) {
    static unsigned char key_buffer[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1} };
    static unsigned char k_out = 0;
    static unsigned char nixie_num = 0;
    reset_timer(0, 1);

    key_buffer[k_out][0] = (key_buffer[k_out][0] << 1) | K_IN1;
    key_buffer[k_out][1] = (key_buffer[k_out][1] << 1) | K_IN2;
    key_buffer[k_out][2] = (key_buffer[k_out][2] << 1) | K_IN3;
    key_buffer[k_out][3] = (key_buffer[k_out][3] << 1) | K_IN4;

    // 扫描/更新按键状态
    for (int i = 0; i < 4; i++) {
        if ((key_buffer[k_out][i] & 0x0f) == 0x0f) {            // 低 4 位都是 1
            key_status[k_out][i] = 1;
        } else if ((key_buffer[k_out][i] & 0x0f) == 0x00) {     // 低 4 位都是 0 
            key_status[k_out][i] = 0;
        }
    }
    enable_k_out(++k_out & 0x3);
    if (++nixie_num >= 6) {
        nixie_num = 0;
    }
    enable_nixietube(nixie_num);
    P0 = nixietube_buffer[nixie_num];
}