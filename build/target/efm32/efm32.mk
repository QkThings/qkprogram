###############################################################################
# QkThings. EFM32 Makefile
###############################################################################

ENERGYMICRO = $(TOOLCHAIN_DIR)/common/energymicro
#WINDOWSCS  ?= GNU Tools ARM Embedded\4.7 2013q1
#LINUXCS    ?= /home/mribeiro/gcc-arm-none-eabi-4_7-2013q1
DEVICE = EFM32G890F128

# Try autodetecting the environment
ifeq ($(SHELLNAMES),)
  # Assume we are making on a Linux platform
  #TOOLDIR := $(LINUXCS)
  TOOLDIR := $(TOOLCHAIN_DIR)/linux/arm/gcc
else
  QUOTE :="
  ifneq ($(COMSPEC),)
    # Assume we are making on a mingw/msys/cygwin platform running on Windows
    # This is a convenient place to override TOOLDIR, DO NOT add trailing
    # whitespace chars, they do matter !
    #TOOLDIR := $(PROGRAMFILES)/$(WINDOWSCS)
    TOOLDIR := $(TOOLCHAIN_DIR)/win/arm/gcc
    ifeq ($(findstring cygdrive,$(shell set)),)
      # We were not on a cygwin platform
      NULLDEVICE := NUL
    endif
  else
    # Assume we are making on a Windows platform
    # This is a convenient place to override TOOLDIR, DO NOT add trailing
    # whitespace chars, they do matter !
    SHELL      := $(SHELLNAMES)
    #TOOLDIR    := $(ProgramFiles)/$(WINDOWSCS)
    TOOLDIR := $(TOOLCHAIN_DIR)/win/arm/gcc
    RMDIRS     := rd /s /q
    RMFILES    := del /s /q
    ALLFILES   := \*.*
    NULLDEVICE := NUL
  endif
endif


CC      = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-gcc$(QUOTE)
LD      = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-ld$(QUOTE)
AR      = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-ar$(QUOTE)
OBJCOPY = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-objcopy$(QUOTE)
DUMP    = $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-objdump$(QUOTE)
PSIZE	= $(QUOTE)$(TOOLDIR)/bin/arm-none-eabi-size$(QUOTE)


###############################################################################
# SOURCE
###############################################################################
INCLUDE_DIR += \
$(ROOT_DIR)/src/hal/efm32 \
$(ENERGYMICRO)/CMSIS/Include \
$(ENERGYMICRO)/Device/EnergyMicro/EFM32G/Include \
$(ENERGYMICRO)/emlib/inc

C_SRC_DIR += \
$(ROOT_DIR)/src/hal/efm32

C_SRC += \
$(ENERGYMICRO)/Device/EnergyMicro/EFM32G/Source/system_efm32g.c \
$(ENERGYMICRO)/emlib/src/em_assert.c \
$(ENERGYMICRO)/emlib/src/em_cmu.c \
$(ENERGYMICRO)/emlib/src/em_ebi.c \
$(ENERGYMICRO)/emlib/src/em_emu.c \
$(ENERGYMICRO)/emlib/src/em_timer.c \
$(ENERGYMICRO)/emlib/src/em_gpio.c \
$(ENERGYMICRO)/emlib/src/em_system.c \
$(ENERGYMICRO)/emlib/src/em_usart.c

S_SRC += $(ENERGYMICRO)/Device/EnergyMicro/EFM32G/Source/GCC/startup_efm32g.S

###############################################################################
# FLAGS
###############################################################################

# -MMD : Don't generate dependencies on system header files.
# -MP  : Add phony targets, useful when a h-file is removed from a project.
# -MF  : Specify a file to write the dependencies to.
DEPFLAGS = -MMD -MP -MF $(@:.o=.d)


# Add -Wa,-ahld=$(LST_DIR)/$(@F:.o=.lst) to CFLAGS to produce assembly list files

CFLAGS += -D$(DEVICE) -Wall -Wextra -mcpu=cortex-m3 -mthumb \
-mfix-cortex-m3-ldrd -ffunction-sections \
-fdata-sections -fomit-frame-pointer -DDEBUG_EFM -DNDEBUG \
$(DEPFLAGS)

ASMFLAGS += -x assembler-with-cpp -mcpu=cortex-m3 -mthumb


# NOTE: The -Wl,--gc-sections flag may interfere with debugging using gdb.

LDFLAGS += -Xlinker -Map=$(LST_DIR)/$(PROJECTNAME).map -mcpu=cortex-m3 \
-mthumb -T$(ENERGYMICRO)/Device/EnergyMicro/EFM32G/Source/GCC/efm32g.ld \
 -Wl,--gc-sections

LIBS = -lm -Wl,--start-group -lgcc -lc -lnosys   -Wl,--end-group 

