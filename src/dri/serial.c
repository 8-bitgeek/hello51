#include "8051.h"
#include "kingst.h"

#ifndef KEIL
__bit serial_tx_end = 1;
__bit serial_rx_end = 0;
#else
bit serial_tx_end = 1;
bit serial_rx_end = 0;
#endif

unsigned char s_buf;

void serial_init(unsigned int baud) {
    // 1. 配置串口为模式 1
    SCON = 0x50;            // 0b 0101 - 0000: 模式 1, REN = 1(接收使能);
    // 2. 配置定时器 T1 为模式 2, 即自动重装模式
    TMOD &= 0x0F;
    TMOD |= 0x20;
    // 3. 根据波特率计算 TH1 和 TL1 的初值, 如果有需要可以使用 PCON 进行波特率加倍
    TH1 = 256 - (CRYSTYLE_FREQ / 12 / 32) / baud;
    TL1 = TH1;
    // 4. 打开定时器控制寄存器 TR1, 启动定时器
    ET1 = 0;                // 可以不写, 强调必须要关闭定时器 1 的中断, 不能再作它用.
    TR1 = 1;

    // 中断使能
    EA = 1;
    ES = 1;
}

/**
 * 实现 putchar 来支持标准库中的 printf 函数
 */
int putchar(int c) {
    while (!serial_tx_end);
    SBUF = c;
    serial_tx_end = 0;
    return 1;
}

#ifndef KEIL
void serial_isr(void) __interrupt(4) __using(4) {
#else
void serial_isr(void) interrupt 4 {
#endif
    if (RI) {
        RI = 0;
        serial_rx_end = 1;
    }
    else {
        TI = 0;
        serial_tx_end = 1;
    }
}