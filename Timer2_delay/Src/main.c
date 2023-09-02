#include "stm32f4xx.h"

void delayMs(int n);

int main(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; /* enable GPIOG clock */

	GPIOG->MODER &= ~GPIO_MODER_MODE13; /* clear pin mode */

	GPIOG->MODER |= GPIO_MODER_MODE13_0; /* set pin to output mode */

	/* Configure Timer2  */
	RCC->APB1ENR |=RCC_APB1ENR_TIM2EN; // enable the timer 2 clock

	TIM2->PSC = 1600 -1; //divide the system clock by 1600
	TIM2->ARR = 10000 - 1; // THIS is use to divide the remaining value of the system clock
	TIM2->CNT = 0; // THis is use to clear the timer
	TIM2->CR1 |= TIM_CR1_CEN; // This enable tim2


	while (1) {
		while ( !(TIM2->SR & TIM_SR_UIF)) {} // wait untill the status register is set

		TIM2->SR &= ~TIM_SR_UIF; // Clear the status register

		GPIOG->ODR ^= 1 << 13;
	}
}
