#ifndef KEIL
extern unsigned char __code nixie_chars[16];
#else
extern unsigned char code nixie_chars[16];
#endif

extern unsigned char nixietube_buffer[6];
extern unsigned char nixietube_enable(unsigned char num_tube);
extern void nixietube_scan();
extern void nixie_show_number(unsigned long num);