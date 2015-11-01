PROJECT_NAME = qkprogram

LIB_INCLUDE_DIRS += \
/include \
/src/include \
/src/protocol/include \
/src/utils/include \
/src/hal/include \
/src/hal/$(TARGET_CPU)

LIB_C_SRC_DIRS += \
/src \
/src/hal \
/src/protocol \
/src/utils

BUILD_TARGET := BUILD_DEVICE

$(info BUILD_TARGET  = $(BUILD_TARGET))

DEFINES += $(BUILD_TARGET) 

ifeq ($(BUILD_TARGET), BUILD_DEVICE)
LIB_BUILD_DIR = $(TARGET_CPU)/$(TARGET_BOARD)/device
else
ifeq ($(BUILD_TARGET), BUILD_COMM)
LIB_BUILD_DIR = $(TARGET_CPU)/$(TARGET_BOARD)/comm
else
$(error BUILD_TARGET is neither BUILD_DEVICE nor BUILD_COMM)
endif
endif

include $(LIB)/target/$(TARGET_CPU)/board/$(TARGET_BOARD).mk
include $(LIB)/target/$(TARGET_CPU)/$(TARGET_CPU).mk

ifeq ($(USE_QKPERIPHERAL), 1)
$(info QK_PROGRAM_USE_PERIPH)

INCLUDE_DIRS += qkperipheral/include

LIBS += -L"qkperipheral/build/$(TARGET_CPU)/$(TARGET_BOARD)/lib" -lqkperipheral

DEFINES += QK_PROGRAM_USE_PERIPH
endif




