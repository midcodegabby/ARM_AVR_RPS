/*
 * Author: Gabriel Rodgers
 * Date: 8/20/2024
 * Purpose: Header file for systick configuration
*/

#ifndef SYSTICK_H
#define SYSTICK_H

void systick_init(void);
void enable_systick(void);
void disable_systick(void);
void SysTick_Handler(void);
void clock_init(void);
volatile uint32_t get_time(void);

#endif
