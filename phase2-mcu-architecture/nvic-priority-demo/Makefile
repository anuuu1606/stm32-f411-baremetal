TARGET = nvic_demo
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -Wall -O0 -g
LDFLAGS = -T linker.ld -nostdlib

all: $(TARGET).bin

$(TARGET).elf: startup.s main.c
	$(CC) $(CFLAGS) $(LDFLAGS) startup.s main.c -o $(TARGET).elf

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

flash: $(TARGET).bin
	openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
	-c "program $(TARGET).bin 0x08000000 verify reset exit"

clean:
	rm -f *.elf *.bin