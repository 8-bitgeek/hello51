#include "kingst.h"

unsigned char key_status[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}};
unsigned char key_status_backup[4][4] = {{1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}, {1, 1, 1, 1}};
unsigned char __code key_code_map[4][4] = {                 // 矩阵按键编号到标准键盘键码的映射表
    {0x31, 0x32, 0x33, 0x26},                               // 数字键 1, 2, 3, 向上键
    {0x34, 0x35, 0x36, 0x25},                               // 数字键 4, 5, 6, 向左键
    {0x37, 0x38, 0x39, 0x28},                               // 数字键 7, 8, 9, 向下键
    {0x30, 0x1b, 0x0d, 0x27}                                // 数字键 0, ESC 键, 回车键, 向右键
};

/**
 * 打开哪个键盘通道 [0 - 3]
 */
unsigned char enable_k_out(unsigned char line) {
    switch (line) {
        case 0:
            K_CON |= 0x0F; K_OUT1 = 0;
            break;
        case 1:
            K_CON |= 0x0F; K_OUT2 = 0;
            break;
        case 2:
            K_CON |= 0x0F; K_OUT3 = 0;
            break;
        case 3:
            K_CON |= 0x0F; K_OUT4 = 0;
            break;
        default:
            return 0;
    }
    return 1;
}

void keyboard_scan(void) {
    /** 用于保存按键的状态, 按位刷新状态 */
    static unsigned char key_buf[4][4] = {{0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}, {0xff, 0xff, 0xff, 0xff}};
    static unsigned char key_out_index = 0;
    key_buf[key_out_index][0] = key_buf[key_out_index][0] << 1 | K_IN1;
    key_buf[key_out_index][1] = key_buf[key_out_index][1] << 1 | K_IN2;
    key_buf[key_out_index][2] = key_buf[key_out_index][2] << 1 | K_IN3;
    key_buf[key_out_index][3] = key_buf[key_out_index][3] << 1 | K_IN4;

    for (unsigned char i = 0; i < 4; i++) {
        if ((key_buf[key_out_index][i] & 0x0f) == 0x0f) {                      // 按键弹起
            key_status[key_out_index][i] = 1;
        } else if ((key_buf[key_out_index][i] & 0x0f) == 0x00) {              // 按键按下
            key_status[key_out_index][i] = 0;
        }
    }

    // 更新使能引脚
    key_out_index++;
    key_out_index &= 0x3;
    enable_k_out(key_out_index);
}