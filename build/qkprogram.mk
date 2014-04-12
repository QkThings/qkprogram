####################################################################
# qkthings                                                      
####################################################################

#TOOLCHAIN_DIR = ../../shared/toolchain
BUILD_TARGET ?= BUILD_DEVICE

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

CFLAGS += -D$(BUILD_TARGET)

include $(QKPROGRAM_DIR)/build/target/$(TARGET_NAME)/$(TARGET_NAME).mk





