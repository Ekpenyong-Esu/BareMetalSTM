#include "stm32f4xx.h"

void delayMs(int n);

int main(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; /* enable GPIOG clock */

	GPIOG->MODER &= ~GPIO_MODER_MODE13; /* clear pin mode */

	GPIOG->MODER |= GPIO_MODER_MODE13_0; /* set pin to output mode */

	/* Configure SysTick */
	SysTick->LOAD = 0xFFFFFF; /* reload with max value */

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;  /* enable it, no interrupt, use system clock */

	NVIC_SetPriority(USART1_IRQn, 0); // Highest priority
	NVIC_EnableIRQ(USART1_IRQn);

	while (1) {

//		/* take bit 23 of SysTick current value and shift it to bit 13 then
//		 write it to PortG */
//		GPIOG->ODR = (SysTick->VAL >> (23 - 13)) & (1 << 13);
	}
}


void SysTick_Handler(void)
{
	/* take bit 23 of SysTick current value and shift it to bit 13 then write it to PortG */

	GPIOG->ODR = (SysTick->VAL >> (23 - 13)) & (1 << 13);
}
