/*
 * lcd.h
 *
 *  Based on code by aostanin - https://github.com/aostanin/avr-hd44780
 */

#ifndef LCD_H_
#define LCD_H_

#include "defines.h"

// LCD Commands
#define LCD_CLEARDISPLAY   0x01
#define LCD_RETURNHOME     0x02
#define LCD_ENTRYMODESET   0x04
#define LCD_DISPLAYCONTROL 0x08
#define LCD_CURSORSHIFT    0x10
#define LCD_FUNCTIONSET    0x20
#define LCD_SETCGRAMADDR   0x40
#define LCD_SETDDRAMADDR   0x80

// Entry mode flags
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// Display control flags
#define LCD_DISPLAYON  0x04
#define LCD_DISPLAYOFF 0x00
#define LCD_CURSORON   0x02
#define LCD_CURSOROFF  0x00
#define LCD_BLINKON    0x01
#define LCD_BLINKOFF   0x00

// Cursor position flags
#define LCD_DISPLAYMOVE 0x08
#define LCD_CURSORMOVE  0x00
#define LCD_MOVERIGHT   0x04
#define LCD_MOVELEFT    0x00

// Function set flags
#define LCD_8BITMODE 0x10
#define LCD_4BITMODE 0x00
#define LCD_2LINE    0x08
#define LCD_1LINE    0x00
#define LCD_5x10DOTS 0x04
#define LCD_5x8DOTS  0x00

/**
 * Initialize LCD
 */
void lcd_init(void);

/**
 * Send a command to the LCD
 */
void lcd_command(uint8_t command);

/**
 * Write a character to the LCD
 */
void lcd_write(uint8_t value);

void lcd_clear(void);
void lcd_return_home(void);

void lcd_enable_cursor(void);
void lcd_disable_cursor(void);

void lcd_set_cursor(uint8_t col, uint8_t row);

void lcd_scroll_left(void);
void lcd_scroll_right(void);

void lcd_set_left_to_right(void);
void lcd_set_right_to_left(void);

void lcd_enable_autoscroll(void);
void lcd_disable_autoscroll(void);

void lcd_create_char(uint8_t location, uint8_t* charmap);

void lcd_puts(const char *string);
void lcd_printf(const char *format, ...);

#endif /* LCD_H_ */
