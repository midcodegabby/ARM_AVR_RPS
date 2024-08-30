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
#define NVIC_IPR13 (*((volatile uint32_t *) (NVIC_IPR + 0x34)))

//enable NVIC
void nvic_enable(void) {
        NVIC_ISER1 |= (1 << 8); //enable interrupt 40 (user button)
        NVIC_ISER1 |= (1 << 20); //enable interrupt 52 (UART4)
}

//disable NVIC
void nvic_disable(void) {
        NVIC_ICER1 |= (1 << 8); //disable interrupt 40 (user button)
        NVIC_ICER1 |= (1 << 20); //disable interrupt 52 (UART4) 
}

//change priorities of nvic
void nvic_priority(void) {
	//change priority of button interrupt (40) to lowest
	NVIC_IPR10 |= (0xF << 4); 

	//change priority of UART4 interrupt (52) to second highest
	//NVIC_IPR13 |= (1 << 4); 
}

