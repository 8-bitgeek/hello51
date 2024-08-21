#include "8051.h"
#include "kingst.h"
#include "timer.h"
#include "led.h"
#include "keyboard.h"
#include "nixietube.h"

__bit rxd_end = 0;
__bit txd_end = 0;
__bit rxd_or_txd = 0;                   // 0 - 接收状态, 1 - 发送状态
unsigned char rxd_buf;
unsigned char txd_buf;

void config_uart(unsigned int baud) {
    TMOD &= 0xf0;
    TMOD |= 0x02;                       // 定时器 0, 工作模式为 8 位自动重载定时器
    TH0 = 256 - (11059200/12)/baud;     // 重载值
}
void start_rxd() {
    TL0 = 256 - ((256 - TH0) >> 1);     // 除以 2 表示从一个信号的中间读取数据
    ET0 = 1;
    TR0 = 1;
    rxd_end = 0;
    rxd_or_txd = 0;                     // 接收状态
}

void start_txd(unsigned char dat) {
    txd_buf = dat;
    TL0 = TH0;
    ET0 = 1;
    TR0 = 1;
    TXD = 0;
    txd_end = 0;
    rxd_or_txd = 1;
}

void main(void) {

    EA = 1;

    config_uart(9600);

    while (1) {
        while (RXD);                // 高电平时为空闲状态, 有信号输入时, 起始位为低电平
        start_rxd();
        while (!rxd_end);
        start_txd(rxd_buf + 1);
        while (!txd_end);
    }
}

void timer0_isr(void) __interrupt(1) __using(1) {
    static unsigned char cnt = 0;
    if (rxd_or_txd) {               // 发送状态
        if (++cnt <= 8) {
            TXD = txd_buf & 0x01;
            txd_buf >>= 1;
        } else if (cnt == 9)  {     // 发送停止位 
            TXD = 1;
        } else {                    // 停止位发送完毕
            cnt = 0;
            TR0 = 0;                // 关闭定时器
            txd_end = 1;            // 发送完毕
        }
    } else {                        // 接收状态
        if (cnt == 0) {             // 起始位
            if (!RXD) {             // 确认一下是不是起始位
                rxd_buf = 0;
                ++cnt;
            } else {
                TR0 = 0;            // 关闭定时器
            }
        } else if (cnt <= 8) {      // 
            rxd_buf >>= 1;
            if (RXD) {
                rxd_buf |= 0x80;    // 高位补 1
            }
            cnt++;
        } else {                    // 停止位
            cnt = 0;
            TR0 = 0;
            if (RXD) {
                rxd_end = 1;        // 接收结束
            }
        }
    }
}