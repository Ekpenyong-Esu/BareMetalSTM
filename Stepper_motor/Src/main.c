/* p10_2.c Stepper motor control
 *
 * This program controls a unipolar stepper motor using PTA7, 6, 5, 4.
 *
 * This program was tested with Keil uVision v5.23 with DFP v2.11.0.
 */


#include "stm32f429xx.h"

void delayMs(int n);

int main(void) {
    const uint16_t steps[] = {0x0080, 0x0020, 0x0040, 0x00C0};
    int i;

    /* GPIOA for motor control */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;    /* enable GPIOA clock */
    GPIOA->MODER &= ~0x0000FF00;            /* clear pin mode */
    GPIOA->MODER |= 0x00005500;             /* set pins to output mode */

    for (;;) {
        GPIOA->ODR = steps[i++ & 3];        /* set the output pattern */
        delayMs(100);
    }
}

/* 16 MHz SYSCLK */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}
