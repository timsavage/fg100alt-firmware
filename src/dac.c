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
	DAC_DDR = 0xFF;  // Enable all output pins on PORTD
	DAC_PORT = 0x7F;  // Set output to the middle (is offset to 0)
	DAC_DISABLE;
}

void dac_start(uint8_t* waveform, uint32_t frequency) {
	uint32_t step = (frequency * DAC_STEP_CONSTANT) + 0.5;
	DAC_ENABLE;
	ddsloop(step, waveform);
	DAC_DISABLE;
	DAC_PORT = 0x7F;
}
