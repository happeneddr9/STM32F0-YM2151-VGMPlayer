################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/TIMER/TIMER.cpp 

OBJS += \
./src/TIMER/TIMER.o 

CPP_DEPS += \
./src/TIMER/TIMER.d 


# Each subdirectory must supply rules for building sources it contributes
src/TIMER/%.o: ../src/TIMER/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo $(PWD)
	arm-none-eabi-g++ -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -std=c++1y -DSTM32 -DSTM32F0 -DSTM32F030C8Tx -DDEBUG -DSTM32F030 -DUSE_STDPERIPH_DRIVER -I"C:/Users/user/Desktop/happeneddr9/Github/STM32F0-YM2151-VGMPlayer/StdPeriph_Driver/inc" -I"C:/Users/user/Desktop/happeneddr9/Github/STM32F0-YM2151-VGMPlayer/src" -I"C:/Users/user/Desktop/happeneddr9/Github/STM32F0-YM2151-VGMPlayer/inc" -I"C:/Users/user/Desktop/happeneddr9/Github/STM32F0-YM2151-VGMPlayer/CMSIS/device" -I"C:/Users/user/Desktop/happeneddr9/Github/STM32F0-YM2151-VGMPlayer/CMSIS/core" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


