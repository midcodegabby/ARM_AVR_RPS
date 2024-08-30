/*
 * main.c
 *
 *  Created on: May 29, 2024
 *  Author: Gabriel Rodgers
 *  Purpose: Handle most Rock Paper Scissors logic
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

//globals
static volatile uint8_t my_hand;
volatile uint8_t opponent_hand;
volatile uint8_t gamephase;
static uint8_t SendReady = 240;

void change_hand(void) {
	//this function allows for hand change
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

void gamephase_incr(void) {
	//this function increments the gamephase variable
	gamephase++;
}

int main(void) {

	//initialize all of the stuff
	sysclk_init();
	clock_init();
	peripheral_clk_init();
	gpio_button_init();
	gpio_led_init();
	gpio_uart_init();
	uart_init();
	exti_init();
	nvic_enable();
	nvic_priority();

	//initialize variables
	gamephase = 0;

	while (1){

		//send out start message
		if (gamephase == 0){
			printf("\nWelcome!\n");
			printf("Please press user button 1\n\n");
			exti_enable();
			my_hand = 0;
			opponent_hand = 0;
			gamephase = 1;
		}
		
		//message for when ARM mcu sends SendReady signal first
		else if ((gamephase == 2) && (opponent_hand == SendReady)) {
			gamephase = 3;
			printf("GAME START\n\n");
			exti_enable();
		}

		//start delay if we are in the correct phase
		if (gamephase == 3) {
			
			//enable button inputs
			exti_enable();

			delay(7500); //delay for 7.5s
				     
			//at this point it is time to end the game!
			uart_transmit(my_hand);

			//disable button inputs
			exti_disable(); 

			printf("\n");

			switch (opponent_hand) {
				
			 case 1:
				printf("Opponent: Rock\n");
				break;
			 case 2:
				printf("Opponent: Paper\n");
				break;
			 case 3:
				printf("Opponent: Scissors\n");
				break;
			}

			switch (my_hand) {
				
			 case 1:
				printf("You: Rock\n");
				break;
			 case 2:
				printf("You: Paper\n");
				break;
			 case 3:
				printf("You: Scissors\n");
				break;
			}

			gamephase = 4;

			//do another delay: 
			delay(7500);
		}

		//winning/losing screen
		if (gamephase == 4) {

			//rock beats scissors
			if ((my_hand == 1) && (opponent_hand == 3)){
				printf("\nYou Won!\n\n");
			}

			//scissors loses to rock
			else if ((my_hand == 3) && (opponent_hand == 1)){
				printf("\nYou Lost!\n\n");
			}

			//all other cases are simple
			else if (my_hand < opponent_hand){
				printf("\nYou Lost!\n\n");
			}

			//all other cases are simple
			else if (my_hand > opponent_hand){
				printf("\nYou Won!\n\n");
			}

			//case for draw
			else if (my_hand == opponent_hand){
				printf("\nDraw!\n\n");
			}

			delay(7500);

			gamephase = 0;
			
		}
	}

	return 0;
}

