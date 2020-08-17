######################################
# target
######################################
TARGET = renard-phy-s2lp-demo-ht32sx

######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
BUILD_DIR = build
GDBCFG_DIR = gdb

CFGDIR_APP = $(abspath conf)
LIBDIR_APP = $(abspath lib)
SRCDIR_APP = $(abspath src)

######################################
# source
######################################
C_SOURCES = \
$(wildcard $(SRCDIR_APP)/*.c) \
$(wildcard $(LIBDIR_APP)/stm32lib/STM32L0xx_HAL_Driver/Src/*.c) \
$(LIBDIR_APP)/stm32lib/CMSIS/STM32L0xx/Source/Templates/system_stm32l0xx.c

ASM_SOURCES = $(LIBDIR_APP)/stm32lib/CMSIS/STM32L0xx/Source/Templates/gcc/startup_stm32l053xx.s

#######################################
# renard-phy-s2lp
#######################################
RENARD_PHY_S2LP_DIR := $(LIBDIR_APP)/renard-phy-s2lp/
RENARD_PHY_S2LP := $(RENARD_PHY_S2LP_DIR)renard-phy-s2lp.a

#######################################
# renard-phy-s2lp-hal-stm32
#######################################
RENARD_PHY_S2LP_HAL_STM32_DIR := $(LIBDIR_APP)/renard-phy-s2lp-hal-stm32/
RENARD_PHY_S2LP_HAL_STM32 := $(RENARD_PHY_S2LP_HAL_STM32_DIR)renard-phy-s2lp-hal-stm32.a

#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-

CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
AR = $(PREFIX)ar

HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

GDB = $(PREFIX)gdb
OPENOCD = openocd

#######################################
# CFLAGS
#######################################
# Architecture-specific configuration
ARCH_DEFS = -DUSE_HAL_DRIVER -DSTM32L053xx

ARCH_INCLUDES = \
-I$(CFGDIR_APP) \
-I$(LIBDIR_APP)/CMSIS_5/CMSIS/Core/Include \
-I$(LIBDIR_APP)/stm32lib/CMSIS/STM32L0xx/Include \
-I$(LIBDIR_APP)/stm32lib/STM32L0xx_HAL_Driver/Inc \

ARCH_MCU = -mcpu=cortex-m0plus -mthumb $(FLOAT-ABI) --specs=nano.specs

ARCHFLAGS = $(ARCH_DEFS) $(ARCH_INCLUDES) $(ARCH_MCU)

# C includes
C_INCLUDES = $(ARCH_INCLUDES) -I$(LIBDIR_APP)/renard-phy-s2lp/src -I$(LIBDIR_APP)/renard-phy-s2lp/librenard/src

# compile gcc flags
ASFLAGS = $(ARCH_MCU) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(ARCH_MCU) $(ARCH_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections -std=c99

# quick hack to generate different SNs for new compiles
CFLAGS += -DCOMPILE_TIME=`date '+%s'`

ifeq ($(DEBUG), 1)
CFLAGS += -g -ggdb -DDEBUG
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = $(CFGDIR_APP)/STM32L052T8YX_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys
LDFLAGS = $(ARCH_MCU) -T$(LDSCRIPT) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(RENARD_PHY_S2LP):
	$(MAKE) -C $(RENARD_PHY_S2LP_DIR) CC=$(CC) AR=$(AR) ARCHFLAGS="$(ARCHFLAGS)"

$(RENARD_PHY_S2LP_HAL_STM32):
	$(MAKE) -C $(RENARD_PHY_S2LP_HAL_STM32_DIR) CC=$(CC) AR=$(AR) ARCHFLAGS="$(ARCHFLAGS)"

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) $(RENARD_PHY_S2LP) $(RENARD_PHY_S2LP_HAL_STM32) Makefile
	$(CC) $(OBJECTS) $(RENARD_PHY_S2LP) $(RENARD_PHY_S2LP_HAL_STM32) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@

$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@

$(BUILD_DIR):
	mkdir $@

$(GDBCFG_DIR):
	mkdir $@

#######################################
# flash firmware to board
#######################################
openocd-flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash reset
	$(OPENOCD) -f interface/stlink-v2.cfg -f target/stm32l0.cfg -c "program $< reset exit 0x8000000"

stlink-flash: $(BUILD_DIR)/$(TARGET).bin
	st-flash write $< 0x8000000

#######################################
# debug using gdb + openocd
#######################################
# First start openocd, then only use gdb-flash and gdb-debug to connect to target, no stlink / openocd-flash!
openocd:
	openocd -f interface/stlink-v2.cfg -f target/stm32l0.cfg

gdb-flash: $(BUILD_DIR)/$(TARGET).elf $(GDBCFG_DIR)
	echo "target remote localhost:3333" > $(GDBCFG_DIR)/flash.cfg
	echo "monitor program $< reset" >> $(GDBCFG_DIR)/flash.cfg
	echo "quit" >> $(GDBCFG_DIR)/flash.cfg
	$(GDB) -x $(GDBCFG_DIR)/flash.cfg

gdb-debug: $(GDBCFG_DIR)
	echo "target remote localhost:3333" > $(GDBCFG_DIR)/debug.cfg
	echo "monitor reset init" >> $(GDBCFG_DIR)/debug.cfg
	$(GDB) -x $(GDBCFG_DIR)/debug.cfg $(BUILD_DIR)/$(TARGET).elf

#######################################
# clean up
#######################################
clean:
	$(MAKE) -C $(RENARD_PHY_S2LP_DIR) clean
	$(MAKE) -C $(RENARD_PHY_S2LP_HAL_STM32_DIR) clean
	-rm -fR $(BUILD_DIR)
	-rm -fR $(GDBCFG_DIR)

#######################################
# dependencies
#######################################
.PHONY: $(RENARD_PHY_S2LP) $(RENARD_PHY_S2LP_HAL_STM32)

-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***
