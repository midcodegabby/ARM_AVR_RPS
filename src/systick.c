/*
 * Author: Gabriel Rodgers
 * Date: 8/20/2024
 * Purpose: Enable and configure systick registers for a delay.
*/

#include <stdint.h>
#include "clock.h"

//Register Definitions
#define STK 0xE000E010

#define STK_CTRL (*((volatile uint32_t *) STK))
#define STK_LOAD (*((volatile uint32_t *) (STK + 0x04)))

//initialize systick
void systick_init(uint32_t ms) {
	
	//set clock source of systick to AHB
	STK_CTRL |= (1<<2);

	//enable interrupts when systick reaches 0
	STK_CTRL |= (1<<1);

	//find the amount of pulses required to reach ms:
	//uint32_t pulses = (ms*4*1000);

	//load the pulses var into the load register
	STK_LOAD |= (0x3D0900); 
}

//enable systick
void enable_systick(void) {
	STK_CTRL |= (1);
}

//disable systick
void disable_systick(void) {
	STK_CTRL &= ~(1);
}
