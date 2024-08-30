/*
 * Author: Gabriel Rodgers
 * Date: 8/20/2024
*/

#ifndef SYSTICK_H
#define SYSTICK_H

void systick_init(void);
void systick_enable(void);
void systick_disable(void);
void SysTick_Handler(void);
void clock_init(void);
volatile uint32_t get_time(void);

#endif
