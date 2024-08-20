/*
 * Author: Gabriel Rodgers
 * Date: 5/15/2024
 * Purpose: configure sysclock within the STM32L476RG MCU using a header file
 *
 */

#ifndef CLOCK_H
#define CLOCK_H

#define RESTART_CLK 4

//define a bunch of functions:
void wait(uint32_t ms, uint32_t mhz);
void sysclk_init(void);
void peripheral_clk_init(void);

#endif
