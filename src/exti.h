/*
 * exti.h
 *
 *  Created on: August 15, 2024
 *      Author: Gabriel Rodgers
 */

#ifndef INC_EXTI_H_
#define INC_EXTI_H_

void exti_init(void);
void exti_enable(void);
void exti_disable(void);
void EXTI15_10_IRQHandler(void);

#endif 
