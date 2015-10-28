#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <math.h>

#include "defines.h"
#include "lcd.h"
#include "ui.h"
#include "dds.h"


void init(void) {
	wdt_disable();

	// Initialize the LCD and display splash message
	lcd_init();
	ui_show_splash();

	dds_init();

	// Configure strobe pin
	DDRB = DDRB & ~_BV(BUTTON_STROBE);

	// Configure interrupt for Start/Stop button
	DDRC  &= ~_BV(PINC3);
	PORTC |= _BV(PINC3);  // Enable pull up resistor

	// Enable interrupts
	sei();
}

void process_button_event(uint8_t button, uint8_t state) {
	static uint8_t button_status[BUTTON_COUNT];

	if (state) {
		if (button_status[button]) {
			ui_handle_event(button, BUTTON_REPEAT);
		} else {
			ui_handle_event(button, BUTTON_PRESS);
		}
		button_status[button] = BUTTON_DOWN;
	} else {
		if (button_status[button]) {
			ui_handle_event(button, BUTTON_RELEASE);
		}
		button_status[button] = BUTTON_UP;
	}
	_delay_ms(10);
}

/**
 * Check status of strobe pins
 */
void check_strobe_pin(uint8_t button, uint8_t pin) {
	PORTB = (PORTB & 0xFF & ~(0x0F << PORTB2)) | _BV(pin);
	_delay_ms(1);
	process_button_event(button, PINB & _BV(BUTTON_STROBE));
}

/**
 * Generate UI events
 */
void generate_ui_events() {
	process_button_event(RUN_STOP_BUTTON, !(PINC & _BV(RUN_STOP_PIN)));
	check_strobe_pin(MODE_BUTTON, MODE_PIN);
	check_strobe_pin(CURSOR_BUTTON, CURSOR_PIN);
	check_strobe_pin(PLUS_BUTTON, PLUS_PIN);
	check_strobe_pin(MINUS_BUTTON, MINUS_PIN);
	_delay_ms(50);
}

void loop(void) {
	while(1) {
		generate_ui_events();

		if (DDS_IS_ENABLED) {
			lcd_disable_cursor();
			dds_start(ui_state.frequency);
			lcd_enable_cursor();
			DDS_DISABLE;
		}
	}
}

int main(void) {
	init();
	_delay_ms(2000);
	ui_redraw_display();

	loop();
	return 1;
}
