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

const uint8_t marker[] = {
	0x00,	// 00000
	0x00,	// 00000
	0x0C,	// 01100
	0x16,	// 10110
	0x1E,	// 11110
	0x0C,	// 01100
	0x00,	// 00000
	0x00	// 00000
};
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

void ui_init(void) {
	lcd_create_char(0x01, marker);
}


/**********************************************************
 * Splash display
 *********************************************************/

void ui_show_splash(void) {
	lcd_clear();
	lcd_printf("FG-100 NG   v%01u.%01u", VERSION_MAJOR, VERSION_MINOR);
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


/**********************************************************
 * Helpers
 *********************************************************/

void inline _draw_frequency_input(uint32_t value, uint8_t col, uint8_t row, uint8_t focus) {
	lcd_set_cursor(col, row);
	lcd_printf("%06luHz", value);
	if (focus) {
		lcd_set_cursor(col + (5 - ui_state.cursor_offset), row);
	}
}


/**********************************************************
 * Page 0
 *********************************************************/

#define FIELD_FUNCTION  0
#define FIELD_FREQUENCY 1

void _page0_draw(void) {
	uint8_t current_field = ui_state.current_field;

	lcd_write(current_field == FIELD_FUNCTION ? 0x01 : ' ');
	lcd_printf("Fn: %s", dds_wave_names[ui_state.function]);

	lcd_set_cursor(0, 1);
	lcd_write(current_field == FIELD_FREQUENCY ? 0x01 : ' ');
	lcd_puts("Fq:"); _draw_frequency_input(ui_state.frequency, 5, 1, current_field == FIELD_FREQUENCY);

	lcd_enable_cursor();
}

uint8_t _page0_handle_events(uint8_t ctrl, uint8_t event) {
	if (ctrl == MODE_BUTTON) {
		ui_state.current_field = (ui_state.current_field + 1) % 2;
		return 1;
	}

	switch (ui_state.current_field) {
	case FIELD_FUNCTION:
		switch(ctrl) {
		case PLUS_BUTTON:
			if (event == BUTTON_PRESS) {
				ui_state.function = (ui_state.function + 1) % 5;
				return 1;
			}
			break;

		case MINUS_BUTTON:
			if (event == BUTTON_PRESS) {
				ui_state.function = (ui_state.function - 1) % 5;
				return 1;
			}
			break;
		}
		break;

	case FIELD_FREQUENCY:
		switch(ctrl) {
		case CURSOR_BUTTON:
			if (event == BUTTON_PRESS) {
				ui_state.cursor_offset = (ui_state.cursor_offset + 1) % 6;
				return 1;
			}
			break;

		case PLUS_BUTTON:
			if ((event == BUTTON_PRESS) | (event == BUTTON_REPEAT)) {
				uint32_t frequency = ui_state.frequency + powersOfTen[ui_state.cursor_offset];
				if (DDS_FREQ_IN_RANGE(frequency)) {
					ui_state.frequency = frequency;
					return 1;
				}
			}
			break;

		case MINUS_BUTTON:
			if ((event == BUTTON_PRESS) | (event == BUTTON_REPEAT)) {
				uint32_t frequency = ui_state.frequency - powersOfTen[ui_state.cursor_offset];
				if (DDS_FREQ_IN_RANGE(frequency)) {
					ui_state.frequency = frequency;
					return 1;
				}
			}
			break;
		}
		break;
	}
	return 0;
}


/**********************************************************
 * Page 1
 *********************************************************/

void _page1_draw(void) {
	if (ui_state.current_field == 0) { lcd_write(0x01); } else { lcd_write(' '); }
	lcd_puts("E fq:"); _draw_frequency_input(ui_state.frequency, 7, 1, 1);
}

uint8_t _page1_handle_events(uint8_t ctrl, uint8_t event) {
	return 0;
}


/**********************************************************
 * Router events/rendering
 *********************************************************/

void ui_draw_display() {
	lcd_clear();
	switch(ui_state.current_page) {
	case 0: _page0_draw(); break;
	case 1: _page1_draw(); break;
	}
}

void ui_handle_event(uint8_t ctrl, uint8_t event) {
	uint8_t changed = 0;

	if (ctrl == RUN_STOP_BUTTON) {
		if (event == BUTTON_PRESS) {
			dds_select_wave(ui_state.function);
			DDS_ENABLE;
		}
	} else {
		switch(ui_state.current_page) {
		case 0:
			changed = _page0_handle_events(ctrl, event);
			break;
		case 1:
			changed = _page1_handle_events(ctrl, event);
			break;
		}

		if (changed) ui_draw_display();
	}
}
