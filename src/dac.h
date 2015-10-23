/*
 * dac.h
 *
 *  Created on: 23 Oct 2015
 *      Author: tims
 */

#ifndef SRC_DAC_H_
#define SRC_DAC_H_

#include "defines.h"

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
