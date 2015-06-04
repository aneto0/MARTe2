COMPILAZIONE

arm-none-eabi-gcc -Wall -mcpu=cortex-m4 -mlittle-endian -mthumb -I/home/pc/Scrivania/STM32Cube_FW_F4_V1.5.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Inc -I/home/pc/Scrivania/STM32Cube_FW_F4_V1.5.0/Drivers/CMSIS/Include -I/home/pc/Scrivania/STM32Cube_FW_F4_V1.5.0/Drivers/STM32F4xx_HAL_Driver/Inc/ -I/home/pc/Scrivania/STM32Cube_FW_F4_V1.5.0/Drivers/BSP/STM32F4-Discovery/ -DSTM32F401xE -Os -c ../../../../../../Drivers/STM32F4xx_HAL_Driver/Src/* -o *.o^C


LINKING

arm-none-eabi-gcc -mcpu=cortex-m4 -mlittle-endian -mthumb -DSTM32F407xx -T/home/pc/Scrivania/STM32Cube_FW_F4_V1.5.0/Projects/STM32F4-Discovery/Templates/TrueSTUDIO/STM32F4-Discovery/STM32F407VG_FLASH.ld -I/home/pc/Scrivania/STM32Cube_FW_F4_V1.5.0/Drivers/STM32F4xx_HAL_Driver/Src/ -Wl,--gc-sections main.o startup_stm32f407xx.o system_stm32f4xx.o stm32f4xx_it.o stm32f4_discovery.o stm32f4xx_hal.o -o main.elf


