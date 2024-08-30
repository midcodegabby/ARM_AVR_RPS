This makefile allows for programming and debugging the STM32L476xxx mcu with the files in this project.
Make sure all files are in the same directory when using this makefile.

command uses:
all -> compile and link all files into a .elf file called final.elf 
semi -> compile and link all files into a .elf file called final_sh.elf (can be used for semihosting)
clean -> remove all object files, map files, and .elf files
load -> setup a connection to the mcu using OpenOCD
client -> run the gdb executable from the ARM GNU toolchain to allow for connection to the OpenOCD server running by the load command
