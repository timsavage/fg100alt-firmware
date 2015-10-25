/*
 * lcd.c
 *
 *  Based on code by aostanin - https://github.com/aostanin/avr-hd44780
 */

#include <stdarg.h>
#include <stdio.h>
#include <util/delay.h>

#include "lcd.h"


void lcd_send(uint8_t value, uint8_t mode);
void lcd_write_nibble(uint8_t nibble);

static uint8_t lcd_displayparams;
#define BUFFER_SIZE 17
static char lcd_buffer[BUFFER_SIZE];

void lcd_command(uint8_t command) {
	lcd_send(command, 0);
}

void lcd_write(uint8_t value) {
	lcd_send(value, 1);
}

void lcd_send(uint8_t value, uint8_t mode) {
	if (mode) {
		LCD_CTRL_PORT = LCD_CTRL_PORT | (1 << LCD_RS);
	} else {
		LCD_CTRL_PORT = LCD_CTRL_PORT & ~(1 << LCD_RS);
	}
	lcd_write_nibble(value >> 4);
	lcd_write_nibble(value);
}

void lcd_write_nibble(uint8_t nibble) {
	nibble = (
		(nibble & 1) << 3 |
		(nibble & 2) << 1 |
		(nibble & 4) >> 1 |
		(nibble & 8) >> 3
	);
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xFF & ~(0x0F << LCD_LOW_BYTE)) | ((nibble & 0x0F) << LCD_LOW_BYTE);

	LCD_CTRL_PORT = LCD_CTRL_PORT & ~(1 << LCD_EN);
	LCD_CTRL_PORT = LCD_CTRL_PORT | (1 << LCD_EN);
	LCD_CTRL_PORT = LCD_CTRL_PORT & ~(1 << LCD_EN);
	_delay_ms(0.04);
}

void lcd_init(void) {
	// Configure control pins
	LCD_CTRL_DDR = LCD_CTRL_DDR
		| (1 << LCD_RS)
		| (1 << LCD_EN);

	// Configure data pins
	LCD_DATA_DDR = LCD_DATA_DDR | (0x0F << LCD_LOW_BYTE);

	// Wait for LCD
	_delay_ms(15);

	LCD_CTRL_PORT = LCD_CTRL_PORT
		& ~(1 << LCD_EN)
		& ~(1 << LCD_RS);

	_delay_ms(4.1);
	lcd_write_nibble(0x03);
	_delay_ms(4.1);
	lcd_write_nibble(0x03);
	_delay_ms(4.1);
	lcd_write_nibble(0x03);
	_delay_ms(4.1);
	lcd_write_nibble(0x02); // Set 4 bit

	lcd_command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS);

	lcd_displayparams = LCD_CURSOROFF | LCD_BLINKOFF | LCD_DISPLAYON;
	lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_on(void) {
  lcd_displayparams |= LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_off(void) {
  lcd_displayparams &= ~LCD_DISPLAYON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_clear(void) {
  lcd_command(LCD_CLEARDISPLAY);
  _delay_ms(2);
}

void lcd_return_home(void) {
  lcd_command(LCD_RETURNHOME);
  _delay_ms(2);
}

void lcd_enable_blinking(void) {
  lcd_displayparams |= LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_disable_blinking(void) {
  lcd_displayparams &= ~LCD_BLINKON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_enable_cursor(void) {
  lcd_displayparams |= LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_disable_cursor(void) {
  lcd_displayparams &= ~LCD_CURSORON;
  lcd_command(LCD_DISPLAYCONTROL | lcd_displayparams);
}

void lcd_scroll_left(void) {
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void lcd_scroll_right(void) {
  lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void lcd_set_left_to_right(void) {
  lcd_displayparams |= LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_set_right_to_left(void) {
  lcd_displayparams &= ~LCD_ENTRYLEFT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_enable_autoscroll(void) {
  lcd_displayparams |= LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_disable_autoscroll(void) {
  lcd_displayparams &= ~LCD_ENTRYSHIFTINCREMENT;
  lcd_command(LCD_ENTRYMODESET | lcd_displayparams);
}

void lcd_create_char(uint8_t location, uint8_t *charmap) {
	int i;
	lcd_command(LCD_SETCGRAMADDR | ((location & 0x7) << 3));
	for (i = 0; i < 8; i++) {
		lcd_write(charmap[i]);
	}
}

void lcd_set_cursor(uint8_t col, uint8_t row) {
  static uint8_t offsets[] = { 0x00, 0x40, 0x14, 0x54 };

  if (row > 1) {
    row = 1;
  }

  lcd_command(LCD_SETDDRAMADDR | (col + offsets[row]));
}

void lcd_puts(const char* string) {
	const char* it;
	for (it = string; *it; it++) {
    	lcd_write(*it);
	}
}

void lcd_printf(const char* format, ...) {
  va_list args;

  va_start(args, format);
  vsnprintf(lcd_buffer, BUFFER_SIZE, format, args);
  va_end(args);

  lcd_puts(lcd_buffer);
}
