#include "inc/serial.h"
#include "kingst.h"
#include <8051.h>
// #include "lcd1602.h"
#include "timer.h"
#include <stdio.h>
#include "nixietube.h"
// #include "keyboard.h"
#include "ir.h"

void main(void) {
    EA = 1;
    timer_init(0);
    timer_setup(0, 1);
    timer_start(0);
    timer_enable_int(0);
    serial_init(9600);
    nixietube_enable(0);
    // lcd_init();
    // while (1) {
    //     for (unsigned int i = 0; i < 4; i++) {
    //         for (unsigned int j = 0; j < 4; j++) {
    //             if (key_status_backup[i][j] != key_status[i][j]) { 
    //                 if (key_status_backup[i][j] == 0) {             // 弹起
    //                     // key_action(key_code_map[i][j]);
    //                     printf_small("i = %d, j = %d\n", i, j);
    //                     printf_small("key press: [%d]\n", key_code_map[i][j]);
    //                     // lcd_print("this is keyboard test");
    //                 }
    //                 key_status_backup[i][j] = key_status[i][j];     // 更新
    //             }
    //         }
    //     }
    // }
}

void serial_isr(void) __interrupt(4) __using(4) {
    if (RI) {
        RI = 0;
        // lcd_receieve(SBUF);
    }
    else {
        TI = 0;
        // tx_end = 1;
    }
}

void timer0_isr(void) __interrupt(1) __using(1) {
    timer_reset(0, 1);
    // keyboard_scan();
}