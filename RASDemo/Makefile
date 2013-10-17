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

# Path to the root of your StellarisWare folder
SW_DIR      := ${HOME}
INCLUDES    += ${SW_DIR}

# Location of the linker script
LD_SCRIPT   := LM4F.ld

# FPU Type
FPU          := hard
#FPU          = softfp

# Configuration Variables
###############################################################################


###############################################################################
# Tool Definitions
PREFIX      := arm-none-eabi

# Path to the root of your ARM toolchain
TOOL        := $(dir $(shell which ${PREFIX}-gcc))

CC          := $(TOOL)/${PREFIX}-gcc
LD          := $(TOOL)/${PREFIX}-ld
AR          := $(TOOL)/${PREFIX}-ar
AS          := $(TOOL)/${PREFIX}-as
NM          := $(TOOL)/${PREFIX}-nm
OBJCOPY     := $(TOOL)/${PREFIX}-objcopy
OBJDUMP     := $(TOOL)/${PREFIX}-objdump
RANLIB      := $(TOOL)/${PREFIX}-ranlib
STRIP       := $(TOOL)/${PREFIX}-strip
SIZE        := $(TOOL)/${PREFIX}-size
READELF     := $(TOOL)/${PREFIX}-readelf
DEBUG       := $(TOOL)/${PREFIX}-gdb
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
CFLAGS     += -g
CFLAGS     += -mthumb
CFLAGS     += -mcpu=cortex-m4
CFLAGS     += -mfpu=fpv4-sp-d16
CFLAGS     += -mfloat-abi=$(FPU)
CFLAGS     += -O3
CFLAGS     += -ffunction-sections
CFLAGS     += -fdata-sections
CFLAGS     += -MD
CFLAGS     += -std=gnu99
CFLAGS     += -Wall
CFLAGS     += -pedantic
CFLAGS     += -Dgcc
CFLAGS     += -DPART_LM4F120H5QR
CFLAGS     += -DTARGET_IS_BLIZZARD_RA1
CFLAGS     += -fsingle-precision-constant
CFLAGS     += $(addprefix -I, ${INCLUDES}) 

LIBS	   += c
LIBS	   += RAS
LIBS       += driver-cm4f
LIBS	   += m
LIBS	   += gcc

LDFLAGS    += -g
LDFLAGS    += --gc-sections
LDFLAGS	   += -L ../RASLib/output/

LDFLAGS	   += -L ${SW_DIR}/StellarisWare/driverlib/gcc-cm4f/
LDFLAGS	   += -L ${SW_DIR}/StellarisWare/utils/gcc-cm4f/

ifeq (${FPU},hard)
LDFLAGS    += -L ${TOOL}/../${PREFIX}/lib/thumb/cortex-m4/float-abi-hard/fpuv4-sp-d16/
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
	$(LD) -T ${LD_SCRIPT} $(LDFLAGS) -o $@ $(OBJECTS) $(patsubst %,-l%, ${LIBS})

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
