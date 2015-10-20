#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "waveforms.h"

#define DAC PORTD

int main(void);

void init(void) {
	wdt_disable();

	// Set DAC port to output
	DDRD = 0xFF;  // Enable all output pins on PORTD
}

#define C_LOOP_CYCLES 11 // per iteration

void cloop(uint16_t step, uint8_t* waveform) {
	uint16_t counter = 0;

	while(1) {
		counter += step;
		DAC = waveform[counter >> 8];
	}
}

//#define ASM_LOOP_CYCLES 8 // per iteration

extern void asmloop(uint32_t step, uint8_t* waveform);

const float c_stepConst = 6.7108864; // (256.0^3 * ASM_LOOP_CYCLES) / F_CPU;

int main(void) {
	init();

	uint32_t freq = 5000;
	uint32_t step = (freq * c_stepConst) + 0.5;

	asmloop(step, waves[0]);

	return 1;
}
