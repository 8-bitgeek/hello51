#include <8052.h>
#include <stdio.h>
#include "kingst.h"
#include "inc/serial.h"
#include "ir.h"
#include "motor.h"

unsigned char * identify_key();

void main(void) {
    serial_init(9600);
    ir_init(0);                     // use timer0 to work with serial

    while (1) {
        if (serial_rx_end) {
            motor_stop_flag = 0;
            serial_rx_end = 0;
            unsigned char code = SBUF;
            __bit dir;
            printf_small("rec code: %c\n", code);
            if (code == 0x01) {
                motor_turn(1, 180);
            } else if (code == 0x02) {
                motor_turn(0, 180);
            } else if (code == 0x03) {
                motor_stop_flag = 1;
            }
        }
    }
}

unsigned char * identify_key() {
    unsigned char * code;
    switch (ir_code[2]) {
        case 69:
            code = "1";
            break;
        case 70:
            code = "2";
            break;
        case 71:
            code = "3";
            break;
        case 68:
            code = "4";
            break;
        case 64:
            code = "5";
            break;
        case 67:
            code = "6";
            break;
        case 7:
            code = "7";
            break;
        case 21:
            code = "8";
            break;
        case 9:
            code = "9";
            break;
        case 22:
            code = "*";
            break;
        case 25:
            code = "0";
            break;
        case 13:
            code = "#";
            break;
        case 8:
            code = "LEFT";
            break;
        case 24:
            code = "UP";
            break;
        case 90:
            code = "RIGHT";
            break;
        case 28:
            code = "OK";
            break;
        case 82:
            code = "DOWN";
            break;
        default:
            code = "UNKNOWN";
            printf_small("Unknown KEY pressed!\n");
            break;
    }
    return code;
}