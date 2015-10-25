/*
 * dac.c
 *
 *  Created on: 23 Oct 2015
 *      Author: tims
 */

#include "dac.h"

extern void ddsloop(uint32_t step, uint8_t* waveform);

void dac_init(void) {
	// Set DAC port to output
	DDRD = 0xFF;  // Enable all output pins on PORTD
}

void dac_start(uint8_t* waveform, uint32_t frequency) {
	uint32_t step = (frequency * DAC_STEP_CONSTANT) + 0.5;
	SPCR = 1;
	ddsloop(step, waveform);
	SPCR = 0;
}
