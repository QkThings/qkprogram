# Arduino makefile
#
# This makefile allows you to build sketches from the command line
# without the Arduino environment (or Java).
#
# The Arduino environment does preliminary processing on a sketch before
# compiling it.  If you're using this makefile instead, you'll need to do
# a few things differently:
#
#   - Give your program's file a .cpp extension (e.g. foo.cpp).
#
#   - Put this line at top of your code: #include <WProgram.h>
#
#   - Write prototypes for all your functions (or define them before you
#     call them).  A prototype declares the types of parameters a
#     function will take and what type of value it will return.  This
#     means that you can have a call to a function before the definition
#     of the function.  A function prototype looks like the first line of
#     the function, with a semi-colon at the end.  For example:
#     int digitalRead(int pin);
#
# Instructions for using the makefile:
#
#  1. Copy this file into the folder with your sketch.
#
#  2. Below, modify the line containing "TARGET" to refer to the name of
#     of your program's file without an extension (e.g. TARGET = foo).
#
#  3. Modify the line containg "ARDUINO" to point the directory that
#     contains the Arduino core (for normal Arduino installations, this
#     is the lib/targets/arduino sub-directory).
#
#  4. Modify the line containing "PORT" to refer to the filename
#     representing the USB or serial connection to your Arduino board
#     (e.g. PORT = /dev/tty.USB0).  If the exact name of this file
#     changes, you can use * as a wildcard (e.g. PORT = /dev/tty.USB*).
#
#  5. At the command line, change to the directory containing your
#     program's file and the makefile.
#
#  6. Type "make" and press enter to compile/verify your program.
#
#  7. Type "make upload", reset your Arduino board, and press enter  to
#     upload your program to the Arduino board.
#
# $Id$

PORT = COM3
TARGET = atmega
ARDUINO = 
SRC = $(TARGET).c
CXXSRC = 
MCU = atmega168
F_CPU = 16000000
FORMAT = ihex
UPLOAD_RATE = 19200
AVRDUDE_CONF = C:\Users\mribeiro\Copy\qkthings\engineering\embedded\toolchain\common\arduino


INCLUDE_DIR += \
$(ROOT_DIR)/src/hal/arduino

C_SRC_DIR += \
$(ROOT_DIR)/src/hal/arduino

#OPTIMIZE = s

# Place -D or -U options here
CDEFS = -DF_CPU=$(F_CPU)
CXXDEFS = -DF_CPU=$(F_CPU)

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = #-std=gnu99
CDEBUG = #-g$(DEBUG)
CWARN = #-Wall -Wstrict-prototypes
CTUNING = #-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
#CEXTRA = -Wa,-adhlns=$(<:.c=.lst)

CFLAGS += $(CDEBUG) $(CDEFS) $(CWARN) $(CSTANDARD) $(CEXTRA)
CFLAGS += -mmcu=$(MCU) -I.
#ASFLAGS = -Wa,-adhlns=$(<:.S=.lst),-gstabs 
LDFLAGS = $(CFLAGS)

# Programming support using avrdude. Settings and variables.
AVRDUDE_PROGRAMMER = stk500
AVRDUDE_PORT = $(PORT)
AVRDUDE_WRITE_FLASH = -U flash:w:$(EXE_DIR)/$(PROJECTNAME).bin:i
AVRDUDE_VERBOSE = #-v -v -v -v 
AVRDUDE_FLAGS = -C$(AVRDUDE_CONF)\avrdude.conf $(AVRDUDE_VERBOSE) -p$(MCU) -carduino -P$(PORT) -b$(UPLOAD_RATE) -D 


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

UPLOAD_CMD = $(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH)


