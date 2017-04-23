/*-----------------------------------------------------------------------------
 * main.c
 *
 * Created: 25 Oct 2015
 * Author: tims
 * Revised: 17 Apr, 2017
 * By: crHARPER
 *
 *-----------------------------------------------------------------------------
 */
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <math.h>


#include "defines.h"
#include "config.h"
#include "lcd.h"
#include "ui.h"
#include "dds.h"

extern Config_Eeprom_Type ui_state;


//-----------------------------------------------------------------------------
void init(void) {
    // Do first to set correct DC offset of output
    // fixes bug in original code
    dds_init();
    
    wdt_disable();
 
    // Configure Strobe as Input
    DDRB  &= ~_BV(BUTTON_STROBE);
    PORTB |=  _BV(BUTTON_STROBE);   // Enable pull up resistor
    // Configure Run/Stop as Input
    DDRC  &= ~_BV(PINC3);
    PORTC |=  _BV(PINC3);           // Enable pull up resistor

    // Initialize the LCD and display splash message
    lcd_init();
    ui_show_splash();
    
    // Enable interrupts
    sei();
}


//-----------------------------------------------------------------------------
uint8_t check_strobe_pin(uint8_t pin) {
    uint8_t temp;

    // set PortB nibble for corrisponding button bit
    PORTB = (PORTB & ~(0x0F << PORTB2)) | _BV(pin);
    _delay_ms(5);  // settling time 
    temp = (PINB & _BV(BUTTON_STROBE)); //read
    PORTB = (PORTB & ~(0x0F << PORTB2));
    return temp;
}


//-----------------------------------------------------------------------------
// Generate UI events
void operator_input() {
    uint8_t buttons = 0;
    
    if (RUN) {
        buttons |= RUN_STOP_BUTTON;
    }

    // strobed button inputs must be checked one at a time
    
    if (check_strobe_pin(MODE_PIN)) {
        buttons |= MODE_BUTTON;
    }
    if (check_strobe_pin(CURSOR_PIN)) {
        buttons |= CURSOR_BUTTON;
    }
    if (check_strobe_pin(PLUS_PIN)) {
        buttons |= PLUS_BUTTON;
    }
    if (check_strobe_pin(MINUS_PIN)) {
        buttons |= MINUS_BUTTON;
    }
    
    ui_handle_event(buttons);
}



//-----------------------------------------------------------------------------
int main(void) {
    init();
    
    config_init();  //get saves setting from EEPROM
    
    _delay_ms(2000);
    ui_redraw_display();

    while (1) {
        operator_input();

        if (DDS_IS_ENABLED) {
            lcd_disable_cursor();
        
            dds_start(ui_state.frequency);
        
            lcd_enable_cursor();
            DDS_DISABLE;
        }
        
        // implement sleep mode here to save power
        _delay_ms(100);
    }
}
