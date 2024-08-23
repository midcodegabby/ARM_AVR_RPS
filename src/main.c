/*
 * main.c
 *
 *  Created on: May 29, 2024
 *  Author: Gabriel Rodgers
 *  Purpose: run the main code?
 */

#include <stdint.h>
#include <stdio.h>

#include "clock.h"
#include "gpio.h"
#include "nvic.h"
#include "exti.h"
#include "systick.h"
#include "uart.h"

//semihosting init function:
extern void initialize_monitor_handles(void);

extern uint8_t opponent_hand; 
volatile uint8_t my_hand = 0;
volatile uint8_t gamephase = 0;

int main(void) {

	//initialize all of the stuff
	sysclk_init();
	clock_init();
	peripheral_clk_init();
	gpio_button_init();
	gpio_led_init();
	gpio_uart_init();
	uart_init();
	exti_enable();
	nvic_enable();
	nvic_priority();

	while (1){
		uart_transmit(0xFF);
		delay(1000);
	
	}

	return 0;
}

