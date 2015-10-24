#include <util/delay.h>
#include <avr/wdt.h>
#include <math.h>

#include "defines.h"
#include "waveforms.h"

#include "lcd.h"
#include "dac.h"

const uint8_t c_lcdMode    = 0b00111110;
const uint8_t c_buttonMode = 0b00000010;

uint8_t g_function = 0;
uint8_t g_adjustOffset = 3;
uint32_t g_freq = 500;

int main(void);
void loop(void);

void init(void) {
	wdt_disable();

	// Initialize the LCD and display splash message
	lcd_init();
	lcd_clear();
	lcd_puts("FG-100 Alt  v0.1");
	lcd_set_cursor(1, 1);
	lcd_puts("savage.company");

	dac_init();

	// Configure strobe pin
	DDRB = DDRB & ~_BV(BUTTON_STROBE);
}

void update_lcd() {
	lcd_clear();
	lcd_printf("Wave: Sign");
	lcd_set_cursor(0, 1);
	lcd_printf("Freq: %06uHz", g_freq);
	if (g_adjustOffset >= 0) {
		lcd_set_cursor(12 - g_adjustOffset, 1);
		lcd_enable_blinking();
		lcd_enable_cursor();
	} else {
		lcd_disable_blinking();
		lcd_disable_cursor();
	}
}

uint8_t check_button(uint8_t pin) {
	_delay_ms(50);
	PORTB = (PORTB & 0xFF & ~(0x0F << PORTB2)) | _BV(pin);
	_delay_ms(10);
	return PINB & _BV(PINB1);
}

void handle_buttons() {
	uint8_t changed = 0;

	if (check_button(MODE_PIN)) {
		g_function = (g_function + 1) % 5;
		changed = 1;
	}

	if (check_button(CURSOR_PIN)) {
		g_adjustOffset = (g_adjustOffset + 1) % 6;
		changed = 1;
	}

	if (check_button(PLUS_PIN)) {
		g_freq += (10 ^ g_adjustOffset);
		changed = 1;
	}

	if (check_button(MINUS_PIN)) {
		g_freq -= (10 ^ g_adjustOffset);
		changed = 1;
	}

	if (changed) {
		update_lcd();
	}
}

int main(void) {
	init();

	dac_set_freq(500);
	dac_set_waveform(waves[0]);

	_delay_ms(2000);

	update_lcd();
	loop();

	return 1;
}

void loop(void) {
	while(1) {
		handle_buttons();
	}

	//	dac_start();

}
