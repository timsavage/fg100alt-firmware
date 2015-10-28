#include <avr/io.h>
#include "dds_defines.h"

.section	.text

; extern void ddsloop(uint32_t step, uint8_t* waveform);
.global 	ddsloop

#define I 		r17
#define FIX_L 	r18
#define FIX_H 	r19

#define STEP_3 		r25
#define STEP_2 		r24
#define STEP_1 		r23
#define STEP_0 		r22
#define WAVEFORM_H	r21
#define WAVEFORM_L	r20

ddsloop:
	clr		I
	clr		FIX_L
	clr		FIX_H
	mov		r31, WAVEFORM_H
	mov		r30, WAVEFORM_L

; only works if WAVEFORM_L == 0
loop:
	add		FIX_L, STEP_0			; 1 clock
	adc		FIX_H, STEP_1			; 1 clock
	adc		r30, STEP_2				; 1 clock
	ld		I, Z					; 2 clocks
	out		_SFR_IO_ADDR(DDS_PORT), I	; 1 clock
	sbic	_SFR_IO_ADDR(PINC), PINC3	; 1 clock (if unset)
	rjmp	loop					; 2 clocks
	ret								; --------
									; 9 clocks
