################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FATFS/App/fatfs.c 

OBJS += \
./FATFS/App/fatfs.o 

C_DEPS += \
./FATFS/App/fatfs.d 


# Each subdirectory must supply rules for building sources it contributes
FATFS/App/fatfs.o: ../FATFS/App/fatfs.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F413xx -DDEBUG -c -I../FATFS/App -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src -I../Core/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Include -I../FATFS/Target -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"FATFS/App/fatfs.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

