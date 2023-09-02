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
void USART1_write(int c);
void delayMs(int);
/*------------------------------------------------------------------
 ---------MAIN function
 *-------------------------------------------------------------------*/
int main(void) {

	USART1_Init(); /* initialize USART2 */

	while (1) { /* Loop forever */
		USART1_write('Y');
		USART1_write('e');
		USART1_write('s');
		USART1_write('\n');
		USART1_write('\r');
		delayMs(10); /* leave a gap between messages */
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

	// Enable UART1 and GPIOA clocks
	  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;

	  // Configure PA9 and PA10 pins for UART1 TX and RX
	  GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9;
	  GPIOA->AFR[1] |= 7 << GPIO_AFRH_AFSEL9_Pos; // ALT7 for UART1_TX

	  GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL10;
	  GPIOA->AFR[1] |= 7 << GPIO_AFRH_AFSEL10_Pos; // ALT7 for UART1_RX

	  GPIOA->MODER &= ~(GPIO_MODER_MODER9 | GPIO_MODER_MODER10);
	  GPIOA->MODER |= GPIO_MODER_MODER9_1 | GPIO_MODER_MODER10_1; // Enable alternate function

	  // Configure UART1
	  USART1->BRR = 16000000/9600;//0x683; // 9600 baud @ 16MHz
	  USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE; // Enable TX, RX, UART
	  USART1->CR2 = 0x0000; //USART_CR2_STOP_1 ; // 1 stop bit
	  USART1->CR3 = 0x0000; // No flow control
}

void delayMs(int n) {
	int i;
	for (; n > 0; n--)
		for (i = 0; i < 2000; i++);
}


/* Write a character to USART2 */
void USART1_write(int ch) {
	while (!(USART1->SR & 0x0080)) {} // wait until Tx buffer empty
	USART1->DR = ch;
}



