

#include <stm32f411xe.h>
#include "led.h"
#include <stdint.h>

extern void loop_3cycles(int);


void led_init() {


//RCC->AHB1 = GPIOAEN
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN_Msk;
//GPIOA_MODER5 = 01
	GPIOA->MODER &= ~GPIO_MODER_MODER5;
	GPIOA->MODER |= GPIO_MODER_MODER5_0;


}

void led_on() {
//GPIOA_BSRR = BS5
	GPIOA->BSRR |= GPIO_BSRR_BS5;
}

void led_off() {

	GPIOA->BSRR |= GPIO_BSRR_BR5;
}

void delay(uint32_t ms){
	for(int i= ms; i>0; i--){
		/* Each clock cycles is 1/15MHz
		 * So per ms there are: 1*10^-3 * 15^10^6 / 3 = 5000
		 * loops through the small 3-clock cycle helper function. */
		 loop_3cycles(5000);
	}
}
