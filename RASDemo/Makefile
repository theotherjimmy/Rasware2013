all:
###############################################################################
# Configuration Variables

#Are you Jimmy?
Jimmy-P	    := yes

#Taget Binary Name
TARGET      := RASDemo

# List all the source files here
SOURCES     := $(wildcard *.c)

INCLUDES    := ../

# Path to the root of your ARM toolchain
TOOL        := $(shell dirname `which arm-none-eabi-gcc`)

# Path to the root of your StellarisWare folder
ifeq (${Jimmy-P},yes)
SW_DIR      := ${HOME}
else
TWD_DIR	:=  ../../TivaWare
endif

# Location of a linker script, doesnt matter which one, they're the same
LD_SCRIPT   := LM4F.ld

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
CFLAGS     += -O3
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
CFLAGS     += -I$(SW_DIR) -I$(INCLUDES)

LIBS	   += c
LIBS	   += RAS
LIBS       += driver-cm4f
LIBS       += utils-cm4f
LIBS	   += m
LIBS	   += gcc

LDFLAGS    += -T $(LD_SCRIPT)
LDFLAGS    += -g
LDFLAGS	   += -L ../RASLib/output/

LDFLAGS	   += -L ${SW_DIR}/StellarisWare/driverlib/gcc-cm4f/
LDFLAGS	   += -L ${SW_DIR}/StellarisWare/driverlib/gcc-cm4f/

ifeq (${FPU},hard)
LDFLAGS    += -L ${TOOL}/../arm-none-eabi/lib/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16/
LDFLAGS	   += -L /usr/lib/gcc/arm-none-eabi/4.8.0/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16/
else
LDFLAGS    += -L ${TOOL}/../arm-none-eabi/lib/thumb/cortex-m4/
LDFLAGS	   += -L /usr/lib/gcc/arm-none-eabi/4.8.0/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16/
endif
LDFLAGS	   +=

LDFLAGS    += --gc-sections
LDFLAGS    += -nostdlib
# Flag Definitions
###############################################################################

OBJECTS    := $(patsubst %.o, bin/%.o, $(SOURCES:.c=.o))
ASMS       := $(patsubst %.s, bin/%.s, $(SOURCES:.c=.s))

-include ${OBJECTS:.o=.d}

# Create the Directories we need
$(eval $(shell	$(MKDIR) bin))

# Object File Directory, keeps things tidy
###############################################################################
# Command Definitions, Leave it alone unless you hate yourself.

all: bin/$(TARGET).axf

asm: $(ASMS)

# Compiler Command
bin/%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $< -MT $@ -MT ${@:.o=.s}

# Create Assembly
bin/%.s: %.c
	$(CC) -S $(CFLAGS) -o $@ $< -MT ${@:.s=.o} -MT $@

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
	$(RM) bin

flash: bin/${TARGET}.axf
	$(FLASH) bin/$(TARGET).axf;                                              

debug: bin/${TARGET}.out
	bash	./.debug bin/${TARGET}.out

uart: flash
	${UART} /dev/lm4f 115200

# Command Definitions, Leave it alone unless you hate yourself.
###############################################################################
