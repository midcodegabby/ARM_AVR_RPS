/*
 * exti.c
 *
 *  Created on: August 15, 2024
 *  Author: Gabriel Rodgers
 *  Purpose: to enable interrupts and handle IRQs
 */

#include <stdint.h>
#include <stdio.h>
#include "exti.h"
#include "clock.h"
#include "nvic.h"
#include "gpio.h"
#include "systick.h"
#include "uart.h"

#define EXTI 0x40010400
#define SYSCFG 0x40010000

#define EXTI_IMR1 (*((volatile uint32_t *) EXTI))
#define EXTI_EMR1 (*((volatile uint32_t *) (EXTI + 0x04)))
#define EXTI_PR1 (*((volatile uint32_t *) (EXTI + 0x14)))
#define EXTI_RTSR1 (*((volatile uint32_t *) (EXTI + 0x08)))
#define EXTI_FTSR1 (*((volatile uint32_t *) (EXTI + 0x0C)))
#define SYSCFG_EXTICR4 (*((volatile uint32_t *) (SYSCFG + 0x14)))

extern volatile uint8_t my_hand;
extern volatile uint8_t gamephase;
extern volatile uint8_t opponent_hand;
volatile uint8_t SendReady = 240;

//enable interrupts
void exti_init(void) {

        //enable interrupts for line 13, corresponding to the user button
        //EXTI_IMR1 |= (1 << 13);

        //enable interrupts for line 13, corresponding to UART4
        //EXTI_IMR1 |= (1 << 13);

        //enable events for line 13
        //EXTI_EMR1 |= (1 << 13);

        //enable falling edge trigger for line 13
        EXTI_FTSR1 |= (1 << 13);

        //disable rising edge trigger for line 13
        EXTI_RTSR1 &= ~(1 << 13);

        //choose PC13 to be the event trigger for EXTI13
        SYSCFG_EXTICR4 |= (1 << 5);
}

void exti_enable(void) {
        //enable interrupts for line 13, corresponding to the user button
        EXTI_IMR1 |= (1 << 13);
}

void exti_disable(void) {
        //disable interrupts for line 13, corresponding to the user button
        EXTI_IMR1 &= ~(1 << 13);
}

//IRQ handler for button push interrupt; this either starts the game or changes the hand
void EXTI15_10_IRQHandler(void) {
	
	//disable interrupts
        nvic_disable(); 
	
	gpio_led_toggle();

	//if the gamephase is 1 and no opponent signal, then send out the start signal:
	if ((gamephase == 1) && (opponent_hand == 0)) {
		uart_transmit(SendReady);
		
		//change the gamephase
		gamephase = 2;

		//disable button inputs
		exti_disable(); 

		//print out the next message
		printf("READY. Waiting for the opponent\n\n");
	}	

	else if ((gamephase == 1) && (opponent_hand == SendReady)) {
		uart_transmit(SendReady);
		
		//change the gamephase
		gamephase = 3;

		//disable button inputs
		exti_disable(); 

	}

	//handle button inputs during hand selection
	else if (gamephase == 3) {
		switch (my_hand) {
		 case 0:
			my_hand = 1;
			printf("Rock\n");
			break;
		
		 case 1:
			my_hand = 2;
			printf("Paper\n");
			break;

		 case 2:
			my_hand = 3;
			printf("Scissors\n");
			break;

		 case 3:
			my_hand = 1;
			printf("Rock\n");
			break;
		}
	}

	
	//button input does nothing during phase 1 and 3

        //clear any pending interrupts and re-enable interrupts
        EXTI_PR1 |= (1 << 13);
	nvic_enable();
}

