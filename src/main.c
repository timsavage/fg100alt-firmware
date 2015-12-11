#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <math.h>

#include "defines.h"
#include "lcd.h"
#include "ui.h"
#include "dds.h"


void init_mcu(void) {
	wdt_disable();

	// Initialize the LCD and display splash message
	lcd_init();

	ui_show_splash();

	// UI/DDS initialize
	ui_init();
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
	uint8_t* btn = &button_status[button];

	// Append state to history
	*btn = *btn << 1;
	*btn |= (state != 0);

	if ((*btn & 0xC7) == 0x07) {
		*btn = 0xFF;
		ui_handle_event(button, BUTTON_PRESS);
	} else if ((*btn & 0xC7) == 0xC0) {
		*btn = 0;
		ui_handle_event(button, BUTTON_RELEASE);
	}

//	if (*btn == 0xFF) {
//		ui_handle_event(button, BUTTON_REPEAT);
//	}
}


/**
 * Check status of strobe pins
 */
void check_strobe_pin(uint8_t button, uint8_t pin) {
	// Turn on one of the 4 pins
	PORTB = (PORTB & 0xFF & ~(0x0F << PORTB2)) | _BV(pin);
	_delay_ms(1);
	// Process events
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
	_delay_ms(10);
}


void loop(void) {
	while(1) {
		generate_ui_events();

		if (DDS_IS_ENABLED) {
<<<<<<< HEAD
			ui_show_fixed();
=======
			lcd_disable_cursor();
			_delay_ms(50);  // Hack to ensure button is released
>>>>>>> heads/release-0.1
			dds_start(ui_state.frequency);
//			dds_start_sweep(ui_state.frequency);
			DDS_DISABLE;
			ui_draw_display();
		}
	}
}


int main(void) {
	init_mcu();

	_delay_ms(2000);
	ui_draw_display();

	loop();
	return 1;
}
