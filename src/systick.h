/*
 * Author: Gabriel Rodgers
 * Date: 8/20/2024
 * Purpose: Header file for systick configuration
*/

#ifndef SYSTICK_H
#define SYSTICK_H

void systick_init(uint32_t ms);
void enable_systick(void);
void disable_systick(void);

#endif
