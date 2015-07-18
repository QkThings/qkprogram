####################################################################
# qkthings                                                      
####################################################################

PROJECT_NAME = qkprogram

####################################################################
# SOURCE                  
####################################################################

QKPROGRAM_DIR = qkprogram
QKPERIPHERAL_DIR = qkperipheral

INCLUDE_DIRS += \
$(QKPROGRAM_DIR)/include \
$(QKPROGRAM_DIR)/src/include \
$(QKPROGRAM_DIR)/src/hal/include \
$(QKPROGRAM_DIR)/src/protocol/include \
$(QKPROGRAM_DIR)/src/utils/include

C_SRC_DIRS += \
$(QKPROGRAM_DIR)/src \
$(QKPROGRAM_DIR)/src/hal \
$(QKPROGRAM_DIR)/src/protocol \
$(QKPROGRAM_DIR)/src/utils

s_SRC_FILES += 
S_SRC_FILES += 

####################################################################
# TARGET                
####################################################################

BUILD_TARGET := BUILD_DEVICE
$(info BUILD_TARGET  = $(BUILD_TARGET))

DEFINES += $(BUILD_TARGET) 

ifneq ($(TARGET),)
#include $(TARGET_GLOBAL)
#include $(QKPROGRAM_DIR)/target/$(TARGET_CPU)/$(TARGET_CPU).mk
INCLUDE_DIRS += \
$(QKPROGRAM_DIR)/src/hal/$(TARGET_CPU)
endif

ifeq ($(USE_PERIPH), 1)
$(info QK_PROGRAM_USE_PERIPH)

INCLUDE_DIRS += qkperipheral/include

LIBS += -L"qkperipheral/build/$(TARGET_CPU)/$(TARGET_BOARD)/lib" -lqkperipheral

DEFINES += QK_PROGRAM_USE_PERIPH
endif

####################################################################
# LIBS              
####################################################################

#ifneq
#LIBS += \
#-Lqkperipheral/build/lib/$(TARGET_NAME)/$(TARGET_VARIANT) \
#-lqkperipheral

#ifeq ($(BUILD_TARGET), BUILD_DEVICE)
#LIB_DIR := $(LIB_DIR)/device
#endif
#ifeq ($(BUILD_TARGET), BUILD_COMM)
#LIB_DIR := $(LIB_DIR)/comm
#endif



