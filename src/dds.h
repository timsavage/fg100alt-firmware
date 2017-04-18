/*-----------------------------------------------------------------------------
 * dds.h
 *
 * Created: 23 Oct 2015
 * Author: tims
 *
 *-----------------------------------------------------------------------------
 */
#ifndef DDS_H_
#define DDS_H_

#include <avr/io.h>
#include <src/dds_defines.h>

// DDS wave names
extern const char* dds_wave_names[];

// Check that the DDS is enabled
#define DDS_IS_ENABLED 	((DDS_BREAK_REGISTER >> DDS_BREAK_BIT) & 1)

#define DDS_ENABLE 		DDS_BREAK_REGISTER |=  _BV(DDS_BREAK_BIT)
#define DDS_DISABLE 	DDS_BREAK_REGISTER &= ~_BV(DDS_BREAK_BIT)


void dds_init(void);
void dds_select_wave(uint8_t);
void dds_start(uint32_t);

#endif /* DDS_H_ */
