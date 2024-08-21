int timer_init(unsigned char timer);
unsigned char timer_setup(unsigned char timer, unsigned int ms);
unsigned char timer_start(unsigned char timer);
unsigned char timer_reset(unsigned char timer, unsigned int ms);
void timer_enable_int(unsigned char timer);