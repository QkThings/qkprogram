###############################################################################
# qkthings
###############################################################################

include $(TARGET_GLOBAL)

# Build specific properties
_TAL_TYPE = ATMEGARF_TAL_1
_PAL_TYPE = ATMEGA128RFA1
_PAL_GENERIC_TYPE = MEGA_RF
_BOARD_TYPE = deRFmega128_22X00_deRFtoRCB_SENS_TERM_BOARD
_HIGHEST_STACK_LAYER = MAC

PATH_ROOT = /home/mribeiro/Copy/Atmel/MAC_v_2_6_1
PATH_TAL = $(PATH_ROOT)/TAL
PATH_MAC = $(PATH_ROOT)/MAC
PATH_TAL_CB = $(PATH_ROOT)/TAL/Src
PATH_PAL = $(PATH_ROOT)/PAL
PATH_RES = $(PATH_ROOT)/Resources
PATH_GLOB_INC = $(PATH_ROOT)/Includes
PATH_SIO_SUPPORT = $(PATH_ROOT)/Applications/Helper_Files/SIO_Support

###############################################################################
# FLAGS
###############################################################################

CFLAGS += -DDEBUG=0
CFLAGS += -DSIO_HUB -DUSB0
CFLAGS += -DFFD
CFLAGS += -DREDUCED_PARAM_CHECK
CFLAGS += -DHIGH_DATA_RATE_SUPPORT
CFLAGS += -DTAL_TYPE=$(_TAL_TYPE)
CFLAGS += -DPAL_GENERIC_TYPE=$(_PAL_GENERIC_TYPE)
CFLAGS += -DPAL_TYPE=$(_PAL_TYPE)
CFLAGS += -DVENDOR_BOARDTYPES=1
CFLAGS += -DBOARD_TYPE=$(_BOARD_TYPE)
CFLAGS += -DHIGHEST_STACK_LAYER=$(_HIGHEST_STACK_LAYER)

###############################################################################
# SOURCE
###############################################################################
INCLUDE_DIR += \
$(QKPROGRAM_DIR)/lib/hal/derfmega \
$(PATH_SIO_SUPPORT)/Inc \
$(PATH_ROOT)/Include \
$(PATH_ROOT)/MAC/Inc \
$(PATH_ROOT)/TAL/Inc/ \
$(PATH_ROOT)/TAL/$(_TAL_TYPE)/Inc/ \
$(PATH_ROOT)/PAL/Inc \
$(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/Generic/Inc \
$(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Inc/ \
$(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards \
$(PATH_ROOT)/PAL/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)

C_SRC_DIR += \
$(QKPROGRAM_DIR)/lib/hal/derfmega \
$(PATH_SIO_SUPPORT)/Src \
$(PATH_PAL)/$(_PAL_GENERIC_TYPE)/Generic/Src \
$(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Src \
$(PATH_PAL)/$(_PAL_GENERIC_TYPE)/$(_PAL_TYPE)/Boards/$(_BOARD_TYPE)

C_SRC += 

###############################################################################
# FEATURES
###############################################################################

FEATURES = \
_QK_FEAT_RTC \
_QK_FEAT_EEPROM \
_QK_FEAT_POWER_MANAGEMENT \
_QK_FEAT_FRAGMENTATION

SETTINGS = \
_QK_HAL_UART_POLLING
