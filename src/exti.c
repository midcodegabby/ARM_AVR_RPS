/*
 * exti.c
 *
 *  Created on: August 15, 2024
 *  Author: Gabriel Rodgers
 *  Purpose: to enable interrupts and handle IRQs
 */

#include <stdint.h>
#include <stdio.h>
#include "exti.h"
#include "nvic.h"
#include "gpio.h"


#define EXTI 0x40010400
#define SYSCFG 0x40010000

#define EXTI_IMR1 (*((volatile uint32_t *) EXTI))
#define EXTI_EMR1 (*((volatile uint32_t *) (EXTI + 0x04)))
#define EXTI_PR1 (*((volatile uint32_t *) (EXTI + 0x14)))
#define EXTI_RTSR1 (*((volatile uint32_t *) (EXTI + 0x08)))
#define EXTI_FTSR1 (*((volatile uint32_t *) (EXTI + 0x0C)))
#define SYSCFG_EXTICR4 (*((volatile uint32_t *) (SYSCFG + 0x14)))

//enable interrupts
void enable_exti(void) {

        //enable interrupts for line 13, corresponding to the user button
        EXTI_IMR1 |= (1 << 13);

        //enable events for line 13
        //EXTI_EMR1 |= (1 << 13);

        //enable falling edge trigger for line 13
        EXTI_FTSR1 |= (1 << 13);

        //disable rising edge trigger for line 13
        EXTI_RTSR1 &= ~(1 << 13);

        //choose PC13 to be the event trigger for EXTI13
        SYSCFG_EXTICR4 |= (1 << 5);
}

//IRQ handler for button push interrupt
void EXTI15_10_IRQHandler(void) {

        disable_nvic(); 

	printf("You pressed the button!\n");

        for(uint32_t i = 0; i < 10; i++);

        gpio_led_toggle();

        //clear any pending interrupts
        EXTI_PR1 |= (1 << 13);


        //disable interrupts and events for line 13
        //EXTI_IMR1 &= ~(1 << 13);
        //EXTI_EMR1 &= ~(1 << 13);


        //enable interrupts and events for line 13
        //EXTI_IMR1 |= (1 << 13);
        //EXTI_EMR1 |= (1 << 13);

        enable_nvic();
}

