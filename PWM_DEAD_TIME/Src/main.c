#include "stm32f4xx.h"

int main(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  /* enable GPIOA clock */

	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;  // Clear the alternate funtion fir pin 5
	GPIOA->AFR[0] |= (3UL << GPIO_AFRL_AFSEL5_Pos); // AF1 (Alternate Function 1) = 001

	GPIOA->MODER &= ~GPIO_MODER_MODER5;  /* Clear mode bits for PA5 */
	GPIOA->MODER |= GPIO_MODER_MODER5_1; /* Set PA5 to alternate function mode */

	//========================================================================================================
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;  /* enable GPIOC clock */

	GPIOC->AFR[0] &= ~GPIO_AFRL_AFSEL6;  // Clear the alternate funtion fir pin 6
	GPIOC->AFR[0] |= (3UL << GPIO_AFRL_AFSEL6_Pos); // AF1 (Alternate Function 1) = 001

	GPIOC->MODER &= ~GPIO_MODER_MODER6;  /* Clear mode bits for PC6 */
	GPIOC->MODER |= GPIO_MODER_MODER6_1; /* Set PA5 to alternate function mode */

	//=========================================================================================================

	/* Setup TIM2 */
	RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;  /* enable TIM8 clock */
	TIM8->PSC = 16;                  /* divided by 16 */
	TIM8->ARR = 1000 - 1;               /* divided by 1000000/1000 to run at 1KHz */
	TIM8->CNT = 0;
	TIM8->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; /* PWM mode 1 */
	TIM8->CCER = TIM_CCER_CC1E | TIM_CCER_CC1NE;          /* enable PWM Ch1 */
	TIM8->CCR1 = 500;               /* pulse width 50 percent of the period */
	TIM8->BDTR |= TIM_BDTR_MOE | 0xFF << 0; /* enable output, set max dead time */
	TIM8->CR1 = TIM_CR1_CEN | TIM_CR1_CKD_1 | TIM_CR1_CMS_1 | TIM_CR1_CMS_0; /* slow DT clock, center-aligned*/

	while(1) {
		/* Your application code goes here */
	}
}