#include "stm32f4xx.h"

void delayMs(int n);

int main(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; /* enable GPIOG clock */

	GPIOG->MODER &= ~GPIO_MODER_MODE13; /* clear pin mode */

	GPIOG->MODER |= GPIO_MODER_MODE13_0; /* set pin to output mode */

	/* Configure Timer2  */
	RCC->APB1ENR |=RCC_APB1ENR_TIM2EN; // enable the timer 2 clock

	TIM2->PSC = 16000-1; //divide the system clock by 1000
	TIM2->ARR = 1000 - 1; // THIS is use to divide the remaining value of the system clock
							// to give it 1Khz in 1Seconds
	TIM2->CNT = 0; // THis is use to clear the timer
	TIM2->CR1 |= TIM_CR1_CEN; // This enable tim2

	// Enable Timer 2 Update Interrupt
	TIM2->DIER |= TIM_DIER_UIE;

	NVIC_SetPriority(TIM2_IRQn, 0); // Highest priority
	NVIC_EnableIRQ(TIM2_IRQn);


	while (1)
	{
	}
}


void TIM2_IRQHandler(void)
{
	TIM2->SR &= ~TIM_SR_UIF; // Clear the UIF register
	GPIOG->ODR ^= 1 << 13;

}
