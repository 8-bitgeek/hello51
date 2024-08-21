#include "8052.h"
#include <stdio.h>

#ifndef KEIL
#define LCD1602_DB P0
#define LCD1602_RS P1_0
#define LCD1602_RW P1_1
#define LCD1602_E P1_5
#else
#define LCD1602_DB P0
#define LCD1602_RS P1^0
#define LCD1602_RW P1^1
#define LCD1602_E P1^5
#endif


unsigned char lcd_buf[32] = {};

#ifndef KEIL
__bit lcd_rx_end = 0;
#else
bit lcd_rx_end = 0;
#endif

static unsigned char x = 0, y = 0;
/**
 * 等待 lcd1602 可用
 */
void wait_for_idle(void) {
    unsigned char stat;
    LCD1602_DB = 0xFF;              // 读之前必须要�? io 口拉高为高电�?
    LCD1602_RS = 0;
    LCD1602_RW = 1;
    do {
        LCD1602_E = 1;
        stat = LCD1602_DB;
        LCD1602_E = 0;
    } while (stat & 0x80);           // 最高位�? 0 表示可用
}

void lcd_write_cmd(unsigned char cmd) {
    wait_for_idle();
    LCD1602_RS = 0;
    LCD1602_RW = 0;
    LCD1602_DB = cmd;
    LCD1602_E = 1;
    LCD1602_E = 0;
}

void lcd_write_dat(unsigned char dat) {
    wait_for_idle();
    LCD1602_RS = 1;
    LCD1602_RW = 0;
    LCD1602_DB = dat;
    LCD1602_E = 1;
    LCD1602_E = 0;
}

void lcd_init(void) {
    lcd_write_cmd(0x38);                        // 16 * 2 显示, 5*7 点阵, 8 位数据接�?
    lcd_write_cmd(0x0c);                        // 显示器开, 光标关闭
    lcd_write_cmd(0x06);                        // 文字不动, 地址自动 +1
    lcd_write_cmd(0x01);                        // 清屏
}

void lcd_set_cursor(unsigned char x, unsigned char y) {
    unsigned char addr;
    if (y == 0) {                               // 得到 lcd1602 �? ram 地址
        addr = 0x00 + x;
    } else {
        addr = 0x40 + x;
    }
    lcd_write_cmd(addr | 0x80);
}


// void lcd_print(unsigned char x, unsigned char y, unsigned char * str) {
//     lcd_set_cursor(x, y);
//     while (*str != '\0') {
//         lcd_write_dat(*str++);
//     }
// }

void lcd_putchar(unsigned char x, unsigned char y, unsigned char c) {
    lcd_set_cursor(x, y);
    printf("x = %d, y = %d, c = %c\n", x, y, c);
    lcd_write_dat(c);
}

void lcd_print(unsigned char * str) {
    lcd_write_cmd(0x01);                        // 清屏
    x = 0; y = 0;
    while (*str != '\0' && x + y * 16 <= 32) {
        if (x == 17) {
            y = 1;
            x = 0;
        }
        lcd_putchar(x++, y, *str++);
    }
}

#ifndef KEIL
__bit lcd_receieve(unsigned char ch) {
#else
bit lcd_receieve(unsigned char ch) {
#endif
    static unsigned char index;
    lcd_buf[index++] = ch;
    if (ch == '\n') {
        lcd_buf[--index] = '\0';
        index = 0;
        lcd_rx_end = 1;
        return 1;
    }
    return 0;
}