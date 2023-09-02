#include "stm32f4xx.h"

void delayMs(int n);

int main(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; /* enable GPIOG  clock */

	GPIOG->MODER &= ~GPIO_MODER_MODE14; /* clear pin mode */

	GPIOG->MODER |= GPIO_MODER_MODE14_0; /* set pin to output mode */

	while (1) {
		delayMs(100); 				/* delay 100 ms */
		GPIOG->ODR ^=  (1 << 14); 	/*toggle red LED */
	}
}
void delayMs(int n) {
	int i;
	/* Configure SysTick */
	SysTick->LOAD = 16000; /* reload with number of clocks permillisecond */

	SysTick->VAL = 0; /* clear current value register */

	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk; /* Enable the timer */

	for (i = 0; i < n; i++) {
		while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk ) == 0) /* wait until the COUNTFLAG is set */
		{
		}
	}
	SysTick->CTRL = 0; /* Stop the timer (Enable = 0) */
}
