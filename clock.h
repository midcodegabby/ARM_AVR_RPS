/*
 * Author: Gabriel Rodgers
 * Date: 5/15/2024
 * Purpose: configure sysclock within the STM32L476RG MCU using a header file
 *
 */

#ifndef CLOCK_H
#define CLOCK_H

//define a bunch of functions:
void clk_for_sysclk(uint32_t clk_f);
void clk_for_peripherals();
