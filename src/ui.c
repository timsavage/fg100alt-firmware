/*
 * ui.c
 *
 *  Created on: 25 Oct 2015
 *      Author: tims
 */

#include <avr/pgmspace.h>
#include "ui.h"
#include "lcd.h"

const uint32_t powersOfTen[] = { 1, 10, 100, 1000, 10000, 100000 };
const char* functionNames[] = {
	"Sign", "Triangle", "Sawtooth", "R Sawtooth", "Square"
};

void ui_show_splash(void) {
	lcd_clear();
	lcd_printf(PSTR("FG-100 Alt  v%01u.%01u"), VERSION_MAJOR, VERSION_MINOR);
	lcd_set_cursor(1, 1);
	lcd_puts(PSTR("savage.company"));
}


void ui_redraw_display(State* state) {
	lcd_clear();
	lcd_printf("Wave: %s", functionNames[state->function]);
	lcd_set_cursor(0, 1);
	lcd_printf("Freq: %06uHz", state->frequency);
	if (state->frequencyOffset >= 0) {
		lcd_set_cursor(11 - state->frequencyOffset, 1);
		lcd_enable_cursor();
	} else {
		lcd_disable_cursor();
	}
}


void ui_handle_event(uint8_t interface, uint8_t event, State* state) {
	uint8_t changed = 0;

	switch(interface) {
	case MODE_BUTTON:
		if (event == BUTTON_PRESS) {
			state->function = (state->function + 1) % 5;
			changed = 1;
		}
		break;

	case CURSOR_BUTTON:
		if (event == BUTTON_PRESS) {
			state->frequencyOffset = (state->frequencyOffset + 1) % 6;
			changed = 1;
		}
		break;

	case PLUS_BUTTON:
		if ((event == BUTTON_PRESS) | (event == BUTTON_REPEAT)) {
			state->frequency += powersOfTen[state->frequencyOffset];
			changed = 1;
		}
		break;

	case MINUS_BUTTON:
		if ((event == BUTTON_PRESS) | (event == BUTTON_REPEAT)) {

			state->frequency -= powersOfTen[state->frequencyOffset];

			changed = 1;
		}
		break;
	}

	if (changed) ui_redraw_display(state);
}
