################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/SDcard/FATFS/FatFile.cpp \
../src/SDcard/FATFS/FatFileLFN.cpp \
../src/SDcard/FATFS/FatFilePrint.cpp \
../src/SDcard/FATFS/FatFileSFN.cpp \
../src/SDcard/FATFS/FatVolume.cpp \
../src/SDcard/FATFS/FmtNumber.cpp 

OBJS += \
./src/SDcard/FATFS/FatFile.o \
./src/SDcard/FATFS/FatFileLFN.o \
./src/SDcard/FATFS/FatFilePrint.o \
./src/SDcard/FATFS/FatFileSFN.o \
./src/SDcard/FATFS/FatVolume.o \
./src/SDcard/FATFS/FmtNumber.o 

CPP_DEPS += \
./src/SDcard/FATFS/FatFile.d \
./src/SDcard/FATFS/FatFileLFN.d \
./src/SDcard/FATFS/FatFilePrint.d \
./src/SDcard/FATFS/FatFileSFN.d \
./src/SDcard/FATFS/FatVolume.d \
./src/SDcard/FATFS/FmtNumber.d 


# Each subdirectory must supply rules for building sources it contributes
src/SDcard/FATFS/%.o: ../src/SDcard/FATFS/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo $(PWD)
	arm-none-eabi-g++ -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -std=c++1y -DSTM32 -DSTM32F0 -DSTM32F030C8Tx -DDEBUG -DSTM32F030 -DUSE_STDPERIPH_DRIVER -I"C:/Users/user/Desktop/happeneddr9/Github/STM32F0-YM2151-VGMPlayer/StdPeriph_Driver/inc" -I"C:/Users/user/Desktop/happeneddr9/Github/STM32F0-YM2151-VGMPlayer/src" -I"C:/Users/user/Desktop/happeneddr9/Github/STM32F0-YM2151-VGMPlayer/inc" -I"C:/Users/user/Desktop/happeneddr9/Github/STM32F0-YM2151-VGMPlayer/CMSIS/device" -I"C:/Users/user/Desktop/happeneddr9/Github/STM32F0-YM2151-VGMPlayer/CMSIS/core" -Os -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


