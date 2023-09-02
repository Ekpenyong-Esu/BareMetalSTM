#include "stm32f4xx.h"


int main(void) {

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; /* enable GPIOG clock */

	GPIOG->MODER &= ~GPIO_MODER_MODE13; /* clear pin mode */

	GPIOG->MODER |= GPIO_MODER_MODE13_0; /* set pin to output mode *//*Configure SysTick */

	SysTick->LOAD = 3200000 - 1; /* reload with number of clocks persecond */

	SysTick->VAL = 0;  // clear the current register value

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk; /* enable it, no interrupt, use system clock */

	while (1) {
		if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) { /* if COUNT flag is set */
			GPIOG->ODR ^= (1 << 13); /* toggle green LED */
		}
	}
}
