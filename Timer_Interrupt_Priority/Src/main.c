#include "stm32f4xx.h"

#define PRIO_TIM3 3
#define PRIO_TIM4 4


void delayMs(int n);

int main(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; /* enable GPIOG clock */

	GPIOG->MODER &= ~GPIO_MODER_MODE13; /* clear pin mode */

	GPIOG->MODER |= GPIO_MODER_MODE13_0; /* set pin to output mode */

	/* Configure Timer2  */
	RCC->APB1ENR |=RCC_APB1ENR_TIM3EN ; // enable the timer 3 clock

	TIM3->PSC = 1000-1; //divide the system clock by 1000
	TIM3->ARR = 15500 - 1; // THIS is use to divide the remaining value of the system clock
							// to give it 1Khz in 1Seconds

	TIM3->CR1 |= TIM_CR1_CEN; // This enable tim4

	// Enable Timer 2 Update Interrupt
	TIM3->DIER |= TIM_DIER_UIE;

	NVIC_SetPriority(TIM3_IRQn, PRIO_TIM3); // Highest priority
	NVIC_EnableIRQ(TIM3_IRQn);



	GPIOG->MODER &= ~GPIO_MODER_MODE14; /* clear pin mode */

	GPIOG->MODER |= GPIO_MODER_MODE14_0; /* set pin to output mode */

	GPIOG->MODER &= ~GPIO_MODER_MODE15; /* clear pin mode */

	GPIOG->MODER |= GPIO_MODER_MODE15_0; /* set pin to output mode */


	/* Configure Timer2  */
	RCC->APB1ENR |=RCC_APB1ENR_TIM4EN; // enable the timer 2 clock

	TIM4->PSC = 1000-1; //divide the system clock by 1000
	TIM4->ARR = 1562 - 1; // THIS is use to divide the remaining value of the system clock

	TIM4->CR1 |= TIM_CR1_CEN; // This enable tim3

	// Enable Timer 2 Update Interrupt
	TIM4->DIER |= TIM_DIER_UIE;

	NVIC_SetPriority(TIM4_IRQn, PRIO_TIM4); // Highest priority
	NVIC_EnableIRQ(TIM4_IRQn);


	while (1)
	{
	}
}


void TIM3_IRQHandler(void)
{

	GPIOG->ODR |= 1 << 13;
	delayMs(500);
	GPIOG->ODR &= ~(1 << 13);

	TIM3->SR &= ~TIM_SR_UIF; // Clear the UIF register
}



void TIM4_IRQHandler(void)
{

	GPIOG->ODR |= 1 << 14;
	GPIOG->ODR |= 1 << 15;
	delayMs(25);
	GPIOG->ODR &= ~(1 << 14);
	GPIOG->ODR &= ~(1 << 15);

	TIM4->SR &= ~TIM_SR_UIF; // Clear the UIF register

}


void delayMs(int n)
{
	int i;
	for (; n > 0; n--)
		for (i = 0; i < 3195; i++);
}