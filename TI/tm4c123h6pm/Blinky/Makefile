#Taget Binary Name
TARGET      = main

# List all the source files here
SOURCES     = $(wildcard src/*.c)

# Includes are located in the Include directory
INCLUDES    = -Iinc

# Path to the root of your ARM toolchain
TOOL        = $(shell dirname `which arm-none-eabi-gcc`)

# Path to the root of your StellarisWare folder
TW_DIR      = ../../TivaWare

# Location of a loader script, doesnt matter which one, they're the same
LD_SCRIPT   = lm4f.ld

# Object File Directory, keeps things tidy
OBJECTS     = $(patsubst %.o, %.o, $(SOURCES:.c=.o))
ASMS        = $(patsubst %.s, %.s, $(SOURCES:.c=.s))

# FPU Type
#FPU          = hard
FPU          = softfp

# Remove # to enable verbose
VERBOSE      = #1


# Tool Definitions
###############################################################################
CC          = $(TOOL)/arm-none-eabi-gcc
LD          = $(TOOL)/arm-none-eabi-ld
AR          = $(TOOL)/arm-none-eabi-ar
AS          = $(TOOL)/arm-none-eabi-as
NM          = $(TOOL)/arm-none-eabi-nm
OBJCOPY     = $(TOOL)/arm-none-eabi-objcopy
OBJDUMP     = $(TOOL)/arm-none-eabi-objdump
RANLIB      = $(TOOL)/arm-none-eabi-ranlib
STRIP       = $(TOOL)/arm-none-eabi-strip
SIZE        = $(TOOL)/arm-none-eabi-size
READELF     = $(TOOL)/arm-none-eabi-readelf
DEBUG       = $(TOOL)/arm-none-eabi-gdb
FLASH       = $(shell which lm4flash)
CP          = cp -p
RM          = rm -rf
MV          = mv
MKDIR       = mkdir -p
###############################################################################


# Flag Definitions
###############################################################################
CFLAGS     += -mthumb
CFLAGS     += -mcpu=cortex-m4
CFLAGS     += -mfloat-abi=$(FPU)
CFLAGS     += -mfpu=fpv4-sp-d16
CFLAGS     += -Os
CFLAGS     += -ffunction-sections
CFLAGS     += -fdata-sections
CFLAGS     += -MD
CFLAGS     += -std=c99
CFLAGS     += -Wall
CFLAGS     += -pedantic
CFLAGS     += -g
CFLAGS     += -DPART_LM4F120H5QR
CFLAGS     += -Dgcc
CFLAGS     += -DTARGET_IS_BLIZZARD_RA1
CFLAGS     += -fsingle-precision-constant
CFLAGS     += -I$(TW_DIR) $(INCLUDES)

ifeq ($(FPU),hard)
	LIBGCC  = $(TOOL)/../lib/gcc/arm-none-eabi/$(shell $(CC) --version | grep gcc | sed "s/^.* //g")/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16/libgcc.a
	LIBM    = $(TOOL)/../arm-none-eabi/lib/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16/libm.a
	LIBC    = $(TOOL)/../arm-none-eabi/lib/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16/libc.a
	#DRIVER_LIB	= $(TW_DIR)/driverlib/gcc-hard/libdriver-hard.a
else
	LIBGCC  = $(TOOL)/../lib/gcc/arm-none-eabi/$(shell $(CC) --version | grep gcc | sed "s/^.* //g")/thumb/cortex-m4/libgcc.a
	LIBM    = $(TOOL)/../arm-none-eabi/lib/thumb/cortex-m4/libm.a
	LIBC    = $(TOOL)/../arm-none-eabi/lib/thumb/cortex-m4/libc.a
endif

DRIVER_LIB	= $(TW_DIR)/driverlib/gcc/libdriver.a

LIBS        = '$(LIBM)' '$(LIBC)' '$(LIBGCC)' '$(DRIVER_LIB)'

LDFLAGS    += -T $(LD_SCRIPT)
LDFLAGS    += -g
LDFLAGS    += --entry ResetISR
LDFLAGS    += --gc-sections
LDFLAGS    += -Map bin/$(TARGET).map
LDFLAGS    += --cref
LDFLAGS    += -nostdlib

# Command Definitions, Leave it alone unless you hate yourself.
###############################################################################
all: dirs bin/$(TARGET).bin bin/$(TARGET).axf bin/$(TARGET).lst size

asm: $(ASMS)

# Compiler Command
bin/%.o: %.c
#	@if [ 'x${VERBOSE}' = x ];                                                \
	 then                                                                     \
	     echo "CC           ${<}";                                            \
	 else                                                                     \
	     echo $(CC) -c $(CFLAGS) -o $@ $<;                                    \
	 fi
	$(MKDIR) $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<

# Create Assembly
bin/%.s: %.c
#	@if [ 'x${VERBOSE}' = x ];                                                \
	 then                                                                     \
	     echo "CC -S        ${<}";                                            \
	 else                                                                     \
	     echo $(CC) -S -c $(CFLAGS) -o $@ $<;                                 \
	 fi
	$(MKDIR) $(dir $@)
	$(CC) -c $(CFLAGS) -o $@ $<

# Linker Command
bin/$(TARGET).out: $(OBJECTS)
#	@if [ 'x${VERBOSE}' = x ];                                                \
	 then                                                                     \
	     echo "LD           $@";                                              \
	 else                                                                     \
	     echo $(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS);                      \
	 fi
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

# Create the Final Image
bin/$(TARGET).bin: bin/$(TARGET).out
# 	@if [ 'x${VERBOSE}' = x ];                                                \
	 then                                                                     \
	     echo "OBJCOPY      ${<} $@";                                         \
	 else                                                                     \
	     echo $(OBJCOPY) -O binary bin/$(TARGET).out bin/$(TARGET).bin;       \
	 fi
	$(OBJCOPY) -O binary bin/$(TARGET).out bin/$(TARGET).bin

# Create files for gdb
bin/$(TARGET).axf: bin/$(TARGET).out
	$(CP) bin/$(TARGET).out bin/$(TARGET).axf                                 

# Create files for gdb
bin/$(TARGET).lst: bin/$(TARGET).axf
	$(OBJDUMP) -S bin/$(TARGET).axf > bin/$(TARGET).lst                          


# Calculate the Size of the Image
size: bin/$(TARGET).out
#	@if [ 'x${VERBOSE}' = x ];                                                \
	 then                                                                     \
	     echo "SIZE      ${<}";                                               \
	 else                                                                     \
	     echo $(SIZE) $<;                                                     \
	 fi
	$(SIZE) $<


# Create the Directories we need
dirs:
	$(MKDIR) bin

# Cleanup
clean:
	$(RM) bin src/*.o src/*.d

# Flash The Board
flash: all
#	@if [ 'x${VERBOSE}' = x ];                                                \
	 then                                                                     \
	     echo "  FLASH    bin/$(TARGET).bin";                                 \
	 else                                                                     \
	     echo $(FLASH) bin/$(TARGET).bin;                                     \
	 fi
	$(FLASH) bin/$(TARGET).bin;                                              \
  $(MV) src/*.o src/*.d bin



# Start OpenOCD and run GDB
debug:
	./.debug


# Redo, Clean->Compile Fresh Image, and Install It.
redo: clean all flash
###############################################################################

