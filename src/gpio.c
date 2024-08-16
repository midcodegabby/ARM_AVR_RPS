/*
 * gpio.c
 *
 *  Created on: May 29, 2024
 *  Author: Gabriel Rodgers
 *  Purpose: configure GPIO Port to be UART port and turn on LED2
 */

//include files
#include <stdint.h>
#include "gpio.h"

//definitions and register mapping
#define GPIOA 0x48000000
#define GPIOC 0x48000800

#define GPIOA_MODER (*((volatile uint32_t *) GPIOA))
#define GPIOA_OTYPER (*((volatile uint32_t *) (GPIOA + 0x04)))
#define GPIOA_OSPEEDR (*((volatile uint32_t *) (GPIOA + 0x08)))
#define GPIOA_PUPDR (*((volatile uint32_t *) (GPIOA + 0x0C)))
#define GPIOA_ODR (*((volatile uint32_t *) (GPIOA + 0x14)))
#define GPIOA_BSRR (*((volatile uint32_t *) (GPIOA + 0x18)))
#define GPIOA_AFRL (*((volatile uint32_t *) (GPIOA + 0x20)))

#define GPIOC_MODER (*((volatile uint32_t *) GPIOC))
#define GPIOC_PUPDR (*((volatile uint32_t *) (GPIOC + 0x0C)))
#define GPIOC_IDR (*((volatile uint32_t *) (GPIOC + 0x10)))

#define RCC_AHB2ENR (*((volatile uint32_t *) (0x40021000 + 0x4C)))

/*
void gpio_uart_init(void) {
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
*/
void gpio_button_init(void) {

	//set button pin (B1, connected to PC13) to be general purpose input mode
	GPIOC_MODER &= ~(0x3 << 26);

	//set input to pull up R
	GPIOC_PUPDR &= ~(0x3 << 26);
	GPIOC_PUPDR |= (1 << 26);

}

uint8_t gpio_button_poll(uint8_t button) {

	//check input data register and assign it to the button parameter
	button = ((GPIOC_IDR >> 13) & 1);

	//if:
	//button == 1, then button has not been pushed
	//button == 0, then button has been pushed
	
	return button;
}

void gpio_led_init(void) {

	//set LED pin (GPIOA pin 5) to be general purpose output mode
	GPIOA_MODER |= (1 << 10);
	GPIOA_MODER &= ~(1 << 11);

	GPIOA_OTYPER &= ~(1 << 5); // set PA5 to be push-pull
}

void gpio_led_on(void) {
	
	//turn the led on
	GPIOA_BSRR |= (1 << 5); 
}

void gpio_led_off(void) {
	
	//turn the led off
	GPIOA_ODR &= ~(1 << 5); 
}

void gpio_led_toggle(void) {

	GPIOA_ODR ^= (1 << 5); 
}

