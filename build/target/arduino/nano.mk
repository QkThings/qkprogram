
###############################################################################
# QkThings. Arduino Makefile
###############################################################################

ifeq ($(SHELLNAMES),)
CC      = $(TOOLCHAIN_DIR)/linux/avr/bin/avr-gcc
OBJCOPY = $(TOOLCHAIN_DIR)/linux/avr/bin/avr-objcopy
AR		= $(TOOLCHAIN_DIR)/linux/avr/bin/avr-ar
DUMP    = $(TOOLCHAIN_DIR)/linux/avr/bin/avr-objdump
PSIZE	= $(TOOLCHAIN_DIR)/linux/avr/bin/avr-size
else
CC      = $(TOOLCHAIN_DIR)/win/avr/bin/avr-gcc
OBJCOPY = $(TOOLCHAIN_DIR)/win/avr/bin/avr-objcopy
AR		= $(TOOLCHAIN_DIR)/win/avr/bin/avr-ar
DUMP    = $(TOOLCHAIN_DIR)/win/avr/bin/avr-objdump
PSIZE	= $(TOOLCHAIN_DIR)/win/avr/bin/avr-size
endif

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
$(QKPROGRAM_ROOT_DIR)/lib/hal/arduino

C_SRC_DIR += \
$(QKPROGRAM_ROOT_DIR)/lib/hal/arduino
###############################################################################
# FLAGS
###############################################################################
BOOTLOADER_ADDRESS=1F800
CFLAGS += -mmcu=$(MCU) -DF_CPU=$(F_CPU) -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections $(CFLAGS) 
LDFLAGS += -Wl,-u,vfprintf -lprintf_min # enable printf
#LDFLAGS += -Wl,-u,vfprintf -lprintf_flt -lm # printf with floating point support
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs
CFLAGS += -std=gnu99 -Wall -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -W -Wall -Wextra

###############################################################################
# AVRDUDE
###############################################################################
PORT = COM19
UPLOAD_RATE = 115200
ifeq ($(SHELLNAMES),)
AVRDUDE = $(TOOLCHAIN_DIR)/linux/avr/avrdude
else
AVRDUDE = $(TOOLCHAIN_DIR)/win/avr/avrdude
endif
AVRDUDE_CONF = $(TOOLCHAIN_DIR)/common/arduino
AVRDUDE_PROGRAMMER = stk500
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:$(FILE):i
AVRDUDE_VERBOSE = -V#-v -v -v -v 
AVRDUDE_FLAGS = -C$(AVRDUDE_CONF)/avrdude.conf $(AVRDUDE_VERBOSE) -p$(MCU) -carduino -P$(PORT) -b$(UPLOAD_RATE) -D 

###############################################################################
# UPLOAD
###############################################################################
UPLOAD_CMD = $(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)


