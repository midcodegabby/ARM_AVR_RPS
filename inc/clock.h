/*
 * Author: Gabriel Rodgers
 * Date: 5/15/2024
 * Purpose: configure sysclock within the STM32L476RG MCU using a header file
 *
 */

#ifndef CLOCK_H
#define CLOCK_H

//define a bunch of functions:
void sysclk_init();
void peripheral_clk_init();

#endif
