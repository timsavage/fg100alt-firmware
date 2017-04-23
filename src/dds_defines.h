/*-----------------------------------------------------------------------------
 * dds_defines.h
 *
 * Created: 28 Oct 2015
 * Author: tims
 *
 *-----------------------------------------------------------------------------
 */
#ifndef DDS_DEFINES_H_
#define DDS_DEFINES_H_

/**
 * DDS Step constant
 *
 * This value should be defined in your Makefile (alongside F_CPU)
 *
 * This value is used to calculate the step size required to generate a
 * waveform at the specified frequency. The value is defined as:
 *
 * 	DDS_STEP_CONSTANT = (256^3 * ASM_LOOP_CYCLES) / F_CPU
 *
 * The value of 256 ^ 3 is derived from the length of the waveform table and
 * the offset of the bits used to index into the waveform table. The offset is
 * 256 << 16 (or 256 ^ 2).
 *
 * ASM_LOOP_CYCLES is the number of clock cycles required to complete one cycle
 * of the DDS generation loop, the current value is 9 clock cycles.
 *
 * F_CPU (this is defined in the Makefile as it is also used to scale
 * operations in <util/delay.h>) is the clock speed in Hz.
 *
 * It is easier to pre-calculate this value, this Google Sheets document can be
 * used to calculate the value:
 *
 * https://docs.google.com/spreadsheets/d/1Oab6_2TZ5SzAQHbTxMHLZFGgJnOQwO2SQYt80mpjkCc/edit?usp=sharing
 */
#ifndef DDS_STEP_CONSTANT
/* prevent compiler error by supplying a default based on a 30Mhz clock */
# warning "DDS_STEP_CONSTANT not defined for \"dds_defines.h\""
#define DDS_STEP_CONSTANT 5.0331648
#endif

// Define DDS port (defaults to PORTD)
#ifndef DDS_PORT
#define DDS_PORT PORTD
#endif

// Define DDS DDR (defaults to DDRD)
#ifndef DDS_DDR
#define DDS_DDR DDRD
#endif

// Register used to break the DDS cycle
#ifndef DDS_BREAK_REGISTER
#define DDS_BREAK_REGISTER GPIOR0
#endif

// Bit within the Break register
#ifndef DDS_BREAK_BIT
#define DDS_BREAK_BIT GPIOR00
#endif

// Minimum frequency
#ifndef DDS_MIN_FREQ
#define DDS_MIN_FREQ 0
#endif

// Maximum frequency (supported by hardware), default is 500kHz
#ifndef DDS_MAX_FREQ
#define DDS_MAX_FREQ 500000
#endif

// Check if frequency is in DDS range
#define DDS_FREQ_IN_RANGE(freq)	((freq <= DDS_MAX_FREQ) & (freq >= DDS_MIN_FREQ))

// Waveforms
#define DDS_SINE_WAVE 		0
#define DDS_TRIANGLE_WAVE 	1
#define DDS_SAWTOOTH_WAVE 	2
#define DDS_RSAWTOOTH_WAVE 	3
#define DDS_SQUARE_WAVE 	4

// Number of waveforms defined in the DDS waveform table.
#define DDS_WAVE_COUNT (DDS_SQUARE_WAVE + 1)

#endif /* DDS_DEFINES_H_ */
