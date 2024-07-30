#include <stdint.h>

//#include "clock.h"
#include "gpio.h"

#define GPIOA 0x48000000
#define GPIOC 0x48000800

#define GPIOA_ODR (*((volatile uint32_t *) GPIOA + 0x14))
#define GPIOC_IDR (*((volatile uint32_t *) GPIOC + 0x10))
const uint32_t const1 = 100;
int main(void) {

	//initialize all of the stuff
	//sysclk_init();
	//peripheral_clk_init();
	//gpio_button_init();
	gpio_led_init();

	//uint32_t idr_contents;

	//infinite loop checking if the button has been pressed
	
	while (1){
		//read the value of the IDR register
		//right shift GPIOC_IDR by 13, negate it, then & with one
		//idr_contents = (~(GPIOC_IDR >> 13) & 1);

		//toggle LED: first mask the other bits of idr_contents
		//if (idr_contents & (1 << 0)){
		//	GPIOA_ODR ^= (1 << 5);
		//}
	}
	

	return 0;
}

