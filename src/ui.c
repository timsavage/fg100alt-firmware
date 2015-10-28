/*
 * ui.c
 *
 *  Created on: 25 Oct 2015
 *      Author: tims
 */

#include "ui.h"
#include "lcd.h"
#include "dds.h"

const uint32_t powersOfTen[] = { 1, 10, 100, 1000, 10000, 100000 };

UIState ui_state = {
	500,	// Frequency
	2,		// Frequency offset
	0,		// Wave
};

void ui_show_splash(void) {
	lcd_clear();
	lcd_printf("FG-100 Alt  v%01u.%01u", VERSION_MAJOR, VERSION_MINOR);
	lcd_set_cursor(1, 1);
	lcd_puts("savage.company");
}

void ui_redraw_display() {
	lcd_clear();
	lcd_printf("Wave: %s", dds_wave_names[ui_state.wave_form]);
	lcd_set_cursor(0, 1);
	lcd_printf("Freq: %05uHz", ui_state.frequency);
	if (ui_state.frequency_offset >= 0) {
		lcd_set_cursor(10 - ui_state.frequency_offset, 1);
		lcd_enable_cursor();
	} else {
		lcd_disable_cursor();
	}
}

void ui_handle_event(uint8_t interface, uint8_t event) {
	uint8_t changed = 0;

	switch(interface) {
	case MODE_BUTTON:
		if (event == BUTTON_PRESS) {
			ui_state.wave_form = (ui_state.wave_form + 1) % 5;
			changed = 1;
		}
		break;

	case CURSOR_BUTTON:
		if (event == BUTTON_PRESS) {
			ui_state.frequency_offset = (ui_state.frequency_offset + 1) % 5;
			changed = 1;
		}
		break;

	case PLUS_BUTTON:
		if ((event == BUTTON_PRESS) | (event == BUTTON_REPEAT)) {
			uint32_t frequency = ui_state.frequency + powersOfTen[ui_state.frequency_offset];
			if (DDS_FREQ_IN_RANGE(frequency)) {
				ui_state.frequency = frequency;
				changed = 1;
			}
		}
		break;

	case MINUS_BUTTON:
		if ((event == BUTTON_PRESS) | (event == BUTTON_REPEAT)) {
			uint32_t frequency = ui_state.frequency - powersOfTen[ui_state.frequency_offset];
			if (DDS_FREQ_IN_RANGE(frequency)) {
				ui_state.frequency = frequency;
				changed = 1;
			}
		}
		break;

	case RUN_STOP_BUTTON:
		if (event == BUTTON_PRESS) {
			dds_select_wave(ui_state.wave_form);
			DDS_ENABLE;
		}
	}

	if (changed) ui_redraw_display();
}
