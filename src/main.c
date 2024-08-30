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

volatile uint8_t my_hand;
volatile uint8_t opponent_hand;
volatile uint8_t gamephase;

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
	uint8_t SendReady = 240;


	while (1){

		//send out start message
		if (gamephase == 0){
			printf("\nWelcome!\n");
			printf("Please press User Button 1\n\n");
			exti_enable();
			my_hand = 0;
			opponent_hand = 0;
			gamephase = 1;
		}

		//check if game has started
		else if (gamephase == 3) {
			gamephase = 4;
			printf("GAME START\n\n");
		}

		//start delay if we are in the correct phase
		else if (gamephase == 4) {
			
			//enable button inputs
			exti_enable();
			delay(7500); //delay for 7.5s
				     
			//at this point it is time to end the game!
			uart_transmit(my_hand);

			//disable button inputs
			exti_disable(); 

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

			gamephase = 5;

			//do another delay: 
			delay(7500);
		}

		//winning/losing screen
		if (gamephase == 5) {

			//rock beats scissors
			if ((my_hand == 1) && (opponent_hand == 3)){
				printf("You Won!\n\n");
			}

			//scissors loses to rock
			else if ((my_hand == 3) && (opponent_hand == 1)){
				printf("You Lost!\n\n");
			}

			//all other cases are simple
			else if (my_hand < opponent_hand){
				printf("You Lost!\n\n");
			}

			//all other cases are simple
			else if (my_hand > opponent_hand){
				printf("You Won!\n\n");
			}

			//case for draw
			else if (my_hand == opponent_hand){
				printf("Draw!\n\n");
			}

			delay(7500);

			gamephase = 0;
			
		}
	}

	return 0;
}

