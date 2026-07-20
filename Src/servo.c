/*
 * servo.c
 *
 *  Created on: Jun 11, 2026
 *      Author: aishm
 */

#include "stm32f411xe.h"
#include "servo.h"
void servo_init() {
//PB6 is TIM4_CH1

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //enable clock for TIM4_CH1N

	GPIOB->MODER &= ~GPIO_MODER_MODER6;//clear bits
	GPIOB->MODER |= GPIO_MODER_MODER6_1;

	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6;//clear bits
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_1;



	TIM4->CCMR1 &= ~TIM_CCMR1_OC1M; //clear bits
	TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2; //set to PWM mode 1

	TIM4->PSC = 319;

	TIM4->ARR = 999; //set PWM frequency to 50Hz

	TIM4->CCER |= TIM_CCER_CC1E;

	TIM4->CR1 |= TIM_CR1_CEN;

}

void turn_cw(){
	TIM4->CCR1 = 49; //1ms
}

void turn_ccw(){
	TIM4->CCR1 = 99; //2ms
}

void center_servo(){
	TIM4->CCR1 = 74; //1.5ms
}
