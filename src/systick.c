/*
 * Author: Gabriel Rodgers
 * Date: 8/20/2024
 * Purpose: Enable and configure systick registers for a delay.
*/

#include <stdint.h>
#include <stdio.h>
#include "clock.h"
#include "gpio.h"
#include "nvic.h"

//Register Definitions
#define STK 0xE000E010

#define STK_CTRL (*((volatile uint32_t *) STK))
#define STK_LOAD (*((volatile uint32_t *) (STK + 0x04)))

//define the clock speed defined in clock.h
extern volatile uint32_t CLK;

//this will be a variable that stores the time in ms
volatile uint32_t time_ms = 0;

//initialize systick
void systick_init(void) {
	
	//set clock source of systick to AHB (comment out for AHB/8)
	STK_CTRL |= (1<<2);

	//enable interrupts when systick reaches 0
	STK_CTRL |= (1<<1);

	//find the amount of pulses required to reach 1 ms; where 4 is the clock frequency in MHz. 
	uint32_t pulses = (1*CLK*1000);

	//load the pulses var into the load register
	STK_LOAD &= 0; 
	STK_LOAD |= (pulses); 
}

//enable systick
void enable_systick(void) {
	STK_CTRL |= (1);
}

//disable systick
void disable_systick(void) {
	STK_CTRL &= ~(1);
}

//IRQ handler for systick interrupt
void SysTick_Handler(void) {

        //disable interrupts and systick
        disable_nvic();
        //disable_systick();

        //gpio_led_toggle();
	time_ms++; 	//increment the clock time

        //enable interrupts and systick
	//enable_systick();
        enable_nvic();
}

//initialize the clock
void clock_init(void) {
	
	//initialize the systick
	systick_init();

	//enable the systick
	enable_systick();
}

//return the current time
volatile uint32_t get_time(void) {
	return time_ms;
}
	













