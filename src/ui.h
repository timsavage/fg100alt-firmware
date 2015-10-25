/*
 * ui.h
 *
 *  Created on: 25 Oct 2015
 *      Author: tims
 */

#ifndef SRC_UI_H_
#define SRC_UI_H_

#include "defines.h"

/**
 * Show splash display
 */
void ui_show_splash(void);

/**
 * Redraw the current display to the LCD
 */
void ui_redraw_display(State* state);

/**
 * Handle a interface event (eg button press)
 */
void ui_handle_event(uint8_t interface, uint8_t event, State* state);

#endif /* SRC_UI_H_ */
