###############################################################################
# qkthings
###############################################################################

include $(TARGET_GLOBAL)

###############################################################################
# SOURCE
###############################################################################
INCLUDE_DIR += \
$(QKPROGRAM_DIR)/lib/hal/derfmega 

C_SRC_DIR += \
$(QKPROGRAM_DIR)/lib/hal/derfmega

C_SRC += 

FEATURES = \
_QK_FEAT_RTC \
_QK_FEAT_EEPROM \
_QK_FEAT_POWER_MANAGEMENT \
_QK_FEAT_FRAGMENTATION
