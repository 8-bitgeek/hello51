#include "kingst.h"
#include <stdio.h>

__bit motor_stop_flag = 0;
/** @param dir: 1 - anticlockwise 0 - clockwise */
#ifndef KEIL
void motor_turn(__bit dir, unsigned long angle) {
    unsigned char __code beat_code[8] = {
#else
void motor_turn(bit dir, unsigned long angle) {
    unsigned char code beat_code[8] = {
#endif
        0x0e, 0x0c, 0x0d, 0x09, 0x0b, 0x03, 0x07, 0x06
    };

    unsigned char tmp;
    unsigned char index = 0;
    unsigned long beats = 0;
    beats = (angle * 4076) / 360;           // 4076(4096 in theory) means 1 round
    
    while (beats-- && !motor_stop_flag) {
        if (dir) {
            index++;
            index &= 0x07;
        } else {
            index--;
            index &= 0x07;
        }
        tmp = P1;
        tmp = tmp & 0xf0;
        tmp |= beat_code[index];
        P1 = tmp;
        for (int i = 200; i >= 0; i--);       // delay 200ms
    }
    P1 = P1 | 0x0f;
}