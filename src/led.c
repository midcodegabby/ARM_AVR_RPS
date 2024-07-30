#include <stdint.h>
#include "led.h"

#define GPIOA 0x48000000

#define GPIOA_MODER (*((volatile uint32_t *) GPIOA))
#define GPIOA_OTYPER (*((volatile uint32_t *) GPIOA + 0x04))
#define GPIOA_PUPDR (*((volatile uint32_t *) GPIOA + 0x0C))
#define GPIOA_BSRR (*((volatile uint32_t *) GPIOA + 0x18))
#define GPIOA_AFRL (*((volatile uint32_t *) GPIOA + 0x20))

int main(void) {
	
	//set LED pin (GPIOA pin 5) to be general purpose output mode
        GPIOA_MODER |= (1 << 10);
        GPIOA_MODER &= ~(1 << 11);

        GPIOA_OTYPER &= ~(1 << 5); // set PA5 to be push-pull

        //set LED pin to be Z (floating) so that led stays on
        GPIOA_PUPDR &= ~(1 << 11);
        GPIOA_PUPDR &= ~(1 << 10);

        //GPIOA_ODR |= (1 << 5); // set led ON
        GPIOA_BSRR |= (1 << 5);
	
	//initialize all of the stuff
	//sysclk_init();
	//peripheral_clk_init();
	//gpio_button_init();
	//gpio_led_init();

	//uint32_t idr_contents;

	//infinite loop checking if the button has been pressed
	while (1){
		//read the value of the IDR register
		//right shift GPIOC_IDR by 13, negate it, then & with one
		//idr_contents = (~(GPIOC_IDR >> 13) & 1);

		//toggle LED: first mask the other bits of idr_contents
		//if (idr_contents & (1 << 0)){
			//GPIOA_ODR ^= (1 << 5);
		//}
	}
	

	return 0;
}

