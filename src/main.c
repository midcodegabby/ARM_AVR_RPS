/*
 * main.c
 *
 *  Created on: May 29, 2024
 *  Author: Gabriel Rodgers
 *  Purpose: run the main code?
 */

#include <stdint.h>

#include "clock.h"
#include "gpio.h"
#include "nvic.h"
#include "exti.h"

int main(void) {

	//initialize all of the stuff
	//sysclk_init();
	peripheral_clk_init();
	gpio_button_init();
	gpio_led_init();
	enable_exti();
	enable_nvic();
	gpio_led_on();

	while (1){
		//gpio_led_on();	
	}

	return 0;
}

