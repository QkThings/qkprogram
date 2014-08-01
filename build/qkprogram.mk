####################################################################
# qkthings                                                      
####################################################################

####################################################################
# SOURCE                  
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

####################################################################
# TARGET                
####################################################################

BUILD_TARGET ?= BUILD_DEVICE
$(info BUILD_TARGET = $(BUILD_TARGET))

DEFINES += $(BUILD_TARGET) 

include $(TARGET_GLOBAL)
include $(QKPROGRAM_DIR)/build/target/$(TARGET_NAME)/$(TARGET_NAME).mk

####################################################################
# LIB DIR                
####################################################################

ifeq ($(BUILD_TARGET), BUILD_DEVICE)
LIB_DIR := $(LIB_DIR)/device
endif
ifeq ($(BUILD_TARGET), BUILD_COMM)
LIB_DIR := $(LIB_DIR)/comm
endif



