#ifndef KEIL
void serial_isr(void) __interrupt(4) __using(4);            // interrupt routine must declare here like this!
extern __bit serial_tx_end;
extern __bit serial_rx_end;
#else
void serial_isr(void) interrupt 4;
extern bit serial_tx_end;
extern bit serial_rx_end;
#endif

extern unsigned char s_buf;
void serial_init(unsigned int baud);
// void putchar(unsigned char c);
int putchar(int c);
void print(char * str);