/*
 * dac.c
 *
 *  Created on: 23 Oct 2015
 *      Author: tims
 */

#include "dac.h"

extern void ddsloop(uint32_t step, uint8_t* waveform);

static uint32_t dac_frequency = 1000;
static uint8_t* dac_waveform = 0;

void dac_init(void) {
	// Set DAC port to output
	DDRD = 0xFF;  // Enable all output pins on PORTD
}

void dac_start(void) {
	if (dac_waveform == 0) return;
	uint32_t step = (dac_frequency * DAC_STEP) + 0.5;
	ddsloop(step, dac_waveform);
}

void dac_break(void) {

}

void dac_set_waveform(uint8_t* waveform) {
	dac_waveform = waveform;
}

void dac_set_freq(uint32_t freq) {
	dac_frequency = freq;
}
uint32_t dac_get_freq(void) {
	return dac_frequency;
}
