ENTRY(Reset_Handler)

MEMORY
{
	FLASH (rx):ORIGIN = 0x08000000, LENGTH = 1024K
	SRAM1 (rwx):ORIGIN = 0x20000000, LENGTH = 96K
	SRAM2 (rwx):ORIGIN = 0x10000000, LENGTH = 32K
}

SECTIONS
{
	/*The following sections are created in the following order*/
	/*This section should include .text section of all input files*/
	.text : 
	{
		/*At this point in the ls the location symbol (.) is the same as VMA which is FLASH (0x0800 0000)*/
		/*The * char says to merge .text section of all input files*/
		*(.isr_vector)
		*(.text)
		*(.rodata)
		. = ALIGN(4);
		_etext = .; /*Store the updated location counter value in to a symbol called _etext, note that
			     the location counter only tracks the VMA, not the LMA!!!*/

	} > FLASH 
	/*This part shows where to put the section in memory*/
	/* syntax: > vma AT> lma, for this case no need for second part due to vma = lma
	/* vma = virtual memory address, lma = load memory address*/

	/*This section should include .data section of all input files*/
	.data : 
	{
		_sdata = .;
		*(.data)
		. = ALIGN(4);
		_edata = .;

	} > SRAM1 AT> FLASH 
	/* this section is loaded into the flash right after the .text section, but it is within the SRAM*/

	/*This section should include .bss section of all input files*/
	.bss : 
	{
		_sbss = .;
		*(.bss)
		*(COMMON)
		. = ALIGN(4);
		_ebss = .;
	
	} > SRAM1
}
