###############################################################################
# Makefile for the project termo_mvn
###############################################################################

## General Flags
PROJECT = termo_mvn
MCU = atmega328p
TARGET = termo_mvn.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=termo_mvn.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Objects that must be built in order to link
OBJECTS = termo_mvn.o interface.o kbd.o rtos.o max7219.o ds18x20.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) termo_mvn.hex termo_mvn.eep termo_mvn.lss size

## Compile
termo_mvn.o: ../termo_mvn.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

interface.o: ../interface.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

kbd.o: ../kbd.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

rtos.o: ../rtos.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

max7219.o: ../max7219.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

ds18x20.o: ../ds18x20.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) termo_mvn.elf dep/* termo_mvn.hex termo_mvn.eep termo_mvn.lss termo_mvn.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

