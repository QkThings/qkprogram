###############################################################################
# QkThings. Arduino Makefile
###############################################################################

CC      = ..\..\toolchain\win\avr\bin\avr-gcc
OBJCOPY = ..\..\toolchain\win\avr\bin\avr-objcopy
DUMP    = ..\..\toolchain\win\avr\bin\avr-objdump
PSIZE	= ..\..\toolchain\win\avr\bin\avr-size

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
LDFLAGS = -Wl,--gc-sections $(CFLAGS)
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs
CFLAGS += -std=gnu99 -Wall -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums

###############################################################################
# AVRDUDE
###############################################################################
PORT = COM19
UPLOAD_RATE = 57600
AVRDUDE = ..\..\toolchain\win\avr\bin\avrdude
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


