#### Project configuration options ####

# Name of target controller
MCU = atmega328p

# Project name
PROJECTNAME = fg100alt

# MCU Clock frequency
CLK_FREQ = 16000000UL
# CLK_FREQ =  1000000UL
# CLK_FREQ = 20000000UL

# Source files
SRC = main.c

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
		-DF_CPU=$(CLK_FREQ) -Wall

# Assembler
override ASMFLAGS = -I. $(INCLUDES) -mmcu=$(MCU)

# Linker
override LDFLAGS = -Wl,-Map,$(TRG).map


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

install: upload

# Linking
$(TRG): $(OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(TRG) $(OBJS)

# Generate object files
%.c.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.S.o: %.S
	$(CC) S(ASMFLAGS) -c $< -o $@

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
