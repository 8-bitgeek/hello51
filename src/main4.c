// #include "inc/serial.h"
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
    // serial_init(9600);
    // nixietube_enable(0);
    ir_init();
    PT0 = 1;                                // 閰嶇�? T0 涓柇涓洪珮浼樺厛绾�?, 瑙ｅ喅澶栭儴涓柇鎺ユ敹鏁版嵁鏃跺欢鏃跺瀹氭椂鍣�? 0 鐨勫奖鍝�?
    // printf("ir initialize....");
    while (1) {
        if (ir_flag) {
            ir_flag = 0;
            // nixietube_buffer[5] = nixie_chars[ir_code[0] >> 4];         // 鐀��埛鐮佹樉绀�?
            // nixietube_buffer[4] = nixie_chars[ir_code[0] & 0x0f];
            // nixietube_buffer[2] = nixie_chars[ir_code[2] >> 4];         // 閿爜鏄剧ず
            // nixietube_buffer[1] = nixie_chars[ir_code[0] & 0x0f];
            nixietube_buffer[5] = nixie_chars[ir_code[0] >> 4];         // 鐀��埛鐮佹樉绀�?
            nixietube_buffer[4] = nixie_chars[ir_code[0] & 0x0f];
            nixietube_buffer[1] = nixie_chars[ir_code[2] >> 4];         // 閿爜鏄剧ず
            nixietube_buffer[0] = nixie_chars[ir_code[2] & 0x0f];
        }
    }
}

// void serial_isr(void) __interrupt(4) __using(4) {
//     if (RI) {
//         RI = 0;
//         // lcd_receieve(SBUF);
//     }
//     else {
//         TI = 0;
//         tx_end = 1;
//     }
// }

void timer0_isr(void) __interrupt(1) __using(1) {
    timer_reset(0, 1);
    nixietube_scan();
}