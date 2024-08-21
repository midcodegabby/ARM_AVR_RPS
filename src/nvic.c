/*
 * nvic.c
 *
 *  Created on: August 15, 2024
 *  Author: Gabriel Rodgers
 *  Purpose: To enable interrupts through the NVIC
 */

#include <stdint.h>
#include "nvic.h"


#define NVIC_ISER 0xE000E100
#define NVIC_ICER 0xE000E180
#define NVIC_IPR 0xE000E400

#define NVIC_ISER1 (*((volatile uint32_t *) (NVIC_ISER + 0x04)))
#define NVIC_ICER1 (*((volatile uint32_t *) (NVIC_ICER + 0x04)))
#define NVIC_IPR10 (*((volatile uint32_t *) (NVIC_IPR + 0x28)))

//enable NVIC
void enable_nvic(void) {
        NVIC_ISER1 |= (1 << 8); //enable interrupt 40
}

//disable NVIC
void disable_nvic(void) {
        NVIC_ICER1 |= (1 << 8); //disable interrupt 40
}

//change priorities of nvic
void change_priority_nvic(void) {
	//change priority of button interrupt (40) to lowest
	NVIC_IPR10 |= (0xF << 4); 
}

