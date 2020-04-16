################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include src/flash/spi/subdir.mk
-include src/TIMER/subdir.mk
-include src/Sound/YM2151/subdir.mk
-include src/Sound/subdir.mk
-include src/SDcard/spi/subdir.mk
-include src/SDcard/FATFS/subdir.mk
-include src/RTC/subdir.mk
-include src/Print/stream/subdir.mk
-include src/Print/subdir.mk
-include src/I2CMaster/subdir.mk
-include src/HardwareUart/subdir.mk
-include src/Display/TFTLCD/ST77xx/ST7789/subdir.mk
-include src/Display/TFTLCD/ST77xx/ST7735/subdir.mk
-include src/Display/TFTLCD/ST77xx/subdir.mk
-include src/Display/TFTLCD/ILI9341/subdir.mk
-include src/Display/TFTLCD/subdir.mk
-include src/Display/Fonts/RREFonts/subdir.mk
-include src/Display/subdir.mk
-include src/DAC/External/subdir.mk
-include src/Arduino/subdir.mk
-include src/ADC/internal/subdir.mk
-include src/subdir.mk
-include StdPeriph_Driver/src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: YM2151_VGM_fin.elf

# Tool invocations
YM2151_VGM_fin.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU G++ Linker'
	arm-none-eabi-g++ -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -specs=nosys.specs  -specs=nano.specs -fwide-exec-charset=UTF-16 -T"C:/Users/user/Desktop/happeneddr9/Github/STM32F0-YM2151-VGMPlayer/LinkerScript.ld" -Wl,-Map=output.map -Wl,--gc-sections -fno-exceptions -fno-rtti -o "YM2151_VGM_fin.elf" @"objects.list" $(USER_OBJS) $(LIBS) -lm
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

post-build:
	-@echo 'Generating binary and Printing size information:'
	arm-none-eabi-objcopy -O binary "YM2151_VGM_fin.elf" "YM2151_VGM_fin.bin"
	arm-none-eabi-size "YM2151_VGM_fin.elf"
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
