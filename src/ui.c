/*
 * ui.c
 *
 *  Created on: 25 Oct 2015
 *      Author: tims
 */

#include <string.h>
#include "ui.h"
#include "lcd.h"
#include "dds.h"


const uint32_t powersOfTen[] = { 1, 10, 100, 1000, 10000, 100000 };

UIState ui_state = {
	DDS_SIGN_WAVE,	// Function
	500,			// Frequency
	DDS_MODE_FIXED, // Wave mode
	1500,			// End sweep frequency
	5000,			// Sweep period (ms)
	0,				// Current page
	0,				// Current field
	2				// Offset of the cursor when editing a value.
};

void ui_show_splash(void) {
	lcd_clear();
	lcd_printf("FG-100 Alt  v%01u.%01u", VERSION_MAJOR, VERSION_MINOR);
	lcd_set_cursor(1, 1);
	lcd_puts("savage.company");
}

void ui_show_fixed(void) {
	lcd_disable_cursor();
	lcd_clear();
	lcd_set_cursor(5 - (strlen(dds_wave_names[ui_state.function])/ 2), 0);
	lcd_printf("Wave: %s", dds_wave_names[ui_state.function]);
	lcd_set_cursor(4, 1);
	lcd_printf("%06luHz", ui_state.frequency);
}

void ui_show_sweep(void) {
	lcd_disable_cursor();
	lcd_clear();
	lcd_printf("Sweep: %s", dds_wave_names[ui_state.function]);
	lcd_set_cursor(0, 1);
	lcd_printf("%06lu-%06luHz", ui_state.frequency, ui_state.end_frequency);
}


void inline _draw_frequency_input(uint32_t value, uint8_t col, uint8_t row, uint8_t focus) {
	lcd_set_cursor(col, row);
	lcd_printf("%06luHz", value);
	if (focus) {
		lcd_set_cursor(col + (5 - ui_state.cursor_offset), row);
	}
}

void _page0_redraw(void) {
	if (ui_state.current_field == 0) lcd_write(0x7E);
	lcd_printf("f(x): %s", dds_wave_names[ui_state.function]);
	lcd_set_cursor(0, 1);
	lcd_puts(" Freq:"); _draw_frequency_input(ui_state.frequency, 6, 1, 1);
	lcd_enable_cursor();
}

void page0_handle_event(uint8_t target, uint8_t event) {

}

void ui_redraw_display() {
	lcd_clear();
	switch(ui_state.current_page) {
	case 0:
		_page0_redraw();
		break;
	}
}

void ui_handle_event(uint8_t interface, uint8_t event) {
	uint8_t changed = 0;

	switch(interface) {
	case MODE_BUTTON:
		if (event == BUTTON_PRESS) {
			ui_state.function = (ui_state.function + 1) % 5;
			changed = 1;
		}
		break;

	case CURSOR_BUTTON:
		if (event == BUTTON_PRESS) {
			ui_state.cursor_offset = (ui_state.cursor_offset + 1) % 6;
			changed = 1;
		}
		break;

	case PLUS_BUTTON:
		if ((event == BUTTON_PRESS) | (event == BUTTON_REPEAT)) {
			uint32_t frequency = ui_state.frequency + powersOfTen[ui_state.cursor_offset];
			if (DDS_FREQ_IN_RANGE(frequency)) {
				ui_state.frequency = frequency;
				changed = 1;
			}
		}
		break;

	case MINUS_BUTTON:
		if ((event == BUTTON_PRESS) | (event == BUTTON_REPEAT)) {
			uint32_t frequency = ui_state.frequency - powersOfTen[ui_state.cursor_offset];
			if (DDS_FREQ_IN_RANGE(frequency)) {
				ui_state.frequency = frequency;
				changed = 1;
			}
		}
		break;

	case RUN_STOP_BUTTON:
		if (event == BUTTON_PRESS) {
			dds_select_wave(ui_state.function);
			DDS_ENABLE;
		}
	}

	if (changed) ui_redraw_display();
}
