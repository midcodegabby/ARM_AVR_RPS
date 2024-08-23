# note for compilation: can call a specific function defined here by using
# [make func_name] --> e.g., make clean
CC= arm-none-eabi-gcc
MACH= cortex-m4
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft --std=gnu11 -Wall -o0 -g3
LDFLAGS= -mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=nano.specs -T stm32_ls.ld -Wl,-Map=final.map 
LDFLAGS_SH= -mcpu=$(MACH) -mthumb -mfloat-abi=soft --specs=rdimon.specs -T stm32_ls.ld -Wl,-Map=final.map 
# -Wl specifies a linker input, -nostdlib used for no stdlib, soft means using nano emulation of fpu
#  --specs=nano.specs allows for stdlib nano
#  --specs=rdimon.specs allows for semihosting; LDFLAGS_SH is LDFLAGS for Semi Hosting!

all: main.o gpio.o clock.o uart.o nvic.o exti.o systick.o sysmem.o syscalls.o stm32_startup.o final.elf 

semi: main.o gpio.o clock.o uart.o nvic.o exti.o systick.o sysmem.o syscalls.o stm32_startup.o final_sh.elf

main.o:main.c
	$(CC) $(CFLAGS) -o $@ $^

gpio.o:gpio.c
	$(CC) $(CFLAGS) -o $@ $^

clock.o:clock.c
	$(CC) $(CFLAGS) -o $@ $^

uart.o:uart.c
	$(CC) $(CFLAGS) -o $@ $^
	
nvic.o:nvic.c
	$(CC) $(CFLAGS) -o $@ $^

exti.o:exti.c
	$(CC) $(CFLAGS) -o $@ $^

systick.o:systick.c
	$(CC) $(CFLAGS) -o $@ $^
	
sysmem.o:sysmem.c
	$(CC) $(CFLAGS) -o $@ $^

syscalls.o:syscalls.c
	$(CC) $(CFLAGS) -o $@ $^

stm32_startup.o:stm32_startup.c
	$(CC) $(CFLAGS) -o $@ $^

final.elf: main.o gpio.o clock.o uart.o nvic.o exti.o systick.o sysmem.o syscalls.o stm32_startup.o
	$(CC) $(LDFLAGS) -o $@ $^

# do not link syscalls.o with .elf file; stdlib does the syscalls!
final_sh.elf: main.o gpio.o clock.o uart.o nvic.o exti.o systick.o stm32_startup.o
	$(CC) $(LDFLAGS_SH) -o $@ $^

clean:
	rm -rf *.o *.elf *.map

load:
	openocd -f board/st_nucleo_l4.cfg

client:
	arm-none-eabi-gdb

# -Wall: show all warnings
#  clean: function description deletes all .o and .elf files
#  -c: do not link the files
#  -o: create object files
#

