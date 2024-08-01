# note for compilation: can call a specific function defined here by using
# [make func_name] --> e.g., make clean
CC=arm-none-eabi-gcc
MACH=cortex-m4
CFLAGS=-c -mcpu=$(MACH) -mthumb --std=gnu11 -Wall -o0
LDFLAGS=-nostdlib -T stm32_ls.ld -Wl,-Map=final.map #-Wl specifies a linker input

all: main.o gpio.o stm32_startup.o final.elf

main.o:main.c
	$(CC) $(CFLAGS) -o $@ $^

gpio.o:gpio.c
	$(CC) $(CFLAGS) -o $@ $^

stm32_startup.o:stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

final.elf: main.o gpio.o stm32_startup.o
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -rf *.o *.elf *.map

load:
	openocd -f board/st_nucleo_l4.cfg

# -Wall: show all warnings
#  clean: function description deletes all .o and .elf files
#  -c: do not link the files
#  -o: create object files
#

