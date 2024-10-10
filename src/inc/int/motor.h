#include "kingst.h"

#ifndef KEIL
void motor_turn(__bit dir, unsigned long angle);
extern __bit motor_stop_flag;
#else
void motor_turn(bit dir, unsigned long angle);
extern bit motor_stop_flag;
#endif