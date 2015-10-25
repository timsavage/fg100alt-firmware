/*
 * dac.h
 *
 *  Created on: 23 Oct 2015
 *      Author: tims
 */

#ifndef SRC_DAC_H_
#define SRC_DAC_H_

#include <avr/io.h>

/**
 * DAC Step constant
 *
 * This value should be defined in your Makefile (alongside F_CPU)
 *
 * This value is used to calculate the step size required to generate a
 * waveform at the specified frequency. The value is defined as:
 *
 * 	DAC_STEP_CONSTANT = (256^3 * ASM_LOOP_CYCLES) / F_CPU
 *
 * The value of 256 ^ 3 is derived from the length of the waveform table and
 * the offset of the bits used to index into the waveform table. The offset is
 * 256 << 16 (or 256 ^ 2).
 *
 * ASM_LOOP_CYCLES is the number of clock cycles required to complete one cycle
 * of the DDS generation loop, the current value is 8 clock cycles.
 *
 * F_CPU (this is defined in the Makefile as it is also used to scale
 * operations in <util/delay.h>) is the clock speed in Hz.
 *
 * It is easier to pre-calculate this value, this Google Sheets document can be
 * used to calculate this value:
 *
 * https://docs.google.com/spreadsheets/d/1Oab6_2TZ5SzAQHbTxMHLZFGgJnOQwO2SQYt80mpjkCc/edit?usp=sharing
 */
#ifndef DAC_STEP_CONSTANT
/* prevent compiler error by supplying a default based on a 20Mhz clock */
# warning "DAC_STEP_CONSTANT not defined for \"dac.h\""
#define DAC_STEP_CONSTANT 6.7108864
#endif

// Define DAC port (defaults to PORTD)
#ifndef DAC_PORT
#define DAC_PORT PORTD
#endif

// Define DAC DDR (defaults to DDRD)
#ifndef DAC_DDR
#define DAC_DDR DDRD
#endif


/**
 * Init DAC
 */
void dac_init(void);

/**
 * Start/break DDS waveform generation
 */
void dac_start(void);
void dac_break(void);

/**
 * Set waveform
 */
void dac_set_waveform(uint8_t* waveform);

/**
 * Frequency control
 */
void dac_set_freq(uint32_t freq);
uint32_t dac_get_freq(void);

#endif /* SRC_DAC_H_ */
