#ifndef __KINGST_H__
#define __KINGST_H__

#define CRYSTYLE_FREQ 11059200
#define K_CON P2

#ifndef KEIL

#include <8051.h>
// LED display
#define ENLED   P1_4

// 74h138
#define ADDR3   P1_3
#define ADDR2   P1_2
#define ADDR1   P1_1
#define ADDR0   P1_0

// Keyboard definies
#define K_OUT1  P2_3
#define K_OUT2  P2_2
#define K_OUT3  P2_1
#define K_OUT4  P2_0
#define K_IN1   P2_4
#define K_IN2   P2_5
#define K_IN3   P2_6
#define K_IN4   P2_7

// Normal LED
#define N_LED0  P0_0
#define N_LED1  P0_1
#define N_LED2  P0_2
#define N_LED3  P0_3
#define N_LED4  P0_4
#define N_LED5  P0_5
#define N_LED6  P0_6
#define N_LED7  P0_7

#else
#include <reg52.h>
// LED display
#define ENLED   P1^4

// 74h138
#define ADDR3   P1^3
#define ADDR2   P1^2
#define ADDR1   P1^1
#define ADDR0   P1_0

// Keyboard definies
#define K_OUT1  P2^3
#define K_OUT2  P2^2
#define K_OUT3  P2^1
#define K_OUT4  P2^0
#define K_IN1   P2^4
#define K_IN2   P2^5
#define K_IN3   P2^6
#define K_IN4   P2^7

// Normal LED
#define N_LED0  P0^0
#define N_LED1  P0^1
#define N_LED2  P0^2
#define N_LED3  P0^3
#define N_LED4  P0^4
#define N_LED5  P0^5
#define N_LED6  P0^6
#define N_LED7  P0^7
#endif

#define _nop_() __asm nop __endasm


#endif