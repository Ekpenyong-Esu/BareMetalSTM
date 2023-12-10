#include "stm32f429xx.h"

void delayMs(int n);

int main(void) {
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; /* enable GPIOA clock */
    GPIOG->MODER &= ~GPIO_MODER_MODE13;  /* clear pin mode */
    GPIOG->MODER |= GPIO_MODER_MODE13_0;  /* set pin to output mode */

    while(1) {
        GPIOG->BSRR = GPIO_BSRR_BS_13;    /* turn on output */
        delayMs(500);
        GPIOG->BSRR = GPIO_BSRR_BR_13;    /* turn off output */
        delayMs(500);
    }
}

/* 16 MHz SYSCLK */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}
