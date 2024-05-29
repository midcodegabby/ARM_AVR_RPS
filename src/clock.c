/*
 * Author: Gabriel Rodgers
 * Date: 5/15/2024
 * Purpose: configure sysclock within the STM32L476RG MCU
 */

//include files

#include "clock.h"
#include <stdint.h>


//define the base register value for the RCC portion of memory:
#define RCC 0x40021000

//define the first register in the RCC memory section
//this method directly dereferences the memory itself to access the registers
#define RCC_CR (*((volatile uint32_t *) RCC)) 	//sysclk config
#define RCC_CFGR (*((volatile uint32_t *) RCC + 0x08))	//clk config
#define RCC_AHB2ENR (*((volatile uint32_t *) RCC + 0x4C))	//clk enable for peripherals
#define RCC_CCIPR (*((volatile uint32_t *) RCC + 0x88))		//clk config for peripherals

void sysclk_init() {
	//This function initializes the sysclk to work at 32 MHz, using the MSI clk

	//set MSI range to 32MHz
	RCC_CR |= 0xA0;

	//MSI EN
	RCC_CR |= 1;

	//wait for MSI to be ready; this waits for RCC_CR
	while (RCC_CR & 0x2);


}

int main() {
	sysclk_init();
	return 0;
}
