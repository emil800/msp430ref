###########################################################
# Toolchain
###########################################################
CC = $(MSPGCC_BIN_DIR)/msp430-elf-gcc
RM = rm

###########################################################
# Directories
###########################################################
MSPGCC_ROOT_DIR = /Users/emil/ti/msp430-gcc
MSPGCC_BIN_DIR = $(MSPGCC_ROOT_DIR)/bin
INCLUDE_GCC_DIR = /Users/emil/ti/msp430-gcc/include /
INCLUDE_DIRS = $(INCLUDE_GCC_DIR)
LIB_DIRS = $(INCLUDE_GCC_DIR)
ROOT = .
SRC_DIR = $(ROOT)
OBJ_DIR = $(ROOT)/obj
BIN_DIR = $(ROOT)/bin

###########################################################
# Files
###########################################################
TARGET = $(BIN_DIR)/main.elf
SOURCES = main.c pwm.c adc.c #$(wildcard *.c)
OBJECT_NAMES = $(SOURCES:.c=.o)
OBJECTS = $(patsubst %,$(OBJ_DIR)/%,$(OBJECT_NAMES))

###########################################################
# Flags
###########################################################
MCU = msp430g2553
CFLAGS = -mmcu=$(MCU) $(addprefix -I,$(INCLUDE_DIRS))
LDFLAGS = -mmcu=$(MCU) $(addprefix -L,$(LIB_DIRS))

###########################################################
# Build
###########################################################
$(TARGET): $(OBJECTS)
	@echo "Linking $^"
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: %.c
	@echo "Compiling $^"
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c -o $@ $^

###########################################################
# Phony targets
###########################################################
.PHONY: all clean
###########################################################
# DEBUG
###########################################################

DEBUGGER = mspdebug rf2500

all: $(TARGET)

clean:
	$(RM) $(TARGET) $(OBJECTS)

debug: all
	$(DEBUGGER) $(BIN_DIR)/main.elf