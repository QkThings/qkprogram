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


include $(QKPROGRAM_DIR)/build/target/$(TARGET_NAME)/$(TARGET_NAME).mk

DEFINES += $(BUILD_TARGET) $(FEATURES)






