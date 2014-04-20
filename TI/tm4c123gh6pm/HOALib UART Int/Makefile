###############################################################################
# Configuration Variables

#Taget Binary Name
TARGET      := main

# List all the source files here
SOURCES     := $(wildcard src/*.c)
SOURCESCPP  := $(wildcard src/*.cpp)

# Includes are located in the Include directory
INCLUDES    := inc

# Path to the root of your ARM toolchain
TOOL        := $(shell dirname `which arm-none-eabi-gcc`)

# Path to the root of your StellarisWare folder
TW_DIR	:=  ../../TivaWare

# Location of a linker script, doesnt matter which one, they're the same
LD_SCRIPT   := tm4c.ld

# FPU Type
FPU          := hard
#FPU          = softfp

# Configuration Variables
###############################################################################


###############################################################################
# Tool Definitions
CPP         := $(TOOL)/arm-none-eabi-g++
CC          := $(TOOL)/arm-none-eabi-gcc
LD          := $(TOOL)/arm-none-eabi-ld
AR          := $(TOOL)/arm-none-eabi-ar
AS          := $(TOOL)/arm-none-eabi-as
NM          := $(TOOL)/arm-none-eabi-nm
OBJCOPY     := $(TOOL)/arm-none-eabi-objcopy
OBJDUMP     := $(TOOL)/arm-none-eabi-objdump
RANLIB      := $(TOOL)/arm-none-eabi-ranlib
STRIP       := $(TOOL)/arm-none-eabi-strip
SIZE        := $(TOOL)/arm-none-eabi-size
READELF     := $(TOOL)/arm-none-eabi-readelf
DEBUG       := $(TOOL)/arm-none-eabi-gdb
GCC_VER     := $(shell $(CC) -dumpversion)
FLASH       := $(shell which lm4flash)
CP          := cp -p
RM          := rm -rf
MV          := mv
MKDIR       := mkdir -p
UART        := minicom
# Tool Definitions
###############################################################################


###############################################################################
# Flag Definitions
CFLAGS     += -mthumb
CFLAGS     += -mcpu=cortex-m4
CFLAGS     += -mfloat-abi=$(FPU)
CFLAGS     += -mfpu=fpv4-sp-d16
CFLAGS     += -Os
CFLAGS     += -ffunction-sections
CFLAGS     += -fdata-sections
CFLAGS     += -MD
CFLAGS     += -Wall
CFLAGS     += -pedantic
CFLAGS     += -g
CFLAGS     += -DPART_TM4C123GH6PM
CFLAGS     += -Dgcc
CFLAGS     += -unresolved-symbols=ignore-all
CFLAGS     += -DTARGET_IS_BLIZZARD_RA1
CFLAGS     += -fsingle-precision-constant
CFLAGS     += -I$(TW_DIR) -I$(INCLUDES)

LIBS       += driver
LIBS       += m
LIBS       += c
LIBS       += gcc
LIBS       += stdc++

LDFLAGS    += -T $(LD_SCRIPT)
LDFLAGS    += -g

LDFLAGS    += -L ${TW_DIR}/driverlib/gcc

ifeq (${FPU},hard)
LDFLAGS    += -L /usr/arm-none-eabi/lib/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16
LDFLAGS    += -L /usr/lib/gcc/arm-none-eabi/$(GCC_VER)/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16
else
LDFLAGS    += -L /usr/arm-none-eabi/lib/thumb/cortex-m4
LDFLAGS    += -L /usr/lib/gcc/arm-none-eabi/$(GCC_VER)/thumb/cortex-m4
endif


LDFLAGS    += --entry ResetISR
LDFLAGS    += --gc-sections
LDFLAGS    += -nostdlib
# Flag Definitions
###############################################################################

-include ${OBJECTS:.o=.d}

# Create the Directories we need
$(eval $(shell	$(MKDIR) bin))

# Object File Directory, keeps things tidy
OBJECTS    := $(patsubst src/%.o, bin/%.o, $(SOURCES:.c=.o) $(SOURCESCPP:.cpp=.o))
ASMS       := $(patsubst src/%.s, bin/%.s, $(SOURCES:.c=.o) $(SOURCESCPP:.cpp=.s))

###############################################################################
# Command Definitions, Leave it alone unless you hate yourself.

all: bin/$(TARGET).axf size

asm: $(ASMS)

# Compiler Command (C++)
bin/%.o: src/%.cpp
	$(CPP) -c $(CFLAGS) -fno-rtti -fno-exceptions -std=c++11 -o $@ $< -MT $@ -MT ${@:.o=.s}
	#$(CPP) -c $(CFLAGS) -fno-rtti -fno-exceptions -o $@ $< -MT $@ -MT ${@:.o=.s}

# Create Assembly (C++)
bin/%.s: src/%.cpp
	$(CPP) -S $(CFLAGS) -fno-rtti -fno-exceptions -std=c++11 -o $@ $< -MT ${@:.s=.o} -MT $@
	#$(CPP) -S $(CFLAGS) -fno-rtti -fno-exceptions -o $@ $< -MT ${@:.s=.o} -MT $@

# Compiler Command (C)
bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) -std=c99 -o $@ $< -MT $@ -MT ${@:.o=.s}

# Create Assembly (C)
bin/%.s: src/%.c
	$(CC) -S $(CFLAGS) -std=c99 -o $@ $< -MT ${@:.s=.o} -MT $@

# Linker Command
bin/$(TARGET).out: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(patsubst %,-l%, ${LIBS})

# Create the Final Image
bin/$(TARGET).axf: bin/$(TARGET).out
	$(OBJCOPY) -O binary bin/$(TARGET).out bin/$(TARGET).axf


# Calculate the Size of the Image
size: bin/$(TARGET).out
	$(SIZE) $<

clean:
	$(RM) -r bin

flash: all
	$(FLASH) bin/$(TARGET).axf;                                              

debug: flash 
	bash	./.debug

uart: flash
	${UART} /dev/lm4f 115200

# Command Definitions, Leave it alone unless you hate yourself.
###############################################################################
