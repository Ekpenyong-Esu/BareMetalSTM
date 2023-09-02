#include "stm32f4xx.h"

void delayMs(int n);

int main(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; /* enable GPIOG clock */

	GPIOA->MODER &= ~GPIO_MODER_MODE5; /* clear pin 5 of portA mode */

	GPIOA->MODER |= GPIO_MODER_MODE5_1; /* set pin to alternate mode */


	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;  // Clear the alternate funtion first pin 5
	GPIOA->AFR[0] |= (1U << GPIO_AFRL_AFSEL5_Pos); // AF1 (Alternate Function 1) = 001


	/* Configure Timer2  */
	RCC->APB1ENR |=RCC_APB1ENR_TIM2EN; // enable the timer 2 clock


	// Note that system clock is 16'000'000 MHz
	TIM2->PSC = 1600 -1;   //divide the system clock by 1600
	TIM2->ARR = 10000 - 1; // THIS is use to divide the remaining value of the system clock and th clock run at
						   // 1Hz per 1 second

	TIM2->CCMR1 |= 0x3UL << 4;  // Toggle Mode on CH1 when match with TIM2->CCR1 value
	TIM2->CCR1 = 0;             // This is for the capture compare value when match

	TIM2->CCER |= TIM_CCER_CC1E;  // Enable the output for Channel 1 of the chanel compare register
	TIM2->CNT = 0; 				// THis is use to clear the timer

	TIM2->CR1 |= TIM_CR1_CEN; // This enable tim2 in the control register


	while (1) {
//		while ( !(TIM2->SR & TIM_SR_UIF)) {} // wait untill the UIF Flag register in status register is set
//
//		TIM2->SR &= ~TIM_SR_UIF; // Clear the flag of the update interrupt register in the status register
//
//		GPIOG->ODR ^= 1 << 13;
	}
}










