/*-----------------------------------------------------------------------------
 * ui.h
 *
 * Created: 25 Oct 2015
 * Author: tims
 * Revised: 17 Apr, 2017
 * By: crHARPER
 *
 *-----------------------------------------------------------------------------
 */
#ifndef SRC_UI_H_
#define SRC_UI_H_

#include "defines.h"

void ui_show_splash(void);
void ui_redraw_display(void);
void ui_handle_event(uint8_t);

#define RUN (!(PINC & _BV(RUN_STOP_PIN)))

#define HOLD_TIME 25

#endif /* SRC_UI_H_ */
