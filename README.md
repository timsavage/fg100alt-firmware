# FG-100 alt firmware
Alternate firmware for the FG-100 function generator

The FG 100 is a cheap function generator widely available on Ebay. 

See https://hackaday.io/project/7985-alternate-firmware-for-fg-100-dds-function-gen for details

## Requirements

You will need:

* Obviously a FG-100 (ebay is a good source)
* ATMega328p 28pin DIP microcontroller 
* AVR programmer/ISP (the Makefile defaults to using a USBTiny ISP)
* AVR toolchain (eg avr-gcc, avrdude and gnumake) installed

> Warning: There are many sources for the FG-100 of varying quality and performace. My particular unit has a high quality PCB with quality components (Rubicon capacitors), the output is clean with minimal noise. Other people (including a review on Hackaday) have not had such luck. YMMV

If you are using a different programmer you will need to update the Makefile with the currect programmer code
for AVRdude, update the *AVRDUDE_PROGRAMMER* parameter with the correct value for your programmer (see AVR dude
documentation for info).

## Compile/Install

All development was completed on GNU/Linux so all instructions relate to this OS (feel free to generate a
project file for AVRStudio and send me a pull request ;) ).

Steps to build/upload/set correct fuses:
````
  > make program
````

If you prefer to do the steps separately:
````
  # Compile/Link/Generate Hex
  > make hex
  # Upload to microcontroller
  > make upload
  # Set correct fuses
  > make fuses
````

Finally open up the FG-100, unscrew the LCD and remove the existing ATMega48 from it's socket, replace with your newly flashed ATMega328p, replace the LCD.

~ Enjoy!
