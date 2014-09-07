###############################################################################
# qkthings
###############################################################################

###############################################################################
# SOURCE
###############################################################################
INCLUDE_DIR += \
$(QKPROGRAM_DIR)/lib/hal/chibios\
$(QKPROGRAM_DIR)/lib/hal/chibios/$(TARGET_VARIANT)

C_SRC_DIR += \
$(QKPROGRAM_DIR)/lib/hal/chibios


###############################################################################
# FEATURES
###############################################################################

FEATURES += \
_QK_FEAT_FRAGMENTATION \
_QK_FEAT_UART_POLLING

DEFINES += $(FEATURES)






