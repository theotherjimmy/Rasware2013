###############################################################################
# Configuration Variables

#Are you Jimmy?
Jimmy-P	    := yes

#Taget Binary Name
TARGET      := RAS

# List all the source files here
SOURCES     := $(wildcard src/*.c)

# Includes are located in the Include directory
INCLUDES    := inc

# Path to the root of your ARM toolchain
TOOL        := $(shell dirname `which arm-none-eabi-gcc`)

# Path to the root of your StellarisWare folder
ifeq (${Jimmy-P},yes)
SW_DIR      := ${HOME}
else
TWD_DIR	:=  ../../TivaWare
endif

# Location of a linker script, doesnt matter which one, they're the same
LD_SCRIPT   := tm4c.ld

# FPU Type
FPU          := hard
#FPU          = softfp

# Configuration Variables
###############################################################################


###############################################################################
# Tool Definitions
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
FLASH       := $(shell which lm4flash)
CP          := cp -p
RM          := rm -rf
MV          := mv
MKDIR       := mkdir -p
ifeq (${Jimmy-P},yes)
UART	    := screen
else
UART	    := minicom
endif
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
CFLAGS     += -std=gnu99
CFLAGS     += -Wall
CFLAGS     += -pedantic
CFLAGS     += -g
CFLAGS     += -DPART_LM4F120H5QR
CFLAGS     += -Dgcc
CFLAGS     += -DTARGET_IS_BLIZZARD_RA1
CFLAGS     += -fsingle-precision-constant
ifeq (${Jimmy-P},yes)
CFLAGS     += -I$(SW_DIR) -I$(INCLUDES)
else
CFLAGS     += -I$(SW_DIR) -I$(INCLUDES)
endif
# Flag Definitions
###############################################################################

-include ${OBJECTS:.o=.d}

# Create the Directories we need
$(eval $(shell	$(MKDIR) bin))

# Object File Directory, keeps things tidy
OBJECTS    := $(patsubst src/%.o, bin/%.o, $(SOURCES:.c=.o))
ASMS       := $(patsubst src/%.s, bin/%.s, $(SOURCES:.c=.s))

###############################################################################
# Command Definitions, Leave it alone unless you hate yourself.

all: bin/lib$(TARGET).a 

asm: $(ASMS)

# Compiler Command
bin/%.o: src/%.c
	$(CC) -c $(CFLAGS) -o $@ $< -MT $@ -MT ${@:.o=.s}

# Create Assembly
bin/%.s: src/%.c
	$(CC) -S $(CFLAGS) -o $@ $< -MT ${@:.s=.o} -MT $@

# Create Library
bin/lib%.a : $(OBJECTS)
	$(AR) rc $@ $^

clean:
	$(RM) bin
# Command Definitions, Leave it alone unless you hate yourself.
###############################################################################
