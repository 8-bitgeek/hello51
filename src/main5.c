#include <8051.h>
#include <stdio.h>
#include "kingst.h"
#include "inc/serial.h"
// #include "lcd1602.h"
#include "timer.h"
// #include "nixietube.h"
// #include "keyboard.h"
#include "ir.h"
#include "motor.h"

__bit time200ms_flag = 0;

void main(void) {
    EA = 1;
    ir_init();
    // serial_init(9600);
    while (1) {
        if (ir_flag) {
            ir_flag = 0;
            // printf_small("%d", ir_code[2]);
            if (ir_code[2]) {
                // printf_small("clockwise...\n");
                motor_turn(1, 180);
            } else {
                // printf_small("anticlockwise...\n");
                motor_turn(0, 180);
            }
        }
    }
}
