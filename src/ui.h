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
 * Initialize UI
 */
void ui_init(void);

/**
 * Show splash display
 */
void ui_show_splash(void);

/**
 * Show fixed display
 */
void ui_show_fixed(void);

/**
 * Show sweep display
 */
void ui_show_sweep(void);

/**
 * Redraw the current display to the LCD
 */
void ui_draw_display();

/**
 * Handle a interface event (for example button press)
 */
void ui_handle_event(uint8_t ctrl, uint8_t event);

#endif /* SRC_UI_H_ */
