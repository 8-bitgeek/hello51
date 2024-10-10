#ifndef KEIL
extern __bit ir_flag;
void ir_isr(void) __interrupt(2) __using(2);
#else
extern bit ir_flag;
void ir_isr(void) interrupt 2;                      // interrupt routine must declare here otherwise it's doesn't work.
#endif

extern void ir_init(unsigned char timer);                          // initialization
extern unsigned char ir_code[4];                    // infrared ray receieve buffer