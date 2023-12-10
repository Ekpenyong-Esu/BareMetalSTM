
#include "stm32f4xx.h"

void delayMs(int n);

int main(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  /* enable GPIOA clock */

	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;  // Clear the alternate funtion fir pin 5
    GPIOA->AFR[0] |= (3UL << GPIO_AFRL_AFSEL5_Pos); // AF1 (Alternate Function 1) = 001

    GPIOA->MODER &= ~GPIO_MODER_MODER5;  /* Clear mode bits for PA5 */
    GPIOA->MODER |= GPIO_MODER_MODER5_1; /* Set PA5 to alternate function mode */

    /* Setup TIM2 */
    RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;  /* enable TIM8 clock */
    TIM8->PSC = 16;                  /* divided by 16 */
    TIM8->ARR = 1000 - 1;               /* divided by 1000000/1000 to run at 1KHz */
    TIM8->CNT = 0;
    TIM8->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; /* PWM mode 1 */
    TIM8->CCER = TIM_CCER_CC1NE;          /* enable PWM Ch1 */
    TIM8->CCR1 = 90;               /* pulse width 1/3 of the period */
    TIM8->BDTR = TIM_BDTR_MOE;
    TIM8->CR1 = TIM_CR1_CEN;             /* enable timer */

    while(1)
    {
        TIM8->CCR1 = TIM8->CCR1 * 110 / 100;
        if (TIM8->CCR1 > 1000)
        {
        	TIM8->CCR1 = 90;
        }
        delayMs(50);
    }
}


/* 16 MHz SYSCLK */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}

