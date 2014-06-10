####################################################################
# qkthings                                                      
####################################################################

#TOOLCHAIN_DIR = ../../shared/toolchain

BUILD_TARGET ?= BUILD_DEVICE

$(info BUILD_TARGET = $(BUILD_TARGET))

ifeq ($(BUILD_TARGET), BUILD_DEVICE)
LIB_DIR := $(LIB_DIR)/device
endif
ifeq ($(BUILD_TARGET), BUILD_COMM)
LIB_DIR := $(LIB_DIR)/comm
endif

####################################################################
# DIRS AND FLAGS                     
####################################################################

QKPROGRAM_DIR = qkprogram

INCLUDE_DIR += \
$(QKPROGRAM_DIR)/include \
$(QKPROGRAM_DIR)/lib/sys \
$(QKPROGRAM_DIR)/lib/hal/include \
$(QKPROGRAM_DIR)/lib/core/include \
$(QKPROGRAM_DIR)/lib/protocol/include \
$(QKPROGRAM_DIR)/lib/utils/include

C_SRC_DIR += \
$(QKPROGRAM_DIR)/lib/hal \
$(QKPROGRAM_DIR)/lib/core \
$(QKPROGRAM_DIR)/lib/protocol \
$(QKPROGRAM_DIR)/lib/utils

s_SRC += 
S_SRC += 

# Default constants
#_QK_MAX_PENDING_EVENTS
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

include $(QKPROGRAM_DIR)/build/target/$(TARGET_NAME)/$(TARGET_NAME).mk

DEFINES += $(BUILD_TARGET) $(FEATURES) $(CONSTANTS) $(SETTINGS)






