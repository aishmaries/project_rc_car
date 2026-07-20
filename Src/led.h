/* led setup and turn on functions for user LED 2*/


#ifndef LED_H_
#define LED_H_
#include <stdint.h>

void led_init();

void led_on();

void led_off();

void led_toggle();

void loop_3cycles(int); //implemented in .s file
void delay(uint32_t ms);

#endif
