#include <stdint.h>

#include "clock.h"
#include "gpio.h"

#define EXTI 0x40010400
#define SYSCFG 0x40010000
#define NVIC_ISER 0xE000E100
#define NVIC_ICER 0xE000E180

#define EXTI_IMR1 (*((volatile uint32_t *) EXTI))
#define EXTI_EMR1 (*((volatile uint32_t *) (EXTI + 0x04)))
#define EXTI_RTSR1 (*((volatile uint32_t *) (EXTI + 0x08)))
#define EXTI_FTSR1 (*((volatile uint32_t *) (EXTI + 0x0C)))
#define SYSCFG_EXTICR4 (*((volatile uint32_t *) (SYSCFG + 0x14)))
#define NVIC_ISER1 (*((volatile uint32_t *) (NVIC_ISER + 0x04)))
#define NVIC_ICER1 (*((volatile uint32_t *) (NVIC_ICER + 0x04)))

//enable NVIC
void enable_nvic(void) {
	NVIC_ISER1 |= (1 << 8); //enable interrupt 40
}

//disable NVIC
void disable_nvic(void) {
	NVIC_ICER1 |= (1 << 8); //disable interrupt 40
}

//enable interrupts
void enable_exti(void) {

	//choose PC13 to be the event trigger for EXTI13
	SYSCFG_EXTICR4 |= (1 << 5); 

	//enable interrupts for line 13, corresponding to the user button
	EXTI_IMR1 |= (1 << 13); 

	//enable events for line 13
	EXTI_EMR1 |= (1 << 13);

	//enable falling edge trigger for line 13
	EXTI_FTSR1 |= (1 << 13); 
	
	//disable rising edge trigger for line 13
	EXTI_RTSR1 &= ~(1 << 13); 
}

//IRQ handler for button push interrupt
extern void EXTI15_10_IRQHandler(void) {
	
	disable_nvic(); 

	//disable interrupts and events for line 13
	EXTI_IMR1 &= ~(1 << 13);
	EXTI_EMR1 &= ~(1 << 13);

	gpio_led_toggle();

	//enable interrupts and events for line 13
	EXTI_IMR1 |= (1 << 13);
	EXTI_EMR1 |= (1 << 13);

	enable_nvic();
}

int main(void) {

	//initialize all of the stuff
	//sysclk_init();
	peripheral_clk_init();
	gpio_button_init();
	gpio_led_init();
	enable_exti();
	enable_nvic();

	while (1){
		
	}

	return 0;
}

