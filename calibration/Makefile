#######################################
# Start configuration

# Target Micro
MCU = atmega328p

# Programmer and port
AVRDUDE_PROGRAMMER = usbtiny
AVRDUDE_PORT = usb

# End configuration
#######################################

#### Executables

CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
REMOVE = rm -f

all: help

help:
	@echo "=========================="
	@echo "Clock calibration firmware"
	@echo "=========================="
	@echo ""
	@echo "Firmware outputs a square wave that takes 8 clock cycles/period"
	@echo "the output frequency can then be measured and used to calculate"
	@echo "the actual frequency of your crystal oscillator."
	@echo ""
	@echo "To write ensure the MCU, AVRDUDE_PROGRAMMER & AVRDUDE_PORT" 
	@echo "options are set correctly in the configuration section of"
	@echo "Makefile then run:"
	@echo ""
	@echo "> make install"
	@echo ""

install: write-flash

calibrate.out: calibrate.S
	$(CC) calibrate.S -mmcu=$(MCU) -O2 -g -o $@

calibrate.hex: calibrate.out
	$(OBJCOPY) -j .text -O ihex $< $@

write-flash: calibrate.hex
	sudo $(AVRDUDE) -c $(AVRDUDE_PROGRAMMER) -P $(AVRDUDE_PORT) -p $(MCU) -U flash:w:$<

clean:
	rm *.hex *.out
	