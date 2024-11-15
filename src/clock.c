/*
 * Author: Gabriel Rodgers
 * Date: 5/15/2024
 * Purpose: configure sysclock, peripherals, and delay fn within the STM32L476RG MCU
 */

//include files
#include <stdint.h>
#include <stdio.h>

#include "clock.h"
#include "systick.h"

//define the base register value for the RCC portion of memory:
#define RCC 0x40021000
#define FLASH 0x40022000

//define the first register in the RCC memory section
//this method directly dereferences the memory itself to access the registers
#define RCC_CR (*((volatile uint32_t *) (RCC))) 	//sysclk config
#define RCC_CFGR (*((volatile uint32_t *) (RCC + 0x08)))	//clk config
#define RCC_AHB1ENR (*((volatile uint32_t *) (RCC + 0x48)))	//clk enable for peripherals
#define RCC_AHB2ENR (*((volatile uint32_t *) (RCC + 0x4C)))	//clk enable for peripherals
#define RCC_APB1ENR (*((volatile uint32_t *) (RCC + 0x58)))	//clk enable for peripherals
#define RCC_APB2ENR (*((volatile uint32_t *) (RCC + 0x60)))	//clk enable for peripherals
#define RCC_CCIPR (*((volatile uint32_t *) (RCC + 0x88)))		//clk config for peripherals
									
#define FLASH_ACR (*((volatile uint32_t *) (FLASH)))		

uint32_t SYS_CLK = 32;
uint32_t AHB_CLK = 8;
uint32_t APB1_CLK = 8;

//delay a given amount of time using systick
void delay(uint32_t ms) {
	uint32_t start = get_time();
	while ((start + ms) > get_time()); 
}

void sysclk_init(void) {
	//This function initializes the sysclk to work at 32 MHz, using the MSI clk
	
	//first change the FLASH to have 1 wait state
	FLASH_ACR |= (1 << 0);

	//select MSI range from RCC_CR register
	RCC_CR |= (1 << 3);
	
	//set MSI range to 32 MHz
	RCC_CR &= ~(0xF << 4);
	RCC_CR |= (0xA << 4);

	//set AHB prescaler to be 4; this makes the AHB clock (HCLK) 8 MHz
	RCC_CFGR |= (0x9 << 4);

	//APB1 prescaler is default 1; this makes APB1 clock 8 MHz
}

void peripheral_clk_init(void) {
	//this function initializes the peripherals to have clocks
	//it initializes GPIOA, GPIOC, SYSCFG, and UART4

	//enable clk for peripherals (GPIO Port A)
	RCC_AHB2ENR |= (1 << 0);

	//enable clk for peripherals (GPIO Port C)
	RCC_AHB2ENR |= (1 << 2);

	//enable clk for SYSCFG 
	RCC_APB2ENR |= (1 << 0);

	//enable clk for UART4
	RCC_APB1ENR |= (1 << 19);

	//PCLK (APB1 CLK) is set as UART4 clock by default

	delay(2);
}


