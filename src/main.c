#include <util/delay.h>
#include <avr/wdt.h>

#include "defines.h"
#include "waveforms.h"

#include "lcd.h"
#include "dac.h"

int main(void);

void init(void) {
	wdt_disable();

	// Initialize the LCD and display splash message
	lcd_init();
	lcd_clear();
	lcd_puts("FG-100 Alt  v0.1");
	lcd_set_cursor(1, 1);
	lcd_puts("savage.company");

	dac_init();
}

void update_lcd() {
	lcd_clear();
	lcd_printf("Wave: Sign");
	lcd_set_cursor(0, 1);
	lcd_printf("Freq: %06uHz", dac_get_freq());
}

int main(void) {
	init();

	dac_set_freq(500);
	dac_set_waveform(waves[0]);

	_delay_ms(5000);

	update_lcd();
	dac_start();

	return 1;
}
