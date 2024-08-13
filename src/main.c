#include <stdint.h>

#include "clock.h"
#include "gpio.h"

int main(void) {

	//initialize all of the stuff
	//sysclk_init();
	peripheral_clk_init();
	gpio_button_init();
	gpio_led_init();

	//turn on the led
	gpio_led_on();

	//uint32_t idr_contents;

	//infinite loop checking if the button has been pressed
	uint32_t i = 0;


	while (1){

		if (i == 100000) {
			gpio_led_toggle();
			i = 0;
		}
		i++; 
	}

	return 0;
}

