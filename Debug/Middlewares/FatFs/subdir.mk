################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src/option/ccsbcs.c \
C:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src/diskio.c \
C:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src/ff.c \
C:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c \
C:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src/option/syscall.c 

OBJS += \
./Middlewares/FatFs/ccsbcs.o \
./Middlewares/FatFs/diskio.o \
./Middlewares/FatFs/ff.o \
./Middlewares/FatFs/ff_gen_drv.o \
./Middlewares/FatFs/syscall.o 

C_DEPS += \
./Middlewares/FatFs/ccsbcs.d \
./Middlewares/FatFs/diskio.d \
./Middlewares/FatFs/ff.d \
./Middlewares/FatFs/ff_gen_drv.d \
./Middlewares/FatFs/syscall.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/FatFs/ccsbcs.o: C:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src/option/ccsbcs.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F413xx -DDEBUG -c -I../FATFS/App -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src -I../Core/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Include -I../FATFS/Target -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/FatFs/ccsbcs.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/FatFs/diskio.o: C:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src/diskio.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F413xx -DDEBUG -c -I../FATFS/App -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src -I../Core/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Include -I../FATFS/Target -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/FatFs/diskio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/FatFs/ff.o: C:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src/ff.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F413xx -DDEBUG -c -I../FATFS/App -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src -I../Core/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Include -I../FATFS/Target -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/FatFs/ff.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/FatFs/ff_gen_drv.o: C:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F413xx -DDEBUG -c -I../FATFS/App -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src -I../Core/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Include -I../FATFS/Target -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/FatFs/ff_gen_drv.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/FatFs/syscall.o: C:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src/option/syscall.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F413xx -DDEBUG -c -I../FATFS/App -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Middlewares/Third_Party/FatFs/src -I../Core/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/CMSIS/Include -I../FATFS/Target -IC:/Users/devin/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.1/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/FatFs/syscall.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
