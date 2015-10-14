#include <avr/io.h>

.section .text
.global main

main:
	; Turn off watchdog
	cli
	wdr
	ldi		r16, (0 << WDE)
	sts 	WDTCSR, r16
	sei

	ldi     r16, lo8(-1)
	out 	0xa, r16 	; set port D to output
loop:
	com		r16			; 1 clock
	out		0xb, r16    ; 1 clock
	rjmp	loop		; 2 clocks
