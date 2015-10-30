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
	// Configured frequency (this will be stored to EPROM eventually)
	uint32_t frequency;
	// Selected frequency offset (used to adjust frequency)
	uint8_t frequency_offset;
	// Current output function
	uint8_t wave_form;
	// Start sweep frequency
	uint32_t start_frequency;
	// End sweep frequency
	uint32_t end_frequency;
	// Sweep frequency step
	uint32_t frequency_step;
	// Step period
	uint32_t step_period;
	// Current view
	uint8_t current_view;
} UIState;

extern UIState ui_state;

/**
 * Show splash display
 */
void ui_show_splash(void);

/**
 * Redraw the current display to the LCD
 */
void ui_redraw_display();

/**
 * Handle a interface event (eg button press)
 */
void ui_handle_event(uint8_t interface, uint8_t event);

#endif /* SRC_UI_H_ */
