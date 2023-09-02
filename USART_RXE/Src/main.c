/* p4_1.c Send a string "YES" to USART2
 *
 * USART2 is connected to the ST-Link virtual COM port. * Use Tera
 Term to see the message "YES" on a PC.
 *
 * By default, the clock is running at 16 MHz.
 * The UART2 is configured for 9600 Baud.
 * PA2 - USART2 TX (AF7)
 *
 * This program was tested with Keil uVision v5.24a with DFP v2.11.0
 */
#include "stm32F4xx.h"

void USART1_Init(void);
char USART1_read(void);
void delayMs(int);
void LED_blink(char value);
/*------------------------------------------------------------------
 ---------MAIN function
 *-------------------------------------------------------------------*/
int main(void) {

	char c;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;  /* enable GPIOG clock */

	GPIOG->MODER &= ~(GPIO_MODER_MODER13);  /* clear pin mode */
	GPIOG->MODER |= (GPIO_MODER_MODER13_0);  /* set pin to output mode */

	USART1_Init(); /* initialize USART1 */

	while (1) { /* Loop forever */
		c = USART1_read();  /* wait for a character received */
		LED_blink(c);       /* blink the LED */

	}
}


/*------------------------------------------------------------------
---------Initialize UART pins, Baudrate
*-------------------------------------------------------------------
---------*/

//void USART2_init(void) {
//	RCC->AHB1ENR |= 1; /* Enable GPIOA clock */
//	RCC->APB1ENR |= 0x20000; /* Enable USART2 clock */
//
//	/* Configure PA2 for USART2_TX */
//	GPIOA->AFR[0] &= ~0x0F00;
//	GPIOA->AFR[0] |= 0x0700; /* alt7 for USART2 */
//	GPIOA->MODER &= ~0x0030;
//	GPIOA->MODER |= 0x0020; /* enable alternate function for PA2 */
//	USART2->BRR = 0x0683; /* 9600 baud @ 16 MHz */
//	USART2->CR1 = 0x0008;
//	/* enable Tx, 8-bit data */
//	USART2->CR2 = 0x0000; /* 1 stop bit */
//	USART2->CR3 = 0x0000; /* no flow control */
//	USART2->CR1 |= 0x2000; /* enable USART2 */
//}


void USART1_Init(void)
{
    // Enable peripheral clocks
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    //GPIOA->AFR[1] &= ~(0xF << (4 * 1)); // Clear AF7 for USART1 on PA9 (TX)
    GPIOA->AFR[1] &= ~(0xF << (4 * 2)); // Clear AF7 for USART1 on PA10 (RX)

    //GPIOA->AFR[1] |= 0x7 << (4 * 1); // AF7 for USART1 on PA9 (TX)
    GPIOA->AFR[1] |= 0x7 << (4 * 2); // AF7 for USART1 on PA10 (RX)

    // Configure PA9 (USART1_TX) as alternate function
    GPIOA->MODER &= ~(GPIO_MODER_MODER10);
    GPIOA->MODER |= GPIO_MODER_MODER10_1;

    USART1->CR2 = 0x0000; /* 1 stop bit */
    USART1->CR3 = 0x0000; /* no flow control */

    // Configure USART1: 9600 baud, 8 data bits, 1 stop bit, no parity, transmit enable, USART enable
    USART1->BRR = 16000000 / 115200; // baud rate
    USART1->CR1 = USART_CR1_RE | USART_CR1_UE;
}


void delayMs(int n) {
	int i;
	for (; n > 0; n--)
		for (i = 0; i < 2000; i++);
}


/* Write a character to USART2 */
void USART1_write(int ch) {
	while (!(USART1->SR & 0x0080)) {} // wait until Tx buffer empty
	USART1->DR = (ch & 0xFF);
}


char USART1_read(void){

	while (!(USART1->SR & USART_SR_RXNE));
	return USART1->DR;
}


void LED_blink(char value) {

	value %= 16; /* cap the max count at 15 */

	for (; value > 0; value--) {
		GPIOG->BSRR |= GPIO_BSRR_BS_13; /* turn on LED */
		delayMs(200);
		GPIOG->BSRR |= GPIO_BSRR_BR_13; /* turn off LED */
		delayMs(200);
	}
	delayMs(800);
}


