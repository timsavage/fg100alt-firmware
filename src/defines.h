/*
 * pinouts.h
 *
 *  Created on: 21 Oct 2015
 *      Author: tims
 */

#ifndef PINOUTS_H_
#define PINOUTS_H_

#include <avr/io.h>

#define VERSION_MAJOR 	0
#define VERSION_MINOR	1


/**
 * Input switches
 */
#define RUN_STOP_PIN 	PINC3
#define MODE_PIN 		PINB5
#define CURSOR_PIN 		PINB4
#define PLUS_PIN 		PINB2
#define MINUS_PIN 		PINB3
#define BUTTON_STROBE 	PINB1

// Current state of the button
#define BUTTON_UP 		0
#define BUTTON_DOWN 	1


/**
 * LCD
 */
#define LCD_CTRL_PORT 	PORTC
#define LCD_CTRL_DDR 	DDRC
#define LCD_DATA_PORT 	PORTB
#define LCD_DATA_DDR 	DDRB

#define LCD_RS 			PORTC2
#define LCD_EN 			PORTC1
#define LCD_LOW_BYTE 	PORTB2


/**
 * UI
 *
 * Main loop provides a simple HAL to simplify UI.
 */

// Buttons
#define MODE_BUTTON		0
#define CURSOR_BUTTON	1
#define	PLUS_BUTTON		2
#define MINUS_BUTTON	3

#define BUTTON_COUNT	4

// Events
#define NO_OP			0
#define BUTTON_PRESS	1
#define BUTTON_REPEAT	2
#define BUTTON_RELEASE	3

typedef struct {
	// Configured frequency (this will be stored to EPROM eventually)
	uint32_t frequency;
	// Selected frequency offset (used to adjust frequency)
	uint8_t frequencyOffset;
	// Current output function
	uint8_t function;  // Sign wave
} State;

#endif /* PINOUTS_H_ */
