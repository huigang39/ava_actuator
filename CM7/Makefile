# ---------------------------------------------------------------------------- #
#                                     工程命名                                 #
# ---------------------------------------------------------------------------- #
PROJECT   := actuator_ctrl
DIR_BUILD := build
C         ?= ac6
T         ?= D
M         ?= 0

# ---------------------------------------------------------------------------- #
#                                     目录引用                                 #
# ---------------------------------------------------------------------------- #
DIR_MODULE += ../module
DIR_USER   += ./user
DIR_DRIVER += ./driver

DIR_CMSIS        += ../Drivers/CMSIS
DIR_CMSIS_CORE   += ${DIR_CMSIS}/Core
DIR_CMSIS_DSP    += ${DIR_CMSIS}/DSP
DIR_CMSIS_DEVICE += ${DIR_CMSIS}/Device

DIR_CMSIS_DSP_LIB_GCC := $(DIR_CMSIS_DSP)/Lib/GCC
DIR_CMSIS_DSP_LIB_ARM := $(DIR_CMSIS_DSP)/Lib/ARM

DIR_HAL_DRIVER += ../Drivers/STM32H7xx_HAL_Driver

DIR_COMMON += ../Common

DIR_GCC := ./compiler/gcc
DIR_ARM := ./compiler/arm

# ---------------------------------------------------------------------------- #
#                                      头文件                                  #
# ---------------------------------------------------------------------------- #
DIR_INCLUDE += $(addprefix -I, ./Core/Inc)
DIR_INCLUDE += $(addprefix -I, $(DIR_CMSIS_CORE)/Include $(DIR_CMSIS_DSP)/Include $(DIR_CMSIS_DEVICE)/ST/STM32H7xx/Include)
DIR_INCLUDE += $(addprefix -I, $(DIR_HAL_DRIVER)/Inc $(DIR_HAL_DRIVER)/Inc/Legacy)
DIR_INCLUDE += $(addprefix -I, $(DIR_MODULE))
DIR_INCLUDE += $(addprefix -I, $(DIR_USER)/inc $(DIR_DRIVER)/inc)

# ---------------------------------------------------------------------------- #
#                                      源文件                                  #
# ---------------------------------------------------------------------------- #
ifneq (, $(filter ac5 ac6, $(C)))
	SRC_ASM += $(DIR_ARM)/startup_stm32h745xx_CM7.s
else ifeq ($(C), gcc)
	SRC_ASM += $(DIR_GCC)/startup_stm32h745xx_CM7.s
endif

SRC_C += $(DIR_COMMON)/Src/system_stm32h7xx_dualcore_boot_cm4_cm7.c
SRC_C += $(wildcard $(addsuffix /*.c, ./Core/Src))
SRC_C += $(wildcard $(addsuffix /*.c, $(DIR_USER)/src $(DIR_DRIVER)/src $(DIR_HAL_DRIVER)/Src))

OBJ += $(addprefix $(DIR_BUILD)/, $(notdir $(SRC_ASM:.s=.o)))
OBJ += $(addprefix $(DIR_BUILD)/, $(notdir $(SRC_C:.c=.o)))

vpath %.s $(sort $(dir $(SRC_ASM)))
vpath %.c $(sort $(dir $(SRC_C)))

# ---------------------------------------------------------------------------- #
#                                     链接脚本                                 #
# ---------------------------------------------------------------------------- #
ifneq (, $(filter ac5 ac6, $(C)))
	SCRIPT_LD := --scatter $(DIR_ARM)/stm32h745xx_flash_CM7.sct
	MAP       := --map --list=$(DIR_BUILD)/$(PROJECT).map
else ifeq ($(C), gcc)
	SCRIPT_LD := -T$(DIR_GCC)/stm32h745xx_flash_CM7.ld
	MAP       := -Wl,-Map=$(DIR_BUILD)/$(PROJECT).map,--cref -Wl,--gc-sections
endif

# ---------------------------------------------------------------------------- #
#                                     编译选项                                 #
# ---------------------------------------------------------------------------- #
DEFS += -DMCU
DEFS += -DCORE_CM7 -DSTM32H745xx
DEFS += -DUSE_HAL_DRIVER -DUSE_FULL_LL_DRIVER
DEFS += -DUSE_PWR_DIRECT_SMPS_SUPPLY
#DEFS += -DARM_MATH

ifeq ($(M), 1)
	LIBS += --library_type=microlib
	DEFS += -D__MICROLIB
endif

ifeq ($(T), D)
	OPTS := -O0 -g
	ifeq ($(C), ac5)
		OPTS +=
	else 
		OPTS += -gdwarf-4
	endif
else ifeq ($(T), R)
	OPTS := -O2
	ifeq ($(C), ac5)
		OPTS += --split_sections
	else 
		OPTS += -fdata-sections -ffunction-sections
	endif
endif

# ARMCC (AC5)
ifeq ($(C), ac5)
	CPU       := --cpu Cortex-M7.fp.dp
	MCU       := $(CPU)

	LIBS      := $(DIR_CMSIS_DSP_LIB_ARM)/arm_cortexM7lfdp_math.lib

	FLAGS_C   := $(MCU) $(DEFS) $(OPTS) -std=gnu11 -c
	FLAGS_CXX := $(FLAGS_C)
	FLAGS_AS  := $(MCU) --apcs=interwork
	FLAGS_LD  := $(MCU) $(LIBS) $(SCRIPT_LD) $(MAP) --strict

# ARMCLANG (AC6)
else ifeq ($(C), ac6)
	CPU       := -mcpu=cortex-m7 --target=arm-arm-none-eabi
	FPU       := -mfpu=fpv5-d16
	FLOAT-ABI := -mfloat-abi=hard
	MCU       := $(CPU) $(FPU) $(FLOAT-ABI) -mthumb

	LIBS      := $(DIR_CMSIS_DSP_LIB_ARM)/arm_cortexM7lfdp_math.lib

	FLAGS_C   := $(MCU) $(DEFS) $(OPTS) -std=gnu11 -c
	FLAGS_CXX := $(FLAGS_C)
	FLAGS_AS  := $(FLAGS_C) -masm=auto
	FLAGS_LD  := $(LIBS) $(SCRIPT_LD) $(MAP) --cpu Cortex-M7.fp.dp --strict 

# GCC
else ifeq ($(C), gcc)
	CPU       := -mcpu=cortex-m7
	FPU       := -mfpu=fpv5-d16
	FLOAT-ABI := -mfloat-abi=hard
	MCU       := $(CPU) $(FPU) $(FLOAT-ABI) -mthumb

	LIBS      := -lm -larm_cortexM7lfdp_math
	SPECS     := -specs=nosys.specs -specs=rdimon.specs

	FLAGS_C   := $(MCU) $(DEFS) $(OPTS) -c -Wall -Wextra
	FLAGS_CXX := $(FLAGS_C)
	FLAGS_AS  := $(FLAGS_C) -x assembler-with-cpp
	FLAGS_LD  := $(MCU) $(SPECS) $(LIBS) $(SCRIPT_LD) $(MAP)
endif

# ---------------------------------------------------------------------------- #
#                                      编译器                                  #
# ---------------------------------------------------------------------------- #
PATH_KEIL     := C:/Keil_v5
ifeq ($(C), ac5)
	CC_PREFIX := $(PATH_KEIL)/ARM/ARMCC/bin
	CC        := $(CC_PREFIX)/armcc
	CXX       := $(CC)
	AS        := $(CC_PREFIX)/armasm
	CP        := $(CC_PREFIX)/fromelf
	LD        := $(CC_PREFIX)/armlink
	AR        := $(CC_PREFIX)/armar
	SIZE      := $(CP) --text -z
	HEX       := $(CP) --i32
	BIN       := $(CP) --bin
else ifeq ($(C), ac6)
	CC_PREFIX := $(PATH_KEIL)/ARM/ARMCLANG/bin
	CC        := $(CC_PREFIX)/armclang
	CXX       := $(CC)
	AS        := $(CC)
	CP        := $(CC_PREFIX)/fromelf
	LD        := $(CC_PREFIX)/armlink
	AR        := $(CC_PREFIX)/armar
	SIZE      := $(CP) --text -z
	HEX       := $(CP) --i32
	BIN       := $(CP) --bin
else ifeq ($(C), gcc)
	CC_PREFIX := arm-none-eabi-
	CC        := $(CC_PREFIX)gcc
	CXX       := $(CC_PREFIX)g++
	AS        := $(CC)
	CP        := $(CC_PREFIX)objcopy
	LD        := $(CC_PREFIX)gcc
	GDB       := $(CC_PREFIX)gdb
	SIZE      := $(CC_PREFIX)size
	HEX       := $(CP) -O ihex
	BIN       := $(CP) -O binary -S
endif

# ---------------------------------------------------------------------------- #
#                                     调试器类型                               #
# ---------------------------------------------------------------------------- #
TYPE_FLASH := openocd_swd_flash
TYPE_DEBUG := openocd_swd_debug
TYPE_ERASE := openocd_swd_erase

# ---------------------------------------------------------------------------- #
#                                     伪目标定义                               #
# ---------------------------------------------------------------------------- #
ifeq ($(OS), Windows_NT)
	SHELL := cmd.exe
	MKDIR := md
	RMDIR := rd /s /q
else
	MKDIR := mkdir -p
	RMDIR := rm -rf
endif

ifeq ($(C), gcc)
	HEX_CMD = $(HEX) $< $@
	BIN_CMD = $(BIN) $< $@
else ifeq ($(C), ac6)
	HEX_CMD = $(HEX) $< --output $@
	BIN_CMD = $(BIN) $< --output $@
endif

.PHONY: all flash debug erase clean jlink_flash jlink_debug jlink_erase stlink_flash stlink_debug stlink_erase

TOOL ?= stlink

JLINK_EXE        ?= JLinkExe
JLINK_GDB_SERVER ?= JLinkGDBServer
JLINK_DEVICE     ?= STM32H745II_M7
JLINK_IF         ?= SWD
JLINK_SPEED      ?= 4000

OPENOCD        ?= openocd
OPENOCD_IF     ?= stlink.cfg
OPENOCD_TARGET ?= stm32h7x.cfg

all: $(DIR_BUILD) $(DIR_BUILD)/$(PROJECT).elf $(DIR_BUILD)/$(PROJECT).hex $(DIR_BUILD)/$(PROJECT).bin 
	$(SIZE) $(DIR_BUILD)/$(PROJECT).elf

$(DIR_BUILD):
	$(MKDIR) $(DIR_BUILD)
	$(info SRC_ASM: $(SRC_ASM))
	$(info SRC_C: $(SRC_C))
	$(info OBJ: $(OBJ))
	$(info ------)

$(DIR_BUILD)/%.o: %.s | $(DIR_BUILD)
	$(AS) $(FLAGS_AS) $< -o $@

$(DIR_BUILD)/%.o: %.c | $(DIR_BUILD)
	$(CC) $(FLAGS_C) $(DIR_INCLUDE) $< -o $@

$(DIR_BUILD)/%.o: %.cpp | $(DIR_BUILD)
	$(CXX) $(FLAGS_CXX) $(DIR_INCLUDE) $< -o $@
	
$(DIR_BUILD)/$(PROJECT).elf: $(OBJ)
	$(LD) $(FLAGS_LD) $(OBJ) -o $@

$(DIR_BUILD)/%.hex: $(DIR_BUILD)/%.elf
	$(HEX_CMD)

$(DIR_BUILD)/%.bin: $(DIR_BUILD)/%.elf
	$(BIN_CMD)

flash:
ifeq ($(TOOL), jlink)
	@$(MAKE) jlink_flash
else
	@$(MAKE) stlink_flash
endif

debug:
ifeq ($(TOOL), jlink)
	@$(MAKE) jlink_debug
else
	@$(MAKE) stlink_debug
endif

erase:
ifeq ($(TOOL), jlink)
	@$(MAKE) jlink_erase
else
	@$(MAKE) stlink_erase
endif

jlink_flash: $(DIR_BUILD)/$(PROJECT).bin
	@echo "Flashing firmware to device using J-Link..."
	@echo "halt" > jlink_flash.jlink
	@echo "r" >> jlink_flash.jlink
	@echo "loadfile $(DIR_BUILD)/$(PROJECT).bin 0x08000000" >> jlink_flash.jlink
	@echo "r" >> jlink_flash.jlink
	@echo "go" >> jlink_flash.jlink
	@echo "exit" >> jlink_flash.jlink
	$(JLINK_EXE) -Device $(JLINK_DEVICE) -If $(JLINK_IF) -Speed $(JLINK_SPEED) -CommanderScript jlink_flash.jlink
	@rm jlink_flash.jlink

stlink_flash: $(DIR_BUILD)/$(PROJECT).bin
	@echo "Flashing firmware to device using ST-Link..."
	$(OPENOCD) -f interface/$(OPENOCD_IF) \
	-c "transport select hla_swd" \
	-f target/$(OPENOCD_TARGET) \
	-c "init" \
	-c "reset halt" \
	-c "sleep 100" \
	-c "flash write_image erase $(DIR_BUILD)/$(PROJECT).bin 0x08000000" \
	-c "sleep 100" \
	-c "verify_image $(DIR_BUILD)/$(PROJECT).bin 0x08000000" \
	-c "sleep 100" \
	-c "reset run" \
	-c "shutdown"

jlink_debug: $(DIR_BUILD)/$(PROJECT).elf
	@echo "Starting debug session using J-Link..."
	$(JLINK_GDB_SERVER) -device $(JLINK_DEVICE) -if $(JLINK_IF) -speed $(JLINK_SPEED) -port 2331 &
	@echo "JLinkGDBServer started in background (port 2331)"
	$(GDB) $(DIR_BUILD)/$(PROJECT).elf -ex "target remote localhost:2331" \
	-ex "monitor reset" \
	-ex "load" \
	-ex "monitor halt"

stlink_debug: $(DIR_BUILD)/$(PROJECT).elf
	@echo "Starting debug session using ST-Link..."
	$(OPENOCD) -f interface/$(OPENOCD_IF) \
	-c "transport select hla_swd" \
	-f target/$(OPENOCD_TARGET) \
	-c "init" \
	-c "halt" \
	-c "reset halt" &
	$(GDB) -ex "target extended-remote localhost:3333" \
	-ex "monitor reset halt" \
	-ex "load" \
	-ex "monitor reset init" \
	$(DIR_BUILD)/$(PROJECT).elf

jlink_erase:
	@echo "Erasing device using J-Link..."
	@echo "halt" > jlink_erase.jlink
	@echo "r" >> jlink_erase.jlink
	@echo "erase" >> jlink_erase.jlink
	@echo "r" >> jlink_erase.jlink
	@echo "exit" >> jlink_erase.jlink
	$(JLINK_EXE) -Device $(JLINK_DEVICE) -If $(JLINK_IF) -Speed $(JLINK_SPEED) -CommanderScript jlink_erase.jlink
	@rm jlink_erase.jlink

stlink_erase:
	@echo "Erasing device using ST-Link..."
	$(OPENOCD) -f interface/$(OPENOCD_IF) \
	-c "transport select hla_swd" \
	-f target/$(OPENOCD_TARGET) \
	-c "init" \
	-c "reset halt" \
	-c "sleep 100" \
	-c "stm32h7x mass_erase 0" \
	-c "sleep 100" \
	-c "shutdown"

clean:
	$(RMDIR) $(DIR_BUILD)
