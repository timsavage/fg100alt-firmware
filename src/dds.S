;-----------------------------------------------------------------------------
; dds.S
;
; Created: 25 Oct 2015
; Author: tims
; Revised: 17 Apr, 2017
; By: crHARPER
;
;-----------------------------------------------------------------------------
#include <avr/io.h>
#include "dds_defines.h"

.section	.text

; extern void ddsloop(uint32_t step, uint8_t* waveform);
.global 	ddsloop

#define I 		r17
#define FIX_L		r18
#define FIX_H		r19

; Registers used by C Interface
#define STEP_3		r25
#define STEP_2		r24
#define STEP_1		r23
#define STEP_0		r22
#define WAVEFORM_H	r21
#define WAVEFORM_L	r20

ddsloop:
	; Reset registers to 0
	clr		I
	clr		FIX_L
	clr		FIX_H
	
	; Assign waveform address to Z register (16bit r30/r31)
	mov		r31, WAVEFORM_H
	mov		r30, WAVEFORM_L

; only works if WAVEFORM_L == 0
loop:
	; Add 24bit step to Fix with excess bits applied to the low byte of the Z register
	add		FIX_L, STEP_0			; 1 clock
	adc		FIX_H, STEP_1			; 1 clock
	adc		r30, STEP_2			; 1 clock
	
	; Load value from waveform using 16bit address in Z register into I
	ld		I, Z				; 2 clocks
	
	; Apply I to 8bit DDS_Port
	out		_SFR_IO_ADDR(DDS_PORT), I	; 1 clock
	
	; Check for button press to cancel
	sbic	_SFR_IO_ADDR(PINC), PINC3		; 1 clock (if unset)
	rjmp	loop					; 2 clocks
							; --------
							; 9 clocks
	
; wait for R/S button release
wait:
	sbis	_SFR_IO_ADDR(PINC), PINC3
	rjmp	wait
	ret
