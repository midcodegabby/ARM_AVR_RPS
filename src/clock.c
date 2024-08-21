/*
 * Author: Gabriel Rodgers
 * Date: 5/15/2024
 * Purpose: configure sysclock within the STM32L476RG MCU
 */

//include files
#include <stdint.h>
#include <stdio.h>

#include "clock.h"
#include "systick.h"


//define the base register value for the RCC portion of memory:
#define RCC 0x40021000

//define the first register in the RCC memory section
//this method directly dereferences the memory itself to access the registers
#define RCC_CR (*((volatile uint32_t *) (RCC))) 	//sysclk config
#define RCC_CFGR (*((volatile uint32_t *) (RCC + 0x08)))	//clk config
#define RCC_AHB1ENR (*((volatile uint32_t *) (RCC + 0x48)))	//clk enable for peripherals
#define RCC_AHB2ENR (*((volatile uint32_t *) (RCC + 0x4C)))	//clk enable for peripherals
#define RCC_APB1ENR (*((volatile uint32_t *) (RCC + 0x58)))	//clk enable for peripherals
#define RCC_APB2ENR (*((volatile uint32_t *) (RCC + 0x60)))	//clk enable for peripherals
#define RCC_CCIPR (*((volatile uint32_t *) (RCC + 0x88)))		//clk config for peripherals
									
volatile uint32_t CLK = 4;
volatile uint32_t RE_CLK = 4;
extern volatile uint32_t time_ms;

void delay(uint32_t ms) {
	uint32_t start = time_ms; 

	while ((start + ms) > time_ms); 
}

void sysclk_init(void) {
	//This function initializes the sysclk to work at 32 MHz, using the MSI clk

	//set MSI range to 32MHz
	RCC_CR |= (0xA << 4); //set 7th and 5th bit
	RCC_CR &= (0x5 << 4); //clear 6th and 4th bit

	//MSI EN set
	RCC_CR |= (1);

	//wait for MSI to be ready; this waits for RCC_CR's 2nd bit to be set (indicating that MSI clk is stable
	while (!(RCC_CR & 2));

	//set sysclk to be MSI clk
	RCC_CFGR &= ~(0x3); //clear the first two bits
}


void peripheral_clk_init(void) {
	//this function initializes the peripherals to have clocks

	//enable clk for peripherals (GPIO Port A)
	RCC_AHB2ENR |= (1 << 0);

	//enable clk for peripherals (GPIO Port C)
	RCC_AHB2ENR |= (1 << 2);

	//enable clk for SYSCFG 
	RCC_APB2ENR |= (1 << 0);

	//enable clk for UART4
	//RCC_APB1ENR |= (1 << 19);

	//select sysclk for UART4
	//RCC_CCIPR |= (1 << 6);
	//RCC_CCIPR &= ~(1 << 7);

	for (uint32_t i = 0; i<2; i++);
}


