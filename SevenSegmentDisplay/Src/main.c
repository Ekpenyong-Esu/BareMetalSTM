/* p2_4.c Display number 75 on a 2-digit 7-segment common cathode LED.
 *
 * The segments are driven by Port C0-C6.
 * The digit selects are driven by PB0 and PB1.
 *
 * This program was tested with Keil uVision v5.24a with DFP v2.11.0
 */

#include "stm32f4xx.h"

void delayMs(int n);

int main(void) {
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;    /* enable GPIOB clock */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;    /* enable GPIOC clock */

    GPIOC->MODER &= ~(GPIO_MODER_MODE0_Msk | GPIO_MODER_MODE1_Msk | GPIO_MODER_MODE2_Msk | GPIO_MODER_MODE3_Msk
    		|~GPIO_MODER_MODE4_Msk | GPIO_MODER_MODE5_Msk | GPIO_MODER_MODE6_Msk | GPIO_MODER_MODE7_Msk);// ~0x0000FFFF;    /* clear pin mode */


    GPIOC->MODER |= (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0
    		| GPIO_MODER_MODE4_0 | GPIO_MODER_MODE5_0 |GPIO_MODER_MODE6_0 | GPIO_MODER_MODE7_0);  //0x00005555;    /* set pins to output mode */


    GPIOB->MODER &= ~(GPIO_MODER_MODE0_Msk | GPIO_MODER_MODE1_Msk); //~0x0000000F;    /* clear pin mode */
    GPIOB->MODER |= (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 ); //0x00000005;    /* set pins to output mode */

    for(;;)
    {
        GPIOC->ODR = 0x0007;            /* display tens digit */
        GPIOB->BSRR = GPIO_BSRR_BR_0;    //0x00010000;       /* deselect ones digit */ off the pin zero light
        GPIOB->BSRR = GPIO_BSRR_BS_1;  //0x00000002;       /* select tens digit */on the pin one light
        delayMs(8);
        GPIOC->ODR = 0x006D;            /* display ones digit */
        GPIOB->BSRR = GPIO_BSRR_BR_1; //0x00020000;       /* deselect tens digit */ off the one pin
        GPIOB->BSRR = GPIO_BSRR_BS_0; //0x00000001;       /* select ones digit */  on the zero pin
        delayMs(8);
    }
}
