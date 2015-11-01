/*
 * ui.h
 *
 *  Created on: 25 Oct 2015
 *      Author: tims
 */

#ifndef SRC_UI_H_
#define SRC_UI_H_

#include "defines.h"

typedef struct {
	// Current output function
	uint8_t function;
	// Configured frequency (this will be stored to EPROM eventually)
	uint32_t frequency;
	// Wave mode
	uint8_t wave_mode;
	// End sweep frequency
	uint32_t end_frequency;
	// Step period (ms)
	uint32_t sweep_period;
	// Current page
	uint8_t current_page;
	// Current field
	uint8_t current_field;
	// Offset of the cursor when editing a value.
	uint8_t cursor_offset;
} UIState;

extern UIState ui_state;

/**
 * Show splash display
 */
void ui_show_splash(void);
void ui_show_fixed(void);
void ui_show_sweep(void);


/**
 * Redraw the current display to the LCD
 */
void ui_redraw_display();

/**
 * Handle a interface event (eg button press)
 */
void ui_handle_event(uint8_t interface, uint8_t event);

#endif /* SRC_UI_H_ */
