/*
 * pinouts.h
 *
 *  Created on: 21 Oct 2015
 *      Author: tims
 */

#ifndef PINOUTS_H_
#define PINOUTS_H_

#include <avr/io.h>

// DAC
#define DAC PORTD
#define DAC_DDR DDRD

#define DAC_STEP 6.7108864  // (256.0^3 * ASM_LOOP_CYCLES) / F_CPU


// Switches
#define RUN_STOP_PIN PORTC3

#define MODE_PIN PORTB5
#define CURSOR_PIN PORTB4
#define PLUS_PIN PORTB3
#define MINUS_PIN PORTB2

#define BUTTON_STROBE PORTB1


// LCD
#define LCD_CTRL_PORT PORTC
#define LCD_CTRL_DDR DDRC
#define LCD_DATA_PORT PORTB
#define LCD_DATA_DDR DDRB

#define LCD_RS PORTC2
#define LCD_EN PORTC1
#define LCD_LOW_BYTE PORTB2

#endif /* PINOUTS_H_ */
