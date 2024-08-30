/*
 * uart.h
 *
 *  Created on: August 22, 2024
 *  Author: Gabriel Rodgers
 */

#ifndef INC_UART_H_
#define INC_UART_H_

void uart_init(void);
void uart_transmit(uint8_t data_out); 
void UART4_IRQHandler(void);

#endif 
