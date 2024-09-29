#include "i2c.h"
#include "oled12864.h"
#include <stdio.h>

// #define FS_8X8          0x0                 // common 8, segment 8
// #define FS_8X16         0x1                 // common 8, segment 16  
// #define FS_16X16        0x2

enum FT_SIZE {
    FS_8X8 = 0x11, FS_8X16 = 0x12, FS_16X16 = 0x22
};

/** 
 * check if oled device exist.
 * Return: 1 - exist; 0 - not exist.
 */
unsigned char oled_addr(void) {
    unsigned char is_exits = i2c_write_byte(OLED_ADDR << 1);
    return is_exits;
}

/**
 * send control byte to oled.
 */
void write_Control_byte(u8 mode) {
    i2c_write_byte(mode);
}


/**
 * write data to oled.
 * @param dat: data write to oled
 * @param len: data length to write
 */
void oled_write_data(u8 * dat, u8 len) {
    i2c_start();
    oled_addr();
    write_Control_byte(DATA_MODE);
    for (u8 i = 0; i < len; i++) {
        i2c_write_byte(*dat);
    }
    i2c_stop();
}

/**
 * write commands to oled.
 * @param dat: commands write to oled
 * @param len: commands length to write
 */
void oled_write_cmd(u8 * cmd, u8 len) {
    i2c_start();
    oled_addr();
    write_Control_byte(CMD_MODE);
    for (u8 i = 0; i < len; i++) {
        i2c_write_byte(*cmd++);
    }
    i2c_stop();
}

/**
 * initialize oled.
 */
void oled_init(void) {
    u8 cmds[] = {
        // turn off display
        0xAE,
        // remap common
        0xC8,
        // remap segment
        0xA1,
        // charge pump
        0x8D,
        0x14,
        // turn on display
        0xAF
    };
    oled_write_cmd(cmds, 6);
}

/**
 * clear screen
 */
void oled_clear(void) {
    u8 cmd[] = {0x20, 0x00};
    oled_write_cmd(cmd, 2);                                 // setup addressing mode: horizontal
    u8 data[8] = {0};
    for (u8 i = 0; i < 128; i++) {
        oled_write_data(data, 8);
    }
}

/**
 * set addressing mode 
 */
void oled_addr_mode(u8 addr_mode) {
    u8 cmd[2] = {0x20, addr_mode};
    oled_write_cmd(cmd, 2);
}

/**
 * setup page addressing mode env
 * @param col: column start address
 * @param pag: page start address
 */
static void oled_set_page_pointer(u8 pag, u8 col) {
    u8 page_setup_cmd[3] = {
        0xB0 | pag,              // page start address
        0x00 | col & 0x0F,       // page addressing mode lower nibble of the column
        0x10 | col >> 4,         // higher nibble of the column
    };
    // printf_tiny("page: 0x%x, high_col: 0x%x, low_col: 0x%x\n", page_setup_cmd[0], page_setup_cmd[2], page_setup_cmd[1]);
    oled_write_cmd(page_setup_cmd, sizeof(page_setup_cmd));
}

void oled_print(u8 x, u8 y, u8 * str, u8 len, enum FT_SIZE font_size) {
    oled_addr_mode(ADDR_MODE_PAGE);
    u8 pag = y * (font_size & 0x0F);                                // calculate page num according to font size lower nibble
    u8 col = x * (font_size >> 4) * 8;                              // cal col num
    u8 pag_c = font_size & 0x0F;                                    // cal number of page to use
    u8 col_c = ((font_size >> 4) << 3) * len;                       // cal number of column to use

    // according to cal result then write data to screen ram.
    for (u8 i = 0; i < pag_c; i++) {
        oled_set_page_pointer(pag + i, col);                        // set page number for page addressing mode
        for (u8 j = 0; j < col_c; j++) {
            oled_write_data(str++, 1);
        }
    }
}
