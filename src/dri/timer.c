#include "8051.h"
#include "kingst.h"

/** __interrupt(1) - timer0 __interrupt(3) - timer1 */

static __bit is_0_init = 0;                                     // 是否初始化完�??
static __bit is_1_init = 0;                                     // 是否初始化完�??

/**
 * timer: 要初始化的定时器, value: 0/1
 * 返回 -1 表示设置出错.
 */
int timer_init(unsigned char timer) {
    if (timer == 0) {
        TMOD = TMOD | 0b00000001 & 0b11110001;          // �? 4 位设置为 0b0001, 工作模式 1: 16 位计数器
    } else if (timer == 1) {
        TMOD = TMOD | 0b00010000 & 0b00011111;          // �? 4 位设置为 0b0001, 工作模式 1: 16 位计数器
    } else {
        return -1;
    }
    return 0;
}

/**
 * 启动定时�?
 * 返回: -1 - 未初始化或配置错�?
 */
unsigned char timer_start(unsigned char timer) {
    // 参数不对或者未初始化则返回错误
    if ((timer != 0 && timer != 1) || (timer == 0 && !is_0_init) || (timer == 1 && !is_1_init)) {
        return 0;
    } else if (timer == 0) {
        TR0 = 1;
    } else if (timer == 1) {
        TR1 = 1;
    }     
    return 1;
}

/**
 * 定时器中断使�?
 */
void timer_enable_int(unsigned char timer) {
    switch (timer) {
        case 0:
            ET0 = 1;
            break;
        case 1:
            ET1 = 1;
            break;
        default:
            break;
    }
}

/**
 * 设置定时时间
 */
unsigned char timer_setup(unsigned char timer, unsigned int ms) {
    // 1ms 需�? 921 个计数周�?
    unsigned int one_ms_cnt = CRYSTYLE_FREQ / 12000 - 1;
    unsigned int init_num = 0xffff - (ms * one_ms_cnt);
    if (timer == 0) {
        TH0 = init_num >> 8 & 0xff;
        TL0 = init_num & 0xff;                         // 分别设置定时器初值高位和低位数据
        is_0_init = 1;
    } else if (timer == 1) {
        TH1 = init_num >> 8 & 0xff;
        TL1 = init_num & 0xff;                         // 分别设置定时器初值高位和低位数据
        is_1_init = 1;
    } else {
        return 0;
    }
    return 1;
}


/**
 * 重置计数�?
 */
unsigned char timer_reset(unsigned char timer, unsigned int ms) {
    return timer_setup(timer, ms);
}