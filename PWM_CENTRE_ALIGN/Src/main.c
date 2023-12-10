
#include "stm32f4xx.h"

void delayMs(int n);

void TIM8_center_aligned(void)
{

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;  /* enable GPIOC clock */

	GPIOC->AFR[0] &= ~GPIO_AFRL_AFSEL6;  // Clear the alternate funtion fir pin 6
	GPIOC->AFR[0] |= (3UL << GPIO_AFRL_AFSEL6_Pos); // AF1 (Alternate Function 3) = 011

	GPIOC->MODER &= ~GPIO_MODER_MODER6;  /* Clear mode bits for PC6 */
	GPIOC->MODER |= GPIO_MODER_MODER6_1; /* Set PA5 to alternate function mode */

	/* Setup TIM8 */
	RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;  /* enable TIM8 clock */
	TIM8->PSC = 16-1;                  /* divided by 16000 */
	TIM8->ARR = 1000;               /* divided by 1000/1000 to run at 1KHz */
	TIM8->CNT = 0;
	TIM8->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; /* PWM mode 1 */
	TIM8->CCER = TIM_CCER_CC1E;           /* enable PWM Ch1 */
	TIM8->CCR1 = 300;               /* pulse width 30 of the period */
	TIM8->BDTR = TIM_BDTR_MOE;
	TIM8->CR1 = TIM_CR1_CEN | TIM_CR1_CMS_1;   /* center-aligned, enable timer and auto-reload preload enable */
}

void TIM2_edge_aligned(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  /* enable GPIOA clock */

	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;  // Clear the alternate funtion fir pin 5
	GPIOA->AFR[0] |= (1UL << GPIO_AFRL_AFSEL5_Pos); // AF1 (Alternate Function 1) = 001

	GPIOA->MODER &= ~GPIO_MODER_MODER5;  /* Clear mode bits for PA5 */
	GPIOA->MODER |= GPIO_MODER_MODER5_1; /* Set PA5 to alternate function mode */


	/* Setup TIM2 */
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;  /* enable TIM2 clock */
	TIM2->PSC = 16-1;                  /* divided by 16000 */
	TIM2->ARR = 1000 - 1;               /* divided by 1000/1000 to run at 1KHz */
	TIM2->CNT = 0;
	TIM2->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; /* PWM mode 1 */
	TIM2->CCER = TIM_CCER_CC1E;           /* enable PWM Ch1 */
	TIM2->CCR1 = 300;               /* pulse width 30 of the period */
	TIM2->CR1 = TIM_CR1_CEN;             /* enable timer */
}


/* 16 MHz SYSCLK */
void delayMs(int n) {
	int i;
	for (; n > 0; n--)
		for (i = 0; i < 3195; i++) ;
}

int main(void) {
	TIM8_center_aligned();
	TIM2_edge_aligned();

	while(1)
	{
	}
}
