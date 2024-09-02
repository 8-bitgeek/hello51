void lcd_init(void);
void lcd_putchar(unsigned char x, unsigned char y, unsigned char c);
void lcd_write_dat(unsigned char dat);
void lcd_set_cursor(unsigned char x, unsigned char y);
void lcd_print(unsigned char * str);
void lcd_printf(unsigned char * str, char clear, unsigned char x, unsigned y);
extern unsigned char lcd_buf[32];
#ifndef KEIL
extern __bit lcd_rx_end;
extern __bit lcd_receieve(unsigned char ch);
#else
extern bit lcd_rx_end;
extern bit lcd_receieve(unsigned char ch);
#endif