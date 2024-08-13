/*
 * gpio.h
 *
 *  Created on: May 29, 2024
 *      Author: Gabriel Rodgers
 */

#ifndef INC_GPIO_H_
#define INC_GPIO_H_

//void gpio_uart_init(void);
void gpio_button_init(void);
void gpio_led_init(void);
void gpio_led_on(void);
void gpio_led_off(void);
void gpio_led_toggle(void);


#endif /* INC_GPIO_H_ */
