###############################################################################
# qkthings
###############################################################################

###############################################################################
# SOURCE
###############################################################################
INCLUDE_DIR += \
$(QKPROGRAM_DIR)/lib/hal/efm32 \
$(ENERGYMICRO)/emlib/inc

#$(ENERGYMICRO)/CMSIS/Include 
#$(ENERGYMICRO)/Device/EnergyMicro/EFM32G/Include 

C_SRC_DIR += \
$(QKPROGRAM_DIR)/lib/hal/efm32

#$(ENERGYMICRO)/emlib/src/em_ebi.c 

C_SRC += \
$(ENERGYMICRO)/emlib/src/em_assert.c \
$(ENERGYMICRO)/emlib/src/em_system.c \
$(ENERGYMICRO)/emlib/src/em_cmu.c \
$(ENERGYMICRO)/emlib/src/em_emu.c \
$(ENERGYMICRO)/emlib/src/em_timer.c \
$(ENERGYMICRO)/emlib/src/em_gpio.c \
$(ENERGYMICRO)/emlib/src/em_usart.c

###############################################################################
# FEATURES
###############################################################################

FEATURES += \
_QK_FEAT_RTC \
_QK_FEAT_EEPROM \
_QK_FEAT_POWER_MANAGEMENT \
_QK_FEAT_FRAGMENTATION \
_QK_FEAT_CLOCK_SWITCHING

DEFINES += $(FEATURES)

#define _QK_DEFAULT_SAMPFREQ         (2)
#define _QK_MAX_FIRED_EVENTS         (8)
#define _QK_MAX_PENDING_EVENTS       (8)
#define _QK_EVENT_MAX_ARGS           (8)
#define _QK_ACTION_MAX_ARGS          (8)
#define _QK_PACKET_DATBUF_SIZE       (64)
#define _QK_BOARD_NAME_SIZE          (20)
#define _QK_LABEL_SIZE               (20)
#define QK_EVENT_NAME_SIZE          (16)
#define QK_CONFIG_NAME_SIZE         (10)
#define _QK_IDLE_TIMEOUT             (1)   // Idle timeout (s)
#define _QK_GOTOPD_TIMEOUT           (30)  // Go to power-down timeout (100's of ms)
#define _QK_WAITACK_TIMEOUT          (999) // Wait ACK timeout (ms)

#define _HAL_UART_RXBUF_SIZE         (32)

#define _HAL_UART_BAUD_DEFAULT_LOW   38400
#define _HAL_UART_BAUD_DEFAULT_HIGH  125000





