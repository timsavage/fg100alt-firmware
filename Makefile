#### Project configuration options ####

# Name of target controller
MCU = atmega328p

# Project name
PROJECTNAME = fg100alt

# MCU Clock frequency
CLK_FREQ = 30000000UL
#CLK_FREQ = 20000000UL

# DAC step constant
DDS_STEP_CONSTANT = 5.0331648
#DDS_STEP_CONSTANT = 7.5497472

# Source files
SRC = main.c lcd.c ui.c dds.c dds.S

# Additional include paths
INCLUDES =

# Libraries to link in
LIBS =

# Optimization
# use s (size opt), 1, 2, 3 or 0 (off)
OPTIMIZE = 1

# AVR Dude programmer
AVRDUDE_PROGRAMMER = usbtiny

#### End project configuration ####


#### Flags

# Compiler
override CFLAGS = -I. $(INCLUDES) -g -O$(OPTIMIZE) -mmcu=$(MCU) \
		-DF_CPU=$(CLK_FREQ) -DDDS_STEP_CONSTANT=$(DDS_STEP_CONSTANT) \
		-Wall -Werror -Wl,-section-start=.WaveBuffer=0x00800300 \
		-pedantic -pedantic-errors -std=gnu99 \
		-fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -ffunction-sections

# Assembler
override ASMFLAGS = -I. $(INCLUDES) -mmcu=$(MCU)

# Linker
override LDFLAGS = -Wl,-Map,$(TRG).map $(CFLAGS)

#### Executables

CC = avr-gcc
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
AVRDUDE = avrdude
REMOVE = rm -f


#### Target Names

TRG = $(PROJECTNAME).out
DUMPTRG = $(PROJECTNAME).s

HEXROMTRG = $(PROJECTNAME).hex
HEXTRG = $(HEXROMTRG) $(PROJECTNAME).ee.hex
GDBINITFILE = gdbinit-$(PROJECTNAME)

# Filter files by type
CFILES = $(filter %.c, $(SRC))
ASMFILES = $(filter %.S, $(SRC))

# Generate list of object files
OBJS = $(CFILES:.c=.c.o) $(ASMFILES:.S=.S.o)

# Define .lst files
LST = $(filter %.lst, $(OBJS:.o=.lst))


# Build all
all: $(TRG)
	
stats: $(TRG)
	$(OBJDUMP) -h $(TRG)
	$(SIZE) $(TRG)

hex: $(HEXTRG)

upload: hex
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMER) -p $(MCU) -U flash:w:$(HEXROMTRG)

fuses: 
	$(AVRDUDE) -c $(AVRDUDE_PROGRAMMER) -p $(MCU) -U lfuse:w:0xf7:m -U hfuse:w:0xd9:m

install: fuses upload

# Linking
$(TRG): $(OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TRG) $(OBJS)

# Generate object files
%.c.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.S.o: src/%.S
	$(CC) $(ASMFLAGS) -c $< -o $@

# Generate hex
%.hex: %.out
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

%.ee.hex: %.out
	$(OBJCOPY) -j .eeprom --change-section-lma .eeprom=0 -O ihex $< $@

# GDB Init file
gdbinit: $(GDBINITFILE)
	@echo "file $(TRG)" > $(GDBINITFILE)
	@echo "target remote localhost:1212" >> $(GDBINITFILE)
	@echo "load" >> $(GDBINITFILE)
	@echo "break main" >> $(GDBINITFILE)
	@echo "continue" >> $(GDBINITFILE)
	@echo
	@echo "Use 'avr-gdb -x $(GDBINITFILE)'"

clean:
	$(REMOVE) $(TRG) $(TRG).map
	$(REMOVE) $(OBJS)
	$(REMOVE) $(GDBINITFILE)
	$(REMOVE) *.hex
