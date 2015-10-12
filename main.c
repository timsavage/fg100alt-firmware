#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>

#include "waveforms.h"

#define DAC PORTD

int main(void);

void WDT_off(void) {
	wdt_reset();
	/* Clear WDRF in MCUSR */
	MCUSR &= ~_BV(WDRF);
	/* Write logical one to WDCE and WDE, Keep old prescaler setting to prevent unintentional time-out */
	WDTCSR |= _BV(WDCE) | _BV(WDE);
	/* Turn off WDT */
	WDTCSR = 0x00;
}

void init(void) {
	WDT_off();

	// Set DAC port to output
	DDRD = 0xFF;  // Enable all output pins on PORTD
}

unsigned char counter = 0;

void generate_dds() {
	DAC = sineWave[(counter++) & 0xFF];
}

int main(void) {
	init();
		
	while(1) {
		generate_dds();
	}
}
