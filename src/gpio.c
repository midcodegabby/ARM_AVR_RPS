/*
 * gpio.c
 *
 *  Created on: May 29, 2024
 *  Author: Gabriel Rodgers
 *  Purpose: configure GPIO Port to be UART port
 */

//include files
#include "gpio.h"
#include <stdint.h>

//definitions and register mapping
#define GPIOA 0x48000000

#define GPIOA_MODER (*((volatile uint32_t *) GPIOA))
#define GPIOA_OTYPER (*((volatile uint32_t *) GPIOA + 0x04))
#define GPIOA_OSPEEDR (*((volatile uint32_t *) GPIOA + 0x08))
#define GPIOA_PUPDR (*((volatile uint32_t *) GPIOA + 0x0C))
#define GPIOA_AFRL (*((volatile uint32_t *) GPIOA + 0x20))

void gpio_uart_init() {
	//this function initializes the gpio port to be a uart port

	//set GPIOA pin 0 and 1 to be alt. func. (UART)
	GPIOA_MODER |= (0xA); //set 3rd and 1st bit
	GPIOA_MODER &= ~(0x5); //clear 2nd and 0th bit

	//set output to reset (push-pull) state for PA0 and PA1
	GPIOA_OTYPER &= ~(0x3); //clear 1st and 2nd bit

	//set speed to low
	GPIOA_OSPEEDR &= ~(0xF); //clear bits 3-0

	//set PA0 to pull-down R (TX pin for UART)
	//set PA1 to pull-up R (RX pin for UART)
	GPIOA_PUPDR |= (0x6); //set 2nd and 1st bit
	GPIOA_PUPDR &= ~(0x9); //clear 3rd and 0th bit

	//set PA0 and PA1 to be AF8 alt funcs
	GPIOA_AFRL |= (0x88); //set 7th and 3rd bits
	GPIOA_AFRL &= ~(0x77); //clear bits 6-4 and 2-0

}
