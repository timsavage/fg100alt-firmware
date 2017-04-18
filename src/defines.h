/*-----------------------------------------------------------------------------
 * defines.h
 *
 * Created: 21 Oct 2015
 * Author: tims
 * Revised: 17 Apr, 2017
 * By: crHARPER
 *
 *-----------------------------------------------------------------------------
 */
#ifndef DEFINES_H_
#define DEFINES_H_

#include <avr/io.h>

#define VERSION_MAJOR 	1
#define VERSION_MINOR	0


// Input switches
#define RUN_STOP_PIN 	PINC3

#define MODE_PIN 		PINB5
#define CURSOR_PIN 		PINB4
#define PLUS_PIN 		PINB2
#define MINUS_PIN 		PINB3
#define BUTTON_STROBE 	PINB1


// LCD
#define LCD_CTRL_PORT 	PORTC
#define LCD_CTRL_DDR 	DDRC
#define LCD_DATA_PORT 	PORTB
#define LCD_DATA_DDR 	DDRB

#define LCD_RS 			PORTC2
#define LCD_EN 			PORTC1
#define LCD_LOW_BYTE 	PORTB2


// Button equivalent bits
#define NO_BUTTON       0x00
#define MODE_BUTTON		0x01
#define CURSOR_BUTTON	0x02
#define	PLUS_BUTTON		0x04
#define MINUS_BUTTON	0x08
#define RUN_STOP_BUTTON	0x10


#endif /* DEFINES_H_ */
