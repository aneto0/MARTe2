#User 
BASELIBDIR=/home/pc/EFDA-MARTE-old/branches/MARTe2/Source/Core/
MAINDIR=/home/pc/EFDA-MARTE-old/branches/MARTe2/Source/Core/L0Portability/OperatingSystem/STM/
BUILDPATH=$(MAINDIR)STM32Cube_FW_F4_V1.5.0/MyProjects/gpioSrc/Build/


RTOSDIR=$(MAINDIR)STM32Cube_FW_F4_V1.5.0/Middlewares/Third_Party/FreeRTOS/Source/
CROSSPATH?=/usr/bin/
CROSSPFX=$(CROSSPATH)arm-none-eabi-
CC=$(CROSSPFX)g++
OBJCOPY=$(CROSSPFX)objcopy
OBJDUMP=$(CROSSPFX)objdump
AS=$(CROSSPFX)as
LD=$(CROSSPFX)g++
NM=$(CROSSPFX)nm
FLAGS=-mcpu=cortex-m4 -mlittle-endian -mthumb -DSTM32F407xx

#include the stm headers
INCPATHS= -I./Inc
INCPATHS+= -I$(MAINDIR)STM32Cube_FW_F4_V1.5.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include
INCPATHS+= -I$(MAINDIR)STM32Cube_FW_F4_V1.5.0/Drivers/STM32F4xx_HAL_Driver/Inc/
INCPATHS+= -I$(MAINDIR)STM32Cube_FW_F4_V1.5.0/Drivers/BSP/STM32F4-Discovery/
INCPATHS+= -I$(MAINDIR)STM32Cube_FW_F4_V1.5.0/Drivers/CMSIS/Include/
INCPATHS+= -I$(MAINDIR)STM32Cube_FW_F4_V1.5.0/Drivers/CMSIS/RTOS/Template
INCPATHS+= -I$(MAINDIR)STM32Cube_FW_F4_V1.5.0/Drivers/CMSIS/Device/ST/STM32F4xx/Include
INCPATHS+= -I$(RTOSDIR)portable/GCC/ARM_CM4F
INCPATHS+= -I$(RTOSDIR)CMSIS_RTOS
INCPATHS+= -I$(RTOSDIR)include
INCPATHS+= -I/usr/include
INCPATHS+= -I/usr/include/x86_64-linux-gnu

#include the baselib2 headers
INCPATHS+= -I$(BASELIBDIR)L0Portability/
INCPATHS+= -I$(BASELIBDIR)L1CLib/
INCPATHS+= -I$(BASELIBDIR)LXTypeDev/

CFLAGS=$(FLAGS) -Wall
OPTFLAGS=-Os
#Remember to substitute it with stm macros.
SPECFLAGS=-DARCHITECTURE=ARM -DOPERATING_SYSTEM=STM
CFLAGS+= $(INCPATHS) $(OPTFLAGS)



HALPATH=$(MAINDIR)STM32Cube_FW_F4_V1.5.0/Drivers/STM32F4xx_HAL_Driver/Src/
BSPPATH=$(MAINDIR)STM32Cube_FW_F4_V1.5.0/Drivers/BSP/STM32F4-Discovery/
CMSISPATH=$(MAINDIR)STM32Cube_FW_F4_V1.5.0/Drivers/CMSIS/Device/ST/STM32F4xx/Source/
TARGET=output

#include CPPFILES objs declared in baselib
include $(BASELIBDIR)L0Portability/Makefile.stm
include $(BASELIBDIR)L1CLib/Makefile.stm
#include $(BASELIBDIR)LXTypeDev/Makefile.stm

#i must include also the input cpp which contain the main.


CFILES:=$(shell ls *.c 2>/dev/null)
CFILES+=$(shell ls $(HALPATH)*.c 2>/dev/null)
CFILES+=$(shell ls $(BSPPATH)*.c 2>/dev/null)
CFILES+=$(shell ls $(CMSISPATH)*.c 2>/dev/null)
CFILES+=$(shell ls $(RTOSDIR)*.c 2>/dev/null)
CFILES+=$(shell ls $(RTOSDIR)CMSIS_RTOS/*.c 2>/dev/null)

#startup.s should be in this directory
AFILES:=$(shell ls *.s 2>/dev/null)
#AFILES+=$(shell ls $(HALPATH)*.s 2>/dev/null)
#AFILES+=$(shell ls $(BSPPATH)*.s 2>/dev/null)
#AFILES+=$(shell ls $(CMSISPATH)*.s 2>/dev/null)
COBJS:=$(CFILES:%.c=%.o)
CPPOBJSTMP:=$(CPPFILES:%.cpp=%.o)
CPPOBJS = $(addprefix $(BUILDPATH), $(CPPOBJSTMP))
AOBJS:=$(AFILES:%.s=%.o)
ALLOBJS=$(COBJS) $(CPPOBJS) $(AOBJS)
LPATH=/home/pc/Scrivania/STM32Cube_FW_F4_V1.5.0/Projects/STM32F4-Discovery/Templates/TrueSTUDIO/STM32F4-Discovery/
#LPATH=$(MAINDIR)STM32Cube_FW_F4_V1.5.0/Projects/STM32F4-Discovery/Templates/TrueSTUDIO/STM32F4-Discovery/
LTARGET=$(LPATH)STM32F407VG_FLASH.ld
LFLAGS=-Wl,--gc-sections



all: $(BUILDPATH)$(TARGET).bin $(BUILDPATH)$(TARGET).lst $(BUILDPATH)$(TARGET).sym

boh:
	echo $(ALLOBJS)
	echo $(CPPOBJSTMP)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

$(HALPATH)%.o: $(HALPATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BSPPATH)%.o: $(BSPPATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(CMSISPATH)%.o: $(CMSISPATH)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(RTOSDIR)%.o: $(RTOSDIR)%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(RTOSDIR)CMSIS_RTOS/%.o: $(RTOSDIR)CMSIS_RTOS/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDPATH)%.o: $(BASELIBDIR)L0Portability/%.cpp
	$(CC) -W -Wall -O2 -DSTM32F407xx $(SPECFLAGS) $(INCPATHS) -c $(BASELIBDIR)L0Portability/$*.cpp -o $(BUILDPATH)$*.o


$(BUILDPATH)%.o: $(BASELIBDIR)L1CLib/%.cpp
	$(CC) -W -Wall -O2 -DSTM32F407xx $(SPECFLAGS) $(INCPATHS) -c $(BASELIBDIR)L1CLib/$*.cpp -o $(BUILDPATH)$*.o


#$(BUILDPATH)%.o: $(BASELIBDIR)LXTypeDev/%.cpp
#	$(CC) -W -Wall -O2 $(SPECFLAGS) $(INCPATHS) -c $(BASELIBDIR)LXTypeDev/$*.cpp -o $(BUILDPATH)$*.o


$(BUILDPATH)%.o: %.s
	$(CC) $(CFLAGS) -c $*.s -o $(BUILDPATH)$*.o

$(BUILDPATH)$(TARGET).elf: $(ALLOBJS) $(LTARGET)
	$(LD) -nostdlib  $(FLAGS) -T $(LTARGET) $(LFLAGS) $(ALLOBJS) -lgcc -lm -lc -lrdimon -lnosys -o $@ 

$(BUILDPATH)%.bin: $(BUILDPATH)%.elf
	$(OBJCOPY) -S -O binary  $< $@

$(BUILDPATH)%.lst: $(BUILDPATH)%.elf
	$(OBJDUMP) -d $^ > $@

$(BUILDPATH)%.sym: $(BUILDPATH)%.elf
	$(NM)  $^ | sort > $@

.PRECIOUS: $(BUILDPATH)%.elf

.PHONY: clean

clean:
	rm -f *.o $(BUILDPATH)*.o $(BUILDPATH)*.bin $(BUILDPATH)*.lst  $(BUILDPATH)*.sym  $(BUILDPATH)*.elf 
#$(ALLOBJS)


