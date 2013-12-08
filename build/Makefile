####################################################################
# QkThings                                                         
####################################################################

TARGET = arduino
TOOLCHAIN_DIR = ../../shared/toolchain

####################################################################
# DEFINITIONS                                                      
####################################################################

.SUFFIXES:
.PHONY: all debug release clean upload

PROJECTNAME = qkprogram
OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = lib

OPTIMIZE = 0
FORMAT = binary

include vars.mk
VERSION_STR = "$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_PATCH)"

# Try autodetecting the environment
ifeq ($(SHELLNAMES),)
  # Assume we are making on a Linux platform
else
  QUOTE :="
  ifneq ($(COMSPEC),)
    # Assume we are making on a mingw/msys/cygwin platform running on Windows
    # This is a convenient place to override TOOLDIR, DO NOT add trailing
    # whitespace chars, they do matter !
    ifeq ($(findstring cygdrive,$(shell set)),)
      # We were not on a cygwin platform
      NULLDEVICE := NUL
    endif
  else
    # Assume we are making on a Windows platform
    # This is a convenient place to override TOOLDIR, DO NOT add trailing
    # whitespace chars, they do matter !
    SHELL      := $(SHELLNAMES)
    RMDIRS     := rd /s /q
    RMFILES    := del /s /q
    ALLFILES   := \*.*
    NULLDEVICE := NUL
  endif
endif

RMDIRS     := rm -rf
RMFILES    := rm -rf
ALLFILES   := /*.*
NULLDEVICE := /dev/null
SHELLNAMES := $(ComSpec)$(COMSPEC)

####################################################################
# DIRS AND FLAGS                     
####################################################################

ROOT_DIR = ..
TEST_DIR = $(ROOT_DIR)/test

INCLUDE_DIR += \
$(ROOT_DIR)/include \
$(ROOT_DIR)/src/sys \
$(ROOT_DIR)/src/hal \
$(ROOT_DIR)/src/core \
$(ROOT_DIR)/src/utils 

C_SRC_DIR += \
$(ROOT_DIR)/src/hal \
$(ROOT_DIR)/src/core #\
#$(ROOT_DIR)/test/arduino

s_SRC += 
S_SRC += 

C_SRC = $(wildcard $(addsuffix /*.c, $(C_SRC_DIR)))
C_SRC += $(MAIN)

include target/$(TARGET).mk

CFLAGS += -O$(OPTIMIZE) -g3 -Wall
ARFLAGS = rc

INCLUDEPATHS = $(addprefix -I, $(INCLUDE_DIR))

# Create directories and do a clean which is compatible with parallell make
$(shell mkdir $(OBJ_DIR)>$(NULLDEVICE) 2>&1)
$(shell mkdir $(BIN_DIR)>$(NULLDEVICE) 2>&1)
$(shell mkdir $(LIB_DIR)>$(NULLDEVICE) 2>&1)
ifeq (clean,$(findstring clean, $(MAKECMDGOALS)))
  ifneq ($(filter $(MAKECMDGOALS),all debug release),)
    $(shell $(RMFILES) $(OBJ_DIR)$(ALLFILES)>$(NULLDEVICE) 2>&1)
    $(shell $(RMFILES) $(BIN_DIR)$(ALLFILES)>$(NULLDEVICE) 2>&1)
    $(shell $(RMFILES) $(LST_DIR)$(ALLFILES)>$(NULLDEVICE) 2>&1)
  endif
endif

###############################################################################
# FILES
###############################################################################

C_FILES = $(notdir $(C_SRC) )
S_FILES = $(notdir $(S_SRC) $(s_SRC) )
C_PATHS = $(sort $(dir $(C_SRC) ) )
S_PATHS = $(sort $(dir $(S_SRC) $(s_SRC) ) )
C_OBJS = $(addprefix $(OBJ_DIR)/, $(C_FILES:.c=.o))
C_DEPS = $(addprefix $(OBJ_DIR)/, $(C_FILES:.c=.d))
S_OBJS = $(if $(S_SRC), $(addprefix $(OBJ_DIR)/, $(S_FILES:.S=.o)))
s_OBJS = $(if $(s_SRC), $(addprefix $(OBJ_DIR)/, $(S_FILES:.s=.o)))
OBJS = $(C_OBJS) $(S_OBJS) $(s_OBJS)

vpath %.c $(C_PATHS)
vpath %.s $(S_PATHS)
vpath %.S $(S_PATHS)

###############################################################################
# BUILD
###############################################################################
all: lib

lib:	$(LIB_DIR)/$(PROJECTNAME).a
test:	$(BIN_DIR)/$(PROJECTNAME).bin

# Create objects from C SRC files
$(OBJ_DIR)/%.o: %.c
	@echo Compiling $<
	$(CC) $(CFLAGS) $(INCLUDEPATHS) -c -o $@ $<

# Assemble .s/.S files
$(OBJ_DIR)/%.o: %.s
	@echo Assembling $<
	$(CC) $(ASMFLAGS) $(INCLUDEPATHS) -c -o $@ $<

$(OBJ_DIR)/%.o: %.S
	@echo Assembling $<
	$(CC) $(ASMFLAGS) $(INCLUDEPATHS) -c -o $@ $<

# Link
$(BIN_DIR)/$(PROJECTNAME).out: $(OBJS)
	@echo Linking target: $@
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $(BIN_DIR)/$(PROJECTNAME).out
	
# Create static library
$(LIB_DIR)/$(PROJECTNAME).a: $(OBJS)
	@echo Creating static library: $@
	$(AR) $(ARFLAGS) $(LIB_DIR)/$(PROJECTNAME).a $(OBJS)

# Create binary file
$(BIN_DIR)/$(PROJECTNAME).bin: $(BIN_DIR)/$(PROJECTNAME).out
	@echo Creating binary file
	$(OBJCOPY) -O $(FORMAT) $(BIN_DIR)/$(PROJECTNAME).out $(BIN_DIR)/$(PROJECTNAME).bin
	$(PSIZE) $(BIN_DIR)/$(PROJECTNAME).out
# Uncomment next line to produce assembly listing of entire program
#	$(DUMP) -h -S -C $(BIN_DIR)/$(PROJECTNAME).out>$(LST_DIR)/$(PROJECTNAME)out.lst

clean:
ifeq ($(filter $(MAKECMDGOALS),all debug release),)
	$(RMDIRS) $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)
endif

# include auto-generated dependency files (explicit rules)
ifneq (clean,$(findstring clean, $(MAKECMDGOALS)))
-include $(C_DEPS)
endif

###############################################################################
# UPLOAD
###############################################################################
PORT ?= COM19
FILE ?= $(BIN_DIR)/$(PROJECTNAME).bin
upload:
	$(UPLOAD_CMD)

