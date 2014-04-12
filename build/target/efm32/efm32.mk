###############################################################################
# qkthings
###############################################################################

include $(TARGET_GLOBAL)

###############################################################################
# SOURCE
###############################################################################
INCLUDE_DIR += \
$(QKPROGRAM_DIR)/lib/hal/efm32 \
$(ENERGYMICRO)/CMSIS/Include \
$(ENERGYMICRO)/Device/EnergyMicro/EFM32G/Include \
$(ENERGYMICRO)/emlib/inc

C_SRC_DIR += \
$(QKPROGRAM_DIR)/lib/hal/efm32

#$(ENERGYMICRO)/emlib/src/em_ebi.c \

C_SRC += \
$(ENERGYMICRO)/Device/EnergyMicro/EFM32G/Source/system_efm32g.c \
$(ENERGYMICRO)/emlib/src/em_assert.c \
$(ENERGYMICRO)/emlib/src/em_system.c \
$(ENERGYMICRO)/emlib/src/em_cmu.c \
$(ENERGYMICRO)/emlib/src/em_emu.c \
$(ENERGYMICRO)/emlib/src/em_timer.c \
$(ENERGYMICRO)/emlib/src/em_gpio.c \
$(ENERGYMICRO)/emlib/src/em_usart.c

S_SRC += $(ENERGYMICRO)/Device/EnergyMicro/EFM32G/Source/GCC/startup_efm32g.S

