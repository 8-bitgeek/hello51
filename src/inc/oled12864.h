#include <8052.h>
#include "types.h"

#define OLED_ADDR       0x3c

// command value for following data byte type
#define DATA_MODE       0x40
#define CMD_MODE        0x00

// Addressing mode
#define ADDR_MODE_HOR   0x00                 // Horizontal addressing mode
#define ADDR_MODE_VER   0x01                 // Vertical addressing mode
#define ADDR_MODE_PAGE  0x02                 // Page addressing mode

#define I2C_SCL         P1_0
#define I2C_SDA         P1_1

#define SCK I2C_SCL
#define SDA I2C_SDA

extern enum FT_SIZE;

extern unsigned char oled_addr(void);
void oled_init(void);
void oled_clear(void);
void oled_addr_mode(u8 addr_mode);
void oled_print(u8 x, u8 y, u8 * str, u8 len, u8 font_size);
