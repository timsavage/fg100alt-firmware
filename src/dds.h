/*
 * dds.h
 *
 *  Created on: 23 Oct 2015
 *      Author: tims
 */

#ifndef DDS_H_
#define DDS_H_

#include <avr/io.h>
#include <src/dds_defines.h>

// DDS wave names
extern const char* dds_wave_names[];

// Check that the DDS is enabled
#define DDS_IS_ENABLED 	((DDS_ENABLE_ADDR >> DDS_ENABLE_BIT) & 1)
#define DDS_ENABLE 		DDS_ENABLE_ADDR |=  _BV(DDS_ENABLE_BIT)
#define DDS_DISABLE 	DDS_ENABLE_ADDR &= ~_BV(DDS_ENABLE_BIT)
#define DDS_TOGGLE		DDS_ENABLE_ADDR ^=  _BV(DDS_ENABLE_BIT)


/**
 * Init DDS 
 */
void dds_init(void);

/**
 * Populate the waveform buffer with a waveform from the internally defined wave table.
 */
void dds_select_wave(uint8_t wave_idx);

/**
 * Start/break DDS waveform generation
 */
void dds_start(uint32_t frequency);

/**
 * Start/break DDS sweep waveform generation
 */
void dds_start_sweep(uint32_t frequency);


#endif /* DDS_H_ */
