#include "kingst.h"
#include <stdio.h>
#include "motor.h"

unsigned char ir_code[4];                   // infrared ray receieve buffer

#ifndef KEIL
__bit ir_flag = 0;
__bit ir_timer = 1;
// infrared receieve pin using extern interrupt 1
#define IR_INPUT P3_3
#else
bit ir_flag = 0;
bit ir_timer = 1;
#define IR_INPUT P3^3
#endif

// void ir_init(void) {
void ir_init(unsigned char timer) {
    printf_small("enable timer %d\n", timer);
    IR_INPUT = 1;
    ir_timer = timer ? 1 : 0;
    if (ir_timer == 0) {
        TMOD &= 0xF0;                           // config timer 0 work on mode-1
        TMOD |= 0x01;
        TR0 = 0;
        ET0 = 0;                                // disable timer0 interruption
        IT0 = 1;                                // how to trigger extern interruption
        EX1 = 1;                                // enable extern interrupt 1 
        EA = 1;
    } else if (ir_timer == 1) {
        TMOD &= 0x0f;                           // using timer1
        TMOD |= 0x10;
        TR1 = 0;
        ET1 = 0;
        IT1 = 1;                                // falling edge to trigger external interrupt
        EX1 = 1;                                // enable external interrupt 1
        EA = 1;
    }
}

unsigned int get_high_time() {
    if (ir_timer) {
        TH1 = 0;
        TL1 = 0;
        TR1 = 1;
        while (IR_INPUT) {                      // waitting for logic high end
            if (TH1 > 0x40) {break;}            // 如果等待时间超过 18ms, 则不再等�?
        }                       
        TR1 = 0;
        return (TH1 * 256 + TL1);
    } else {
        TH0 = 0;
        TL0 = 0;
        TR0 = 1;
        while (IR_INPUT) {                      // 高电平持续等�?
            if (TH0 > 0x40) {break;}            // 如果等待时间超过 18ms, 则不再等�?
        }                       
        TR0 = 0;
        return (TH0 * 256 + TL0);
    }
}

unsigned int get_low_time() {
    if (ir_timer) {
        TH1 = 0;
        TL1 = 0;
        TR1 = 1;
        while (!IR_INPUT) {                     // 低电平持续等�?
            if (TH1 > 0x40) {break;}            // 如果等待时间超过 18ms, 则不再等�?
        }                       
        TR1 = 0;
        return (TH1 * 256 + TL1);
    } else {
        TH0 = 0;
        TL0 = 0;
        TR0 = 1;
        while (!IR_INPUT) {                     // 低电平持续等�?
            if (TH0 > 0x40) {break;}            // 如果等待时间超过 18ms, 则不再等�?
        }                       
        TR0 = 0;
        return (TH0 * 256 + TL0);
    }
}

/** 外部中断 1 例程, 中断�? IE1 硬件�? 1, 进入中断后硬件置 0 */
void ir_isr(void) __interrupt(2) __using(2) {
    unsigned int time;
    unsigned char byt = 0;

    // 判断低电平是否为 9ms
    time = get_low_time();
    if (time < 7833 || time > 8755) {           // lt 8.5ms or gt 9.5ms
        return;                                 // wrong signal and return
    }

    // 判断高电平是否为 4.5ms
    time = get_high_time();
    if (time < 3686 || time > 4608) {           // lt 4ms or gt 5ms
        return;                                 // wrong signal and return
    }

    // signal right and receieve 4 bytes code
    for (unsigned char i = 0; i < 4; i++) {
        for (unsigned char j = 0; j < 8; j++) {
            time = get_low_time();
            if (time < 313 || time > 718) {    // 340us - 780us
                return;
            }
            time = get_high_time();
            if (time > 313 && time < 718) {                 // bit 0: 560us 低电�? + 560us 高电�?
                byt >>= 1;
            } else if (time > 1345 && time < 1751) {        // bit 1: 560us 低电�? + 1.68ms 高电�?
                byt >>= 1;
                byt |= 0x80;
            } else {
                // IE1 = 0;
                return;
            }
        }
        ir_code[i] = byt;
    }
    motor_stop_flag = 1;
    ir_flag = 1;
    // IE1 = 0;
}