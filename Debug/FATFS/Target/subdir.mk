################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FATFS/Target/bsp_driver_sd.c \
../FATFS/Target/fatfs_platform.c \
../FATFS/Target/sd_diskio.c 

OBJS += \
./FATFS/Target/bsp_driver_sd.o \
./FATFS/Target/fatfs_platform.o \
./FATFS/Target/sd_diskio.o 

C_DEPS += \
./FATFS/Target/bsp_driver_sd.d \
./FATFS/Target/fatfs_platform.d \
./FATFS/Target/sd_diskio.d 


# Each subdirectory must supply rules for building sources it contributes
FATFS/Target/bsp_driver_sd.o: ../FATFS/Target/bsp_driver_sd.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F413xx -DDEBUG -c -I../FATFS/App -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src -I../Core/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Include -I../FATFS/Target -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Middlewares/Third_Party/FatFs/src -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"FATFS/Target/bsp_driver_sd.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
FATFS/Target/fatfs_platform.o: ../FATFS/Target/fatfs_platform.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F413xx -DDEBUG -c -I../FATFS/App -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src -I../Core/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Include -I../FATFS/Target -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Middlewares/Third_Party/FatFs/src -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"FATFS/Target/fatfs_platform.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
FATFS/Target/sd_diskio.o: ../FATFS/Target/sd_diskio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F413xx -DDEBUG -c -I../FATFS/App -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src -I../Core/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Include -I../FATFS/Target -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Middlewares/Third_Party/FatFs/src -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"FATFS/Target/sd_diskio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

