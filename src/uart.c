/*
 * uart.c
 *
 *  Created on: August 22, 2024
 *  Author: Gabriel Rodgers
 *  Purpose: To initialize and control the UART4 peripheral
 */

#include <stdint.h>
#include <stdio.h>
#include "uart.h"
#include "nvic.h"

#define USART1 0x40013800
#define UART4 0x40004C00

#define UART4_CR1 (*((volatile uint32_t *) (UART4)))
#define UART4_CR2 (*((volatile uint32_t *) (UART4 + 0x04)))
#define UART4_CR3 (*((volatile uint32_t *) (UART4 + 0x08)))
#define UART4_BRR (*((volatile uint32_t *) (UART4 + 0x0C)))
#define UART4_ISR (*((volatile uint32_t *) (UART4 + 0x1C)))
#define UART4_RDR (*((volatile uint32_t *) (UART4 + 0x24)))
#define UART4_TDR (*((volatile uint32_t *) (UART4 + 0x28)))

//initialize the uart
void uart_init(void){
	//oversampling = 8; parity is disabled by default so will keep that way
	UART4_CR1 |= (1 << 15); 

	//enable RXNE interrupt
	UART4_CR1 |= (1 << 5); 

	//enable TX
	UART4_CR1 |= (1 << 3); 

	//enable RX
	UART4_CR1 |= (1 << 2);

	//LSB sent first is enabled by default
	//stop bits = 2
	UART4_CR2 |= (1 << 13); 

	//one-bit sampling
	UART4_CR3 |= (1 << 11);

	//set baud rate to 2400 bps 
	UART4_BRR |= (0x1A05);
	
	//enable UART4: DO LAST
	UART4_CR1 |= (1);
}

//this function transmits a given word
void uart_transmit(volatile uint8_t data_out) {
	
	//transmit the data
	UART4_TDR = data_out;
	
	//keep looping until all data has been transmitted
	while (!(UART4_ISR >> 6) & 1);
}







