###############################################################################
# QkThings. Arduino Makefile
###############################################################################

# Program settings
CC = avr-gcc
CXX = avr-g++
OBJCOPY = avr-objcopy
OBJDUMP = avr-objdump
SIZE = avr-size
NM = avr-nm
AVRDUDE = C:\Users\mribeiro\Copy\qkthings\engineering\embedded\toolchain\win\avr\bin\avrdude.exe
REMOVE = rm -f
MV = mv -f

CC      = avr-gcc
OBJCOPY = avr-objcopy
DUMP    = avr-objdump
PSIZE	= avr-size

OPTIMIZE = s
FORMAT = ihex

###############################################################################
# CPU
###############################################################################
MCU = atmega328p
F_CPU = 16000000

###############################################################################
# SOURCE
###############################################################################
INCLUDE_DIR += \
$(ROOT_DIR)/src/hal/arduino

C_SRC_DIR += \
$(ROOT_DIR)/src/hal/arduino
###############################################################################
# FLAGS
###############################################################################
CFLAGS += -mmcu=$(MCU) -DF_CPU=$(F_CPU) -I. 
LDFLAGS = $(CFLAGS)
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 

###############################################################################
# AVRDUDE
###############################################################################
PORT = COM19
UPLOAD_RATE = 57600
AVRDUDE_CONF = ..\..\toolchain\common\arduino
AVRDUDE_PROGRAMMER = stk500
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:$(EXE_DIR)/$(PROJECTNAME).bin:i
AVRDUDE_VERBOSE = -V#-v -v -v -v 
AVRDUDE_FLAGS = -C$(AVRDUDE_CONF)\avrdude.conf $(AVRDUDE_VERBOSE) -p$(MCU) -carduino -P$(PORT) -b$(UPLOAD_RATE) -D 

###############################################################################
# UPLOAD
###############################################################################
UPLOAD_CMD = $(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)


