/*-----------------------------------------------------------------------------
 * ui.c
 *
 * Created: 25 Oct 2015
 * Author: tims
 * Revised: 17 Apr, 2017
 * By: crHARPER
 *
 *-----------------------------------------------------------------------------
 */

#include "ui.h"
#include "lcd.h"
#include "dds.h"
#include "config.h"

const uint32_t powersOfTen[] = { 1, 10, 100, 1000, 10000, 100000 };

extern Config_Eeprom_Type ui_state;

//-----------------------------------------------------------------------------
void ui_show_splash(void) {
	lcd_clear();
	lcd_printf("CH-100    V%02u.%02u", VERSION_MAJOR, VERSION_MINOR);
	lcd_set_cursor(0, 1);
	lcd_puts("tSavage/crHARPER");
}


//-----------------------------------------------------------------------------
void ui_redraw_display(void) {
    
	lcd_clear();
	lcd_printf("Wave: %s", dds_wave_names[ui_state.waveform]);
	lcd_set_cursor(0, 1);
	lcd_printf("Freq: %06lu Hz", ui_state.frequency);
	
    lcd_set_cursor(11 - ui_state.cursor, 1);
	lcd_enable_cursor();

}


//-----------------------------------------------------------------------------
// returns digit under cursor: 0-9
uint8_t ui_get_digit(uint8_t cursor, uint32_t value){
    uint32_t a, b;
    
    a = value / powersOfTen[cursor];
    b = a % 10;
    return b;
    
}


//-----------------------------------------------------------------------------
// returns value less weighted value under cursor
uint32_t ui_less_digit(uint8_t cursor, uint32_t value){
    uint32_t a, b, c;
    
    a = value % powersOfTen[cursor];
    b = value / powersOfTen[cursor + 1 ];
    b *= powersOfTen[cursor + 1];
    c = a + b;
    return c;
    
}


//-----------------------------------------------------------------------------
void ui_handle_event(uint8_t buttons) {
    static uint8_t hold = 0;
    static uint8_t previous = 0;
    
    // allow for extended button presses
    // use a counter for each button's extended hold function
    static uint8_t cursor_hold = HOLD_TIME;
    
    uint32_t frequency;
    uint8_t digit;
    
    if( !hold ){ 
        // normal button functions    
        switch(buttons) {
            case MODE_BUTTON:
                ui_state.waveform = (ui_state.waveform + 1) % 5;
                ui_redraw_display();
                hold = 1;
                break;
            

            case CURSOR_BUTTON:
                ui_state.cursor = (ui_state.cursor + 1) % 6;
                ui_redraw_display();
                hold = 1;
                break;
            

            case PLUS_BUTTON:
                    
                digit = ui_get_digit( ui_state.cursor, ui_state.frequency );
                
                if( digit >= 9)
                    digit = 0;
                else
                    digit++;
                
                frequency = ui_less_digit( ui_state.cursor, ui_state.frequency );
                frequency += powersOfTen[ui_state.cursor] * digit;
                
                // original increment operation for reference
                //frequency = ui_state.frequency + powersOfTen[ui_state.cursor];
                
                if ( frequency > DDS_MAX_FREQ )
                    frequency = DDS_MIN_FREQ;
               
                if( DDS_FREQ_IN_RANGE(frequency) ){ 
                    ui_state.frequency = frequency;
                    ui_redraw_display();
                }
                
                hold = 1;
                break;
                

            case MINUS_BUTTON:
                
                digit = ui_get_digit( ui_state.cursor, ui_state.frequency );
                
                if( digit )
                    digit--;
                else
                    digit = 9;     
                
                frequency = ui_less_digit( ui_state.cursor, ui_state.frequency );
                frequency += powersOfTen[ui_state.cursor] * digit;
            
                // original decrement operation for reference
                //frequency = ui_state.frequency - powersOfTen[ui_state.cursor];
                
                // Allow freq. of 0 Hz temporarily
                // to simplify operator interaction
                // when setting frequency.  Will get corrected below.    
                if( DDS_FREQ_IN_RANGE(frequency) ){ 
                    ui_state.frequency = frequency;
                    ui_redraw_display();
                }                
                
                hold = 1;
                break;
                

            case RUN_STOP_BUTTON:
                dds_select_wave(ui_state.waveform);
                // Don't enable DSS until button is released
                hold = 1;
                break;
	
        }
    }
    else{
        // all buttons released
        if( !buttons ){
            hold = 0;   // can allow next button input
            if( previous == RUN_STOP_BUTTON){
                // because dds.S will return before button is released
                // we delay the start of the DDS until now
                
                // do not enable for 0 Hz (but you could)
                // square wave output would be held high
                // all other waveforms output 0V
                if( ui_state.frequency )
                    DDS_ENABLE;
                
                // EEPROM is only written here to minimized writes
                config_write(); // save last valid state to EEPROM
            }
            
            // for extended button presses
            // reset hold down counters here
            // since all buttons a released
            cursor_hold = HOLD_TIME;
        }
        else{
            // if a button is held down for an extended period 
            // apply special functions here
            
            // clear frequency value when cursor button is held down
            if( buttons == CURSOR_BUTTON && previous == CURSOR_BUTTON ){
                if( cursor_hold ){
                    if( !--cursor_hold ){ 
                        ui_state.frequency = 0;
                        ui_redraw_display();
                    }
                }
            }
            // end of extended cursor button function
            
            // additional extended button functions here
                    
        }
    }
    
    previous = buttons;
    
}

